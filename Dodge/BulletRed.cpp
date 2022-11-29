/*
* ���� ź�� �����κ�
*/

#include "pch.h"
#include "BulletRed.h"

BulletRed::BulletRed() :MovingObject{ {0.0,0.0},{0.0,0.0},0.0 }, m_bulletSize{} {}

// ������, �̹�������, ź�� ������, ź�� ��ġ�� ���ڷ� �޽��ϴ�
BulletRed::BulletRed(const CString& imageFileName, const CSize m_bulletSize, 
	const Position position, const Direction direction,const double speed) :
	MovingObject{ position,direction,speed }, m_bulletSize {m_bulletSize}
{
	// �̹��� �ε��ϱ�
	m_ImageBulletRed.Load(imageFileName);
}

void BulletRed::update(const double elapsedTime, Position* onlyCalc) {
	MovingObject::update(elapsedTime, onlyCalc);
	if (onlyCalc)
		return;
}

// �ø�Ŀ���� ���ŵ� ������ ������Ʈ ������ ���� �����Լ� ����
void BulletRed::render(FlickerFreeDC& dc) {
	// ������ ������Ʈ�� ��ġ�� �޾ƿ��� �Լ�
	const auto& position{ getPosition() };
	// �ش� ��ġ�� �׸��� �׷��ش�
	m_ImageBulletRed.Draw(dc.GetSafeHdc(), position.x, position.y, m_bulletSize.cx, m_bulletSize.cy);
}