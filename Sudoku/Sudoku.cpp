#define OLC_PGE_APPLICATION
#include "../olcPixelGameEngine.h"
#include <vector>
#include <algorithm>
#include <random>
/*
			px	#	T
Cell		20	9	180
Frame		3	2	6
line		1	6	6
3x3 grid	2	2	4
Total H&W			196

Number = 8x8

*/

enum class GameState
{
	MAIN_MENU,
	PUZZLE_CREATION,
	USER_SOLVING,
	VICTORY,
	DISPLAY_STATS
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

	float easyRecord = 0.0f;
	float mediumRecord = 0.0f;
	float hardRecord = 0.0f;
	UINT8 difficulty = 0;
	int grid[9][9]{};
	int solutionGrid[9][9]{};
	int solverGrid[9][9]{};
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
			ClearCells(difficulty * 10);
			gameState = GameState::USER_SOLVING;
			break;
		case GameState::USER_SOLVING:
			// update timer
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
				gameState = GameState::VICTORY;
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
		DrawString(10, 30, "Sudoku Generator", olc::WHITE, 2);
		DrawString(10, 60, "Select Difficulty:", olc::WHITE, 1);

		DrawString(10, 90, "1. Easy", olc::WHITE, 1);
		DrawString(10, 110, "2. Medium", olc::WHITE, 1);
		DrawString(10, 130, "3. Hard", olc::WHITE, 1);

		if (GetKey(olc::Key::K1).bPressed || GetKey(olc::Key::NP1).bPressed)
		{
			difficulty = 1;
			gameState = GameState::PUZZLE_CREATION;
		}
		if (GetKey(olc::Key::K2).bPressed || GetKey(olc::Key::NP2).bPressed)
		{
			difficulty = 2;
			gameState = GameState::PUZZLE_CREATION;
		}
		if (GetKey(olc::Key::K3).bPressed || GetKey(olc::Key::NP3).bPressed)
		{
			difficulty = 3;
			gameState = GameState::PUZZLE_CREATION;
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
		if (GetKey(olc::Key::RIGHT).bPressed)	cursorX = (cursorX == 8 ? 0 : cursorX + 1);
		if (GetKey(olc::Key::LEFT).bPressed)	cursorX = (cursorX == 0 ? 8 : cursorX - 1);
		if (GetKey(olc::Key::DOWN).bPressed)	cursorY = (cursorY == 8 ? 0 : cursorY + 1);
		if (GetKey(olc::Key::UP).bPressed) 		cursorY = (cursorY == 0 ? 8 : cursorY - 1);

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
		const int nNumberScale = 1;
		const int nNumberSize = 8 * nNumberScale;

		const int nLineBrk = nNumberSize + margin;
		const int nFirstLineX = nHeaderPx + margin;
		const int nFirstLineY = nHeaderPy + margin;

		const int nMinutes = solveTimer / 60;
		const int nSeconds = solveTimer - nMinutes * 60;

		int nMinutesRecord;
		int nSecondsRecord;

		std::string sDifficulty;
		std::string sRecord;


		switch (difficulty)
		{
		case 1: 
			sDifficulty = "Easy";

			nMinutesRecord = easyRecord / 60;
			nSecondsRecord = easyRecord - nMinutesRecord * 60;
			break;
		case 2:
			sDifficulty = "Medium";

			nMinutesRecord = mediumRecord / 60;
			nSecondsRecord = mediumRecord - nMinutesRecord * 60;
			break;
		case 3:
			sDifficulty = "Hard";

			nMinutesRecord = hardRecord / 60;
			nSecondsRecord = hardRecord - nMinutesRecord * 60;
			break;
		default:
			sDifficulty = "ERROR";
			break;
		}

		std::string sMinutes = std::to_string(nMinutes);
		if (nMinutes < 10) sMinutes = "0" + sMinutes;

		std::string sSeconds = std::to_string(nSeconds);
		if (nSeconds < 10) sSeconds = "0" + sSeconds;

		std::string sMinutesRecord = std::to_string(nMinutesRecord);
		if (nMinutesRecord < 10) sMinutesRecord = "0" + sMinutesRecord;

		std::string sSecondsRecord = std::to_string(nSecondsRecord);
		if (nSecondsRecord < 10) sSecondsRecord = "0" + sSecondsRecord;

		DrawString(nFirstLineX + nHeaderWidth / 2, nFirstLineY + 0 * nLineBrk, "Difficulty: " + sDifficulty, olc::BLACK, nNumberScale);
		DrawString(nFirstLineX, nFirstLineY + 0 * nLineBrk, "Time: " + sMinutes + ":" + sSeconds, olc::BLACK, nNumberScale);
		DrawString(nFirstLineX, nFirstLineY + 1 * nLineBrk, "Record: " + sMinutesRecord + ":" + sSecondsRecord, olc::BLACK, nNumberScale);
	}

	void DrawCursor()
	{
		const float cursorMargin = n3x3GridDim;
		const float cursorSize = nCellDim - 2 * cursorMargin;
		FillRect(Grid2Screen(cursorX + 1, cursorY + 1, cursorMargin), olc::vi2d(cursorSize, cursorSize), olc::RED);
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
					if (!IsValidMove(row, col, num)) numColor = olc::DARK_RED;
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
					if ((grid[row][col] == nSelection && row != cursorY && col != cursorX) || 
						(solverGrid[row][col] == nSelection && row != cursorY && col != cursorX))
					{
						DrawString(Grid2Screen(col + 1, row + 1, margin), std::to_string(nSelection), olc::BLUE, nNumberScale);
					}
				}
			}
		}
	}

	void DrawStats()
	{
		switch (difficulty)
		{
		case 1:
			easyRecord = (solveTimer > easyRecord ? solveTimer : easyRecord);
			break;
		case 2:
			mediumRecord = (solveTimer > mediumRecord ? solveTimer : mediumRecord);
			break;
		case 3:
			hardRecord = (solveTimer > hardRecord ? solveTimer : hardRecord);
			break;
		}
		const int nMinutes = (int)(solveTimer / 60);
		const int nSeconds = (int)(solveTimer - nMinutes * 60);
		const std::string sSeconds = (nSeconds < 10 ? ("0" + std::to_string(nSeconds)) : std::to_string(nSeconds));

		const int nEasyMinutes = (int)(easyRecord / 60);
		const int nEasySeconds = (int)(easyRecord - nEasyMinutes * 60);
		const std::string sEasySeconds = (nEasySeconds < 10 ? ("0" + std::to_string(nEasySeconds)) : std::to_string(nEasySeconds));

		const int nMediumMinutes = (int)(mediumRecord / 60);
		const int nMediumSeconds = (int)(mediumRecord - nMediumMinutes * 60);
		const std::string sMediumSeconds = (nMediumSeconds < 10 ? ("0" + std::to_string(nMediumSeconds)) : std::to_string(nMediumSeconds));

		const int nHardMinutes = (int)(hardRecord / 60);
		const int nHardSeconds = (int)(hardRecord - nHardMinutes * 60);
		const std::string sHardSeconds = (nHardSeconds < 10 ? ("0" + std::to_string(nHardSeconds)) : std::to_string(nHardSeconds));


		int col = 30;
		DrawString(10, col, "Puzzle Solved!", olc::WHITE, 2); col += 30;
		DrawString(10, col, "Solve Time: " + std::to_string(nMinutes) + ":" + sSeconds, olc::WHITE, 1); col += 30;

		DrawString(10, col, "Best Times:", olc::WHITE, 2); col += 30;
		DrawString(10, col, "Easy: " + std::to_string(nEasyMinutes) + ":" + sEasySeconds, olc::WHITE, 1); col += 15;
		DrawString(10, col, "Medium: " + std::to_string(nMediumMinutes) + ":" + sMediumSeconds, olc::WHITE, 1); col += 15;
		DrawString(10, col, "Hard: " + std::to_string(nHardMinutes) + ":" + sHardSeconds, olc::WHITE, 1); col += 30;

		DrawString(10, col, "Press Enter for Main Menu", olc::WHITE, 1); col += 15;
		DrawString(10, col, "Press Escape to Exit", olc::WHITE, 1);

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
		int scale = ScreenWidth() / Solved.length() / 9;
		int px = (ScreenWidth() - Solved.length() * scale * 8) / 2;
		int py = (ScreenHeight() - scale) / 2;

		if ((int)(solveDelayTimer * 2) % 3 != 0)
			DrawString(px, py, Solved, olc::RED, scale);

		if (solveDelayTimer > delay || GetKey(olc::Key::SPACE).bPressed || GetKey(olc::Key::ENTER).bPressed || GetKey(olc::Key::ESCAPE).bPressed)
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

};

int main()
{
	Sudoku sudoku;
	if (sudoku.Construct(400, 500, 1, 1))
		sudoku.Start();
	return 0;
}