#include "Board.h"
#include <iostream>
#include <string>
#include <utility>

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
    const int maxH = GetMaxHeight();
    board.resize(maxH*maxH);
    char c = emptyCode;
    for(int i = 0; i < maxH*maxH; i++)
        board.push_back(c);
}

void Board::SetTile(Coordinate coordinate, char color) {
    board[getIndex(coordinate)] = color;
}

char Board::GetTile(Coordinate coordinate) const {
    return board[getIndex(coordinate)];
}

int Board::getIndex(Coordinate coordinate) const {
    return coordinate.letter * GetMaxHeight() + coordinate.number;
}

int Board::GetMaxHeight() const {
    return 2 * size - 1;
}

void Board::LoadGameBoard() {
    int lineLength = size; blackPawnsOnBoard = 0; whitePawnsOnBoard = 0;
    vector<vector<Coordinate>> coordinates = GetStraightLines();
    int rows = 0;
    for(int line = 0; line < coordinates.size(); line++){
        int columns = 0;
        char color = ' '; bool tooLong = false;
        while(color == ' ' || color == '\n')
            color = (char) cin.get();
        while(true) {
            if(color == '\n') {
                rows++; break;
            }
            while (color == ' ') {
                color = (char) cin.get();
            }
            if(columns == lineLength) {
                tooLong = true;
                break;
            }
            if(color == whiteCode)
                whitePawnsOnBoard++;
            else if(color == blackCode)
                blackPawnsOnBoard++;
            SetTile(coordinates[line][columns], color);
            color = (char) cin.get();
            columns++;
        }
        if(columns < lineLength || tooLong) {
            cout << "WRONG_BOARD_ROW_LENGTH" << endl;
            gameState = InvalidInput;
            return;
        }
        if(line < size - 1)
            lineLength++;
        else
            lineLength--;
    }
}

void Board::PrintBoard() const{
    int lineLength = size;
    vector<vector<Coordinate>> coordinates = GetStraightLines();
    for(int line = 0; line < coordinates.size(); line++){
        const int spaces = GetMaxHeight() - lineLength;
        for(int i = 0; i < spaces; i++)
            cout << ' ';
        for(auto &coordinate : coordinates[line])
            cout << GetTile(coordinate) << ' ';
        cout << endl;
        if(line < size - 1)
            lineLength++;
        else
            lineLength--;
    }
    cout << endl;
}

void Board::PrintGameState() const {
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
            break;
        case InvalidInput:
            cout << "invalid_input" << endl;
            break;
    }
}

void Board::PushPawns(Coordinate from, Coordinate to) {
    char color = currentPlayer;
    lastCommand.from = from.Increment();
    lastCommand.to = to.Increment();
    lastCommand.color = color;
    if(IsMoveValid(from, to) && MovePawns(from, to)) {
        if (currentPlayer == blackCode) {
            blackPawns--; blackPawnsOnBoard++;
        }
        else {
            whitePawns--; blackPawnsOnBoard++;
        }
        currentPlayer = (currentPlayer == blackCode) ? whiteCode : blackCode;
    }
    else {
        gameState = BadMove;
        return;
    }
}


void Board::DoMove(Coordinate from, Coordinate to) {
    Board tempBoard = *this;
    tempBoard.PushPawns(from.Decrement(), to.Decrement()); // Decrementing to match the indexing of the board
    if(gameState == InProgress)
        lastCommand = tempBoard.lastCommand;
    gameState = tempBoard.gameState;
    if(gameState != InProgress)
        return;
    *this = tempBoard;
}

vector<Coordinate> Board::GetNeighbours(Coordinate from) const {
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

bool Board::IsMoveValid(Coordinate from, Coordinate to) const {
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
    char toColor = GetTile(to);
    bool isOk = true;
    if(toColor != emptyCode)
        isOk = MovePawns(to, NextCoordinate(from, to));
    char fromColor = currentPlayer;
    if(IsInBounds(from)) {
        fromColor = GetTile(from);
        SetTile(from, emptyCode);
    }
    SetTile(to, fromColor);
    return isOk;
}

bool Board::IsInBounds(Coordinate coordinate) const {
    if(coordinate.letter < 0 || coordinate.letter >= GetMaxHeight())
        return false;
    int distanceFromCenter = coordinate.letter - (size - 1);
    if(distanceFromCenter < 0)
        distanceFromCenter *= -1;
    int maxNumber = GetMaxHeight() - distanceFromCenter - 1;
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


vector<Board::CoordinateLine> Board::GetUpLines() const {
    vector<vector<Coordinate>> lines;
    for(int i = 0; i < GetMaxHeight(); i++){
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

vector<Board::CoordinateLine> Board::GetStraightLines() const {
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

vector<Board::CoordinateLine> Board::GetDownLines() const {
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

vector<Board::CoordinateLine> Board::GetLines() const {
    vector<vector<Coordinate>> lines, upLines = GetUpLines(), straightLines = GetStraightLines(), downLines = GetDownLines();
    lines.insert(lines.end(), upLines.begin(), upLines.end());
    lines.insert(lines.end(), straightLines.begin(), straightLines.end());
    lines.insert(lines.end(), downLines.begin(), downLines.end());
    return lines;
}

vector<Board::CaptureLine> Board::GetCaptureLines() const {
    vector<CaptureLine> captureLines;
    vector<vector<Coordinate>> lines = GetLines();
    for(auto &line : lines){
        vector<Coordinate> currentCaptureLine;
        char color = emptyCode;
        int currentWhite = 0, currentBlack = 0; bool isCaptureLine = false;
        for(auto &coordinate : line) {
            char currentColor = GetTile(coordinate);
            if (currentColor != emptyCode)
                currentCaptureLine.push_back(coordinate);
            if (currentColor == whiteCode)
                currentWhite++;
            else
                currentWhite = 0;
            if (currentColor == blackCode)
                currentBlack++;
            else
                currentBlack = 0;
            if (currentWhite >= pawnsToCollect) {
                color = whiteCode;
                isCaptureLine = true;
            }
            if( currentBlack >= pawnsToCollect) {
                color = currentColor;
                isCaptureLine = true;
            }
            if(currentColor == emptyCode){
                if(isCaptureLine){
                    captureLines.emplace_back(currentCaptureLine, color);
                }
                currentCaptureLine.clear();
                isCaptureLine = false;
            }
        }
        if(isCaptureLine)
            captureLines.emplace_back(currentCaptureLine, color);
    }
    return captureLines;
}

void Board::RemovePawn(Coordinate coordinate, char lineColor) {
    if(lineColor == emptyCode)
        lineColor = currentPlayer;
    char tileColor = GetTile(coordinate);
    if(tileColor == lineColor){
        if(tileColor == whiteCode)
            whitePawns++;
        else
            blackPawns++;
    }
    if(tileColor == whiteCode)
        whitePawnsOnBoard--;
    else if(tileColor == blackCode)
        blackPawnsOnBoard--;
    SetTile(coordinate, emptyCode);
}

vector<vector<int>> Board::GetCaptureLinesCount() const {
    vector<vector<int>> captureLinesCount(GetMaxHeight(), vector<int>(GetMaxHeight(), 0));
    vector<CaptureLine> captureLines = GetCaptureLines();
    for(auto &line : captureLines){
        for(auto &coordinate : line.coordinates){
            captureLinesCount[coordinate.letter][coordinate.number]++;
        }
    }
    return captureLinesCount;
}

int Board::MaxCaptureLines() {
    int max = 0;
    vector<vector<int>> captureLinesCount = GetCaptureLinesCount();
    for(auto &line : captureLinesCount){
        for(auto &count : line){
            if(count > max)
                max = count;
        }
    }
    return max;
}

unordered_set<Board> Board::PossibleBoardsAfterCapture() {
    unordered_set<Board> boards;
    FillSetWithPossibleBoards(*this, boards);
    return boards;
}

void Board::FillSetWithPossibleBoards(Board &currentBoard, unordered_set<Board> &boardsSet){
    vector<CaptureLine> captureLines = currentBoard.GetCaptureLines();
    if(captureLines.empty()){
        boardsSet.insert(currentBoard);
        return;
    }
    for(auto &line : captureLines){
        Board nextBoard = currentBoard;
        nextBoard.RemoveCaptureLine(line);
        FillSetWithPossibleBoards(nextBoard, boardsSet);
    }
}


void Board::RemoveCaptureLine(const Board::CaptureLine &captureLine) {
    for(auto &coordinate : captureLine.coordinates){
        RemovePawn(coordinate, captureLine.color);
    }
}

bool Board::operator==(const Board &other) const {
    if(this->board != other.board)
        return false;
    if(this->currentPlayer != other.currentPlayer)
        return false;
    if(this->whitePawns != other.whitePawns || this->blackPawns != other.blackPawns)
        return false;
    if(this->whiteStartingPawns != other.whiteStartingPawns || this->blackStartingPawns != other.blackStartingPawns)
        return false;
    if(this->size != other.size || this->pawnsToCollect != other.pawnsToCollect)
        return false;
    if(this->gameState != other.gameState)
        return false;
    return true;
}

std::ostream &operator<<(ostream &os, const Coordinate &c) {
    const int size = 26;
    string s;
    int letter = c.letter;
    while(letter >= 0){
        s +=  char(letter % size + 'A');
        letter /= size;
        letter--;
    }
    reverse(s.begin(), s.end());
    os << s << c.number + 1;
    return os;
}

std::istream &operator>>(istream &is, Coordinate &c) {
    int letter = 0;
    int number = 0;
    char input;
    while(is >> input){
        const int size = 26;
        if(input >= 'A' && input <= 'Z'){
            input -= 'A';
            letter = letter * size + input+1;
        }
        else if(input >= 'a' && input <= 'z'){
            input -= 'a';
            letter = letter * size + input+1;
        } else {
            is.putback(input);
            break;
        }
    }
    is >> number;
    c.letter = letter - 1;
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

Coordinate Coordinate::Increment() const {
    return Coordinate(letter + 1, number + 1);
}

bool Coordinate::operator==(Coordinate &other) const {
    return letter == other.letter && number == other.number;
}

Board::CaptureLine::CaptureLine(Board::CoordinateLine coordinates, char color) {
    this->coordinates = std::move(coordinates);
    this->color = color;
}
