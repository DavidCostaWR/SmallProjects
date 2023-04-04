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
    // King constructor implementation
}

King::~King(){
    // King destructor implementation
}

bool King::isValidMove(Position from, Position to) {
    // Place the King's move behavior here
    return true;
}

wchar_t King::getSymbol() {
    return bIsWhite ? 'K' : 'k';
}

// ************ QUEEN

Queen::Queen(bool isWhite, Position pos) {
    // Queen constructor implementation
}

Queen::~Queen() {
    // Queen destructor implementation
}

bool Queen::isValidMove(Position from, Position to) {
    // Place the Queen's move behavior here
    return true;
}

wchar_t Queen::getSymbol() {
    return bIsWhite ? 'Q' : 'q';
}

// ************ ROOK

Rook::Rook(bool isWhite, Position pos) {
    // Rook destructor implementation
}

Rook::~Rook() {
    // Rook destructor implementation
}

bool Rook::isValidMove(Position from, Position to) {
    // Place the Rook's move behavior here
    return true;
};

wchar_t Rook::getSymbol() {
    return bIsWhite ? 'R' : 'r';
}

// ************ KNIGH

Knight::Knight(bool isWhite, Position pos) {
    // Knight destructor implementation
}

Knight::~Knight() {
    // Knight destructor implementation
}

bool Knight::isValidMove(Position from, Position to) {
    // Place the Knight's move behavior here
    return true;
};

wchar_t Knight::getSymbol() {
    return bIsWhite ? 'N' : 'n';
}

// ************ BISHOP

Bishop::Bishop(bool isWhite, Position pos) {
    // Bishop destructor implementation
}

Bishop::~Bishop() {
    // Bishop destructor implementation
}

bool Bishop::isValidMove(Position from, Position to) {
    // Place the Bishop's move behavior here
    return true;
};

wchar_t Bishop::getSymbol() {
    return bIsWhite ? 'B' : 'b';
}

// ************ PAWN

Pawn::Pawn(bool isWhite, Position pos) {
    // Pawn destructor implementation
}

Pawn::~Pawn() {
    // Pawn destructor implementation
}

bool Pawn::isValidMove(Position from, Position to) {
    // Place the Pawn's move behavior here
    return true;
};

wchar_t Pawn::getSymbol() {
    return bIsWhite ? 'P' : 'p';
}
