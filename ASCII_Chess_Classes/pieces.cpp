#include "Pieces.h"
#include "Position.h"

Piece::Piece(){
    // Default constructor implementation
}

Piece::~Piece(){
    // Destructor implementation
}

// ************ KING

King::King(bool isWhite, Position pos){
    bIsWhite = isWhite;
    pPos = pos;
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
