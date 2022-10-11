/*
* ź�� ������ ���� �ڵ� / Ŭ���� ����
*/
#include<cassert>
#include "pch.h"
#include "MovingObject.h"

MovingObject::MovingObject(const Position position, const Direction direction, const double speed) :
	m_position{ position },  m_speed{speed}
{
	// �������� ������ ���� �Ѵ�
	if ((!(int)m_direction.x) || (!(int)m_direction.y)) {
		assert(false);
	}
	setDirection(direction);
}

void MovingObject::update(const double elapsedTime) {
	const double distance = m_speed * elapsedTime;
	// �̵��� �� ��ġ = ���� ��ġ + ��������*�Ÿ�
	m_position.x = m_position.x + m_direction.x * distance;
	m_position.y = m_position.y + m_direction.y * distance;
}

void MovingObject::setDirection(const Direction& newDirection) {
	// �������� ����, ź���� ���� ����
	const double newVectorLength = sqrt(pow(newDirection.x, 2) + pow(newDirection.y, 2));
	m_direction.x = newDirection.x / newVectorLength;
	m_direction.y = newDirection.y / newVectorLength;
}