#ifndef __GAME_HPP__
#define __GAME_HPP__

#include "snake.hpp"
#include "eventhandler.hpp"
#include <thread>
#include "inputdev.hpp"
#include "food.hpp"

class Game
{
public:
    Game();
    void run();
    static bool is_over;    //判断游戏是否结束

private:
    //草地
    Ground *_ground;
    //蛇
    Snake *_snake;
    //食物生产者
    Fooder *_food;
    //屏幕
    Screen *_screen;
    //输入设备
    InputDev *_input_dev;
    //事件处理对象(生产者)
    EventHandler *_handler;
    thread *t1; //处理触摸屏
    thread *t2; //处理事件循环
    thread *t3; //处理蛇移动
    thread *t4; //处理食物产生与清除
};

#endif