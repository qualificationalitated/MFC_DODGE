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
	CRect m_playingArea{ 0,0,1000,800 }; // �÷��� ����
	bool m_gameStarted{ false }; // ���� ���ۻ��� ǥ��
	bool m_gameEnded{ false }; // ���� �������
	void updateBullet(double timeElapsed,BulletRed& bulletRed); // �Ѿ� ������Ʈ
	void updatePlayer(double timeElapsed); // �÷��̾� ������Ʈ
	std::chrono::steady_clock::time_point m_previousUpdateTime; // ������Ʈ�� ���� �ð� ��� ����
	std::chrono::steady_clock::time_point m_gameStartTime; // ���� ���۵� �ð��� ����ϴ� �κ� 
	std::chrono::milliseconds m_gameScore; //�ð��� ������� �ϴ� ���� ����
	BulletRed* generateBullet();
	boolean playerCollisionDetection(double r1x, double r1y, double r1w, double r1h, double r2x, double r2y, double r2w, double r2h);

	// ź�� �ӵ�, ź�� ����, �÷��̾� �ӵ� ���� ����
	int m_nowBulletCount = 0,m_maxBulletCount=25;
	int m_endScore = 0;
	double m_initBulletSpeed = 100.0;
	double m_initPlayerSpeed = 250.0;

	// ź�� & �÷��̾� ��ü ����
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

