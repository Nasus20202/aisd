#pragma once
#include "Board.h"
#include <unordered_set>
#include <vector>

class Solver {
private:
    Board board;
public:
    Solver(const Board& board);
    [[nodiscard]] std::unordered_set<Board> GetPossibleMoves() const;
};