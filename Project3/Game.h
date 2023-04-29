#pragma once
#include "Board.h"

class Game {
private:
    bool exit = false;
    Board board;

    void LoadGameBoard();
    void PrintGameBoard();
    void Query();
public:
    void Run();
};