#ifndef PIECES_H
#define PIECES_H
#include "Position.h"

class Piece
{
public:
	Piece(); // constructor
	~Piece(); // destructor

	bool bIsWhite;
	Position pPos;
	bool bIsAlive = true;

	virtual bool isValidMove(Position from, Position to) = 0;
	virtual wchar_t getSymbol() = 0;
};

class King : public Piece
{
public:
	King(bool isWhite, Position pos); // constructor
	~King(); // destructor

	bool isValidMove(Position from, Position to) override;
	wchar_t getSymbol() override;
};

class Queen : public Piece
{
public:
	Queen(bool isWhite, Position pos); // constructor
	~Queen(); // destructor

	bool isValidMove(Position from, Position to) override;
	wchar_t getSymbol() override;
};

class Rook : public Piece
{
public:
	Rook(bool isWhite, Position pos); // constructor
	~Rook(); // destructor

	bool isValidMove(Position from, Position to) override;
	wchar_t getSymbol() override;
};

class Knight : public Piece
{
public:
	Knight(bool isWhite, Position pos); // constructor
	~Knight(); // destructor

	bool isValidMove(Position from, Position to) override;
	wchar_t getSymbol() override;
};

class Bishop : public Piece
{
public:
	Bishop(bool isWhite, Position pos); // constructor
	~Bishop(); // destructor

	bool isValidMove(Position from, Position to) override;
	wchar_t getSymbol() override;
};

class Pawn : public Piece
{
public:
	Pawn(bool isWhite, Position pos); // constructor
	~Pawn(); // destructor

	bool isValidMove(Position from, Position to) override;
	wchar_t getSymbol() override;
};


#endif // PIECES_H