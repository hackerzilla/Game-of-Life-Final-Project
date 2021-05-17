#include "Utility.h"

// simulation logic

void SetPoint(SDL_Point* point, int x, int y) 
{
	point->x = x;
	point->y = y;
}

// math

int gcd(int a, int b) 
{
	// Euclidean Algorithm: https://en.wikipedia.org/wiki/Greatest_common_divisor

	if (a == 0)
	{
		return b;
	} 	else if (b == 0)
	{
		return a;
	} 	else if (a > b)
	{
		return gcd(a % b, b);
	} 	else if (b > a)
	{
		return gcd(b % a, a);
	}
}

int lcm(int a, int b)
{
	// How to calculate / what is lcm https://en.wikipedia.org/wiki/Least_common_multiple

	return a * b / gcd(a, b);
}

int signum(int n)
{
	if (n < 0) return -1;
	if (n > 0) return 1;
	return 0;
}

// file I/O

// save the state in a file named with the date and time
void SaveStateToFile() 
{
	// 1. learn JSON
	// 2. write 2D bool array to a JSON formatted file
	// 3. ???
	// 4. profit
}

SDL_Texture* LoadTexture(SDL_Renderer* renderer, const char* fileName)
{
	SDL_Surface* tempSurface = IMG_Load(fileName);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
	SDL_FreeSurface(tempSurface);
	return texture;
}