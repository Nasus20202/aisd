#pragma once
#include "Board.h"

class Game {
private:
    bool exit = false;
    Board board;

    // Game
    void LoadGameBoard();
    void PrintGameBoard() const;
    void PrintCoordinates() const;
    void PrintGameState() const;
    void Query();
    void DoMove();
    MoveStatus RemoveCapturedPawns();

    // Solver
    void GenerateAllPossibleMoves();
    void GenerateAllPossibleMovesExtended();
    void GenerateAllPossibleMovesNumber();
    void GenerateAllPossibleMovesNumberExtended();
public:
    void Run();
};