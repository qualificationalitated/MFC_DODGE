#pragma once
#include<chrono>
#include "BulletRed.h"
#include "Player.h"
#include <time.h>
#include <math.h>
#include <afxtempl.h>

enum class KeyDirection {ArrowUp,ArrowDown,ArrowRight,ArrowLeft};

class Game
{
private:
	CRect m_playingArea{ 0,0,1000,800 };
	bool m_gameStarted{ false };
	bool m_gameEnded{ false };
	void updateBullet(double timeElapsed,BulletRed& bulletRed);
	void updatePlayer(double timeElapsed);
	std::chrono::steady_clock::time_point m_previousUpdateTime;
	// Åº¸· & ÇÃ·¹ÀÌ¾î °´Ã¼ »ý¼º
	Player m_player{ _T("Player.png"),CSize{44, 40}, Position{m_playingArea.right / 2.0,m_playingArea.bottom / 2.0}, Direction{0, 0}, 250.0 };
	// void setInitialBallStatus();
	// void makeRedBulletGroup();
	int randomSpeed = (rand() % 100) + 150.0;
	int randPosX{ (rand() % 1020) }, randPosY{ (rand() % 885 ) };
	BulletRed m_RedBullet1{ _T("BulletRed.png") ,CSize{14,14},
		Position{(double)randPosX,(double)randPosY},
		Direction{m_playingArea.CenterPoint().x - (double)randPosX ,m_playingArea.CenterPoint().y - (double)randPosY },
		randomSpeed };
	
	// BulletRed m_RedBullet1{ _T("BulletRed.png") ,CSize{14,14}, Position{200.0,100.0}, Direction{1.0,1.0},200.0 };
	// BulletRed m_RedBullet2{ _T("BulletRed.png") ,CSize{14,14}, Position{200.0,100.0}, Direction{1.0,-3.0},200.0 };
	// BulletRed m_RedBullet3{ _T("BulletRed.png") ,CSize{14,14}, Position{200.0,100.0}, Direction{4.0,1.0},200.0 };
	

public:
	void render(FlickerFreeDC& dc);
	void update();
	
	void DirKeyDown(KeyDirection key);
	void DirKeyUp(KeyDirection key);

	void start();
	void end();
	int getGameStatus();
	
};

