#ifndef __SIZE_HPP__
#define __SIZE_HPP__

class Size
{
public:
    Size(int w,int h);
    int getW() const;
    int getH() const;
    void setW(int w);
    void setH(int h);
    void setSize(int w,int h);

private:
    int _w;
    int _h;
};

#endif