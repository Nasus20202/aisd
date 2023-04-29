#include "Board.h"
#include <iostream>

using namespace std;

Board::Board(int boardSize, int pawnsToCollect, int blackStartingPawns, int whiteStartingPawns, int blackPawns,
             int whitePawns, char currentPlayer) {
    this->size = boardSize;
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

void Board::setTile(int letter, int number, char color) {
    board[getIndex(letter, number)] = color;
}

char Board::getTile(int letter, int number) {
    return board[getIndex(letter, number)];
}

int Board::getIndex(int letter, int number) {
    return letter * getMaxHeight() + number;
}

int Board::getMaxHeight() {
    return 2 * size - 1;
}

void Board::LoadGameBoard() {
    int lineLength = size;
    for(int letter = 0; letter < getMaxHeight(); letter++){
        for(int number = 0; number < lineLength; number++){
            char c;
            cin >> c;
            setTile(letter, number, c);
        }
        if(letter < size - 1)
            lineLength++;
        else
            lineLength--;
    }
}

void Board::PrintBoard() {
    int lineLength = size;
    for(int letter = 0; letter < getMaxHeight(); letter++){
        const int spaces = getMaxHeight() - lineLength;
        for(int i = 0; i < spaces; i++)
            cout << ' ';
        for(int number = 0; number < lineLength; number++){
            cout << getTile(letter, number) << ' ';
        }
        cout << endl;
        if(letter < size - 1)
            lineLength++;
        else
            lineLength--;
    }
}
