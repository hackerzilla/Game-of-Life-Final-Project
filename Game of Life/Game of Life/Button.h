#pragma once
#include "SDL.h"

class Button
{
public:
    unsigned int x;
    unsigned int y;
    unsigned int width;
    unsigned int height;

    // color 
    int r;
    int g;
    int b;
    int a;

    SDL_Rect rect;
    Button(unsigned int x, unsigned int y, unsigned int width, unsigned int height, int r, int g, int b, int a);
    ~Button();
};