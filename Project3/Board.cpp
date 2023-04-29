#include "Board.h"
#include <iostream>

using namespace std;

Board::Board(int boardSize, int pawnsToCollect, int blackStartingPawns, int whiteStartingPawns, int blackPawns,
             int whitePawns, char currentPlayer) {
    this->boardSize = boardSize;
    this->pawnsToCollect = pawnsToCollect;
    this->blackStartingPawns = blackStartingPawns;
    this->whiteStartingPawns = whiteStartingPawns;
    this->blackPawns = blackPawns;
    this->whitePawns = whitePawns;
    this->currentPlayer = currentPlayer;
    const int maxH = getMaxHeight();
    board.resize(maxH*maxH);
    char c = emptyTile;
    for(int i = 0; i < maxH*maxH; i++)
        board.pushBack(c);
}

void Board::setTile(int y, int x, char color) {
    board[getIndex(y, x)] = color;
}

char Board::getTile(int y, int x) {
    return board[getIndex(y, x)];
}

int Board::getIndex(int y, int x) {
    return y*getMaxHeight()+x;
}

int Board::getMaxHeight() {
    return 2*boardSize-1;
}

void Board::LoadGameBoard() {
    int lineLength = boardSize;
    for(int line = 0; line < getMaxHeight(); line++){
        for(int i = 0; i < lineLength; i++){
            char c;
            cin >> c;
            setTile(line, i, c);
        }
        if(line < boardSize-1)
            lineLength++;
        else
            lineLength--;
    }
}

void Board::PrintBoard() {
    int lineLength = boardSize;
    for(int line = 0; line < getMaxHeight(); line++){
        const int spaces = getMaxHeight() - lineLength;
        for(int i = 0; i < spaces; i++)
            cout << ' ';
        for(int i = 0; i < lineLength; i++){
            cout << getTile(line, i) << ' ';
        }
        cout << endl;
        if(line < boardSize-1)
            lineLength++;
        else
            lineLength--;
    }
}
