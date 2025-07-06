#include "game.hpp"

bool Game::is_over = false;

Game::Game()
{
    //获取屏幕对象指针
    _screen = &Screen::getScreen();
    _ground = new Ground(0,0,LCD_W,LCD_H,COLOR_BLACK,2,COLOR_RED,_screen,24,40,COLOR_GREEN,1,COLOR_BLACK);
    _ground->draw();
    _handler = new EventHandler;    //实例化一个生产者对象
    _input_dev = new InputDev(_handler);    //实例化一个输入设备对象
    _snake = new Snake(_ground);
    _snake->draw();
    _snake->addObserver(_handler);
    _food = new Fooder{_ground,_snake,10*1000};
    _snake->setFooder(_food);
    //初始化线程
    t1 = _input_dev->startReadTouch();
    t2 = _handler->startEventLoop();
}

void Game::run()
{   
    _screen->lcdDrawBMP(0,0,"1.bmp");
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    _ground->draw();
    _snake->draw();
    t3 = _snake->startMove();
    t4 = _food->strartFooder();
    
    while (1)
    {
        if (is_over)
        {
           
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            _screen->brushBg(COLOR_BLACK);
            _screen->lcdDrawBMP(0,0,"2.bmp");
            std::this_thread::sleep_for(std::chrono::milliseconds(3000));
            return;
        }
    }
    t1->join();
    t2->join();
    t3->join();
    t4->join();
    
}