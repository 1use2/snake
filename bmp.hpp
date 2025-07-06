#ifndef __BMP_HPP__
#define __BMP_HPP__
#include"color.hpp"
void lcdBrushBG(unsigned int color);
void lcdDrawPoin(int x, int y, unsigned int color);
void lcdDrawRect(int x0, int y0, int w, int h, unsigned int color);
void lcdDrawCircle(int x0, int y0, int r, unsigned int color);
void lcdDrawWord(int x0, int y0, char *pword, int w_w, int w_h, unsigned int color);
void lcdDrawWords(int x0, int y0, char *str, unsigned int color);
void lcdDrawNumbers(int x0, int y0, int num, unsigned int color);
void lcdDrawBMP(int x0, int y0, const char* pic_path);
#endif