#include <iostream>
#include <vector>
#include <Windows.h>
#include <stdio.h>

#include "..\olcConsoleGameEngine.h"
using namespace std;

enum chessPieces {
	wPawn = 10,
	wRook = 11,
	wKnight = 12,
	wBishop = 13,
	wQueen = 14,
	wKing = 15,

	bPawn = 20,
	bRook = 21,
	bKnight = 22,
	bBishop = 23,
	bQueen = 24,
	bKing = 25
};
enum boardColumn {
	a = 1, b, c, d, e, f, g, h
};
int nGameBoard[8 * 8];
int nTurn;
bool bGame;
bool prevLeftArrowState;
bool prevRightArrowState;
bool prevUpArrowState;
bool prevDownArrowState;
int tempPiece;
int cursorPosition;

void PlacePiece(int piece, int Column, int Row)
{
	nGameBoard[(Row - 1) * 8 + Column - 1] = piece;
};

void StartGame()
{
	// Initializations
	prevLeftArrowState = false;
	prevRightArrowState = false;
	prevUpArrowState = false;
	prevDownArrowState = false;
	bGame = true;
	tempPiece = nGameBoard[0];
	cursorPosition = 0;

	//Clear Board
	for (int cell = 0; cell < 8 * 8; cell++)
		nGameBoard[cell] = 0;

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
}

void DrawBoard()
{
	for (int column = 0; column < 8; column++)
	{
		for (int row = 0; row < 8; row++)
		{
			cout << nGameBoard[(column) * 8 + row] << ((nGameBoard[(column) * 8 + row] == 0) ? "  " : " ");
		}
		cout << endl;
	}
}

int UpdateCursorPosition(int cursorPosition)
{
		// Check if the key is currently pressed
		bool currLeftArrowState = (GetAsyncKeyState(VK_LEFT) & 0x8000);
		bool currRightArrowState = (GetAsyncKeyState(VK_RIGHT) & 0x8000);
		bool currUpArrowState = (GetAsyncKeyState(VK_UP) & 0x8000);
		bool currDownArrowState = (GetAsyncKeyState(VK_DOWN) & 0x8000);

		// Check if the key was released
		bool leftArrowReleased = (prevLeftArrowState && !currLeftArrowState);
		bool rightArrowReleased = (prevRightArrowState && !currRightArrowState);
		bool upArrowReleased = (prevUpArrowState && !currUpArrowState);
		bool downArrowReleased = (prevDownArrowState && !currDownArrowState);

		// Update the previous state of key
		prevLeftArrowState = currLeftArrowState;
		prevRightArrowState = currRightArrowState;
		prevUpArrowState = currUpArrowState;
		prevDownArrowState = currDownArrowState;

		if (rightArrowReleased)
		{
			nGameBoard[cursorPosition] = tempPiece;
			if (cursorPosition >= 8 * 8 - 1)
				cursorPosition = -1;
			cursorPosition++;
			tempPiece = nGameBoard[cursorPosition];
		}

		if (leftArrowReleased)
		{
			nGameBoard[cursorPosition] = tempPiece;
			if (cursorPosition <= 0)
				cursorPosition = 8*8;
			cursorPosition--;
		}

		if (downArrowReleased)
		{
			nGameBoard[cursorPosition] = tempPiece;
			if (cursorPosition >= 8 * 8 - 1)
				cursorPosition = -8;
			if (cursorPosition / 8 == 7 && cursorPosition > 0)
				cursorPosition = (cursorPosition % 8) - 7;
			cursorPosition += 8;
		}

		if (upArrowReleased)
		{
			nGameBoard[cursorPosition] = tempPiece;
			if (cursorPosition <= 0)
				cursorPosition = 8 * 9 - 1;
			if (cursorPosition / 8 == 0 && cursorPosition < 8 * 8)
				cursorPosition = 8 * 8 + cursorPosition - 1;
			cursorPosition -= 8;
		}

		if (nGameBoard[cursorPosition] != 99)
			tempPiece = nGameBoard[cursorPosition];

		return cursorPosition;
}

int main()
{
	StartGame();

	while(bGame)
	{
		cursorPosition = UpdateCursorPosition(cursorPosition);

		nGameBoard[cursorPosition] = 99;

		std::system("CLS");
		DrawBoard();
		//Sleep(1);
	}

	std::system("PAUSE");

	return 0;
}