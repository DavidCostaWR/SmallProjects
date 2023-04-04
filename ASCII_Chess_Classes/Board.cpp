#include <iostream>
#include "Board.h"
#include "Pieces.h"
#include "Position.h"

Board::Board()
{
    // Create pieces
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {

            Position pos = { x,y };
            int index = x + y * WIDTH;

            if (y == 0 || y == 7) {
                if (x == 0 || x == 7) {
                    pieces[index] = new Rook(y == 0 ? false : true, pos);
                }
                if (x == 1 || x == 6) {
                    pieces[index] = new Knight(y == 0 ? false : true, pos);
                }
                if (x == 2 || x == 5) {
                    pieces[index] = new Bishop(y == 0 ? false : true, pos);
                } 
                if (x == 3) {
                    pieces[index] = new Queen(y == 0 ? false : true, pos);
                }
                if (x == 4) {
                    pieces[index] = new King(y == 0 ? false : true, pos);
                }
            }
            if (y == 1 || y == 6) {
                pieces[index] = new Pawn(y == 1 ? false : true, pos);
            }
            if (y > 1 && y < 6) {
                pieces[index] = nullptr;
            }
            if (pieces[index] == nullptr) {
                board[index] = ' ';
            }
            else {
                board[index] = pieces[index]->getSymbol();
            }
        }
    }
}

Board::~Board()
{
    for (int i = 0; i < HEIGHT * WIDTH; i++) {
        delete pieces[i];
    }
}

void Board::printBoard() const
{
    // Print the board to the console
}

wchar_t Board::getPiece(int x, int y) const
{
    // Return the piece at position (x, y)
}

void Board::setPiece(int x, int y, Piece* piece)
{
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
        return;
    }
    board[y * WIDTH + x] = piece->getSymbol();
}