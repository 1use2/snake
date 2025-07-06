#include "screen.hpp"

#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <iostream>
using std::cout;
using std::endl;

//初始化屏幕对象
Screen Screen::screen{};

//构造函数
Screen::Screen(string path,int w,int h,uchar depth):
    _size(w,h),_depth(depth),_dev_path(path)
{
    //打开屏幕设备文件
    fd_lcd = open(_dev_path.c_str(),O_RDWR);
    if (-1 == fd_lcd)
    {
        //打开屏幕设备文件失败
        throw std::runtime_error(string{"open lcd error:"} + strerror(errno));
    }
    //建立映射关系
    _p_map = static_cast<uint*>(mmap(NULL,BUF_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,fd_lcd,0));
    if (MAP_FAILED == _p_map)
    {
        //屏幕映射失败
        throw std::runtime_error(string{"lcd mmap eroor:"} + strerror(errno));
    }
    cout << "lcd init success!" << endl;
}

//析构函数
Screen::~Screen()
{
    //解除映射
    munmap(_p_map,BUF_SIZE);
    //关闭屏幕
    close(fd_lcd);
    cout << "touch closed!" << endl;
}

//提供一个能够获取这个唯一的实例的接口
Screen& Screen::getScreen()
{
    return screen;
}

//提供一些访问成员的方法
int Screen::getW() const
{
    return _size.getW();
}

int Screen::getH() const
{
    return _size.getH();
}

uchar Screen::getDepth() const
{
    return _depth;
}

string Screen::getPath() const
{
    return _dev_path;
}

void Screen::drawPoint(int x,int y,uint color)
{
    if (x >= 0 and x < LCD_W and y >= 0 and y < LCD_H)
    {
        //是屏幕上面的点
        *(_p_map+x+y*LCD_W) = color;
    }
}

void Screen::drawPoint(int x,int y,Color color)
{
    if (x >= 0 and x < LCD_W and y >= 0 and y < LCD_H)
    {
        //是屏幕上面的点
        *(_p_map+x+y*LCD_W) = color.getColor();
    }
}

void Screen::drawPoint(Point point,uint color)
{
    int x = point.getX();
    int y = point.getY();
    if (x >= 0 and x < LCD_W and y >= 0 and y < LCD_H)
    {
        //是屏幕上面的点
        *(_p_map+x+y*LCD_W) = color;
    }
}

void Screen::drawPoint(Point point,Color color)
{
   int x = point.getX();
    int y = point.getY();
    if (x >= 0 and x < LCD_W and y >= 0 and y < LCD_H)
    {
        //是屏幕上面的点
        *(_p_map+x+y*LCD_W) = color.getColor();
    } 
}

void Screen::drawRect(Rectangle &rect)
{
    //画矩形的主体
    int x0 = rect._point.getX();
    int y0 = rect._point.getY();
    int w = rect._size.getW();
    int h = rect._size.getH();
    for (int y = y0; y < y0+h; y++)
    {
        for (int x = x0; x < x0+w; x++)
        {
            drawPoint(x,y,rect._full_color.getColor());
        }
    }
    // //画矩形的边框
    int bw = rect._border_width;
    for (int y = y0; y < y0+h; y++)
    {
        for (int x = x0; x < x0+w; x++)
        {
            if (y < y0+bw or y >= y0+h-bw or x < x0+bw or x >= x0+w-bw)
            {
                drawPoint(x,y,rect._border_color.getColor());
            }
        }
    }
}

void Screen::brushBg(uint color)
{
    for (int y = 0; y < LCD_H; y++)
    {
        for (int x = 0; x < LCD_W; x++)
        {
            drawPoint(x,y,color);
        }
    }
}

void Screen::brushBg(const Color& c)
{
    for (int y = 0; y < LCD_H; y++)
    {
        for (int x = 0; x < LCD_W; x++)
        {
            drawPoint(x,y,c.getColor());
        }
    }
}
void Screen::lcdDrawBMP(int x0,int y0,const char* pic_path)
{
    //打开图片文件
    int fd_pic = open(pic_path,O_RDONLY);
    if (-1 == fd_pic)
    {
        perror("open pic error");
        return;
    }
    
    //读取图片属性(魔数、像素数组偏移地址、宽度、高度、色深)
    short MS,depth; //魔数，色深
    int offset,width,height;    //像素数组偏移地址，宽度，高度
    read(fd_pic,&MS,2);
    //判断文件是不是bmp图片，如果不是bmp图片就结束
    if (0x4d42 != MS)
    {
        printf("this pic is not bmp!\n");
        close(fd_pic);
        return;
    }
    lseek(fd_pic,0x0A,SEEK_SET);
    read(fd_pic,&offset,4);
    lseek(fd_pic,0x12,SEEK_SET);
    read(fd_pic,&width,4);
    read(fd_pic,&height,4);
    lseek(fd_pic,2,SEEK_CUR);
    read(fd_pic,&depth,2);
    //打印读到的数据
    //printf("MS=0x%x,offset=0x%x,width=%d,height=%d,depth=%d\n",MS,offset,width,height,depth);

    //读取图片的像素数组数据
    int full_bytes = (4-(width*depth/8)%4)%4;  //每一行的填充字节数=(4-这一行多的字节数)%4
    int total_full = full_bytes*abs(height);  //总的填充字节数=每一行的填充字节数*行数
    int buf_size = abs(height)*width*depth/8+total_full;    //像素数组大小=像素点的个数x每个像素点颜色值的字节数+填充的总字节数
    unsigned char color_buf[buf_size];   //颜色数组，用来保存所有像素点的颜色值
    lseek(fd_pic,offset,SEEK_SET);
    read(fd_pic,color_buf,buf_size);
    //一个一个像素点地显示图片(将图片上的像素点一个个地画到屏幕上去)
    unsigned char *p = color_buf;   //为了方便访问数据，通过指针p来访问
    unsigned char a,r,g,b;  //4个颜色分量
    for (int y = 0; y < abs(height); y++)
    {
        for (int x = 0; x < width; x++)
        {
            //获取一个像素点的4个颜色分量
            b = *p++;
            g = *p++;
            r = *p++;
            if (depth == 32)
            {
                //是32位的bmp图片，有4个颜色分量，可以直接获取a的值
                a = *p++;
            }
            else if (depth == 24)
            {
                //是24位的bmp图片，有3个颜色分量，a需要手动赋值
                a = 0;
            }
            //将获取到的4个颜色分量按照argb的顺序重新排列组合成一个新的颜色值
            // a:      0010 0100
            // r:      1101 1010
            // g:      0101 1010
            // b:      1001 0110
            // 合成：
            //         0010 0100 0000 0000 0000 0000 0000 0000
            //                   1101 1010 0000 0000 0000 0000
            //                             0101 1010 0000 0000
            //                                       1001 0110
            unsigned int color = a << 24 | r << 16 | g << 8 | b;
            //显示颜色
            if (height > 0)
            {
                //图片在保存是是从下到上从左到右保存的
                //所以显示也要从下到上显示
                
                drawPoint(x+x0,height-1-y+y0,color);
            }
            else
            {
                
                drawPoint(x+x0,y+y0,color);
            }
        }
        //跳过这一行后面的填充字节
        p += full_bytes;
    }
    
    //关闭图片文件
    close(fd_pic);
}