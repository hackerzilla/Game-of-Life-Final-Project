#include "Texture.h"

// https://www.youtube.com/watch?v=YrWQsuDT3NE&list=PLhfAbcv9cehhkG7ZQK0nfIGJC_C-wSLrx&index=5

Texture::Texture(SDL_Renderer* renderer, const char* filePath, SDL_Rect& rect)
{
    texture = LoadTexture(renderer, filePath);
    targetRect.x = rect.x;
    targetRect.y = rect.y;
    targetRect.w = rect.w;
    targetRect.h = rect.h;
}