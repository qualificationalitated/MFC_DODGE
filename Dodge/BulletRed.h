#pragma once
/*
���� ź�� ��ü ������ ���� �������
*/

#include<string>
#include "FlickerFreeDC.h"
#include "MovingObject.h"

class BulletRed : public MovingObject
{
public:
	// �Ѿ� �����Լ�
	BulletRed(const CString& imageFileName, CSize m_bulletSize,
		Position position, Direction direction,double speed);

	
	// �Ѿ� ���� update �Լ�
	void update(const double elapsedTime, Position* onlyCalc = nullptr) override;
	
	// �Ѿ� �ٽñ׸���
	void render(FlickerFreeDC& dc) override;

	const CSize& getBulletSize() { return m_bulletSize; }
private:
	CImage m_ImageBulletRed; // ź�� �̹���
	CSize m_bulletSize; // ź�� ũ��
};

