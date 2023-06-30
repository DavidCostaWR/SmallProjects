#define OLC_PGE_APPLICATION
#include "../olcPixelGameEngine.h"
#include <vector>

using olc::vf2d;
using std::vector;
using std::pair;

// Constants

class sBall
{
public:
	vf2d p, v, a;
	float radius, mass, rot;
	int id;
	bool bSelected, bStable;
};

// Override base class with your custom functionality
class BallsInes : public olc::PixelGameEngine
{
public:
	BallsInes()
	{
		// Name your application
		sAppName = "Balls";
	}

private:
	vector<sBall> vecBalls;

public:
	olc::Sprite* spriteBalls = nullptr;
	float fBallTimer = 0, fBallFrequency = 0.0f;
	bool bBounce = true, bStability = false, bGravity = true;
	enum CueState { Hold, SelectBall, PositionCue, Shoot };
	int smCue = Hold;
	int nCalcPerFrame = 1;
	sBall* SelectedBall = nullptr;
	std::map<std::string, olc::Decal*> m_mapDecals;

public:
	float CalcDistance(float x1, float y1, float x2, float y2)
	{
		return sqrtf((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
	}
	float CalcDistance(vf2d p1, vf2d p2)
	{
		return sqrtf((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
	}

	void AddBall(vf2d pos, float r = 5.0f, float rot = 0.0f)
	{
		sBall b;
		float scale = 2.0f;
		b.p = pos;
		b.v = { 0, 0 };
		b.a = { 0, 0 };
		b.radius = r * scale;
		b.mass = r * 10.0f;
		b.rot = rot/180 * 2 * 3,14159;

		b.id = vecBalls.size();
		vecBalls.emplace_back(b);
	}

	void WrapAround(sBall& ball) {
		if (ball.p.x < -ball.radius) ball.p.x += ScreenWidth() + ball.radius;
		if (ball.p.y < -ball.radius) ball.p.y += ScreenHeight() + ball.radius;
		if (ball.p.x > ScreenWidth() + ball.radius) ball.p.x -= ScreenWidth() + ball.radius;
		if (ball.p.y > ScreenHeight() + ball.radius) ball.p.y -= ScreenHeight() + ball.radius;
	}

	void Bounce(sBall& ball)
	{
		if (ball.p.x > ScreenWidth() - ball.radius)
		{
			ball.p.x = ScreenWidth() - ball.radius;
			ball.v.x *= -0.5f;
		}
		if (ball.p.x < ball.radius)
		{
			ball.p.x = ball.radius;
			ball.v.x *= -0.5f;
		}
		if (ball.p.y > ScreenHeight() - ball.radius)
		{
			ball.p.y = ScreenHeight() - ball.radius;
			ball.v.y *= -0.5f;
		}
		if (ball.p.y < ball.radius)
		{
			ball.p.y = ball.radius;
			ball.v.y *= -0.5f;
		}

	}

	bool DoCirclesOverlap(const sBall& ball, const sBall& target)
	{
		return CalcDistance(ball.p, target.p) <= ball.radius + target.radius;
	}

	bool IsCursorInBall(const sBall& ball)
	{
		const float mx = GetMouseX();
		const float my = GetMouseY();
		const float px = ball.p.x;
		const float py = ball.p.y;
		const float r = ball.radius;
		return	mx <= px + r && mx >= px - r && my <= py + r && my >= py - r;
	}

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		olc::Sprite* s = new olc::Sprite("Assets/Ines.png");
		olc::Decal* d = new olc::Decal(s);
		m_mapDecals["sInes"] = d;

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{

		// User input
		if (GetKey(olc::Key::ESCAPE).bReleased)	return false;								// Quit game

		if (GetKey(olc::Key::SPACE).bPressed)	bBounce = !bBounce;							// Toggle bounce / wrap around screen
		if (GetKey(olc::Key::G).bPressed)		bGravity = !bGravity;

		if (GetKey(olc::Key::UP).bHeld)			fBallFrequency += 10 * fElapsedTime;		// Update ball population frequency
		if (GetKey(olc::Key::DOWN).bHeld)		fBallFrequency -= 10 * fElapsedTime;
		if (GetKey(olc::Key::LEFT).bPressed)	fBallFrequency = (int)fBallFrequency - 1;
		if (GetKey(olc::Key::RIGHT).bPressed)	fBallFrequency = (int)fBallFrequency + 1;

		if (GetMouse(1).bPressed && smCue != PositionCue)									// Add Ball by mouse click
			AddBall({ (float)GetMouseX(), (float)GetMouseY() }, rand() % 8 + 4, rand() % 365);

		if (GetKey(olc::Key::BACK).bPressed && smCue != PositionCue)						// Remove last ball
			vecBalls.pop_back();

		if (GetKey(olc::Key::R).bPressed)		fBallFrequency = 0;							// Reset ball population frequency
		if (fBallFrequency <= 0)				fBallFrequency = 0;

		// Add Ball periodically
		fBallTimer += fElapsedTime;
		if (fBallTimer >= 1 / fBallFrequency && smCue != PositionCue) {
			fBallTimer = 0;
			AddBall({ (float)(rand() % ScreenWidth()),0 },
				rand() % 8 + 4, rand() % 365);
		}

		for (int i = 0; i < nCalcPerFrame; i++)

		{
			// Ball Physics
			vector<pair<sBall*, sBall*>> vecCollidingPairs;

			bStability = true;
			// Update Ball Positions

			// Static collisions, i.e. overlap
			for (auto& ball : vecBalls)
			{
				for (auto& target : vecBalls)
				{
					if (ball.id != target.id) {
						if (DoCirclesOverlap(ball, target))
						{
							// Collision has occured
							vecCollidingPairs.push_back({ &ball, &target });

							// Distance between ball centers
							float fDistance = CalcDistance(ball.p, target.p);

							// Calculate displacement required
							float fOverlap = 0.5 * (fDistance - ball.radius - target.radius);

							// Displace balls away from collision
							ball.p -= fOverlap * (ball.p - target.p) / fDistance;
							target.p -= fOverlap * (target.p - ball.p) / fDistance;
						}
					}
				}
			}

			// Work out dynamic collisions
			for (auto c : vecCollidingPairs)
			{
				sBall* b1 = c.first;
				sBall* b2 = c.second;

				// Distance between balls
				float fDistance = CalcDistance(b1->p, b2->p);

				// Normal
				float nx = (b2->p.x - b1->p.x) / fDistance;
				float ny = (b2->p.y - b1->p.y) / fDistance;

				// Tangent
				float tx = -ny;
				float ty = nx;

				// Dot Product Tangent
				float dpTan1 = b1->v.x * tx + b1->v.y * ty;
				float dpTan2 = b2->v.x * tx + b2->v.y * ty;

				// Dot Product Normal
				float dpNorm1 = b1->v.x * nx + b1->v.y * ny;
				float dpNorm2 = b2->v.x * nx + b2->v.y * ny;

				// Conservation of momentum in 1D
				float m1 = (dpNorm1 * (b1->mass - b2->mass) + 2.f * b2->mass * dpNorm2) / (b1->mass + b2->mass);
				float m2 = (dpNorm2 * (b2->mass - b1->mass) + 2.f * b1->mass * dpNorm1) / (b1->mass + b2->mass);

				// Update ball velocities
				b1->v.x = tx * dpTan1 + nx * m1;
				b1->v.y = ty * dpTan1 + ny * m1;
				b2->v.x = tx * dpTan2 + nx * m2;
				b2->v.y = ty * dpTan2 + ny * m2;

				// Clamp velocity near zero
				if (b1->v.x * b1->v.x + b1->v.y * b1->v.y < 0.1f)
					b1->v = { 0, 0 };
				if (b2->v.x * b2->v.x + b2->v.y * b2->v.y < 0.1f)
					b2->v = { 0, 0 };
			}

		}
		for (auto& ball : vecBalls)
		{
			ball.bStable = false;
			// Add Drag to emulate rolling friction
			ball.a = -ball.v * ball.mass * 0.01f;
			ball.a.y += 100.0f * bGravity;

			// Update ball physics
			ball.v += ball.a * fElapsedTime;
			ball.p += ball.v * fElapsedTime;

			// Wrap the balls around screen or bounce

			if (!bBounce)
				WrapAround(ball);
			if (bBounce)
				Bounce(ball);

			// Clamp velocity near zero
			if (ball.v.x * ball.v.x + ball.v.y * ball.v.y < 0.1f)
			{
				ball.v = { 0, 0 };
				ball.bStable = true;
			}

			// Rotation

			ball.rot += ball.v.x / ball.radius / 100;

			// Determine overall stability
			bStability &= ball.bStable;

		}

		if (GetKey(olc::Key::CTRL).bHeld) {
			if (GetMouse(0).bPressed) {
				for (auto& ball : vecBalls) {
					if (IsCursorInBall(ball)) {
						ball.bSelected = !ball.bSelected;
					}
				}
			}
		}

		if (GetKey(olc::Key::UP).bHeld) {
			for (auto& ball : vecBalls) {
				if (ball.bSelected)
					ball.v.y = -10;
			}
		}

		// Cue state machine
		switch (smCue)
		{
		case Hold:
			if (true/*bStability*/)
				smCue = SelectBall;
			break;
		case SelectBall:
			if (GetMouse(0).bPressed)
			{
				for (auto& ball : vecBalls) {
					if (IsCursorInBall(ball))
					{
						SelectedBall = &ball;
						smCue = PositionCue;
						break;
					}
				}
			}
			break;
		case PositionCue:
			SelectedBall->v.x = (GetMouseX() - SelectedBall->p.x) * 100.0f;
			SelectedBall->v.y = (GetMouseY() - SelectedBall->p.y) * 100.0f;
			if (GetMouse(0).bPressed)
			{
				SelectedBall = nullptr;
				smCue = Shoot;
			}
			break;
		case Shoot:
			smCue = Hold;
			break;
		}



		// Clear Screen
		FillRect({ 0,0 }, { ScreenWidth(),ScreenHeight() }, olc::BLACK);

		// Draw Balls
		for (auto ball : vecBalls)
		{
			float scale = ball.radius / 256;
			vf2d o = { ball.radius,ball.radius };
			DrawRotatedDecal(ball.p, m_mapDecals["sInes"], ball.rot, {256,256}, {scale, scale});
			//DrawCircle(ball.p, ball.radius);
			/*if (SelectedBall != nullptr)
				if (ball.id == SelectedBall->id) FillCircle(ball.p, ball.radius);*/
		}

		DrawStringDecal({ 4,0 }, "###  " + std::to_string((int)fBallFrequency) + " Ines por segundo  ###");
		DrawStringDecal({ 4,10 }, "Mouse Esq: Selecionar uma Ines");
		DrawStringDecal({ 4,20 }, "Mouse Drt: Gerar uma Ines");
		DrawStringDecal({ 4,30 }, "Tecla -> : Mais Ineses");
		DrawStringDecal({ 4,40 }, "Tecla <- : Menos Ineses");

		//DrawStringDecal({ 4,12 }, bStability ? "Stable" : "Unstable");
		//DrawStringDecal({ 4,8 }, bGravity ? "Gravity ON" : "Gravity OFF");
	}
};

int main()
{
	BallsInes game;
	if (game.Construct(960, 540, 2, 2,true))
		game.Start();
	return 0;
}