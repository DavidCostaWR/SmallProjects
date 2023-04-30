#pragma once

#ifndef PGE_CHESS_H
#define PGE_CHESS_H

#include "../olcPixelGameEngine.h"
#include "Position.h"
#include "Pieces.h"
#include "Board.h"
#include <vector>
#include <iostream>

class Board;
class Piece;

const std::string cPieceType[6] = {"KING", "QUEEN", "ROOK", "KNIGHT", "BISHOP", "PAWN"};

class PGE_Chess : public olc::PixelGameEngine
{
public:
	PGE_Chess();
	const int test1 = 3;
	std::vector <int> test;
	std::map<std::string, olc::Decal*> m_mapDecals;
	int nScreenWidth, nScreenHeight;
	int nNewScreenWidth, nNewScreenHeight;
	float fScaleToScreen;
	int nGamePhase;
	bool bGameOver;
	enum GamePhase {ChoosePiece, MovePiece, PlacePiece};

	float nBoardWidth, nBoardHeight;
	float nBoardOffsetX, nBoardOffsetY, fTileSize;
	olc::vf2d boardFrame;

	bool InGameBounds(int x, int y);
	Board* board = nullptr;

	int turn;

	template<typename T>
	void Log(std::string name, const T& value) {
		std::cout << name << ": " << value << std::endl;
	}
	void Log(std::string name, const olc::vi2d value) {
		std::cout << name << ": (" << value.x << ", " << value.y << ")" << std::endl;
	}
	void Log(std::string name, const olc::vf2d value) {
		std::cout << name << ": (" << value.x << ", " << value.y << ")" << std::endl;
	}
	void Log(std::string name) {
		std::cout << name << std::endl;
	}


private:
	std::map<std::string, olc::Sprite*> m_mapSprites;
	Piece* activePiece = nullptr;

protected:
	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;
	bool UpdateBoard(float fElapsedTime);
	bool EndGame(float fElapsedTime);
};
#endif // !PGE_CHESS_H
