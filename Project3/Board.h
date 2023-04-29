#pragma once
#include "Vector.h"


class Board {
private:
    Vector<char> board;
    int getIndex(int width, int height);
    int getMaxHeight();
public:
    static const char blackPawn = 'B', whitePawn = 'W', emptyTile = '_';

    int boardSize;
    int pawnsToCollect;
    int blackStartingPawns;
    int whiteStartingPawns;
    int blackPawns;
    int whitePawns;
    char currentPlayer;

    Board(int boardSize = 4, int pawnsToCollect =  4, int blackStartingPawns = 15, int whiteStartingPawns = 15, int blackPawns = 12, int whitePawns = 12, char currentPlayer = whitePawn);
    void setTile(int y, int x, char color);
    char getTile(int y, int x);
    void PrintBoard();
    void LoadGameBoard();
};
