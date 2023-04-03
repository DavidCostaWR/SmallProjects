#pragma once
#include <iostream>
#include <vector>
#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <string>
#include <list>

enum PieceType { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING };
struct Position { int x = 0; int y = 0; };
bool operator== (const Position& pos1, const Position& pos2) { return (pos1.x == pos2.x && pos1.y == pos2.y); }

// enums
enum boardColumn { a = 1, b, c, d, e, f, g, h };
enum GamePhase { CHOOSE_PIECE, CHOOSE_DESTINATION };

// constants
const int nScreenWidth = 120, nScreenHeight = 30;
const int nBoardWidth = 8, nBoardHeight = 8;
const int nWPawnStartRow = 6, nBPawnStartRow = 1;

wchar_t* board = new wchar_t[nBoardWidth * nBoardHeight];
wchar_t tempPiece;

int nTurn, nGamePhase = CHOOSE_PIECE;
bool bGame, prevLeftArrowState, prevRightArrowState, prevUpArrowState, prevDownArrowState, prevSpaceBarState;

bool leftArrowReleased = false;
bool rightArrowReleased = false;
bool upArrowReleased = false;
bool downArrowReleased = false;
bool spaceBarReleased = false;

list<Position> PiecePlacement;
list<wchar_t> CapturedP1;
list<wchar_t> CapturedP2;
vector<cPiece*> pieces;

Position cursorPosition = { 0, 7 };
Position selectedPiecePosition;