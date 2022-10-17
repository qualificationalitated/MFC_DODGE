#pragma once
/*
붉은 탄막 객체 구현을 위한 헤더파일
*/

#include<string>
#include "FlickerFreeDC.h"
#include "MovingObject.h"

class BulletRed : public MovingObject
{
public:
	// 총알 생성함수
	BulletRed(const CString& imageFileName, CSize m_bulletSize,
		Position position, Direction direction,double speed);

	
	// 총알 상태 update 함수
	void update(const double elapsedTime, Position* onlyCalc = nullptr) override;
	
	// 총알 다시그리기
	void render(FlickerFreeDC& dc) override;

	const CSize& getBulletSize() { return m_bulletSize; }
private:
	CImage m_ImageBulletRed; // 탄막 이미지
	CSize m_bulletSize; // 탄막 크기
};

