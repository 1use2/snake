# snake
通过GEC6818开发板，使用C++编程语言在开发板上运行贪吃蛇小游戏。

这是一个还没有特别成熟的贪吃蛇小游戏，因为课设的时间的问题，我实现了具备触摸屏控制、食物随机生成、边界检测等功能的贪吃蛇游戏。 通过多线程处理输入与游戏逻辑，利用帧缓冲技术实现图形渲染，最终在开发板的 LCD 触摸屏上运行。但是游戏不太美观，我在代码中封装了关于图片在类的多种函数，可以自行对游戏进行修改。

将main传到开发板上运行可以直接开始游戏。

注意：这是在开发板上运行的游戏，在电脑上运行会失败！！！（可能提示头文件无法识别等语法错误）

1.C语言编译器：
arm-linux-gcc编译器。
2.软件环境：
VM虚拟机，Ubuntu18.04操作系统，windows 10操作系统。
3.调试工具
Secure CRT。
