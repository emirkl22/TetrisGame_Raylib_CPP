#include "game.h"
#include <random>
Game::Game()
{
    grid = Grid();
    blocks = GetallBlocks();
    currentBlock = GetRandomBlock();
    nextBlock = GetRandomBlock();
    isGameOver = false;
    score = 0;
    InitAudioDevice(); // Initialize audio device for sound effects
    music = LoadMusicStream("Sounds/music.mp3"); // Load background music
    PlayMusicStream(music); // Start playing the music
    rotateSound = LoadSound("Sounds/rotate.mp3"); // Load sound for block rotation
    clearSound = LoadSound("Sounds/clear.mp3"); // Load sound for clearing rows
}
Game::~Game()
{
    UnloadSound(rotateSound); // Unload sound when the game ends
    UnloadSound(clearSound); // Unload sound when the game ends
    UnloadMusicStream(music); // Unload music when the game ends
    CloseAudioDevice(); // Close audio device when the game ends
}


Block Game::GetRandomBlock()
{
    if (blocks.empty())
    {
        blocks = GetallBlocks(); // Reset the blocks if empty
    }
    int randomIndex = rand() % blocks.size();
    Block block = blocks[randomIndex];
    blocks.erase(blocks.begin() + randomIndex);
    return block;
}

std::vector<Block> Game::GetallBlocks()
{
    return {
        LBlock(),
        JBlock(),
        IBlock(),
        OBlock(),
        SBlock(),
        TBlock(),
        ZBlock()};
}

void Game::Draw()
{
    grid.Draw();
    currentBlock.Draw(11, 11);
    switch(nextBlock.id){
        case 3:
            nextBlock.Draw(255,290);
            break;
        case 4:
            nextBlock.Draw(255,280);
            break;
        default:
            nextBlock.Draw(270, 270);
            break;
    }
}

void Game::HandleInput()
{
    int keyPressed = GetKeyPressed();
    if (isGameOver && keyPressed == KEY_R)
    {
        isGameOver = false; // Reset the game if 'R' is pressed
        Reset();
    }
    switch (keyPressed)
    {
    case KEY_LEFT:
        MoveBlockLeft();
        break;
    case KEY_RIGHT:
        MoveBlockRight();
        break;
    case KEY_DOWN:
        MoveBlockDown();
        UpdateScore(0, 1); // Add points for moving down
        break;
    case KEY_UP:
        RotateBlock();
        break;
    }
}

void Game::MoveBlockLeft()
{
    if (!isGameOver)
    {
        currentBlock.Move(0, -1);
        if (IsBlockOutside() || !BlockFits())
        {
            currentBlock.Move(0, 1); // Move back if outside
        }
    }
}

void Game::MoveBlockRight()
{
    if (!isGameOver)
    {
        currentBlock.Move(0, 1);
        if (IsBlockOutside() || !BlockFits())
        {
            currentBlock.Move(0, -1); // Move back if outside
        }
    }
}

void Game::MoveBlockDown()
{
    if (!isGameOver)
    { // Do not move if game is over
        currentBlock.Move(1, 0);
        if (IsBlockOutside() || !BlockFits())
        {
            currentBlock.Move(-1, 0); // Move back if outside
            LockBlock();
        }
    }
}

bool Game::IsBlockOutside()
{
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for (Position item : tiles)
    {
        if (grid.IsCellOutside(item.row, item.column))
        {
            return true; // If any tile is outside the grid, return true
        }
    }
    return false;
}

void Game::RotateBlock()
{
    if (!isGameOver)
    {
        currentBlock.Rotate();
        if (IsBlockOutside() || !BlockFits())
        {
            currentBlock.UndoRotate();
        }
        else{
            PlaySound(rotateSound); // Play rotation sound if the block is successfully rotated
        }
    }
}

void Game::LockBlock()
{
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for (Position item : tiles)
    {
        grid.grid[item.row][item.column] = currentBlock.id; // Lock the block in the grid
    }
    currentBlock = nextBlock; // Move to the next block
    if (!BlockFits())
    {
        isGameOver = true; // If the next block doesn't fit, game over
    }
    nextBlock = GetRandomBlock();           // Get a new next block
    int rowsCleared = grid.ClearFullRows(); // I both define the var and run a function
    if(rowsCleared > 0)
    {
        PlaySound(clearSound); // Play sound if rows are cleared
        UpdateScore(rowsCleared, 0);            // Update score based on cleared rows
    }
}

bool Game::BlockFits()
{
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for (Position item : tiles)
    {
        if (!grid.IsCellEmpty(item.row, item.column))
        {
            return false; // If any tile is outside or not empty, return false
        }
    }
    return true; // All tiles fit in the grid
}

void Game::Reset()
{
    grid.Initialize();               // Reset the grid
    currentBlock = GetRandomBlock(); // Get a new current block
    nextBlock = GetRandomBlock();    // Get a new next block
    score = 0;                       // Reset the score
}

void Game::UpdateScore(int linesCleared, int moveDownPoints)
{
    switch (linesCleared)
    {
    case 1:
        score += 100 + moveDownPoints; // 100 points for clearing one line
        break;
    case 2:
        score += 300 + moveDownPoints; // 300 points for clearing two lines
        break;
    case 3:
        score += 500 + moveDownPoints; // 500 points for clearing three lines
        break;
    default:
        break;
    }
    score += moveDownPoints; // Add points for moving down
}
