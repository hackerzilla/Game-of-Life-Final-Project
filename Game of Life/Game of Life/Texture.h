#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "Utility.h"

class Texture
{
public:
    Texture(SDL_Renderer* renderer, const char* fileName, SDL_Rect& targetRect);
    SDL_Texture* texture;
    SDL_Rect targetRect;
};