#pragma once
#include <vector>
#include <unordered_set>
#include <istream>

enum GameState {
    InProgress,
    BlackWon,
    WhiteWon,
    BadMove,
    DeadLock,
    InvalidInput
};


struct Coordinate {
    int letter, number;
    [[nodiscard]] Coordinate Decrement() const;
    [[nodiscard]] Coordinate Increment() const;
    explicit Coordinate(int letter = 0, int number = 0);
    bool operator==(Coordinate &other) const;
    friend std::istream &operator>>(std::istream &is, Coordinate &c);
    friend std::ostream &operator<<(std::ostream &os, const Coordinate &c);
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
    typedef std::vector<Coordinate> CoordinateLine;
    struct CaptureLine {
        CoordinateLine coordinates;
        char color;
        CaptureLine(const Board::CoordinateLine &coordinates, char color);
    };

    static const char blackCode = 'B', whiteCode = 'W', emptyCode = '_';

    int size;
    int pawnsToCollect;
    int blackStartingPawns;
    int whiteStartingPawns;
    int blackPawns;
    int whitePawns;
    int blackPawnsOnBoard = 0;
    int whitePawnsOnBoard = 0;
    char currentPlayer;
    GameState gameState = InProgress;
    Command lastCommand;

    explicit Board(int boardSize = 4, int pawnsToCollect =  4, int blackStartingPawns = 15, int whiteStartingPawns = 15, int blackPawns = 12, int whitePawns = 12, char currentPlayer = whiteCode);
    void SetTile(Coordinate coordinate, char color);
    [[nodiscard]] char GetTile(Coordinate coordinate) const;
    [[nodiscard]] int GetMaxHeight() const;
    void PushPawns(Coordinate from, Coordinate to);


    void PrintBoard() const;
    void PrintGameState() const;
    void LoadGameBoard();

    bool operator==(const Board &other) const;

    [[nodiscard]] Coordinate NextCoordinate(Coordinate from, Coordinate to) const;
    bool MovePawns(Coordinate from, Coordinate to);
    void DoMove(Coordinate from, Coordinate to);
    std::unordered_set<Board> PossibleBoardsAfterCapture();
    [[nodiscard]] bool IsInBounds(Coordinate coordinate) const;
    bool IsMoveValid(Coordinate from, Coordinate to) const;
    void RemovePawn(Coordinate coordinate, char lineColor = emptyCode);
    void RemoveCaptureLine(const Board::CaptureLine &captureLine);

    [[nodiscard]] std::vector<Coordinate> GetNeighbours(Coordinate from) const;
    // Generate all possible straight lines on the board
    [[nodiscard]] std::vector<CoordinateLine> GetUpLines() const;
    [[nodiscard]] std::vector<CoordinateLine> GetStraightLines() const;
    [[nodiscard]] std::vector<CoordinateLine> GetDownLines() const;
    [[nodiscard]] std::vector<CoordinateLine> GetLines() const;

    // Count how many capture lines each pawn is in
    [[nodiscard]] std::vector<CaptureLine> GetCaptureLines() const;


private: // helpers
    void FillSetWithPossibleBoards(Board &currentBoard, std::unordered_set<Board> &boardsSet);

    friend std::hash<Board>;
};

// https://stackoverflow.com/questions/20511347/a-good-hash-function-for-a-vector
template<>
struct std::hash<std::vector<char>> {
    std::size_t operator()(const std::vector<char> &board) const {
        std::size_t seed = board.size();
        for(auto &i : board)
            seed ^= hash<char>()(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};

template<>
struct std::hash<Board> {
    std::size_t operator()(const Board &board) const {
        return hash<int>()(board.size) ^
               hash<int>()(board.pawnsToCollect) ^
               hash<int>()(board.blackStartingPawns) ^
               hash<int>()(board.whiteStartingPawns) ^
               hash<int>()(board.blackPawns) ^
               hash<int>()(board.whitePawns) ^
               hash<char>()(board.currentPlayer) ^
               hash<std::vector<char>>()(board.board);
    }
};