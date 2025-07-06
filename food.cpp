#include "food.hpp"
#include <time.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

Color food_colors[7] = {{101,75,245},{230,119,91},{67,245,244},{150,150,171},{191,192,129},{200,72,249},{254,221,64}};
Food::Food(int i,int j,int timeout,Color c):_i(i),_j(j),_timeout(timeout),_color(c){}
Food::Food(int i,int j,int timeout,uint c):_i(i),_j(j),_timeout(timeout),_color(c){}

int Food::getI() const
{
    return _i;
}

int Food::getJ() const
{
    return _j;
}

int Food::getTimeout() const
{
    return _timeout;
}

void Food::setTimeout(int t)
{
    _timeout = t;
}

void Food::move(int i,int j)
{
    _i = i;
    _j = j;
}

uint Food::getColor() const
{
    return _color.getColor();
}

void Food::setColor(uint color)
{
    _color.setColor(color);
}

void Food::setColor(Color &color)
{
    _color.setColor(color);
}

Fooder::Fooder(Ground *g,Snake *s,int t):_g(g),_s(s),_timeout(t)
{
    //设置随机种子
    srand(time(NULL));
    //在随机位置产生一个食物
    createFood();
}

void Fooder::createFood()
{
    while (1)
    {
        //随机一个位置
        int i = rand()%_g->get_lines(); //随机一个地图范围内的行号
        int j = rand()%_g->get_columns();   //随机一个地图范围内的列号
        //判断该位置是否已经存在食物了
        if (!isFood(i,j) and !_s->isBody(i,j))
        {
            //该位置不是食物且不是在蛇身上，就产生食物，否则就换个位置
            _foods.push_back(Food{i,j,_timeout,food_colors[rand()%7]});
            //将食物显示出来
            _g->draw_item(i,j,_foods.back().getColor());
            break;
        }
    }
}

void Fooder::createFood(int i,int j)
{
    if (!isFood(i,j) and !_s->isBody(i,j))
    {
        //该位置不是食物且不是在蛇身上，就产生食物，否则就不添加
        _foods.push_back(Food{i,j,_timeout,food_colors[rand()%7]});
        //将食物显示出来
        _g->draw_item(i,j,_foods.back().getColor());
    }
}

void Fooder::deleteFood(int i,int j)   //删除某个位置的食物
{
    if(isFood(i,j))
    {
        //从地图上将食物擦掉
        _g->draw_item(i,j,_g->get_item_fill_color());
        for (auto it = _foods.begin(); it != _foods.end(); ++it)
        {
            if ((*it).getI() == i and (*it).getJ() == j)
            {
                _foods.erase(it);
                return;
            }
        }
    }
}

void Fooder::clearTimeoutFood()    //清除超时(过期)的食物
{
    auto it2 = _foods.begin();
    ++it2;
    for (auto it = _foods.begin(); it != _foods.end(); )
    {
        //如果食物的剩余时间小于等于0了，就将它清除掉
        if ((*it).getTimeout() <= 0)
        {
            //从地图上将食物擦掉
            _g->draw_item((*it).getI(),(*it).getJ(),_g->get_item_fill_color());
            _foods.erase(it);
            //迭代器失效
            it = it2;
            ++it2;
        }
        else
        {
            ++it;
            ++it2;
        }
    }
}

bool Fooder::isFood(int i,int j)   //判断某个位置是否为食物
{
    for (auto it = _foods.begin(); it != _foods.end(); ++it)
    {
        if ((*it).getI() == i and (*it).getJ() == j)
        {
            return true;
        }
    }
    return false;
}

void Fooder::foodFlash()   //让食物闪烁
{
    //让食物换一种颜色
    for (auto it = _foods.begin(); it != _foods.end(); ++it)
    {
        //先保存当前颜色值，避免换的颜色和当前颜色一样
        uint color = (*it).getColor();
        while (1)
        {
            char index = rand()%7;
            if (food_colors[index].getColor() != color)
            {
                //找到了一种不同的颜色
                //更换颜色
                (*it).setColor(food_colors[index]);
                //重新画食物
                _g->draw_item((*it).getI(),(*it).getJ(),(*it).getColor());
                break;
            }
        }
    }
}

void Fooder::fooderRun()  //食物管理者管理食物(定时产生食物，定时消除食物)
{
    int time = 0;   //用来记录循环次数
    while (1)
    {
        if(!Game::is_over)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(400));
            //食物闪烁
            foodFlash();
            //减少所有食物的消亡时间
            for (auto it = _foods.begin(); it != _foods.end(); ++it)
            {
                (*it).setTimeout((*it).getTimeout()-400);
            }
            //消除过期的食物
            clearTimeoutFood();
            //每两秒钟产生一个食物
            time++;
            if (time == 5)
            {
                //两秒钟了
                createFood();
                time = 0;
            }
        }
        else
        {
            auto it2 = _foods.begin();
            ++it2;
            for (auto it = _foods.begin(); it != _foods.end(); )
            {
                
                //从地图上将食物擦掉
                _g->draw_item((*it).getI(),(*it).getJ(),_g->get_item_fill_color());
                _foods.erase(it);
                //迭代器失效
                it = it2;
                ++it2;
                
            }
        }
    }
}

thread *Fooder::strartFooder() //食物管理者启动线程
{
    return new thread{&Fooder::fooderRun,this};
}