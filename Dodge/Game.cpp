#include "pch.h"
#include "Game.h"

void Game::render(FlickerFreeDC& dc) {
	Gdiplus::Graphics gr(dc);
	if (!m_gameStarted) {
		// ���� ����, �ʱ� ����ȭ�� �������ֱ�
		dc.TextOut(m_playingArea.Width() / 2, m_playingArea.Height() / 4 * 3, _T("���� ������ ���� �����̽��ٸ� �����ּ���"));
		return;
	}
	CString tempDir, tempTime;
	m_gameScore = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - m_gameStartTime);

	// �÷��̾� �̵� ���� ǥ��
	tempDir.Format(_T("Dir x : %d, Dir y : %d"), (int)m_player.getDirection().x, (int)m_player.getDirection().y);
	dc.TextOut(100, 50, tempDir);

	// ���� �ð��� Ȱ���� ���ھ� ���
	tempTime.Format(_T("score : %d"), m_gameScore /10);
	dc.TextOut(100, 100, tempTime);

	// �÷��̾� �� ��ü ������
	m_player.render(dc);
	
	for(int i=0; i < 5; i++){
		bulletList[i]->render(dc);
	}
	// -------------------------------------------------------------------------------------------------------------------------------------------- -
	/*
	m_RedBullet1.render(dc);
	m_RedBullet2.render(dc);
	m_RedBullet3.render(dc);
	*/
	
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
	CSize BulletSize = bulletRed.getBulletSize();
	CRect PlayerPos = m_player.getHitbox();
	
	// ���� ��Ҵٸ�, �÷��̾ ���� �̵�
	if (BulletPos.y + BulletSize.cy >= m_playingArea.bottom ||
		BulletPos.y <= m_playingArea.top ||
		BulletPos.x + BulletSize.cx >= m_playingArea.right ||
		BulletPos.x <= m_playingArea.left
		) {
		// auto oldDirection{ bulletRed.getDirection() };
		bulletRed.setDirection(Direction{ PlayerPos.CenterPoint().x-BulletPos.x, PlayerPos.CenterPoint().y - BulletPos.y });
	}
	if (playerCollisionDetection(PlayerPos.left, PlayerPos.top, PlayerPos.Width(), PlayerPos.Height(), BulletPos.x, BulletPos.y, BulletSize.cx, BulletSize.cy)) {
		// �浹, �浹�� ���� ������� �Ǹ� �ȴ�.
		end();
		bulletRed.setDirection(Direction{ -bulletRed.getDirection().x, -bulletRed.getDirection().y });
	}
	// �� �����̱�
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
	for (int i = 0; i < 5; i++) {
		updateBullet(timeElapsed, *bulletList[i]);
	}
	/*
	updateBullet(timeElapsed, m_RedBullet1);
	updateBullet(timeElapsed, m_RedBullet2);
	updateBullet(timeElapsed, m_RedBullet3);
	*/
	updatePlayer(timeElapsed);

}

void Game::start() {
	srand((unsigned int)(time(NULL)));
	m_gameEnded = false;
	m_gameStarted = true;
	srand(time(NULL));
	for (int i = 0; i < 5; i++) {
		double speed = rand() % 100 + 150.0;
		double randPosX{ (double)(rand() % 1020) }, randPosY{ (double)(rand() % 885) };
		bulletList[i] = new BulletRed{ _T("BulletRed.png") ,CSize{14,14}, 
			Position{randPosX,randPosY}, 
			Direction{m_playingArea.CenterPoint().x - (double)randPosX ,m_playingArea.CenterPoint().y - (double)randPosY },
			speed };
	}

	m_previousUpdateTime = std::chrono::high_resolution_clock::now();
	m_gameStartTime = std::chrono::high_resolution_clock::now();
}

void Game::end(){
	m_gameEnded = true; // ȭ�� ������ �ȴ���
	/* 
	* ���� ������ ����
	* 1. �÷��̾� �� ź�� ���� ����
	* 2. ȭ�� �߾ӿ� �÷��̾� ���� ǥ��
	* 3. �ٽ� �÷��� �Ͻ�? ���� ǥ��
	* 4. �ְ��� ǥ��(���� dino rush ����)
	* ���� ������ ã�ƺ���
	*/ 
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