#include "InputManager.h"
using namespace std;

InputManager::InputManager(SimulationState& simulation, Renderer& renderer)
    : simulation(simulation), renderer(renderer), ui(*renderer.ui),
	mouseDeltaX(0), mouseDeltaY(0), event(),
	mouse_pos(), start_pos(), cellPaintbrushSize(0), leftMouseIsHeld(false),
	rightMouseIsHeld(false), 
	isEnteringFilenameForLoad(false), isEnteringFilenameForSave(false)
{}

InputManager::~InputManager(){}

void InputManager::HandleInput()
{
    switch (event.type)
    {
    case SDL_MOUSEBUTTONDOWN:
        HandleMouseButtonDown();
        break;
    case SDL_MOUSEBUTTONUP:
        HandleMouseButtonUp();
        break;
    case SDL_MOUSEMOTION:
        HandleMouseMotion();
        break;
    case SDL_MOUSEWHEEL:
        HandleMouseWheel();
        break;
	case SDL_TEXTINPUT:
		HandleTextInput();
		break;
	case SDL_KEYDOWN:
		HandleKeyDown();
		break;
    default:
        break;
    }
}

void InputManager::HandleMouseButtonDown()
{
	// get mouse location in pixels ; (0,0) is top left pixel of window
	SetPoint(&mouse_pos, event.button.x, event.button.y);

	// start mouse hold
	if (event.button.button == SDL_BUTTON_LEFT)
	{
		HandleLeftMouseDown();
	}
	else if (event.button.button == SDL_BUTTON_RIGHT)
	{
		HandleRightMouseDown();
	}
}

void InputManager::HandleLeftMouseDown()
{
	leftMouseIsHeld = true;

	if (SDL_PointInRect(&mouse_pos, &ui.quit->rect))
	{
		SDL_Log("Quit Button pressed, quiting!");
		simulation.gameIsRunning = false;
	}
	else if (SDL_PointInRect(&mouse_pos, &ui.playPause->rect))
	{
		SDL_Log("Pause/play button pressed, toggling simulation on/off!");
		simulation.ToggleSimulation();
	}
	else if (SDL_PointInRect(&mouse_pos, &ui.stepThrough->rect))
	{
		SDL_Log("Step through button pressed, running simulation for one step.");
		simulation.Step();
	}
	else if (SDL_PointInRect(&mouse_pos, &ui.clearMatrix->rect))
	{
		SDL_Log("Clear button pressed, clearing the simulation space!");
		simulation.ClearArrays();
	}
	else if (SDL_PointInRect(&mouse_pos, &ui.saveState->rect))
	{
		SDL_Log("Save button pressed, allowing text input.");
		inputFileName.clear();
		SDL_StartTextInput();
		isEnteringFilenameForSave = true;
		renderer.shouldMessageDisplay = true;
	}
	else if (SDL_PointInRect(&mouse_pos, &ui.loadState->rect))
	{
		SDL_Log("Load button pressed, allowing text input.");
		inputFileName.clear();
		SDL_StartTextInput();
		isEnteringFilenameForLoad = true;
		renderer.shouldMessageDisplay = true;
	}
	else if (SDL_PointInRect(&mouse_pos, &ui.messagePrompt->rect))
	{
		// nothing for now
	}
	else if (SDL_PointInRect(&mouse_pos, &ui.plusSize->rect))
	{
		SDL_Log("Plus button pressed, increasing paint brush size.");
		if (cellPaintbrushSize < 10)
		{
			cellPaintbrushSize++;
		}
	}
	else if (SDL_PointInRect(&mouse_pos, &ui.minusSize->rect))
	{
		SDL_Log("Minus button pressed, decreasing paint brush size.");
		if (cellPaintbrushSize > 0)
		{
			cellPaintbrushSize--;
		}
	}
	else if (!simulation.simulationIsRunning)
	{
		// cell "painting" mode
		int row = renderer.YPixelToCellMatrixRow(mouse_pos.y);
		int column = renderer.XPixelToCellMatrixColumn(mouse_pos.x);
		simulation.SetCellsInRadius(row, column, cellPaintbrushSize);
	}
}

void InputManager::HandleRightMouseDown()
{
	rightMouseIsHeld = true;
	SDL_SetRelativeMouseMode(SDL_TRUE);
}

void InputManager::HandleMouseButtonUp()
{
	// end mouse hold
	if (event.button.button == SDL_BUTTON_LEFT)
	{
		leftMouseIsHeld = false;
	} 	
	else if (event.button.button == SDL_BUTTON_RIGHT)
	{
		rightMouseIsHeld = false;
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}
}

void InputManager::HandleMouseMotion()
{
	SetPoint(&mouse_pos, event.button.x, event.button.y);
	
	// panning the camera
	if (rightMouseIsHeld)
	{
		mouseDeltaX = -event.motion.xrel;
		mouseDeltaY = -event.motion.yrel;
		renderer.origin.x += mouseDeltaX;
		renderer.origin.y += mouseDeltaY;
	}
	// cell "painting" mode
	// this will crash if it doesn't filter out the condition of the mouse being on top of a button when released
	else if (!(SDL_PointInRect(&mouse_pos, &ui.toolbar->rect)) && leftMouseIsHeld)
	{
		// this should read more like:
		// Cell cell = simualation.GetCell(GetWorldPointFromScreenPixel(mouse_pos)); 
		// this will require refactoring some x, y pairs into Point objects
		int row = renderer.YPixelToCellMatrixRow(mouse_pos.y);
		int column = renderer.XPixelToCellMatrixColumn(mouse_pos.x);
		simulation.SetCellsInRadius(row, column, cellPaintbrushSize);
	}
}

void InputManager::HandleMouseWheel()
{
	int sign = signum(event.wheel.y);
	bool cannotDecreasePPC = sign == -1 && renderer.pixels_per_cell <= renderer.min_ppc;
	bool cannotIncreasePPC = sign == 1 && renderer.pixels_per_cell >= renderer.max_ppc;

	if (!cannotDecreasePPC && !cannotIncreasePPC)
	{
		// increment or decrement pixels_per_cell
		int ppc_change = sign;

		int old_pixels_per_cell = renderer.pixels_per_cell;
		renderer.pixels_per_cell += ppc_change;

		
		// need to change the origin so that the screen stays somewhat centered on the simulation
		SDL_FPoint originChange;
		
		originChange.x = (simulation.columns / 2.0f) * renderer.pixels_per_cell - (simulation.columns / 2.0f) * old_pixels_per_cell;
		originChange.y = (simulation.rows / 2.0f) * renderer.pixels_per_cell - (simulation.rows / 2.0f) * old_pixels_per_cell;
		
		renderer.origin.x += originChange.x;
		renderer.origin.y += originChange.y;


		// clamp the pixels_per_cell variable to a reasonable range 
		if (renderer.pixels_per_cell < renderer.min_ppc)
		{
			renderer.pixels_per_cell = renderer.min_ppc;
		} 	else if (renderer.pixels_per_cell > renderer.max_ppc)
		{
			renderer.pixels_per_cell = renderer.max_ppc;
		}
	}
}

void InputManager::HandleTextInput()
{
	SDL_Log("Text input received.");
	char* input = event.text.text;
	if (input)
	{
		inputFileName.push_back(*input);
	}
}

void InputManager::HandleKeyDown()
{
	switch (event.key.keysym.sym)
	{
	case SDLK_RETURN: // enter
		if (SDL_IsTextInputActive())
		{
			if (isEnteringFilenameForSave)
			{
				cout << "\nEnter pressed, stopping text input and saving simulation to file: " << inputFileName << "\n";
				SDL_StopTextInput();
				simulation.SaveStateToFile(inputFileName);
				isEnteringFilenameForSave = false;
				renderer.shouldMessageDisplay = false;
			}
			else if (isEnteringFilenameForLoad)
			{
				cout << "\nEnter pressed, stopping text input and loading simulation from file: " << inputFileName << "\n";
				SDL_StopTextInput();
				simulation.LoadStateFromFile(inputFileName);
				isEnteringFilenameForLoad = false;
				renderer.shouldMessageDisplay = false;
			}
		}
		break;
	default:
		break;
	}
}