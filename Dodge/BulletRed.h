#pragma once
/*
���� ź�� ��ü ������ ���� �������
*/

#include<string>
#include<memory>
#include "FlickerFreeDC.h"
#include "MovingObject.h"

class BulletRed : public MovingObject
{
public:
	// ������, ���� ��ü ������ ����
	BulletRed();

	// ������, �̹�������, ź�� ������, ź�� ��ġ�� ���ڷ� �޾� ���ۿ� ��ü ����
	BulletRed(const CString& imageFileName, CSize m_bulletSize,
		Position position, Direction direction,double speed);
	
	// �Ѿ� ���� update �Լ�
	void update(const double elapsedTime, Position* onlyCalc = nullptr) override;
	
	// �Ѿ� �ٽñ׸���
	// �ø�Ŀ���� ���ŵ� ������ ������Ʈ ������ ���� render
	void render(FlickerFreeDC& dc) override;

	// �Ѿ� ũ�� ���� �Լ�
	const CSize& getBulletSize() { return m_bulletSize; }

private:
	CImage m_ImageBulletRed; // ź�� �̹���
	CSize m_bulletSize; // ź�� ũ��
};