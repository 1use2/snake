#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <stdlib.h>

int fd_lcd; //帧缓冲设备文件的文件描述符
unsigned int *pmap; //映射区首地址
int size_fb0 = 800*480*4;   //帧缓冲设备文件大小=像素点的个数x每个像素点颜色值的字节数

//外部声明
 char words[5][93][12];   //文字取模结果
 int word_width;  //文字宽度
 int word_height;  //文字高度
 char numbers[10][29][16/8];  //数组取模结果
 int num_width; //数字宽度
 int num_height;    //数字高度
 char small_letter[26][29][16/8]; //小写字母取模结果
 char capital_letter[26][29][16/8];   //大写字母取模结果
 int letter_width;  //字母宽度
int letter_height; //字母高度
/******************************************************
 * 功能：刷背景色
 * 返回值：无
 * 参数：
 *      color：背景色的颜色值
******************************************************/
void lcdBrushBG(unsigned int color)
{
    for (int y = 0; y < 480; y++)
    {
        for (int x = 0; x < 800; x++)
        {
            *(pmap+y*800+x) = color;
        } 
    }
}

/******************************************************
 * 功能：在指定的位置画某个颜色的像素点
 * 返回值：无
 * 参数：
 *      x,y：像素点的坐标
 *      color：像素点的颜色值
******************************************************/
void lcdDrawPoin(int x,int y,unsigned int color)
{
    if(x >= 0 && x < 800 && y >= 0 && y < 480)
    {
        //这个坐标是屏幕范围内的点
        *(pmap+y*800+x) = color;
    }
}

/******************************************************
 * 功能：画矩形(实心)
 * 返回值：无
 * 参数：
 *      x0,y0：矩形左上顶点的坐标
 *      w,h：矩形的宽度高度
 *      color：矩形的填充颜色值
******************************************************/
void lcdDrawRect(int x0,int y0,int w,int h,unsigned int color)
{
    for (int y = y0; y < y0+h; y++)
    {
        for (int x = x0; x < x0+w; x++)
        {
            lcdDrawPoin(x,y,color);
        }
    }
}

/******************************************************
 * 功能：画圆形(实心)
 * 返回值：无
 * 参数：
 *      x0,y0：圆心的坐标
 *      r：圆的半径
 *      color：圆形的填充颜色值
******************************************************/
void lcdDrawCircle(int x0,int y0,int r,unsigned int color)
{
    for (int y = y0-r; y < y0+r; y++)
    {
        for (int x = x0-r; x < x0+r; x++)
        {
            if ((y-y0)*(y-y0)+(x-x0)*(x-x0) <= r*r)
            {
                //该像素点是圆内及圆上的点
                lcdDrawPoin(x,y,color);
            }
        }
    }
}

/******************************************************
 * 功能：在指定的位置显示一个文字
 * 返回值：无
 * 参数：
 *      x0,y0：文字显示的位置(左上角坐标)
 *      pword：指向要取模的文字取模结果的地址
 *      w_w,w_h：要取模的文字的宽度和高度
 *      color：文字的颜色
******************************************************/
void lcdDrawWord(int x0,int y0,char *pword,int w_w,int w_h,unsigned int color)
{
    //遍历取模结果，一个字节一个字节地拿出来分析
    for (int h = 0; h < w_h; h++)    //一共有这么多行的数据
    {
        for (int w = 0; w < w_w/8; w++) //每一行有WORD_W/8个字节的数据
        {
            for (int i = 7; i >= 0; i--) //每一个字节有8个像素点状态，循环8次
            {
                if(*(pword+h*w_w/8+w)>>i & 0x01)
                {
                    //该位的值为1，则这个像素点是文字上的点，显示
                    int x = (7-i+w*8+x0)%800;
                    int y = (h+y0)%480;
                    lcdDrawPoin(x,y,color);
                }
            }
        }  
    }
}

/******************************************************
 * 功能：在指定的位置显示字符串
 * 返回值：无
 * 参数：
 *      x0,y0：最左边的文字左上角的坐标
 *      str：要显示的字符串的内容
 *      color：文字的颜色
******************************************************/
void lcdDrawWords(int x0,int y0,char *str,unsigned int color)
{
    //判断是字母、数字还是汉字
    int index = 0;  //字符串的下标
    int pre_w = 0;  //保存前面所有字符的宽度
    while (str[index])
    {
        if(str[index] >= '0' && str[index] <= '9')
        {
            //是数字，显示数字
            lcdDrawWord(x0+pre_w,y0,(char*)numbers[str[index]-'0'],num_width,num_height,color);
            index++;
            pre_w += num_width;
        }
        else if(str[index] >= 'A' && str[index] <= 'Z')
        {
            //大写字母
            lcdDrawWord(x0+pre_w,y0,(char*)capital_letter[str[index]-'A'],letter_width,letter_height,color);
            index++;
            pre_w += letter_width;
        }
        else if (str[index] >= 'a' && str[index] <= 'z')
        {
            //小写字母
            lcdDrawWord(x0+pre_w,y0,(char*)small_letter[str[index]-'a'],letter_width,letter_height,color);
            index++;
            pre_w += letter_width;
        }
        else if (strncmp(str+index,"鸡",3)==0)
        {
            //显示 鸡
            lcdDrawWord(x0+pre_w,y0,(char*)words[0],word_width,word_height,color);
            index += 3;
            pre_w += word_width;
        }
        else if (strncmp(str+index,"你",3)==0)
        {
            //显示 你
            lcdDrawWord(x0+pre_w,y0,(char*)words[1],word_width,word_height,color);
            index += 3;
            pre_w += word_width;
        }
        else if (strncmp(str+index,"太",3)==0)
        {
            //显示 太
            lcdDrawWord(x0+pre_w,y0,(char*)words[2],word_width,word_height,color);
            index += 3;
            pre_w += word_width;
        }
        else if (strncmp(str+index,"美",3)==0)
        {
            //显示 美
            lcdDrawWord(x0+pre_w,y0,(char*)words[3],word_width,word_height,color);
            index += 3;
            pre_w += word_width;
        }
        else if (strncmp(str+index,"！",3)==0)
        {
            //显示 ！
            lcdDrawWord(x0+pre_w,y0,(char*)words[4],word_width,word_height,color);
            index += 3;
            pre_w += word_width;
        }
        else
        {
            //其他字符，不显示
            printf("无效字符！\n");
            index++;
        }
    }
}

/******************************************************
 * 功能：在指定的位置显示数字
 * 返回值：无
 * 参数：
 *      x0,y0：最高位数组左上角的坐标
 *      num：要显示数字
 *      color：数字的颜色
******************************************************/
void lcdDrawNumbers(int x0,int y0,int num,unsigned int color)
{
    //提取数字各位的值
    char buf[20] = {0}; //用来保存数字各位的值
    int wei = 0;    //用来记录数字的位数
    do
    {
       buf[wei++] = num%10;
       num /= 10;
    } while (num);
    //显示数字
    for (int i = wei-1; i >= 0; i--)
    {
        lcdDrawWord(x0+(wei-1-i)*num_width,y0,(char*)numbers[buf[i]],num_width,num_height,color);
    }
}

/******************************************************
 * 功能：在指定的位置显示BMP图片
 * 返回值：无
 * 参数：
 *      x0,y0：图片显示的位置(左上顶点坐标)
 *      pic_path：要显示的图片的路径
******************************************************/
void lcdDrawBMP(int x0,int y0,const char* pic_path)
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
                lcdDrawPoin(x+x0,height-1-y+y0,color);
            }
            else
            {
                lcdDrawPoin(x+x0,y+y0,color);
            }
        }
        //跳过这一行后面的填充字节
        p += full_bytes;
    }
    
    //关闭图片文件
    close(fd_pic);
}
