#include "eventhandler.hpp"

void EventHandler::addEvent(InputEvent e)    //添加一个事件
{
    unique_lock<mutex> lm{_ev_lock};    //为了保护事件队列有序访问，用一个锁管理对象来管理共享资源，防止多个生产者同时插入数据
    _events.push(e);
    _has_event.notify_one();    //事件队列中有事件了，通知消费者来消费
}

InputEvent EventHandler::popEvent()          //消费一个事件
{
    unique_lock<mutex> lm{_ev_lock};    //为了保护事件队列有序访问
    InputEvent e = _events.front();
    _events.pop();
    return e;
}

void EventHandler::eventLoop()   //事件循环(循环检测事件)，需要一个线程来执行它
{
    while (1)
    {
        //当没有事件时，让所有消费者等待
        if (_events.empty())
        {
            unique_lock<mutex> lm{_ev_lock};
            _has_event.wait(lm);
        }
        else
        {
            //有数据了，通知所有的消费者来消费
            //先获取一个事件
            InputEvent e = popEvent();
            //通知每一个消费者
            unique_lock<mutex> lm{_ob_lock};
            for (auto it = _observers.begin(); it != _observers.end(); ++it)
            {
                if (e == INPUTEVENT_TS_MOV_UP)
                {
                    //向上移动
                    (*it)->changeDirection(MOVE_UP);
                }
                else if(e == INPUTEVENT_TS_MOV_DOWN)
                {
                    //向下移动
                    (*it)->changeDirection(MOVE_DOWN);
                }
                else if(e == INPUTEVENT_TS_MOV_LEFT)
                {
                    //向左移动
                    (*it)->changeDirection(MOVE_LEFT);
                }
                else if(e == INPUTEVENT_TS_MOV_RIGHT)
                {
                    //向右移动
                    (*it)->changeDirection(MOVE_RIGHT);
                }
            }
        }
    }
}

void EventHandler::addObserver(EventObserver *eo)   //添加一个消费者
{
    unique_lock<mutex> lm{_ob_lock};
    _observers.push_back(eo);
}

void EventHandler::deletObserver(EventObserver *eo) //删除一个消费者
{
    unique_lock<mutex> lm{_ob_lock};
    _observers.remove(eo);
}

thread* EventHandler::startEventLoop()   //启动线程函数
{
    return new thread{&EventHandler::eventLoop,this};
}