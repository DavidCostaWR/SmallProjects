#include "Pieces.h"
//#include "PGE_Chess.h"

//PGE_Chess* Piece::g_engine = nullptr;

Piece::Piece(){
    // Default constructor implementation
    bIsAlive = true;
}

Piece::~Piece(){
    // Destructor implementation
}

//void Piece::DrawSelf(olc::PixelGameEngine* gfx) {
    //olc::vf2d boardFrame = { 64 + 15, 64 - 2.5 };
    //float squareSize = 71.5f;
    //olc::vf2d pieceSize = { 26,46 };
    //olc::vf2d scale = { 1.5f, 1.5f };

    //olc::vf2d boardPosition = { boardFrame.x + (float)pPos.x * squareSize,boardFrame.y + (float)pPos.y * squareSize };
    //olc::vf2d sourcePos = { pieceSize.x * nPieceType, pieceSize.y * (!bIsWhite) };

    //g_engine->DrawPartialDecal(boardPosition, g_engine->m_mapDecals["pieces"], sourcePos, pieceSize, scale);
//}
// ************ KING

King::King(bool isWhite, Position pos){
    bIsWhite = isWhite;
    pPos = pos;
    nPieceType = KING;
}

King::~King(){
    // King destructor implementation
}

bool King::isValidMove(Position from, Position to) {
    // Place the King's move behavior here
    return true;
}

wchar_t King::getSymbol() {
    return 'K';
}

// ************ QUEEN

Queen::Queen(bool isWhite, Position pos) {
    bIsWhite = isWhite;
    pPos = pos;
    nPieceType = QUEEN;
}

Queen::~Queen() {
    // Queen destructor implementation
}

bool Queen::isValidMove(Position from, Position to) {
    // Place the Queen's move behavior here
    return true;
}

wchar_t Queen::getSymbol() {
    return 'Q';
}

// ************ ROOK

Rook::Rook(bool isWhite, Position pos) {
    bIsWhite = isWhite;
    pPos = pos;
    nPieceType = ROOK;
}

Rook::~Rook() {
    // Rook destructor implementation
}

bool Rook::isValidMove(Position from, Position to) {
    // Place the Rook's move behavior here
    return true;
};

wchar_t Rook::getSymbol() {
    return 'R';
}

// ************ KNIGH

Knight::Knight(bool isWhite, Position pos) {
    bIsWhite = isWhite;
    pPos = pos;
    nPieceType = KNIGHT;
}

Knight::~Knight() {
    // Knight destructor implementation
}

bool Knight::isValidMove(Position from, Position to) {
    // Place the Knight's move behavior here
    return true;
};

wchar_t Knight::getSymbol() {
    return 'N';
}

// ************ BISHOP

Bishop::Bishop(bool isWhite, Position pos) {
    bIsWhite = isWhite;
    pPos = pos;
    nPieceType = BISHOP;
}

Bishop::~Bishop() {
    // Bishop destructor implementation
}

bool Bishop::isValidMove(Position from, Position to) {
    // Place the Bishop's move behavior here
    return true;
};

wchar_t Bishop::getSymbol() {
    return 'B';
}

// ************ PAWN

Pawn::Pawn(bool isWhite, Position pos) {
    bIsWhite = isWhite;
    pPos = pos;
    nPieceType = PAWN;
}

Pawn::~Pawn() {
    // Pawn destructor implementation
}

bool Pawn::isValidMove(Position from, Position to) {
    // Place the Pawn's move behavior here
    return true;
};

wchar_t Pawn::getSymbol() {
    return 'P';
}
