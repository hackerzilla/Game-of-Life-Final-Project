/*
* Author: Julian Pearson Rickenbach
* Date of Creation: 4/23/2021 6:09 PM
* Date of Last Update: 5/17/2021 7:58 AM
* Dev Log:
*	This is my CIS 27 final project.
*	Its a version of Conway's Game of Life. 
*	For this I will be using Visual Studio and
*	SDL (Simple Direct Media Layer).
*
* Program High-Level Logic:
*	Setup
*		Initialize SDL window
*		Create two cell matrices
*	Running Loop
*		Handle input
*		If the simulation is running, calculate the correct aliveness of each cell
*		Render the UI (button toolbar)
*		Render the current state of the most up-to-date cell matrix
*/

#include "SDL.h"
#include <iostream>
#include <ctime>
#include <random>
#include <cstdio>
#include <cstdlib>

#include "Utility.h"
#include "Button.h"
#include "SimulationState.h"
#include "Renderer.h"
#include "UIManager.h"
#include "InputManager.h"

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	
	//				   matrix rows / columns
	SimulationState simulation(150, 150);
	//                    window width / height
	Renderer renderer(simulation, 1920, 1080);
	InputManager input(simulation, renderer);

	while (simulation.gameIsRunning)
	{
		// ----- Input Events ------
		
		while (SDL_PollEvent(&input.event) && simulation.gameIsRunning)
		{
			input.HandleInput();
		}

		// ----- Simulation Update Logic ------
		
		if (simulation.simulationIsRunning)
		{
			simulation.UpdateCellMatrices();
		}

		// -----Render------
		
		// clear all to a black screen
		renderer.Clear();
		renderer.DrawButtons();
		renderer.DrawTextures();

		// draw all visible cells
		renderer.DrawCells(simulation);
		renderer.DrawGrid(simulation);
		// DEBUG renderer.DrawOrigin();
		
		// render everything that was "drawn" up until this point in this frame
		renderer.Render();

		if (simulation.simulationIsRunning)
		{
			SDL_Delay(renderer.SIMULATION_FRAME_DELAY);
		}
		else 
		{
			SDL_Delay(renderer.NON_SIMULATION_FRAME_DELAY);
		}

		if (simulation.simulationIsRunning)
		{
			simulation.SwapMatrices();
		}
		if (simulation.simulationIsRunning && simulation.step)
		{
			simulation.ToggleSimulation();
			simulation.step = false;
		}
	}

	simulation.SaveStateToFile("autosave");

	return 0;
}