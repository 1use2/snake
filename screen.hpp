#ifndef __SCREEN_HPP__
#define __SCREEN_HPP__

#include "rectangle.hpp"
#include "bmp.hpp"
#include <string>
using std::string;

#define LCD_W   800
#define LCD_H   480
#define LCD_DEPTH 32
#define BUF_SIZE    LCD_W*LCD_H*LCD_DEPTH/8

class Rectangle;

//单例模式
class Screen
{
public:
    //析构函数
    ~Screen();
    //提供一个能够获取这个唯一的实例的接口
    static Screen& getScreen();
    //提供一些访问成员的方法
    int getW() const;
    int getH() const;
    uchar getDepth() const;
    string getPath() const;
    void drawPoint(int x,int y,uint color);
    void drawPoint(int x,int y,Color color);
    void drawPoint(Point point,uint color);
    void drawPoint(Point point,Color color);
    void drawRect(Rectangle &rect);
    void brushBg(uint color);
    void brushBg(const Color& c);
    void lcdDrawBMP(int x0,int y0,const char* pic_path);
    string _dev_path;
    int fd_lcd;
    uint* _p_map;

private:
    //为了防止用户创建对象,将构造函数设置为私有的
    Screen(string path = "/dev/fb0",int w = LCD_W,int h = LCD_H,uchar depth = LCD_DEPTH);
    //删除该类的拷贝构造函数
    Screen(const Screen&) = delete;
    //使用静态成员对象作为这个类的唯一实例(只要包含这个类,就能实例化一个对象)
    //静态成员在使用前必须进行初始化,且必须是在类外进行初始化
    static Screen screen;
    Size _size;
    uchar _depth;
     
    
};

#endif