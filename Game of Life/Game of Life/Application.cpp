/*
* Author: Julian Pearson Rickenbach
* Date of Creation: 4/23/2021 6:09 PM
* Date of Last Update: 4/23/2021 6:09 PM
* Dev Log:
	This is my CIS 27 final project.
	Its a version of John Conway's Game of Life
	of my own making. For this I will be using
	Visual Studio and SDL (Simple Direct Media Layer).

	Since I already have an idea of how the game's
	implementation will look like, the first task ahead of
	me is to figure out how to open a window in SDL and draw
	graphics to it.
*/

#include "SDL.h"
#include <iostream>
#include <ctime>
#include <random>
#include <cstdio>
#include <cstdlib>

const unsigned int BUTTON_HEIGHT = 32;
const unsigned int BUTTON_WIDTH = 64;
const unsigned int NON_SIMULATION_FRAME_DELAY = 10;
const unsigned int SIMULATION_FRAME_DELAY = 100;
const unsigned int MIN_GRID_GRANULARITY = 7; // in pixels

void SetPoint(SDL_Point* point, int x, int y);
int GetNeighborCount(bool** arr, int x, int y, int xMax, int yMax);
void ClearArray(bool** arr, int xMax, int yMax);
void SaveStateToFile();
void DrawCells(SDL_Renderer* renderer, bool** cells, int pixels_per_cell, int cells_width, int cells_height, int view_width, int view_height, unsigned int originX, unsigned int originY);
void DrawGrid(SDL_Renderer* renderer, int pixels_per_cell, int view_width, int view_height);
int gcd(int a, int b);
bool* GetCell(int x, int y, bool** cells, unsigned int cells_width, unsigned int cells_height, unsigned int pixels_per_cell, unsigned int originX, unsigned int originY, int& x_out, int& y_out);


int main(int argc, char** argv)
{
	std::srand(time(NULL));

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window* window = SDL_CreateWindow("Game of Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 900, 900, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	
	// flag vars
	bool gameIsRunning = true;
	bool simulationIsRunning = false;
	bool mouseIsHeld = false;
	bool is_grid_displaying = true;

	// temp containers
	SDL_Event event;
	SDL_Point mouse_pos;

	// setting up simulation space and viewport vars
	unsigned int originX(0), originY(0);
	int cells_width = 100;
	int cells_height = 100;
	int view_width;
	int view_height;
	SDL_GetWindowSize(window, &view_width, &view_height);
	view_height = view_height - BUTTON_HEIGHT; // the simulation space should begin below the buttons.
	unsigned int pixels_per_cell = gcd( gcd(view_height, cells_height), gcd(view_width, cells_width) );
	//std::cout << "h: " << view_height << ", w: " << view_width << ", ppc: " << pixels_per_cell << std::endl;
	unsigned int ppc_increment = pixels_per_cell;
	unsigned int min_ppc = ppc_increment;
	unsigned int max_ppc = 10 * min_ppc;

	// rect to represent top toolbar
	SDL_Rect toolbar_rect;
	toolbar_rect.x = 0;
	toolbar_rect.y = 0;
	toolbar_rect.h = BUTTON_HEIGHT;
	toolbar_rect.w = view_width;

	// quit button
	SDL_Rect quit_button;
	quit_button.x = 0;
	quit_button.y = 0;
	quit_button.h = 32;
	quit_button.w = 64;
	// pause/play button
	SDL_Rect pauseplay_button;
	pauseplay_button.x = quit_button.w;
	pauseplay_button.y = 0;
	pauseplay_button.h = quit_button.h;
	pauseplay_button.w = quit_button.w;
	// clear simulation button
	SDL_Rect clear_button;
	clear_button.h = quit_button.h;
	clear_button.w = quit_button.w;
	clear_button.x = view_width - clear_button.w;
	clear_button.y = 0;
	// save state button
	SDL_Rect save_button;
	save_button.h = BUTTON_HEIGHT;
	save_button.w = BUTTON_WIDTH;
	save_button.x = view_width - 2*BUTTON_WIDTH;
	save_button.y = 0;

	// test rect
	SDL_Rect test_rect;
	test_rect.x = 0;
	test_rect.y = BUTTON_HEIGHT;
	test_rect.w = pixels_per_cell;
	test_rect.h = pixels_per_cell;
	
	bool** currentCellMatrix = new bool* [cells_height];
	bool** nextCellMatrix = new bool* [cells_height];
	for (int i = 0; i < cells_width; i++)
	{
		currentCellMatrix[i] = new bool[cells_width];
		nextCellMatrix[i] = new bool[cells_width];

		for (int j = 0; j < cells_width; j++)
		{
			currentCellMatrix[i][j] = false;
			nextCellMatrix[i][j] = false;
		}
	}

	// pseudo-random state generation
	/*
	for (int i = 0; i < cells_height; i++)
	{
		for (int j = 0; j < cells_width; j++)
		{
			cells[i][j] = std::rand() % 2;
		}
	}*/
	/*
	for (int i = 0; i < cells_height; i++)
	{
		for (int j = 0; j < cells_width; j++)
		{
			//cells[i][j] = true;
			cells[i][j] = i % 2 && j % 2; // checkerboard generation
		}
	}*/

	while (gameIsRunning)
	{
		// ----- Input Events ------
		
		while (SDL_PollEvent(&event) && gameIsRunning)
		{
			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				// start mouse hold
				mouseIsHeld = true;
				// mouse position as a point object
				SetPoint(&mouse_pos, event.button.x, event.button.y);

				// quit button handling
				if (SDL_PointInRect(&mouse_pos, &quit_button))
				{
					SDL_Log("Quit Button pressed, quiting!");
					gameIsRunning = false;
				}
				// pause/play button handling
				else if (SDL_PointInRect(&mouse_pos, &pauseplay_button))
				{
					SDL_Log("Pause/play button pressed, toggling simulation on/off!");
					simulationIsRunning = !simulationIsRunning;
				}
				// clear button 
				else if (SDL_PointInRect(&mouse_pos, &clear_button))
				{
					SDL_Log("Clear button pressed, clearing the simulation space!");
					ClearArray(currentCellMatrix, cells_width, cells_height);
				}
				// save to file button
				else if (SDL_PointInRect(&mouse_pos, &save_button))
				{
					SDL_Log("Saving the simulation state to a file.");
					SaveStateToFile();
				}
				// toggle the pixel on/off
				else if (!simulationIsRunning)
				{
					// need to rewrite the mouse to cell detection code
					int x, y;
					bool* cell = GetCell(mouse_pos.x, mouse_pos.y, currentCellMatrix, cells_width, cells_height, pixels_per_cell, originX, originY, x, y);
					if (cell != nullptr) *cell = !*cell;
					
				}
			}
			else if (mouseIsHeld && event.type == SDL_MOUSEMOTION)
			{
				SetPoint(&mouse_pos, event.button.x, event.button.y);

				// this will crash if it doesn't filter out the condition of the mouse being on top of a button when released
				if (!(SDL_PointInRect(&mouse_pos, &toolbar_rect)))
				{
					// detect the cell that the mouse point is in. then flip that cell
					int x, y;
					bool* cell = GetCell(mouse_pos.x, mouse_pos.y, currentCellMatrix, cells_width, cells_height, pixels_per_cell, originX, originY, x, y);
					if (cell != nullptr) *cell = !*cell;
				}
			}
			else if (event.type == SDL_MOUSEBUTTONUP)
			{
				mouseIsHeld = false;
			}
			else if (event.type == SDL_MOUSEWHEEL)
			{
				// increment or decrement pixels_per_cell
				int ppc_change = event.wheel.y;

				pixels_per_cell += ppc_change;
				
				// clamp the pixels_per_cell variable to a reasonable range 
				if (pixels_per_cell < min_ppc)
				{
					pixels_per_cell = min_ppc;
				}
				else if (pixels_per_cell > max_ppc)
				{
					pixels_per_cell = max_ppc;
				}
			}
			else if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_DOWN)
				{
					originY += pixels_per_cell;
				} 
				else if (event.key.keysym.sym == SDLK_UP)
				{
					if (originY >= pixels_per_cell)
					{
						originY -= pixels_per_cell;
					}
				}
				else if (event.key.keysym.sym == SDLK_LEFT)
				{
					if (originX >= pixels_per_cell)
					{
						originX -= pixels_per_cell;
					}
				}
				else if (event.key.keysym.sym == SDLK_RIGHT)
				{
					originX += pixels_per_cell;
				}
			}
		}

		// ----- Simulation Update Logic ------
		
		if (simulationIsRunning)
		{
			for (int i = 0; i < cells_height; i++)
			{
				for (int j = 0; j < cells_width; j++)
				{
					unsigned int neighborCount = GetNeighborCount(currentCellMatrix, j, i, cells_width - 1, cells_height - 1);
					
					bool isAlive = currentCellMatrix[i][j];
					bool shouldSurvive = neighborCount == 2 || neighborCount == 3;
					bool shouldLive = neighborCount == 3;

					if (isAlive)
					{
						std::cout << "cell [i,j] : [" << i << "," << j << "] n = " << neighborCount << std::endl;
						if (shouldSurvive)
						{
							nextCellMatrix[i][j] = true;
						}
					}
					else if (!isAlive && shouldLive)
					{
						std::cout << "cell [i,j] : [" << i << "," << j << "] alive = false n = " << neighborCount << std::endl;

						nextCellMatrix[i][j] = true;
					}
					else
					{
						nextCellMatrix[i][j] = false;
					}
				}
			}
		}

		// -----Render------
		
		// clear all to a black screen
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// render quit button
		SDL_SetRenderDrawColor(renderer, 255, 123, 123, 255);
		SDL_RenderFillRect(renderer, &quit_button);

		// render pause/play button
		SDL_SetRenderDrawColor(renderer, 123, 255, 123, 255);
		SDL_RenderFillRect(renderer, &pauseplay_button);

		// render clear button
		SDL_SetRenderDrawColor(renderer, 255, 128, 0, 255);
		SDL_RenderFillRect(renderer, &clear_button);

		// render save button
		SDL_SetRenderDrawColor(renderer, 127, 0, 255, 255);
		SDL_RenderFillRect(renderer, &save_button);

		// draw each cell
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		if (simulationIsRunning)
		{
			// render next cell frame
			DrawCells(renderer, nextCellMatrix, pixels_per_cell, cells_width, cells_height, view_width, view_height, originX, originY);
		}
		else
		{
			// render current
			DrawCells(renderer, currentCellMatrix, pixels_per_cell, cells_width, cells_height, view_width, view_height, originX, originY);
		}
		
		if (pixels_per_cell > MIN_GRID_GRANULARITY && is_grid_displaying) 
		{
			DrawGrid(renderer, pixels_per_cell, view_width, view_height);
		}

		// render everything!
		SDL_RenderPresent(renderer);

		if (simulationIsRunning)
		{
			SDL_Delay(SIMULATION_FRAME_DELAY);
		}
		else 
		{
			SDL_Delay(NON_SIMULATION_FRAME_DELAY);
		}

		if (simulationIsRunning)
		{
			// for swap 
			bool** temp = currentCellMatrix;

			currentCellMatrix = nextCellMatrix;
			nextCellMatrix = temp;
			simulationIsRunning = false;
		}
	}
	
	// free cells memory
	for (int i = 0; i < cells_height; i++)
	{
		delete[] currentCellMatrix[i];
		delete[] nextCellMatrix[i];
	}
	delete[] currentCellMatrix;
	delete[] nextCellMatrix;
	
	return 0;
}

void SetPoint(SDL_Point* point, int x, int y)
{
	point->x = x;
	point->y = y;
}

int GetNeighborCount(bool** arr, int x, int y, int xMax, int yMax)
{
	// check surrounding neighbors for existence
	unsigned int result = 0;

	//
	//   0 0 0
	//   0 X 0
	//   0 0 0
	//
	
	// top left
	if (y != 0 && x != 0) 
		if (arr[y - 1][x - 1]) result++;

	// top
	if (y != 0) 
		if (arr[y - 1][x]) result++;

	// top right
	if (y != 0 && x != xMax) 
		if (arr[y - 1][x + 1]) result++;

	// right
	if (x != xMax) 
		if (arr[y][x + 1]) result++;

	// bottom right
	if (y != yMax && x != xMax)
		if (arr[y + 1][x + 1]) result++;

	// bottom
	if (y != yMax) 
		if (arr[y + 1][x]) result++;

	// bottom left
	if (y != yMax && x != 0) 
		if (arr[y + 1][x - 1]) result++;

	// left
	if (x != 0) 
		if (arr[y][x - 1]) result++;

	return result;
}

void ClearArray(bool** arr, int xMax, int yMax)
{
	for (int i = 0; i < yMax; i++)
	{
		for (int j = 0; j < xMax; j++)
		{
			arr[i][j] = false;
		}
	}
}

// save the state in a file named with the date and time
void SaveStateToFile()
{
	// 1. learn JSON
	// 2. write 2D bool array to a JSON formatted file
	// 3. ???
	// 4. profit
}

void DrawCells(SDL_Renderer* renderer, bool** cells, int pixels_per_cell, int cells_width, int cells_height, int view_width, int view_height, unsigned int originX, unsigned int originY)
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	// screen bounds are from 0 -> view_width and 0 -> view_height
	// number of cells that can fit:
		// horizontal = view_width / pixel_per_cell;
		// vertical = view_height / pixel_per_cell;

	int x, y; // x and y pixel (in the usable screen space)
	unsigned int j, i; // the corresponding indices of cells

	for (y = BUTTON_HEIGHT; y < view_height; y += pixels_per_cell)
	{
		for (x = 0; x < view_width; x += pixels_per_cell)
		{
			// use the x and y pixel to find the corresponding cell in cells

			j = (originX + x) / pixels_per_cell;
			i = (originY + y - BUTTON_HEIGHT) / pixels_per_cell;
			// check that ith and jth indices are within the array bounds
			if (i < cells_height - 1 && j < cells_width - 1)
			{
				if (cells[i][j])
				{
					SDL_Rect cell_rect;
					cell_rect.w = pixels_per_cell;
					cell_rect.h = pixels_per_cell;
					cell_rect.x = x;
					cell_rect.y = y;
					SDL_RenderFillRect(renderer, &cell_rect);
				}
			}
		}
	}
}

void DrawGrid(SDL_Renderer* renderer, int pixel_per_cell, int view_width, int view_height)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);

	int x, y;

	for (x = 0; x < view_width; x += pixel_per_cell)
	{
		SDL_RenderDrawLine(renderer, x, BUTTON_HEIGHT, x, view_height + BUTTON_HEIGHT);
	}
	for (y = BUTTON_HEIGHT; y < view_width; y += pixel_per_cell)
	{
		SDL_RenderDrawLine(renderer, 0, y, view_width, y);
	}
}

int gcd(int a, int b)
{
	// Euclidean Algorithm: https://en.wikipedia.org/wiki/Greatest_common_divisor

	if (a == 0)
	{
		return b;
	}
	else if (b == 0)
	{
		return a;
	}
	else if (a > b)
	{
		return gcd(a % b, b);
	}
	else if (b > a)
	{
		return gcd(b % a, a);
	}
}

bool* GetCell(int x, int y, bool** cells, unsigned int cells_width, unsigned int cells_height, unsigned int pixels_per_cell, unsigned int originX, unsigned int originY, int& x_out, int& y_out)
{
	x_out = -1;
	y_out = -1;
	int j = (originX + x) / pixels_per_cell;
	int i = (originY + y - BUTTON_HEIGHT) / pixels_per_cell;
	// check that ith and jth indices are within the array bounds
	if (i < cells_height - 1 && j < cells_width - 1)
	{
		x_out = j;
		y_out = i;
		return &cells[i][j];
	}
	return nullptr;
}