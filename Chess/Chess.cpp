#include <iostream>
#include <vector>
#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <string>

//#include "..\olcConsoleGameEngine.h"
using namespace std;

enum chessPieces {
	blank = 0, frame,
	wPawn = 11,	wRook, wKnight, wBishop, wQueen, wKing,
	bPawn = 21, bRook, bKnight, bBishop, bQueen,bKing
};
enum boardColumn {
	a = 1, b, c, d, e, f, g, h
};

const int nScreenWidth = 120, nScreenHeight = 30;
const int nBoardWidth = 8, nBoardHeight = 8;

wchar_t *board = new wchar_t[nBoardWidth * nBoardHeight];
wchar_t tempPiece;

int nTurn = 1;
bool bGame, prevLeftArrowState, prevRightArrowState, prevUpArrowState, prevDownArrowState, prevSpaceBarState;

string sPlayer1;
string sPlayer2;

struct Position {
	int x = 0;
	int y = 0;
};

Position cursorPosition = { 0, 7 };

void vRefreshDisplay(wchar_t* screen)
{
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0, 0 }, &dwBytesWritten);
}

void ClearBoard()
{
	for (int x = 0; x < nBoardWidth; x++)
	{
		for (int y = 0; y < nBoardHeight; y++)
			board[x + y * nBoardWidth] = L' ';
	}
}

void PlacePiece(int nPiece, int x, int y)
{
	wchar_t cPiece = ' ';

	switch (nPiece % 10){
	case(1):
		cPiece = 'P';
		break;
	case(2):
		cPiece = 'R';
		break;
	case(3):
		cPiece = 'N';
		break;
	case(4):
		cPiece = 'B';
		break;
	case(5):
		cPiece = 'Q';
		break;
	case(6):
		cPiece = 'K';
		break;
	}
	board[x - 1 + (y - 1) * nBoardWidth] = cPiece;
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

void UpdateKeyPress()
{
		// Check if the key is currently pressed
		bool currLeftArrowState = (GetAsyncKeyState(VK_LEFT) & 0x8000);
		bool currRightArrowState = (GetAsyncKeyState(VK_RIGHT) & 0x8000);
		bool currUpArrowState = (GetAsyncKeyState(VK_UP) & 0x8000);
		bool currDownArrowState = (GetAsyncKeyState(VK_DOWN) & 0x8000);
		bool currSpaceBarState = (GetAsyncKeyState(VK_SPACE) & 0x8000);

		// Check if the key was released
		bool leftArrowReleased = (prevLeftArrowState && !currLeftArrowState);
		bool rightArrowReleased = (prevRightArrowState && !currRightArrowState);
		bool upArrowReleased = (prevUpArrowState && !currUpArrowState);
		bool downArrowReleased = (prevDownArrowState && !currDownArrowState);
		bool spaceBarReleased = (prevSpaceBarState && !currSpaceBarState);

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
		}

		if (spaceBarReleased)
		{
			nTurn++;
			if (nTurn % 2 == 1)
				cursorPosition = { 0, 7 };
			if (nTurn % 2 == 0)
				cursorPosition = { 0, 0 };
		}
}

int main()
{
	// Use current time as seed for random generator
	srand(time(0));

	wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
	screen[nScreenWidth * nScreenHeight - 1] = '\0';

	// StartGame
	// Initializations
	prevLeftArrowState = false;
	prevRightArrowState = false;
	prevUpArrowState = false;
	prevDownArrowState = false;
	bGame = true;


	//Clear Board
	ClearBoard();

	//Place pawns
	for (int i = 1; i <= 8; i++)
	{
		PlacePiece(wPawn, i, 1);
		PlacePiece(bPawn, i, 8);
	}

	//Place Black Pieces
	PlacePiece(bRook, a, 7);
	PlacePiece(bKnight, b, 7);
	PlacePiece(bBishop, c, 7);
	PlacePiece(bQueen, d, 7);
	PlacePiece(bKing, e, 7);
	PlacePiece(bBishop, f, 7);
	PlacePiece(bKnight, g, 7);
	PlacePiece(bRook, h, 7);

	//Place White Pieces
	PlacePiece(wRook, a, 2);
	PlacePiece(wKnight, b, 2);
	PlacePiece(wBishop, c, 2);
	PlacePiece(wQueen, d, 2);
	PlacePiece(wKing, e, 2);
	PlacePiece(wBishop, f, 2);
	PlacePiece(wKnight, g, 2);
	PlacePiece(wRook, h, 2);

	tempPiece = board[0];

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

	bGame = true;

	while(bGame)
	{
		UpdateKeyPress();

		// Clear Screen
		for (int i = 0; i < nScreenWidth * nScreenHeight; i++)
			screen[i] = L' ';

		// Draw Borders
		for (int y = 0; y < nScreenHeight; y++)
		{
			for (int x = 0; x < nScreenWidth; x++)
			{
				if(y == 0 || y == 3 || y == nScreenHeight - 1)
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

		// Write pieces captured by players

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
				screen[(35 + 7 * x) + (7 + 2 * y) * nScreenWidth] = board[x + y * nBoardWidth];
		}

		// Draw cursor
		screen[(34 + 7 * cursorPosition.x) + (7 + 2 * cursorPosition.y) * nScreenWidth] = L'*';

		// Display Frame
		vRefreshDisplay(screen);
	}

	std::system("PAUSE");

	return 0;
}