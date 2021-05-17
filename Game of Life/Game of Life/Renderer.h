#pragma once
#include "SDL.h"
#include "Button.h"
#include "Texture.h"
#include "SimulationState.h"
#include "UIManager.h"

class Renderer
{
private:
    SDL_Window* window;
    SDL_Renderer* renderer;   

public:
    // structors
    Renderer(SimulationState& simulation, int width, int height);
    ~Renderer();

    // containers
    UIManager* ui;
    Texture* quitText;
    Texture* playPause;
    Texture* stepThrough;
    Texture* clearMatrix;
    Texture* saveState;
    Texture* loadState;
    Texture* messagePrompt;
    Texture* plusButton;
    Texture* minusButton;

    // data
    const unsigned int NON_SIMULATION_FRAME_DELAY = 10;
    const unsigned int SIMULATION_FRAME_DELAY = 100;
    const unsigned int BUTTON_HEIGHT = 64;
    const unsigned int BUTTON_WIDTH = 128;
    unsigned int min_ppc;
    unsigned int max_ppc;

    int window_width;
    int window_height;
    SDL_Point origin;
    unsigned int pixels_per_cell;
    
    
    // flags
    bool is_grid_displaying = true;
    bool shouldMessageDisplay = false;

    // methods
    SDL_Window* GetWindow(void);
    SDL_Renderer* get(void);

    void Render(void);
    void Clear(void);

    void DrawButton(Button* button);
    void DrawButtons(void);
    void DrawTexture(Texture* texture);
    void DrawTextures();
    void DrawCells(SimulationState& simulation);
    void DrawGrid(SimulationState& simulation);
    void DrawOrigin(void);

    int YPixelToCellMatrixRow(int y);
    int XPixelToCellMatrixColumn(int x);
    int CellRowToYPixel(int row);
    int CellColumnToXPixel(int column);
    
};