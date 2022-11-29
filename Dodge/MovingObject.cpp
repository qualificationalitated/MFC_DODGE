/*
* 탄막 움직임 제어 코드 / 클래스 구현
*/
#include<cassert>
#include "pch.h"
#include "MovingObject.h"

MovingObject::MovingObject(const Position position, const Direction direction, const double speed) :
	m_position{ position },  m_speed{speed}
{
	// 객체 이동방향 결정
	setDirection(direction);
}

void MovingObject::update(const double elapsedTime,Position* onlyCalc) {
	const double distance = m_speed * elapsedTime;
	if (onlyCalc) {
		onlyCalc->x = m_position.x + m_direction.x * distance;
		onlyCalc->y = m_position.y + m_direction.y * distance;
	}
	// 이동할 새 위치 = 기존 위치 + 단위벡터(방향)*거리
	const double vectorLength = sqrt(pow(m_direction.x, 2) + pow(m_direction.y, 2));
	if (m_direction.x || m_direction.y) {
		m_position.x = m_position.x + m_direction.x / vectorLength * distance;
		m_position.y = m_position.y + m_direction.y / vectorLength * distance;
	}
}

void MovingObject::setDirection(const Direction& newDirection) {
	// 방향 결정
	m_direction = newDirection;
}