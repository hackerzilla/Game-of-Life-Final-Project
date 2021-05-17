#include "UIManager.h"

UIManager::UIManager(int window_width)
{
	// buttons      position:                  x  y   	       width         height    red green blue alpha
	toolbar       = new Button(                0, 0,     window_width, BUTTON_HEIGHT,   0,   0,   0,   0);
	quit          = new Button(                0, 0,     BUTTON_WIDTH, BUTTON_HEIGHT, 255, 123, 123, 255);
	playPause     = new Button(     BUTTON_WIDTH, 0,     BUTTON_WIDTH, BUTTON_HEIGHT, 123, 255, 123, 255);
	stepThrough   = new Button(2  * BUTTON_WIDTH, 0,     BUTTON_WIDTH, BUTTON_HEIGHT, 123, 123, 123, 255);
	clearMatrix   = new Button(3  * BUTTON_WIDTH, 0,     BUTTON_WIDTH, BUTTON_HEIGHT, 255, 128,   0, 255);
	saveState     = new Button(4  * BUTTON_WIDTH, 0,     BUTTON_WIDTH, BUTTON_HEIGHT, 127,   0, 255, 255);
	loadState     = new Button(5  * BUTTON_WIDTH, 0,     BUTTON_WIDTH, BUTTON_HEIGHT,   0, 127, 255, 255);
	messagePrompt = new Button(6  * BUTTON_WIDTH, 0, 3 * BUTTON_WIDTH, BUTTON_HEIGHT,   0,   0,   0, 255);
	plusSize      = new Button(9  * BUTTON_WIDTH, 0,     BUTTON_WIDTH, BUTTON_HEIGHT, 200,  10,  10, 255);
	minusSize	  = new Button(10 * BUTTON_WIDTH, 0,	 BUTTON_WIDTH, BUTTON_HEIGHT,  10,  10, 200, 255);
}

UIManager::~UIManager()
{
	delete toolbar;
	delete quit;
	delete playPause;
	delete stepThrough;
	delete clearMatrix;
	delete saveState;
	delete loadState;
	delete messagePrompt;
	delete plusSize;
	delete minusSize;
}