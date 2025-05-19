// MonitorInOutDlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CME8000.h"
#include "MonitorInOutDlg.h"
#include "afxdialogex.h"

// CMonitorInOutDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMonitorInOutDlg, CDialogEx)

CMonitorInOutDlg::CMonitorInOutDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMonitorInOutDlg::IDD, pParent)
{
}

CMonitorInOutDlg::~CMonitorInOutDlg()
{
}

void CMonitorInOutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_MONITOR_IN_OUT, m_tabMonitorInOut);
}

BEGIN_MESSAGE_MAP(CMonitorInOutDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB_MONITOR_IN_OUT, &CMonitorInOutDlg::OnTcnSelchangingTabMonitorInOut)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MONITOR_IN_OUT, &CMonitorInOutDlg::OnTcnSelchangeTabMonitorInOut)
END_MESSAGE_MAP()

// CMonitorInOutDlg 메시지 처리기입니다.

BOOL CMonitorInOutDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(this, 0, 70, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	Initial_Controls();

	m_pMonitorInOutTabDlg = new CMonitorInOutTabDlg(&m_tabMonitorInOut);
	m_pMonitorInOutTabDlg->Create(IDD_MONITOR_IN_OUT_TAB_DLG, &m_tabMonitorInOut);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CMonitorInOutDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CMonitorInOutDlg::OnDestroy() 
{
	CDialogEx::OnDestroy();

	m_pMonitorInOutTabDlg->DestroyWindow();
	if (m_pMonitorInOutTabDlg) delete m_pMonitorInOutTabDlg;
	m_pMonitorInOutTabDlg = NULL;
}

void CMonitorInOutDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow) {
		int nTab = m_tabMonitorInOut.GetCurSel();
		m_pMonitorInOutTabDlg->Set_InOutTab(nTab);
		m_pMonitorInOutTabDlg->ShowWindow(SW_SHOW);
	} else {
		m_pMonitorInOutTabDlg->ShowWindow(SW_HIDE);
	}
}

void CMonitorInOutDlg::OnTcnSelchangingTabMonitorInOut(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_pMonitorInOutTabDlg->ShowWindow(SW_HIDE);
	*pResult = 0;
}

void CMonitorInOutDlg::OnTcnSelchangeTabMonitorInOut(NMHDR *pNMHDR, LRESULT *pResult)
{
	int nTab = m_tabMonitorInOut.GetCurSel();
	m_pMonitorInOutTabDlg->Set_InOutTab(nTab);
	m_pMonitorInOutTabDlg->ShowWindow(SW_SHOW);
	*pResult = 0;
}

///////////////////////////////////////////////////////////////////////////////
// User Functions

void CMonitorInOutDlg::Initial_Controls() 
{
	m_tabMonitorInOut.InsertItem( 0, "[00]");
	m_tabMonitorInOut.InsertItem( 1, "[01]");
	m_tabMonitorInOut.InsertItem( 2, "[02]");
	m_tabMonitorInOut.InsertItem( 3, "[03]");
	m_tabMonitorInOut.InsertItem( 4, "[04]");
	m_tabMonitorInOut.InsertItem( 5, "[05]");
	m_tabMonitorInOut.InsertItem( 6, "[06]");
	m_tabMonitorInOut.InsertItem( 7, "[07]");
	m_tabMonitorInOut.InsertItem( 8, "[08]");
	m_tabMonitorInOut.InsertItem( 9, "[09]");
	m_tabMonitorInOut.InsertItem(10, "[10]");
	m_tabMonitorInOut.InsertItem(11, "[11]");
	m_tabMonitorInOut.InsertItem(12, "[12]");
	m_tabMonitorInOut.InsertItem(13, "[13]");
	m_tabMonitorInOut.SetItemSize(CSize(70, 50));
	m_tabMonitorInOut.Init_Ctrl("바탕", 14, TRUE, RGB(0x00, 0x00, 0xFF), COLOR_DEFAULT);
}

void CMonitorInOutDlg::Display_Status()
{
	m_pMonitorInOutTabDlg->Display_Status();
}

///////////////////////////////////////////////////////////////////////////////
