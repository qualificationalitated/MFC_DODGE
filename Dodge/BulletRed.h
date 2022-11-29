#pragma once
/*
붉은 탄막 객체 구현을 위한 헤더파일
*/

#include<string>
#include<memory>
#include "FlickerFreeDC.h"
#include "MovingObject.h"

class BulletRed : public MovingObject
{
public:
	// 생성자, 더미 객체 생성을 위함
	BulletRed();

	// 생성자, 이미지형태, 탄막 사이즈, 탄막 위치를 인자로 받아 동작용 객체 생성
	BulletRed(const CString& imageFileName, CSize m_bulletSize,
		Position position, Direction direction,double speed);
	
	// 총알 상태 update 함수
	void update(const double elapsedTime, Position* onlyCalc = nullptr) override;
	
	// 총알 다시그리기
	// 플리커링이 제거된 형태의 오브젝트 구현을 위한 render
	void render(FlickerFreeDC& dc) override;

	// 총알 크기 리턴 함수
	const CSize& getBulletSize() { return m_bulletSize; }

private:
	CImage m_ImageBulletRed; // 탄막 이미지
	CSize m_bulletSize; // 탄막 크기
};