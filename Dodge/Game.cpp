#include "pch.h"
#include "Game.h"

void Game::render(FlickerFreeDC& dc) {
	Gdiplus::Graphics gr(dc);
	CFont font_300, font_200; // 화면 글자출력을 위한 폰트
	font_300.CreatePointFont(300, _T("궁서"));
	font_200.CreatePointFont(200, _T("궁서"));

	// 게임이 시작하지 않았다면, 게임 시작 전 초기화면 출력
	if (!m_gameStarted) {
		// 프로젝트 제목과 게임 설명을 출력
		dc.SelectObject(&font_300);
		SetTextAlign(dc, TA_CENTER);
		dc.TextOut(m_playingArea.CenterPoint().x, m_playingArea.CenterPoint().y - 120, _T("MFC_Dodge"));
		dc.SelectObject(&font_200);
		dc.TextOut(m_playingArea.CenterPoint().x, m_playingArea.CenterPoint().y+120, _T("게임 시작을 위해 스페이스바를 눌러주세요"));
		return;
	}
	
	// 게임이 진행중이라면, 게임 진행용 화면 출력
	if (m_gameStarted && !m_gameEnded) {
		// 플레이 화면과 점수, 총알 갯수 출력
		CString tempDir, tempTime; // 점수 계산 및 움직임을 위한 시간 함수
		m_gameScore = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - m_gameStartTime); // 점수
		
		// 생존 시간을 활용한 스코어 계산
		tempTime.Format(_T("score : %d"), (int)m_gameScore.count() / 10);
		dc.TextOut(30, 30, tempTime);
		
		// 화면에 나와있는 총알 갯수
		tempTime.Format(_T("Bullet Count : %d"), m_nowBulletCount);
		dc.TextOut(30, 50, tempTime);
		
		// 플레이어 및 탄막 렌더링
		m_player.render(dc);
		for (int i = 1; i <= m_nowBulletCount; i++) 
			bulletList[i].render(dc);
	}
	
	// 플레이어 피격으로 게임이 종료되면, 종료화면 출력
	else if (m_gameEnded) {
		// 게임 재시작 안내, 플레이어 점수를 출력
		CString scoreText; // 점수 출력을 위한 스트링
		dc.SelectObject(&font_200);
		SetTextAlign(dc, TA_CENTER);
		dc.TextOut(m_playingArea.CenterPoint().x , m_playingArea.CenterPoint().y-90, _T("게임종료!"));
		scoreText.Format(_T("최종점수 : %d"), m_endScore);
		dc.TextOut(m_playingArea.CenterPoint().x, m_playingArea.CenterPoint().y-120, scoreText);
		dc.TextOut(m_playingArea.CenterPoint().x, m_playingArea.CenterPoint().y + 60, _T("스페이스 바를 누르면 재시작합니다"));
	}
}

void Game::updateBullet(double timeElapsed,BulletRed& bulletRed) {

	// 총알이 벽에 부딛히거나, 벽을 통과하고자 할때인지 확인
	// 만일 충돌했다면, 총알의 방향을 바꾸고(튕기기) 공 움직임을 제어한다
	bulletRed.update(timeElapsed);
	Position BulletPos = bulletRed.getPosition(); // 업데이트 할 총알 위치
	CSize BulletSize = bulletRed.getBulletSize(); // 업데이트 할 총알 크기
	CRect PlayerPos = m_player.getHitbox(); // 플레이어 히트박스
	double speed = bulletRed.getSpeed(); // 현제 총알의 속도

	// 벽에 닿았다면, 플레이어를 향해 이동 & 새로운 속도가 랜덤하게 부여
	if (BulletPos.y + BulletSize.cy >= m_playingArea.bottom ||
		BulletPos.y <= m_playingArea.top ||
		BulletPos.x + BulletSize.cx >= m_playingArea.right ||
		BulletPos.x <= m_playingArea.left
		) 
	{
		// 속도가 임계치를 넘지 않았다면, 총알 기본속도를 증가시킵니다.
		m_initBulletSpeed < 200 ? m_initBulletSpeed += 0.5 : m_initBulletSpeed = m_initBulletSpeed;
		speed = rand() % 150 + m_initBulletSpeed;
		// 총알의 이동 각도를 플레이어쪽으로 설정
		bulletRed.setDirection(Direction{ PlayerPos.CenterPoint().x-BulletPos.x, PlayerPos.CenterPoint().y - BulletPos.y });
	}
	// 충돌시, 게임을 종료를 위한 함수 호출
	if (playerCollisionDetection(PlayerPos.left, PlayerPos.top, PlayerPos.Width(), PlayerPos.Height(), BulletPos.x, BulletPos.y, BulletSize.cx, BulletSize.cy)) {
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

	auto currentTime = std::chrono::high_resolution_clock::now(); // 업데이트가 호출된 지금의 시간
	double timeElapsed{ std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - m_previousUpdateTime).count() / 1000.0 }; // 이전 시간과 현제 시간의 차이
	m_previousUpdateTime = currentTime; // 이전 시간을 최신화한다

	// 만일 총알 갯수가 임계치를 넘지 않았다면, 추가 총알을 배열에 넣고 화면에 출력한다.
	if ((m_gameScore.count()/10) / 200 >m_nowBulletCount-5 && m_nowBulletCount < m_maxBulletCount) {
		bulletList[++m_nowBulletCount] = *generateBullet();
	}

	// 시간 값을 바탕으로 모든 객체들 업데이트하기
	updatePlayer(timeElapsed);
	for (int i = 1; i <= m_nowBulletCount; i++) 
		updateBullet(timeElapsed, bulletList[i]);
	

}

void Game::start() {

	// 게임 상태 업데이트
	m_gameEnded = false;
	m_gameStarted = true;
	m_nowBulletCount = 5; // 초기 총알 갯수 설정

	// 랜덤한 위치와 속도 부여를 위한 랜덤값 설정
	srand((unsigned int)(time(NULL)));
	bulletList = new BulletRed[m_maxBulletCount+1]; // 총알을 동적 생성할 객체배열

	// 플레이어 위치 초기화
	m_player.setPosition(Position{ m_playingArea.right / 2.0,m_playingArea.bottom / 2.0 });
	
	// 초기 총알갯수만큼 총알 생성
	for (int i = 1; i <= m_nowBulletCount; i++) {
		bulletList[i] = *generateBullet();
	}
	// 업데이트된 이전 시간 갱신
	m_previousUpdateTime = std::chrono::high_resolution_clock::now();
	m_gameStartTime = m_previousUpdateTime;
}

void Game::end(){
	// 게임 종료로 상태 업데이트
	m_gameEnded = true;
	m_endScore = m_gameScore.count() / 10;
}
void Game::setInitialGame() {
	// 게임 초기상태로 되돌리기
	m_gameStarted = 0;
}

BulletRed* Game::generateBullet() {
	double speed = rand() % 150 + m_initBulletSpeed; // 랜덤한 속도 결정
	double randPosX{ (double)(rand() % 1020) }, randPosY{ (double)(rand() % 885) }; // 랜덤한 생성위치 결정
	int loc = rand() % 4; // 총알이 만들어질 벽 위치 결정
	
	// 값에 따라 총알을 벽에 붙임
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

	// 총알 객체를 생성해서 그 주소를 반환
	return new BulletRed{ _T("BulletRed.png") ,CSize{14,14}, Position{randPosX,randPosY},
			Direction{m_playingArea.CenterPoint().x - (double)randPosX ,m_playingArea.CenterPoint().y - (double)randPosY },
			speed };
}

void Game::DirKeyDown(KeyDirection key) {	
	Direction nextDir= m_player.getDirection(); // 눌린 키 상태를 받아오기
	// 눌린 키 값에 따라 움직일 방향 제어
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
	// 플레이어 이동 방향 결정
	m_player.setDirection({ nextDir.x, nextDir.y });
}

void Game::DirKeyUp(KeyDirection key) {
	Direction nextDir = m_player.getDirection(); // 떼진 키 상태를 받아오기
	// 떼진 키 값에 따라 움직이던 방향으로의 벡터값 제거
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
	// 플레이어 이동 방향 결정
	m_player.setDirection({ nextDir.x, nextDir.y });
}