#include <iostream>
#include "Board.h"
#include "Pieces.h"
#include "Position.h"
#include <Windows.h>

Board::Board()
{
    // Initializations
    alivePieces.clear();
    // Create pieces
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {

            Position pos = { x,y };
            int index = x + y * WIDTH;

            if (y == 0 || y == 7) {
                if (x == 0 || x == 7) {
                    board[index] = new Rook(y == 0 ? false : true, pos);
                }
                if (x == 1 || x == 6) {
                    board[index] = new Knight(y == 0 ? false : true, pos);
                }
                if (x == 2 || x == 5) {
                    board[index] = new Bishop(y == 0 ? false : true, pos);
                } 
                if (x == 3) {
                    board[index] = new Queen(y == 0 ? false : true, pos);
                }
                if (x == 4) {
                    board[index] = new King(y == 0 ? false : true, pos);
                }
            }
            if (y == 1 || y == 6) {
                board[index] = new Pawn(y == 1 ? false : true, pos);
            }
            if (y > 1 && y < 6) {
                board[index] = nullptr;
            }
        }
    }
}

Board::~Board()
{
    for (int i = 0; i < HEIGHT * WIDTH; i++) {
        delete board[i];
    }
}

void Board::printBoard(HANDLE hConsole) {

    std::cout << "    A  B  C  D  E  F  G  H\n";
    for (int y = 0; y < HEIGHT; y++) {
        std::cout << " " << HEIGHT - y << " ";
        for (int x = 0; x < WIDTH; x++) {
            Position pos = { x,y };
            Piece* p = getPiece(pos);
            if ((x + y) % 2 == 0) {
                if (!p) {
                    SetConsoleTextAttribute(hConsole, BACKGROUND_GREEN);
                    std::cout << "  ";
                }
                else {
                    if (p->bIsWhite) {
                        SetConsoleTextAttribute(hConsole, BACKGROUND_GREEN | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                    }
                    else {
                        SetConsoleTextAttribute(hConsole, BACKGROUND_GREEN);
                    }
                    std::wcout << " " << p->getSymbol();
                }
            }
            else {
                if (!p) {
                    SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE); 
                    std::cout << "  ";
                }
                else {
                    if (p->bIsWhite) {
                        SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                    }
                    else {
                        SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE);
                    }
                    std::wcout << " " << p->getSymbol();
                }
                
            }
            std::cout << " ";
        }
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        std::cout << " " << HEIGHT - y << "\n";
    }

    // Print the bottom line
    //SetConsoleTextAttribute(hConsole, BACKGROUND_GREEN | BACKGROUND_BLUE);
    std::cout << "    A  B  C  D  E  F  G  H\n";
}

Piece* Board::getPiece(Position pos){
    return board[pos.x + pos.y * WIDTH];
}

void Board::setPiece(Position pos, Piece* piece)
{
    if (pos.x < 0 || pos.x >= WIDTH || pos.y < 0 || pos.y >= HEIGHT) {
        return;
    }
    board[pos.y * WIDTH + pos.x] = piece;
}

void Board::updateBoard() {
    //clear board
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            int index = x + y * WIDTH;
            board[index] = nullptr;
        }
    }
    //remove captured
    alivePieces.erase(std::remove_if(alivePieces.begin(), alivePieces.end(), [](Piece* p) { return !p->bIsAlive; }), alivePieces.end());
}
