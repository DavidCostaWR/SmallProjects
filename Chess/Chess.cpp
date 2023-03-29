#include <iostream>
#include <vector>
#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <string>
#include <list>

//#include "..\olcConsoleGameEngine.h"
using namespace std;

enum boardColumn {
	a = 1, b, c, d, e, f, g, h
};
enum GamePhase {
	CHOOSE_PIECE = 0, PLACE_PIECE, END_TURN
};

const int nScreenWidth = 120, nScreenHeight = 30;
const int nBoardWidth = 8, nBoardHeight = 8;

wchar_t *board = new wchar_t[nBoardWidth * nBoardHeight];
wchar_t tempPiece;

int nTurn = 1, nGamePhase = CHOOSE_PIECE;
bool bGame, prevLeftArrowState, prevRightArrowState, prevUpArrowState, prevDownArrowState, prevSpaceBarState;

bool leftArrowReleased = false;
bool rightArrowReleased = false;
bool upArrowReleased = false;
bool downArrowReleased = false;
bool spaceBarReleased = false;

string sPlayer1;
string sPlayer2;

struct Position { int x = 0; int y = 0;};

list<Position> PiecePlacement;
list<wchar_t> CapturedP1;
list<wchar_t> CapturedP2;

Position cursorPosition = { 0, 7 };
Position selectedPiecePosition;


void vRefreshDisplay(wchar_t* screen)
{
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0, 0 }, &dwBytesWritten);
}

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

void ClearBoard()
{
	for (int x = 0; x < nBoardWidth; x++)
	{
		for (int y = 0; y < nBoardHeight; y++)
			board[x + y * nBoardWidth] = L' ';
	}
}

void ClearScreen(wchar_t* screen)
{
	for (int i = 0; i < nScreenWidth * nScreenHeight; i++)
		screen[i] = L' ';
}

void PlacePiece(wchar_t Piece, int x, int y)
{
	board[x - 1 + (y - 1) * nBoardWidth] = Piece;
};

void DrawBoard()
{
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			wcout << board[x + y * 8] << " ";
		}
		cout << endl;
	}
}

bool UpdateKeyPress()
{
		// Check if the key is currently pressed
		bool currLeftArrowState = (GetAsyncKeyState(VK_LEFT) & 0x8000);
		bool currRightArrowState = (GetAsyncKeyState(VK_RIGHT) & 0x8000);
		bool currUpArrowState = (GetAsyncKeyState(VK_UP) & 0x8000);
		bool currDownArrowState = (GetAsyncKeyState(VK_DOWN) & 0x8000);
		bool currSpaceBarState = (GetAsyncKeyState(VK_SPACE) & 0x8000);

		// Check if the key was released
		leftArrowReleased = (prevLeftArrowState && !currLeftArrowState);
		rightArrowReleased = (prevRightArrowState && !currRightArrowState);
		upArrowReleased = (prevUpArrowState && !currUpArrowState);
		downArrowReleased = (prevDownArrowState && !currDownArrowState);
		spaceBarReleased = (prevSpaceBarState && !currSpaceBarState);

		// Update the previous state of key
		prevLeftArrowState = currLeftArrowState;
		prevRightArrowState = currRightArrowState;
		prevUpArrowState = currUpArrowState;
		prevDownArrowState = currDownArrowState;
		prevSpaceBarState = currSpaceBarState;

		if (rightArrowReleased)
		{
			cursorPosition.x++;
			if (cursorPosition.x > 7)
			{
				cursorPosition.x = 0;
				cursorPosition.y++;
			}
			if (cursorPosition.y > 7)
				cursorPosition.y = 0;
			return true;
		}

		if (leftArrowReleased)
		{
			cursorPosition.x--;
			if (cursorPosition.x < 0)
			{
				cursorPosition.x = 7;
				cursorPosition.y--;
			}
			if (cursorPosition.y < 0)
				cursorPosition.y = 7;
			return true;
		}

		if (downArrowReleased)
		{
			cursorPosition.y++;
			if (cursorPosition.y > 7)
			{
				cursorPosition.y = 0;
				cursorPosition.x++;
			}
			if (cursorPosition.x > 7)
				cursorPosition.x = 0;
			return true;
		}

		if (upArrowReleased)
		{
			cursorPosition.y--;
			if (cursorPosition.y < 0)
			{
				cursorPosition.y = 7;
				cursorPosition.x--;
			}
			if (cursorPosition.x < 0)
				cursorPosition.x = 7;
			return true;
		}

		if (spaceBarReleased)
			return true;
		return false;
}

void ResetBoard()
{
	prevLeftArrowState = false;
	prevRightArrowState = false;
	prevUpArrowState = false;
	prevDownArrowState = false;
	CapturedP1.clear();
	CapturedP2.clear();


	//Clear Board
	ClearBoard();

	//Place pawns
	for (int i = 1; i <= 8; i++)
	{
		PlacePiece('P', i, 7);
		PlacePiece('p', i, 2);
	}

	//Place Black Pieces
	PlacePiece('r', a, 1);
	PlacePiece('n', b, 1);
	PlacePiece('b', c, 1);
	PlacePiece('q', d, 1);
	PlacePiece('k', e, 1);
	PlacePiece('b', f, 1);
	PlacePiece('n', g, 1);
	PlacePiece('r', h, 1);

	//Place White Pieces
	PlacePiece('R', a, 8);
	PlacePiece('N', b, 8);
	PlacePiece('B', c, 8);
	PlacePiece('Q', d, 8);
	PlacePiece('K', e, 8);
	PlacePiece('B', f, 8);
	PlacePiece('N', g, 8);
	PlacePiece('R', h, 8);
};

void GameOver(string player, wchar_t* screen)
{
	const string GameOver = "Congratulations " + player + ". You have won the match";
	const string Exit = "Press SPACE to exit";
	for (int i = 0; i < nScreenWidth; i++)
	{
		for (int j = -3; j < 4; j++)
		{
			screen[i + (nScreenHeight / 2 + j) * nScreenWidth] = ' ';
		}
	}
	for(int i = 0; i < GameOver.size(); i++)
		screen[(nScreenWidth - GameOver.size()) / 2 + i + (nScreenHeight / 2 - 1) * nScreenWidth] = GameOver[i];
	for (int i = 0; i < Exit.size(); i++)
		screen[(nScreenWidth - Exit.size()) / 2 + i + (nScreenHeight / 2 + 1) * nScreenWidth] = Exit[i];
	bGame = false;
	while(!spaceBarReleased){}
}

int main()
{
		// Use current time as seed for random generator
		srand(time(0));
		wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
		screen[nScreenWidth * nScreenHeight - 1] = '\0';

	while(true)
	{
		ClearBoard();
		// Collect player names and randomize order
		cout << "Player 1: ";
		cin >> sPlayer1;
		cout << "Player 2: ";
		cin >> sPlayer2;
		if (rand() % 2 == 1)
		{
			string temp = sPlayer1;
			sPlayer1 = sPlayer2;
			sPlayer2 = temp;
		}

		ResetBoard();
		bGame = true;

		while (bGame)
		{

			// Game Mechanics
			switch (nGamePhase)
			{
			case(CHOOSE_PIECE):
				if (spaceBarReleased)
				{
					if (nTurn % 2 == 1 && !IsWhitePiece(cursorPosition))
						break;
					if (nTurn % 2 == 0 && !IsBlackPiece(cursorPosition))
						break;
					selectedPiecePosition = cursorPosition;
					nGamePhase++;
					break;
				}
				PiecePlacement.clear();
				if (nTurn % 2 == 1)
				{
					int x = cursorPosition.x, y = cursorPosition.y;
					Position New;
					bool PieceMoved = false;
					// PAWN
					if (board[x + y * nBoardWidth] == 'P')
					{
						// Top Row : TODO - Choose piece to switch
						if (y == 0)
							break;

						// 1 row
						New = { x, y - 1 };
						if (board[New.x + New.y * nBoardWidth] == ' ')
							PiecePlacement.push_back(New);

						// 2 rows
						New = { x, y - 2 };
						if (y == 6 && board[x + (y - 1) * nBoardWidth] == ' ')
							PiecePlacement.push_back(New);

						// Diagonal
						New = { x + 1 , y - 1 };
						if (IsBlackPiece(New))
							PiecePlacement.push_back(New);
						New = { x - 1 , y - 1 };
						if (IsBlackPiece(New))
							PiecePlacement.push_back(New);

						break;
					}
					// KNIGHT
					if (board[x + y * nBoardWidth] == 'N')
					{
						for (int X = -2; X < 3; X++)
						{
							for (int Y = -2; Y < 3; Y++)
							{
								if (abs(X) + abs(Y) == 3)
								{
									New.x = x + X; New.y = y + Y;
									if (InGameBounds(New))
										if (board[New.x + New.y * nBoardWidth] == ' ' || IsBlackPiece(New))
											PiecePlacement.push_back(New);
								}
							}
						}
						break;
					}
					// BISHOP
					if (board[x + y * nBoardWidth] == 'B')
					{
						// NE
						bool hitPiece = false;
						New = cursorPosition;
						while (!hitPiece)
						{
							New = { New.x + 1, New.y - 1 };
							if (!hitPiece && !IsWhitePiece(New) && InGameBounds(New))
							{
								PiecePlacement.push_back(New);
								if (IsBlackPiece(New))
									hitPiece = true;
							}
							else
								hitPiece = true;
						}
						// SE
						hitPiece = false;
						New = cursorPosition;
						while (!hitPiece)
						{
							New = { New.x + 1, New.y + 1 };
							if (!hitPiece && !IsWhitePiece(New) && InGameBounds(New))
							{
								PiecePlacement.push_back(New);
								if (IsBlackPiece(New))
									hitPiece = true;
							}
							else
								hitPiece = true;
						}
						// SW
						hitPiece = false;
						New = cursorPosition;
						while (!hitPiece)
						{
							New = { New.x - 1, New.y + 1 };
							if (!hitPiece && !IsWhitePiece(New) && InGameBounds(New))
							{
								PiecePlacement.push_back(New);
								if (IsBlackPiece(New))
									hitPiece = true;
							}
							else
								hitPiece = true;
						}
						// NW
						hitPiece = false;
						New = cursorPosition;
						while (!hitPiece)
						{
							New = { New.x - 1, New.y - 1 };
							if (!hitPiece && !IsWhitePiece(New) && InGameBounds(New))
							{
								PiecePlacement.push_back(New);
								if (IsBlackPiece(New))
									hitPiece = true;
							}
							else
								hitPiece = true;
						}
						break;
					}
					// ROOK
					if (board[x + y * nBoardWidth] == 'R')
					{
						// N
						bool hitPiece = false;
						New = cursorPosition;
						while (!hitPiece)
						{
							New = { New.x, New.y - 1 };
							if (!hitPiece && !IsWhitePiece(New) && InGameBounds(New))
							{
								PiecePlacement.push_back(New);
								if (IsBlackPiece(New))
									hitPiece = true;
							}
							else
								hitPiece = true;
						}
						// S
						hitPiece = false;
						New = cursorPosition;
						while (!hitPiece)
						{
							New = { New.x, New.y + 1 };
							if (!hitPiece && !IsWhitePiece(New) && InGameBounds(New))
							{
								PiecePlacement.push_back(New);
								if (IsBlackPiece(New))
									hitPiece = true;
							}
							else
								hitPiece = true;
						}
						// E
						hitPiece = false;
						New = cursorPosition;
						while (!hitPiece)
						{
							New = { New.x + 1, New.y };
							if (!hitPiece && !IsWhitePiece(New) && InGameBounds(New))
							{
								PiecePlacement.push_back(New);
								if (IsBlackPiece(New))
									hitPiece = true;
							}
							else
								hitPiece = true;
						}
						// W
						hitPiece = false;
						New = cursorPosition;
						while (!hitPiece)
						{
							New = { New.x - 1, New.y };
							if (!hitPiece && !IsWhitePiece(New) && InGameBounds(New))
							{
								PiecePlacement.push_back(New);
								if (IsBlackPiece(New))
									hitPiece = true;
							}
							else
								hitPiece = true;
						}
						break;
						// Todo: Castling
					}
					// QUEEN
					if (board[x + y * nBoardWidth] == 'Q')
					{
						// N
						bool hitPiece = false;
						New = cursorPosition;
						while (!hitPiece)
						{
							New = { New.x, New.y - 1 };
							if (!hitPiece && !IsWhitePiece(New) && InGameBounds(New))
							{
								PiecePlacement.push_back(New);
								if (IsBlackPiece(New))
									hitPiece = true;
							}
							else
								hitPiece = true;
						}
						// S
						hitPiece = false;
						New = cursorPosition;
						while (!hitPiece)
						{
							New = { New.x, New.y + 1 };
							if (!hitPiece && !IsWhitePiece(New) && InGameBounds(New))
							{
								PiecePlacement.push_back(New);
								if (IsBlackPiece(New))
									hitPiece = true;
							}
							else
								hitPiece = true;
						}
						// E
						hitPiece = false;
						New = cursorPosition;
						while (!hitPiece)
						{
							New = { New.x + 1, New.y };
							if (!hitPiece && !IsWhitePiece(New) && InGameBounds(New))
							{
								PiecePlacement.push_back(New);
								if (IsBlackPiece(New))
									hitPiece = true;
							}
							else
								hitPiece = true;
						}
						// W
						hitPiece = false;
						New = cursorPosition;
						while (!hitPiece)
						{
							New = { New.x - 1, New.y };
							if (!hitPiece && !IsWhitePiece(New) && InGameBounds(New))
							{
								PiecePlacement.push_back(New);
								if (IsBlackPiece(New))
									hitPiece = true;
							}
							else
								hitPiece = true;
						}

						// NE
						hitPiece = false;
						New = cursorPosition;
						while (!hitPiece)
						{
							New = { New.x + 1, New.y - 1 };
							if (!hitPiece && !IsWhitePiece(New) && InGameBounds(New))
							{
								PiecePlacement.push_back(New);
								if (IsBlackPiece(New))
									hitPiece = true;
							}
							else
								hitPiece = true;
						}
						// SE
						hitPiece = false;
						New = cursorPosition;
						while (!hitPiece)
						{
							New = { New.x + 1, New.y + 1 };
							if (!hitPiece && !IsWhitePiece(New) && InGameBounds(New))
							{
								PiecePlacement.push_back(New);
								if (IsBlackPiece(New))
									hitPiece = true;
							}
							else
								hitPiece = true;
						}
						// SW
						hitPiece = false;
						New = cursorPosition;
						while (!hitPiece)
						{
							New = { New.x - 1, New.y + 1 };
							if (!hitPiece && !IsWhitePiece(New) && InGameBounds(New))
							{
								PiecePlacement.push_back(New);
								if (IsBlackPiece(New))
									hitPiece = true;
							}
							else
								hitPiece = true;
						}
						// NW
						hitPiece = false;
						New = cursorPosition;
						while (!hitPiece)
						{
							New = { New.x - 1, New.y - 1 };
							if (!hitPiece && !IsWhitePiece(New) && InGameBounds(New))
							{
								PiecePlacement.push_back(New);
								if (IsBlackPiece(New))
									hitPiece = true;
							}
							else
								hitPiece = true;
						}

						break;
					}
					// KING
					if (board[x + y * nBoardWidth] == 'K')
					{
						// N
						New = cursorPosition;
						New = { New.x, New.y - 1 };
						if (!IsWhitePiece(New) && InGameBounds(New))
							PiecePlacement.push_back(New);
						// S
						New = cursorPosition;
						New = { New.x, New.y + 1 };
						if (!IsWhitePiece(New) && InGameBounds(New))
							PiecePlacement.push_back(New);
						// E
						New = cursorPosition;
						New = { New.x + 1, New.y };
						if (!IsWhitePiece(New) && InGameBounds(New))
							PiecePlacement.push_back(New);
						// W
						New = cursorPosition;
						New = { New.x - 1, New.y };
						if (!IsWhitePiece(New) && InGameBounds(New))
							PiecePlacement.push_back(New);

						// NE
						New = cursorPosition;
						New = { New.x + 1, New.y - 1 };
						if (!IsWhitePiece(New) && InGameBounds(New))
							PiecePlacement.push_back(New);
						// SE
						New = cursorPosition;
						New = { New.x + 1, New.y + 1 };
						if (!IsWhitePiece(New) && InGameBounds(New))
							PiecePlacement.push_back(New);
						// SW
						New = cursorPosition;
						New = { New.x - 1, New.y + 1 };
						if (!IsWhitePiece(New) && InGameBounds(New))
							PiecePlacement.push_back(New);
						// NW
						New = cursorPosition;
						New = { New.x - 1, New.y - 1 };
						if (!IsWhitePiece(New) && InGameBounds(New))
							PiecePlacement.push_back(New);

						break;
					}
				}

				if (nTurn % 2 == 0)
				{
					int x = cursorPosition.x, y = cursorPosition.y;
					Position New;
					// PAWN
					if (board[x + y * nBoardWidth] == 'p')
					{
						// Top Row : TODO - Choose piece to switch
						if (y == 7)
							break;

						// 1 row
						New = { x, y + 1 };
						if (board[New.x + New.y * nBoardWidth] == ' ')
							PiecePlacement.push_back(New);

						// 2 rows
						New = { x, y + 2 };
						if (y == 1 && board[x + (y + 1) * nBoardWidth] == ' ')
							PiecePlacement.push_back(New);

						// Diagonal
						New = { x + 1 , y + 1 };
						if (IsWhitePiece(New))
							PiecePlacement.push_back(New);
						New = { x - 1 , y + 1 };
						if (IsWhitePiece(New))
							PiecePlacement.push_back(New);

						break;
					}
					// KNIGHT
					if (board[x + y * nBoardWidth] == 'n')
					{
						for (int X = -2; X < 3; X++)
						{
							for (int Y = -2; Y < 3; Y++)
							{
								if (abs(X) + abs(Y) == 3)
								{
									New.x = x + X; New.y = y + Y;
									if (InGameBounds(New))
										if (board[New.x + New.y * nBoardWidth] == ' ' || IsWhitePiece(New))
											PiecePlacement.push_back(New);
								}
							}
						}
						break;
					}
					// BISHOP
					if (board[x + y * nBoardWidth] == 'b')
					{
						// NE
						bool hitPiece = false;
						New = cursorPosition;
						while (!hitPiece)
						{
							New = { New.x + 1, New.y - 1 };
							if (!hitPiece && !IsBlackPiece(New) && InGameBounds(New))
							{
								PiecePlacement.push_back(New);
								if (IsWhitePiece(New))
									hitPiece = true;
							}
							else
								hitPiece = true;
						}
						// SE
						hitPiece = false;
						New = cursorPosition;
						while (!hitPiece)
						{
							New = { New.x + 1, New.y + 1 };
							if (!hitPiece && !IsBlackPiece(New) && InGameBounds(New))
							{
								PiecePlacement.push_back(New);
								if (IsWhitePiece(New))
									hitPiece = true;
							}
							else
								hitPiece = true;
						}
						// SW
						hitPiece = false;
						New = cursorPosition;
						while (!hitPiece)
						{
							New = { New.x - 1, New.y + 1 };
							if (!hitPiece && !IsBlackPiece(New) && InGameBounds(New))
							{
								PiecePlacement.push_back(New);
								if (IsWhitePiece(New))
									hitPiece = true;
							}
							else
								hitPiece = true;
						}
						// NW
						hitPiece = false;
						New = cursorPosition;
						while (!hitPiece)
						{
							New = { New.x - 1, New.y - 1 };
							if (!hitPiece && !IsBlackPiece(New) && InGameBounds(New))
							{
								PiecePlacement.push_back(New);
								if (IsWhitePiece(New))
									hitPiece = true;
							}
							else
								hitPiece = true;
						}
						break;
					}
					// ROOK
					if (board[x + y * nBoardWidth] == 'r')
					{
						// N
						bool hitPiece = false;
						New = cursorPosition;
						while (!hitPiece)
						{
							New = { New.x, New.y - 1 };
							if (!hitPiece && !IsBlackPiece(New) && InGameBounds(New))
							{
								PiecePlacement.push_back(New);
								if (IsWhitePiece(New))
									hitPiece = true;
							}
							else
								hitPiece = true;
						}
						// S
						hitPiece = false;
						New = cursorPosition;
						while (!hitPiece)
						{
							New = { New.x, New.y + 1 };
							if (!hitPiece && !IsBlackPiece(New) && InGameBounds(New))
							{
								PiecePlacement.push_back(New);
								if (IsWhitePiece(New))
									hitPiece = true;
							}
							else
								hitPiece = true;
						}
						// E
						hitPiece = false;
						New = cursorPosition;
						while (!hitPiece)
						{
							New = { New.x + 1, New.y };
							if (!hitPiece && !IsBlackPiece(New) && InGameBounds(New))
							{
								PiecePlacement.push_back(New);
								if (IsWhitePiece(New))
									hitPiece = true;
							}
							else
								hitPiece = true;
						}
						// W
						hitPiece = false;
						New = cursorPosition;
						while (!hitPiece)
						{
							New = { New.x - 1, New.y };
							if (!hitPiece && !IsBlackPiece(New) && InGameBounds(New))
							{
								PiecePlacement.push_back(New);
								if (IsWhitePiece(New))
									hitPiece = true;
							}
							else
								hitPiece = true;
						}
						break;
						// Todo: Castling
					}
					// QUEEN
					if (board[x + y * nBoardWidth] == 'q')
					{
						// N
						bool hitPiece = false;
						New = cursorPosition;
						while (!hitPiece)
						{
							New = { New.x, New.y - 1 };
							if (!hitPiece && !IsBlackPiece(New) && InGameBounds(New))
							{
								PiecePlacement.push_back(New);
								if (IsWhitePiece(New))
									hitPiece = true;
							}
							else
								hitPiece = true;
						}
						// S
						hitPiece = false;
						New = cursorPosition;
						while (!hitPiece)
						{
							New = { New.x, New.y + 1 };
							if (!hitPiece && !IsBlackPiece(New) && InGameBounds(New))
							{
								PiecePlacement.push_back(New);
								if (IsWhitePiece(New))
									hitPiece = true;
							}
							else
								hitPiece = true;
						}
						// E
						hitPiece = false;
						New = cursorPosition;
						while (!hitPiece)
						{
							New = { New.x + 1, New.y };
							if (!hitPiece && !IsBlackPiece(New) && InGameBounds(New))
							{
								PiecePlacement.push_back(New);
								if (IsWhitePiece(New))
									hitPiece = true;
							}
							else
								hitPiece = true;
						}
						// W
						hitPiece = false;
						New = cursorPosition;
						while (!hitPiece)
						{
							New = { New.x - 1, New.y };
							if (!hitPiece && !IsBlackPiece(New) && InGameBounds(New))
							{
								PiecePlacement.push_back(New);
								if (IsWhitePiece(New))
									hitPiece = true;
							}
							else
								hitPiece = true;
						}

						// NE
						hitPiece = false;
						New = cursorPosition;
						while (!hitPiece)
						{
							New = { New.x + 1, New.y - 1 };
							if (!hitPiece && !IsBlackPiece(New) && InGameBounds(New))
							{
								PiecePlacement.push_back(New);
								if (IsWhitePiece(New))
									hitPiece = true;
							}
							else
								hitPiece = true;
						}
						// SE
						hitPiece = false;
						New = cursorPosition;
						while (!hitPiece)
						{
							New = { New.x + 1, New.y + 1 };
							if (!hitPiece && !IsBlackPiece(New) && InGameBounds(New))
							{
								PiecePlacement.push_back(New);
								if (IsWhitePiece(New))
									hitPiece = true;
							}
							else
								hitPiece = true;
						}
						// SW
						hitPiece = false;
						New = cursorPosition;
						while (!hitPiece)
						{
							New = { New.x - 1, New.y + 1 };
							if (!hitPiece && !IsBlackPiece(New) && InGameBounds(New))
							{
								PiecePlacement.push_back(New);
								if (IsWhitePiece(New))
									hitPiece = true;
							}
							else
								hitPiece = true;
						}
						// NW
						hitPiece = false;
						New = cursorPosition;
						while (!hitPiece)
						{
							New = { New.x - 1, New.y - 1 };
							if (!hitPiece && !IsBlackPiece(New) && InGameBounds(New))
							{
								PiecePlacement.push_back(New);
								if (IsWhitePiece(New))
									hitPiece = true;
							}
							else
								hitPiece = true;
						}

						break;
					}
					// KING
					if (board[x + y * nBoardWidth] == 'k')
					{
						// N
						New = cursorPosition;
						New = { New.x, New.y - 1 };
						if (!IsBlackPiece(New) && InGameBounds(New))
							PiecePlacement.push_back(New);
						// S
						New = cursorPosition;
						New = { New.x, New.y + 1 };
						if (!IsBlackPiece(New) && InGameBounds(New))
							PiecePlacement.push_back(New);
						// E
						New = cursorPosition;
						New = { New.x + 1, New.y };
						if (!IsBlackPiece(New) && InGameBounds(New))
							PiecePlacement.push_back(New);
						// W
						New = cursorPosition;
						New = { New.x - 1, New.y };
						if (!IsBlackPiece(New) && InGameBounds(New))
							PiecePlacement.push_back(New);

						// NE
						New = cursorPosition;
						New = { New.x + 1, New.y - 1 };
						if (!IsBlackPiece(New) && InGameBounds(New))
							PiecePlacement.push_back(New);
						// SE
						New = cursorPosition;
						New = { New.x + 1, New.y + 1 };
						if (!IsBlackPiece(New) && InGameBounds(New))
							PiecePlacement.push_back(New);
						// SW
						New = cursorPosition;
						New = { New.x - 1, New.y + 1 };
						if (!IsBlackPiece(New) && InGameBounds(New))
							PiecePlacement.push_back(New);
						// NW
						New = cursorPosition;
						New = { New.x - 1, New.y - 1 };
						if (!IsBlackPiece(New) && InGameBounds(New))
							PiecePlacement.push_back(New);

						break;
					}
				}

			case(PLACE_PIECE):
				if (spaceBarReleased)
				{
					int x = cursorPosition.x, y = cursorPosition.y;
					Position New = { x, y };
					bool isPiecePlaced = false;
					for (auto& place : PiecePlacement)
					{
						if (place.x == x && place.y == y)
						{
							if (IsBlackPiece(place))
								CapturedP1.push_back(board[x + y * nBoardWidth]);
							if (IsWhitePiece(place))
								CapturedP2.push_back(board[x + y * nBoardWidth]);

							board[x + y * nBoardWidth] = board[selectedPiecePosition.x + selectedPiecePosition.y * nBoardWidth];
							board[selectedPiecePosition.x + selectedPiecePosition.y * nBoardWidth] = ' ';
							PiecePlacement.clear();
							nGamePhase = CHOOSE_PIECE;
							nTurn++;
							cursorPosition = { 0, (nTurn % 2 == 0) ? 0 : 7 };
							isPiecePlaced = true;
							break;
						}
					}
					if (!isPiecePlaced)
					{
						PiecePlacement.clear();
						nGamePhase = CHOOSE_PIECE;
					}
				}
			}

			// Clear Screen
			ClearScreen(screen);

			// Draw Borders
			for (int y = 0; y < nScreenHeight; y++)
			{
				for (int x = 0; x < nScreenWidth; x++)
				{
					if (y == 0 || y == 3 || y == nScreenHeight - 1)
						screen[x + y * nScreenWidth] = L'.';
					if ((x == 0 || x == nScreenWidth - 1) && y > 0)
						screen[x + y * nScreenWidth] = L':';
				}
			}

			// Write name of the game and turn
			const wstring sNameOfGame = L"ASCII Chess";
			wstring sTurn = L"Turn: " + to_wstring(nTurn);
			for (int i = 0; i < sNameOfGame.length(); i++)
				screen[nScreenWidth + i + (nScreenWidth - sNameOfGame.length()) / 2] = sNameOfGame[i];
			for (int i = 0; i < sTurn.length(); i++)
				screen[2 * nScreenWidth + i + (nScreenWidth - sTurn.length()) / 2] = sTurn[i];


			// Write Players names and mark player's turn
			for (int i = 0; i < sPlayer1.length(); i++)
				screen[nScreenWidth + i + 3] = sPlayer1[i];

			for (int i = 0; i < sPlayer2.length(); i++)
				screen[2 * nScreenWidth - sPlayer2.length() - 3 + i] = sPlayer2[i];

			if (nTurn % 2 == 1)
			{
				screen[nScreenWidth + 2] = L'*';
				screen[nScreenWidth + sPlayer1.length() + 3] = L'*';
			}

			if (nTurn % 2 == 0)
			{
				screen[2 * nScreenWidth - 3] = L'*';
				screen[2 * nScreenWidth - sPlayer2.length() - 4] = L'*';
			}

			// Draw board grid
			for (int y = 6; y < 23; y++)
			{
				for (int x = 31; x < nScreenWidth - 32; x++)
				{
					if (x % 7 == 3)
						screen[x + y * nScreenWidth] = L'|';
					if (y % 2 == 0)
						screen[x + y * nScreenWidth] = L'\u2014';
				}
			}

			// Draw pieces in the board
			for (int y = 0; y < nBoardHeight; y++)
			{
				for (int x = 0; x < nBoardWidth; x++)
				{
					screen[(35 + 7 * x) + (7 + 2 * y) * nScreenWidth] = board[x + y * nBoardWidth];
				}
			}

			// Draw piece placement
			for (auto& pos : PiecePlacement)
				screen[(34 + 7 * pos.x) + (7 + 2 * pos.y) * nScreenWidth] = L'#';

			// Draw cursor
			screen[(34 + 7 * cursorPosition.x) + (7 + 2 * cursorPosition.y) * nScreenWidth] = L'*';

			// Draw captured pieces
			int aux = 0;
			for (auto& piece : CapturedP1)
			{
				screen[2 * nScreenWidth + aux + 3] = piece;
				if (piece == 'k')
					GameOver(sPlayer1, screen);
				aux++;
			}
			aux = 0;
			for (auto& piece : CapturedP2)
			{
				screen[3 * nScreenWidth - CapturedP2.size() - 3 + aux] = piece;
				if (piece == 'k')
					GameOver(sPlayer2, screen);
				aux++;
			}

			// Display Frame
			vRefreshDisplay(screen);

			while (!UpdateKeyPress());
		}
	}

	return 0;
}