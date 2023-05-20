#include "Game.h"
#include <iostream>
#include <string>

using namespace std;

void Game::Run() {
    while (!exit)
        Query();
}

void Game::Query() {
    string input;
    cin >> input;
    if(cin.eof())
        exit = true;
    for(char &c : input)
        c = (char) toupper(c);

    if (input == "EXIT")
        exit = true;
    else if(input == "LOAD_GAME_BOARD")
        LoadGameBoard();
    else if(input == "PRINT_GAME_BOARD") {
        PrintGameBoard(); cout << endl;
    }
    else if(input == "PRINT_COORDINATES")
        PrintCoordinates();
    else if(input == "PRINT_GAME_STATE")
        PrintGameState();
    else if(input == "DO_MOVE")
        DoMove();
}

void Game::LoadGameBoard() {
    int boardSize, pawnsToCollect, blackStartingPawns, whiteStartingPawns, blackPawns, whitePawns;
    char currentPlayer;
    cin >> boardSize >> pawnsToCollect >> whiteStartingPawns >> blackStartingPawns >> whitePawns >> blackPawns >> currentPlayer;
    board = Board(boardSize, pawnsToCollect, blackStartingPawns, whiteStartingPawns, blackPawns, whitePawns, currentPlayer);
    board.LoadGameBoard();
}

void Game::PrintGameBoard() const {
    board.PrintBoard();
}

void Game::PrintCoordinates() const {
    int lineLength = board.size;
    // temp board that is used to get the coordinates, it's size+1 to show coordinates outside the board
    Board temp(lineLength+1);
    vector<vector<Coordinate>> coordinates = temp.GetStraightLines();
    for(int line = 0; line < int(coordinates.size()); line++){
        const int spaces = board.GetMaxHeight() - lineLength + 1;
        for(int i = 0; i < spaces; i++)
            cout << "  ";
        for(auto &coordinate : coordinates[line])
            cout << coordinate << "  ";
        cout << endl;
        if(line < board.size)
            lineLength++;
        else
            lineLength--;
    }
}

void Game::PrintGameState() const {
    board.PrintGameState();
}

void Game::DoMove() {
    Coordinate from, to; char c;
    cin >> from >> c;
    if(c != '-')
        cin.putback(c);
    cin >> to;
    board.DoMove(from, to);
    board = RemoveCapturedPawns(board);
}

Board Game::RemoveCapturedPawns(Board &nextBoard) {
    vector<Board::CaptureLine> captureLines = nextBoard.GetCaptureLines();
    if(captureLines.empty())
        return nextBoard;
    if(captureLines.size() == 1) {
        nextBoard.RemoveCaptureLine(captureLines[0]);
        return nextBoard;
    }

    char color, temp;
    cin >> color >> temp;
    if(temp != ':')
        cin.putback(temp);
    color = (char) toupper(color);

    vector<Coordinate> chosenCoordinates(nextBoard.pawnsToCollect), coordinates;
    for(auto& coordinate : chosenCoordinates){
        Coordinate c;
        cin >> c;
        coordinate = c.Decrement();
    }
    for(auto& captureLine : captureLines){
        vector<bool> chosenCoordinatesFound(chosenCoordinates.size(), false);
        for(auto& coordinate : captureLine.coordinates){
            for(int i = 0; i < int(chosenCoordinates.size()); i++){
                if(coordinate == chosenCoordinates[i]){
                    chosenCoordinatesFound[i] = true;
                    break;
                }
            }
        }
        bool allFound = true;
        for(bool found : chosenCoordinatesFound){
            if(!found){
                allFound = false;
                break;
            }
        }
        if(allFound){
            coordinates = captureLine.coordinates;
            break;
        }
    }
    nextBoard.RemoveCaptureLine(Board::CaptureLine(coordinates, color));
    return RemoveCapturedPawns(nextBoard);
}

