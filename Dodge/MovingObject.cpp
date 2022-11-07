/*
* 탄막 움직임 제어 코드 / 클래스 구현
*/
#include<cassert>
#include "pch.h"
#include "MovingObject.h"

MovingObject::MovingObject(const Position position, const Direction direction, const double speed) :
	m_position{ position },  m_speed{speed}
{
	// 움직임이 없으면 뭔가 한다
	if ((!(int)m_direction.x) || (!(int)m_direction.y)) {
		assert(false);
	}
	setDirection(direction);
}

void MovingObject::update(const double elapsedTime,Position* onlyCalc) {
	const double distance = m_speed * elapsedTime;
	if (onlyCalc) {
		onlyCalc->x = m_position.x + m_direction.x * distance;
		onlyCalc->y = m_position.y + m_direction.y * distance;
	}
	// 이동할 새 위치 = 기존 위치 + 단위벡터*거리
	m_position.x = m_position.x + m_direction.x * distance;
	m_position.y = m_position.y + m_direction.y * distance;
}

void MovingObject::setDirection(const Direction& newDirection) {
	// 단위벡터 생성, 탄막의 방향 결정
	// 0,0 넣으면 터짐, 0벡터가 안되는거 수정 예정
	const double newVectorLength = sqrt(pow(newDirection.x, 2) + pow(newDirection.y, 2));
	m_direction.x = newDirection.x / newVectorLength;
	m_direction.y = newDirection.y / newVectorLength;
}