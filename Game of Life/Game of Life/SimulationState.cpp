#include "SimulationState.h"
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

SimulationState::SimulationState(int rows, int columns)
    : rows(rows), columns(columns), 
      gameIsRunning(true), simulationIsRunning(false), step(false)
{
    // initialize cell matrices to empty 2D arrays
    currentCellMatrix = new bool* [rows];
    nextCellMatrix = new bool* [rows];
    for (int i = 0; i < rows; i++)
    {
        currentCellMatrix[i] = new bool[columns];
        nextCellMatrix[i] = new bool[columns];

        for (int j = 0; j < columns; j++)
        {
            currentCellMatrix[i][j] = false;
            nextCellMatrix[i][j] = false;
        }
    }
}

SimulationState::~SimulationState()
{
    // free memory
    for (int i = 0; i < rows; i++)
    {
        delete[] currentCellMatrix[i];
        delete[] nextCellMatrix[i];
    }
    delete[] currentCellMatrix;
    delete[] nextCellMatrix;
}

void SimulationState::SwapMatrices(void)
{
    bool** temp = currentCellMatrix;

    currentCellMatrix = nextCellMatrix;
    nextCellMatrix = temp;
}

bool* SimulationState::GetCell(int row, int column) {
    
    // check that ith and jth indices are within the array bounds
    if (row < rows - 1 && column < columns - 1)
    {
        if (simulationIsRunning)
        {
            return &nextCellMatrix[row][column];
        }
        else
        {
            return &currentCellMatrix[row][column];
        }
    }
    return nullptr; // return null if no cell exists there
}

void SimulationState::SetCell(int row, int column, bool value)
{
    nextCellMatrix[row][column] = value;
}

void SimulationState::ClearArray(bool** arr) {
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            arr[i][j] = false;
        }
    }
}

void SimulationState::ClearArrays(void)
{
    ClearArray(currentCellMatrix);
    ClearArray(nextCellMatrix);
}

int SimulationState::GetNeighborCount(int row, int column) {
    // check surrounding neighbors for existence
    unsigned int result = 0;

    //
    //   0 0 0
    //   0 X 0
    //   0 0 0
    //
    int yMax = rows - 1;
    int xMax = columns - 1;
    // top left
    if (row != 0 && column != 0)
        if (currentCellMatrix[row - 1][column - 1]) result++;

    // top
    if (row != 0)
        if (currentCellMatrix[row - 1][column]) result++;

    // top right
    if (row != 0 && column != xMax)
        if (currentCellMatrix[row - 1][column + 1]) result++;

    // right
    if (column != xMax)
        if (currentCellMatrix[row][column + 1]) result++;

    // bottom right
    if (row != yMax && column != xMax)
        if (currentCellMatrix[row + 1][column + 1]) result++;

    // bottom
    if (row != yMax)
        if (currentCellMatrix[row + 1][column]) result++;

    // bottom left
    if (row != yMax && column != 0)
        if (currentCellMatrix[row + 1][column - 1]) result++;

    // left
    if (column != 0)
        if (currentCellMatrix[row][column - 1]) result++;

    return result;
}

bool SimulationState::ShouldCellLive(int row, int column)
{
    unsigned int neighborCount = GetNeighborCount(row, column);

    bool isAlive = currentCellMatrix[row][column];
    bool shouldSurvive = neighborCount == 2 || neighborCount == 3;
    bool shouldRevive = neighborCount == 3;

    if (isAlive)
    {
        //std::cout << "cell [i,j] : [" << row << "," << column << "] n = " << neighborCount << std::endl;
        if (shouldSurvive)
        {
            return true;
        }
    } 					
    else if (!isAlive && shouldRevive)
    {
        //std::cout << "cell [i,j] : [" << row << "," << column << "] alive = false n = " << neighborCount << std::endl;

        return true;
    } 					
    else
    {
        return false;
    }
}

void SimulationState::SetCellsInRadius(int row, int column, int radius)
{
    int minRow = row - radius;
    int maxRow = row + radius;
    int minColumn = column - radius;
    int maxColumn = column + radius;

    for (int i = minRow; i <= maxRow; i++)
    {
        for (int j = minColumn; j <= maxColumn; j++)
        {
            bool* cell = GetCell(i, j);
            if (cell != nullptr)
            {
                *cell = true;
            }
        }
    }
}

void SimulationState::UpdateCellMatrices()
{
    for (unsigned int i = 0; i < rows; i++)
    {
        for (unsigned int j = 0; j < columns; j++)
        {
            bool shouldCurrentCellBeAlive = ShouldCellLive(i, j);
            SetCell(i, j, shouldCurrentCellBeAlive);
        }
    }
}

void SimulationState::SaveStateToFile(std::string filename)
{
    std::string fullFilePath = "SavedStates/" + filename + ".txt";
    cout << "\n From sim state SaveStateToFile(): fullFilePath = " << fullFilePath << "\n";
    ofstream outputFile(fullFilePath, ios::out);

    if (outputFile.is_open())
    {
        outputFile << "rows," << rows << ",columns, " << columns << ",\n";

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
            {
                outputFile << (currentCellMatrix[i][j]) << ", ";
            }
            outputFile << '\n';
        }
    }

    outputFile.close();
}

void SimulationState::LoadStateFromFile(std::string filename)
{
    std::string fullFilePath = "SavedStates/" + filename + ".txt";
    cout << "\n From sim state LoadStateFromFile(): fullFilePath = " << fullFilePath << "\n";
    ifstream inputFile(fullFilePath, ios::in);

    if (inputFile.is_open())
    {
        char temp[256];
        int newRows(0), newColumns(0);

        inputFile.getline(temp, 256, ',');
        inputFile.getline(temp, 256, ',');
        newRows = atoi(temp);

        inputFile.getline(temp, 256, ',');
        inputFile.getline(temp, 256, ',');
        newColumns = atoi(temp);

        if (newRows && newColumns)
        {
            // delete old arrays from memory
            for (int i = 0; i < rows; i++)
            {
                delete[] currentCellMatrix[i];
                delete[] nextCellMatrix[i];
            }
            delete[] currentCellMatrix;
            delete[] nextCellMatrix;

            // set rows and columns to their new values
            rows = newRows;
            columns = newColumns;

            // dynamically allocate new arrays
            currentCellMatrix = new bool* [rows];
            nextCellMatrix = new bool* [rows];
            for (int i = 0; i < rows; i++)
            {
                currentCellMatrix[i] = new bool[columns];
                nextCellMatrix[i] = new bool[columns];

                for (int j = 0; j < columns; j++)
                {
                    inputFile.getline(temp, 256, ',');
                    if (atoi(temp))
                    {
                        currentCellMatrix[i][j] = atoi(temp);
                        nextCellMatrix[i][j] = atoi(temp);
                    }
                    else
                    {
                        currentCellMatrix[i][j] = false;
                        nextCellMatrix[i][j] = false;
                    }
                }
            }
        }
    }

    inputFile.close();
}

void SimulationState::ToggleSimulation(void)
{
    simulationIsRunning = !simulationIsRunning;
}

void SimulationState::Step(void)
{
    simulationIsRunning = true;
    step = true;
}