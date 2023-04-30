#include "Board.h"

PGE_Chess* Board::g_engine = nullptr;

Board::Board()
{
    // Initializations
    alivePieces.clear();

    // Create pieces
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLUMNS; x++) {

            olc::vi2d pos = { x,y };
            int index = x + y * COLUMNS;

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
    for (int i = 0; i < ROWS * COLUMNS; i++) {
        delete board[i];
    }
}

void Board::printBoard() {
}

Piece* Board::getPiece(olc::vi2d pos){
    for (auto piece : alivePieces) {
        if (piece->pPos == pos) {
            return piece;
        }
    }
    return nullptr;

    //return board[pos.x + pos.y * COLUMNS];
}

bool Board::isMoveLegal(Piece* activePiece, olc::vi2d pos)
{
    for (auto& move : activePiece->nMoves) {
        if (move == pos) {
            return true;
        }
    }
    return false;
}

bool removePiece(Piece* pieceToRemove) {
    //alivePieces

}

void Board::updateBoard() {
    //remove captured
    alivePieces.erase(std::remove_if(alivePieces.begin(), alivePieces.end(), [](Piece* p) { return !p->bIsAlive; }), alivePieces.end());

    // Clear Board
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLUMNS; x++) {
            int index = x + y * COLUMNS;
            board[index] = nullptr;
        }
    }
    for (auto& piece : alivePieces) {
        int index = piece->pPos.x + piece->pPos.y * COLUMNS;
        board[index] = piece;
    }
}
