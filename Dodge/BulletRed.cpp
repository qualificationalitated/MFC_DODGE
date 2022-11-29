/*
* 붉은 탄막 구현부분
*/

#include "pch.h"
#include "BulletRed.h"

BulletRed::BulletRed() :MovingObject{ {0.0,0.0},{0.0,0.0},0.0 }, m_bulletSize{} {}

// 생성자, 이미지형태, 탄막 사이즈, 탄막 위치를 인자로 받습니다
BulletRed::BulletRed(const CString& imageFileName, const CSize m_bulletSize, 
	const Position position, const Direction direction,const double speed) :
	MovingObject{ position,direction,speed }, m_bulletSize {m_bulletSize}
{
	// 이미지 로드하기
	m_ImageBulletRed.Load(imageFileName);
}

void BulletRed::update(const double elapsedTime, Position* onlyCalc) {
	MovingObject::update(elapsedTime, onlyCalc);
	if (onlyCalc)
		return;
}

// 플리커링이 제거된 형태의 오브젝트 구현을 위한 렌더함수 구현
void BulletRed::render(FlickerFreeDC& dc) {
	// 생성할 오브젝트의 위치를 받아오는 함수
	const auto& position{ getPosition() };
	// 해당 위치에 그림을 그려준다
	m_ImageBulletRed.Draw(dc.GetSafeHdc(), position.x, position.y, m_bulletSize.cx, m_bulletSize.cy);
}