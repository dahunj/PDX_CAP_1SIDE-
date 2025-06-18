// SetupMotionTabDlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CME8000.h"
#include "SetupMotionTabDlg.h"
#include "afxdialogex.h"

#include "LogFile.h"
#include "AJinAXL.h"
#include "Common.h"

#include "SetupDlg.h"

// CSetupMotionTabDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSetupMotionTabDlg, CDialogEx)

CSetupMotionTabDlg::CSetupMotionTabDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetupMotionTabDlg::IDD, pParent)
{
}

CSetupMotionTabDlg::~CSetupMotionTabDlg()
{
}

void CSetupMotionTabDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for (int i = 0; i < 10; i++) DDX_Control(pDX, IDC_GROUP_0 + i, m_Group[i]);
	for (int i = 0; i < 62; i++) DDX_Control(pDX, IDC_LABEL_0 + i, m_Label[i]);
	for (int i = 0; i < 8; i++) DDX_Control(pDX, IDC_STC_AXIS_NAME_0 + i, m_stcAxisName[i]);
	for (int i = 0; i < 8; i++) DDX_Control(pDX, IDC_STC_AXIS_POS_0 + i, m_stcAxisPos[i]);
	for (int i = 0; i < 8; i++) DDX_Control(pDX, IDC_STC_AXIS_VEL_0 + i, m_stcAxisVel[i]);
	for (int i = 0; i < 8; i++) DDX_Control(pDX, IDC_LED_AXIS_SON_0 + i, m_ledAxisSOn[i]);
	for (int i = 0; i < 8; i++) DDX_Control(pDX, IDC_LED_AXIS_ORG_0 + i, m_ledAxisOrg[i]);
	for (int i = 0; i < 8; i++) DDX_Control(pDX, IDC_LED_AXIS_ELP_0 + i, m_ledAxisElp[i]);
	for (int i = 0; i < 8; i++) DDX_Control(pDX, IDC_LED_AXIS_ELN_0 + i, m_ledAxisEln[i]);
	for (int i = 0; i < 8; i++) DDX_Control(pDX, IDC_LED_AXIS_ALM_0 + i, m_ledAxisAlm[i]);
	for (int i = 0; i < 8; i++) DDX_Control(pDX, IDC_LED_AXIS_INP_0 + i, m_ledAxisInP[i]);
	for (int i = 0; i < 8; i++) DDX_Control(pDX, IDC_LED_AXIS_RUN_0 + i, m_ledAxisRun[i]);
	for (int i = 0; i < 8; i++) DDX_Control(pDX, IDC_LED_AXIS_HOM_0 + i, m_ledAxisHom[i]);
	for (int i = 0; i < 8; i++) DDX_Control(pDX, IDC_CHK_S_ON_0 + i, m_chkSOn[i]);
	for (int i = 0; i < 8; i++) DDX_Control(pDX, IDC_BTN_RESET_0 + i, m_btnReset[i]);
	for (int i = 0; i < 8; i++) DDX_Control(pDX, IDC_BTN_HOME_0 + i, m_btnHome[i]);
	for (int i = 0; i < 8; i++) DDX_Control(pDX, IDC_BTN_STOP_0 + i, m_btnStop[i]);
	for (int i = 0; i < 8; i++) DDX_Control(pDX, IDC_BTN_JOG_P_0 + i, m_btnJogP[i]);
	for (int i = 0; i < 8; i++) DDX_Control(pDX, IDC_BTN_JOG_N_0 + i, m_btnJogN[i]);
	for (int i = 0; i < 8; i++) DDX_Control(pDX, IDC_STC_ABS_DIST_0 + i, m_stcAbsDist[i]);
	for (int i = 0; i < 8; i++) DDX_Control(pDX, IDC_BTN_ABS_MOVE_0 + i, m_btnAbsMove[i]);
	for (int i = 0; i < 8; i++) DDX_Control(pDX, IDC_STC_REL_DIST_0 + i, m_stcRelDist[i]);
	for (int i = 0; i < 8; i++) DDX_Control(pDX, IDC_BTN_REL_MOVE_P_0 + i, m_btnRelMoveP[i]);
	for (int i = 0; i < 8; i++) DDX_Control(pDX, IDC_BTN_REL_MOVE_N_0 + i, m_btnRelMoveN[i]);
	for (int i = 0; i < 8; i++) DDX_Control(pDX, IDC_STC_SPEED_M_0 + i, m_stcSpeedM[i]);
	for (int i = 0; i < 8; i++) DDX_Control(pDX, IDC_STC_SPEED_J_0 + i, m_stcSpeedJ[i]);
	for (int i = 0; i < 8; i++) DDX_Control(pDX, IDC_STC_ACCEL_0 + i, m_stcAccel[i]);
}

BEGIN_MESSAGE_MAP(CSetupMotionTabDlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHK_S_ON_0, IDC_CHK_S_ON_7, OnChkSOnClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_RESET_0, IDC_BTN_RESET_7, OnBtnResetClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_HOME_0, IDC_BTN_HOME_7, OnBtnHomeClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_STOP_0, IDC_BTN_STOP_7, OnBtnStopClick)
	ON_MESSAGE(UM_CS_LBUTTON_DOWN, OnCsLButtonDown)
	ON_MESSAGE(UM_CS_LBUTTON_UP, OnCsLButtonUp)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_ABS_DIST_0, IDC_STC_ABS_DIST_7, OnStcAbsDistClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_ABS_MOVE_0, IDC_BTN_ABS_MOVE_7, OnBtnAbsMoveClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_REL_DIST_0, IDC_STC_REL_DIST_7, OnStcRelDistClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_REL_MOVE_P_0, IDC_BTN_REL_MOVE_P_7, OnBtnRelMovePClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_REL_MOVE_N_0, IDC_BTN_REL_MOVE_N_7, OnBtnRelMoveNClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_SPEED_M_0, IDC_STC_SPEED_M_7, OnStcSpeedMClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_SPEED_J_0, IDC_STC_SPEED_J_7, OnStcSpeedJClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_ACCEL_0, IDC_STC_ACCEL_7, OnStcAccelClick)
END_MESSAGE_MAP()

// CSetupMotionTabDlg 메시지 처리기입니다.

BOOL CSetupMotionTabDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(NULL, 5, 65, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	Initial_Controls();

	m_nMotionTab = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CSetupMotionTabDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CSetupMotionTabDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (!bShow) return;

	int nMaxAxis = (m_nMotionTab == 4 ? 3 : 8);
	int nStartAx = m_nMotionTab * 8;

	BOOL bVisible = (nMaxAxis < 4 ? FALSE : TRUE);
	m_Group[1].ShowWindow(bVisible);
	for (int i = 0; i < 11; i++) m_Label[i + 11].ShowWindow(bVisible);
	
	for (int i = 0; i < 8; i++) {
		bVisible = (i < nMaxAxis ? TRUE : FALSE);
		if (m_nMotionTab == 0 && i == 3) bVisible = FALSE;

		m_stcAxisName[i].ShowWindow(bVisible);
		m_stcAxisPos[i].ShowWindow(bVisible);
		m_stcAxisVel[i].ShowWindow(bVisible);
		m_ledAxisSOn[i].ShowWindow(bVisible);
		m_ledAxisOrg[i].ShowWindow(bVisible);
		m_ledAxisElp[i].ShowWindow(bVisible);
		m_ledAxisEln[i].ShowWindow(bVisible);
		m_ledAxisAlm[i].ShowWindow(bVisible);
		m_ledAxisInP[i].ShowWindow(bVisible);
		m_ledAxisRun[i].ShowWindow(bVisible);
		m_ledAxisHom[i].ShowWindow(bVisible);

		m_Group[i + 2].ShowWindow(bVisible);
		for (int j = 0; j < 5; j++) m_Label[i * 5 + j + 22].ShowWindow(bVisible);

		m_chkSOn[i].ShowWindow(bVisible);
		m_btnReset[i].ShowWindow(bVisible);
		m_btnHome[i].ShowWindow(bVisible);
		m_btnStop[i].ShowWindow(bVisible);
		m_btnJogP[i].ShowWindow(bVisible);
		m_btnJogN[i].ShowWindow(bVisible);
		m_stcAbsDist[i].ShowWindow(bVisible);
		m_btnAbsMove[i].ShowWindow(bVisible);
		m_stcRelDist[i].ShowWindow(bVisible);
		m_btnRelMoveP[i].ShowWindow(bVisible);
		m_btnRelMoveN[i].ShowWindow(bVisible);
		m_stcSpeedM[i].ShowWindow(bVisible);
		m_stcSpeedJ[i].ShowWindow(bVisible);
		m_stcAccel[i].ShowWindow(bVisible);

		if (!bVisible) continue;

		CString strName, strText;

		strName = g_objAJinAXL.Get_AxisName(nStartAx + i);
		m_stcAxisName[i].SetWindowText(" " + strName);
		m_Group[i + 2].SetWindowText(strName);

		g_objAJinAXL.Use_OrgAxis(nStartAx + i) ? m_ledAxisOrg[i].ShowWindow(SW_SHOW) : m_ledAxisOrg[i].ShowWindow(SW_HIDE);
		g_objAJinAXL.Use_ElpAxis(nStartAx + i) ? m_ledAxisElp[i].ShowWindow(SW_SHOW) : m_ledAxisElp[i].ShowWindow(SW_HIDE);
		g_objAJinAXL.Use_ElnAxis(nStartAx + i) ? m_ledAxisEln[i].ShowWindow(SW_SHOW) : m_ledAxisEln[i].ShowWindow(SW_HIDE);

		g_objAJinAXL.Get_ServoOn(nStartAx + i) ? m_chkSOn[i].SetCheck(TRUE) : m_chkSOn[i].SetCheck(FALSE);

		m_stcAbsDist[i].SetWindowText("0.000");
		m_stcRelDist[i].SetWindowText("0.000");

		AXIS_PARAM *pParam = g_objAJinAXL.Get_pParam(nStartAx + i);
		strText.Format("%0.1lf", pParam->dSpeedM);
		m_stcSpeedM[i].SetWindowText(strText);

		strText.Format("%0.1lf", pParam->dSpeedJ);
		m_stcSpeedJ[i].SetWindowText(strText);			

		strText.Format("%0.1lf", pParam->dAccel);
		m_stcAccel[i].SetWindowText(strText);

		if (g_dlgSetup.Get_LoginUser() == 2) {	// SI User
			m_stcAbsDist[i].EnableWindow(TRUE);
			m_btnAbsMove[i].EnableWindow(TRUE);
			m_btnJogP[i].EnableWindow(TRUE);
			m_btnJogN[i].EnableWindow(TRUE);
		} else {
			m_stcAbsDist[i].EnableWindow(FALSE);
			m_btnAbsMove[i].EnableWindow(FALSE);
			m_btnJogP[i].EnableWindow(FALSE);
			m_btnJogN[i].EnableWindow(FALSE);
		}
	}

	m_strLog.Format("[Setup Motion] Show Window - Tab (%d)", m_nMotionTab);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CSetupMotionTabDlg::OnChkSOnClick(UINT nID)
{
	int ID = nID - IDC_CHK_S_ON_0;
	int nStartAx = m_nMotionTab * 8;

	if (m_chkSOn[ID].GetCheck()) g_objAJinAXL.Set_ServoOn(nStartAx + ID);
	else g_objAJinAXL.Set_ServoOff(nStartAx + ID);

	m_strLog.Format("[Setup Motion] Servo On/Off - %s", g_objAJinAXL.Get_AxisName(nStartAx + ID));
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CSetupMotionTabDlg::OnBtnResetClick(UINT nID)
{
	int ID = nID - IDC_BTN_RESET_0;
	int nStartAx = m_nMotionTab * 8;

	g_objAJinAXL.Stop_Motion(nStartAx + ID);
	g_objAJinAXL.Alarm_Reset(nStartAx + ID);

	m_strLog.Format("[Setup Motion] Reset - %s", g_objAJinAXL.Get_AxisName(nStartAx + ID));
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CSetupMotionTabDlg::OnBtnHomeClick(UINT nID)
{
	int ID = nID - IDC_BTN_HOME_0;
	int nStartAx = m_nMotionTab * 8;

	if (nStartAx + ID == 4 || nStartAx + ID == 6) {	// Load Stage1/2 X Axis Tray 유무 확인
		DX_DATA_04 *pDX04 = g_objAJinAXL.Get_pDX04();
		if (pDX04->iLoadStage1Exist || pDX04->iLoadStage2Exist) {
			AfxMessageBox("Load Stage1/2번에 Tray가 있습니다. Tray 제거 후 진행하세요."); return;
		}
	}
	g_objAJinAXL.Home_Search(nStartAx + ID);

	m_strLog.Format("[Setup Motion] Homing - %s", g_objAJinAXL.Get_AxisName(nStartAx + ID));
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CSetupMotionTabDlg::OnBtnStopClick(UINT nID)
{
	int ID = nID - IDC_BTN_STOP_0;
	int nStartAx = m_nMotionTab * 8;

	g_objAJinAXL.Stop_Motion(nStartAx + ID);

	m_strLog.Format("[Setup Motion] Stop - %s", g_objAJinAXL.Get_AxisName(nStartAx + ID));
	g_objLogFile.Save_HandlerLog(m_strLog);
}

LRESULT CSetupMotionTabDlg::OnCsLButtonDown(WPARAM wParam, LPARAM lParam)
{
	UINT nID = (int)wParam;
	int nStartAx = m_nMotionTab * 8;

	if (nID >= IDC_BTN_JOG_P_0 && nID <= IDC_BTN_JOG_P_7) {			// Jog +
		int ID = nID - IDC_BTN_JOG_P_0;
		g_objAJinAXL.Jog_Positive(nStartAx + ID);
		m_strLog.Format("[Setup Motion] Jog Positive - %s", g_objAJinAXL.Get_AxisName(nStartAx + ID));

	} else if (nID >= IDC_BTN_JOG_N_0 && nID <= IDC_BTN_JOG_N_7) {	// Jog -
		int ID = nID - IDC_BTN_JOG_N_0;
		g_objAJinAXL.Jog_Negative(nStartAx + ID);
		m_strLog.Format("[Setup Motion] Jog Negative - %s", g_objAJinAXL.Get_AxisName(nStartAx + ID));
	}

	g_objLogFile.Save_HandlerLog(m_strLog);
	return 0;
}

LRESULT CSetupMotionTabDlg::OnCsLButtonUp(WPARAM wParam, LPARAM lParam)
{
	UINT nID = (int)wParam;
	int nStartAx = m_nMotionTab * 8;

	if (nID >= IDC_BTN_JOG_P_0 && nID <= IDC_BTN_JOG_P_7) {			// Jog +
		int ID = nID - IDC_BTN_JOG_P_0;
		g_objAJinAXL.Stop_Motion(nStartAx + ID);
		m_strLog.Format("[Setup Motion] Stop Positive - %s", g_objAJinAXL.Get_AxisName(nStartAx + ID));

	} else if (nID >= IDC_BTN_JOG_N_0 && nID <= IDC_BTN_JOG_N_7) {	// Jog -
		int ID = nID - IDC_BTN_JOG_N_0;
		g_objAJinAXL.Stop_Motion(nStartAx + ID);
		m_strLog.Format("[Setup Motion] Stop Negative - %s", g_objAJinAXL.Get_AxisName(nStartAx + ID));
	}

	g_objLogFile.Save_HandlerLog(m_strLog);
	return 0;
}

void CSetupMotionTabDlg::OnStcAbsDistClick(UINT nID)
{
	int ID = nID - IDC_STC_ABS_DIST_0;
	CString strOld, strNew;
	m_stcAbsDist[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	m_stcAbsDist[ID].SetWindowText(strNew);
}

void CSetupMotionTabDlg::OnBtnAbsMoveClick(UINT nID)
{
	int ID = nID - IDC_BTN_ABS_MOVE_0;
	int nStartAx = m_nMotionTab * 8;

	CString strText;
	m_stcAbsDist[ID].GetWindowText(strText);
	double dDist = atof(strText);

	if (Check_Interlock(nStartAx + ID) == FALSE) return;

	g_objAJinAXL.Move_Absolute(nStartAx + ID, dDist);

	m_strLog.Format("[Setup Motion] Move Absolute - %s, %0.3lf", g_objAJinAXL.Get_AxisName(nStartAx + ID), dDist);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CSetupMotionTabDlg::OnStcRelDistClick(UINT nID)
{
	int ID = nID - IDC_STC_REL_DIST_0;
	int nStartAx = m_nMotionTab * 8;

	CString strOld, strNew;
	m_stcRelDist[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	if (g_dlgSetup.Get_LoginUser() != 2) {
		double dNew = atof(strNew);
		if (dNew > 5.0) strNew = strOld;
	}
	m_stcRelDist[ID].SetWindowText(strNew);
}

void CSetupMotionTabDlg::OnBtnRelMovePClick(UINT nID)
{
	int ID = nID - IDC_BTN_REL_MOVE_P_0;
	int nStartAx = m_nMotionTab * 8;

	CString strText;
	m_stcRelDist[ID].GetWindowText(strText);
	double dDist = atof(strText);

	g_objAJinAXL.Move_Relative(nStartAx + ID, dDist);

	m_strLog.Format("[Setup Motion] Move Relative(+) - %s, %0.3lf", g_objAJinAXL.Get_AxisName(nStartAx + ID), dDist);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CSetupMotionTabDlg::OnBtnRelMoveNClick(UINT nID)
{
	int ID = nID - IDC_BTN_REL_MOVE_N_0;
	int nStartAx = m_nMotionTab * 8;

	CString strText;
	m_stcRelDist[ID].GetWindowText(strText);
	double dDist = atof(strText) * -1.0;

	g_objAJinAXL.Move_Relative(nStartAx + ID, dDist);

	m_strLog.Format("[Setup Motion] Move Relative(-) - %s, %0.3lf", g_objAJinAXL.Get_AxisName(nStartAx + ID), dDist);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CSetupMotionTabDlg::OnStcSpeedMClick(UINT nID)
{
	int ID = nID - IDC_STC_SPEED_M_0;

	CString strOld, strNew;
	m_stcSpeedM[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	m_stcSpeedM[ID].SetWindowText(strNew);
}

void CSetupMotionTabDlg::OnStcSpeedJClick(UINT nID)
{
	int ID = nID - IDC_STC_SPEED_J_0;

	CString strOld, strNew;
	m_stcSpeedJ[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	m_stcSpeedJ[ID].SetWindowText(strNew);
}

void CSetupMotionTabDlg::OnStcAccelClick(UINT nID)
{
	int ID = nID - IDC_STC_ACCEL_0;

	CString strOld, strNew;
	m_stcAccel[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	m_stcAccel[ID].SetWindowText(strNew);
}

///////////////////////////////////////////////////////////////////////////////
// User Functions

void CSetupMotionTabDlg::Initial_Controls() 
{
	for (int i = 0; i < 10; i++) m_Group[i].Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0x00, 0xFF), COLOR_DEFAULT);
	for (int i = 0; i < 62; i++) m_Label[i].Init_Ctrl("바탕", 10, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x60, 0x60, 0x60));

	for (int i = 0; i < 8; i++) m_stcAxisName[i].Init_Ctrl("바탕", 10, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x80, 0x00, 0x00));
	for (int i = 0; i < 8; i++) m_stcAxisPos[i].Init_Ctrl("바탕", 11, TRUE, RGB(0xFF, 0x00, 0x00), RGB(0xE0, 0xE0, 0xE0));
	for (int i = 0; i < 8; i++) m_stcAxisVel[i].Init_Ctrl("바탕", 11, TRUE, RGB(0x00, 0x00, 0xFF), RGB(0xE0, 0xE0, 0xE0));

	for (int i = 0; i < 8; i++) m_ledAxisSOn[i].Init_Ctrl(RGB(0x00, 0xFF, 0x00), RGB(0x40, 0x40, 0x40));
	for (int i = 0; i < 8; i++) m_ledAxisOrg[i].Init_Ctrl(RGB(0x00, 0xFF, 0x00), RGB(0x40, 0x40, 0x40));
	for (int i = 0; i < 8; i++) m_ledAxisElp[i].Init_Ctrl(RGB(0x00, 0xFF, 0x00), RGB(0x40, 0x40, 0x40));
	for (int i = 0; i < 8; i++) m_ledAxisEln[i].Init_Ctrl(RGB(0x00, 0xFF, 0x00), RGB(0x40, 0x40, 0x40));
	for (int i = 0; i < 8; i++) m_ledAxisAlm[i].Init_Ctrl(RGB(0x00, 0xFF, 0x00), RGB(0x40, 0x40, 0x40));
	for (int i = 0; i < 8; i++) m_ledAxisInP[i].Init_Ctrl(RGB(0x00, 0xFF, 0x00), RGB(0x40, 0x40, 0x40));
	for (int i = 0; i < 8; i++) m_ledAxisRun[i].Init_Ctrl(RGB(0x00, 0xFF, 0x00), RGB(0x40, 0x40, 0x40));
	for (int i = 0; i < 8; i++) m_ledAxisHom[i].Init_Ctrl(RGB(0x00, 0xFF, 0x00), RGB(0x40, 0x40, 0x40));

	for (int i = 0; i < 8; i++) m_chkSOn[i].Init_Ctrl("바탕", 12, TRUE, RGB(0xFF, 0x63, 0x47), COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 8; i++) m_btnReset[i].Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0x80, 0x00), COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 8; i++) m_btnHome[i].Init_Ctrl("바탕", 12, TRUE, RGB(0x22, 0x8B, 0x22), COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 8; i++) m_btnStop[i].Init_Ctrl("바탕", 12, TRUE, RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 8; i++) m_btnJogP[i].Init_Ctrl("바탕", 12, TRUE, RGB(0x80, 0x00, 0x80), COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 8; i++) m_btnJogN[i].Init_Ctrl("바탕", 12, TRUE, RGB(0x80, 0x00, 0x80), COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 8; i++) m_stcAbsDist[i].Init_Ctrl("바탕", 11, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xFF, 0xFF, 0xC0));
	for (int i = 0; i < 8; i++) m_btnAbsMove[i].Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0x00, 0xFF), COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 8; i++) m_stcRelDist[i].Init_Ctrl("바탕", 11, TRUE, RGB(0x00, 0x00, 0x00), RGB(0x80, 0xFF, 0xC0));
	for (int i = 0; i < 8; i++) m_btnRelMoveP[i].Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0x00, 0xFF), COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 8; i++) m_btnRelMoveN[i].Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0x00, 0xFF), COLOR_DEFAULT, 0, 0);

	for (int i = 0; i < 8; i++) m_stcSpeedM[i].Init_Ctrl("바탕", 11, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xFF, 0xEF, 0xD5));
	for (int i = 0; i < 8; i++) m_stcSpeedJ[i].Init_Ctrl("바탕", 11, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xFF, 0xEF, 0xD5));
	for (int i = 0; i < 8; i++) m_stcAccel[i].Init_Ctrl("바탕", 11, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xC0, 0xFF, 0xF0));
}

void CSetupMotionTabDlg::Display_Status()
{
	int nMaxAxis = (m_nMotionTab == 4 ? 3 : 8);
	int nStartAx = m_nMotionTab * 8;

	CString strPos, strVel;

	for (int i = 0; i < nMaxAxis; i++) {
		AXIS_STATUS *pStatus = g_objAJinAXL.Get_pStatus(nStartAx + i);

		strPos.Format("%0.3lf", pStatus->dPos);
		m_stcAxisPos[i].SetWindowText(strPos);

		strVel.Format("%0.3lf", pStatus->dVel);
		m_stcAxisVel[i].SetWindowText(strVel);

		pStatus->bSOn ? m_ledAxisSOn[i].On() : m_ledAxisSOn[i].Off();
		pStatus->bOrg ? m_ledAxisOrg[i].On() : m_ledAxisOrg[i].Off();
		pStatus->bELP ? m_ledAxisElp[i].On() : m_ledAxisElp[i].Off();
		pStatus->bELN ? m_ledAxisEln[i].On() : m_ledAxisEln[i].Off();
		pStatus->bALM ? m_ledAxisAlm[i].On() : m_ledAxisAlm[i].Off();
		pStatus->bInP ? m_ledAxisInP[i].On() : m_ledAxisInP[i].Off();
		pStatus->bRun ? m_ledAxisRun[i].On() : m_ledAxisRun[i].Off();
		pStatus->bHom ? m_ledAxisHom[i].On() : m_ledAxisHom[i].Off();
	}
}

void CSetupMotionTabDlg::Save_MotionData(int nTab) 
{
	int nEnd = (nTab == 4 ? 3 : 8);
	int nStartAx = m_nMotionTab * 8;

	CString strValue;
	double dSpeedM, dSpeedJ, dAccel;
	for (int i = 0; i < nEnd; i++) {
		m_stcSpeedM[i].GetWindowText(strValue);	dSpeedM = atof(strValue);
		m_stcSpeedJ[i].GetWindowText(strValue);	dSpeedJ = atof(strValue);
		m_stcAccel[i].GetWindowText(strValue);	dAccel = atof(strValue);
		g_objAJinAXL.Save_MotionParam(nStartAx + i, dSpeedM, dSpeedJ, dAccel);
	}

	Cancel_MotionData(nTab);
}

void CSetupMotionTabDlg::Cancel_MotionData(int nTab) 
{
	int nEnd = (nTab == 4 ? 3 : 8);
	int nStartAx = m_nMotionTab * 8;

	CString strValue;
	for (int i = 0; i < nEnd; i++) {
		g_objAJinAXL.Read_MotionParam(nStartAx + i);
		AXIS_PARAM *pParam = g_objAJinAXL.Get_pParam(nStartAx + i);

		strValue.Format("%0.1lf", pParam->dSpeedM);	m_stcSpeedM[i].SetWindowText(strValue);
		strValue.Format("%0.1lf", pParam->dSpeedJ);	m_stcSpeedJ[i].SetWindowText(strValue);
		strValue.Format("%0.1lf", pParam->dAccel);	m_stcAccel[i].SetWindowText(strValue);
	}
}

///////////////////////////////////////////////////////////////////////////////

BOOL CSetupMotionTabDlg::Check_Interlock(int AxisNo)
{
	if(AxisNo == AX_TRAY_PICKER_X)
	{
		if(!g_objCommon.Check_Position(AX_TRAY_PICKER_Z, 0))
		{
			AfxMessageBox("Tray Picker Z Ready Up 상태가 아닙니다. 확인 후 진행하세요.");
			return FALSE;
		}
	}

	if(AxisNo == AX_LOAD_STAGE1_X)
	{
		double curPosZ1 = g_objAJinAXL.Get_Position(AX_LOAD_STAGE1_Z);
		double curPosZ2 = g_objAJinAXL.Get_Position(AX_LOAD_STAGE2_Z);

		if(fabs(curPosZ1 - curPosZ2) < 60) 
		{
			AfxMessageBox(_T("Load Stage 간 충돌 위험 있습니다."));
			return FALSE;
		}		

		if(!g_objCommon.Check_Position(AX_LOAD_PICKER_Z, 0))
		{
			AfxMessageBox("Load Picker Z Ready Up 상태가 아닙니다. 확인 후 진행하세요.");
			return FALSE;
		}
	}


	if(AxisNo == AX_LOAD_STAGE1_Z)
	{
		double curPosY1 = g_objAJinAXL.Get_Position(AX_LOAD_STAGE1_X);
		double curPosY2 = g_objAJinAXL.Get_Position(AX_LOAD_STAGE2_X);

		if(fabs(curPosY1 - curPosY2) < 150) 
		{
			AfxMessageBox(_T("Load Stage 간 충돌 위험 있습니다."));
			return FALSE;
		}		

		if(!g_objCommon.Check_Position(AX_LOAD_PICKER_Z, 0))
		{
			AfxMessageBox("Load Picker Z Ready Up 상태가 아닙니다. 확인 후 진행하세요.");
			return FALSE;
		}
	}

	if(AxisNo == AX_LOAD_STAGE2_Z)
	{
		double curPosY1 = g_objAJinAXL.Get_Position(AX_LOAD_STAGE1_X);
		double curPosY2 = g_objAJinAXL.Get_Position(AX_LOAD_STAGE2_X);

		if(fabs(curPosY1 - curPosY2) < 150) 
		{
			AfxMessageBox(_T("Load Stage 간 충돌 위험 있습니다."));
			return FALSE;
		}		

		if(!g_objCommon.Check_Position(AX_LOAD_PICKER_Z, 0))
		{
			AfxMessageBox("Load Picker Z Ready Up 상태가 아닙니다. 확인 후 진행하세요.");
			return FALSE;
		}
	}



	if(AxisNo == AX_LOAD_STAGE2_X)
	{
		double curPosZ1 = g_objAJinAXL.Get_Position(AX_LOAD_STAGE1_Z);
		double curPosZ2 = g_objAJinAXL.Get_Position(AX_LOAD_STAGE2_Z);

		if(fabs(curPosZ1 - curPosZ2) < 60) 
		{
			AfxMessageBox(_T("Load Stage 간 충돌 위험 있습니다."));
			return FALSE;
		}			

		if(!g_objCommon.Check_Position(AX_LOAD_PICKER_Z, 0))
		{
			AfxMessageBox("Load Picker Z Ready Up 상태가 아닙니다. 확인 후 진행하세요.");
			return FALSE;
		}		
	}

	if(AxisNo == AX_LOAD_PICKER_Y)
	{		
		if(!g_objCommon.Check_Position(AX_LOAD_PICKER_Z, 0))
		{
			AfxMessageBox("Load Picker Z Ready Up 상태가 아닙니다. 확인 후 진행하세요.");
			return FALSE;
		}		
	}

	if(AxisNo == AX_CAP_STAGE1_X)
	{
		double curPosZ1 = g_objAJinAXL.Get_Position(AX_CAP_STAGE1_Z);
		double curPosZ2 = g_objAJinAXL.Get_Position(AX_CAP_STAGE2_Z);

		if(fabs(curPosZ1 - curPosZ2) < 65) 
		{
			AfxMessageBox(_T("Cap Stage 간 충돌 위험 있습니다."));
			return FALSE;
		}		

		if(!g_objCommon.Check_Position(AX_CAP_PICKER_Z, 0))
		{
			AfxMessageBox("Cap Picker Z Ready Up 상태가 아닙니다. 확인 후 진행하세요.");
			return FALSE;
		}

	}

	if(AxisNo == AX_CAP_STAGE2_X)
	{
		double curPosZ1 = g_objAJinAXL.Get_Position(AX_CAP_STAGE1_Z);
		double curPosZ2 = g_objAJinAXL.Get_Position(AX_CAP_STAGE2_Z);

		if(fabs(curPosZ1 - curPosZ2) < 65) 
		{
			AfxMessageBox(_T("Cap Stage 간 충돌 위험 있습니다."));
			return FALSE;
		}		

		if(!g_objCommon.Check_Position(AX_CAP_PICKER_Z, 0))
		{
			AfxMessageBox("Cap Picker Z Ready Up 상태가 아닙니다. 확인 후 진행하세요.");
			return FALSE;
		}
	}



	if(AxisNo == AX_CAP_STAGE1_Z)
	{
		double curPosY1 = g_objAJinAXL.Get_Position(AX_CAP_STAGE1_X);
		double curPosY2 = g_objAJinAXL.Get_Position(AX_CAP_STAGE2_X);

		if(fabs(curPosY1 - curPosY2) < 150) 
		{
			AfxMessageBox(_T("Cap Stage 간 충돌 위험 있습니다."));
			return FALSE;
		}		

		if(!g_objCommon.Check_Position(AX_CAP_PICKER_Z, 0))
		{
			AfxMessageBox("Cap Picker Z Ready Up 상태가 아닙니다. 확인 후 진행하세요.");
			return FALSE;
		}

	}

	if(AxisNo == AX_CAP_STAGE2_Z)
	{
		double curPosY1 = g_objAJinAXL.Get_Position(AX_CAP_STAGE1_X);
		double curPosY2 = g_objAJinAXL.Get_Position(AX_CAP_STAGE2_X);

		if(fabs(curPosY1 - curPosY2) < 150) 
		{
			AfxMessageBox(_T("Cap Stage 간 충돌 위험 있습니다."));
			return FALSE;
		}		

		if(!g_objCommon.Check_Position(AX_CAP_PICKER_Z, 0))
		{
			AfxMessageBox("Cap Picker Z Ready Up 상태가 아닙니다. 확인 후 진행하세요.");
			return FALSE;
		}

	}

	if(AxisNo == AX_CAP_PICKER_Y)
	{
		if(!g_objCommon.Check_Position(AX_CAP_PICKER_Z, 0))
		{
			AfxMessageBox("Cap Picker Z Ready Up 상태가 아닙니다. 확인 후 진행하세요.");
			return FALSE;
		}
	}

	if(AxisNo == AX_ASSY_PICKER_Y)
	{
		if(!g_objCommon.Check_Position(AX_ASSY_PICKER_Z, 0))
		{
			AfxMessageBox("Assy Picker Z Ready Up 상태가 아닙니다. 확인 후 진행하세요.");
			return FALSE;
		}
	}


	if(AxisNo == AX_CAP_BUFFER_STAGE_Y)
	{		
		///////////////////////////////////////////////////////////
		if(!g_objCommon.Check_Position(AX_ASSY_PICKER_Z, 0))
		{
			AfxMessageBox("Assy Picker Z Ready Up 상태가 아닙니다. 확인 후 진행하세요.");
			return FALSE;
		}

		if(!g_objCommon.Check_Position(AX_CAP_PICKER_Z, 0))
		{
			AfxMessageBox("Cap Picker Z Ready Up 상태가 아닙니다. 확인 후 진행하세요.");
			return FALSE;
		}
	}

	if(AxisNo == AX_UNLOAD_PICKER_X)
	{
		if(!g_objCommon.Check_Position(AX_UNLOAD_PICKER_Z, 0))
		{
			AfxMessageBox("Unload Picker Z Ready Up 상태가 아닙니다. 확인 후 진행하세요.");
			return FALSE;
		}
	}

	if(AxisNo == AX_UNLOAD_STAGE1_Y)
	{
		double curPosZ1 = g_objAJinAXL.Get_Position(AX_UNLOAD_STAGE1_Z);
		double curPosZ2 = g_objAJinAXL.Get_Position(AX_UNLOAD_STAGE2_Z);

		if(fabs(curPosZ1 - curPosZ2) < 55) 
		{
			AfxMessageBox(_T("Unload Stage 간 충돌 위험 있습니다."));
			return FALSE;
		}		

		if(!g_objCommon.Check_Position(AX_UNLOAD_PICKER_Z, 0))
		{
			AfxMessageBox("Unload Picker Z Ready Up 상태가 아닙니다. 확인 후 진행하세요.");
			return FALSE;
		}
	}

	if(AxisNo == AX_UNLOAD_STAGE2_Y)
	{
		double curPosZ1 = g_objAJinAXL.Get_Position(AX_UNLOAD_STAGE1_Z);
		double curPosZ2 = g_objAJinAXL.Get_Position(AX_UNLOAD_STAGE2_Z);

		if(fabs(curPosZ1 - curPosZ2) < 55) 
		{
			AfxMessageBox(_T("Unload Stage 간 충돌 위험 있습니다."));
			return FALSE;
		}		

		if(!g_objCommon.Check_Position(AX_UNLOAD_PICKER_Z, 0))
		{
			AfxMessageBox("Unload Picker Z Ready Up 상태가 아닙니다. 확인 후 진행하세요.");
			return FALSE;
		}
	}


	if(AxisNo == AX_UNLOAD_STAGE1_Z)
	{
		double curPosY1 = g_objAJinAXL.Get_Position(AX_UNLOAD_STAGE1_Y);
		double curPosY2 = g_objAJinAXL.Get_Position(AX_UNLOAD_STAGE2_Y);

		if(fabs(curPosY1 - curPosY2) < 150) 
		{
			AfxMessageBox(_T("Unload Stage 간 충돌 위험 있습니다."));
			return FALSE;
		}		

		if(!g_objCommon.Check_Position(AX_UNLOAD_PICKER_Z, 0))
		{
			AfxMessageBox("Unload Picker Z Ready Up 상태가 아닙니다. 확인 후 진행하세요.");
			return FALSE;
		}

	}


	if(AxisNo == AX_UNLOAD_STAGE2_Z)
	{
		double curPosY1 = g_objAJinAXL.Get_Position(AX_UNLOAD_STAGE1_Y);
		double curPosY2 = g_objAJinAXL.Get_Position(AX_UNLOAD_STAGE2_Y);

		if(fabs(curPosY1 - curPosY2) < 150) 
		{
			AfxMessageBox(_T("Unload Stage 간 충돌 위험 있습니다."));
			return FALSE;
		}		

		if(!g_objCommon.Check_Position(AX_UNLOAD_PICKER_Z, 0))
		{
			AfxMessageBox("Unload Picker Z Ready Up 상태가 아닙니다. 확인 후 진행하세요.");
			return FALSE;
		}
	}
	return TRUE;
}
