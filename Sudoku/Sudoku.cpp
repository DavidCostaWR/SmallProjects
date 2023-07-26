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
	DISPLAY_STATS
};

class Sudoku : public olc::PixelGameEngine
{
private:
	const UINT8 nCellDim = 40;
	const UINT8 nFrameDim = 6;
	const UINT8 nlineDim = 1;
	const UINT8 n3x3GridDim = 3;
	const UINT8 nNumberScale = 3;

	float easyRecord = 0.0f;
	float mediumRecord = 0.0f;
	float hardRecord = 0.0f;
	UINT8 difficulty = 0;
	UINT8 grid[9][9]{};
	UINT8 cursorX = 0;
	UINT8 cursorY = 0;
	bool quit = false;
	float solveTimer = 0.0f;
	float fBackgroundTimer = 0.0f;
	GameState gameState = GameState::MAIN_MENU;
	std::mt19937 rng{ std::random_device{}() };

public:
	Sudoku() { sAppName = "Sudoku"; }

public:
	bool OnUserCreate() override
	{
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		switch (gameState)
		{
		case GameState::MAIN_MENU:
			// Handle main menu
			solveTimer = 0;

			DrawBackground(fElapsedTime, olc::DARK_BLUE, olc::Pixel(0, 0, 200));
			DrawMainMenu();
			break;
		case GameState::PUZZLE_CREATION:
			GenerateSudoku();
			ClearCells(difficulty * 10);
			gameState = GameState::USER_SOLVING;
			break;
		case GameState::USER_SOLVING:
			solveTimer += fElapsedTime;
			HandleInput();

			DrawBackground(fElapsedTime, olc::DARK_BLUE, olc::Pixel(0, 0, 200));
			DrawGrid();
			DrawCursor();
			DrawNumbers();
			if (CheckVictory())
				gameState = GameState::DISPLAY_STATS;
			break;
		case GameState::DISPLAY_STATS:
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
	}

	void ClearCells(int cellsToClear)
	{
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
			if (grid[i][col] == num || grid[row][i] == num)
				return false;
		}

		int startRow = row - row % 3;
		int startCol = col - col % 3;
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				if (grid[i + startRow][j + startCol] == num)
					return false;
			}
		}

		return true;
	}

	bool CheckVictory()
	{
		for (int i = 0; i < 9; ++i) {
			for (int j = 0; j < 9; ++j) {
				if (grid[i][j] == 0)
					return false;
			}
		}
		return true;
	}

	void HandleInput()
	{
		int newCellValue = 0;

		if (GetKey(olc::Key::ESCAPE).bPressed) gameState = GameState::MAIN_MENU;

		if (GetKey(olc::Key::RIGHT).bPressed) {
			cursorX = (cursorX == 8 ? 0 : cursorX + 1);
		}
		if (GetKey(olc::Key::LEFT).bPressed) {
			cursorX = (cursorX == 0 ? 8 : cursorX - 1);
		}
		if (GetKey(olc::Key::DOWN).bPressed) {
			cursorY = (cursorY == 8 ? 0 : cursorY + 1);
		}
		if (GetKey(olc::Key::UP).bPressed) {
			cursorY = (cursorY == 0 ? 8 : cursorY - 1);
		}

		if (GetKey(olc::Key::K1).bPressed || GetKey(olc::Key::NP1).bPressed) newCellValue = 1;
		if (GetKey(olc::Key::K2).bPressed || GetKey(olc::Key::NP2).bPressed) newCellValue = 2;
		if (GetKey(olc::Key::K3).bPressed || GetKey(olc::Key::NP3).bPressed) newCellValue = 3;
		if (GetKey(olc::Key::K4).bPressed || GetKey(olc::Key::NP4).bPressed) newCellValue = 4;
		if (GetKey(olc::Key::K5).bPressed || GetKey(olc::Key::NP5).bPressed) newCellValue = 5;
		if (GetKey(olc::Key::K6).bPressed || GetKey(olc::Key::NP6).bPressed) newCellValue = 6;
		if (GetKey(olc::Key::K7).bPressed || GetKey(olc::Key::NP7).bPressed) newCellValue = 7;
		if (GetKey(olc::Key::K8).bPressed || GetKey(olc::Key::NP8).bPressed) newCellValue = 8;
		if (GetKey(olc::Key::K9).bPressed || GetKey(olc::Key::NP9).bPressed) newCellValue = 9;

		if (grid[cursorY][cursorX] == 0 && newCellValue != 0 && IsValidMove(cursorY, cursorX, newCellValue))
			grid[cursorY][cursorX] = newCellValue;
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

	void DrawCursor()
	{
		const int nBoardDim = 9 * nCellDim + 2 * nFrameDim;

		// Center board with screen
		int px = (ScreenWidth() - nBoardDim) / 2 + nFrameDim;
		int py = (ScreenHeight() - nBoardDim) / 2 + nFrameDim;

		int cursorScreenX = px + cursorX * nCellDim + n3x3GridDim;
		int cursorScreenY = py + cursorY * nCellDim + n3x3GridDim;
		FillRect(cursorScreenX, cursorScreenY, nCellDim - 2 * n3x3GridDim, nCellDim - 2 * n3x3GridDim, olc::CYAN);
	}

	void DrawGrid()
	{
		const int nBoardDim = 9 * nCellDim + 2 * nFrameDim;

		// Center board with screen
		int px = (ScreenWidth() - nBoardDim) / 2;
		int py = (ScreenHeight() - nBoardDim) / 2;

		// Draw white board
		FillRect(px, py, nBoardDim, nBoardDim, olc::WHITE);	

		// Draw Frame
		for (int i = 0; i < nFrameDim; ++i)
		{
			DrawRect(px + i, py + i, nBoardDim - 2 * i, nBoardDim - 2 * i, olc::BLACK);
		}

		// Draw grid
		for (int i = 1; i < 9; ++i)
		{
			int x = px + i * nCellDim + nFrameDim;
			int y = py + i * nCellDim + nFrameDim;

			for (int j = 0; j < (int)(nlineDim / 2) + 1; ++j)
			{
				DrawLine(x + j, py + nFrameDim, x + j, py + nFrameDim + nBoardDim - 2 * nFrameDim, olc::GREY);
				DrawLine(x - j, py + nFrameDim, x - j, py + nFrameDim + nBoardDim - 2 * nFrameDim, olc::GREY);
				DrawLine(px + nFrameDim, y + j, px + nFrameDim + nBoardDim - 2 * nFrameDim, y + j, olc::GREY);
				DrawLine(px + nFrameDim, y - j, px + nFrameDim + nBoardDim - 2 * nFrameDim, y - j, olc::GREY);
			}
		}

		// Draw 3x3 grid
		for (int i = 1; i < 3; ++i)
		{
			int x = px + i * 3 * nCellDim + nFrameDim;
			int y = py + i * 3 * nCellDim + nFrameDim;

			for (int j = 0; j < (int)(n3x3GridDim / 2) + 1; ++j)
			{
				DrawLine(x - j, py + nFrameDim, x - j, py + nFrameDim + nBoardDim - 2 * nFrameDim, olc::DARK_GREY);
				DrawLine(x + j, py + nFrameDim, x + j, py + nFrameDim + nBoardDim - 2 * nFrameDim, olc::DARK_GREY);
				DrawLine(px + nFrameDim, y + j, px + nFrameDim + nBoardDim - 2 * nFrameDim, y + j, olc::DARK_GREY);
				DrawLine(px + nFrameDim, y - j, px + nFrameDim + nBoardDim - 2 * nFrameDim, y - j, olc::DARK_GREY);
			}
		}
	}
		
	void DrawNumbers()
	{
		const int nBoardDim = 9 * nCellDim + 2 * nFrameDim;
		const int nNumberSize = 8 * nNumberScale;

		int px = (ScreenWidth() - nBoardDim) / 2 + nFrameDim;
		int py = (ScreenHeight() - nBoardDim) / 2 + nFrameDim;

		for (int row = 0; row < 9; ++row)
		{
			for (int col = 0; col < 9; ++col)
			{
				int num = grid[row][col];
				if (num != 0)
				{
					int x = px + col * nCellDim + (nCellDim - nNumberSize) / 2;
					int y = py + row * nCellDim + (nCellDim - nNumberSize) / 2;
					DrawString(x, y, std::to_string(num), olc::BLACK, nNumberScale);
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
};

int main()
{
	Sudoku sudoku;
	if (sudoku.Construct(400, 400, 1, 1))
		sudoku.Start();
	return 0;
}