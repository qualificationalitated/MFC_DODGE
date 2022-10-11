/*
* 붉은 탄막 구현부분
*/

#include "pch.h"
#include "BulletRed.h"

// 생성자, 이미지형태, 탄막 사이즈, 탄막 위치를 인자로 받습니다
BulletRed::BulletRed(const CString& imageFileName, const CSize m_bulletSize, 
	const Position position, const Direction direction,const double speed) :
	MovingObject{ position,direction,speed }, m_bulletSize {m_bulletSize}
{
	// 이미지 로드하기
	m_ImageBulletRed.Load(imageFileName);
}

void BulletRed::update(const double elapsedTime) {
	MovingObject::update(elapsedTime);
}

// 플리커링 제거를 위한 렌더함수 구현
void BulletRed::render(FlickerFreeDC& dc) {
	const auto& position{ getPosition() };
	m_ImageBulletRed.Draw(dc.GetSafeHdc(), position.x, position.y, m_bulletSize.cx, m_bulletSize.cy);
}