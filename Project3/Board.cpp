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

void Board::setTile(Coordinate coordinate, char color) {
    board[getIndex(coordinate)] = color;
}

char Board::getTile(Coordinate coordinate) {
    return board[getIndex(coordinate)];
}

int Board::getIndex(Coordinate coordinate) {
    return coordinate.letter * getMaxHeight() + coordinate.number;
}

int Board::getMaxHeight() {
    return 2 * size - 1;
}

void Board::LoadGameBoard() {
    int lineLength = size; int black = 0, white = 0;
    for(int letter = 0; letter < getMaxHeight(); letter++){
        for(int number = 0; number < lineLength; number++){
            char c;
            cin >> c;
            if(c == blackPawn)
                black++;
            else if(c == whitePawn)
                white++;
            setTile(Coordinate(letter, number), c);
        }
        if(letter < size - 1)
            lineLength++;
        else
            lineLength--;
    }
    if(blackPawns + black != blackStartingPawns){
        gameState = BadMove;
        lastCommand.color = blackPawn;
        cout << "Bad input: black pawns count is not correct" << endl;
    }
    if(whitePawns + white != whiteStartingPawns){
        gameState = BadMove;
        lastCommand.color = whitePawn;
        cout << "Bad input: white pawns count is not correct" << endl;
    }
}

void Board::PrintBoard() {
    int lineLength = size;
    for(int letter = 0; letter < getMaxHeight(); letter++){
        const int spaces = getMaxHeight() - lineLength;
        for(int i = 0; i < spaces; i++)
            cout << ' ';
        for(int number = 0; number < lineLength; number++){
            cout << getTile(Coordinate(letter, number)) << ' ';
        }
        cout << endl;
        if(letter < size - 1)
            lineLength++;
        else
            lineLength--;
    }
}

void Board::PrintGameState() {
    switch (gameState) {
        case InProgress:
            cout << "in_progress" << endl;
            break;
        case BlackWon:
            cout << "black_win" << endl;
            break;
        case WhiteWon:
            cout << "white_win" << endl;
            break;
        case BadMove:
            cout << "bad_move " << lastCommand.color << " " << lastCommand.from << " " << lastCommand.to << " " << endl;
            break;
        case DeadLock:
            cout << "dead_lock " << lastCommand.color << endl;
    }
}

void Board::DoMove(Coordinate from, Coordinate to) {
    char color = currentPlayer;
    lastCommand.from = from.Increment();
    lastCommand.to = to.Increment();
    lastCommand.color = color;
    if(IsMoveValid(from, to) && MovePawns(from, to)) {
        if (currentPlayer == blackPawn)
            blackPawns--;
        else
            whitePawns--;
        currentPlayer = (currentPlayer == blackPawn) ? whitePawn : blackPawn;
    }
    else {
        gameState = BadMove;
        return;
    }
}

Vector<Coordinate> Board::GetNeighbours(Coordinate from) {
    Vector<Coordinate> neighbours(6);
    for(int letter = from.letter -1; letter <= from.letter +1; letter++){
        for(int number = from.number -1; number <= from.number + 1; number++){
            if(letter == from.letter && number == from.number)
                number++;
            Coordinate neighbour(letter, number);
            if(IsInBounds(neighbour))
                neighbours.pushBack(neighbour);
        }
    }
    return neighbours;
}

bool Board::IsMoveValid(Coordinate from, Coordinate to) {
    if(IsInBounds(from) || !IsInBounds(to)){
        return false;
    }
    Vector<Coordinate> neighbours = GetNeighbours(from);
    bool found = false;
    for(int i = 0; i < neighbours.size(); i++){
        if(neighbours[i] == to){
            found = true;
            break;
        }
    }
    if(!found)
        return false;
    return true;
}

bool Board::MovePawns(Coordinate from, Coordinate to) {
    if(!IsInBounds(to))
        return false;
    char toColor = getTile(to);
    bool isOk = true;
    if(toColor != emptyTile){
        isOk = MovePawns(to, NextCoordinate(from, to));
    }
    char fromColor = currentPlayer;
    if(IsInBounds(from)) {
        fromColor = getTile(from);
        setTile(from, emptyTile);
    }
    setTile(to, fromColor);
    return isOk;
}


bool Board::IsInBounds(Coordinate coordinate) {
    if(coordinate.letter < 0 || coordinate.letter >= getMaxHeight())
        return false;
    const int maxNumber = coordinate.letter + size - 1;
    if(coordinate.number < 0 || coordinate.number > maxNumber)
        return false;
    return true;
}

Coordinate Board::NextCoordinate(Coordinate from, Coordinate to) {
    int nextLetter;
    if(from.letter == to.letter)
        nextLetter = to.letter;
    else if(to.letter > from.letter)
        nextLetter = to.letter + 1;
    else
        nextLetter = to.letter - 1;
    int nextNumber = 2*to.number - from.number;
    int offset = nextNumber - size + 1;
    if(offset > 0)
        nextNumber -= offset;
    return Coordinate(nextLetter, nextNumber);
}

std::ostream &operator<<(ostream &os, Coordinate &c) {
    os << (char)('A' + c.letter) << c.number + 1;
    return os;
}

std::istream &operator>>(istream &is, Coordinate &c) {
    int letter = 0;
    int number = 0;
    char input;
    while(is >> input){
        if(input >= 'A' && input <= 'Z'){
            input -= 'A';
            letter = letter * 10 + input;
        }
        else if(input >= 'a' && input <= 'z'){
            input -= 'a';
            letter = letter * 10 + input;
        } else {
            is.putback(input);
            break;
        }
    }
    is >> number;
    c.letter = letter;
    c.number = number - 1;
    return is;
}

Coordinate::Coordinate(int letter, int number) {
    this->letter = letter;
    this->number = number;
}

Coordinate Coordinate::Decrement() {
    return Coordinate(letter - 1, number - 1);
}

bool Coordinate::operator==(Coordinate &other) {
    return letter == other.letter && number == other.number;
}

Coordinate Coordinate::Increment() {
    return Coordinate(letter + 1, number + 1);
}
