#pragma once
#include "Board.h"

class Game {
private:
    bool exit = false;
    Board board;

    void LoadGameBoard();
    void PrintGameBoard() const;
    void PrintCoordinates() const;
    void PrintGameState() const;
    void Query();

    void DoMove();
    Board RemoveCapturedPawns(Board& nextBoard);
public:
    void Run();
};