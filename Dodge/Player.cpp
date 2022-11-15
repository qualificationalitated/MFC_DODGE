#include "pch.h"
#include "Player.h"

Player::Player(const CString& imageFileName, const CSize playerSize,
	const Position position, const Direction direction, const double speed) :
	MovingObject{ position,direction,speed }, m_PlayerSize{ playerSize }
{
	// 이미지 로드하기
	m_ImagePlayer.Load(imageFileName);
}

void Player::update(const double elapsedTime, Position* onlyCalc) {
	MovingObject::update(elapsedTime, onlyCalc);
	
}

// 플리커링이 제거된 형태의 오브젝트 구현을 위한 렌더함수 구현
void Player::render(FlickerFreeDC& dc) {
	// 생성할 오브젝트의 위치를 받아오는 함수
	const auto& position{ getPosition() };
	// 해당 위치에 그림을 그려준다
	m_ImagePlayer.Draw(dc.GetSafeHdc(), position.x, position.y, m_PlayerSize.cx, m_PlayerSize.cy);
}

CRect Player::getHitbox() {
	CRect hitbox;
	hitbox.left = getPosition().x + getPlayerSize().cx * 0.17;
	hitbox.right = getPosition().x + getPlayerSize().cx * 0.83;
	hitbox.top = getPosition().y + getPlayerSize().cy * 0.2;
	hitbox.bottom = getPosition().y + getPlayerSize().cy * 0.8;
	return hitbox;
}