#pragma once
#include "Vector.h"


class Board {
private:
    int getIndex(int letter, int number);
    Vector<char> board;
public:
    static const char blackPawn = 'B', whitePawn = 'W', emptyTile = '_';

    int size;
    int pawnsToCollect;
    int blackStartingPawns;
    int whiteStartingPawns;
    int blackPawns;
    int whitePawns;
    char currentPlayer;

    Board(int boardSize = 4, int pawnsToCollect =  4, int blackStartingPawns = 15, int whiteStartingPawns = 15, int blackPawns = 12, int whitePawns = 12, char currentPlayer = whitePawn);
    void setTile(int letter, int number, char color);
    char getTile(int letter, int number);
    void PrintBoard();
    void LoadGameBoard();
    int getMaxHeight();
};
