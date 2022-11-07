#include "pch.h"
#include "Game.h"

void Game::render(FlickerFreeDC& dc) {
	if (!m_gameStarted)
		return;
	Gdiplus::Graphics gr(dc);
	Pen pen(Color(255, 255, 255, 0), 10);
	SolidBrush blackBrush(Color(255, 20, 60, 170));
	//draw Play Area
	gr.FillRectangle(&blackBrush, 0, m_playingArea.left, m_playingArea.Width(), m_playingArea.Height());
	gr.DrawRectangle(&pen, m_playingArea.top, m_playingArea.left,
		m_playingArea.Width(), m_playingArea.Height());
	// 플레이어 및 객체 렌더링
	m_player.render(dc);
	m_RedBullet1.render(dc);
	m_RedBullet2.render(dc);
	m_RedBullet3.render(dc);
	
	//플레이어 히트박스 확인
	auto hitbox{ m_player.getHitbox() };
	Pen pen4Hitbox(Color(255, 255, 255, 0), 2);
	gr.DrawRectangle(&pen4Hitbox, hitbox.left, hitbox.top, hitbox.Width(), hitbox.Height());

	
	
}

void Game::updateBullet(double timeElapsed,BulletRed& bulletRed) {

	// 총알이 벽에 부딛히거나, 벽을 통과하고자 할때인지 확인
	// 만일 충돌했다면, 총알의 방향을 바꾸고(튕기기) 공 움직임을 제어한다
	bulletRed.update(timeElapsed);
	// 바닥면 & 윗면
	if (bulletRed.getPosition().y + bulletRed.getBulletSize().cy >= m_playingArea.bottom ||
		bulletRed.getPosition().y <= m_playingArea.top) {
		auto oldDirection{ bulletRed.getDirection() };
		bulletRed.setDirection(Direction{ oldDirection.x,-oldDirection.y });
		// m_RedBullet.setPosition(oldPosition);
		// m_RedBullet.update(timeElapsed);
	}
	// 오른쪽면
	if (bulletRed.getPosition().x + bulletRed.getBulletSize().cx >= m_playingArea.right ||
		bulletRed.getPosition().x <= m_playingArea.left) {
		auto oldDirection{ bulletRed.getDirection() };
		bulletRed.setDirection(Direction{ -oldDirection.x,oldDirection.y });
		// m_RedBullet.setPosition(oldPosition);
		// m_RedBullet.update(timeElapsed);
	}

	// 플레이어 접촉 확인 부분
	// auto hitbox{ m_player.getHitbox() };
	// if(플레이어 접촉)
	// {게임 종료 및 게임 종료 메세지 출력}


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
	
}

void Game::update() {
	if (!m_gameStarted)
		return;
	auto currentTime = std::chrono::high_resolution_clock::now();
	double timeElapsed{ std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - m_previousUpdateTime).count() / 1000.0 };
	m_previousUpdateTime = currentTime;

	// 모든 객체들 업데이트하기
	updateBullet(timeElapsed, m_RedBullet1);
	updateBullet(timeElapsed, m_RedBullet2);
	updateBullet(timeElapsed, m_RedBullet3);
	updatePlayer(timeElapsed);

}

void Game::start() {
	m_gameStarted = true;
	m_previousUpdateTime = std::chrono::high_resolution_clock::now();
}

void Game::DirKeyDown(KeyDirection key) {
	
	const double speed{ 250.0 };
	switch (key)
	{
	case KeyDirection::ArrowUp:
		m_player.setDirection({ 0.0 ,-1.0 });
		m_player.setSpeed(speed);
		break;
	case KeyDirection::ArrowDown:
		m_player.setDirection({ 0.0, 1.0 });
		m_player.setSpeed(speed);
		break;
	case KeyDirection::ArrowRight:
		m_player.setDirection({ 1.0, 0.0 });
		m_player.setSpeed(speed);
		break;
	case KeyDirection::ArrowLeft:
		m_player.setDirection({ -1.0, 0.0 });
		m_player.setSpeed(speed);
		break;
	}
}

void Game::DirKeyUp(KeyDirection key) {

	const double speed{ 250.0 };
	switch (key)
	{
	default:
		m_player.setSpeed(0.0);
		break;

		/*
	case KeyDirection::ArrowUp:
		m_player.setSpeed(0.0);
		break;
	case KeyDirection::ArrowDown:
		m_player.setSpeed(0.0);
		break;
	case KeyDirection::ArrowRight:
		m_player.setSpeed(0.0);
		break;
	case KeyDirection::ArrowLeft:
		m_player.setSpeed(0.0);
		break;
		*/
	}
}