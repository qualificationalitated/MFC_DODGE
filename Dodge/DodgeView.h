
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
	std::chrono::steady_clock::time_point m_previousUpdateTime; // 탄막 위치 설정용 변수
	Game m_game; // 플레이하는 게임을 나타내는 객체
	

public:
	// 플리커링 제거를 위한 설정중 하나
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	// 시간이 지나며 자동으로 화면을 업데이트 한다
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	// 게임 시작 및 플레이어 이동을 제어하기 위한 키 다운
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	// 플레이어 이동을 제어하기 위한 키 업
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // DodgeView.cpp의 디버그 버전
inline CDodgeDoc* CDodgeView::GetDocument() const
   { return reinterpret_cast<CDodgeDoc*>(m_pDocument); }
#endif

