#include "Game.h"
#include "String.h"
#include <iostream>

using namespace std;

void Game::Run() {
    while (!exit)
        Query();
}

void Game::Query() {
    String input;
    cin >> input;
    if (input == "EXIT")
        exit = true;
    else if(input == "LOAD_GAME_BOARD")
        LoadGameBoard();
    else if(input == "PRINT_GAME_BOARD")
        PrintGameBoard();
    else if(input == "PRINT_COORDINATES")
        PrintCoordinates();
    else if(input == "PRINT_GAME_STATE")
        PrintGameState();
    else if(input == "DO_MOVE")
        DoMove();
    else if(input.size() > 0)
        cout << "Unknown command" << endl;
}

void Game::LoadGameBoard() {
    int boardSize, pawnsToCollect, blackStartingPawns, whiteStartingPawns, blackPawns, whitePawns;
    char currentPlayer;
    cin >> boardSize >> pawnsToCollect >> whiteStartingPawns >> blackStartingPawns >> whitePawns >> blackPawns >> currentPlayer;
    board = Board(boardSize, pawnsToCollect, blackStartingPawns, whiteStartingPawns, blackPawns, whitePawns, currentPlayer);
    board.LoadGameBoard();
}

void Game::PrintGameBoard() {
    board.PrintBoard();
}

void Game::PrintCoordinates() {
    int lineLength = board.size;
    for(int letter = 0; letter <= board.getMaxHeight() + 1; letter++){
        const int spaces = board.getMaxHeight() - lineLength+1;
        for(int i = 0; i < spaces; i++)
            cout << "  ";
        for(int number = 0; number <= lineLength; number++) {
            Coordinate coordinate(letter, number);
            cout << coordinate << "  ";
        }
        cout << endl;
        if(letter < board.size)
            lineLength++;
        else
            lineLength--;
    }
}

void Game::PrintGameState() {
    board.PrintGameState();
}

void Game::DoMove() {
    Coordinate from, to;
    cin >> from >> to;
    Board tempBoard = board;
    tempBoard.DoMove(from.Decrement(), to.Decrement()); // Decrementing to match the indexing of the board
    if(board.gameState != BadMove)
        board.lastCommand = tempBoard.lastCommand;
    board.gameState = tempBoard.gameState;
    if(board.gameState != BadMove)
        board = tempBoard;
}
