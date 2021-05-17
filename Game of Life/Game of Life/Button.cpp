#include "Button.h"

Button::Button(unsigned int x, unsigned int y, unsigned int width, unsigned int height, int r, int g, int b, int a)
    : x(x), y(y), width(width), height(height), r(r), b(b), g(g), a(a)
{
    rect.x = x;
    rect.y = y;
    rect.w = width;
    rect.h = height;;
}

Button::~Button() {}