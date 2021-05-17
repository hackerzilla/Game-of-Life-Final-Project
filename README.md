# Game-of-Life-Final-Project
I'm making a John Conway's game of life simulation using C++ and SDL for a final project.

CIS 27 Final by Julian Pearson Rickenbach
Date of submission: 5/17/2021

Note: Canvas did not allow me to uplaod .h files, so instead I uploaded
my .h files as .txt. 

Dependencies:
Visual Studio
SDL - download here: https://www.libsdl.org/ 
SDL_image - download here: https://www.libsdl.org/projects/SDL_image/


Program Setup Procedure: 

1. New Visual Studio Solution
2. Put all source files into project directory
3. Copy SDL2 folder into project directory
4. Copy SDL2_image-2.0.5 into project directory
5. Create folder called "Textures" and place all .png files into that folder
6. Create folder called "SavedStates" and place all .txt files into that folder.
7. Goto project properties
8. Within project properies goto Configuration Properties -> VC++ Directories
9. In VC++ directories add "$(ProjectDir)\SDL2_image-2.0.5\include" to "Include Directories"
10. In VC++ directories add "$(ProjectDir)\SDL2_image-2.0.5\lib\x64" to "Library Directories"
11. Within project properties goto Configuration Properties -> C/C++ -> General
12. Within C/C++ -> General add "$(ProjectDir)\SDL2\include" and "$(ProjectDir)\SDL2_image-2.0.5\include" to
	"Additional Include Directories"
13. Within project properties goto Configuration Properties -> Linker -> General
14. In Linker -> General add "$(ProjectDir)\SDL2\lib\x64" and "$(ProjectDir)\SDL2_image-2.0.5\lib\x64" to 
	"Additional Library Directories"
15. Within project properties goto Configuration Properties -> Linker -> Input
16. In Linker -> Input add "SDL2main.lib", "SDL2.lib" and "SDL2_image.lib" to "Additional Dependencies"
17. Open the solution and press F5 to run program! Enjoy!

p.s. Feel free to email me at j.pearbach@yahoo.com if it doesn't build or execute properly.
