#pragma once

#include<string>
#include<memory>
#include "FlickerFreeDC.h"
#include "MovingObject.h"

class Player : public MovingObject
{
public:
	// �÷��̾� �����Լ�
	Player(const CString& imageFileName, CSize playerSize,
		Position position, Direction direction, double speed);
	// �÷��̾� ���� update �Լ�
	void update(const double elapsedTime, Position* onlyCalc = nullptr) override;
	// �÷��̾� �ٽñ׸���
	void render(FlickerFreeDC& dc) override;
	// �÷��̾� ũ�� ���
	const CSize& getPlayerSize() { return m_PlayerSize; }
	// �÷��̾� ��Ʈ�ڽ� ���
	CRect getHitbox();
private:
	CImage m_ImagePlayer; // �÷��̾� �̹���
	CSize m_PlayerSize; // �÷��̾� ũ��
};

