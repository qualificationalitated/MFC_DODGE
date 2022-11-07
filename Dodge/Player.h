#pragma once

#include<string>
#include<memory>
#include "FlickerFreeDC.h"
#include "MovingObject.h"

class Player : public MovingObject
{
public:
	// 플레이어 생성함수
	Player(const CString& imageFileName, CSize playerSize,
		Position position, Direction direction, double speed);
	// 플레이어 상태 update 함수
	void update(const double elapsedTime, Position* onlyCalc = nullptr) override;
	// 플레이어 다시그리기
	void render(FlickerFreeDC& dc) override;
	// 플레이어 크기 얻기
	const CSize& getPlayerSize() { return m_PlayerSize; }
	// 플레이어 히트박스 얻기
	CRect getHitbox();
private:
	CImage m_ImagePlayer; // 플레이어 이미지
	CSize m_PlayerSize; // 플레이어 크기
};

