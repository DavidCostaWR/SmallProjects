//#include "Pieces.h"
//#include "Board.h"
#include "PGE_Chess.h"

int main()
{
	// Use olcConsoleGameEngine derived app
	PGE_Chess game;
	if (game.Construct(1920/2, 1080/2, 1, 1, false))
		game.Start();
	return 0;
}