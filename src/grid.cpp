#include "grid.h"
#include <iostream>
#include "colors.h"

Grid :: Grid(){
    numRows = 20;
    numCols = 10;
    cellSize = 30;
    Initialize();
    Print();
    colors = GetCellColors();
}

void Grid :: Initialize(){
    for (int row = 0; row<numRows; row++){
        for(int column = 0; column<numCols; column++){
            grid[row][column] = 0;
        }
    }
}

void Grid :: Print(){
    for (int row = 0; row<numRows; row++){
        for(int column = 0; column<numCols; column++){
            std::cout << " " << grid[row][column]; 
        }
        std::cout << std::endl;
    }
}

void Grid::Draw(){
    for (int row = 0; row < numRows; row++){
        for(int column = 0; column<numCols; column++){
            int cellValue = grid[row][column];
            DrawRectangle(column*cellSize + 11, row*cellSize + 11, cellSize -1, cellSize -1, colors[cellValue]);
        }
        std::cout << std::endl;
    }
}

bool Grid::IsCellOutside(int row, int column)
{
    if(row >= 0 && row < numRows && column >= 0 && column < numCols){
        return false; // Cell is inside the grid
    } 
    return true; // Cell is outside the grid
}

bool Grid::IsCellEmpty(int row, int column)
{
    if(grid[row][column] == 0){
        return true; // Cell is empty
    }
    return false; // Cell is not empty
}

bool Grid::IsRowFull(int row)
{
    for(int column = 0; column < numCols; column++){
        if(grid[row][column] == 0){
            return false; // If any cell in the row is empty, the row is not full
        }
    }
    return true;
}

void Grid::ClearRow(int row)
{
    for(int column = 0; column < numCols; column++){
        grid[row][column] = 0; // Clear the row by setting all cells to 0
    }
}

void Grid::MoveRowDown(int row, int numRows)
{
    for(int column = 0; column < numCols; column++){
        grid[row + numRows][column] = grid[row][column]; // Move the row down by copying its values
        grid[row][column] = 0; // Clear the original row
    }
}

int Grid::ClearFullRows()
{
    int completed = 0;
    for(int row = numRows - 1; row >= 0; row--){
        if(IsRowFull(row)){
            ClearRow(row); // Clear the full row
            completed++; // Increment the count of completed rows
        }
        else if(completed > 0){
            MoveRowDown(row, completed); // Move the row down if there are completed rows above it
        }
    }
    return completed;
}

