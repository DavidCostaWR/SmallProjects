#pragma once

#ifndef PIECES_H
#define PIECES_H

#include "Position.h"
#include "PGE_Chess.h"
#include <list>

class PGE_Chess;

class Piece
{
public:
	Piece(); // constructor
	~Piece(); // destructor

public:
	int nPieceType;
	bool bIsWhite, bIsAlive, bSelected;
	olc::vf2d pPos;
	const olc::vf2d pieceSize = { 130,230 };
	//olc::vf2d scale = { 1.5f, 1.5f };
	std::list<olc::vi2d> nMoves;

	enum PIECE_TYPE { KING, QUEEN, ROOK, KNIGHT, BISHOP, PAWN };


public:
	virtual bool isValidMove(olc::vf2d from, olc::vf2d to) = 0;
	virtual bool getMoves() = 0;
	void DrawPiece(olc::vf2d scale, int Phase, olc::vf2d mousePos, olc::vi2d boardPos);

	static PGE_Chess* g_engine;
};

class King : public Piece
{
public:
	King(bool isWhite, olc::vf2d pos); // constructor
	~King(); // destructor

	bool isValidMove(olc::vf2d from, olc::vf2d to) override;
	bool getMoves() override;
};

class Queen : public Piece
{
public:
	Queen(bool isWhite, olc::vf2d pos); // constructor
	~Queen(); // destructor

	bool isValidMove(olc::vf2d from, olc::vf2d to) override;
	bool getMoves() override;
};

class Rook : public Piece
{
public:
	Rook(bool isWhite, olc::vf2d pos); // constructor
	~Rook(); // destructor

	bool isValidMove(olc::vf2d from, olc::vf2d to) override;
	bool getMoves() override;
};

class Knight : public Piece
{
public:
	Knight(bool isWhite, olc::vf2d pos); // constructor
	~Knight(); // destructor

	bool isValidMove(olc::vf2d from, olc::vf2d to) override;
	bool getMoves() override;
};

class Bishop : public Piece
{
public:
	Bishop(bool isWhite, olc::vf2d pos); // constructor
	~Bishop(); // destructor

	bool isValidMove(olc::vf2d from, olc::vf2d to) override;
	bool getMoves() override;
};

class Pawn : public Piece
{
public:
	Pawn(bool isWhite, olc::vf2d pos); // constructor
	~Pawn(); // destructor

	bool isValidMove(olc::vf2d from, olc::vf2d to) override;
	bool getMoves() override;
};


#endif // PIECES_H