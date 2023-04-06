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
                    alivePieces.push_back(board[index]);
                }
                if (x == 1 || x == 6) {
                    board[index] = new Knight(y == 0 ? false : true, pos);
                    alivePieces.push_back(board[index]);
                }
                if (x == 2 || x == 5) {
                    board[index] = new Bishop(y == 0 ? false : true, pos);
                    alivePieces.push_back(board[index]);
                } 
                if (x == 3) {
                    board[index] = new Queen(y == 0 ? false : true, pos);
                    alivePieces.push_back(board[index]);
                }
                if (x == 4) {
                    board[index] = new King(y == 0 ? false : true, pos);
                    alivePieces.push_back(board[index]);
                }
            }
            if (y == 1 || y == 6) {
                board[index] = new Pawn(y == 1 ? false : true, pos);
                alivePieces.push_back(board[index]);
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

void Board::printBoard() {
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
