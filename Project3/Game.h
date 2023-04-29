#pragma once
#include "Board.h"

class Game {
private:
    bool exit = false;
    Board board;

    void LoadGameBoard();
    void PrintGameBoard();
    void PrintCoordinates();
    void Query();
public:
    void Run();

    void PrintGameState();

    void DoMove();
};