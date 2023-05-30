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
    else if(input == "IS_GAME_OVER")
        IsGameOver();
}

void Game::LoadGameBoard() {
    int boardSize, pawnsToCollect, blackStartingPawns, whiteStartingPawns, blackPawns, whitePawns;
    char currentPlayer;
    cin >> boardSize >> pawnsToCollect >> whiteStartingPawns >> blackStartingPawns >> whitePawns >> blackPawns >> currentPlayer;
    board = Board(boardSize, pawnsToCollect, blackStartingPawns, whiteStartingPawns, blackPawns, whitePawns, currentPlayer);
    boardLoaded = board.LoadGameBoard();
}

void Game::PrintGameBoard() const {
    if(boardLoaded)
        board.PrintBoard();
    else
        cout << "EMPTY_BOARD" << endl;
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
    if(status == Valid)
        status = board.moveStatus;
    switch (status) {
        case Valid:
            cout << "MOVE_COMMITTED" << endl; break;
        case InvalidIndex:
            cout << "BAD_MOVE_" << wrong << "_IS_WRONG_INDEX" << endl; break;
        case InvalidStartingPoint:
            cout << "BAD_MOVE_" << wrong << "_IS_WRONG_STARTING_FIELD" << endl; break;
        case InvalidDestination:
            cout << "BAD_MOVE_" << wrong << "_IS_WRONG_DESTINATION_FIELD" << endl; break;
        case FullRow:
            cout << "BAD_MOVE_ROW_IS_FULL" << endl; break;
        case UnknownDirection:
            cout << "UNKNOWN_MOVE_DIRECTION" << endl; break;
        case WrongCaptureLineIndex:
            cout << "WRONG_INDEX_OF_CHOSEN_ROW" << endl; break;
        case WrongCaptureLineColor:
            cout << "WRONG_COLOR_OF_CHOSEN_ROW" << endl; break;
    }
}

MoveStatus Game::RemoveCapturedPawns() {
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
            Coordinate first, last;
            cin >> first >> last;
            first = first.Decrement();
            last = last.Decrement();
            Board::CaptureLine* captureLine = nullptr;
            for(auto& line : captureLines){
                int firstIndex = -1, lastIndex = -1;
                for(int i = 0; i < int(line.coordinates.size()); i++){
                    if(line.coordinates[i] == first)
                        firstIndex = i;
                    if(line.coordinates[i] == last)
                        lastIndex = i;
                    if(firstIndex != -1 && lastIndex != -1) {
                        if(firstIndex > lastIndex)
                            swap(firstIndex, lastIndex);
                        int before = firstIndex - 1, after = lastIndex + 1;

                        if(before >= 0 && board.GetTile(line.coordinates[before]) == color)
                            break;
                        if(after < int(line.coordinates.size()) && board.GetTile(line.coordinates[after]) == color)
                            break;
                        captureLine = &line; break;
                    }
                }
                if(captureLine != nullptr)
                    break;
            }
            if(captureLine == nullptr){
                board.moveStatus = MoveStatus::WrongCaptureLineIndex;
                return board.moveStatus;
            }
            if(color != captureLine->color){
                board.moveStatus = MoveStatus::WrongCaptureLineColor;
                return board.moveStatus;
            }
            board.RemoveCaptureLine(*captureLine);
        }
        board.RemoveSimpleCaptures();
        captureLines = board.GetCaptureLines();
    }
    return board.moveStatus;
}

void Game::GenerateAllPossibleMoves() {
    Solver solver(board);
    unordered_set<Board> possibleMoves = solver.GetPossibleMoves();
    int i = 0;
    for(auto& currentBoard : possibleMoves){
        cout << ++i << ". " << currentBoard.lastCommand.from << '-' << currentBoard.lastCommand.to << endl;
        currentBoard.PrintBoard();
    }
}

void Game::GenerateAllPossibleMovesExtended() {
    Solver solver(board);
    unordered_set<Board> possibleMoves = solver.GetPossibleMoves();
    for(auto& currentBoard : possibleMoves){
        if(currentBoard.gameState == GameState::BlackWon || currentBoard.gameState == GameState::WhiteWon){
            cout << currentBoard.lastCommand.from << '-' << currentBoard.lastCommand.to << endl;
            currentBoard.PrintBoard();
            currentBoard.PrintGameState();
            return;
        }
    }
    int i = 0;
    for(auto& currentBoard : possibleMoves){
        cout << ++i << ". " << currentBoard.lastCommand.from << '-' << currentBoard.lastCommand.to << endl;
        currentBoard.PrintBoard();
    }
}

void Game::GenerateAllPossibleMovesNumber() {
    Solver solver(board);
    unordered_set<Board> possibleMoves = solver.GetPossibleMoves();
    cout << possibleMoves.size() << "_UNIQUE_MOVES" << endl;
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

void Game::IsGameOver() const {
    bool whiteWon = false, blackWon = false;
    if(board.gameState == WhiteWon)
        whiteWon = true;
    else if(board.gameState == BlackWon)
        blackWon = true;
    else if(board.gameState == DeadLock){
        if(board.currentPlayer == Board::whiteCode)
            blackWon = true;
        else
            whiteWon = true;
    }
    else if(board.gameState == InProgress){
        cout << "GAME_IN_PROGRESS" << endl;
    }
    if(whiteWon)
        cout << "THE_WINNER_IS_WHITE" << endl;
    else if(blackWon)
        cout << "THE_WINNER_IS_BLACK" << endl;
}
