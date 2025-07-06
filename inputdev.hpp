#ifndef __INPUTDEV_HPP__
#define __INPUTDEV_HPP__

#include "eventhandler.hpp"

//输入设备是一种生产者
class InputDev
{
public:
    InputDev(EventHandler *handler,string dev_path = "/dev/input/event0");
    ~InputDev();
    void readTouch();   //读取触摸屏数据，循环读取，需要一个线程来执行
    thread *startReadTouch();

private:
    string _dev_path;
    EventHandler *_handler; //生产者类型的指针，输入设备是一种生产者，生产出来的数据应该要加入到数据队列中去
                            //要通过这个生产者将数据添加到事件队列中去
    int fd_touch;
};


#endif