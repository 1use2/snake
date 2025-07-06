#ifndef __FOOD_HPP__
#define __FOOD_HPP__

#include "snake.hpp"
#include "Game.hpp"
//食物类型
class Food
{
public:
    Food(int i,int j,int timeout,Color c);
    Food(int i,int j,int timeout,uint c);
    int getTimeout() const;
    void setTimeout(int t);
    int getI() const;
    int getJ() const;
    void move(int i,int j);
    uint getColor() const;
    void setColor(uint color);
    void setColor(Color &color);
private:
    int _i,_j;    //食物的位置
    Color _color;   //食物的颜色
    int _timeout;   //食物的消亡时间，单位为ms
};

//食物管理者
class Fooder
{
public:
    Fooder(Ground *g,Snake *s,int t);
    void createFood();
    void createFood(int i,int j);
    void deleteFood(int i,int j);   //删除某个位置的食物
    void clearTimeoutFood();    //清除超时(过期)的食物
    bool isFood(int i,int j);   //判断某个位置是否为食物
    void foodFlash();   //让食物闪烁
    void fooderRun();  //食物管理者管理食物(定时产生食物，定时消除食物)
    thread *strartFooder(); //食物管理者启动线程
private:
    list<Food> _foods;  //食物链表
    Ground *_g; //食物是出现在哪个草地上
    Snake *_s;  //食物给哪条蛇吃
    int _timeout;
};

#endif