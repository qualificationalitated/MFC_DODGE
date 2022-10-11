/*
* 움직이는 탄막의 움직임 제어를 위한 객체
*/

#pragma once
struct Direction
{
	double x;
	double y;
};

// 움직이는 물체를 
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

	// 물체를 움직이는 함수
	virtual void update(const double elapsedTime);

	// 렌더링하는 함수
	virtual void render(FlickerFreeDC& dc) {}
	// 총알의 방향만 결정, 속도를 결정하지는 않는다
	void setDirection(const Direction& newDirection);
	void setPosition(const Position& position) { m_position = position; }
	void setSpeed(const double speed) { m_speed = speed;  }

	const Position& getPosition() const { return m_position; }
	double getSpeed() { return m_speed; }

private:
	// @m_speed :pixel per sec
	double m_speed{ 0.0 };
	// 방향벡터의 크기는 언제나 1로 설정한다
	Direction m_direction;
	Position m_position;
};

