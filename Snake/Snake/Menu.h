#pragma once
#include "Game.h"

struct Menu
{
    Game* currentGame;
    
    bool isPlaying = true;
    
public:
    void StartMenu();
    
private:
    std::string PrintAndGetInput(const std::string& menuTitle, const std::string* menuOptions, int optionsCount);
};
