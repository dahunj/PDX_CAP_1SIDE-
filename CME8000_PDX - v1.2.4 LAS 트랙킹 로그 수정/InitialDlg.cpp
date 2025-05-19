// InitialDlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CME8000.h"
#include "InitialDlg.h"
#include "afxdialogex.h"

#include "LogFile.h"
#include "Common.h"

#include "Inspector.h"
#include "AviUDP.h"
#include "SequenceInit.h"

#include "OperatorDlg.h"
#include "CME8000Dlg.h"

// CInitialDlg 대화 상자입니다.
CInitialDlg g_dlgInitial;

IMPLEMENT_DYNAMIC(CInitialDlg, CDialogEx)

CInitialDlg::CInitialDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInitialDlg::IDD, pParent)
{
}

CInitialDlg::~CInitialDlg()
{
}

void CInitialDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for (int i = 0; i < 6; i++) DDX_Control(pDX, IDC_GROUP_0 + i, m_Group[i]);
	for (int i = 0; i < 6; i++) DDX_Control(pDX, IDC_LABEL_0 + i, m_Label[i]);
	for (int i = 0; i < 46; i++) DDX_Control(pDX, IDC_STC_AXIS_NAME_0 + i, m_stcAxisName[i]); 
	for (int i = 0; i < 46; i++) DDX_Control(pDX, IDC_LED_AXIS_RUN_0 + i, m_ledAxisRun[i]);
	for (int i = 0; i < 46; i++) DDX_Control(pDX, IDC_LED_AXIS_HOM_0 + i, m_ledAxisHom[i]);
	DDX_Control(pDX, IDC_RDO_INIT_START, m_rdoInitStart);
	DDX_Control(pDX, IDC_RDO_INIT_STOP, m_rdoInitStop);
	DDX_Control(pDX, IDC_LED_INITIAL_OK, m_ledInitialOK);
	for (int i = 0; i < 3; i++) DDX_Control(pDX, IDC_IMAGE_0 + i, m_Image[i]);
	for (int i = 0; i < 8; i++) DDX_Control(pDX, IDC_PIC_INIT_FLOW_0 + i, m_picInitFlow[i]);
	for (int i = 0; i < 8; i++) DDX_Control(pDX, IDC_STC_INIT_FLOW_0 + i, m_stcInitFlow[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_LED_MAIN_AIR_0 + i, m_ledMainAir[i]);
	for (int i = 0; i < 3; i++) DDX_Control(pDX, IDC_LED_EMG_SW_0 + i, m_ledEmgSw[i]);
	for (int i = 0; i < 13; i++) DDX_Control(pDX, IDC_LED_DOOR_OPEN_0 + i, m_ledDoorOpen[i]);
	for (int i = 0; i < 14; i++) DDX_Control(pDX, IDC_STC_INIT_CASE_0 + i, m_stcInitCase[i]);
}

BEGIN_MESSAGE_MAP(CInitialDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RDO_INIT_START, &CInitialDlg::OnBnClickedRdoInitStart)
	ON_BN_CLICKED(IDC_RDO_INIT_STOP, &CInitialDlg::OnBnClickedRdoInitStop)
	ON_BN_CLICKED(IDC_BUTTON1, &CInitialDlg::OnBnClickedButton1)
END_MESSAGE_MAP()

// CInitialDlg 메시지 처리기입니다.

BOOL CInitialDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(this, 0, 75, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	Initial_Controls();

	for (int i = 0; i < 14; i++) m_stcInitCase[i].SetWindowText("000");
	for (int i = AXIS_COUNT; i < 46; i++) {
		m_stcAxisName[i].ShowWindow(FALSE);
		m_ledAxisRun[i].ShowWindow(FALSE);
		m_ledAxisHom[i].ShowWindow(FALSE);
	}

	m_bInitialRunning = FALSE;
	m_rdoInitStop.SetCheck(TRUE);
	m_rdoInitStop.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CInitialDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CInitialDlg::OnDestroy() 
{
	CDialogEx::OnDestroy();

	KillTimer(0);
}

void CInitialDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow) {
		for (int i = 0; i < AXIS_COUNT; i++) {
			CString strName = g_objAJinAXL.Get_AxisName(i);
			m_stcAxisName[i].SetWindowText(" " + strName);
		}
		g_objCommon.Locking_MainDoor(FALSE);
		SetTimer(0, 100, NULL);

	} else {
		KillTimer(0);
	}
}

void CInitialDlg::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(0);

	DX_DATA_12 *pDX12 = g_objAJinAXL.Get_pDX12();

	if (pDX12->iStartSw && !m_rdoInitStart.GetCheck()) {
		g_objLogFile.Save_HandlerLog("[Initial Mode] START S/W Push");
		m_rdoInitStart.SetCheck(TRUE);

	} else if (pDX12->iStopSw && !m_rdoInitStop.GetCheck()) {
		g_objLogFile.Save_HandlerLog("[Initial Mode] STOP S/W Push");
		m_rdoInitStop.SetCheck(TRUE);
	}

	Display_Status();

	CCME8000Dlg *pMainDlg = (CCME8000Dlg*)AfxGetApp()->GetMainWnd();

	if (m_rdoInitStart.GetCheck()) {
		if (!m_bInitialRunning) {	// First Start
			if (g_objCommon.Show_MsgBox(2, "Do you want to Initialize?") == IDOK) {
				m_bInitialRunning = TRUE;

				g_objCommon.Locking_MainDoor(TRUE);
				pMainDlg->Set_CurrentState(STATE_INIT);
				pMainDlg->Enable_ModeButton(FALSE);

				g_objInspector.Set_InitialRequest();

				m_rdoInitStart.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);
				m_rdoInitStop.Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);

				m_nBackColorLoop = 0;
				Set_StatusColor(0, 2);	// Green
				for (int i = 1; i < 8; i++) Set_StatusColor(i, 0);	// White

				g_objSequenceInit.Begin_InitialThread();

			} else {
				m_rdoInitStop.SetCheck(TRUE);
			}

		} else {					// Running
			Display_Initial();

			if (g_objSequenceInit.Get_InitComplete()) {
				m_bInitialRunning = FALSE;

				m_rdoInitStop.SetCheck(TRUE);
				m_rdoInitStart.Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
				m_rdoInitStop.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);

				for (int i = 1; i < 8; i++) Set_StatusColor(i, 2);	// Green

				g_objLogFile.Save_HandlerLog("[Initial Mode] Initialization is complete");

				g_objCommon.Show_MsgBox(1, "Initialization is complete.");
				g_objCommon.Save_MotionPos();

				g_objInspector.Set_TimeUpdate();

				pMainDlg->Set_CurrentState(STATE_STOP);
				pMainDlg->Enable_ModeButton(TRUE);
				pMainDlg->Set_CurrentMode(MODE_OPERATOR);

				g_dlgOperator.ShowWindow(SW_SHOW);
				return;

			} else if (!g_objSequenceInit.Get_ThreadInitial()) {
				g_objLogFile.Save_HandlerLog("[Initial Mode] Auto STOP");
				m_rdoInitStop.SetCheck(TRUE);
			}
		}

	} else if (m_rdoInitStop.GetCheck()) {	
		if (m_bInitialRunning) {	// First Stop
			m_bInitialRunning = FALSE;

			m_rdoInitStart.Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
			m_rdoInitStop.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);
			g_objSequenceInit.End_InitialThread();

			int nState = theApp.Get_MainState();
			if (nState != STATE_ALARM && nState != STATE_ERROR) pMainDlg->Set_CurrentState(STATE_NONE);
			pMainDlg->Enable_ModeButton(TRUE);
			g_objCommon.Locking_MainDoor(FALSE);

		} else {
			int nState = theApp.Get_MainState();
			if (nState != STATE_ERROR) g_objCommon.Check_MainEmgAir();	// Emg & Main Air
		}
	}

	SetTimer(0, 100, NULL);
	CDialogEx::OnTimer(nIDEvent);
}

void CInitialDlg::OnBnClickedRdoInitStart()
{
	g_objLogFile.Save_HandlerLog("[Initial Mode] START Button push");
}

void CInitialDlg::OnBnClickedRdoInitStop()
{
	g_objLogFile.Save_HandlerLog("[Initial Mode] STOP Button push");
}

///////////////////////////////////////////////////////////////////////////////
// User Functions

void CInitialDlg::Initial_Controls()
{
	for (int i = 0; i < 6; i++) m_Group[i].Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0x00, 0xFF), COLOR_DEFAULT);
	for (int i = 0; i < 6; i++) m_Label[i].Init_Ctrl("바탕", 11, FALSE, RGB(0xFF, 0xFF, 0xFF),RGB(0x80, 0x80, 0x80));
	for (int i = 0; i < 46; i++) m_stcAxisName[i].Init_Ctrl("바탕", 10, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x80, 0x00, 0x00));
	for (int i = 0; i < 46; i++) m_ledAxisRun[i].Init_Ctrl(RGB(0x00, 0xFF, 0x00), RGB(0x40, 0x40, 0x40));
	for (int i = 0; i < 46; i++) m_ledAxisHom[i].Init_Ctrl(RGB(0x00, 0xFF, 0x00), RGB(0x40, 0x40, 0x40));
	m_rdoInitStart.Init_Ctrl("바탕", 15, TRUE, RGB(0x00, 0x00, 0x00), COLOR_DEFAULT, 0, 0);
	m_rdoInitStop.Init_Ctrl("바탕", 15, TRUE, RGB(0x00, 0x00, 0x00), COLOR_DEFAULT, 0, 0);
	m_ledInitialOK.Init_Ctrl("바탕", 15, TRUE, RGB(0x00, 0x00, 0xA0),COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em32);
	for (int i = 0; i < 3; i++) m_bmpImage[i].LoadBitmap(IDB_ARW4_DN);
	for (int i = 0; i < 3; i++) m_Image[i].SetBitmap(m_bmpImage[i]);
	for (int i = 0; i < 3; i++) m_Image[i].SetWindowPos(NULL, 0, 0, 36, 36, SWP_NOZORDER | SWP_NOMOVE);
	for (int i = 0; i < 8; i++) m_picInitFlow[i].Init_Ctrl(COLOR_DEFAULT, RGB(0xFF, 0xFF, 0xFF));
	for (int i = 0; i < 8; i++) m_stcInitFlow[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xFF, 0x00));
	for (int i = 0; i < 2; i++) m_ledMainAir[i].Init_Ctrl("바탕", 11, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i < 3; i++) m_ledEmgSw[i].Init_Ctrl("바탕", 11, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emRed, CLedCS::em16);
	for (int i = 0; i < 13; i++) m_ledDoorOpen[i].Init_Ctrl("바탕", 11, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	m_stcInitCase[0].Init_Ctrl("바탕", 10, TRUE, RGB(0xFF, 0xFF, 0x00), RGB(0x80, 0x80, 0x80));
	for (int i = 1; i < 14; i++) m_stcInitCase[i].Init_Ctrl("바탕", 10, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x80, 0x80, 0x80));
}

void CInitialDlg::Display_Status()
{
	for (int i = 0; i < AXIS_COUNT; i++) {
		g_objAJinAXL.Get_MotorRun(i) ? m_ledAxisRun[i].On() : m_ledAxisRun[i].Off();
		g_objAJinAXL.Get_HomeDone(i) ? m_ledAxisHom[i].On() : m_ledAxisHom[i].Off();
	}

	DX_DATA_12 *pDX12 = g_objAJinAXL.Get_pDX12();
	DX_DATA_13 *pDX13 = g_objAJinAXL.Get_pDX13();

	m_ledMainAir[0].Set_On(pDX12->iMainAir1);
	m_ledMainAir[1].Set_On(pDX12->iMainAir2);

	m_ledEmgSw[0].Set_On(pDX12->iEmgSw1);
	m_ledEmgSw[1].Set_On(pDX12->iEmgSw2);
	m_ledEmgSw[2].Set_On(pDX12->iEmgSw3);

	for (int i = 0; i < 13; i++) m_ledDoorOpen[i].Set_On((pDX13->nValue >> i) & 1);

	BOOL bOk = g_objSequenceInit.Get_InitComplete();
	m_ledInitialOK.Set_On(bOk);

	CString strTemp;
	int *pCase = g_objSequenceInit.Get_InitialCase();
	for (int i = 0; i < 11; i++) { strTemp.Format("%03d", *(pCase + i)); m_stcInitCase[i].Set_Text(strTemp); }
}

void CInitialDlg::Display_Initial() 
{
	if (m_nBackColorLoop == 10) m_nBackColorLoop = 0;
	m_nBackColorLoop++;

	int *pCase = g_objSequenceInit.Get_InitialCase();
	if (*(pCase + 0) == 0) return;							// Main
	
	if (*(pCase + 0) < 50) {												// Main
		if (m_nBackColorLoop == 1) Set_StatusColor(1, 1);	// Red
		if (m_nBackColorLoop == 6) Set_StatusColor(1, 3);	// Blue
	} else if (*(pCase + 0) == 50) {
		if (m_nBackColorLoop == 1) Set_StatusColor(1, 2);	// Green
	}

	if (*(pCase + 0) < 50) return;							// Main

	if (*(pCase + 1) < 90 || *(pCase + 2) < 90) {							// Load
		if (m_nBackColorLoop == 1) Set_StatusColor(2, 1);	// Red
		if (m_nBackColorLoop == 6) Set_StatusColor(2, 3);	// Blue
	} else {
		if (m_nBackColorLoop == 1) Set_StatusColor(2, 2);	// Green
	}

	if (*(pCase + 1) < 90 || *(pCase + 3) < 90 || *(pCase + 4) < 90) {		// Index & Align Vision
		if (m_nBackColorLoop == 1) Set_StatusColor(3, 1);	// Red
		if (m_nBackColorLoop == 6) Set_StatusColor(3, 3);	// Blue
	} else {
		if (m_nBackColorLoop == 1) Set_StatusColor(3, 2);	// Green
	}

	if (*(pCase + 5) < 90 || *(pCase + 6) < 90 || *(pCase + 7) < 90) {		// Cap
		if (m_nBackColorLoop == 1) Set_StatusColor(4, 1);	// Red
		if (m_nBackColorLoop == 6) Set_StatusColor(4, 3);	// Blue
	} else {
		if (m_nBackColorLoop == 1) Set_StatusColor(4, 2);	// Green
	}

	if (*(pCase + 4) < 90 || *(pCase + 8) < 90) {							// Assy & Trans
		if (m_nBackColorLoop == 1) Set_StatusColor(5, 1);	// Red
		if (m_nBackColorLoop == 6) Set_StatusColor(5, 3);	// Blue
	} else {
		if (m_nBackColorLoop == 1) Set_StatusColor(5, 2);	// Green
	}

	if (*(pCase + 9) < 90 || *(pCase + 10) < 90) {							// Unload
		if (m_nBackColorLoop == 1) Set_StatusColor(6, 1);	// Red
		if (m_nBackColorLoop == 6) Set_StatusColor(6, 3);	// Blue
	} else {
		if (m_nBackColorLoop == 1) Set_StatusColor(6, 2);	// Green
	}
}

void CInitialDlg::Set_StatusColor(int nIdx, int nColor)
{
	if (nIdx < 0 || nIdx > 7) { AfxMessageBox("[Set_StatusColor] Wrong Index"); return; }

	COLORREF colorBack = RGB(0xFF, 0xFF, 0xFF);			// White
	if (nColor == 1) colorBack = RGB(0xFF, 0x00, 0x00);	// Red
	if (nColor == 2) colorBack = RGB(0x00, 0xFF, 0x00);	// Green
	if (nColor == 3) colorBack = RGB(0x00, 0x00, 0xFF);	// Blue

	m_picInitFlow[nIdx].Set_Color(COLOR_DEFAULT, colorBack);
	m_stcInitFlow[nIdx].Invalidate(FALSE);
}
///////////////////////////////////////////////////////////////////////////////

void CInitialDlg::OnBnClickedButton1()
{
// 	g_objAJinAXL.Save_AxisList();
// 	g_objCommon.Show_Error(3201);
// 	g_objInspector.Set_ConnectRequest();
}
