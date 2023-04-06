#pragma once

#include "../olcPixelGameEngine.h"
//#include "Pieces.h"
#include "Board.h"

class PGE_Chess : public olc::PixelGameEngine
{
public:
	PGE_Chess();
	std::map<std::string, olc::Decal*> m_mapDecals;
	int nScreenWidth, nScreenHeight;
	int nNewScreenWidth, nNewScreenHeight;
private:
	std::map<std::string, olc::Sprite*> m_mapSprites;
	Board* board = nullptr;
	//Piece* piece = nullptr;
protected:
	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;
	bool UpdateBoard(float fElapsedTime);
};