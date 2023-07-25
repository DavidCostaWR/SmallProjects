#define OLC_PGE_APPLICATION
#include "../olcPixelGameEngine.h"
#include <vector>
#include <algorithm>
#include <random>


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
	float easyRecord = 0.0f;
	float mediumRecord = 0.0f;
	float hardRecord = 0.0f;
	int difficulty = 0;
	int grid[9][9]{};
	int cursorX = 0;
	int cursorY = 0;
	bool quit = false;
	float solveTimer = 0.0f;
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
			Clear(olc::BLACK);
			DrawCursor(10,10, 180, 180);
			DrawGrid(10, 10, 180, 180);
			DrawNumbers(10, 10, 180, 180);
			if (CheckVictory())
				gameState = GameState::DISPLAY_STATS;
			break;
		case GameState::DISPLAY_STATS:
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
		Clear(olc::BLACK);
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

	void DrawCursor(int x, int y, int width, int height)
	{
		int cellSize = width / 9;
		int cursorScreenX = x + cursorX * cellSize;
		int cursorScreenY = y + cursorY * cellSize;
		FillRect(cursorScreenX, cursorScreenY, cellSize, cellSize, olc::CYAN);
	}
	
	void DrawGrid(int x, int y, int width, int height)
	{
		int cellSize = width / 9;
		for (int i = 0; i <= 9; ++i)
		{
			int px = x + i * cellSize;
			int py = y + i * cellSize;
			DrawLine(px, y, px, y + height);
			DrawLine(x, py, x + width, py);
			if (i % 3 == 0)
			{
				FillRect(px - 1, y, 2, height, olc::DARK_GREY);
				FillRect(x, py - 1, width, 2, olc::DARK_GREY);
			}
		}
	}

	void DrawNumbers(int x, int y, int width, int height)
	{
		int cellSize = width / 9;
		for (int row = 0; row < 9; ++row)
		{
			for (int col = 0; col < 9; ++col)
			{
				int num = grid[row][col];
				if (num != 0)
				{
					int px = x + col * cellSize + cellSize / 6;
					int py = y + row * cellSize + cellSize / 6;
					DrawString(px, py, std::to_string(num), olc::WHITE, 2);
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
		int minutes = (int)(solveTimer / 60);
		int seconds = (int)(solveTimer - minutes * 60);
		Clear(olc::BLACK);
		int col = 30;
		DrawString(10, col, "Puzzle Solved!", olc::WHITE, 2); col += 30;
		DrawString(10, col, "Solve Time: " + std::to_string(minutes) + ":" + std::to_string(seconds), olc::WHITE, 1); col += 30;
		DrawString(10, col, "Best Times:", olc::WHITE, 2); col += 30;
		DrawString(10, col, "Easy: " + std::to_string((int)(easyRecord / 60)) + ":" + std::to_string((int)(easyRecord - (int)(easyRecord / 60) * 60))); col += 15;
		DrawString(10, col, "Medium: " + std::to_string((int)(mediumRecord / 60)) + ":" + std::to_string((int)(mediumRecord - (int)(mediumRecord / 60) * 60))); col += 15;
		DrawString(10, col, "Hard: " + std::to_string((int)(hardRecord / 60)) + ":" + std::to_string((int)(hardRecord - (int)(hardRecord / 60) * 60))); col += 30;
		DrawString(10, col, "Press Enter for Main Menu", olc::WHITE, 1); col += 15;
		DrawString(10, col, "Press Escape to Exit", olc::WHITE, 1);

		if (GetKey(olc::Key::ENTER).bPressed) gameState = GameState::MAIN_MENU;
		if (GetKey(olc::Key::ESCAPE).bPressed) quit = true;
	}
};

int main()
{
	Sudoku sudoku;
	if (sudoku.Construct(300, 300, 2, 2))
		sudoku.Start();
	return 0;
}