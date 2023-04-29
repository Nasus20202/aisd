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
    else if(input.size() > 0)
        cout << "Unknown command" << endl;
}

void Game::LoadGameBoard() {
    int boardSize, pawnsToCollect, blackStartingPawns, whiteStartingPawns, blackPawns, whitePawns;
    char currentPlayer;
    cin >> boardSize >> pawnsToCollect >> blackStartingPawns >> whiteStartingPawns >> blackPawns >> whitePawns >> currentPlayer;
    board = Board(boardSize, pawnsToCollect, blackStartingPawns, whiteStartingPawns, blackPawns, whitePawns, currentPlayer);
    board.LoadGameBoard();
}

void Game::PrintGameBoard() {
    board.PrintBoard();
}
