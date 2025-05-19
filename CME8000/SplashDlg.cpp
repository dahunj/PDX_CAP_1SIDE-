// SplashDlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CME8000.h"
#include "SplashDlg.h"
#include "afxdialogex.h"

// CSplashDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSplashDlg, CDialogEx)

CSplashDlg::CSplashDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSplashDlg::IDD, pParent)
{
}

CSplashDlg::~CSplashDlg()
{
}

void CSplashDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_LABEL_0 + i, m_Label[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_IMAGE_0 + i, m_Image[i]);
}

BEGIN_MESSAGE_MAP(CSplashDlg, CDialogEx)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

// CSplashDlg 메시지 처리기입니다.

BOOL CSplashDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowText("Splash");

	Initial_Controls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CSplashDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

HBRUSH CSplashDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	if (nCtlColor == CTLCOLOR_DLG)
		return (HBRUSH)GetStockObject(WHITE_BRUSH);

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

///////////////////////////////////////////////////////////////////////////////
// User Functions

void CSplashDlg::Initial_Controls() 
{
	m_Label[0].Init_Ctrl("Arial Black", 20, TRUE, RGB(0x00, 0x00, 0xFF), RGB(0xFF, 0xFF, 0xFF));
	m_Label[1].Init_Ctrl("Arial Black", 24, TRUE, RGB(0xFF, 0x00, 0x00), RGB(0xFF, 0xFF, 0xFF));
	m_Label[2].Init_Ctrl("바탕", 12, TRUE, RGB(0x80, 0x20, 0x20), RGB(0xFF, 0xFF, 0xFF));
	m_Label[3].Init_Ctrl("바탕", 12, TRUE, RGB(0x80, 0x20, 0x20), RGB(0xFF, 0xFF, 0xFF));

	m_bmpImage[0].LoadBitmap(IDB_SYNAPSE_IMAGE);
	m_Image[0].SetBitmap(m_bmpImage[0]);
	m_Image[0].SetWindowPos(this, 0, 0, 780, 340, SWP_NOMOVE | SWP_NOZORDER);

	m_bmpImage[1].LoadBitmap(IDB_SYNAPSE_LOGO);
	m_Image[1].SetBitmap(m_bmpImage[1]);
	m_Image[1].SetWindowPos(this, 0, 0, 260, 75, SWP_NOMOVE);
}

///////////////////////////////////////////////////////////////////////////////
