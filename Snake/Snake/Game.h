#pragma once
#include <vector>
#include <string>

struct Game
{
    
    struct S_Tile
    {
        char character;

        bool isWall;
        bool underSnake;
        bool underFruit;
        bool isUnderHead;

        int xPos;
        int yPos;
    };

    struct S_Fruit
    {
        char character;

        bool isEaten = false;

        int xPos;
        int yPos;
    };

    struct S_Snake
    {
        char character;

        int xPos;
        int yPos;

        int lastXPosition;
        int lastYPosition;
    };

    struct S_Input
    {
        int horizontalDirection = -1;
        int verticalDirection = 0;
    };

    struct S_MapData
    {
        int height = 40;
        int width = 20;
        std::vector<std::vector<S_Tile>> map;
        char roof_floor = '=';
        char walls = '|';
    };

    struct S_GameData
    {
        int speed;
        int fruitCount = 4;
        int score = 0;

        bool isPlaying;
        bool isDead = false;

        std::vector<S_Snake> snake;
        std::vector<S_Fruit> fruits;

        S_MapData mapData;
        S_Input inputData;
    };
    
public:
    void PlayGame(const std::string& level, int fruitCount);
    

private:
    void StartGame(const std::string& level, S_GameData& gameData);
    
    void UpdateInput(S_GameData& gameData);
    void UpdateGame(S_GameData& gameData);
    void UpdateRender(S_GameData& gameData);

    void Save(S_GameData& gameData, const std::string& level);
    
    void SetNewFruitPosition(S_Fruit& fruit, S_GameData& gameData);
    
    void SetTileUnderHead(S_GameData& gameData, int snakeXPos, int snakeYPos, bool should);
    void SetTileUnderSnake(S_GameData& gameData, int snakeXPos, int snakeYPos, bool should);
    void SetTileUnderFruit(S_GameData& gameData, int fruitXPos, int fruitYPos, bool should);

};
