#include "Solver.h"

using namespace std;

Solver::Solver(const Board &board) : board(board) {}

unordered_set<Board> Solver::GetPossibleMoves() const {
    unordered_set<Board> possibleMoves;
    Board biggerBoard(board.size + 1);
    for(auto& line : biggerBoard.lines){
        if(line.size() < 2)
            continue;
        Coordinate firstFrom = line[0], firstTo = line[1];
        Coordinate lastTo = line[line.size() - 2], lastFrom = line[line.size() - 1];
        Board firstBoard = board, lastBoard = board;
        firstBoard.DoMove(firstFrom, firstTo);
        lastBoard.DoMove(lastFrom, lastTo);
        if(firstBoard.gameState != GameState::BadMove) {
            unordered_set<Board> possibleBoardsAfterCapture = firstBoard.PossibleBoardsAfterCapture();
            for(auto& possibleBoard : possibleBoardsAfterCapture)
                possibleMoves.insert(possibleBoard);
        }
        if(lastBoard.gameState != GameState::BadMove) {
            unordered_set<Board> possibleBoardsAfterCapture = lastBoard.PossibleBoardsAfterCapture();
            for(auto& possibleBoard : possibleBoardsAfterCapture)
                possibleMoves.insert(possibleBoard);
        }
    }
    return possibleMoves;
}

