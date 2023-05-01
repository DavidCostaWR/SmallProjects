#pragma once

#ifndef BOARD_H
#define BOARD_H

#include "Pieces.h"

#include <vector>
#include <Windows.h>

class Piece;
class PGE_Chess;

class Board {
public:
    static const int COLUMNS = 8, ROWS = 8;
    std::vector<Piece*> alivePieces;
    static PGE_Chess* g_engine;

public:
    Board();
    ~Board();

    Piece* getPiece(olc::vi2d pos);
    void printBoard();
    bool isMoveLegal(Piece* activePiece, olc::vi2d pos);
    void updateBoard();

private:
    Piece* board[COLUMNS * ROWS];
};

#endif // BOARD_H