#pragma once
#include "Chess.h"

bool InGameBounds(Position P)
{
	return (P.x >= 0 && P.x < nBoardWidth && P.y >= 0 && P.y < nBoardHeight);
}

bool IsBlackPiece(Position P)
{
	if (P.x >= 0 && P.x < nBoardWidth && P.y >= 0 && P.y < nBoardHeight)
	{
		wchar_t piece = board[P.x + P.y * nBoardWidth];
		return (piece == 'p' || piece == 'n' || piece == 'b' || piece == 'r' || piece == 'q' || piece == 'k');
	}
	return false;
}

bool IsWhitePiece(Position P)
{
	if (P.x >= 0 && P.x < nBoardWidth && P.y >= 0 && P.y < nBoardHeight)
	{
		wchar_t piece = board[P.x + P.y * nBoardWidth];
		return (piece == 'P' || piece == 'N' || piece == 'B' || piece == 'R' || piece == 'Q' || piece == 'K');
	}
	return false;
}

bool IsBlankSpace(Position P)
{
	return (board[P.x + P.y * nBoardWidth] == ' ');
}

cPiece* GetPiece(Position position)
{
	for (cPiece* piece : pieces)
	{
		if (piece->GetPosition() == position)
		{
			return piece;
		}
	}
	return nullptr;
}

class cPiece {
public:
	cPiece(PieceType type, Position position, bool bIsWhite)
		: m_Type(type), m_Position(position), m_IsWhite(bIsWhite), m_NumMoves(0) {}
	virtual ~cPiece() {}

	virtual void GetValidMoves(list <Position>& moves) = 0;
	virtual bool MovePiece(Position newPos) = 0;

	PieceType GetType() { return m_Type; }
	Position GetPosition() { return m_Position; }
	bool IsWhite() { return m_IsWhite; }
	int GetNumMoves() { return m_NumMoves; }
	void IncrementNumMoves() { m_NumMoves++; }

protected:
	PieceType m_Type;
	Position m_Position;
	bool m_IsWhite;
	int m_NumMoves;
};

// Class representing a pawn piece
class cPawn : public cPiece {
public:
	// Constructor, ': cPiece()' inherits the members and functionality of cPiece and properly initializes them.
	cPawn(Position position, bool bIsWhite) : cPiece(PAWN, position, bIsWhite) {}

	void GetValidMoves(list <Position>& moves) override {
		int x = m_Position.x;
		int y = m_Position.y;

		// Determines the direction based on color
		int dir = m_IsWhite ? -1 : 1;
		Position nextPos = { x, y + dir };

		// Check forward movement (TODO: Check for checkmate)
		if (InGameBounds(nextPos) && IsBlankSpace(nextPos)) {
			moves.push_back(nextPos);
			if (m_NumMoves == 0) {
				Position nextNextPos = { x, y + dir * 2 };
				if (InGameBounds(nextNextPos) && IsBlankSpace(nextNextPos))
					moves.push_back(nextNextPos);
			}
		}

		// Check diagonal captures
		for (int dx = -1; dx <= 1; dx += 2) {
			Position capture = { x + dx, y + dir };
			if (InGameBounds(capture) && ((m_IsWhite && IsBlackPiece(capture)) || (!m_IsWhite && IsWhitePiece(capture)))) {
				moves.push_back(capture);
			}
		}

		// Check en passant
		if (y == m_IsWhite ? (nBPawnStartRow + 1) : (nWPawnStartRow - 1)) {
			for (int dx = -1; dx <= 1; dx += 2) {
				Position capture = { x + dx, y };
				if (InGameBounds(capture) && ((m_IsWhite && IsBlackPiece(capture)) || (!m_IsWhite && IsWhitePiece(capture)))) {
					cPiece* piece = GetPiece(capture);
					if (piece && piece->GetType() == PAWN && piece->GetNumMoves() == 1) {
						moves.push_back(capture);
					}
				}
			}
		}
	}
	bool MovePiece(Position newPos)
	{
		// Check if move is valid
		list<Position> moves;
		GetValidMoves(moves);
		for (auto& move : moves) {}
	}
};