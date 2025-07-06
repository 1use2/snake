#ifndef __EVENTHANDLER_HPP__
#define __EVENTHANDLER_HPP__

#include <queue>
#include <mutex>
#include <thread>
#include <list>
#include <condition_variable>
using namespace std;

enum MovDirection
{
    MOVE_UNKNOWN,   //未知移动方向
    MOVE_UP,        //向上移动
    MOVE_DOWN,      //向下移动
    MOVE_LEFT,      //向左移动
    MOVE_RIGHT      //向右移动
};

enum InputEvent
{
    INPUTEVENT_UNKOWN,  //未知事件
    INPUTEVENT_TS_PRESS,    //触摸屏按下
    INPUTEVENT_TS_MOV_UP,   //向上滑
    INPUTEVENT_TS_MOV_DOWN,
    INPUTEVENT_TS_MOV_LEFT,
    INPUTEVENT_TS_MOV_RIGHT,
    INPUTEVENT_TS_RELEASE   //触摸屏释放
};

//事件监听者(消费者)
class EventObserver
{
public:
    virtual void changeDirection(MovDirection) = 0; //纯虚函数
};

//事件处理者(生产者)
class EventHandler
{
public:
    void addEvent(InputEvent e);    //添加一个事件
    InputEvent popEvent();          //消费一个事件
    void eventLoop();   //事件循环(循环检测事件)，需要一个线程来执行它
    void addObserver(EventObserver *eo);   //添加一个消费者
    void deletObserver(EventObserver *eo); //删除一个消费者
    thread* startEventLoop();   //启动线程函数

private:
    queue<InputEvent> _events;   //事件队列，存储了所有产生的事件值
    mutex _ev_lock;  //互斥锁，用来保护events中的事件值有序访问
    condition_variable _has_event;   //条件变量，当有数据了的时候，就去通知消费者
    list<EventObserver*> _observers; //事件监听者列表(消费者列表)
    mutex _ob_lock;
};


#endif