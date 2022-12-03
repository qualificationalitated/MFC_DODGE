#include "pch.h"
#include "Game.h"

void Game::render(FlickerFreeDC& dc) {
	Gdiplus::Graphics gr(dc);
	CFont font_300, font_200; // ȭ�� ��������� ���� ��Ʈ
	font_300.CreatePointFont(300, _T("�ü�"));
	font_200.CreatePointFont(200, _T("�ü�"));

	// ������ �������� �ʾҴٸ�, ���� ���� �� �ʱ�ȭ�� ���
	if (!m_gameStarted) {
		// ������Ʈ ����� ���� ������ ���
		dc.SelectObject(&font_300);
		SetTextAlign(dc, TA_CENTER);
		dc.TextOut(m_playingArea.CenterPoint().x, m_playingArea.CenterPoint().y - 120, _T("MFC_Dodge"));
		dc.SelectObject(&font_200);
		dc.TextOut(m_playingArea.CenterPoint().x, m_playingArea.CenterPoint().y+120, _T("���� ������ ���� �����̽��ٸ� �����ּ���"));
		return;
	}
	
	// ������ �������̶��, ���� ����� ȭ�� ���
	if (m_gameStarted && !m_gameEnded) {
		// �÷��� ȭ��� ����, �Ѿ� ���� ���
		CString tempDir, tempTime; // ���� ��� �� �������� ���� �ð� �Լ�
		m_gameScore = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - m_gameStartTime); // ����
		
		// ���� �ð��� Ȱ���� ���ھ� ���
		tempTime.Format(_T("score : %d"), (int)m_gameScore.count() / 10);
		dc.TextOut(30, 30, tempTime);
		
		// ȭ�鿡 �����ִ� �Ѿ� ����
		tempTime.Format(_T("Bullet Count : %d"), m_nowBulletCount);
		dc.TextOut(30, 50, tempTime);
		
		// �÷��̾� �� ź�� ������
		m_player.render(dc);
		for (int i = 1; i <= m_nowBulletCount; i++) 
			bulletList[i].render(dc);
	}
	
	// �÷��̾� �ǰ����� ������ ����Ǹ�, ����ȭ�� ���
	else if (m_gameEnded) {
		// ���� ����� �ȳ�, �÷��̾� ������ ���
		CString scoreText; // ���� ����� ���� ��Ʈ��
		dc.SelectObject(&font_200);
		SetTextAlign(dc, TA_CENTER);
		dc.TextOut(m_playingArea.CenterPoint().x , m_playingArea.CenterPoint().y-90, _T("��������!"));
		scoreText.Format(_T("�������� : %d"), m_endScore);
		dc.TextOut(m_playingArea.CenterPoint().x, m_playingArea.CenterPoint().y-120, scoreText);
		dc.TextOut(m_playingArea.CenterPoint().x, m_playingArea.CenterPoint().y + 60, _T("�����̽� �ٸ� ������ ������մϴ�"));
	}
}

void Game::updateBullet(double timeElapsed,BulletRed& bulletRed) {

	// �Ѿ��� ���� �ε����ų�, ���� ����ϰ��� �Ҷ����� Ȯ��
	// ���� �浹�ߴٸ�, �Ѿ��� ������ �ٲٰ�(ƨ���) �� �������� �����Ѵ�
	bulletRed.update(timeElapsed);
	Position BulletPos = bulletRed.getPosition(); // ������Ʈ �� �Ѿ� ��ġ
	CSize BulletSize = bulletRed.getBulletSize(); // ������Ʈ �� �Ѿ� ũ��
	CRect PlayerPos = m_player.getHitbox(); // �÷��̾� ��Ʈ�ڽ�
	double speed = bulletRed.getSpeed(); // ���� �Ѿ��� �ӵ�

	// ���� ��Ҵٸ�, �÷��̾ ���� �̵� & ���ο� �ӵ��� �����ϰ� �ο�
	if (BulletPos.y + BulletSize.cy >= m_playingArea.bottom ||
		BulletPos.y <= m_playingArea.top ||
		BulletPos.x + BulletSize.cx >= m_playingArea.right ||
		BulletPos.x <= m_playingArea.left
		) 
	{
		// �ӵ��� �Ӱ�ġ�� ���� �ʾҴٸ�, �Ѿ� �⺻�ӵ��� ������ŵ�ϴ�.
		m_initBulletSpeed < 200 ? m_initBulletSpeed += 0.5 : m_initBulletSpeed = m_initBulletSpeed;
		speed = rand() % 150 + m_initBulletSpeed;
		// �Ѿ��� �̵� ������ �÷��̾������� ����
		bulletRed.setDirection(Direction{ PlayerPos.CenterPoint().x-BulletPos.x, PlayerPos.CenterPoint().y - BulletPos.y });
	}
	// �浹��, ������ ���Ḧ ���� �Լ� ȣ��
	if (playerCollisionDetection(PlayerPos.left, PlayerPos.top, PlayerPos.Width(), PlayerPos.Height(), BulletPos.x, BulletPos.y, BulletSize.cx, BulletSize.cy)) {
		end();
	}

	// �� �����̱�
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
	// �÷��̾� �� �Ѿ� �̵� ���� ����
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
}

int Game::getGameStatus() {
	// ���� �������� 0, �������� 1, ����� 2 ����
	if (!m_gameStarted)
		return 0;
	else
		return m_gameEnded + 1;
}

void Game::update() {
	// ���������ʾҰų�, ������ �����ٸ� ������Ʈ ���� �ʴ´�.
	if (!m_gameStarted || m_gameEnded)
		return;

	auto currentTime = std::chrono::high_resolution_clock::now(); // ������Ʈ�� ȣ��� ������ �ð�
	double timeElapsed{ std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - m_previousUpdateTime).count() / 1000.0 }; // ���� �ð��� ���� �ð��� ����
	m_previousUpdateTime = currentTime; // ���� �ð��� �ֽ�ȭ�Ѵ�

	// ���� �Ѿ� ������ �Ӱ�ġ�� ���� �ʾҴٸ�, �߰� �Ѿ��� �迭�� �ְ� ȭ�鿡 ����Ѵ�.
	if ((m_gameScore.count()/10) / 200 >m_nowBulletCount-5 && m_nowBulletCount < m_maxBulletCount) {
		bulletList[++m_nowBulletCount] = *generateBullet();
	}

	// �ð� ���� �������� ��� ��ü�� ������Ʈ�ϱ�
	updatePlayer(timeElapsed);
	for (int i = 1; i <= m_nowBulletCount; i++) 
		updateBullet(timeElapsed, bulletList[i]);
	

}

void Game::start() {

	// ���� ���� ������Ʈ
	m_gameEnded = false;
	m_gameStarted = true;
	m_nowBulletCount = 5; // �ʱ� �Ѿ� ���� ����

	// ������ ��ġ�� �ӵ� �ο��� ���� ������ ����
	srand((unsigned int)(time(NULL)));
	bulletList = new BulletRed[m_maxBulletCount+1]; // �Ѿ��� ���� ������ ��ü�迭

	// �÷��̾� ��ġ �ʱ�ȭ
	m_player.setPosition(Position{ m_playingArea.right / 2.0,m_playingArea.bottom / 2.0 });
	
	// �ʱ� �Ѿ˰�����ŭ �Ѿ� ����
	for (int i = 1; i <= m_nowBulletCount; i++) {
		bulletList[i] = *generateBullet();
	}
	// ������Ʈ�� ���� �ð� ����
	m_previousUpdateTime = std::chrono::high_resolution_clock::now();
	m_gameStartTime = m_previousUpdateTime;
}

void Game::end(){
	// ���� ����� ���� ������Ʈ
	m_gameEnded = true;
	m_endScore = m_gameScore.count() / 10;
}
void Game::setInitialGame() {
	// ���� �ʱ���·� �ǵ�����
	m_gameStarted = 0;
}

BulletRed* Game::generateBullet() {
	double speed = rand() % 150 + m_initBulletSpeed; // ������ �ӵ� ����
	double randPosX{ (double)(rand() % 1020) }, randPosY{ (double)(rand() % 885) }; // ������ ������ġ ����
	int loc = rand() % 4; // �Ѿ��� ������� �� ��ġ ����
	
	// ���� ���� �Ѿ��� ���� ����
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

	// �Ѿ� ��ü�� �����ؼ� �� �ּҸ� ��ȯ
	return new BulletRed{ _T("BulletRed.png") ,CSize{14,14}, Position{randPosX,randPosY},
			Direction{m_playingArea.CenterPoint().x - (double)randPosX ,m_playingArea.CenterPoint().y - (double)randPosY },
			speed };
}

void Game::DirKeyDown(KeyDirection key) {	
	Direction nextDir= m_player.getDirection(); // ���� Ű ���¸� �޾ƿ���
	// ���� Ű ���� ���� ������ ���� ����
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
	// �÷��̾� �̵� ���� ����
	m_player.setDirection({ nextDir.x, nextDir.y });
}

void Game::DirKeyUp(KeyDirection key) {
	Direction nextDir = m_player.getDirection(); // ���� Ű ���¸� �޾ƿ���
	// ���� Ű ���� ���� �����̴� ���������� ���Ͱ� ����
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
	// �÷��̾� �̵� ���� ����
	m_player.setDirection({ nextDir.x, nextDir.y });
}