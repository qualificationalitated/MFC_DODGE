#include "pch.h"
#include "Game.h"

void Game::render(FlickerFreeDC& dc) {
	if (!m_gameStarted)
		return;
	m_RedBullet.render(dc);
	m_RedBullet2.render(dc);
	m_RedBullet3.render(dc);
	//draw Play Area
	Gdiplus::Graphics gr(dc);
	Pen pen(Color(255, 0, 0, 255), 10);
	gr.DrawRectangle(&pen, m_playingArea.top, m_playingArea.left, 
		m_playingArea.Width(), m_playingArea.Height());
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

	// 공 움직이기
	bulletRed.update(timeElapsed);
}

void Game::update() {
	if (!m_gameStarted)
		return;
	auto currentTime = std::chrono::high_resolution_clock::now();
	double timeElapsed{ std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - m_previousUpdateTime).count() / 1000.0 };
	m_previousUpdateTime = currentTime;
	Position newPosition, oldPosition{m_RedBullet.getPosition()};

	updateBullet(timeElapsed,m_RedBullet);
	updateBullet(timeElapsed, m_RedBullet2);
	updateBullet(timeElapsed, m_RedBullet3);

}

void Game::start() {
	m_gameStarted = true;
	m_previousUpdateTime = std::chrono::high_resolution_clock::now();
	m_RedBullet.setDirection({ 1,1 });
}