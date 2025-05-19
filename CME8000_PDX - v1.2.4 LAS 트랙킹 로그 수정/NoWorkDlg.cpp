// NoWorkDlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CME8000.h"
#include "NoWorkDlg.h"
#include "afxdialogex.h"

#include "Common.h"
#include "LogFile.h"
#include "CME8000Dlg.h"

// CNoWorkDlg 대화 상자입니다.
CNoWorkDlg g_dlgNoWork;

IMPLEMENT_DYNAMIC(CNoWorkDlg, CDialogEx)

CNoWorkDlg::CNoWorkDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNoWorkDlg::IDD, pParent)
{
}

CNoWorkDlg::~CNoWorkDlg()
{
}

void CNoWorkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LBL_TITLE, m_LabelTitle);
	DDX_Control(pDX, IDC_GROUP_STOP, m_group[0]);
	DDX_Control(pDX, IDC_GROUP_LOSS, m_group[1]);
	for (int i = 0; i < 16; i++) DDX_Control(pDX, IDC_RDO_STOP_0 + i, m_rdoStopReason[i]);
	DDX_Control(pDX, IDC_LBL_OPER_ID, m_lblOperId);
	DDX_Control(pDX, IDC_STC_OPER_ID, m_stcOperId);
	for (int i = 0; i < 3; i++) DDX_Control(pDX, IDC_LBL_TIME_0 + i, m_lblTime[i]);
	for (int i = 0; i < 3; i++) DDX_Control(pDX, IDC_STC_TIME_0 + i, m_stcTime[i]);
	DDX_Control(pDX, IDC_STC_EXIT, m_stcExit);
}

BEGIN_MESSAGE_MAP(CNoWorkDlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_STN_CLICKED(IDC_STC_OPER_ID, &CNoWorkDlg::OnStnClickedStcOperId)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RDO_STOP_0, IDC_RDO_STOP_15, OnBtnClickedStopLoss)
	ON_STN_CLICKED(IDC_STC_EXIT, &CNoWorkDlg::OnStnClickedStcExit)
END_MESSAGE_MAP()

// CNoWorkDlg 메시지 처리기입니다.

BOOL CNoWorkDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	if (!Initial_Controls())  { EndDialog(IDCANCEL); return FALSE; }
	if (!Read_StopLossList()) { EndDialog(IDCANCEL); return FALSE; }

	m_bStart = FALSE;
	m_strOperId = m_strStartTime = m_strCode = m_strTxt = "";

	return TRUE;
}

BOOL CNoWorkDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CNoWorkDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow) {
		if (m_bStart) {
			Set_NoWorkReport(TRUE);
			SetTimer(0, 500, NULL);
			m_stcTime[0].SetWindowText(m_strStartTime); 
			for (int i = 0; i < 2; i++) m_lblTime[i].ShowWindow(TRUE);
			for (int i = 0; i < 2; i++) m_stcTime[i].ShowWindow(TRUE);
		} else {
			for (int i = 0; i < 2; i++) m_lblTime[i].ShowWindow(FALSE);
			for (int i = 0; i < 2; i++) m_stcTime[i].ShowWindow(FALSE);
		}
	} else {
		KillTimer(0);
		m_bStart = FALSE;
		m_strTxt = m_strOperId = m_strCode = m_strStartTime = "";

		m_stcOperId.SetWindowText("");
		for (int i = 0; i < 3; i++)	m_stcTime[i].SetWindowText("");
		for (int i = 0; i < 16; i++) m_rdoStopReason[i].SetCheck(FALSE);
	}
}


void CNoWorkDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (m_bStart) {
		DWORD dwTerm = GetTickCount() - m_dwStartTime;

		int nHour = ( int)dwTerm / 3600000;
		int nMin  = ((int)dwTerm - nHour * 3600000) / 60000;
		int nSec  = ((int)dwTerm - nHour * 3600000 - nMin * 60000) / 1000;

		CString strTime;
		strTime.Format("%02d:%02d:%02d", nHour, nMin, nSec);
		m_stcTime[2].SetWindowText(strTime);
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CNoWorkDlg::OnStnClickedStcOperId()
{
	CString strKey;
	if (g_objCommon.Show_KeyPad(strKey) != IDOK) return;

	m_stcOperId.SetWindowText(strKey);
	m_strOperId = strKey;
}

void CNoWorkDlg::OnBtnClickedStopLoss(UINT nID)
{
	int nIndex = nID - IDC_RDO_STOP_0;

	if (m_strOperId == "") { AfxMessageBox(" ID 입력 후 선택 하십시오."); m_rdoStopReason[nIndex].SetCheck(FALSE); return; }

	CString strCode, strTxt, strTemp;

	if (nIndex > 3) {
		m_strCode = m_strLossArray[nIndex-4][0];
		m_strTxt = m_strLossArray[nIndex-4][2];
	} else {
		m_strCode = m_strStopArray[nIndex][0];
		m_strTxt = m_strStopArray[nIndex][2];
	}

	if (!m_bStart) {
		CTime CurTime = CTime::GetCurrentTime();
		m_dwStartTime = GetTickCount();
		m_strStartTime.Format("%04d%02d%02d%02d%02d%02d", CurTime.GetYear(), CurTime.GetMonth(), CurTime.GetDay(), CurTime.GetHour(), CurTime.GetMinute(), CurTime.GetSecond());

		m_bStart = TRUE;
		SetTimer(0, 500, NULL);
		Set_NoWorkReport(TRUE);

		CString strTemp;
		strTemp.Format("[IDLE] \r\n %s \r\n %s", m_strCode, m_strTxt);
		g_objCommon.Show_MsgBox(3, strTemp);	// Moadl로 MSG박스 띄움

	} else {
		Set_NoWorkReport(FALSE);
		g_dlgNoWork.ShowWindow(FALSE);
	}

}

void CNoWorkDlg::OnStnClickedStcExit()
{
	if (m_strCode == "" && m_bStart) { AfxMessageBox(" 비가동 사유를 선택하십시오."); return; }

	if (g_dlgNoWork.IsWindowVisible()) g_dlgNoWork.ShowWindow(SW_HIDE);
}

///////////////////////////////////////////////////////////////////////////////

BOOL CNoWorkDlg::Initial_Controls() 
{
	CIniFileCS INI(gsCurrentDir + "\\System\\StopLoss.ini");
	if (!INI.Check_File()) { AfxMessageBox("StopLoss.ini File Not Found!!!"); return FALSE; }

	CString strRead, strKey, strCode, strTxt, strTemp;
	int nTemp;
	char chSep = ',';

	m_LabelTitle.Init_Ctrl("Arial", 30, TRUE,RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x00, 0xFF));
	for (int i = 0; i < 2; i ++) m_group[i].Init_Ctrl("Arial", 20, TRUE, RGB(0x10, 0x10, 0xD0), COLOR_DEFAULT);
	//버튼식으로 변경 중
	for (int i = 0; i < 16; i ++) m_rdoStopReason[i].Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0x00, 0x00), COLOR_DEFAULT, 0, CRadioCS::emLeft);

	m_lblOperId.Init_Ctrl("Arial", 13, FALSE, RGB(0x10, 0x10, 0xD0), COLOR_DEFAULT);
	m_stcOperId.Init_Ctrl("Segoe UI", 13, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xFF, 0xFF, 0xFF));
	for (int i = 0; i < 3; i++) m_stcTime[i].Init_Ctrl("Segoe UI", 13, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xFF, 0xFF, 0xFF));
	for (int i = 0; i < 3; i++) m_lblTime[i].Init_Ctrl("Arial", 13, FALSE, RGB(0x10, 0x10, 0xD0), COLOR_DEFAULT);
	m_stcExit.Init_Ctrl("Arial", 12, TRUE,RGB(0xFF, 0xFF, 0xFF), RGB(0xF0, 0x60, 0xF0));

	for (int i = 0; i < 12 ; i++) {
		strKey.Format("%02d", i);

		if (i < 4) {
			strRead=INI.Get_String("NOWORK_STOP", strKey, "");
			if (strRead == "") continue;
			AfxExtractSubString(strCode, strRead, 0, chSep);
			AfxExtractSubString(strTxt, strRead, 1, chSep);

			nTemp=strTxt.Find(" ");
			strTemp = (nTemp == -1 ? strTxt : strTxt.Mid(0, nTemp));

			m_rdoStopReason[i].SetWindowText(strTemp);
			if (strTemp == "Buffer") m_rdoStopReason[i].EnableWindow(FALSE);
		}

		strRead=INI.Get_String("NOWORK_LOSS", strKey, "");
		if (strRead == "") break;

		AfxExtractSubString(strCode, strRead, 0, chSep);
		AfxExtractSubString(strTxt, strRead, 1, chSep);

		nTemp = strTxt.Find(" ");
		strTemp = (nTemp == -1 ? strTxt : strTxt.Mid(0, nTemp));

		m_rdoStopReason[i + 4].SetWindowText(strTemp);
		if (strTemp == "Buffer") m_rdoStopReason[i + 4].EnableWindow(FALSE);
	}
	return TRUE;
}

void CNoWorkDlg::Set_NoWorkReport(BOOL bSet)
{
	if (!m_bStart) return;

	CString strTime, strLog, strCode, strOper;
	CString strSet = (bSet ? "SET" : "RESET");

	strOper = m_strOperId;
	strCode = m_strCode;

	if (m_strOperId == "") strOper = "OPER_ID";
	if (m_strCode == "") strCode = "N00003";

	strLog.Format("Idle Report %s : %s,%s,%s,%s", strSet, gData.sOperID, m_strStartTime, m_strCode, m_strTxt);
	g_objLogFile.Save_HandlerLog(strLog);

}

BOOL CNoWorkDlg::Read_StopLossList()
{
	//초기화
	for (int i = 0; i < 12; i++) {
		if (i < 4) m_strStopArray[i][0] = m_strStopArray[i][1] = m_strStopArray[i][2] = "";
		m_strLossArray[i][0] = m_strLossArray[i][1] = m_strLossArray[i][2] = "";
	}

	CIniFileCS INI(gsCurrentDir + "\\System\\StopLoss.ini");
	if (!INI.Check_File()) { AfxMessageBox("StopLoss.ini File Not Found!!!"); return FALSE; }

	CString	strRead, strCode, strTxt, strMsg, strTemp, strKey;
	char chSep = ',';

	for (int i = 0; i < 4; i++) {
		strKey.Format("%02d", i);
		strRead=INI.Get_String("NOWORK_STOP", strKey, "");

		if (strRead == "") break;

		AfxExtractSubString(strCode, strRead, 0, chSep);
		AfxExtractSubString(strTxt, strRead, 1, chSep);
		AfxExtractSubString(strMsg, strRead, 2, chSep);

		m_strStopArray[i][0] = strCode;
		m_strStopArray[i][1] = strTxt;
		m_strStopArray[i][2] = strMsg;
	}

	for (int i = 0; i < 12; i++) {
		strKey.Format("%02d", i);
		strRead=INI.Get_String("NOWORK_LOSS", strKey, "");

		if (strRead == "") break;

		AfxExtractSubString(strCode, strRead, 0, chSep);
		AfxExtractSubString(strTxt, strRead, 1, chSep);
		AfxExtractSubString(strMsg, strRead, 2, chSep);

		m_strLossArray[i][0] = strCode;
		m_strLossArray[i][1] = strTxt;
		m_strLossArray[i][2] = strMsg;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
