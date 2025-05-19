// MonitorInOutTabDlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CME8000.h"
#include "MonitorInOutTabDlg.h"
#include "afxdialogex.h"

#include "LogFile.h"
#include "AJinAXL.h"

// CMonitorInOutTabDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMonitorInOutTabDlg, CDialogEx)

CMonitorInOutTabDlg::CMonitorInOutTabDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMonitorInOutTabDlg::IDD, pParent)
{
}

CMonitorInOutTabDlg::~CMonitorInOutTabDlg()
{
}

void CMonitorInOutTabDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_GROUP_0 + i, m_Group[i]);
	for (int i = 0; i < 32; i++) DDX_Control(pDX, IDC_LED_IN_0 + i, m_ledIn[i]);
	for (int i = 0; i < 32; i++) DDX_Control(pDX, IDC_LED_OUT_0 + i, m_ledOut[i]);
}

BEGIN_MESSAGE_MAP(CMonitorInOutTabDlg, CDialogEx)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

// CMonitorInOutTabDlg 메시지 처리기입니다.

BOOL CMonitorInOutTabDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(NULL, 5, 60, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	Initial_Controls();

	m_nInOutTab = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CMonitorInOutTabDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CMonitorInOutTabDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (!bShow) return;

	if (m_nInOutTab < 0 || m_nInOutTab > 15) return;

	CString strKey, strName, strText;

	CIniFileCS INI(gsCurrentDir + "\\System\\InOutList.ini");
	if (!INI.Check_File()) {
		AfxMessageBox("InOutList.ini File Not Found!!!");
		return;
	}

	for (int i = 0; i < 32; i++) {
		strKey.Format("%04d", m_nInOutTab * 100 + i);

		strName = INI.Get_String("INPUT", strKey, "");
		strText.Format("[%s] %s", strKey, strName);
		m_ledIn[i].SetWindowText(strText);

		strName = INI.Get_String("OUTPUT", strKey, "");
		strText.Format("[%s] %s", strKey, strName);
		m_ledOut[i].SetWindowText(strText);
	}

	m_strLog.Format("[Monitor InOut] Show Window - Tab (%d)", m_nInOutTab);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

///////////////////////////////////////////////////////////////////////////////
// User Functions

void CMonitorInOutTabDlg::Initial_Controls() 
{
	m_Group[0].Init_Ctrl("바탕", 12, TRUE, RGB(0x20, 0x80, 0x20), COLOR_DEFAULT);
	m_Group[1].Init_Ctrl("바탕", 12, TRUE, RGB(0x80, 0x10, 0x10), COLOR_DEFAULT);
	for (int i = 0; i < 32; i++) m_ledIn[i].Init_Ctrl("굴림", 9, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i < 32; i++) m_ledOut[i].Init_Ctrl("굴림", 9, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
}

void CMonitorInOutTabDlg::Display_Status()
{
	DXY_DATA *pDX = g_objAJinAXL.Get_pDX(m_nInOutTab);
	DXY_DATA *pDY = g_objAJinAXL.Get_pDY(m_nInOutTab);

	for (int i = 0; i < 32; i++) {
		if ((pDX->nValue >> i) & 1) m_ledIn[i].Set_On(TRUE);	// On
		else m_ledIn[i].Set_On(FALSE);							// Off

		if ((pDY->nValue >> i) & 1) m_ledOut[i].Set_On(TRUE);	// On
		else m_ledOut[i].Set_On(FALSE);							// Off
	}
}

///////////////////////////////////////////////////////////////////////////////
