#pragma once
#include <string>

class SimulationState
{
private:
    bool** currentCellMatrix;
    bool** nextCellMatrix;
public:
    // structors
    SimulationState(int rows, int columns);
    ~SimulationState();

    // data
    unsigned int rows;
    unsigned int columns;
    
    // flags
    bool gameIsRunning;
    bool simulationIsRunning;
    bool step;

    // methods
    bool* GetCell(int yPixel, int xPixel);
    void SetCell(int row, int column, bool value);
    void ClearArray(bool** arr);
    void ClearArrays(void);
    int GetNeighborCount(int row, int column);
    bool ShouldCellLive(int row, int column);
    void SetCellsInRadius(int row, int column, int radius);

    void UpdateCellMatrices();
    void SwapMatrices(void);
    void SaveStateToFile(std::string filename);
    void LoadStateFromFile(std::string filename);
    void ToggleSimulation(void);
    void Step(void);
};