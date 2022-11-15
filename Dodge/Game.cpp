#include "pch.h"
#include "Game.h"

void Game::render(FlickerFreeDC& dc) {
	Gdiplus::Graphics gr(dc);
	if (!m_gameStarted) {
		// 게임 시작, 초기 실행화면 설정해주기
		dc.TextOut(m_playingArea.Width() / 2, m_playingArea.Height() / 4 * 3, _T("게임 시작을 위해 스페이스바를 눌러주세요"));
		return;
	}
	CString tempDir;
	tempDir.Format(_T("x : %lf, y : %lf"), m_player.getDirection().x, m_player.getDirection().y);
	dc.TextOut(100, 50, tempDir);
	// 플레이어 및 객체 렌더링
	m_player.render(dc);
	m_RedBullet1.render(dc);
	// m_RedBullet2.render(dc);
	// m_RedBullet3.render(dc);
	
	//플레이어 히트박스 확인
	auto hitbox{ m_player.getHitbox() };
	Pen pen4Hitbox(Color(255, 255, 255, 0), 2);
	gr.DrawRectangle(&pen4Hitbox, hitbox.left, hitbox.top, hitbox.Width(), hitbox.Height());
}

void Game::updateBullet(double timeElapsed,BulletRed& bulletRed) {

	// 총알이 벽에 부딛히거나, 벽을 통과하고자 할때인지 확인
	// 만일 충돌했다면, 총알의 방향을 바꾸고(튕기기) 공 움직임을 제어한다
	bulletRed.update(timeElapsed);
	Position BulletPos = bulletRed.getPosition();
	CRect PlayerPos = m_player.getHitbox();
	// 벽에 닿았다면, 플레이어를 향해 이동
	if (BulletPos.y + bulletRed.getBulletSize().cy >= m_playingArea.bottom ||
		BulletPos.y <= m_playingArea.top ||
		BulletPos.x + bulletRed.getBulletSize().cx >= m_playingArea.right ||
		BulletPos.x <= m_playingArea.left
		) {
		// auto oldDirection{ bulletRed.getDirection() };
		bulletRed.setDirection(Direction{ PlayerPos.CenterPoint().x-BulletPos.x, PlayerPos.CenterPoint().y - BulletPos.y });
	}
	// 공 움직이기
	bulletRed.update(timeElapsed);
}
void Game::updatePlayer(double timeElapsed) {
	m_player.update(timeElapsed);
	// 벽 넘어 이동 금지
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
	// 플레이어 접촉 확인 부분
	// auto hitbox{ m_player.getHitbox() };
	// if(플레이어 접촉)
	// {m_game.setGameStatus, 게임 종료 설정해주고, 버틴 점수 띄워주기}

}

int Game::getGameStatus() {
	// 시작 안했으면 0, 동작중은 1, 종료시 2 리턴
	if (!m_gameStarted)
		return 0;
	else
		return m_gameEnded + 1;
}

void Game::update() {
	if (!m_gameStarted || m_gameEnded)
		return;
	auto currentTime = std::chrono::high_resolution_clock::now();
	double timeElapsed{ std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - m_previousUpdateTime).count() / 1000.0 };
	m_previousUpdateTime = currentTime;

	// 모든 객체들 업데이트하기
	updateBullet(timeElapsed, m_RedBullet1);
	// updateBullet(timeElapsed, m_RedBullet2);
	// updateBullet(timeElapsed, m_RedBullet3);
	updatePlayer(timeElapsed);

}

void Game::start() {
	srand((unsigned int)(time(NULL)));
	m_gameStarted = true;
	m_previousUpdateTime = std::chrono::high_resolution_clock::now();
	// 총알 생성도 여기서 할까요?
}

void Game::end(){
	m_gameEnded = true;
}

// void Game::makeRedBulletGroup(){}

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