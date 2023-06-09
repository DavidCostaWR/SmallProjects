#include <random>
#include "olcConsoleGameEngine.h"

enum Keys
{
	Key_W = 87,
	Key_S = 83,
	Key_A = 65,
	Key_D = 68

};
struct sBall
{
	float px, py;
	float vx, vy;
	float speed;
	float ang;
	float radius = 2.0f;
	bool dead = false;
};

struct sBar
{
	float px, py;
	float vy;
	float w, h;
	std::string side;
};

float deg2rad(float ang)
{
	return ang * 3.14159f / 180.0f;
}

class Pong : public olcConsoleGameEngine
{
public:
	Pong()
	{
		m_sAppName = L"Pong";
	}

private:
	std::vector<sBall> vecBalls;
	std::vector<sBar> vecBars;
	float fGameSpeed;
	float fSpeedModifier;
	int nBallCounter;
	int nDefenseCounter;
	
	void AddBall(float x, float y, float Speed)
	{
		float angOffset = 30.0f;	//degrees
		std::random_device rd;
		std::mt19937 gen(rd());
		//std::uniform_real_distribution<float> dis(-1, 1);
		std::uniform_real_distribution<float> ang(deg2rad(angOffset), deg2rad(90.0f - angOffset));
		std::uniform_real_distribution<float> quad(1, 5);

		sBall b;
		
		b.px = x;
		b.py = y;
		b.speed = Speed;

		b.ang = ang(gen) * (int)quad(gen);
		b.vx = cosf(b.ang) * b.speed;
		b.vy = sinf(b.ang) * b.speed;

		vecBalls.emplace_back(b);
	}

public:
	bool OnUserCreate()
	{
		// Set game parameters
		fGameSpeed = 200;
		fSpeedModifier = 1.25f;
		nBallCounter = 0;

		// Create Bars
		sBar LBar;

		LBar.w = 4;
		LBar.h = ScreenHeight() / 8;
		LBar.py = ScreenHeight() / 2;
		LBar.px = 8;
		LBar.vy = 0;

		sBar RBar = LBar;
		RBar.px = ScreenWidth() - LBar.px;

		LBar.side = "Left";
		RBar.side = "Right";

		vecBars.emplace_back(LBar);
		vecBars.emplace_back(RBar);
		return true;
	}

	bool OnUserUpdate(float fElapsedTime)
	{
		//Clear Screen
		Fill(0, 0, ScreenWidth(), ScreenHeight(), ' ');

		//Create Ball
		if (vecBalls.empty())
		{
			AddBall(ScreenWidth() / 2, ScreenHeight() / 2, fGameSpeed);
			nBallCounter++;
		}

		//Add balls after set number of defenses
		if (nDefenseCounter >= 5)
		{
			AddBall(ScreenWidth() / 2, ScreenHeight() / 2, fGameSpeed);
			nBallCounter++;
			nDefenseCounter -= 5;
		}

		//Add balls manually
		if (m_keys[VK_RETURN].bPressed)
		{
			AddBall(ScreenWidth() / 2, ScreenHeight() / 2, fGameSpeed);
			nBallCounter++;
		}

		//Update speed
		if (nBallCounter >= 5)
		{
			fGameSpeed *= fSpeedModifier;
			nBallCounter -= 5;
		}

		//Update bars position
		/*if (IsFocused()) {
			for (auto& bar : vecBars) {
				bar.py = m_mousePosY;
			}
		}*/

		//Update bars position
		for (auto& bar : vecBars)
		{
			bar.vy = 0;

			if (bar.side == "Left") 
			{
				float Speed = 300;
				if (m_keys[VK_LSHIFT].bHeld)
					Speed *= 2;
				if (m_keys[Key_W].bHeld)
					bar.vy = -Speed;
				else if (m_keys[Key_S].bHeld)
					bar.vy = Speed;
			}

			if (bar.side == "Right")
			{
				float Speed = 300;
				if (m_keys[VK_RSHIFT].bHeld)
					Speed *= 2;
				if (m_keys[VK_UP].bHeld)
					bar.vy = -Speed;
				else if (m_keys[VK_DOWN].bHeld)
					bar.vy = Speed;
			}

			bar.py += bar.vy * fElapsedTime;
			if (bar.py < 0)
				bar.py = 0;
			if (bar.py > ScreenHeight())
				bar.py = ScreenHeight();
		}

		//Update balls position
		for (auto &ball : vecBalls)
		{
			ball.vx = cosf(ball.ang) * ball.speed;
			ball.vy = sinf(ball.ang) * ball.speed;

			ball.px += ball.vx * fElapsedTime;
			ball.py += ball.vy * fElapsedTime;

			// collision with bars
			for (auto& bar : vecBars)
			{
				int rad = (ball.vx > 0) ? ball.radius : -ball.radius;

				if (ball.py >= bar.py - bar.h / 2 &&
					ball.py <= bar.py + bar.h / 2 &&
					abs(ball.px - bar.px) <= (ball.radius + bar.w / 2))
				{
					/*ball.ang += deg2rad(180);
					if (ball.ang >= deg2rad(360))
						ball.ang -= deg2rad(360);*/

					if (ball.vx > 0)
					{
						ball.ang = (bar.py - ball.py) / (bar.h/2) * deg2rad(70) + deg2rad(180);
					}
					else
					{
						ball.ang = (ball.py - bar.py) / (bar.h / 2) * deg2rad(70);
						if (ball.ang < 0)
							ball.ang += deg2rad(360);
					}

					nDefenseCounter += 1;
					break;
				}
			}

			// collision with edge
			if (ball.py >= ScreenHeight() - ball.radius || ball.py <= ball.radius)
			{
				ball.ang = atan2(-ball.vy, ball.vx);
			}

			if (ball.px >= ScreenWidth() - ball.radius || ball.px <= ball.radius)
			{
				ball.dead = true;
				nDefenseCounter =0;

			}
		}

		//Remove balls
		vecBalls.erase(std::remove_if(vecBalls.begin(), vecBalls.end(), [](sBall& ball) {
			return ball.dead;
			}), vecBalls.end());

		//Draw balls
		for (auto &ball : vecBalls) 
		{
			FillCircle(ball.px, ball.py, ball.radius);
		}

		//Draw bars
		for (auto& bar : vecBars)
		{
			Fill(bar.px - bar.w / 2,
				bar.py - bar.h / 2,
				bar.px + bar.w / 2,
				bar.py + bar.h / 2);
		}

		return true;
	}
};


int main()
{
	Pong game;
	if (game.ConstructConsole(640, 320, 2, 2))
		game.Start();
	else
		std::wcout << L"Could not contruct console" << std::endl;

	return 0;
};