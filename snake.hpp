#ifndef __SNAKE_HPP__
#define __SNAKE_HPP__

#include "color.hpp"
#include <list>
using std::list;
#include "ground.hpp"
#include "eventhandler.hpp"
#include <thread>

class Fooder;
//蛇身
class BodyItem
{
public:
    BodyItem(int i,int j,Color c = COLOR_RED);
    int getI() const;
    int getJ() const;
    void setI(int i);
    void setJ(int j);
    uint getColor() const;
    void setColor(uint color);
    void setColor(Color &color);

private:
    int _i,_j;    //在第i行j列的位置
    Color _color;   //蛇身的颜色
};

//蛇类，是一个消费者，要将蛇加入到消费者队列中去
class Snake:public EventObserver
{
public:
    Snake(Ground *g,Color color = COLOR_RED,int speed = 200);
    int getSpeed() const;   //获取当前移动速度
    void setSpeed(int speed);  //设置移动速度
    void draw();    //画蛇
    void hide();    //隐藏蛇
    void move();    //移动
    void changeDirection(MovDirection dir); //改变蛇的方向
    // void eat(); //吃到一个食物了
    void addObserver(EventHandler *eh); //添加到消费者队列中去
    thread* startMove();    //蛇开始移动
    bool isOver();  //判断蛇是否死了
    bool isBody(int i,int j);   //判断某个位置是不是蛇身
    void setFooder(Fooder *f);
private:
    list<BodyItem> _body;   //蛇身
    Color _color;   //蛇身的颜色
    Ground *_g;     //草地
    int _speed; //蛇移动的速度(单位为毫秒)
    MovDirection _dirction; //移动方向
    Fooder *_fooder;
};

#endif