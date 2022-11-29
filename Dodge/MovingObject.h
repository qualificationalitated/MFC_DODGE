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

	// 화면 객체 상태 최신화
	virtual void update(const double elapsedTime, Position* onlyCalc=nullptr);

	// 렌더링
	virtual void render(FlickerFreeDC& dc) {}

	// 객체의 방향 결정
	void setDirection(const Direction& newDirection);

	// 객체의 방향 리턴
	Direction getDirection() const { return m_direction; }

	// 객체의 위치 결정
	void setPosition(const Position& position) { m_position = position; }

	// 객체 좌표상 위치 리턴
	const Position& getPosition() const { return m_position; }

	// 객체 속도 결정
	void setSpeed(const double speed) { m_speed = speed;  }

	// 객체의 속도 리턴
	double getSpeed() const { return m_speed; }
	
private:
	double m_speed{ 0.0 }; // 초당 이동 픽셀수
	Direction m_direction; // 방향벡터, 크기는 언제나 1로 설정한다
	Position m_position; // 객체 위치
};

