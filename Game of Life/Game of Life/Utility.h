#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "Button.h"

// simulation logic
void SetPoint(SDL_Point* point, int x, int y);

// math
int lcm(int a, int b);
int gcd(int a, int b);
int signum(int n);

// file I/O
void SaveStateToFile();

SDL_Texture* LoadTexture(SDL_Renderer* renderer, const char* fileName);