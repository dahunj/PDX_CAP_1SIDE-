// SetupInOutTabDlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CME8000.h"
#include "SetupInOutTabDlg.h"
#include "afxdialogex.h"

#include "LogFile.h"
#include "AJinAXL.h"
#include "Common.h"

// CSetupInOutTabDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSetupInOutTabDlg, CDialogEx)

CSetupInOutTabDlg::CSetupInOutTabDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetupInOutTabDlg::IDD, pParent)
{
}

CSetupInOutTabDlg::~CSetupInOutTabDlg()
{
}

void CSetupInOutTabDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_GROUP_0 + i, m_Group[i]);
	for (int i = 0; i < 64; i++) DDX_Control(pDX, IDC_STC_IN_0 + i, m_stcIn[i]);
	for (int i = 0; i < 64; i++) DDX_Control(pDX, IDC_CHK_OUT_0 + i, m_chkOut[i]);
}

BEGIN_MESSAGE_MAP(CSetupInOutTabDlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHK_OUT_0, IDC_CHK_OUT_63, OnChkOutClick)
END_MESSAGE_MAP()

// CSetupInOutTabDlg 메시지 처리기입니다.

BOOL CSetupInOutTabDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(NULL, 5, 65, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	Initial_Controls();

	m_nInOutTab = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CSetupInOutTabDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CSetupInOutTabDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (!bShow) return;

	CString strKey, strName, strText;

	CIniFileCS INI(gsCurrentDir + "\\System\\InOutList.ini");
	if (!INI.Check_File()) {
		AfxMessageBox("InOutList.ini File Not Found!!!");
		return;
	}

	BOOL bVisible = (m_nInOutTab == 6 ? TRUE : TRUE);
	int nMaxCount = (m_nInOutTab == 6 ? 2 : 2);

	for (int i = 0; i < 32; i++) m_stcIn[i + 32].ShowWindow(bVisible);
	for (int i = 0; i < 32; i++) m_chkOut[i + 32].ShowWindow(bVisible);

	for (int i = 0; i < nMaxCount; i++) {
		int nPort = m_nInOutTab * 2 + i;
		for (int j = 0; j < 32; j++) {
			strKey.Format("%04d", nPort * 100 + j);

			strName = INI.Get_String("INPUT", strKey, "");
			strText.Format("[%s] %s", strKey, strName);
			m_stcIn[i * 32 + j].SetWindowText(strText);

			strName = INI.Get_String("OUTPUT", strKey, "");
			strText.Format("[%s] %s", strKey, strName);
			m_chkOut[i * 32 + j].SetWindowText(strText);
		}
	}

	// 출력 Display
	for (int i = 0; i < nMaxCount; i++) {
		int nPort = m_nInOutTab * 2 + i;
		DXY_DATA *pDY = g_objAJinAXL.Get_pDY(nPort);

		for (int j = 0; j < 32; j++) {
			if ((pDY->nValue >> j) & 1) Set_CheckBox(i * 32 + j, TRUE);
			else Set_CheckBox(i * 32 + j, FALSE);
		}
	}

	m_strLog.Format("[Setup InOut] Show Window - Tab (%d)", m_nInOutTab);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CSetupInOutTabDlg::OnChkOutClick(UINT nID)
{
	int ID = nID - IDC_CHK_OUT_0;

	if (m_chkOut[ID].GetCheck()) {
		m_chkOut[ID].Set_Color(RGB(0x01, 0x00, 0x00), RGB(0xFF, 0xC0, 0x00));	// On
		Set_Output(ID, TRUE);
	} else {
		m_chkOut[ID].Set_Color(RGB(0xFF, 0xFF, 0xFF), RGB(0x40, 0x20, 0x20));	// Off
		Set_Output(ID, FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// User Functions

void CSetupInOutTabDlg::Initial_Controls() 
{
	m_Group[0].Init_Ctrl("바탕", 12, TRUE, RGB(0x20, 0x80, 0x20), COLOR_DEFAULT);
	m_Group[1].Init_Ctrl("바탕", 12, TRUE, RGB(0x80, 0x10, 0x10), COLOR_DEFAULT);
	for (int i = 0; i < 64; i++) m_stcIn[i].Init_Ctrl("바탕", 10, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x20, 0x50, 0x00));			// Off
	for (int i = 0; i < 64; i++) m_chkOut[i].Init_Ctrl("바탕", 10, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x40, 0x20, 0x20), 0, 0);	// Off
}

void CSetupInOutTabDlg::Display_Status()
{
	int nMaxCount = (m_nInOutTab == 7 ? 1 : 2);

	for (int i = 0; i < nMaxCount; i++) {
		int nPort = m_nInOutTab * 2 + i;
		DXY_DATA *pDX = g_objAJinAXL.Get_pDX(nPort);

		for (int j = 0; j < 32; j++) {
			if ((pDX->nValue >> j) & 1) m_stcIn[i * 32 + j].Set_Color(RGB(0x01, 0x00, 0x00), RGB(0x20, 0xFF, 0x20));	// On
			else						m_stcIn[i * 32 + j].Set_Color(RGB(0xFF, 0xFF, 0xFF), RGB(0x20, 0x50, 0x00));	// Off
		}
	}
}

void CSetupInOutTabDlg::Set_CheckBox(int nIdx, BOOL bChk)
{
	if (bChk) {
		m_chkOut[nIdx].SetCheck(TRUE);
		m_chkOut[nIdx].Set_Color(RGB(0x01, 0x00, 0x00), RGB(0xFF, 0xC0, 0x00));	// On
	} else {
		m_chkOut[nIdx].SetCheck(FALSE);
		m_chkOut[nIdx].Set_Color(RGB(0xFF, 0xFF, 0xFF), RGB(0x40, 0x20, 0x20));	// Off
	}
}

void CSetupInOutTabDlg::Set_Output(int nIdx, BOOL bOut)
{
	int nPort = m_nInOutTab * 2 + nIdx / 32;
	DXY_DATA *pDY = g_objAJinAXL.Get_pDY(nPort);

	if (bOut) pDY->nValue |= (1 << nIdx);
	else	  pDY->nValue &= ~(1 << nIdx);

	g_objAJinAXL.Write_Output(nPort);
}

/////////////////////////////////////////////////////////////////////////////
