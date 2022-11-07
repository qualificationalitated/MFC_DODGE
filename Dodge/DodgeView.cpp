
// DodgeView.cpp: CDodgeView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Dodge.h"
#endif

#include "DodgeDoc.h"
#include "DodgeView.h"
#include "FlickerFreeDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDodgeView

IMPLEMENT_DYNCREATE(CDodgeView, CView)

BEGIN_MESSAGE_MAP(CDodgeView, CView)
//	ON_WM_LBUTTONDOWN()
ON_WM_MOUSEMOVE()
ON_WM_ERASEBKGND()
ON_WM_TIMER()
ON_WM_LBUTTONDOWN()
//ON_WM_RBUTTONDOWN()
//ON_WM_GETMINMAXINFO()
ON_WM_KEYDOWN()
ON_WM_KEYUP()
END_MESSAGE_MAP()

// CDodgeView 생성/소멸

CDodgeView::CDodgeView() noexcept
{
	// m_BulletRed.Load(_T("BulletRed.png"));

}

CDodgeView::~CDodgeView()
{
}

BOOL CDodgeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.
	return CView::PreCreateWindow(cs);
}

// CDodgeView 그리기

void CDodgeView::OnDraw(CDC* pDC)
{
	CDodgeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	FlickerFreeDC dc{ pDC };
	// m_RedBullet.render(dc);
	m_game.render(dc);
}


// CDodgeView 진단

#ifdef _DEBUG
void CDodgeView::AssertValid() const
{
	CView::AssertValid();
}

void CDodgeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDodgeDoc* CDodgeView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDodgeDoc)));
	return (CDodgeDoc*)m_pDocument;
}
#endif //_DEBUG


// CDodgeView 메시지 처리기


void CDodgeView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	// pos++;
	// Invalidate();
	// UpdateWindow();
	CView::OnMouseMove(nFlags, point);
}

// 화면 플리커링 제거를 위한 메세지 핸들러
BOOL CDodgeView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return FALSE;
}


void CDodgeView::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == GAME_TIMER) {

		// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
		auto currentTime = std::chrono::high_resolution_clock::now();
		double timeElapsed{ std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - m_previousUpdateTime).count()/1000.0 };
		m_previousUpdateTime = currentTime;

		// m_RedBullet.update(timeElapsed);

		m_game.update();
		Invalidate();
		UpdateWindow();
	}

	CView::OnTimer(nIDEvent);
}


void CDodgeView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// 마우스 좌클릭시 게임을 시작
	m_game.start();
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_previousUpdateTime = std::chrono::high_resolution_clock::now();
	SetTimer(GAME_TIMER,10,NULL);
}



void CDodgeView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nChar)
	{
	case VK_LEFT:
		m_game.DirKeyDown(KeyDirection::ArrowLeft);
		break;
	case VK_RIGHT:
		m_game.DirKeyDown(KeyDirection::ArrowRight);
		break;
	case VK_UP:
		m_game.DirKeyDown(KeyDirection::ArrowUp);
		break;
	case VK_DOWN:
		m_game.DirKeyDown(KeyDirection::ArrowDown);
		break;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CDodgeView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nChar)
	{
	case VK_LEFT:
		m_game.DirKeyUp(KeyDirection::ArrowLeft);
		break;
	case VK_RIGHT:
		m_game.DirKeyUp(KeyDirection::ArrowRight);
		break;
	case VK_UP:
		m_game.DirKeyUp(KeyDirection::ArrowUp);
		break;
	case VK_DOWN:
		m_game.DirKeyUp(KeyDirection::ArrowDown);
		break;
	}

	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}
