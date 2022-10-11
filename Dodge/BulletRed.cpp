/*
* ���� ź�� �����κ�
*/

#include "pch.h"
#include "BulletRed.h"

// ������, �̹�������, ź�� ������, ź�� ��ġ�� ���ڷ� �޽��ϴ�
BulletRed::BulletRed(const CString& imageFileName, const CSize m_bulletSize, 
	const Position position, const Direction direction,const double speed) :
	MovingObject{ position,direction,speed }, m_bulletSize {m_bulletSize}
{
	// �̹��� �ε��ϱ�
	m_ImageBulletRed.Load(imageFileName);
}

void BulletRed::update(const double elapsedTime) {
	MovingObject::update(elapsedTime);
}

// �ø�Ŀ�� ���Ÿ� ���� �����Լ� ����
void BulletRed::render(FlickerFreeDC& dc) {
	const auto& position{ getPosition() };
	m_ImageBulletRed.Draw(dc.GetSafeHdc(), position.x, position.y, m_bulletSize.cx, m_bulletSize.cy);
}