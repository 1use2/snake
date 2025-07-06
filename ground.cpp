#include "ground.hpp"

Ground::Ground(int x0,int y0,int w,int h,uint color,int bw,uint bcolor,Screen *s,int lines,int columns,
    uint item_color,int itme_bw,uint item_bcolor):Rectangle(x0,y0,w,h,color,bw,bcolor),
    _s(s),_lines(lines),_columns(columns),_item_fill_color(item_color),_item_border_width(itme_bw),
    _item_border_color(item_bcolor),_item_size(getW()/_columns,getH()/_lines){}

//画草地
void Ground::draw()
{
    //画大格子(地图)
    _s->drawRect(*this);
    //画小格子，i行j列
    for (int i = 0; i < _lines; i++)
    {
        for (int j = 0; j < _columns; j++)
        {
            draw_item(i,j);
        }
    }
}

//画小格子
void Ground::draw_item(int i,int j)
{
    //实例化一个小格子
    int w = _item_size.getW();
    int h = _item_size.getH();
    Rectangle item{j*w,i*h,w,h,_item_fill_color.getColor(),_item_border_width,_item_border_color.getColor()};
    //画小格子
    _s->drawRect(item);
}

void Ground::draw_item(int i,int j,uint color)
{
    //实例化一个小格子
    int w = _item_size.getW();
    int h = _item_size.getH();
    Rectangle item{j*w,i*h,w,h,color,_item_border_width,_item_border_color.getColor()};
    //画小格子
    _s->drawRect(item);
}

void Ground::draw_item(int i,int j,Color color)
{
    //实例化一个小格子
    int w = _item_size.getW();
    int h = _item_size.getH();
    Rectangle item{j*w,i*h,w,h,color.getColor(),_item_border_width,_item_border_color.getColor()};
    //画小格子
    _s->drawRect(item);
}
void Ground::draw_snaketou(int w,int h,MovDirection _dirction)
{
    int j = _item_size.getW()*w;
    int i = _item_size.getH()*h;
    switch (_dirction)
        {
        case MOVE_UP:
            _s->lcdDrawBMP(i,j,"5.bmp");//蛇头朝上
            break;
        case MOVE_DOWN:
             _s->lcdDrawBMP(i,j,"3.bmp");//蛇头朝下
            break;
        case MOVE_LEFT:
             _s->lcdDrawBMP(i,j,"4.bmp");//蛇头朝左
            break;
        case MOVE_RIGHT:
             _s->lcdDrawBMP(i,j,"6.bmp");//蛇头朝右
            break;
        default:
            break;
        }
    
}
// 绘制蛇身
void Ground::draw_snakeshen(int w,int h) {
    int j = _item_size.getW()*w;
    int i = _item_size.getH()*h;
    _s->lcdDrawBMP(i, j, "7.bmp"); // 使用对应图片资源
}

// 绘制蛇尾
void Ground::draw_snakewb(int w,int h) {
    int j = _item_size.getW()*w;
    int i = _item_size.getH()*h;
    _s->lcdDrawBMP(i, j, "7.bmp"); // 使用对应图片资源
}

//获取格子的行数
int Ground::get_lines() const
{
    return _lines;
}

//获取格子的列数
int Ground::get_columns() const
{
    return _columns;
}

uint Ground::get_item_fill_color() const
{
    return _item_fill_color.getColor();
}