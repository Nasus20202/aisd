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
        board.push_back(c);
}

void Board::setTile(Coordinate coordinate, char color) {
    board[getIndex(coordinate)] = color;
}

char Board::getTile(Coordinate coordinate) {
    return board[getIndex(coordinate)];
}

int Board::getIndex(Coordinate coordinate) const {
    return coordinate.letter * getMaxHeight() + coordinate.number;
}

int Board::getMaxHeight() const {
    return 2 * size - 1;
}

void Board::LoadGameBoard() {
    int lineLength = size;
    for(int letter = 0; letter < getMaxHeight(); letter++){
        for(int number = 0; number < lineLength; number++){
            char c;
            cin >> c;
            setTile(Coordinate(letter, number), c);
        }
        if(letter < size - 1)
            lineLength++;
        else
            lineLength--;
    }
}

void Board::PrintBoard() {
    int lineLength = size;
    vector<vector<Coordinate>> coordinates = GetStraightLines();
    for(int line = 0; line < coordinates.size(); line++){
        const int spaces = getMaxHeight() - lineLength;
        for(int i = 0; i < spaces; i++)
            cout << ' ';
        for(auto &coordinate : coordinates[line])
            cout << getTile(coordinate) << ' ';
        cout << endl;
        if(line < size - 1)
            lineLength++;
        else
            lineLength--;
    }
    //vector<vector<Coordinate>>
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

vector<Coordinate> Board::GetNeighbours(Coordinate from) {
    vector<Coordinate> neighbours(6);
    vector<Coordinate> possibleNeighbours(6);
    possibleNeighbours.emplace_back(from.letter, from.number - 1); // left
    possibleNeighbours.emplace_back(from.letter, from.number + 1); // right
    if(from.letter < size - 1){
        possibleNeighbours.emplace_back(from.letter - 1, from.number - 1 ); // top left
        possibleNeighbours.emplace_back(from.letter - 1, from.number); // top right
        possibleNeighbours.emplace_back(from.letter + 1, from.number); // bottom left
        possibleNeighbours.emplace_back(from.letter + 1, from.number + 1); // bottom right
    }
    else if(from.letter == size - 1){
        possibleNeighbours.emplace_back(from.letter - 1, from.number - 1 ); // top left
        possibleNeighbours.emplace_back(from.letter - 1, from.number); // top right
        possibleNeighbours.emplace_back(from.letter + 1, from.number - 1); // bottom left
        possibleNeighbours.emplace_back(from.letter + 1, from.number); // bottom right
    }
    else {
        possibleNeighbours.emplace_back(from.letter - 1, from.number); // top left
        possibleNeighbours.emplace_back(from.letter - 1, from.number + 1); // top right
        possibleNeighbours.emplace_back(from.letter + 1, from.number - 1); // bottom left
        possibleNeighbours.emplace_back(from.letter + 1, from.number); // bottom right
    }
    for(auto possibleNeighbour : possibleNeighbours){
        if(IsInBounds(possibleNeighbour))
            neighbours.push_back(possibleNeighbour);
    }
    return neighbours;
}

bool Board::IsMoveValid(Coordinate from, Coordinate to) {
    if(IsInBounds(from))
        return false;
    if(!IsInBounds(to))
        return false;
    vector<Coordinate> neighbours = GetNeighbours(from);
    bool found = false;
    for(auto & neighbour : neighbours){
        if(neighbour == to){
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
    if(toColor != emptyTile)
        isOk = MovePawns(to, NextCoordinate(from, to));
    char fromColor = currentPlayer;
    if(IsInBounds(from)) {
        fromColor = getTile(from);
        setTile(from, emptyTile);
    }
    setTile(to, fromColor);
    return isOk;
}

bool Board::IsInBounds(Coordinate coordinate) const {
    if(coordinate.letter < 0 || coordinate.letter >= getMaxHeight())
        return false;
    int distanceFromCenter = coordinate.letter - (size - 1);
    if(distanceFromCenter < 0)
        distanceFromCenter *= -1;
    int maxNumber = getMaxHeight() - distanceFromCenter - 1;
    if(coordinate.number < 0 || coordinate.number > maxNumber)
        return false;
    return true;
}

Coordinate Board::NextCoordinate(Coordinate from, Coordinate to) const {
    int nextLetter = 2*to.letter-from.letter, nextNumber = 0;
    // Calculate next number based on the direction
    if(from.letter == to.letter) // same level
        nextNumber = 2*to.number - from.number;
    else if(from.letter < to.letter) // going down
    {
        if(to.letter < size - 1){ // top part
            if(to.number == from.number){ // going down left
                nextNumber = to.number;
            }
            else{
                nextNumber = to.number + 1; // going down right
            }
        }
        else if (to.letter >= size){ // bottom part
            if(to.number == from.number){ // going down right
                nextNumber = to.number;
            } else {
                nextNumber = to.number - 1; // going down left
            }
        }
        else{ // middle part
            if(to.number == from.number){ // going down left
                nextNumber = to.number - 1;
            } else {
                nextNumber = to.number; // going down right
            }
        }
    } else { // going top
        if(to.letter < size - 1) { // top part
            if(to.number == from.number){ // going top right
                nextNumber = to.number;
            } else {
                nextNumber = to.number - 1; // going top left
            }
        }
        else if(to.letter >= size){ // bottom part
            if(to.number == from.number){ // going top left
                nextNumber = to.number;
            } else {
                nextNumber = to.number + 1; // going top right
            }
        } else { // middle part
            if(to.number == from.number){ // going top left
                nextNumber = to.number - 1;
            } else {
                nextNumber = to.number; // going top left
            }
        }
    }
    return Coordinate(nextLetter, nextNumber);
}


vector<vector<Coordinate>> Board::GetUpLines() const {
    vector<vector<Coordinate>> lines;
    for(int i = 0; i < getMaxHeight(); i++){
        Coordinate from(i, -1);
        Coordinate to(i, 0);
        vector<Coordinate> line;
        while(IsInBounds(to)){
            line.push_back(to);
            Coordinate next = NextCoordinate(from, to);
            from = to;
            to = next;
        }
        lines.push_back(line);
    }
    return lines;
}

vector<vector<Coordinate>> Board::GetStraightLines() const {
    vector<vector<Coordinate>> lines;
    for(int i = size-1; i > 0; i--){
        Coordinate from(-1, i-1);
        Coordinate to(0, i);
        vector<Coordinate> line;
        while(IsInBounds(to)){
            line.push_back(to);
            Coordinate next = NextCoordinate(from, to);
            from = to;
            to = next;
        }
        lines.push_back(line);
    }
    for(int i = 0; i < size; i++){
        Coordinate from(i-1, -1);
        Coordinate to(i, 0);
        vector<Coordinate> line;
        while(IsInBounds(to)){
            line.push_back(to);
            Coordinate next = NextCoordinate(from, to);
            from = to;
            to = next;
        }
        lines.push_back(line);
    }
    return lines;
}

vector<vector<Coordinate>> Board::GetDownLines() const {
    vector<vector<Coordinate>> lines;
    for(int i = 0; i < size; i++){
        Coordinate from(-1, i);
        Coordinate to(0, i);
        vector<Coordinate> line;
        while(IsInBounds(to)){
            line.push_back(to);
            Coordinate next = NextCoordinate(from, to);
            from = to;
            to = next;
        }
        lines.push_back(line);
    }
    for(int i = 1; i < size; i++){
        Coordinate from(i-1, size+i-1);
        Coordinate to(i, size+i-1);
        vector<Coordinate> line;
        while(IsInBounds(to)){
            line.push_back(to);
            Coordinate next = NextCoordinate(from, to);
            from = to;
            to = next;
        }
        lines.push_back(line);
    }
    return lines;
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

Coordinate Coordinate::Decrement() const {
    return Coordinate(letter - 1, number - 1);
}

bool Coordinate::operator==(Coordinate &other) const {
    return letter == other.letter && number == other.number;
}

Coordinate Coordinate::Increment() const {
    return Coordinate(letter + 1, number + 1);
}
