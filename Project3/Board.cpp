#include "Board.h"
#include <iostream>
#include <string>

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
    for(int i = 0; i < maxH*maxH; i++)
        board[i] = '\0';
    lines = GetLines();
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

bool Board::LoadGameBoard() {
    int lineLength = size; blackPawnsOnBoard = 0; whitePawnsOnBoard = 0;
    vector<vector<Coordinate>> coordinates = GetStraightLines();
    int rows = 0;
    for(int line = 0; line < int(coordinates.size()); line++){
        int columns = 0;
        char color = ' '; bool tooLong = false;
        while(color == ' ' || color == '\n')
            color = (char) cin.get();
        while(true) {
            while (color != blackCode && color != whiteCode && color != emptyCode && color != '\n') {
                color = (char) cin.get();
            }
            if(color == '\n') {
                rows++; break;
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
            return false;
        }
        if(line < size - 1)
            lineLength++;
        else
            lineLength--;
    }
    bool rightNumberOfPawns = true;
    if(whitePawns + whitePawnsOnBoard > whiteStartingPawns) {
        cout << "WRONG_WHITE_PAWNS_NUMBER" << endl; rightNumberOfPawns = false;
    }
    else if(blackPawns + blackPawnsOnBoard > blackStartingPawns) {
        cout << "WRONG_BLACK_PAWNS_NUMBER" << endl; rightNumberOfPawns = false;
    }
    if(!rightNumberOfPawns) {
        gameState = InvalidInput;
        return false;
    }
    vector<CaptureLine> captureLines = GetCaptureLines();
    if(!captureLines.empty()) {
        if(captureLines.size() == 1)
            cout << "ERROR_FOUND_1_ROW_OF_LENGTH_K" << endl;
        else
            cout << "ERROR_FOUND_" << captureLines.size() << "_ROWS_OF_LENGTH_K" << endl;
        gameState = InvalidInput;
        return false;
    }
    cout << "BOARD_STATE_OK" << endl;
    CheckForStateChange();
    return true;
}

void Board::PrintBoard() const{
    cout << size << " " << pawnsToCollect << " " << whiteStartingPawns << " " << blackStartingPawns << endl;
    cout << whitePawns << " " << blackPawns << " " << currentPlayer << endl;
    int lineLength = size;
    vector<vector<Coordinate>> coordinates = GetStraightLines();
    for(int line = 0; line < int(coordinates.size()); line++){
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


MoveStatus Board::DoMove(Coordinate from, Coordinate to) {
    moveStatus = MoveStatus::Valid;
    Board tempBoard = *this;
    tempBoard.PushPawns(from.Decrement(), to.Decrement()); // Decrementing to match the indexing of the board
    if(gameState == InProgress)
        lastCommand = tempBoard.lastCommand;
    gameState = tempBoard.gameState;
    wrongCoordinate = tempBoard.wrongCoordinate;
    if(gameState == BadMove)
        return tempBoard.moveStatus;
    *this = tempBoard;
    CheckForStateChange();
    return moveStatus;
}

vector<Coordinate> Board::GetNeighbours(Coordinate from) const {
    vector<Coordinate> neighbours;
    vector<Coordinate> possibleNeighbours;
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
    bool fromIsOnCircumference = IsOnCircumference(from);
    if(!IsInBounds(from) && !fromIsOnCircumference) {
        moveStatus = MoveStatus::InvalidIndex;
        wrongCoordinate = from;
        return false;
    }
    else if(!fromIsOnCircumference) {
        moveStatus = MoveStatus::InvalidStartingPoint;
        wrongCoordinate = from;
        return false;
    }
    if(IsOnCircumference(to)) {
        moveStatus = MoveStatus::InvalidDestination;
        wrongCoordinate = to;
        return false;
    }
    if(!IsInBounds(to)) {
        moveStatus = MoveStatus::InvalidIndex;
        wrongCoordinate = to;
        return false;
    }
    vector<Coordinate> neighbours = GetNeighbours(from);
    bool found = false;
    for(auto & neighbour : neighbours){
        if(neighbour == to){
            found = true;
            break;
        }
    }
    if(!found) {
        moveStatus = MoveStatus::UnknownDirection;
        return false;
    }
    return true;
}

bool Board::MovePawns(Coordinate from, Coordinate to) {
    if(!IsInBounds(to)) {
        moveStatus = MoveStatus::FullRow;
        return false;
    }
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

bool Board::IsOnCircumference(Coordinate coordinate) const {
    vector<Coordinate> neighbours = GetNeighbours(coordinate);
    if(IsInBounds(coordinate))
        return false;
    for(auto & neighbour : neighbours){
        if(IsInBounds(neighbour))
            return true;
    }
    return false;
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
    vector<vector<Coordinate>> upLines;
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
        upLines.push_back(line);
    }
    return upLines;
}

vector<Board::CoordinateLine> Board::GetStraightLines() const {
    vector<vector<Coordinate>> straightLines;
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
        straightLines.push_back(line);
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
        straightLines.push_back(line);
    }
    return straightLines;
}

vector<Board::CoordinateLine> Board::GetDownLines() const {
    vector<vector<Coordinate>> downLines;
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
        downLines.push_back(line);
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
        downLines.push_back(line);
    }
    return downLines;
}

vector<Board::CoordinateLine> Board::GetLines() const {
    vector<vector<Coordinate>> allLines, upLines = GetUpLines(), straightLines = GetStraightLines(), downLines = GetDownLines();
    allLines.insert(allLines.end(), upLines.begin(), upLines.end());
    allLines.insert(allLines.end(), straightLines.begin(), straightLines.end());
    allLines.insert(allLines.end(), downLines.begin(), downLines.end());
    return allLines;
}

vector<Board::CaptureLine> Board::GetCaptureLines() const {
    vector<CaptureLine> captureLines;
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
                color = emptyCode;
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

void Board::RemoveSimpleCaptures() {
    vector<CaptureLine> captureLines = GetCaptureLines();
    vector<vector<CaptureLine*>> captureLinesField(GetMaxHeight()*GetMaxHeight(), vector<CaptureLine*>());
    for(auto &captureLine : captureLines){
        for(auto &coordinate : captureLine.coordinates){
            captureLinesField[coordinate.number*GetMaxHeight() + coordinate.letter].push_back(&captureLine);
        }
    }
    for(auto &captureLine : captureLines){
        bool isSimpleCapture = true;
        for(auto &coordinate : captureLine.coordinates){
            if(captureLinesField[coordinate.number*GetMaxHeight() + coordinate.letter].size() > 1){
                isSimpleCapture = false;
                break;
            }
        }
        if(isSimpleCapture){
            RemoveCaptureLine(captureLine);
        }
    }
}

void Board::CheckForStateChange() {
    int freeTiles = 0;
    for(char c : board)
        if(c == emptyCode)
            freeTiles++;
    if(freeTiles == 0){
        gameState = DeadLock;
        return;
    }
    if(whitePawns == 0)
        gameState = BlackWon;
    else if(blackPawns == 0)
        gameState = WhiteWon;
}

std::ostream &operator<<(ostream &os, const Coordinate &c) {
    const int size = 26;
    string s;
    int letter = c.letter;
    while(letter >= 0){
        s +=  char(letter % size + 'a');
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

Board::CaptureLine::CaptureLine(const Board::CoordinateLine &coordinates, char color) : coordinates(coordinates), color(color) {}
