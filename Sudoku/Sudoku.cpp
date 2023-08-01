#define OLC_PGE_APPLICATION
#include "../olcPixelGameEngine.h"
#include <vector>
#include <algorithm>
#include <random>
#include <fstream>
#include <sstream>
#include <iostream>

/*
			px	#	T
Cell		20	9	180
Frame		3	2	6
line		1	6	6
3x3 grid	2	2	4
Total H&W			196

Number = 8x8

*/

// Enums
enum class GameState
{
	NONE,
	MAIN_MENU,
	PUZZLE_CREATION,
	USER_SOLVING,
	VICTORY,
	DISPLAY_STATS
};
enum class Difficulty {
	NONE,
	EASY,
	MEDIUM,
	HARD
};

// Structs
struct BestTimes
{
	float easy = std::numeric_limits<float>::max();
	float medium = std::numeric_limits<float>::max();
	float hard = std::numeric_limits<float>::max();
};

class Sudoku : public olc::PixelGameEngine
{
private:
	// Dimensions
	const UINT16 nCellDim = 40;
	const UINT16 nFrameDim = 6;
	const UINT16 nlineDim = 1;
	const UINT16 n3x3GridDim = 3;
	const int nBoardDim = 9 * nCellDim + 6 * nlineDim + 2 * n3x3GridDim;

	const UINT16 nNumberScale = 3;
	
	const UINT16 nHeaderHeight = 40;
	const UINT16 nHeaderWidth = nBoardDim;
	const UINT16 nHeaderGridSpace = 5;

	// Center board and header
	const UINT16 nGamespaceWidth = nBoardDim + 2 * nFrameDim;
	const UINT16 nGamespaceHeight = nBoardDim + nHeaderHeight + 4 * nFrameDim + nHeaderGridSpace;

	int nHeaderPx;
	int nHeaderPy;

	int nGridPx;
	int nGridPy;

	// Flags
	bool quit = false;
	bool bHighlightNumberSelected = true;

	BestTimes bestTimes;

	Difficulty difficulty = Difficulty::NONE;

	int grid[9][9]{};
	int solutionGrid[9][9]{};
	int solverGrid[9][9]{};
	std::vector<int> candidates[9][9];
	UINT8 cursorX = 0;
	UINT8 cursorY = 0;
	float solveTimer = 0.0f;
	float solveDelayTimer = 0.0f;
	float fBackgroundTimer = 0.0f;
	GameState gameState = GameState::MAIN_MENU;
	std::mt19937 rng{ std::random_device{}() };

public:
	Sudoku() { sAppName = "Sudoku"; }

public:
	bool OnUserCreate() override
	{
		CenterGame();
		LoadBestTimes(); // Load best times from file
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		switch (gameState)
		{
		case GameState::MAIN_MENU:
			// reset timers
			solveTimer = 0;
			solveDelayTimer;
			// Draw routines
			DrawBackground(fElapsedTime, olc::DARK_BLUE, olc::Pixel(0, 0, 200));
			DrawMainMenu();
			break;
		case GameState::PUZZLE_CREATION:
			// Create puzzle
			GenerateSudoku();
			ClearCells((int)difficulty * 10);
			gameState = GameState::USER_SOLVING;
			break;
		case GameState::USER_SOLVING:
			// update timer
			if(IsFocused() || InBoard(GetMouseX(),GetMouseY()))
				solveTimer += fElapsedTime;
			// Handle user input
			HandleInput();
			// Draw routunes
			DrawBackground(fElapsedTime, olc::DARK_BLUE, olc::Pixel(0, 0, 200));
			DrawGrid();
			DrawHeader();
			DrawCursor();
			DrawNumbers();
			if (bHighlightNumberSelected) HighlightNumbers();
			// Check for victory
			if (CheckVictory())
			{
				UpdateBestTime();
				SaveBestTimes();
				gameState = GameState::VICTORY;
			}
			break;
		case GameState::VICTORY:
			DrawVictory(fElapsedTime);
			break;
		case GameState::DISPLAY_STATS:
			// Draw routines
			DrawBackground(fElapsedTime, olc::RED, olc::DARK_CYAN);
			DrawStats();
			break;
		}

		return !quit;
	}

private:
	void GenerateSudoku()
	{
		ClearGrid();
		if (!SolveSudoku(0, 0))
			std::cout << "Could not generate a Sudoku puzzle!" << std::endl;
	}

	void DrawMainMenu()
	{
		int nTextSize = 1;
		int nTitleSize = 2;

		int nLineBrk1 = nTextSize * 8 * 2;
		int nLineBrk2 = nTitleSize * 8 * 2;

		int px = 4 * nTextSize;
		int py = nLineBrk2;

		DrawString(px, py, "Sudoku Generator", olc::WHITE, nTitleSize);
		py += nLineBrk2 + nLineBrk1;
		DrawString(px, py, "Select Difficulty:", olc::WHITE, nTextSize);
		DrawString(px + nBoardDim/2, py, "Best Times:", olc::WHITE, nTextSize);
		py += nLineBrk1;
		DrawString(px, py, "1. Easy", olc::WHITE, nTextSize);
		DrawString(px + nBoardDim/2, py, Time2String(bestTimes.easy), olc::WHITE, nTextSize);
		py += nLineBrk1;
		DrawString(px, py, "2. Medium", olc::WHITE, nTextSize);
		DrawString(px + nBoardDim/2, py, Time2String(bestTimes.medium), olc::WHITE, nTextSize);
		py += nLineBrk1;
		DrawString(px, py, "3. Hard", olc::WHITE, nTextSize);
		DrawString(px + nBoardDim/2, py, Time2String(bestTimes.hard), olc::WHITE, nTextSize);
		py += nLineBrk2;
		DrawString(px, py, "Press E to Erase Best Times", olc::WHITE, nTextSize);


		if (GetKey(olc::Key::K1).bPressed || GetKey(olc::Key::NP1).bPressed)
		{
			difficulty = Difficulty::EASY;
			gameState = GameState::PUZZLE_CREATION;
		}
		if (GetKey(olc::Key::K2).bPressed || GetKey(olc::Key::NP2).bPressed)
		{
			difficulty = Difficulty::MEDIUM;
			gameState = GameState::PUZZLE_CREATION;
		}
		if (GetKey(olc::Key::K3).bPressed || GetKey(olc::Key::NP3).bPressed)
		{
			difficulty = Difficulty::HARD;
			gameState = GameState::PUZZLE_CREATION;
		}
		if (GetKey(olc::Key::E).bPressed)
		{
			EraseBestTimes();
			SaveBestTimes();
		}
	}

	void ClearGrid()
	{
		std::memset(grid, 0, sizeof(grid));
		std::memset(solverGrid, 0, sizeof(solverGrid));
		std::memset(solutionGrid, 0, sizeof(solutionGrid));
	}

	void ClearCells(int cellsToClear)
	{
		std::memcpy(solutionGrid, grid, sizeof(grid));

		while (cellsToClear > 0)
		{
			int row = rng() % 9;
			int col = rng() % 9;

			if (grid[row][col] != 0)
			{
				int temp = grid[row][col];
				grid[row][col] = 0;

				int solutionCount = 0;
				int backupGrid[9][9];
				std::memcpy(backupGrid, grid, sizeof(grid));

				if (SolveSudoku(0, 0))
				{
					solutionCount++;
					std::memcpy(grid, backupGrid, sizeof(grid));
				}

				if (solutionCount != 1)
					grid[row][col] = temp;
				else
					cellsToClear--;
			}
		}
	}

	bool SolveSudoku(int row, int col)
	{
		if (row == 9)
			return true;

		int nextRow = (col == 8) ? row + 1 : row;
		int nextCol = (col + 1) % 9;

		if (grid[row][col] != 0)
			return SolveSudoku(nextRow, nextCol);

		std::vector<int> nums{ 1,2,3,4,5,6,7,8,9 };
		std::shuffle(nums.begin(), nums.end(), rng);

		for (int num : nums)
		{
			if (IsValidMove(row, col, num))
			{
				grid[row][col] = num;
				if (SolveSudoku(nextRow, nextCol))
					return true;
				grid[row][col] = 0;
			}
		}

		return false;
	}

	bool IsValidMove(int row, int col, int num)
	{
		for (int i = 0; i < 9; ++i)
		{
			if ( (i != row && (grid[i][col] == num || solverGrid[i][col] == num)) ||
				(i != col && (grid[row][i] == num || solverGrid[row][i] == num)) )
				return false;
		}

		int startRow = row - row % 3;
		int startCol = col - col % 3;
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				if (grid[i + startRow][j + startCol] == num )//|| solverGrid[i + startRow][j + startCol] == num)
					return false;
			}
		}

		return true;
	}

	bool CheckVictory()
	{
		for (int i = 0; i < 9; ++i) {
			for (int j = 0; j < 9; ++j) {
				if (solverGrid[i][j] != solutionGrid[i][j] && grid[i][j] != solutionGrid[i][j])
					return false;
			}
		}
		return true;
	}

	void HandleInput()
	{
		int newCellValue = 0;

		// Return to main menu
		if (GetKey(olc::Key::ESCAPE).bPressed)	gameState = GameState::MAIN_MENU;

		// Move cursor
		if (Screen2Grid(GetMouseX(), GetMouseY()).x > 0 && Screen2Grid(GetMouseX(), GetMouseY()).y > 0)
		{
			cursorX = Screen2Grid(GetMouseX(), GetMouseY()).x - 1;
			cursorY = Screen2Grid(GetMouseX(), GetMouseY()).y - 1;
			// Click
			if (GetMouse(0).bPressed && grid[cursorY][cursorX] == 0)
			{
				solverGrid[cursorY][cursorX] = (solverGrid[cursorY][cursorX] + 1) % 10;
			}
			if (GetMouse(1).bPressed && grid[cursorY][cursorX] == 0)
			{
				solverGrid[cursorY][cursorX] = (solverGrid[cursorY][cursorX] + 9) % 10;
			}
		}
		else
		{
			if (GetKey(olc::Key::RIGHT).bPressed)	cursorX = (cursorX == 8 ? 0 : cursorX + 1);
			if (GetKey(olc::Key::LEFT).bPressed)	cursorX = (cursorX == 0 ? 8 : cursorX - 1);
			if (GetKey(olc::Key::DOWN).bPressed)	cursorY = (cursorY == 8 ? 0 : cursorY + 1);
			if (GetKey(olc::Key::UP).bPressed) 		cursorY = (cursorY == 0 ? 8 : cursorY - 1);
		}


		// Toggle highlited numbers
		if (GetKey(olc::Key::H).bPressed) bHighlightNumberSelected = !bHighlightNumberSelected;

		// Fill cell with number
		if (GetKey(olc::Key::K1).bPressed || GetKey(olc::Key::NP1).bPressed) newCellValue = 1;
		if (GetKey(olc::Key::K2).bPressed || GetKey(olc::Key::NP2).bPressed) newCellValue = 2;
		if (GetKey(olc::Key::K3).bPressed || GetKey(olc::Key::NP3).bPressed) newCellValue = 3;
		if (GetKey(olc::Key::K4).bPressed || GetKey(olc::Key::NP4).bPressed) newCellValue = 4;
		if (GetKey(olc::Key::K5).bPressed || GetKey(olc::Key::NP5).bPressed) newCellValue = 5;
		if (GetKey(olc::Key::K6).bPressed || GetKey(olc::Key::NP6).bPressed) newCellValue = 6;
		if (GetKey(olc::Key::K7).bPressed || GetKey(olc::Key::NP7).bPressed) newCellValue = 7;
		if (GetKey(olc::Key::K8).bPressed || GetKey(olc::Key::NP8).bPressed) newCellValue = 8;
		if (GetKey(olc::Key::K9).bPressed || GetKey(olc::Key::NP9).bPressed) newCellValue = 9;

		if (grid[cursorY][cursorX] == 0 && newCellValue != 0)
			solverGrid[cursorY][cursorX] = newCellValue;

		// Delete cell
		if (GetKey(olc::Key::BACK).bPressed || GetKey(olc::Key::DEL).bPressed)
		{
			if (grid[cursorY][cursorX] == 0)
				solverGrid[cursorY][cursorX] = 0;
		}
	}

	void DrawBackground(float fElapsedTime, olc::Pixel topColor, olc::Pixel bottomColor)
	{
		Clear(topColor);
		float fAmplitude = ScreenHeight()/7;
		float fFrequency = 1500.0f;
		
		fBackgroundTimer += fElapsedTime * 0.75f;


		for (int x = 0; x < ScreenWidth(); ++x)
		{
			float fSineWave = (fAmplitude * std::sin(2 * 3.141592 * fFrequency * x - fBackgroundTimer)) + ScreenHeight() * 4/7;
			DrawLine(x, 0, x, fSineWave, bottomColor);
		}
	}

	void DrawHeader()
	{
		FillRect(nHeaderPx - nFrameDim, nHeaderPy - nFrameDim, nHeaderWidth + 2 * nFrameDim, nHeaderHeight + 2 * nFrameDim, olc::BLACK);
		FillRect(nHeaderPx, nHeaderPy, nHeaderWidth, nHeaderHeight, olc::WHITE);

		const int margin = nFrameDim;
		const int nNumberHScale = 1;
		const int nNumberHSize = 8 * nNumberHScale;

		const int nLineBrk = nNumberHSize + margin;
		const int nFirstLineX = nHeaderPx + margin;
		const int nFirstLineY = nHeaderPy + margin;

		std::string sDifficulty;
		float nRecord;
		std::string sRecord;
		boolean isRecordBroken = false;

		switch (difficulty)
		{
		case Difficulty::EASY:
			sDifficulty = "Easy";
			nRecord = bestTimes.easy;
			break;
		case Difficulty::MEDIUM:
			sDifficulty = "Medium";
			nRecord = bestTimes.medium;
			break;
		case Difficulty::HARD:
			sDifficulty = "Hard";
			nRecord = bestTimes.hard;
			break;
		default:
			sDifficulty = "ERROR";
			break;
		}

		DrawString(nFirstLineX, nFirstLineY + 0 * nLineBrk, "Time: " + Time2String(solveTimer), olc::BLACK, nNumberHScale);
		DrawString(nFirstLineX, nFirstLineY + 1 * nLineBrk, "Record: " + Time2String(nRecord), olc::BLACK, nNumberHScale);

		DrawString(nFirstLineX + nHeaderWidth / 2, nFirstLineY + 0 * nLineBrk, "Difficulty: " + sDifficulty, olc::BLACK, nNumberHScale);
		//DrawString(nFirstLineX + nHeaderWidth / 2, nFirstLineY + 1 * nLineBrk, "Mouse: (" + std::to_string((int)Screen2Grid(GetMouseX(), GetMouseY()).x) + ", " + std::to_string((int)Screen2Grid(GetMouseX(), GetMouseY()).y) + ")", olc::BLACK, nNumberHScale);
	}

	void DrawCursor()
	{
		const olc::Pixel color = olc::Pixel(180,180,255,255);
		const float cursorMargin = n3x3GridDim;
		const float cursorSize = nCellDim - 2 * cursorMargin;
		FillRect(Grid2Screen(cursorX + 1, cursorY + 1, cursorMargin), olc::vi2d(cursorSize, cursorSize), color);
	}

	void DrawGrid()
	{

		// Draw white board
		FillRect(nGridPx, nGridPy, nBoardDim, nBoardDim, olc::WHITE);

		// Draw Frame
		for (int i = 0; i < nFrameDim; ++i)
		{
			DrawRect(nGridPx - nFrameDim + i, nGridPy - nFrameDim + i, nBoardDim + 2 * nFrameDim - 2 * i, nBoardDim + 2 * nFrameDim - 2 * i, olc::BLACK);
		}

		// Draw grid
		for (int i = 1; i <= 9; ++i)
		{
			int x = nGridPx + i * nCellDim + (i - 1) * nlineDim + (int)((i - 1) / 3) * (n3x3GridDim - nlineDim);
			int y = nGridPy + i * nCellDim + (i - 1) * nlineDim + (int)((i - 1) / 3) * (n3x3GridDim - nlineDim);

			for (int j = 0; j < nlineDim; ++j)
			{
				// Vertical
				DrawLine(x + j, nGridPy, x + j, nGridPy + nBoardDim, olc::GREY);
				// Horizontal
				DrawLine(nGridPx, y + j, nGridPx + nBoardDim, y + j, olc::GREY);
			}
		}

		// Draw 3x3 grid
		for (int i = 1; i < 3; ++i)
		{
			int x = nGridPx + i * 3 * nCellDim + i * 2 * nlineDim + (i - 1) * n3x3GridDim;
			int y = nGridPy + i * 3 * nCellDim + i * 2 * nlineDim + (i - 1) * n3x3GridDim;

			for (int j = 0; j < n3x3GridDim; ++j)
			{
				DrawLine(x + j, nGridPy, x + j, nGridPy + nBoardDim, olc::DARK_GREY);
				DrawLine(nGridPx, y - j, nGridPx + nBoardDim, y - j, olc::DARK_GREY);
			}
		}
	}
		
	void DrawNumbers()
	{
		const int nNumberSize = 8 * nNumberScale;
		const float margin = (nCellDim - nNumberSize) / 2;

		for (int row = 0; row < 9; ++row)
		{
			for (int col = 0; col < 9; ++col)
			{
				int num = grid[row][col];
				if (num != 0)
				{
					DrawString(Grid2Screen(col + 1, row + 1, margin), std::to_string(num), olc::BLACK, nNumberScale);
				}
				num = solverGrid[row][col];
				if (num != 0)
				{
					olc::Pixel numColor = olc::DARK_GREY;
					if (!IsValidMove(row, col, num)) numColor = olc::RED;
					DrawString(Grid2Screen(col + 1, row + 1, margin), std::to_string(num), numColor, nNumberScale);
				}
			}
		}
	}

	void HighlightNumbers()
	{
		int nSelectionGrid = grid[cursorY][cursorX];
		int nSelectionSolver = solverGrid[cursorY][cursorX];
		if (nSelectionGrid != 0 || nSelectionSolver != 0)
		{
			int nSelection = std::max(nSelectionGrid, nSelectionSolver);

			const int nNumberSize = 8 * nNumberScale;
			const float margin = (nCellDim - nNumberSize) / 2;

			for (int row = 0; row < 9; ++row)
			{
				for (int col = 0; col < 9; ++col)
				{
					if ((grid[row][col] == nSelection && (row != cursorY || col != cursorX)) || 
						(solverGrid[row][col] == nSelection && (row != cursorY || col != cursorX)))
					{
						DrawString(Grid2Screen(col + 1, row + 1, margin), std::to_string(nSelection), olc::BLUE, nNumberScale);
					}
				}
			}
		}
	}

	void DrawStats()
	{
		std::string sDifficulty = "";

		switch (difficulty)
		{
		case Difficulty::EASY:
			sDifficulty = "Easy";
			break;
		case Difficulty::MEDIUM:
			sDifficulty = "Medium";
			break;
		case Difficulty::HARD:
			sDifficulty = "Hard";
			break;
		}
		int nScale = 1;
		int nTab = 8 * 8 * nScale;
		
		int px = 10;
		int py = 30;

		DrawString(px, py, sDifficulty + " Puzzle Solved!", olc::WHITE, 2*nScale);
		py += 30;
		DrawString(px, py, "Solve Time: " + Time2String(solveTimer), olc::WHITE, nScale);
		py += 30;

		DrawString(px, py, "Best Times:", olc::WHITE, 2);
		py += 30;
		DrawString(px, py, "Easy: ", olc::WHITE, nScale);
		DrawString(px + nTab, py, Time2String(bestTimes.easy), olc::WHITE, nScale);
		py += 15;
		DrawString(px, py, "Medium: ", olc::WHITE, nScale);
		DrawString(px + nTab, py, Time2String(bestTimes.medium), olc::WHITE, nScale);
		py += 15;
		DrawString(px, py, "Hard: ", olc::WHITE, nScale);
		DrawString(px + nTab, py, Time2String(bestTimes.hard), olc::WHITE, nScale);
		py += 30;

		DrawString(px, py, "Press Enter for Main Menu", olc::WHITE, nScale);
		py += 15;
		DrawString(px, py, "Press Escape to Exit", olc::WHITE, nScale);

		if (GetKey(olc::Key::ENTER).bPressed) gameState = GameState::MAIN_MENU;
		if (GetKey(olc::Key::ESCAPE).bPressed) quit = true;
	}

	void DrawVictory(float fElapsedTime) {
		solveDelayTimer += fElapsedTime;
		float delay = 10.0f;
		DrawBackground(fElapsedTime, olc::DARK_BLUE, olc::Pixel(0, 0, 200));
		DrawGrid();
		DrawHeader();
		DrawCursor();
		DrawNumbers();
		std::string Solved = "PUZZLE SOLVED";

		switch(difficulty)
		{
		case Difficulty::EASY:
			if (solveTimer <= bestTimes.easy) Solved = "NEW BEST TIME";
			break;
		case Difficulty::MEDIUM:
			if (solveTimer <= bestTimes.medium) Solved = "NEW BEST TIME";
			break;
		case Difficulty::HARD:
			if (solveTimer <= bestTimes.hard) Solved = "NEW BEST TIME";
			break;
		}

		int scale = ScreenWidth() / Solved.length() / 9;
		int px = (ScreenWidth() - Solved.length() * scale * 8) / 2;
		int py = (ScreenHeight() - scale) / 2;

		if ((int)(solveDelayTimer * 2) % 3 != 0)
		{
			FillRect(px - nFrameDim, py - nFrameDim, Solved.length() * scale * 8 + 2 * nFrameDim, scale * 8 + 2 * nFrameDim, olc::GREY);
			DrawString(px, py, Solved, olc::RED, scale);
		}

		if (solveDelayTimer > delay || GetKey(olc::Key::SPACE).bPressed || GetKey(olc::Key::ENTER).bPressed || 
			GetKey(olc::Key::ESCAPE).bPressed || GetMouse(0).bPressed || GetMouse(1).bPressed)
		{
			solveDelayTimer = 0.0f;
			gameState = GameState::DISPLAY_STATS;
		}
	}

	void CenterGame()
	{
		// Center board and header
		nHeaderPx = (ScreenWidth() - nGamespaceWidth) / 2 + nFrameDim;
		nHeaderPy = (ScreenHeight() - nGamespaceHeight) / 2 + nFrameDim;

		nGridPx = (ScreenWidth() - nGamespaceWidth) / 2 + nFrameDim;
		nGridPy = (ScreenHeight() - nGamespaceHeight) / 2 + nHeaderHeight + 3 * nFrameDim + nHeaderGridSpace;
	}

	olc::vf2d Grid2Screen(int px, int py, float margin = 0) // returns top left position of square px, py, offset by margin
	{
		return { nGridPx + (px - 1) * (nlineDim + nCellDim) + (int)((px - 1) / 3) * (n3x3GridDim - nlineDim) + margin, nGridPy + (py - 1) * (nlineDim + nCellDim) + (int)((py - 1) / 3) * (n3x3GridDim - nlineDim) + margin };
	}

	olc::vd2d Screen2Grid(float sx, float sy)
	{
		int nGridX = (sx - nGridPx) / (nCellDim * 3 + nlineDim * 2 + n3x3GridDim);
		int nSubGridX = (sx - (Grid2Screen(nGridX * 3 + 1, 0).x + 1)) / (nCellDim + nlineDim);

		int nGridY = (sy - nGridPy) / (nCellDim * 3 + nlineDim * 3 + n3x3GridDim);
		int nSubGridY = (sy - (Grid2Screen(0,nGridY * 3 + 1).y + 1)) / (nCellDim + nlineDim);

		int px = nGridX * 3 + 1 + nSubGridX;
		int py = nGridY * 3 + 1 + nSubGridY;
		if (sx > nGridPx && sy > nGridPy && px < 10 && py < 10)
			return olc::vd2d(px,py);
		return { 0,0 };
	}

	std::string Time2String(float time)
	{
		float tempTime = time;

		int nMinutes = tempTime / 60;
		tempTime -= nMinutes * 60;
		int nSeconds = tempTime;
		tempTime -= nSeconds;
		int nMilisseconds = tempTime * 1000;

		std::string sMinutes = std::to_string(nMinutes);
		if (nMinutes < 10) sMinutes = "0" + sMinutes;

		std::string sSeconds = std::to_string(nSeconds);
		if (nSeconds < 10) sSeconds = "0" + sSeconds;

		std::string sMilisseconds = std::to_string(nMilisseconds);
		if (nMilisseconds < 100) sMilisseconds = "0" + sMilisseconds;
		if (nMilisseconds < 10) sMilisseconds = "0" + sMilisseconds;

		if (nMinutes < 0) return "--:--.---";

		return sMinutes + ":" + sSeconds + "." + sMilisseconds;
	}

	bool InBoard(float sx, float sy)
	{
		return sx >= nGridPx && sx <= nGridPx + nBoardDim && sy >= nGridPy && sy <= nGridPy + nBoardDim;
	}

	void UpdateBestTime()
	{
		switch (difficulty)
		{
		case Difficulty::EASY:
			if (solveTimer < bestTimes.easy) bestTimes.easy = solveTimer;
			break;
		case Difficulty::MEDIUM:
			if (solveTimer < bestTimes.medium) bestTimes.medium = solveTimer;
			break;
		case Difficulty::HARD:
			if (solveTimer < bestTimes.hard) bestTimes.hard = solveTimer;
			break;
		}
	}

	void SaveBestTimes()
	{
		std::ofstream file("best_times.txt");
		if (file.is_open())
		{
			file << bestTimes.easy << "\n";
			file << bestTimes.medium << "\n";
			file << bestTimes.hard << "\n";
			file.close();
		}
		else
			std::cerr << "Error: Failed to save best times." << std::endl;
	}

	void LoadBestTimes()
	{
		std::ifstream file("best_times.txt");
		if (file.is_open())
		{
			std::string line;
			if (std::getline(file, line)) bestTimes.easy = std::stof(line);
			if (std::getline(file, line)) bestTimes.medium = std::stof(line);
			if (std::getline(file, line)) bestTimes.hard = std::stof(line);
			file.close();
		}
		else {
			std::cerr << "Error: Failed to load best times. Creating a new file." << std::endl;
		}
	}

	void EraseBestTimes()
	{
		bestTimes.easy = std::numeric_limits<float>::max();
		bestTimes.medium = std::numeric_limits<float>::max();
		bestTimes.hard = std::numeric_limits<float>::max();
	}
};


int main()
{
	Sudoku sudoku;
	if (sudoku.Construct(400, 500, 1, 1))
		sudoku.Start();
	return 0;
}