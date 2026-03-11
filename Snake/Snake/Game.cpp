#include "Game.h"
#include "External/console_renderer.h"

#include <chrono>
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <random>


void Game::PlayGame(const std::string& level, int fruitCount)
{
    S_GameData gameData;

    if (fruitCount == 0) gameData.fruitCount = 1;
    else gameData.fruitCount = fruitCount;
    
    StartGame(level, gameData);
    
    while (gameData.isPlaying)
    {
        UpdateInput(gameData);
        UpdateGame(gameData);
        UpdateRender(gameData);

        renderBuffer();
        std::this_thread::sleep_for(std::chrono::milliseconds(gameData.speed));
    }
    
    clearBuffer();
    Save(gameData, level);
}

void Game::StartGame(const std::string& level, S_GameData& gameData)
{
    setupCustomConsole();
    srand(time(0));

    for (int y = 0; y < gameData.mapData.height; y++)
    {
        gameData.mapData.map.push_back(std::vector<S_Tile>());

        for (int x = 0; x < gameData.mapData.width; x++)
        {
            if (x == 0 || x == gameData.mapData.width - 1)
            {
                gameData.mapData.map[y].push_back(S_Tile(gameData.mapData.roof_floor));
                gameData.mapData.map[y][x].isWall = true;
                gameData.mapData.map[y][x].xPos = y;
                gameData.mapData.map[y][x].yPos = x;
            }
            if (y == 0 && x != gameData.mapData.width - 1 || y == gameData.mapData.height - 1 && x != gameData.mapData.
                width - 1)
            {
                gameData.mapData.map[y].push_back(S_Tile(gameData.mapData.walls));
                gameData.mapData.map[y][x].isWall = true;
                gameData.mapData.map[y][x].xPos = y;
                gameData.mapData.map[y][x].yPos = x;
            }
            else
            {
                gameData.mapData.map[y].push_back(S_Tile(' '));
                gameData.mapData.map[y][x].xPos = y;
                gameData.mapData.map[y][x].yPos = x;
            }
        }
    }

    if (level == "1")
    {
        gameData.speed = 150;
    }
    else if (level == "2")
    {
        gameData.speed = 100;
    }
    else if (level == "3")
    {
        gameData.speed = 50;
    }

    for (int y = 0; y < 3; y++)
    {
        gameData.snake.push_back(S_Snake());
        if (y == 0)
        {
            gameData.snake[y].character = '@';
        }
        else gameData.snake[y].character = '+';

        gameData.snake[y].xPos = 20 + y;
        gameData.snake[y].yPos = 10;
    }

    for (int i = 0; i < gameData.fruitCount; ++i)
    {
        gameData.fruits.push_back(S_Fruit());

        gameData.fruits[i].character = 'X';

        std::random_device random1;
        std::mt19937 generator(random1());
        std::uniform_real_distribution<> distribution(1, gameData.mapData.map.size() - 1);

        int newXPos = distribution(generator);

        std::random_device random2;
        std::mt19937 generator2(random1());
        std::uniform_real_distribution<> distribution2(1, gameData.mapData.map[newXPos].size() - 2);

        int newYPos = distribution2(generator2);

        gameData.fruits[i].xPos = newXPos;
        gameData.fruits[i].yPos = newYPos;

        SetTileUnderFruit(gameData, gameData.fruits[i].xPos, gameData.fruits[i].yPos, true);
    }

    gameData.isPlaying = true;
}

void Game::UpdateInput(S_GameData& gameData)
{
    if (getIfLeftKeyIsCurrentlyDown() && gameData.inputData.horizontalDirection != 1)
    {
        gameData.inputData.horizontalDirection = -1;
        gameData.inputData.verticalDirection = 0;
    }
    else if (getIfRightKeyIsCurrentlyDown() && gameData.inputData.horizontalDirection != -1)
    {
        gameData.inputData.horizontalDirection = 1;
        gameData.inputData.verticalDirection = 0;
    }
    else if (getIfUpKeyIsCurrentlyDown() && gameData.inputData.verticalDirection != 1)
    {
        gameData.inputData.verticalDirection = -1;
        gameData.inputData.horizontalDirection = 0;
    }
    else if (getIfDownKeyIsCurrentlyDown() && gameData.inputData.verticalDirection != -1)
    {
        gameData.inputData.verticalDirection = 1;
        gameData.inputData.horizontalDirection = 0;
    }
}

void Game::UpdateGame(S_GameData& gameData)
{
    for (int i = 0; i < gameData.snake.size(); i++)
    {
        gameData.snake[i].lastXPosition = gameData.snake[i].xPos;
        gameData.snake[i].lastYPosition = gameData.snake[i].yPos;

        if (i > 0)
        {
            gameData.snake[i].xPos = gameData.snake[i - 1].lastXPosition;
            gameData.snake[i].yPos = gameData.snake[i - 1].lastYPosition;
            SetTileUnderSnake(gameData, gameData.snake[i].xPos, gameData.snake[i].yPos, true);
            SetTileUnderSnake(gameData, gameData.snake[i].lastXPosition, gameData.snake[i].lastYPosition, false);
        }
        else
        {
            gameData.snake[i].xPos += gameData.inputData.horizontalDirection;
            gameData.snake[i].yPos += gameData.inputData.verticalDirection;
            SetTileUnderHead(gameData, gameData.snake[i].xPos, gameData.snake[i].yPos, true);
            SetTileUnderHead(gameData, gameData.snake[i].lastXPosition, gameData.snake[i].lastYPosition, false);
        }
    }

    for (int i = 0; i < gameData.fruitCount; ++i)
    {
        if (gameData.fruits[i].xPos == gameData.snake[0].xPos && gameData.fruits[i].yPos == gameData.snake[0].yPos)
            gameData.fruits[i].isEaten = true;
    }

    for (int i = 0; i < gameData.fruits.size(); ++i)
    {
        if (gameData.fruits[i].isEaten)
        {
            //int newXPos = std::rand() % gameData.mapData.map.size();
            //int newYPos = std::rand() % gameData.mapData.map[newXPos].size();

            gameData.score++;

            S_Snake newSnake;

            newSnake.character = '+';

            gameData.snake[gameData.snake.size() - 1].lastXPosition = gameData.snake[gameData.snake.size() - 1].xPos;
            gameData.snake[gameData.snake.size() - 1].lastYPosition = gameData.snake[gameData.snake.size() - 1].yPos;
            
            newSnake.xPos = gameData.snake[gameData.snake.size() - 1].lastXPosition;
            newSnake.yPos = gameData.snake[gameData.snake.size() - 1].lastYPosition;

            SetTileUnderSnake(gameData, newSnake.xPos, newSnake.yPos, true);
            
            gameData.snake.push_back(newSnake);
            
            SetNewFruitPosition(gameData.fruits[i], gameData);
            
        }
    }

    for (int x = 0; x < gameData.mapData.map.size(); ++x)
    {
        for (int y = 0; y < gameData.mapData.map[y].size(); ++y)
        {
            S_Tile tile = gameData.mapData.map[x][y];

            if (gameData.snake[0].xPos == tile.xPos && gameData.snake[0].yPos == tile.yPos && tile.isWall)
            {
                gameData.isPlaying = false;
                gameData.isDead = true;
                return;
            }
            if (gameData.snake[0].xPos == tile.xPos && gameData.snake[0].yPos == tile.yPos && tile.underSnake)
            {
                gameData.isPlaying = false;
                gameData.isDead = true;
                return;
            }
        }
    }
}

void Game::UpdateRender(S_GameData& gameData)
{
    for (int i = 0; i < gameData.mapData.map.size(); ++i)
        for (int j = 0; j < gameData.mapData.map[i].size(); ++j)
            drawTile(i, j, gameData.mapData.map[i][j].character, 4);

    for (int i = 0; i < gameData.snake.size(); ++i)
    {
        drawTile(gameData.snake[i].xPos, gameData.snake[i].yPos, gameData.snake[i].character, 2);
    }

    for (int i = 0; i < gameData.fruits.size(); ++i)
    {
        drawTile(gameData.fruits[i].xPos, gameData.fruits[i].yPos, gameData.fruits[i].character, 3);
    }

    drawString(40, 0, "Score: " + std::to_string(gameData.score), FOREGROUND_GREEN);
}

void Game::Save(S_GameData& gameData, const std::string& level)
{
    if (std::ifstream("SaveData.txt"))
    {
        std::string data;
        
        std::ifstream readFile("SaveData.txt");
        
        std::vector<std::string> outPut;
        
        while (getline(readFile, data))
        {
            outPut.push_back(data);
        }

        if (level == "1") outPut[0] = "Level 1: " + std::to_string(gameData.score);
        if (level == "2") outPut[1] = "Level 2: " + std::to_string(gameData.score);
        if (level == "3") outPut[2] = "Level 3: " + std::to_string(gameData.score);
        
        std::ofstream writeFile("SaveData.txt");
        
        for (int i = 0; i < outPut.size(); ++i)
        {
            writeFile << outPut[i] << std::endl;
        }
        
        readFile.close();
        writeFile.close();
    }
    else
    {
        std::ofstream writeFile("SaveData.txt");
        
        if (level == "1")
        {
            writeFile << "Level 1: " + std::to_string(gameData.score) + "\n";
            writeFile << "Level 2: 0\n";
            writeFile << "Level 3: 0\n";
        } 
        if (level == "2")
        {
            writeFile << "Level 1: 0\n";
            writeFile << "Level 2: " + std::to_string(gameData.score) + "\n";
            writeFile << "Level 3: 0\n";
        }
        if (level == "3")
        {
            writeFile << "Level 1: 0\n";
            writeFile << "Level 2: 0\n";
            writeFile << "Level 3: " + std::to_string(gameData.score);
        }
    }
}

void Game::SetNewFruitPosition(S_Fruit& fruit, S_GameData& gameData)
{
    std::random_device random1;
    std::random_device random2;
    int newYPos = 0;
    int newXPos = 0;
    
    gameData.mapData.map[0][0].underFruit = true;
    
    while (gameData.mapData.map[newXPos][newYPos].underFruit || gameData.mapData.map[newXPos][newYPos].underSnake ||
        gameData.mapData.map[newXPos][newYPos].isUnderHead)
    {
        std::mt19937 generator(random1());
        std::uniform_real_distribution<> distribution(1, gameData.mapData.map.size() - 1);

        newXPos = distribution(generator);

        std::mt19937 generator2(random2());
        std::uniform_real_distribution<> distribution2(1, gameData.mapData.map[newXPos].size() - 2);

        newYPos = distribution2(generator2);
    }
    
    gameData.mapData.map[0][0].underFruit = false;

    SetTileUnderFruit(gameData, fruit.xPos, fruit.yPos, false);

    fruit.xPos = newXPos;
    fruit.yPos = newYPos;

    SetTileUnderFruit(gameData, fruit.xPos, fruit.yPos, true);

    fruit.isEaten = false;
}

void Game::SetTileUnderHead(S_GameData& gameData, int snakeXPos, int snakeYPos, bool should)
{
    gameData.mapData.map[snakeXPos][snakeYPos].isUnderHead = should;
}

void Game::SetTileUnderSnake(S_GameData& gameData, int snakeXPos, int snakeYPos, bool should)
{
    gameData.mapData.map[snakeXPos][snakeYPos].underSnake = should;
}

void Game::SetTileUnderFruit(S_GameData& gameData, int fruitXPos, int fruitYPos, bool should)
{
    gameData.mapData.map[fruitXPos][fruitYPos].underFruit = should;
}
