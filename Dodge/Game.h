#pragma once
#include<chrono>
#include "BulletRed.h"
#include "Player.h"
#include <time.h>
#include <math.h>
#include <afxtempl.h>

// 키 방향을 받기 위한 enum 변수
enum class KeyDirection {ArrowUp,ArrowDown,ArrowRight,ArrowLeft};

class Game
{
private:
	// 시스템
	// 총알 상태 업데이트
	void updateBullet(double timeElapsed, BulletRed& bulletRed); 
	// 플레이어 상태 업데이트
	void updatePlayer(double timeElapsed); 

	bool m_gameStarted{ false }; // 게임 시작상태 표시
	bool m_gameEnded{ false }; // 게임 종료상태
	CRect m_playingArea{ 0,0,1000,800 }; // 플레이 영역
	
	// 탄막
	boolean playerCollisionDetection(double r1x, double r1y, double r1w, double r1h, double r2x, double r2y, double r2w, double r2h); // 플레이어 충돌판정 함수
	BulletRed* generateBullet(); // 총알 객체 동적생성 함수

	// 시간 변수
	std::chrono::steady_clock::time_point m_previousUpdateTime; // 업데이트를 위한 시간 기록 변수
	std::chrono::steady_clock::time_point m_gameStartTime; // 게임 시작된 시간을 기록하는 부분 
	std::chrono::milliseconds m_gameScore; //시간을 기반으로 하는 게임 점수
	
	// 탄막 속도, 탄막 갯수, 플레이어 속도 제어 변수
	int m_nowBulletCount = 0,m_maxBulletCount=25;
	int m_endScore = 0;
	double m_initBulletSpeed = 100.0;
	double m_initPlayerSpeed = 250.0;

	// 탄막 & 플레이어 객체 생성
	Player m_player{ _T("Player.png"),CSize{44, 40}, Position{m_playingArea.right / 2.0,m_playingArea.bottom / 2.0}, Direction{0, 0}, m_initPlayerSpeed };
	BulletRed* bulletList;

public:
	// 화면 렌더링을 위한 함수
	void render(FlickerFreeDC& dc);

	// 화면 업데이트용 함수
	void update();

	// 뷰 클래스의 키 다운을 제어하기 위한 함수
	void DirKeyDown(KeyDirection key);

	// 뷰 클래스의 키 업을 제어하기 위한 함수
	void DirKeyUp(KeyDirection key);

	// 게임 시작
	void start();

	// 게임 종료
	void end();

	// 게임 초기화
	void setInitialGame();

	// 게임 상태 반환
	int getGameStatus();
};

