#include <iostream>
#include <vector>
#include <Windows.h>
#include <stdio.h>
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
bool bGame = true;

void PlacePiece(int piece, int Column, int Row)
{
	nGameBoard[(Column - 1) * 8 + Row - 1] = piece;
};

void StartGame()
{
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
	for (int row = 0; row < 8; row++)
	{
		for (int column = 0; column < 8; column++)
		{
			cout << nGameBoard[(column) * 8 + row] << ((nGameBoard[(column) * 8 + row] == 0) ? "  " : " ");
		}
		cout << endl;
	}
}

int main()
{
	StartGame();

	while(bGame)
	{
		std::system("CLS");
		DrawBoard();
		Sleep(1);
	}

							
	std::system("PAUSE");

	return 0;
}