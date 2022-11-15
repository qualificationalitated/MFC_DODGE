/*
* ź�� ������ ���� �ڵ� / Ŭ���� ����
*/
#include<cassert>
#include "pch.h"
#include "MovingObject.h"

MovingObject::MovingObject(const Position position, const Direction direction, const double speed) :
	m_position{ position },  m_speed{speed}
{
	// �������� ������, �����޼����� ����Ѵ�. ���⺤�Ͱ� 0�̸� �ȵǴ� ��Ȳ�̶�
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
	// �̵��� �� ��ġ = ���� ��ġ + ��������(����)*�Ÿ�
	const double vectorLength = sqrt(pow(m_direction.x, 2) + pow(m_direction.y, 2));
	if (m_direction.x || m_direction.y) {
		m_position.x = m_position.x + m_direction.x / vectorLength * distance;
		m_position.y = m_position.y + m_direction.y / vectorLength * distance;
	}
}

void MovingObject::setDirection(const Direction& newDirection) {
	// ���� ���� ����
	// �������� ���� ������ update���� �����Ѵ�. ȿ������ �������� ������.. ����� ��ø�Ǿ� ���� �ְ���� ���� ����
	m_direction = newDirection;
}