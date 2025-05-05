//
// Created by silvi on 5/4/2025.
//

#ifndef GAME_H
#define GAME_H
#include "Player.h"

// g++ main.cpp Game.cpp Player.cpp Room.cpp Object.cpp Profile.cpp -o game.exe && .\game.exe

class Game {
    INPUT_RECORD inputRecord;
    DWORD events;

    std::vector<Profile *> profiles;
    Profile *currentProfile;

    bool running = true;
    int loc=2;

    void ConsoleGoToXY(int x=0, int y=0);
    void GetConsoleSize(int &width, int &height);
    void ClearConsoleLine();
    void PlayRoom(Slots & inv);
    void GameLoop();
    void SlowText(std::string line, int time = 30);
    void CreateProfile();
    void SelectProfile();
public:
    Game();
    ~Game();
    void Run();
};



#endif //GAME_H
