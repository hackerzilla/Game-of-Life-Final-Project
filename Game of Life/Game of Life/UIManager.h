#pragma once

#include "Button.h"
#include "Utility.h"
#include "SDL.h"

class UIManager
{
public:
    UIManager(int window_width);
    ~UIManager();

    Button* toolbar;
    Button* quit;
    Button* playPause;
    Button* stepThrough;
    Button* clearMatrix;
    Button* saveState;
    Button* loadState;
    Button* messagePrompt;
    Button* plusSize;
    Button* minusSize;

    // data
    const unsigned int BUTTON_HEIGHT = 64;
    const unsigned int BUTTON_WIDTH = 128;
};