#include "Renderer.h"
#include "Utility.h"
#include <iostream>
using namespace std;

Renderer::Renderer(SimulationState& simulation, int width, int height) 
	: window_width(0), window_height(0), origin()
{
    window = SDL_CreateWindow("Game of Life",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              width, height,
                              SDL_WINDOW_SHOWN);

	renderer = SDL_CreateRenderer(window, -1, 0);

	SDL_GetWindowSize(window, &window_width, &window_height);
	// window height should reflect the usable pixel space, which doesn't include the toolbar we are placing at the top of the window
	window_height = window_height - BUTTON_HEIGHT;
	
	// we want to set pixels_per_cell to a number that allows cells to be fit perfectly into the number of available pixels
	// we also want the starting pixels_per_cell to be enough to fit the whole simulation inside of the window's pixel 
	// therefore it should be the least common multiple of both the number of horizontal pixels and the number of vertical pixels
	
	// first we will find the number of pixels it would take to have all rows of cells on the screen at once. 
	// Then the same for rows and horizontal pixels.
	//int ppcVertical = window_height / simulation.rows;
	//int ppcHorizontal = window_width / simulation.columns;
	
	// then we find the lowest common multiple of those two minimum pixels_per_cell values to find the lowest ppc value that will fit both
	// horiontal and verticl axes.
	min_ppc = 1;
	//pixels_per_cell = lcm(ppcVertical, ppcHorizontal);
	pixels_per_cell = 10;
	max_ppc = lcm(simulation.rows * pixels_per_cell, simulation.columns * pixels_per_cell);

	origin.x = 0;
	origin.y = 0;

	ui            = new UIManager(window_width);
	quitText      = new Texture(renderer, "Textures/QuitButton.png",        ui->quit->rect);
	playPause     = new Texture(renderer, "Textures/PlayPauseButton.png",   ui->playPause->rect);
	stepThrough   = new Texture(renderer, "Textures/StepButton.png",        ui->stepThrough->rect);
	clearMatrix   = new Texture(renderer, "Textures/ClearMatrixButton.png", ui->clearMatrix->rect);
	saveState     = new Texture(renderer, "Textures/SaveStateButton.png",   ui->saveState->rect);
	loadState     = new Texture(renderer, "Textures/LoadStateButton.png",   ui->loadState->rect);
	messagePrompt = new Texture(renderer, "Textures/SavePrompt.png",        ui->messagePrompt->rect);
	plusButton	  = new Texture(renderer, "Textures/PlusButton.png",		ui->plusSize->rect);
	minusButton   = new Texture(renderer, "Textures/MinusButton.png",		ui->minusSize->rect);
}

Renderer::~Renderer() 
{
	delete ui;
	delete quitText;
	delete playPause;
	delete stepThrough;
	delete clearMatrix;
	delete saveState;
	delete loadState;
	delete messagePrompt;
	delete plusButton;
	delete minusButton;
}

SDL_Window* Renderer::GetWindow(void)
{
    return window;
}

SDL_Renderer* Renderer::get()
{
	return renderer;
}

void Renderer::Render()
{
    SDL_RenderPresent(renderer);
}

void Renderer::Clear()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void Renderer::DrawButton(Button* button)
{
    SDL_SetRenderDrawColor(renderer, button->r, button->g, button->b, button->a);
    SDL_RenderFillRect(renderer, &button->rect);
}

void Renderer::DrawButtons(void)
{ 
	DrawButton(ui->quit);
	DrawButton(ui->playPause);
	DrawButton(ui->stepThrough);
	DrawButton(ui->clearMatrix);
	DrawButton(ui->saveState);
	DrawButton(ui->loadState);
	DrawButton(ui->messagePrompt);
	DrawButton(ui->plusSize);
	DrawButton(ui->minusSize);
}

void Renderer::DrawTexture(Texture* texture)
{
	SDL_RenderCopy(renderer, texture->texture, NULL, &texture->targetRect);
}

void Renderer::DrawTextures(void)
{
	DrawTexture(quitText);
	DrawTexture(playPause);
	DrawTexture(stepThrough);
	DrawTexture(clearMatrix);
	DrawTexture(saveState);
	DrawTexture(loadState);
	if (shouldMessageDisplay) DrawTexture(messagePrompt);
	DrawTexture(plusButton);
	DrawTexture(minusButton);
}

void Renderer::DrawCells(SimulationState& simulation) {

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	
	SDL_FRect cell_rect;
	int x, y; // pixel of window (upper left corner is 0,0)
	// transform window relative pixels to a cell in the cell matrix
	// start rendering below the buttons
	for (y = BUTTON_HEIGHT; y < window_height; y += pixels_per_cell) 
	{
		int row = YPixelToCellMatrixRow(y);

		for (x = 0; x < window_width; x += pixels_per_cell)
		{
			int column = XPixelToCellMatrixColumn(x);

			if (simulation.GetCell(row, column)) // cell exists in matrix
			{
				bool cellIsAlive = *simulation.GetCell(row, column); 
				if (cellIsAlive)
				{
					cell_rect.w = pixels_per_cell;
					cell_rect.h = pixels_per_cell;
					cell_rect.x = x;
					cell_rect.y = y;

					SDL_RenderFillRectF(renderer, &cell_rect);
				}
			}
		}
	}
}

void Renderer::DrawGrid(SimulationState& simulation) {
	if (is_grid_displaying && pixels_per_cell > 1)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

		// find the top left element (beginning of cell matrix) in screen pixels
		int minXPixel = CellColumnToXPixel(0);
		int minYPixel = CellRowToYPixel(0);

		int maxXPixel = minXPixel + (simulation.columns * pixels_per_cell);
		if (maxXPixel > window_width) maxXPixel = window_width;

		int maxYPixel = minYPixel + (simulation.rows * pixels_per_cell);
		if (maxYPixel > window_height) maxYPixel = window_height;

		// draw vertical lines
		for (int x = 0; x < maxXPixel; x += pixels_per_cell)
		{
			SDL_RenderDrawLine(renderer, x, minYPixel, x, maxYPixel);
		}
		// draw horizontal lines
		for (int y = BUTTON_HEIGHT; y < maxYPixel; y += pixels_per_cell)
		{
			SDL_RenderDrawLine(renderer, minXPixel, y, maxXPixel, y);
		}
	}
}

void Renderer::DrawOrigin(void)
{
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderDrawPoint(renderer, origin.x, origin.y);
}

int Renderer::YPixelToCellMatrixRow(int y)
{
	return (y + origin.y - BUTTON_HEIGHT) / pixels_per_cell;
}
int Renderer::XPixelToCellMatrixColumn(int x)
{
	return (x + origin.x) / pixels_per_cell;
}

int Renderer::CellRowToYPixel(int row)
{
	int result = (row / pixels_per_cell) - origin.y + BUTTON_HEIGHT;
	if (result < 0)
	{
		result = BUTTON_HEIGHT;
	}
	if (result < BUTTON_HEIGHT)
	{
		result = BUTTON_HEIGHT;
	} 	else if (result > window_height + BUTTON_HEIGHT)
	{
		result = window_height + BUTTON_HEIGHT;
	}
	return result;
}
int Renderer::CellColumnToXPixel(int column)
{
	int result = (column / pixels_per_cell) - origin.x;
	if (result < 0)
	{
		result = 0;
	} else if (result > window_width)
	{
		result = window_width;
	}
	return result;
}