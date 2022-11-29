#include "pch.h"
#include "Game.h"

void Game::render(FlickerFreeDC& dc) {
	Gdiplus::Graphics gr(dc);
	CFont font_300, font_200;
	font_300.CreatePointFont(300, _T("궁서"));
	font_200.CreatePointFont(200, _T("궁서"));
	// 게임 시작 전 초기화면
	if (!m_gameStarted) {
		// 게임 시작, 초기 실행화면 설정해주기
		dc.SelectObject(&font_300);
		SetTextAlign(dc, TA_CENTER);
		dc.TextOut(m_playingArea.CenterPoint().x, m_playingArea.CenterPoint().y - 120, _T("MFC_Dodge"));
		dc.SelectObject(&font_200);
		dc.TextOut(m_playingArea.CenterPoint().x, m_playingArea.CenterPoint().y+120, _T("게임 시작을 위해 스페이스바를 눌러주세요"));
		// dc.DrawText(, -1, m_playingArea, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		return;
	}
	
	// 게임 진행중 화면
	if (m_gameStarted && !m_gameEnded) {
		CString tempDir, tempTime;
		m_gameScore = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - m_gameStartTime);
		
		// 생존 시간을 활용한 스코어 계산
		tempTime.Format(_T("score : %d"), m_gameScore.count() / 10);
		dc.TextOut(30, 30, tempTime);
		
		// 화면에 나와있는 총알 갯수
		tempTime.Format(_T("Bullet Count : %d"), m_nowBulletCount);
		dc.TextOut(30, 50, tempTime);
		
		// 플레이어 및 객체 렌더링
		m_player.render(dc);
		for (int i = 1; i <= m_nowBulletCount; i++) 
			bulletList[i].render(dc);
		
		//플레이어 히트박스 확인
		/*
		auto hitbox{ m_player.getHitbox() };
		Pen pen4Hitbox(Color(255, 255, 255, 0), 2);
		gr.DrawRectangle(&pen4Hitbox, hitbox.left, hitbox.top, hitbox.Width(), hitbox.Height());
		*/
	}
	
	// 플레이어 피격 후 화면
	else if (m_gameEnded) {
		CString scoreText;
		dc.SelectObject(&font_200);
		SetTextAlign(dc, TA_CENTER);
		dc.TextOut(m_playingArea.CenterPoint().x , m_playingArea.CenterPoint().y, _T("게임종료!"));
		scoreText.Format(_T("최종점수 : %d"), m_endScore);
		dc.TextOut(m_playingArea.CenterPoint().x, m_playingArea.CenterPoint().y-30, scoreText);
	}
}

void Game::updateBullet(double timeElapsed,BulletRed& bulletRed) {

	// 총알이 벽에 부딛히거나, 벽을 통과하고자 할때인지 확인
	// 만일 충돌했다면, 총알의 방향을 바꾸고(튕기기) 공 움직임을 제어한다
	bulletRed.update(timeElapsed);
	Position BulletPos = bulletRed.getPosition();
	CSize BulletSize = bulletRed.getBulletSize();
	CRect PlayerPos = m_player.getHitbox();
	double speed = bulletRed.getSpeed();

	// 벽에 닿았다면, 플레이어를 향해 이동
	if (BulletPos.y + BulletSize.cy >= m_playingArea.bottom ||
		BulletPos.y <= m_playingArea.top ||
		BulletPos.x + BulletSize.cx >= m_playingArea.right ||
		BulletPos.x <= m_playingArea.left
		) {
		m_initBulletSpeed < 200 ? m_initBulletSpeed += 0.5 : m_initBulletSpeed = m_initBulletSpeed;
		speed = rand() % 150 + m_initBulletSpeed;
		// auto oldDirection{ bulletRed.getDirection() };
		bulletRed.setDirection(Direction{ PlayerPos.CenterPoint().x-BulletPos.x, PlayerPos.CenterPoint().y - BulletPos.y });
	}
	if (playerCollisionDetection(PlayerPos.left, PlayerPos.top, PlayerPos.Width(), PlayerPos.Height(), BulletPos.x, BulletPos.y, BulletSize.cx, BulletSize.cy)) {
		// 충돌시 게임종료
		end();
	}
	// 공 움직이기
	bulletRed.setSpeed(speed);
	bulletRed.update(timeElapsed);
}

boolean Game::playerCollisionDetection(double r1x, double r1y, double r1w, double r1h, double r2x, double r2y, double r2w, double r2h) {

	// are the sides of one rectangle touching the other?

	if (r1x + r1w >= r2x &&    // r1 right edge past r2 left
		r1x <= r2x + r2w &&    // r1 left edge past r2 right
		r1y + r1h >= r2y &&    // r1 top edge past r2 bottom
		r1y <= r2y + r2h) {    // r1 bottom edge past r2 top
		return true;
	}
	return false;
}

void Game::updatePlayer(double timeElapsed) {
	m_player.update(timeElapsed);
	// 플레이어 벽 넘어 이동 금지 설정
	// 위 경계
	if (m_player.getHitbox().top <= m_playingArea.top)
		m_player.setPosition({ m_player.getPosition().x, (double)m_playingArea.top });
	// 아래 경계
	if (m_player.getHitbox().bottom >= m_playingArea.bottom)
		m_player.setPosition({ m_player.getPosition().x, (double)m_playingArea.bottom - m_player.getPlayerSize().cy});
	// 왼쪽 경계
	if (m_player.getHitbox().left <= m_playingArea.left)
		m_player.setPosition({ (double)m_playingArea.left, m_player.getPosition().y });
	// 오른쪽 경계
	if (m_player.getHitbox().right >= m_playingArea.right) 
		m_player.setPosition({ (double)m_playingArea.right - m_player.getPlayerSize().cx, m_player.getPosition().y});
}

int Game::getGameStatus() {
	// 시작 안했으면 0, 동작중은 1, 종료시 2 리턴
	if (!m_gameStarted)
		return 0;
	else
		return m_gameEnded + 1;
}

void Game::update() {
	// 시작하지않았거나, 게임이 끝났다면 업데이트 하지 않는다.
	if (!m_gameStarted || m_gameEnded)
		return;

	auto currentTime = std::chrono::high_resolution_clock::now();
	double timeElapsed{ std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - m_previousUpdateTime).count() / 1000.0 };
	m_previousUpdateTime = currentTime;

	if ((m_gameScore.count()/10) / 200 >m_nowBulletCount-5 && m_nowBulletCount < m_maxBulletCount) {
		bulletList[++m_nowBulletCount] = *generateBullet();
	}

	// 모든 객체들 업데이트하기
	updatePlayer(timeElapsed);
	for (int i = 1; i <= m_nowBulletCount; i++) 
		updateBullet(timeElapsed, bulletList[i]);
	

}

void Game::start() {

	// 게임 상태 업데이트
	m_gameEnded = false;
	m_gameStarted = true;
	// 생성할 총알 갯수 설정
	m_nowBulletCount = 5;

	srand((unsigned int)(time(NULL)));
	bulletList = new BulletRed[m_maxBulletCount+1];
	for (int i = 1; i <= m_nowBulletCount; i++) {
		bulletList[i] = *generateBullet();
	}
	m_previousUpdateTime = std::chrono::high_resolution_clock::now();
	m_gameStartTime = m_previousUpdateTime;
}

void Game::end(){
	m_gameEnded = true;
	m_endScore = m_gameScore.count() / 10;
}

BulletRed* Game::generateBullet() {
	double speed = rand() % 150 + m_initBulletSpeed;
	double randPosX{ (double)(rand() % 1020) }, randPosY{ (double)(rand() % 885) };
	int loc = rand() % 4;
	switch (loc)
	{
	case 0:
		randPosX = 0.0;
		break;
	case 1:
		randPosY = 0.0;
		break;
	case 2:
		randPosX = m_playingArea.right-1;
		break;
	case 3:
		randPosY = m_playingArea.bottom-1;
		break;
	}
	return new BulletRed{ _T("BulletRed.png") ,CSize{14,14}, Position{randPosX,randPosY},
			Direction{m_playingArea.CenterPoint().x - (double)randPosX ,m_playingArea.CenterPoint().y - (double)randPosY },
			speed };
}



void Game::DirKeyDown(KeyDirection key) {
	
	Direction nextDir= m_player.getDirection();
	switch (key)
	{
	case KeyDirection::ArrowUp:
		nextDir.y > -1 ? nextDir.y -= 1 : nextDir.y = -1;
		break;
	case KeyDirection::ArrowDown:
		nextDir.y < 1 ? nextDir.y += 1 : nextDir.y = 1;
		break;
	case KeyDirection::ArrowRight:
		nextDir.x < 1 ? nextDir.x += 1 : nextDir.x = 1;
		break;
	case KeyDirection::ArrowLeft:
		nextDir.x > -1 ? nextDir.x -= 1 : nextDir.x = -1;
		break;
	}
	m_player.setDirection({ nextDir.x, nextDir.y });
}

void Game::DirKeyUp(KeyDirection key) {

	Direction nextDir = m_player.getDirection();
	switch (key)
	{
	case KeyDirection::ArrowUp:
		nextDir.y < 1 ? nextDir.y += 1 : nextDir.y = 1;
		break;
	case KeyDirection::ArrowDown:
		nextDir.y > -1 ? nextDir.y -= 1 : nextDir.y = -1;
		break;
	case KeyDirection::ArrowRight:
		nextDir.x > -1 ? nextDir.x -= 1 : nextDir.x = -1;
		break;
	case KeyDirection::ArrowLeft:
		nextDir.x < 1 ? nextDir.x += 1 : nextDir.x = 1;
		break;
	}
	m_player.setDirection({ nextDir.x, nextDir.y });
}