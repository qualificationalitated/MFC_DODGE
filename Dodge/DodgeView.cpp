
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
ON_WM_ERASEBKGND()
ON_WM_TIMER()
ON_WM_KEYDOWN()
ON_WM_KEYUP()
END_MESSAGE_MAP()

// CDodgeView 생성/소멸

CDodgeView::CDodgeView() noexcept
{
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
	FlickerFreeDC dc{ pDC }; // 플리커링이 제거된 dc 객체로 구현
	Gdiplus::Graphics gr(dc); // 그래픽으로 화면 출력을 위한 객체 생성

	Pen pen(Color(255, 255, 255, 0), 10);
	SolidBrush bgdBrush(Color(255, 17, 38, 79));
	CRect windowRect;
	
	// 화면 배경 및 테두리 선 설정
	GetClientRect(windowRect);
	gr.FillRectangle(&bgdBrush, 0, windowRect.left, windowRect.Width(), windowRect.Height());
	gr.DrawRectangle(&pen, windowRect.top, windowRect.left, windowRect.Width(), windowRect.Height());
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

		// 시간이 지나며 자동으로 화면을 업데이트합니다.
		m_game.update();
		Invalidate();
		UpdateWindow();
	}

	CView::OnTimer(nIDEvent);
}

void CDodgeView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// 스페이스바 클릭시 게임이 시작되지 않았다면
	if(nChar==VK_SPACE && !m_game.getGameStatus()) {
		// 게임 시작 및 시작시간을 체크
		m_game.start();
		m_previousUpdateTime = std::chrono::high_resolution_clock::now();
		SetTimer(GAME_TIMER, 10, NULL);
	}
	// 스페이스바 클릭시 게임이 종료된 상태라면
	else if (nChar == VK_SPACE && m_game.getGameStatus() == 2) {
		// 게임 초기상태로 되돌리기
		m_game.setInitialGame();
	}
	// 게임이 플레이중이라면
	else if (m_game.getGameStatus() == 1) {
		// 유저의 키 입력에 따라 플레이어를 움직인다.
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
	}
}

void CDodgeView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// 유저의 키 입력에 따라 플레이어 이동을 멈춘다.
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
