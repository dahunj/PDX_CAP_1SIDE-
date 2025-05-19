// SetupInOutDlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CME8000.h"
#include "SetupInOutDlg.h"
#include "afxdialogex.h"

// CSetupInOutDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSetupInOutDlg, CDialogEx)

CSetupInOutDlg::CSetupInOutDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetupInOutDlg::IDD, pParent)
{
}

CSetupInOutDlg::~CSetupInOutDlg()
{
}

void CSetupInOutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_SETUP_IN_OUT, m_tabSetupInOut);
}

BEGIN_MESSAGE_MAP(CSetupInOutDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB_SETUP_IN_OUT, &CSetupInOutDlg::OnTcnSelchangingTabSetupInOut)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_SETUP_IN_OUT, &CSetupInOutDlg::OnTcnSelchangeTabSetupInOut)
END_MESSAGE_MAP()

// CSetupInOutDlg 메시지 처리기입니다.

BOOL CSetupInOutDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(this, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	Initial_Controls();

	m_pSetupInOutTabDlg = new CSetupInOutTabDlg(&m_tabSetupInOut);
	m_pSetupInOutTabDlg->Create(IDD_SETUP_IN_OUT_TAB_DLG, &m_tabSetupInOut);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CSetupInOutDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CSetupInOutDlg::OnDestroy() 
{
	CDialogEx::OnDestroy();

	m_pSetupInOutTabDlg->DestroyWindow();
	if (m_pSetupInOutTabDlg) delete m_pSetupInOutTabDlg;
	m_pSetupInOutTabDlg = NULL;
}

void CSetupInOutDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow) {
		int nTab = m_tabSetupInOut.GetCurSel();
		m_pSetupInOutTabDlg->Set_InOutTab(nTab);
		m_pSetupInOutTabDlg->ShowWindow(SW_SHOW);

	} else {
		m_pSetupInOutTabDlg->ShowWindow(SW_HIDE);
	}
}

void CSetupInOutDlg::OnTcnSelchangingTabSetupInOut(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_pSetupInOutTabDlg->ShowWindow(SW_HIDE);
	*pResult = 0;
}

void CSetupInOutDlg::OnTcnSelchangeTabSetupInOut(NMHDR *pNMHDR, LRESULT *pResult)
{
	int nTab = m_tabSetupInOut.GetCurSel();
	m_pSetupInOutTabDlg->Set_InOutTab(nTab);
	m_pSetupInOutTabDlg->ShowWindow(SW_SHOW);
	*pResult = 0;
}

///////////////////////////////////////////////////////////////////////////////
// User Functions

void CSetupInOutDlg::Initial_Controls() 
{
	m_tabSetupInOut.InsertItem( 0, "[00] - [01]");
	m_tabSetupInOut.InsertItem( 1, "[02] - [03]");
	m_tabSetupInOut.InsertItem( 2, "[04] - [05]");
	m_tabSetupInOut.InsertItem( 3, "[06] - [07]");
	m_tabSetupInOut.InsertItem( 4, "[08] - [09]");
	m_tabSetupInOut.InsertItem( 5, "[10] - [11]");
	m_tabSetupInOut.InsertItem( 6, "[12] - [13]");
	m_tabSetupInOut.SetItemSize(CSize(210, 60));
	m_tabSetupInOut.Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0x00, 0xFF), COLOR_DEFAULT);
}

void CSetupInOutDlg::Display_Status()
{
	m_pSetupInOutTabDlg->Display_Status();
}

///////////////////////////////////////////////////////////////////////////////
