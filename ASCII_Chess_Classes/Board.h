#ifndef BOARD_H
#define BOARD_H
#include <vector>
#include <Windows.h>
#include "Position.h"
#include "Pieces.h"

class Board {
public:
    static const int WIDTH = 8, HEIGHT = 8;

    Board();
    ~Board();

    void printBoard(HANDLE hConsole);
    Piece* getPiece(Position pos);
    void setPiece(Position pos, Piece* piece);
    void updateBoard();

private:
    Piece* board[HEIGHT * WIDTH];
    std::vector<Piece*> alivePieces;
};

#endif // BOARD_H