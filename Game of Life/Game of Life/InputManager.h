#pragma once
#include <iostream>
#include "SDL.h"
#include "Button.h"
#include "Utility.h"
#include "SimulationState.h"
#include "Renderer.h"
#include "UIManager.h"

class InputManager
{
private:
	SimulationState& simulation;
	Renderer& renderer;
	UIManager& ui;
public:
	// structors
	InputManager(SimulationState&, Renderer&);
	~InputManager();

	// containers
	SDL_Event event;
	SDL_Point mouse_pos;
	SDL_Point start_pos;
	std::string inputFileName;

	// data
	float mouseDeltaX;
	float mouseDeltaY;
	int cellPaintbrushSize;
	
	// flags
	bool leftMouseIsHeld;
	bool rightMouseIsHeld;
	bool isEnteringFilenameForSave;
	bool isEnteringFilenameForLoad;

	// methods
	void HandleInput();
	void HandleMouseButtonDown();
	void HandleLeftMouseDown();
	void HandleRightMouseDown();
	void HandleMouseButtonUp();
	void HandleMouseMotion();
	void HandleMouseWheel();
	void HandleTextInput();
	void HandleKeyDown();
};