#ifndef __GROUND_HPP__
#define __GROUND_HPP__

#include "screen.hpp"
#include "eventhandler.hpp"
/*
x0,y0：地图的显示位置(左上角)
w,h：地图的宽度和高度
color：地图的填充色(没实际用处，后面都会被小格子覆盖)
bw：地图的边框宽度
bcolor：地图的边框颜色
s：屏幕对象
lines：小格子的行数
columns：小格子的列数
item_color：小格子的填充颜色
itme_bw：小格子的边框宽度
item_bcolor：小格子边框颜色
*/

class Ground:public Rectangle
{
public:
    Ground(int x0,int y0,int w,int h,uint color,int bw,uint bcolor,Screen *s,int lines,int columns,
        uint item_color,int itme_bw,uint item_bcolor);
    //画草地
    void draw();
    //画小格子
    void draw_item(int i,int j);
    void draw_item(int i,int j,uint color);
    void draw_item(int i,int j,Color color);
    void draw_snaketou(int i,int j,MovDirection _dirction);
    void draw_snakeshen(int i, int j);
    void draw_snakewb(int i, int j);
    //获取格子的行数
    int get_lines() const;
    //获取格子的列数
    int get_columns() const;
    uint get_item_fill_color() const;

private:
    int _lines; //小格子的行数
    int _columns;   //小格子的列数
    Size _item_size; //小格子的大小
    int _item_border_width;    //小格子边框的宽度
    Color _item_border_color;   //小格子边框的颜色
    Color _item_fill_color; //小格子内部的填充色
    Screen *_s; //屏幕指针
};

#endif