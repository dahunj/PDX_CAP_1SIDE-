// MonitorMotionDlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CME8000.h"
#include "MonitorMotionDlg.h"
#include "afxdialogex.h"

// CMonitorMotionDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMonitorMotionDlg, CDialogEx)

CMonitorMotionDlg::CMonitorMotionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMonitorMotionDlg::IDD, pParent)
{
}

CMonitorMotionDlg::~CMonitorMotionDlg()
{
}

void CMonitorMotionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_MONITOR_MOTION, m_tabMonitorMotion);
}

BEGIN_MESSAGE_MAP(CMonitorMotionDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB_MONITOR_MOTION, &CMonitorMotionDlg::OnTcnSelchangingTabMonitorMotion)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MONITOR_MOTION, &CMonitorMotionDlg::OnTcnSelchangeTabMonitorMotion)
END_MESSAGE_MAP()

// CMonitorMotionDlg 메시지 처리기입니다.

BOOL CMonitorMotionDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(this, 0, 70, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	Initial_Controls();

	m_pMonitorMotionTabDlg = new CMonitorMotionTabDlg(&m_tabMonitorMotion);
	m_pMonitorMotionTabDlg->Create(IDD_MONITOR_MOTION_TAB_DLG, &m_tabMonitorMotion);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CMonitorMotionDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CMonitorMotionDlg::OnDestroy() 
{
	CDialogEx::OnDestroy();

	m_pMonitorMotionTabDlg->DestroyWindow();
	if (m_pMonitorMotionTabDlg) delete m_pMonitorMotionTabDlg;
	m_pMonitorMotionTabDlg = NULL;
}

void CMonitorMotionDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow) {
		int nTab = m_tabMonitorMotion.GetCurSel();
		m_pMonitorMotionTabDlg->Set_MotionTab(nTab);
		m_pMonitorMotionTabDlg->ShowWindow(SW_SHOW);
	} else {
		m_pMonitorMotionTabDlg->ShowWindow(SW_HIDE);
	}
}

void CMonitorMotionDlg::OnTcnSelchangingTabMonitorMotion(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_pMonitorMotionTabDlg->ShowWindow(SW_HIDE);
	*pResult = 0;
}

void CMonitorMotionDlg::OnTcnSelchangeTabMonitorMotion(NMHDR *pNMHDR, LRESULT *pResult)
{
	int nTab = m_tabMonitorMotion.GetCurSel();
	m_pMonitorMotionTabDlg->Set_MotionTab(nTab);
	m_pMonitorMotionTabDlg->ShowWindow(SW_SHOW);
	*pResult = 0;
}

///////////////////////////////////////////////////////////////////////////////
// User Functions

void CMonitorMotionDlg::Initial_Controls() 
{
	m_tabMonitorMotion.InsertItem(0, "[00-15]");
	m_tabMonitorMotion.InsertItem(1, "[16-31]");
	m_tabMonitorMotion.InsertItem(2, "[32-34]");
	m_tabMonitorMotion.SetItemSize(CSize(370, 50));
	m_tabMonitorMotion.Init_Ctrl("바탕", 14, TRUE, RGB(0x00, 0x00, 0xFF), COLOR_DEFAULT);	
}

void CMonitorMotionDlg::Display_Status()
{
	m_pMonitorMotionTabDlg->Display_Status();
}

///////////////////////////////////////////////////////////////////////////////
