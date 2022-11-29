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
	CRect m_playingArea{ 0,0,1000,800 }; // 플레이 영역
	bool m_gameStarted{ false }; // 게임 시작상태 표시
	bool m_gameEnded{ false }; // 게임 종료상태
	void updateBullet(double timeElapsed,BulletRed& bulletRed); // 총알 업데이트
	void updatePlayer(double timeElapsed); // 플레이어 업데이트
	std::chrono::steady_clock::time_point m_previousUpdateTime; // 업데이트를 위한 시간 기록 변수
	std::chrono::steady_clock::time_point m_gameStartTime; // 게임 시작된 시간을 기록하는 부분 
	std::chrono::milliseconds m_gameScore; //시간을 기반으로 하는 게임 점수
	BulletRed* generateBullet();
	boolean playerCollisionDetection(double r1x, double r1y, double r1w, double r1h, double r2x, double r2y, double r2w, double r2h);

	// 탄막 속도, 탄막 갯수, 플레이어 속도 제어 변수
	int m_nowBulletCount = 0,m_maxBulletCount=25;
	int m_endScore = 0;
	double m_initBulletSpeed = 100.0;
	double m_initPlayerSpeed = 250.0;

	// 탄막 & 플레이어 객체 생성
	Player m_player{ _T("Player.png"),CSize{44, 40}, Position{m_playingArea.right / 2.0,m_playingArea.bottom / 2.0}, Direction{0, 0}, m_initPlayerSpeed };
	BulletRed* bulletList;

public:
	void render(FlickerFreeDC& dc);
	void update();
	
	void DirKeyDown(KeyDirection key);
	void DirKeyUp(KeyDirection key);

	void start();
	void end();
	int getGameStatus();
};

