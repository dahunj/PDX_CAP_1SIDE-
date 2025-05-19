// MonitorMotionTab1Dlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CME8000.h"
#include "MonitorMotionTabDlg.h"
#include "afxdialogex.h"

#include "LogFile.h"
#include "AJinAXL.h"

// CMonitorMotionTabDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMonitorMotionTabDlg, CDialogEx)

CMonitorMotionTabDlg::CMonitorMotionTabDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMonitorMotionTabDlg::IDD, pParent)
{
}

CMonitorMotionTabDlg::~CMonitorMotionTabDlg()
{
}

void CMonitorMotionTabDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GROUP_0, m_Group);
	for (int i = 0; i < 11; i++) DDX_Control(pDX, IDC_LABEL_0 + i, m_Label[i]);
	for (int i = 0; i < 16; i++) DDX_Control(pDX, IDC_STC_AXIS_NAME_0 + i, m_stcAxisName[i]);
	for (int i = 0; i < 16; i++) DDX_Control(pDX, IDC_STC_AXIS_POS_0 + i, m_stcAxisPos[i]);
	for (int i = 0; i < 16; i++) DDX_Control(pDX, IDC_STC_AXIS_VEL_0 + i, m_stcAxisVel[i]);
	for (int i = 0; i < 16; i++) DDX_Control(pDX, IDC_LED_AXIS_SON_0 + i, m_ledAxisSOn[i]);
	for (int i = 0; i < 16; i++) DDX_Control(pDX, IDC_LED_AXIS_ORG_0 + i, m_ledAxisOrg[i]);
	for (int i = 0; i < 16; i++) DDX_Control(pDX, IDC_LED_AXIS_ELP_0 + i, m_ledAxisElp[i]);
	for (int i = 0; i < 16; i++) DDX_Control(pDX, IDC_LED_AXIS_ELN_0 + i, m_ledAxisEln[i]);
	for (int i = 0; i < 16; i++) DDX_Control(pDX, IDC_LED_AXIS_ALM_0 + i, m_ledAxisAlm[i]);
	for (int i = 0; i < 16; i++) DDX_Control(pDX, IDC_LED_AXIS_INP_0 + i, m_ledAxisInP[i]);
	for (int i = 0; i < 16; i++) DDX_Control(pDX, IDC_LED_AXIS_RUN_0 + i, m_ledAxisRun[i]);
	for (int i = 0; i < 16; i++) DDX_Control(pDX, IDC_LED_AXIS_HOM_0 + i, m_ledAxisHom[i]);
}

BEGIN_MESSAGE_MAP(CMonitorMotionTabDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

// CMonitorMotionTabDlg 메시지 처리기입니다.

BOOL CMonitorMotionTabDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(NULL, 5, 60, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	Initial_Controls();

	m_nMotionTab = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CMonitorMotionTabDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CMonitorMotionTabDlg::OnDestroy() 
{
	CDialogEx::OnDestroy();
}

void CMonitorMotionTabDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (!bShow) return;

	int nMaxAxis = (m_nMotionTab == 2 ? 3 : 16);
	int nStartAx = m_nMotionTab * 16;

	for (int i = 0; i < 16; i++) {
		BOOL bVisible = (i < nMaxAxis ? TRUE : FALSE);
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

		if (!bVisible) continue;

		CString strName = g_objAJinAXL.Get_AxisName(nStartAx + i);
		m_stcAxisName[i].SetWindowText(" " + strName);

		g_objAJinAXL.Use_OrgAxis(nStartAx + i) ? m_ledAxisOrg[i].ShowWindow(SW_SHOW) : m_ledAxisOrg[i].ShowWindow(SW_HIDE);
		g_objAJinAXL.Use_ElpAxis(nStartAx + i) ? m_ledAxisElp[i].ShowWindow(SW_SHOW) : m_ledAxisElp[i].ShowWindow(SW_HIDE);
		g_objAJinAXL.Use_ElnAxis(nStartAx + i) ? m_ledAxisEln[i].ShowWindow(SW_SHOW) : m_ledAxisEln[i].ShowWindow(SW_HIDE);
	}

	m_strLog.Format("[Monitor Motion] Show Window - Tab (%d)", m_nMotionTab);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

///////////////////////////////////////////////////////////////////////////////
// User Functions

void CMonitorMotionTabDlg::Initial_Controls() 
{
	m_Group.Init_Ctrl("바탕", 12, TRUE, RGB(0x10, 0x50, 0x40), COLOR_DEFAULT);
	for (int i = 0; i < 11; i++) m_Label[i].Init_Ctrl("바탕", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x60, 0x60, 0x60));

	for (int i = 0; i < 16; i++) m_stcAxisName[i].Init_Ctrl("바탕", 10, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x80, 0x00, 0x00));
	for (int i = 0; i < 16; i++) m_stcAxisPos[i].Init_Ctrl("바탕", 12, TRUE, RGB(0xFF, 0x00, 0x00), RGB(0xFF, 0xFF, 0xE0));
	for (int i = 0; i < 16; i++) m_stcAxisVel[i].Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0x00, 0xFF), RGB(0xFF, 0xFF, 0xE0));

	for (int i = 0; i < 16; i++) m_ledAxisSOn[i].Init_Ctrl("바탕", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i < 16; i++) m_ledAxisOrg[i].Init_Ctrl("바탕", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i < 16; i++) m_ledAxisElp[i].Init_Ctrl("바탕", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i < 16; i++) m_ledAxisEln[i].Init_Ctrl("바탕", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i < 16; i++) m_ledAxisAlm[i].Init_Ctrl("바탕", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i < 16; i++) m_ledAxisInP[i].Init_Ctrl("바탕", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i < 16; i++) m_ledAxisRun[i].Init_Ctrl("바탕", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i < 16; i++) m_ledAxisHom[i].Init_Ctrl("바탕", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
}

void CMonitorMotionTabDlg::Display_Status()
{
	CString strPos, strVel;

	int nMaxAxis = (m_nMotionTab == 2 ? 3 : 16);
	int nStartAx = m_nMotionTab * 16;

	for (int i = 0; i < nMaxAxis; i++) {
		AXIS_STATUS *pStatus = g_objAJinAXL.Get_pStatus(nStartAx + i);

		strPos.Format("%0.3lf", pStatus->dPos);
		m_stcAxisPos[i].SetWindowText(strPos);

		strVel.Format("%0.3lf", pStatus->dVel);
		m_stcAxisVel[i].SetWindowText(strVel);

		m_ledAxisSOn[i].Set_On(pStatus->bSOn);
		m_ledAxisOrg[i].Set_On(pStatus->bOrg);
		m_ledAxisElp[i].Set_On(pStatus->bELP);
		m_ledAxisEln[i].Set_On(pStatus->bELN);
		m_ledAxisAlm[i].Set_On(pStatus->bALM);
		m_ledAxisInP[i].Set_On(pStatus->bInP);
		m_ledAxisRun[i].Set_On(pStatus->bRun);
		m_ledAxisHom[i].Set_On(pStatus->bHom);
	}
}

///////////////////////////////////////////////////////////////////////////////
