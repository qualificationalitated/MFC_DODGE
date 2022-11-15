#include "pch.h"
#include "Game.h"

void Game::render(FlickerFreeDC& dc) {
	Gdiplus::Graphics gr(dc);
	if (!m_gameStarted) {
		// ���� ����, �ʱ� ����ȭ�� �������ֱ�
		dc.TextOut(m_playingArea.Width() / 2, m_playingArea.Height() / 4 * 3, _T("���� ������ ���� �����̽��ٸ� �����ּ���"));
		return;
	}
	CString tempDir;
	tempDir.Format(_T("x : %lf, y : %lf"), m_player.getDirection().x, m_player.getDirection().y);
	dc.TextOut(100, 50, tempDir);
	// �÷��̾� �� ��ü ������
	m_player.render(dc);
	m_RedBullet1.render(dc);
	// m_RedBullet2.render(dc);
	// m_RedBullet3.render(dc);
	
	//�÷��̾� ��Ʈ�ڽ� Ȯ��
	auto hitbox{ m_player.getHitbox() };
	Pen pen4Hitbox(Color(255, 255, 255, 0), 2);
	gr.DrawRectangle(&pen4Hitbox, hitbox.left, hitbox.top, hitbox.Width(), hitbox.Height());
}

void Game::updateBullet(double timeElapsed,BulletRed& bulletRed) {

	// �Ѿ��� ���� �ε����ų�, ���� ����ϰ��� �Ҷ����� Ȯ��
	// ���� �浹�ߴٸ�, �Ѿ��� ������ �ٲٰ�(ƨ���) �� �������� �����Ѵ�
	bulletRed.update(timeElapsed);
	Position BulletPos = bulletRed.getPosition();
	CRect PlayerPos = m_player.getHitbox();
	// ���� ��Ҵٸ�, �÷��̾ ���� �̵�
	if (BulletPos.y + bulletRed.getBulletSize().cy >= m_playingArea.bottom ||
		BulletPos.y <= m_playingArea.top ||
		BulletPos.x + bulletRed.getBulletSize().cx >= m_playingArea.right ||
		BulletPos.x <= m_playingArea.left
		) {
		// auto oldDirection{ bulletRed.getDirection() };
		bulletRed.setDirection(Direction{ PlayerPos.CenterPoint().x-BulletPos.x, PlayerPos.CenterPoint().y - BulletPos.y });
	}
	// �� �����̱�
	bulletRed.update(timeElapsed);
}
void Game::updatePlayer(double timeElapsed) {
	m_player.update(timeElapsed);
	// �� �Ѿ� �̵� ����
	// �� ���
	if (m_player.getHitbox().top <= m_playingArea.top)
		m_player.setPosition({ m_player.getPosition().x, (double)m_playingArea.top });
	// �Ʒ� ���
	if (m_player.getHitbox().bottom >= m_playingArea.bottom)
		m_player.setPosition({ m_player.getPosition().x, (double)m_playingArea.bottom - m_player.getPlayerSize().cy});
	// ���� ���
	if (m_player.getHitbox().left <= m_playingArea.left)
		m_player.setPosition({ (double)m_playingArea.left, m_player.getPosition().y });
	// ������ ���
	if (m_player.getHitbox().right >= m_playingArea.right) 
		m_player.setPosition({ (double)m_playingArea.right - m_player.getPlayerSize().cx, m_player.getPosition().y});
	// �÷��̾� ���� Ȯ�� �κ�
	// auto hitbox{ m_player.getHitbox() };
	// if(�÷��̾� ����)
	// {m_game.setGameStatus, ���� ���� �������ְ�, ��ƾ ���� ����ֱ�}

}

int Game::getGameStatus() {
	// ���� �������� 0, �������� 1, ����� 2 ����
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

	// ��� ��ü�� ������Ʈ�ϱ�
	updateBullet(timeElapsed, m_RedBullet1);
	// updateBullet(timeElapsed, m_RedBullet2);
	// updateBullet(timeElapsed, m_RedBullet3);
	updatePlayer(timeElapsed);

}

void Game::start() {
	srand((unsigned int)(time(NULL)));
	m_gameStarted = true;
	m_previousUpdateTime = std::chrono::high_resolution_clock::now();
	// �Ѿ� ������ ���⼭ �ұ��?
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