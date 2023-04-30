#pragma once
#include "Vector.h"
#include <istream>

enum GameState {
    InProgress,
    BlackWon,
    WhiteWon,
    BadMove,
    DeadLock
};


struct Coordinate {
    int letter, number;
    Coordinate Decrement();
    Coordinate Increment();
    Coordinate(int letter = 0, int number = 0);
    bool operator==(Coordinate &other);
    friend std::istream &operator>>(std::istream &is, Coordinate &c);
    friend std::ostream &operator<<(std::ostream &os, Coordinate &c);
};

struct Command {
    Coordinate from, to;
    char color;
};

class Board {
private:
    int getIndex(Coordinate coordinate);
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
    GameState gameState = InProgress;
    Command lastCommand;

    Board(int boardSize = 4, int pawnsToCollect =  4, int blackStartingPawns = 15, int whiteStartingPawns = 15, int blackPawns = 12, int whitePawns = 12, char currentPlayer = whitePawn);
    void setTile(Coordinate coordinate, char color);
    char getTile(Coordinate coordinate);
    int getMaxHeight();

    void DoMove(Coordinate from, Coordinate to);

    void PrintBoard();
    void PrintGameState();
    void LoadGameBoard();

    Coordinate NextCoordinate(Coordinate from, Coordinate to);
    bool MovePawns(Coordinate from, Coordinate to);
    bool IsInBounds(Coordinate coordinate);
    bool IsMoveValid(Coordinate from, Coordinate to);
    Vector<Coordinate> GetNeighbours(Coordinate from);

    Vector<Vector<Coordinate>> GetStraightLines();
    Vector<Vector<Coordinate>> GetLeftToRightDiagonalLines();
    Vector<Vector<Coordinate>> GetRightToLeftDiagonalLines();
};