#include "Pieces.h"

PGE_Chess* Piece::g_engine = nullptr;

Piece::Piece(){
    // Default constructor implementation
    bIsAlive = true;
}

Piece::~Piece(){
    // Destructor implementation
}

void Piece::DrawPiece(olc::vf2d scale, int Phase, olc::vf2d mousePos, olc::vi2d boardPos) {

    olc::vf2d boardPosition = { 0,0 };
    //scale = { 0.3f * g_engine->fScaleToScreen, 0.3f * g_engine->fScaleToScreen };

    if(g_engine->turn % 2) {
        	boardPosition = { g_engine->nBoardOffsetX + g_engine->boardFrame.x + (float)pPos.x * g_engine->fTileSize, g_engine->nBoardOffsetY + g_engine->boardFrame.y + (float)pPos.y * g_engine->fTileSize };
        }
    else {
        	boardPosition = { g_engine->nBoardOffsetX + g_engine->boardFrame.x + (float)pPos.x * g_engine->fTileSize, g_engine->nBoardOffsetY + g_engine->boardFrame.y + (float)(7 - pPos.y) * g_engine->fTileSize };
        }
    
    olc::vf2d sourcePos = { pieceSize.x * nPieceType, pieceSize.y * (!bIsWhite) };
    if (!bSelected) {
        g_engine->DrawPartialDecal(boardPosition, g_engine->m_mapDecals["pieces"], sourcePos, pieceSize, scale);
    }
    if (bSelected) {
        g_engine->DrawPartialDecal(boardPosition, g_engine->m_mapDecals["pieces"], sourcePos, pieceSize, scale, olc::RED);
        for (auto& move : nMoves) {
            if (move.x == boardPos.x && (bIsWhite ? (move.y == boardPos.y) : (move.y == (g_engine->board->ROWS - 1 - boardPos.y)))) {
                g_engine->DrawPartialDecal(mousePos, g_engine->m_mapDecals["pieces"], sourcePos, pieceSize, scale, olc::Pixel(255, 255, 255, 150));
            }

        }
    }
}

King::King(bool isWhite, olc::vf2d pos){
    bIsWhite = isWhite;
    pPos = pos;
    nPieceType = KING;
}

King::~King(){
    // King destructor implementation
}

bool King::isValidMove(olc::vf2d from, olc::vf2d to) {

    return true;
}

bool King::getMoves() {
    nMoves.clear();
    // Place the King's move behavior here
    for (int dx = -1; dx <= 1; dx++)
    {
        for (int dy = -1; dy <= 1; dy++)
        {
            int x = pPos.x + dx;
            int y = pPos.y + dy;
            if (g_engine->InGameBounds(x, y)){
                if (!g_engine->board->getPiece({ x,y })) {
                    nMoves.push_back({ x,y });
                }
                else if (bIsWhite ? !g_engine->board->getPiece({ x,y })->bIsWhite : g_engine->board->getPiece({ x,y })->bIsWhite) {
                    nMoves.push_back({ x,y });
                }
                // Check if it's checkmate
            }
        }
    }
    // To Do: Check for castling
    return true;

}

// ************ QUEEN

Queen::Queen(bool isWhite, olc::vf2d pos) {
    bIsWhite = isWhite;
    pPos = pos;
    nPieceType = QUEEN;
}

Queen::~Queen() {
    // Queen destructor implementation
}

bool Queen::isValidMove(olc::vf2d from, olc::vf2d to) {
    // Place the Queen's move behavior here
    return true;
}

bool Queen::getMoves() {
    nMoves.clear();

    for (int dx = -1; dx <= 1; dx += 1)
    {
        for (int dy = -1; dy <= 1; dy += 1)
        {
            int x = pPos.x + dx;
            int y = pPos.y + dy;

            // In Bounds
            while (g_engine->InGameBounds(x, y)) {

                if (!g_engine->board->getPiece({ x,y }))
                    nMoves.push_back({ x,y });
                else if (bIsWhite ? !g_engine->board->getPiece({ x,y })->bIsWhite : g_engine->board->getPiece({ x,y })->bIsWhite) {
                    nMoves.push_back({ x,y });
                    break;
                }
                else
                    break;

                x += dx;
                y += dy;
            }
        }
    }

    return true;

}

// ************ ROOK

Rook::Rook(bool isWhite, olc::vf2d pos) {
    bIsWhite = isWhite;
    pPos = pos;
    nPieceType = ROOK;
}

Rook::~Rook() {
    // Rook destructor implementation
}

bool Rook::isValidMove(olc::vf2d from, olc::vf2d to) {
    // Place the Rook's move behavior here
    return true;
};

bool Rook::getMoves() {
    nMoves.clear();

    for (int dx = -1; dx <= 1; dx += 2) {
        int x = pPos.x + dx;
        int y = pPos.y;

        // In Bounds
        while (g_engine->InGameBounds(x, y)) {

            if (!g_engine->board->getPiece({ x,y }))
                nMoves.push_back({ x,y });
            else if (bIsWhite ? !g_engine->board->getPiece({ x,y })->bIsWhite : g_engine->board->getPiece({ x,y })->bIsWhite) {
                nMoves.push_back({ x,y });
                break;
            }
            else
                break;

            x += dx;
        }
    }

    for (int dy = -1; dy <= 1; dy += 2)
    {
        int x = pPos.x;
        int y = pPos.y + dy;

        // In Bounds
        while (g_engine->InGameBounds(x, y)) {

            if (!g_engine->board->getPiece({ x,y }))
                nMoves.push_back({ x,y });
            else if (bIsWhite ? !g_engine->board->getPiece({ x,y })->bIsWhite : g_engine->board->getPiece({ x,y })->bIsWhite) {
                nMoves.push_back({ x,y });
                break;
            }
            else
                break;

            y += dy;
        }
    }

    return true;

}

// ************ KNIGH

Knight::Knight(bool isWhite, olc::vf2d pos) {
    bIsWhite = isWhite;
    pPos = pos;
    nPieceType = KNIGHT;
}

Knight::~Knight() {
    // Knight destructor implementation
}

bool Knight::isValidMove(olc::vf2d from, olc::vf2d to) {
    // Place the Knight's move behavior here
    return true;
};

bool Knight::getMoves() {
    nMoves.clear();
    for (int dx = -2; dx <= 2; dx++) {
        for (int dy = -2; dy <= 2; dy++) {
            int x = pPos.x + dx;
            int y = pPos.y + dy;

            if (g_engine->InGameBounds(x, y) && (abs(dx) + abs(dy) == 3)) {
                if (!g_engine->board->getPiece({ x,y }))
                    nMoves.push_back({ x,y });
                else if (bIsWhite ? !g_engine->board->getPiece({ x,y })->bIsWhite : g_engine->board->getPiece({ x,y })->bIsWhite)
                    nMoves.push_back({ x,y });
            }
        }
    }
    return true;

}

// ************ BISHOP

Bishop::Bishop(bool isWhite, olc::vf2d pos) {
    bIsWhite = isWhite;
    pPos = pos;
    nPieceType = BISHOP;
}

Bishop::~Bishop() {
    // Bishop destructor implementation
}

bool Bishop::isValidMove(olc::vf2d from, olc::vf2d to) {
    // Place the Bishop's move behavior here
    return true;
};

bool Bishop::getMoves() {
    nMoves.clear();
    for (int dx = -1; dx <= 1; dx += 2)
    {
        for (int dy = -1; dy <= 1; dy += 2)
        {
            int x = pPos.x + dx;
            int y = pPos.y + dy;

            // In Bounds
            while (g_engine->InGameBounds(x, y)) {

                if (!g_engine->board->getPiece({ x,y }))
                    nMoves.push_back({ x,y });
                else if (bIsWhite ? !g_engine->board->getPiece({ x,y })->bIsWhite : g_engine->board->getPiece({ x,y })->bIsWhite) {
                    nMoves.push_back({ x,y });
                    break;
                }
                else
                    break;

                x += dx;
                y += dy;
            }
        }
    }
    return true;

}

// ************ PAWN

Pawn::Pawn(bool isWhite, olc::vf2d pos) {
    bIsWhite = isWhite;
    pPos = pos;
    nPieceType = PAWN;
}

Pawn::~Pawn() {
    // Pawn destructor implementation
}

bool Pawn::isValidMove(olc::vf2d from, olc::vf2d to) {
    // Place the Pawn's move behavior here
    return true;
};

bool Pawn::getMoves() {
    nMoves.clear();
    int x = pPos.x;
    int y = pPos.y;

    if (pPos.y == (bIsWhite ? 0 : g_engine->board->ROWS - 1))
    {
        // Choose piece, remove pawn and create new piece
        return true;
    }

    y += bIsWhite ? (-1) : 1;
    if (!g_engine->board->getPiece({ x, y })) {
        nMoves.push_back({ x,y });
        y += bIsWhite ? (-1) : 1;
        if ((pPos.y == (bIsWhite ? (g_engine->board->ROWS - 2) : 1)) && !g_engine->board->getPiece({ x, y }))
            nMoves.push_back({ x,y });
    }

    for (int dx = -1; dx <= 1; dx += 2)
    {
        int dy = bIsWhite ? (-1) : 1;
        x = pPos.x + dx;
        y = pPos.y + dy;
        if (g_engine->board->getPiece({ x,y }))
            if (bIsWhite ? !g_engine->board->getPiece({ x,y })->bIsWhite : g_engine->board->getPiece({ x,y })->bIsWhite) {
                nMoves.push_back({ x,y });
            }
    }
    return true;
}
