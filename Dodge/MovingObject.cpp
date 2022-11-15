/*
* 탄막 움직임 제어 코드 / 클래스 구현
*/
#include<cassert>
#include "pch.h"
#include "MovingObject.h"

MovingObject::MovingObject(const Position position, const Direction direction, const double speed) :
	m_position{ position },  m_speed{speed}
{
	// 움직임이 없으면, 에러메세지를 출력한다. 방향벡터가 0이면 안되는 상황이라서
	/*
	if ((!(int)m_direction.x) || (!(int)m_direction.y)) {
		assert(false);
	}
	*/

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
	// 벡터 방향 설정
	// 단위벡터 방향 결정은 update에서 진행한다. 효율성은 떨어지긴 하지만.. 계산이 중첩되어 값이 왜곡됨을 막기 위함
	m_direction = newDirection;
}