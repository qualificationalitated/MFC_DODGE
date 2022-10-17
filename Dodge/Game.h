#pragma once
#include<chrono>
#include "BulletRed.h"

class Game
{
private:
	void updateBullet(double timeElapsed,BulletRed& bulletRed);
	CRect m_playingArea{50,50,900,700};
	bool m_gameStarted{ false };
	std::chrono::steady_clock::time_point m_previousUpdateTime;
	// 河篮 藕阜按眉 积己
	BulletRed m_RedBullet{ _T("BulletRed.png") ,CSize{14,14}, Position{200.0,100.0},
		Direction{1.0,1.0},200.0 };
	BulletRed m_RedBullet2{ _T("BulletRed.png") ,CSize{14,14}, Position{200.0,100.0},
		Direction{1.0,-3.0},200.0 };
	BulletRed m_RedBullet3{ _T("BulletRed.png") ,CSize{14,14}, Position{200.0,100.0},
		Direction{4.0,1.0},200.0 };
public:
	void start();
	void update();
	void render(FlickerFreeDC& dc);

};

