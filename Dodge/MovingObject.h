/*
* �����̴� ź���� ������ ��� ���� ��ü
*/

#pragma once
struct Direction
{
	double x;
	double y;
};

// �����̴� ��ü�� 
struct Position
{
	double x;
	double y;
};

class FlickerFreeDC;
class MovingObject

{
public:
	MovingObject(const Position position,Direction direction, double speed);

	// ȭ�� ��ü ���� �ֽ�ȭ
	virtual void update(const double elapsedTime, Position* onlyCalc=nullptr);

	// ������
	virtual void render(FlickerFreeDC& dc) {}

	// ��ü�� ���� ����
	void setDirection(const Direction& newDirection);

	// ��ü�� ���� ����
	Direction getDirection() const { return m_direction; }

	// ��ü�� ��ġ ����
	void setPosition(const Position& position) { m_position = position; }

	// ��ü ��ǥ�� ��ġ ����
	const Position& getPosition() const { return m_position; }

	// ��ü �ӵ� ����
	void setSpeed(const double speed) { m_speed = speed;  }

	// ��ü�� �ӵ� ����
	double getSpeed() const { return m_speed; }
	
private:
	double m_speed{ 0.0 }; // �ʴ� �̵� �ȼ���
	Direction m_direction; // ���⺤��, ũ��� ������ 1�� �����Ѵ�
	Position m_position; // ��ü ��ġ
};

