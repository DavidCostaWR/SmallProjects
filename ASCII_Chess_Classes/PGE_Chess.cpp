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
	// Initializations
	turn = 1;
	nGamePhase = ChoosePiece;
	nBoardWidth = 700, nBoardHeight = 700;
	activePiece = nullptr;
	bGameOver = false;

	// Get Screen Size
	nScreenWidth = GetScreenSize().x;
	nScreenHeight = GetScreenSize().y;

	// Set instances for other classes to have access to PGE
	Piece::g_engine = this;
	Board::g_engine = this;

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
	if (bGameOver)
		return EndGame(fElapsedTime);
	else
		return UpdateBoard(fElapsedTime);
}

bool PGE_Chess::UpdateBoard(float fElapsedTime) {


	// update board dimensions
	fScaleToScreen = (float)std::min(nScreenHeight, nScreenWidth) / (float)std::max(nBoardWidth, nBoardHeight);
	nBoardOffsetX = (nScreenWidth - nBoardWidth * fScaleToScreen) / 2;
	nBoardOffsetY = (nScreenHeight - nBoardHeight * fScaleToScreen) / 2;
	fTileSize = 71.5f * fScaleToScreen;
	boardFrame = { (64 + 15) * fScaleToScreen, (64 - 2.5f) * fScaleToScreen };

	// Get the current mouse position in screen space
	olc::vi2d mousePos = GetMousePos();
	
	// Calculate the mouse position in board space
	olc::vi2d MouseBoardPos = { (int)((mousePos.x - nBoardOffsetX - 0.75f * boardFrame.x) / fTileSize), (int)((mousePos.y - nBoardOffsetY - boardFrame.y) / fTileSize) };
	
	// Handle input
	if (IsFocused()) {
		if (GetMouse(olc::Mouse::LEFT).bReleased) {		
						
			// update all pieces possible moves
			for (auto& piece : board->alivePieces) {
				piece->getMoves();
			}

			// for even turns, mirror the y position of mouse
			if (turn % 2 == 0)
			{
				MouseBoardPos.y = board->COLUMNS - 1 - (int)((mousePos.y - nBoardOffsetY - boardFrame.y) / fTileSize);
			}

			if (nGamePhase == ChoosePiece) {
				Piece* tempPiece = board->getPiece(MouseBoardPos);

				// Check if tile has piece
				if (!(tempPiece == nullptr)) {

					// Select piece if color matches active turn
					if (turn % 2 == tempPiece->bIsWhite) {
						tempPiece->bSelected = true;
						activePiece = tempPiece;
						tempPiece = nullptr;
						nGamePhase++;
					}

				}
			}

			else if (nGamePhase == MovePiece) {

				// Check for legal move
				if (board->isMoveLegal(activePiece, MouseBoardPos)) {
					nGamePhase = PlacePiece;
				}

				// If not, choose another piece
				else {
					activePiece->bSelected = false;
					activePiece = nullptr;
					nGamePhase = ChoosePiece;
				}

			}

			if (nGamePhase == PlacePiece) {

				// Remove piece if captured
				Piece* pieceToRemove = nullptr;
				pieceToRemove = board->getPiece(MouseBoardPos);
				if (pieceToRemove != nullptr) {
					if (pieceToRemove->nPieceType == Piece::PIECE_TYPE::KING)
						bGameOver = true;
					else
						pieceToRemove->bIsAlive = false;
				}
				Log("PiecePosition", activePiece->pPos);
				Log("Mouse Position", MouseBoardPos);
				// Place piece in new position
				activePiece->pPos = MouseBoardPos;

				Log("PiecePosition", activePiece->pPos);
				Log("Mouse Position", MouseBoardPos);
				// Cleanup
				activePiece->bSelected = false;
				activePiece = nullptr;
				board->updateBoard();
				nGamePhase = ChoosePiece;
				turn += 1;
			}
		}
	}

	// Draw board centered
	DrawDecal({ nBoardOffsetX, nBoardOffsetY}, m_mapDecals["board"],{ fScaleToScreen , fScaleToScreen });

	olc::vf2d scale = { 0.3f * fScaleToScreen, 0.3f * fScaleToScreen };
	olc::vf2d boardPosition = { nBoardOffsetX + boardFrame.x + (float)MouseBoardPos.x * fTileSize, nBoardOffsetY + boardFrame.y + ((float)MouseBoardPos.y - 0.5f) * fTileSize };
			
	// Draw Pieces
	float a = 3.0f;
	olc::vf2d tempPosition = boardPosition;
	tempPosition.y += 30 * fScaleToScreen;
	// Fixed pieces
	for (auto& piece : board->alivePieces) {
		if(!piece->bSelected)
			piece->DrawPiece(scale, nGamePhase, tempPosition, MouseBoardPos);
	}
	// Possible moves
	if (nGamePhase == MovePiece) {
		olc::vf2d fPos = { 0,0 };
		for (auto& pos : activePiece->nMoves) {
			if (!(turn % 2)) {
				fPos = { nBoardOffsetX + 0.81f * boardFrame.x + pos.x * fTileSize, nBoardOffsetY + 1.055f * boardFrame.y + (board->ROWS - 1 - pos.y) * fTileSize };
			}
			if (turn % 2) {
				fPos = { nBoardOffsetX + 0.81f * boardFrame.x + pos.x * fTileSize, nBoardOffsetY + 1.055f * boardFrame.y + pos.y * fTileSize };
			}
			FillRectDecal(fPos, { fTileSize * .99f, fTileSize * .99f }, olc::Pixel(255, 0, 255, 35));
		}
	}

	// Selected piece
	for (auto& piece : board->alivePieces) {
		if (piece->bSelected)
			piece->DrawPiece(scale, nGamePhase, tempPosition, MouseBoardPos);
	}
	// Draw	Mouse
	if (IsFocused()) {
		if (MouseBoardPos.x >= 0 && MouseBoardPos.x < Board::COLUMNS && MouseBoardPos.y >= 0 && MouseBoardPos.y < Board::ROWS) {
			scale = { 0.4f * fScaleToScreen, 0.4f * fScaleToScreen };
			DrawDecal(boardPosition, m_mapDecals["hand"], scale);
		}
	}

	return true;
}

bool PGE_Chess::InGameBounds(int x, int y) {
	return x >= 0 && x < board->COLUMNS && y >= 0 && y < board->ROWS;
}

bool PGE_Chess::EndGame(float fElapsedTime){
	DrawDecal({ nBoardOffsetX, nBoardOffsetY }, m_mapDecals["board"], { fScaleToScreen , fScaleToScreen }, olc::Pixel(255, 255, 255, 127));
	const std::string endGameMessage[2] = { "Press SPACE for NEW GAME", "Press ESC to EXIT" };
	olc::vf2d fCharScale = { 2.0f, 2.0f };
	olc::vf2d fCharDim = { 8.0f * fCharScale.x, 8.0f * fCharScale.y };

	for (int i = 0; i < std::size(endGameMessage); i++) {

		FillRectDecal({ ScreenWidth() / 2 - endGameMessage[i].length() * fCharDim.x / 2 - 5,
			(ScreenHeight() - std::size(endGameMessage) * (fCharDim.y + 5)) / 2 + i * (fCharDim.y + 5) - 5},
			{ endGameMessage[i].length() * fCharDim.x + 10, fCharDim.y + 10},
			olc::Pixel(255,255,255,100));
		DrawStringDecal({ ScreenWidth() / 2 - endGameMessage[i].length() * fCharDim.x / 2,
			(ScreenHeight() - std::size(endGameMessage) * (fCharDim.y + 5)) / 2 + i * (fCharDim.y + 5) },
			endGameMessage[i],
			olc::VERY_DARK_GREEN,
			fCharScale);
	}
	if (GetKey(olc::Key::SPACE).bReleased) {
		// Initializations
		turn = 1;
		nGamePhase = ChoosePiece;
		activePiece = nullptr;
		bGameOver = false;

		// Called once at the start, so create things here (consider implementing a resetBoard() functin in the class.
		delete board;
		board = new Board;
	};

	if (GetKey(olc::Key::ESCAPE).bReleased)
		return false;

	return true;
}