#include <iostream>
#include <Windows.h>
#include "Board.h"
#include "Pieces.h"
#include "Position.h"

// Set console
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void CreateConsole(int width, int height, int fontSize)
{
    SMALL_RECT windowRect = { 0, 0, width - 1, height - 1 };
    SetConsoleWindowInfo(hConsole, TRUE, &windowRect);
    CONSOLE_FONT_INFOEX fontInfo = { 0 };
    fontInfo.cbSize = sizeof(fontInfo);
    fontInfo.dwFontSize.Y = fontSize;
    SetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);
}

// Set the console font size


int main()
{
    // Create console
    CreateConsole(30, 12, 32);

    // Initialize board and pieces
    Board* board = new Board;

    board->printBoard(hConsole);
    int f;
    std::cin >> f;
    return 0;
}