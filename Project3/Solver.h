#pragma once
#include "Board.h"
#include <unordered_set>
#include <vector>

class Solver {
private:
    Board board;
public:
    explicit Solver(const Board& board);
    [[nodiscard]] std::unordered_set<Board> GetPossibleMoves() const;
};