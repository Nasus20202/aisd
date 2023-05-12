#pragma once
#include <vector>
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
    [[nodiscard]] Coordinate Decrement() const;
    [[nodiscard]] Coordinate Increment() const;
    explicit Coordinate(int letter = 0, int number = 0);
    bool operator==(Coordinate &other) const;
    friend std::istream &operator>>(std::istream &is, Coordinate &c);
    friend std::ostream &operator<<(std::ostream &os, Coordinate &c);
};

struct Command {
    Coordinate from, to;
    char color;
};

class Board {
private:
    [[nodiscard]] int getIndex(Coordinate coordinate) const;
    std::vector<char> board;
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

    explicit Board(int boardSize = 4, int pawnsToCollect =  4, int blackStartingPawns = 15, int whiteStartingPawns = 15, int blackPawns = 12, int whitePawns = 12, char currentPlayer = whitePawn);
    void setTile(Coordinate coordinate, char color);
    char getTile(Coordinate coordinate);
    [[nodiscard]] int getMaxHeight() const;

    void DoMove(Coordinate from, Coordinate to);

    void PrintBoard();
    void PrintGameState();
    void LoadGameBoard();

    [[nodiscard]] Coordinate NextCoordinate(Coordinate from, Coordinate to) const;
    bool MovePawns(Coordinate from, Coordinate to);
    [[nodiscard]] bool IsInBounds(Coordinate coordinate) const;
    bool IsMoveValid(Coordinate from, Coordinate to);
    std::vector<Coordinate> GetNeighbours(Coordinate from);

    [[nodiscard]] std::vector<std::vector<Coordinate>> GetUpLines() const;
    [[nodiscard]] std::vector<std::vector<Coordinate>> GetStraightLines() const;
    [[nodiscard]] std::vector<std::vector<Coordinate>> GetDownLines() const;
};
