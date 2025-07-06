#include "inputdev.hpp"
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <linux/input.h>

InputDev::InputDev(EventHandler *handler,string dev_path):_handler(handler),_dev_path(dev_path)
{
    //打开触摸屏设备文件
    fd_touch = open(_dev_path.c_str(),O_RDONLY);
    if (fd_touch == -1)
    {
        //打开触摸屏设备文件失败
        throw std::runtime_error(string{"open touch error:"} + strerror(errno));
    }
    cout << "touch init success!" << endl;
}

InputDev::~InputDev()
{
    close(fd_touch);
    cout << "touch closed!" << endl;
}

void InputDev::readTouch()   //读取触摸屏数据，循环读取，需要一个线程来执行
{
    //定义一个输入事件的结构体变量，用来保存读到的输入事件
    struct input_event ev;
    ssize_t res;
    int x0,y0,x,y;
    InputEvent ie = INPUTEVENT_UNKOWN;  //滑动方向
    while (1)
    {
        //循环读取触摸屏数据
        res = read(fd_touch,&ev,sizeof(ev));
        if (res == -1)
        {
            throw std::runtime_error(string{"read touch error:"} + strerror(errno));
        }
        else if (res != sizeof(ev))
        {
            cout << "read touch data error!" << endl;
            continue;
        }
        //分析触摸屏数据
        if (ev.type == EV_ABS)
        {
            //绝对值事件类型
            if (ev.code == ABS_X)
            {
                //触摸点X值的事件，更新x值
                x = ev.value;
                //如果GEC6818屏幕是黑色PCB板，需要进行等比缩放
                x = x*799/1023.0;
            }
            else if (ev.code == ABS_Y)
            {
                //触摸点X值的事件，更新y值
                y = ev.value;
                //如果GEC6818屏幕是黑色PCB板，需要进行等比缩放
                y = y*479/599.0;
            }
        }
        else if (ev.type == EV_KEY and ev.code == BTN_TOUCH)
        {
            //触摸屏按键事件类型(按下和释放)
            if (ev.value == 1)
            {
                //触摸屏被按下(手指接触触摸屏)
                //获取起始点坐标
                x0 = x;
                y0 = y;
            }
            else if (ev.value == 0)
            {
                //触摸屏被释放(手指离开触摸屏)
                //判断滑动方向
                if (abs(x-x0) >= abs(y-y0) and abs(x-x0) >= 30)
                {
                    //水平方向滑动
                    if (x > x0)
                    {
                        //向右滑
                        ie = INPUTEVENT_TS_MOV_RIGHT;
                    }
                    else
                    {
                        //向左滑
                        ie = INPUTEVENT_TS_MOV_LEFT;
                    }
                }
                else if (abs(x-x0) < abs(y-y0) and abs(y-y0) >= 30)
                {
                    //垂直方向滑动
                    if (y > y0)
                    {
                        //向下滑
                        ie = INPUTEVENT_TS_MOV_DOWN;
                    }
                    else
                    {
                        //向上滑
                        ie = INPUTEVENT_TS_MOV_UP;
                    }
                }
                else
                {
                    ie = INPUTEVENT_UNKOWN;
                }
                //将事件添加到生产者的事件队列中去
                if (ie != INPUTEVENT_UNKOWN)
                {
                    _handler->addEvent(ie);
                }
            }
        }
    }
}

thread *InputDev::startReadTouch()
{
    return new thread{&InputDev::readTouch,this};
}