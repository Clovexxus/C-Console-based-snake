#include "Menu.h"

#include <fstream>
#include <iostream>

void Menu::StartMenu()
{
    std::string buttons[4]
    {
        "1. Start",
        "2. Options",
        "3. Highscores",
        "4. Exit"
    };

    std::string levels[4]
    {
        "1: Level 1",
        "2: Level 2",
        "3: Level 3",
        "Other: Back"
    };
    
    int fruitCount = 0;

    while (isPlaying)
    {
        system("cls");
        std::cout.flush();
        
        std::string command = PrintAndGetInput("Snek Menu", buttons, 4);

        if (command == "1")
        {
            std::system("cls");

            const std::string level = PrintAndGetInput("Levels", levels, 4);
            
            if (std::atoi(level.c_str()) < 4 && std::atoi(level.c_str()) != 0)
            {
                currentGame = new Game;
                
                currentGame->PlayGame(level, fruitCount);
                
                delete currentGame;
            }
        }

        if (command == "2")
        {
            system("cls");
            
            std::cout << "Insert how many fruits: \n";
            
            std::string input;
            std::getline(std::cin, input);
            
            fruitCount = std::stoi(input);
            
            std::cout << "\n Press ENTER to go back...";
            
            std::string temp;
            std::getline(std::cin, temp);
        }

        if (command == "3")
        {
            std::system("cls");

            std::ifstream readFile("SaveData.txt");

            std::string data;

            std::cout << "Highscores: \n";

            while (getline(readFile, data))
            {
                std::cout << data << std::endl;
            }

            readFile.close();

            std::cout << "Press ENTER to go back...";
            
            std::string temp;
            std::getline(std::cin, temp);
        }
        
        if (command == "4") isPlaying = false;
    }
}

std::string Menu::PrintAndGetInput(const std::string& menuTitle, const std::string* menuOptions, int optionsCount)
{
    std::cout << menuTitle << std::endl;

    for (int i = 0; i < optionsCount; ++i)
    {
        std::cout << menuOptions[i] << std::endl;
    }

    std::cout << "Please enter an option: ";

    std::string userInput;
    std::getline(std::cin, userInput);
    return userInput;
}
