#include "Game.h"
#include "Solver.h"
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
    else if(input == "BREAK") {
        string line;
        for(int i = 0; i < 50; i++)
            line += "-";
        cout << line << endl;
    }
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
    else if(input == "GEN_ALL_POS_MOV")
        GenerateAllPossibleMoves();
    else if(input == "GEN_ALL_POS_MOV_EXT")
        GenerateAllPossibleMovesExtended();
    else if(input == "GEN_ALL_POS_MOV_NUM")
        GenerateAllPossibleMovesNumber();
    else if(input == "GEN_ALL_POS_MOV_NUM_EXT")
        GenerateAllPossibleMovesNumberExtended();
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
    MoveStatus status = board.DoMove(from, to);
    RemoveCapturedPawns();
    Coordinate wrong = board.wrongCoordinate.Increment();
    switch (status) {
        case MoveStatus::Valid:
            cout << "MOVE_COMMITTED" << endl; break;
        case MoveStatus::InvalidIndex:
            cout << "BAD_MOVE_" << wrong << "_IS_WRONG_INDEX" << endl; break;
        case MoveStatus::InvalidStartingPoint:
            cout << "BAD_MOVE_" << wrong << "_IS_WRONG_STARTING_FIELD" << endl; break;
        case MoveStatus::InvalidDestination:
            cout << "BAD_MOVE_" << wrong << "_IS_WRONG_DESTINATION_FIELD" << endl; break;
        case MoveStatus::FullRow:
            cout << "BAD_MOVE_ROW_IS_FULL" << endl; break;
        case UnknownDirection:
            cout << "UNKNOWN_MOVE_DIRECTION" << endl; break;
    }
}

void Game::RemoveCapturedPawns() {
    board.RemoveSimpleCaptures();
    vector<Board::CaptureLine> captureLines = board.GetCaptureLines();
    while(!captureLines.empty()){
        if(captureLines.size() == 1) {
            board.RemoveCaptureLine(captureLines[0]);
        } else {
            char color, temp;
            cin >> color >> temp;
            if(temp != ':')
                cin.putback(temp);
            color = (char) toupper(color);
            vector<Coordinate> chosenCoordinates(board.pawnsToCollect), coordinates;
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
            board.RemoveCaptureLine(Board::CaptureLine(coordinates, color));
        }
        board.RemoveSimpleCaptures();
        captureLines = board.GetCaptureLines();
    }
}

void Game::GenerateAllPossibleMoves() {
    Solver solver(board);
    unordered_set<Board> possibleMoves = solver.GetPossibleMoves();
    int i = 0;
    for(auto& currentBoard : possibleMoves){
        cout << ++i << '.' << endl;
        currentBoard.PrintBoard();
    }
}

void Game::GenerateAllPossibleMovesExtended() {
    Solver solver(board);
    unordered_set<Board> possibleMoves = solver.GetPossibleMoves();
    for(auto& currentBoard : possibleMoves){
        if(currentBoard.gameState == GameState::BlackWon || currentBoard.gameState == GameState::WhiteWon){
            currentBoard.PrintBoard();
            currentBoard.PrintGameState();
            return;
        }
    }
    int i = 0;
    for(auto& currentBoard : possibleMoves){
        cout << ++i << '.' << endl;
        currentBoard.PrintBoard();
    }
}

void Game::GenerateAllPossibleMovesNumber() {
    Solver solver(board);
    unordered_set<Board> possibleMoves = solver.GetPossibleMoves();
    cout << possibleMoves.size() << endl;
}

void Game::GenerateAllPossibleMovesNumberExtended() {
    Solver solver(board);
    unordered_set<Board> possibleMoves = solver.GetPossibleMoves();
    for(auto& currentBoard : possibleMoves){
        if(currentBoard.gameState == GameState::BlackWon || currentBoard.gameState == GameState::WhiteWon){
            cout << 1 << endl;
            return;
        }
    }
    cout << possibleMoves.size() << endl;
}

