#include "pch.h"
#include "Player.h"

Player::Player(const CString& imageFileName, const CSize playerSize,
	const Position position, const Direction direction, const double speed) :
	MovingObject{ position,direction,speed }, m_PlayerSize{ playerSize }
{
	// �̹��� �ε��ϱ�
	m_ImagePlayer.Load(imageFileName);
}

void Player::update(const double elapsedTime, Position* onlyCalc) {
	MovingObject::update(elapsedTime, onlyCalc);
	
}

// �ø�Ŀ���� ���ŵ� ������ ������Ʈ ������ ���� �����Լ� ����
void Player::render(FlickerFreeDC& dc) {
	// ������ ������Ʈ�� ��ġ�� �޾ƿ��� �Լ�
	const auto& position{ getPosition() };
	// �ش� ��ġ�� �׸��� �׷��ش�
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