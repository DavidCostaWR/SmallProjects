#include "PGE_Chess.h"
/* IDEAS

++++++++++  Update screen size (not working)   ++++++++++
	bool PGE_Chess::OnUserCreate(){
		nNewScreenWidth = nScreenWidth;
		nNewScreenHeight = nScreenHeight;
		}

	bool PGE_Chess::OnUserUpdate(float fElapsedTime){
		if (GetKey(olc::Key::UP).bReleased) {
			nNewScreenHeight += 10;
		}
		if (GetKey(olc::Key::DOWN).bReleased) {
			nNewScreenHeight -= 10;
		}
		if (GetKey(olc::Key::RIGHT).bReleased) {
			nNewScreenWidth += 10;
		}
		if (GetKey(olc::Key::LEFT).bReleased) {
			nNewScreenWidth -= 10;
		}

		if (nNewScreenHeight != nScreenHeight || nNewScreenWidth != nScreenWidth) {
			SetScreenSize(nNewScreenWidth, nNewScreenHeight);
		}
	}

*/

PGE_Chess::PGE_Chess() {
	sAppName = "PGE_Chess";
}

bool PGE_Chess::OnUserCreate()
{
	// Get Screen Size
	nScreenWidth = GetScreenSize().x;
	nScreenHeight = GetScreenSize().y;

	// Set instances for other classes to have access to PGE
	//Piece::g_engine = this;

	// Called once at the start, so create things here
	board = new Board;

	// Load Sprites
	auto load = [&](std::string sName, std::string sFileName)
	{
		olc::Sprite* s = new olc::Sprite(sFileName);
		olc::Decal* d = new olc::Decal(s);
		m_mapDecals[sName] = d;
		m_mapSprites[sName] = s;
	};
	load("board", "Assets/ChessBoard.png");
	load("pieces", "Assets/ChessPieces_130_230.png");
	load("hand", "Assets/Hand.png");

	return true;
}

bool PGE_Chess::OnUserUpdate(float fElapsedTime)
{
	// Called once per frame
	return UpdateBoard(fElapsedTime);
}

bool PGE_Chess::UpdateBoard(float fElapsedTime) {

	// initializations
	int nBoardWidth = 700, nBoardHeight = 700;
	float fScaleToScreen = (float)std::min(nScreenHeight, nScreenWidth) / (float)std::max(nBoardWidth, nBoardHeight);
	float nBoardOffsetX = (nScreenWidth - nBoardWidth * fScaleToScreen) / 2;
	float nBoardOffsetY = (nScreenHeight - nBoardHeight * fScaleToScreen) / 2;
	float fTileSize = 71.5f * fScaleToScreen;
	olc::vf2d boardFrame = { (64 + 15) * fScaleToScreen, (64 - 2.5f) * fScaleToScreen };

	// Handle input
	
	// Get the current mouse position in screen space
	olc::vi2d mousePos = GetMousePos();
	
	// Calculate the mouse position in board space
	olc::vi2d MouseBoardPos = { (int)((mousePos.x - nBoardOffsetX - 0.75f * boardFrame.x) / fTileSize), (int)((mousePos.y - nBoardOffsetY - boardFrame.y) / fTileSize) };

	// Draw board centered
	DrawDecal({ nBoardOffsetX, nBoardOffsetY}, m_mapDecals["board"],{ fScaleToScreen , fScaleToScreen });

	olc::vf2d pieceSize = { 130,230 };
	olc::vf2d scale = { 0.3f * fScaleToScreen, 0.3f * fScaleToScreen };

	for (auto& piece : board->alivePieces) {

		olc::vf2d boardPosition = { nBoardOffsetX+ boardFrame.x + (float)piece->pPos.x * fTileSize, nBoardOffsetY + boardFrame.y + (float)piece->pPos.y * fTileSize };
		olc::vf2d sourcePos = { pieceSize.x * piece->nPieceType, pieceSize.y * (!piece->bIsWhite) };

		DrawPartialDecal(boardPosition, m_mapDecals["pieces"], sourcePos, pieceSize, scale);
	}

	// Draw Mouse
	if (IsFocused()) {
		if (MouseBoardPos.x >= 0 && MouseBoardPos.x < Board::WIDTH && MouseBoardPos.y >= 0 && MouseBoardPos.y < Board::HEIGHT) {
			scale = { 0.4f * fScaleToScreen, 0.4f * fScaleToScreen };
			olc::vf2d boardPosition = { nBoardOffsetX + boardFrame.x + (float)MouseBoardPos.x * fTileSize, nBoardOffsetY + boardFrame.y + ((float)MouseBoardPos.y - 0.5f) * fTileSize };
			DrawDecal(boardPosition, m_mapDecals["hand"], scale);
		}
	}

	

	return true;
}