
// DodgeView.h: CDodgeView 클래스의 인터페이스
//

#pragma once
#include <chrono>
#include "BulletRed.h"
#include "Game.h"

const UINT_PTR GAME_TIMER{1};
class CDodgeView : public CView
{
protected: // serialization에서만 만들어집니다.
	CDodgeView() noexcept;
	DECLARE_DYNCREATE(CDodgeView)

// 특성입니다.
public:
	CDodgeDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 구현입니다.
public:
	virtual ~CDodgeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

private:
	// 탄막 위치 설정용 변수
	std::chrono::steady_clock::time_point m_previousUpdateTime;
	int pos{1};

	// 붉은 탄막객체 생성
	BulletRed m_RedBullet{ _T("BulletRed.png") ,CSize{14,14}, Position{100.0,70.0},
		Direction{1.0,1.0},200.0 };
	Game m_game;

public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // DodgeView.cpp의 디버그 버전
inline CDodgeDoc* CDodgeView::GetDocument() const
   { return reinterpret_cast<CDodgeDoc*>(m_pDocument); }
#endif

