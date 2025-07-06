#include "snake.hpp"
#include <algorithm>
#include <iostream>
using namespace std;
#include "game.hpp"

BodyItem::BodyItem(int i,int j,Color c):_i(i),_j(j),_color(c){}

int BodyItem::getI() const
{
    return _i;
}

int BodyItem::getJ() const
{
    return _j;
}

void BodyItem::setI(int i)
{
    _i = i;
}

void BodyItem::setJ(int j)
{
    _j = j;
}

uint BodyItem::getColor() const
{
    return _color.getColor();
}

void BodyItem::setColor(uint color)
{
    _color.setColor(color);
}

void BodyItem::setColor(Color &color)
{
    _color = color;
}

Snake::Snake(Ground *g,Color color,int speed):_color(color),_g(g),_speed(speed)
{
    //设置移动的默认方向为向右
    _dirction = MOVE_RIGHT;
    //设置初始有3格蛇身
    int i = 10,j = 20;
    for (int k = 0; k < 3; k++)
    {
        BodyItem item(i,j,_color);
        _body.push_back(item);
    }
}

int Snake::getSpeed() const   //获取当前移动速度
{
    return _speed;
}

void Snake::setSpeed(int speed)  //设置移动速度
{
    _speed = speed;
}

void Snake::draw()    //画蛇
{
    //for (auto it:_body)
    //{
        //_g->draw_item(it.getI(),it.getJ(),it.getColor());
    //}
    // 绘制蛇头
    // 绘制蛇头
    int x,y;
    _g->draw_snaketou(_body.front().getI(), _body.front().getJ(), _dirction);

    // 绘制蛇身（除了头和尾）
    auto it = _body.begin();
    ++it; // 跳过头部
    while (it != std::prev(_body.end())) {
        _g->draw_snakeshen(it->getI(), it->getJ());
        x= it->getI();
        y= it->getJ();
        cout<<"x = "<<x<<" y = "<<y<<endl;
        ++it;
    }
    
    // 绘制蛇尾
    if (_body.size() > 1) {
        
        _g->draw_snakewb(_body.back().getI(), _body.back().getJ());
    }
}

void Snake::hide()    //隐藏蛇
{
    if(!Game::is_over)
    {
        for (auto it:_body)
        {
            _g->draw_item(it.getI(),it.getJ(),_g->getFullColor());
        }
    }
    
}

void Snake::move()    //移动
{
    while (1)
    {
        //保存最后一个节点的位置，方便移动完以后，擦除这个节点
        int i = _body.back().getI();
        int j = _body.back().getJ();
        //将除了第一个节点以外的其他节点的位置替换为它前一个节点的位置
        auto it1 = _body.rbegin();
        auto it2 = it1++;   //it2指向后面的节点，it1指向前面的节点
        while (it1 != _body.rend())
        {
            (*it2).setI((*it1).getI());
            (*it2).setJ((*it1).getJ());
            it2 = it1++;
        }
        //将第一个节点按照移动的方向移动一格
        switch (_dirction)
        {
        case MOVE_UP:
            _body.front().setI(_body.front().getI()-1);
            break;
        case MOVE_DOWN:
            _body.front().setI(_body.front().getI()+1);
            break;
        case MOVE_LEFT:
            _body.front().setJ(_body.front().getJ()-1);
            break;
        case MOVE_RIGHT:
            _body.front().setJ(_body.front().getJ()+1);
            break;
        default:
            break;
        }
        //判断蛇头位置是否为食物
        if (_fooder->isFood(_body.front().getI(),_body.front().getJ()))
        {
            //蛇头和食物重合了，则表示吃到食物了
            //将该位置的食物清除
            _fooder->deleteFood(_body.front().getI(),_body.front().getJ());
            //将蛇的长度加1
            BodyItem item(i,j,_color);
            _body.push_back(item);
        }
        
        
        //判断游戏是否结束
        if (isOver())
        {           
            for (auto it:_body)
            {
                _g->draw_item(it.getI(),it.getJ(),_g->getFullColor());
            }  
            Game::is_over = true;
        }
        else
        {
            //将原来最后一个节点的位置擦除
            _g->draw_item(i,j,_g->get_item_fill_color());
            draw();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(_speed));
    }
}

void Snake::changeDirection(MovDirection dir) //改变蛇的方向
{
    if (_dirction == dir)
    {
        return;
    }
    else if ((_dirction == MOVE_UP and dir == MOVE_DOWN)or(_dirction == MOVE_DOWN and dir == MOVE_UP))
    {
        return;
    }
    else if ((_dirction == MOVE_LEFT and dir == MOVE_RIGHT)or(_dirction == MOVE_RIGHT and dir == MOVE_LEFT))
    {
        return;
    }
    _dirction = dir;
}

// void Snake::eat(); //吃到一个食物了

void Snake::addObserver(EventHandler *eh)
{
    eh->addObserver(this);
}

thread* Snake::startMove()    //蛇开始移动
{
    return new thread{&Snake::move,this};
}

bool Snake::isOver()  //判断蛇是否死了
{
    //情况1：蛇头碰到地图边框
    int i = _body.front().getI();
    int j = _body.front().getJ();
    int i_max = _g->get_lines();    //地图的行数
    int j_max = _g->get_columns();  //地图的列数
    if (i >= i_max or j >= j_max or i < 0 or j < 0)
    {
        cout<<"i = " << i << " j = " << j << endl;
        cout << "i_max = " << i_max << " j_max = " << j_max << endl;
        cout << "死亡" << endl;
        return true;    //死了
    }
    //情况2：蛇头碰到了自己
    auto it = _body.begin();
    for ( it++; it!=_body.end(); ++it)
    {
        //遍历除了第一个节点外的所有节点，如果位置重合，就说明碰到自己了
        if ((*it).getI() == i and (*it).getJ() == j)
        {
            //重合了
            return true;
        }
    }
    return false;
}

bool Snake::isBody(int i,int j)   //判断某个位置是不是蛇身
{
    for (auto it = _body.begin(); it != _body.end(); ++it)
    {
        if ((*it).getI() == i and (*it).getJ() == j)
        {
            //该位置是蛇身
            return true;
        }
    }
    return false;   //不是蛇身
}

void Snake::setFooder(Fooder *f)
{
    _fooder = f;
}