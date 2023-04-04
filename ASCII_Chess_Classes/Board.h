#ifndef BOARD_H
#define BOARD_H
#include "Position.h"
#include "Pieces.h"

class Board {
public:
    static const int WIDTH = 8, HEIGHT = 8;

    Board();
    ~Board();

    void printBoard() const;
    wchar_t getPiece(int x, int y) const;
    void setPiece(int x, int y, Piece* piece);


private:
    wchar_t board[HEIGHT * WIDTH];
    Piece* pieces[HEIGHT * WIDTH];
};

#endif // BOARD_H