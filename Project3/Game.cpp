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

    int lineLength = board.size;
    for(int letter = 0; letter < board.getMaxHeight(); letter++){
        for(int number = 0; number < lineLength; number++){
            char c = board.getTile(letter, number);
            cout << char(letter+'B') << number+2 << ' ' << c << ' ';
        }
        cout << endl;
        if(letter < board.size - 1)
            lineLength++;
        else
            lineLength--;
    }
}
