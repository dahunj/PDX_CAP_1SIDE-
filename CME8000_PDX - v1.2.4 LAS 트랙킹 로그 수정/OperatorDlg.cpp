// OperatorDlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CME8000.h"
#include "OperatorDlg.h"
#include "afxdialogex.h"

#include "LogFile.h"
#include "Common.h"
#include "SequenceInit.h"
#include "SequenceMain.h"

#include "WorkDlg.h"
#include "SetupDlg.h"
#include "MonitorDlg.h"
#include "CME8000Dlg.h"

// COperatorDlg 대화 상자입니다.
COperatorDlg g_dlgOperator;

IMPLEMENT_DYNAMIC(COperatorDlg, CDialogEx)

COperatorDlg::COperatorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COperatorDlg::IDD, pParent)
{
}

COperatorDlg::~COperatorDlg()
{
}

void COperatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for (int i = 0; i < 16; i++) DDX_Control(pDX, IDC_LABEL_0 + i, m_Label[i]);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_PIC_OPERATOR_0 + i, m_picOperator[i]);
	for (int i = 0; i < 3; i++) DDX_Control(pDX, IDC_STC_OPER_ALM_TIME_0 + i, m_stcOperAlmTime[i]);
	for (int i = 0; i < 3; i++) DDX_Control(pDX, IDC_STC_OPER_ALM_LIST_0 + i, m_stcOperAlmList[i]);
	DDX_Control(pDX, IDC_STC_OPER_LOT_ID, m_stcOperLotId);
	DDX_Control(pDX, IDC_STC_OPER_OPER_ID, m_stcOperOperId);
	DDX_Control(pDX, IDC_STC_OPER_MODEL, m_stcOperModel);
	for (int i = 0; i < 3; i++) DDX_Control(pDX, IDC_STC_OPER_RESULT_0 + i, m_stcOperResult[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_STC_OPER_DOOR_0 + i, m_stcOperDoor[i]);
	DDX_Control(pDX, IDC_STC_OPER_CM_COUNT, m_stcOperCmCount);
	DDX_Control(pDX, IDC_STC_OPER_TRAY_COUNT, m_stcOperTrayCount);
	DDX_Control(pDX, IDC_STC_OPER_LOAD_TRAY, m_stcOperLoadTray);
	DDX_Control(pDX, IDC_STC_OPER_GOOD_TRAY, m_stcOperGoodTray);
	DDX_Control(pDX, IDC_STC_OPER_N_NG_TRAY, m_stcOperNNgTray);
	DDX_Control(pDX, IDC_STC_OPER_S_NG_TRAY, m_stcOperSNgTray);
	DDX_Control(pDX, IDC_GRD_OPER_LOAD_TRAY, m_grdOperLoadTray);
	DDX_Control(pDX, IDC_GRD_OPER_UNLOAD_TRAY, m_grdOperUnloadTray);
	DDX_Control(pDX, IDC_GRD_OPER_CAP_TRAY, m_grdOperCapTray);
	DDX_Control(pDX, IDC_BTN_OPER_START, m_btnOperStart);
	DDX_Control(pDX, IDC_BTN_OPER_STOP, m_btnOperStop);
	DDX_Control(pDX, IDC_BTN_OPER_CYCLE_STOP, m_btnOperCycleStop);
	DDX_Control(pDX, IDC_BTN_OPER_TEST, m_btnOperTest);
	DDX_Control(pDX, IDC_BTN_OPER_INITIAL, m_btnOperInitial);
	DDX_Control(pDX, IDC_BTN_OPER_WORK, m_btnOperWork);
	DDX_Control(pDX, IDC_BTN_OPER_MANUAL, m_btnOperManual);
	DDX_Control(pDX, IDC_BTN_OPER_SETUP, m_btnOperSetup);
	DDX_Control(pDX, IDC_BTN_OPER_MONITOR, m_btnOperMonitor);
	DDX_Control(pDX, IDC_BTN_OPER_EXIT, m_btnOperExit);
}

BEGIN_MESSAGE_MAP(COperatorDlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_STN_CLICKED(IDC_STC_OPER_LOT_ID, &COperatorDlg::OnStnClickedStcOperLotId)
	ON_STN_CLICKED(IDC_STC_OPER_OPER_ID, &COperatorDlg::OnStnClickedStcOperOperId)
	ON_STN_CLICKED(IDC_STC_OPER_CM_COUNT, &COperatorDlg::OnStnClickedStcOperCmCount)
	ON_BN_CLICKED(IDC_BTN_OPER_START, &COperatorDlg::OnBnClickedBtnOperStart)
	ON_BN_CLICKED(IDC_BTN_OPER_STOP, &COperatorDlg::OnBnClickedBtnOperStop)
	ON_BN_CLICKED(IDC_BTN_OPER_CYCLE_STOP, &COperatorDlg::OnBnClickedBtnOperCycleStop)
	ON_BN_CLICKED(IDC_BTN_OPER_INITIAL, &COperatorDlg::OnBnClickedBtnOperInitial)
	ON_BN_CLICKED(IDC_BTN_OPER_WORK, &COperatorDlg::OnBnClickedBtnOperWork)
	ON_BN_CLICKED(IDC_BTN_OPER_MANUAL, &COperatorDlg::OnBnClickedBtnOperManual)
	ON_BN_CLICKED(IDC_BTN_OPER_SETUP, &COperatorDlg::OnBnClickedBtnOperSetup)
	ON_BN_CLICKED(IDC_BTN_OPER_MONITOR, &COperatorDlg::OnBnClickedBtnOperMonitor)
	ON_BN_CLICKED(IDC_BTN_OPER_EXIT, &COperatorDlg::OnBnClickedBtnOperExit)
	ON_BN_CLICKED(IDC_BTN_OPER_TEST, &COperatorDlg::OnBnClickedBtnOperTest)
END_MESSAGE_MAP()

// COperatorDlg 메시지 처리기입니다.

BOOL COperatorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(this, 0, 75, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	Initial_Controls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL COperatorDlg::PreTranslateMessage(MSG* pMsg)
{
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void COperatorDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow) {
		EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

		if (pEquipData->bUseDoorLock) {
			m_stcOperDoor[0].Set_Color(RGB(0x00, 0x00, 0x00),RGB(0x00, 0xFF, 0x00));
			m_stcOperDoor[1].Set_Color(RGB(0x00, 0x00, 0x00),RGB(0xF0, 0xF0, 0xF0));
		} else {
			m_stcOperDoor[0].Set_Color(RGB(0x00, 0x00, 0x00),RGB(0xF0, 0xF0, 0xF0));
			m_stcOperDoor[1].Set_Color(RGB(0x00, 0x00, 0x00),RGB(0xFF, 0x00, 0x00));
		}

		CString strText;

		g_dlgWork.m_stcLotIdS.GetWindowText(strText);
		if (strText.GetLength() < 1) g_dlgWork.m_stcLotIdR.GetWindowText(strText);
		m_stcOperLotId.SetWindowText(strText);

		m_stcOperOperId.SetWindowText(gData.sOperID);

		g_dlgWork.m_stcCmCountS.GetWindowText(strText);
		if (atoi(strText) < 1) g_dlgWork.m_stcCmCountR.GetWindowText(strText);
		m_stcOperCmCount.SetWindowText(strText);

		g_dlgWork.m_stcTrayCountS.GetWindowText(strText);
		if (atoi(strText) < 1) g_dlgWork.m_stcTrayCountR.GetWindowText(strText);
		m_stcOperTrayCount.SetWindowText(strText);

		m_stcOperModel.SetWindowText("Cap Attach");

		Display_Status();

		if (g_dlgWork.GetSafeHwnd()) g_dlgWork.SetTimer(0, 100, NULL);
		SetTimer(0, 200, NULL);
	} else {
		if (g_dlgWork.GetSafeHwnd()) g_dlgWork.KillTimer(0);
		KillTimer(0);
	}
}

void COperatorDlg::OnTimer(UINT nIDEvent) 
{
	KillTimer(0);

	Display_Status();

	SetTimer(0, 200, NULL);
	CDialogEx::OnTimer(nIDEvent);
}

void COperatorDlg::OnStnClickedStcOperLotId()
{
	CString strKey;
	if (g_objCommon.Show_KeyPad(strKey) != IDOK) return;

	m_stcOperLotId.SetWindowText(strKey);
	g_dlgWork.m_stcLotIdS.SetWindowText(strKey);

	CString sLog;
	int nNo = gData.nLPNo-1;
	if (nNo < 0) nNo = 0;
	sLog.Format("[Operator] Lot ID Input....  LotID[%s] CM[%d] OperID[%s]", gData.sLotID[nNo], gData.nCmMaxCount, gData.sOperID);
	g_objLogFile.Save_HandlerLog(sLog);
}

void COperatorDlg::OnStnClickedStcOperOperId()
{
	CString strKey;
	if (g_objCommon.Show_KeyPad(strKey) != IDOK) return;

	gData.sOperID = strKey;
	m_stcOperOperId.SetWindowText(strKey);

	CString sLog;
	int nNo = gData.nLPNo-1;
	if (nNo < 0) nNo = 0;
	sLog.Format("[Operator] Operator ID Input....  LotID[%s] CM[%d] OperID[%s]", gData.sLotID[nNo], gData.nCmMaxCount, gData.sOperID);
	g_objLogFile.Save_HandlerLog(sLog);
}

void COperatorDlg::OnStnClickedStcOperCmCount()
{
	CString strOld, strNew, strValue;

	g_dlgWork.m_stcCmCountS.GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	int nCmCnt = atoi(strNew);

	int nTrayUseCount = nCmCnt / gData.nCmMaxCount;
	if (nCmCnt % gData.nCmMaxCount) nTrayUseCount++;
	strValue.Format("%d", nTrayUseCount);
	m_stcOperTrayCount.SetWindowText(strValue);
	g_dlgWork.m_stcTrayCountS.SetWindowText(strValue);

	strValue.Format("%d", nCmCnt);
	m_stcOperCmCount.SetWindowText(strValue);
	g_dlgWork.m_stcCmCountS.SetWindowText(strValue);

	CString sLog;
	int nNo = gData.nLPNo-1;
	if (nNo < 0) nNo = 0;
	sLog.Format("[Operator] CM Count Input....  LotID[%s] OperID[%s]", gData.sLotID[nNo], gData.sOperID);
	g_objLogFile.Save_HandlerLog(sLog);
}

void COperatorDlg::OnBnClickedBtnOperStart()
{
	if (gData.bCycleStop) {
		if (g_objCommon.Show_MsgBox(2, "Are you sure you want to Start by Cycle Stop .....") != IDOK) return;
	}

	CString strUser;
	m_stcOperOperId.GetWindowText(strUser);
	if (strUser.GetLength() < 1) {
		g_objCommon.Show_MsgBox(1, "Input Operator ID");
		return;
	}

	g_dlgWork.m_rdoWorkStart.SetCheck(TRUE);
	Enable_OpModeButton(FALSE);

	CString sLog;
	int nNo = gData.nLPNo-1;
	if (nNo < 0) nNo = 0;
	sLog.Format("[Operator] START button push....  LotID[%s] OperID[%s]", gData.sLotID[nNo], gData.sOperID);
	g_objLogFile.Save_HandlerLog(sLog);
}

void COperatorDlg::OnBnClickedBtnOperStop()
{
	g_dlgWork.m_rdoWorkStop.SetCheck(TRUE);
	Enable_OpModeButton(TRUE);

	CString sLog;
	int nNo = gData.nLPNo-1;
	if (nNo < 0) nNo = 0;
	sLog.Format("[Operator] STOP button push....  LotID[%s] OperID[%s]", gData.sLotID[nNo], gData.sOperID);
	g_objLogFile.Save_HandlerLog(sLog);
}

void COperatorDlg::OnBnClickedBtnOperCycleStop()
{
	g_dlgWork.m_rdoWorkStop.SetCheck(TRUE);
	Enable_OpModeButton(TRUE);

	CString sLog;
	int nNo = gData.nLPNo-1;
	if (nNo < 0) nNo = 0;
	sLog.Format("[Operator] PAUSE button push....  LotID[%s] OperID[%s]", gData.sLotID[nNo], gData.sOperID);
	g_objLogFile.Save_HandlerLog(sLog);

// 	if (gData.bCycleStop) return;
// 
// 	if (g_dlgWork.m_rdoWorkStart.GetCheck()) {
// 		g_objCommon.Show_MsgBox(1, "Not allowed Cycle Stop in Auto Run.");
// 		return;
// 	}
// 
// 	if (g_objCommon.Show_MsgBox(2, "Are you sure you want to Cycle Stop?") != IDOK) return;
// 
// 	g_dlgWork.m_chkCycleStop.SetCheck(TRUE);
// 	g_dlgWork.m_chkCycleStop.Set_Color(RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x00, 0xFF));
// 	m_btnOperCycleStop.Set_Color(RGB(0x00, 0x00, 0x00), RGB(0x80, 0xFF, 0xFF));
// 	gData.bCycleStop = TRUE;
// 
// 	CString sLog;
// 	int nNo = gData.nLPNo-1;
// 	if (nNo < 0) nNo = 0;
// 	sLog.Format("[Operator] Cycle Stop button push....  LotID[%s] OperID[%s]", gData.sLotID[nNo], gData.sOperID);
// 	g_objLogFile.Save_HandlerLog(sLog);
}

void COperatorDlg::OnBnClickedBtnOperInitial()
{
	ShowWindow(SW_HIDE);
	CCME8000Dlg *pMainDlg = (CCME8000Dlg*)AfxGetApp()->GetMainWnd();
	pMainDlg->Set_CurrentMode(MODE_INITIAL);
}

void COperatorDlg::OnBnClickedBtnOperWork()
{
	CCME8000Dlg *pMainDlg = (CCME8000Dlg*)AfxGetApp()->GetMainWnd();
#ifdef DRY_RUN_TEST
	ShowWindow(SW_HIDE);
	g_dlgSetup.Set_LoginUser(2);	// SI
	pMainDlg->Set_CurrentMode(MODE_WORK);
#else
	int nUser = 0;
	if (g_objCommon.Show_Password(nUser) != IDOK) return;

	ShowWindow(SW_HIDE);
	pMainDlg->Set_CurrentMode(MODE_WORK);
#endif
// 	ShowWindow(SW_HIDE);
// 	CCME8000Dlg *pMainDlg = (CCME8000Dlg*)AfxGetApp()->GetMainWnd();
// 	pMainDlg->Set_CurrentMode(MODE_WORK);
}

void COperatorDlg::OnBnClickedBtnOperManual()
{
	CCME8000Dlg *pMainDlg = (CCME8000Dlg*)AfxGetApp()->GetMainWnd();
#ifdef DRY_RUN_TEST
	ShowWindow(SW_HIDE);
	g_dlgSetup.Set_LoginUser(2);	// SI
	pMainDlg->Set_CurrentMode(MODE_MANUAL);
#else
	int nUser = 0;
	if (g_objCommon.Show_Password(nUser) != IDOK) return;

	ShowWindow(SW_HIDE);
	pMainDlg->Set_CurrentMode(MODE_MANUAL);
#endif

// 	ShowWindow(SW_HIDE);
// 	CCME8000Dlg *pMainDlg = (CCME8000Dlg*)AfxGetApp()->GetMainWnd();
// 	pMainDlg->Set_CurrentMode(MODE_MANUAL);
}

void COperatorDlg::OnBnClickedBtnOperSetup()
{
	CCME8000Dlg *pMainDlg = (CCME8000Dlg*)AfxGetApp()->GetMainWnd();
#ifdef DRY_RUN_TEST
	ShowWindow(SW_HIDE);
	g_dlgSetup.Set_LoginUser(2);	// SI
	pMainDlg->Set_CurrentMode(MODE_SETUP);
#else
	int nUser = 0;
	if (g_objCommon.Show_Password(nUser) != IDOK) return;
	g_dlgSetup.Set_LoginUser(nUser);

	ShowWindow(SW_HIDE);
	pMainDlg->Set_CurrentMode(MODE_SETUP);
#endif
}

void COperatorDlg::OnBnClickedBtnOperMonitor()
{
	if (g_dlgMonitor.IsWindowVisible()) g_dlgMonitor.ShowWindow(SW_HIDE);
	else g_dlgMonitor.ShowWindow(SW_SHOW);	
}

void COperatorDlg::OnBnClickedBtnOperExit()
{
	if (g_objCommon.Show_MsgBox(2, "Are you sure you want to exit the program?") != IDOK) return;

	CString sLog;
	int nNo = gData.nLPNo-1;
	if (nNo < 0) nNo = 0;
	sLog.Format("[Operator] Program Exit button push....  LotID[%s] OperID[%s]", gData.sLotID[nNo], gData.sOperID);
	g_objLogFile.Save_HandlerLog(sLog);

	CCME8000Dlg *pMainDlg = (CCME8000Dlg*)AfxGetApp()->GetMainWnd();
	pMainDlg->Exit_System(EXIT_SYSTEM_NONE);
}

void COperatorDlg::OnBnClickedBtnOperEngineer()
{
#ifdef DRY_RUN_TEST
	ShowWindow(SW_HIDE);
	g_dlgSetup.Set_LoginUser(2);	// SI
#else
	int nUser = 0;
	if (g_objCommon.Show_Password(nUser) != IDOK) return;
	ShowWindow(SW_HIDE);
#endif

// 	int nUser = 0;
// 	if (g_objCommon.Show_Password(nUser) != IDOK) return;
// 	ShowWindow(SW_HIDE);
}

///////////////////////////////////////////////////////////////////////////////
// User Functions

void COperatorDlg::Initial_Controls() 
{
	for (int i = 0; i < 16; i++) m_Label[i].Init_Ctrl("Segoe UI", 12, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xE6, 0xE6, 0xE6));
	for (int i = 0; i < 4; i++) m_picOperator[i].Init_Ctrl(COLOR_DEFAULT, RGB(0xFF, 0xFF, 0xFF));
	for (int i = 0; i < 3; i++) m_stcOperAlmTime[i].Init_Ctrl("Segoe UI", 12, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xFF, 0xFF, 0xFF));
	for (int i = 0; i < 3; i++) m_stcOperAlmList[i].Init_Ctrl("Segoe UI", 10, FALSE, RGB(0x00, 0x00, 0x00), RGB(0xFF, 0xFF, 0xFF));
	m_stcOperLotId.Init_Ctrl("Segoe UI", 14, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xFF, 0xFF, 0xFF));
	m_stcOperOperId.Init_Ctrl("Segoe UI", 14, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xFF, 0xFF, 0xFF));
	m_stcOperModel.Init_Ctrl("Segoe UI", 14, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xE6, 0xE6, 0xE6));
	for (int i = 0; i < 3; i++) m_stcOperResult[i].Init_Ctrl("Segoe UI", 14, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xFF, 0xFF, 0xFF));
	for (int i = 0; i < 2; i++) m_stcOperDoor[i].Init_Ctrl("Segoe UI", 20, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xF0, 0xF0, 0xF0));
	m_stcOperTrayCount.Init_Ctrl("Segoe UI", 12, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xE6, 0xE6, 0xE6));
	m_stcOperCmCount.Init_Ctrl("Segoe UI", 12, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xFF, 0xFF, 0xFF));
	m_stcOperLoadTray.Init_Ctrl("Segoe UI", 12, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xFF, 0xFF, 0xFF));
	m_stcOperGoodTray.Init_Ctrl("Segoe UI", 12, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xFF, 0xFF, 0xFF));
	m_stcOperNNgTray.Init_Ctrl("Segoe UI", 12, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xFF, 0xFF, 0xFF));
	m_stcOperSNgTray.Init_Ctrl("Segoe UI", 12, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xFF, 0xFF, 0xFF));
	Initial_Grid(&m_grdOperLoadTray, gData.nTrayY, gData.nTrayX);
	Initial_Grid(&m_grdOperCapTray, gData.nTrayY, gData.nTrayX);
	Initial_Grid(&m_grdOperUnloadTray, gData.nTrayY, gData.nTrayX);
	m_btnOperStart.Init_Ctrl("Segoe UI", 40, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xD7, 0xD7, 0xD7), NULL, CButtonCS::emTop);
	m_btnOperStop.Init_Ctrl("Segoe UI", 40, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xD7, 0xD7, 0xD7), NULL, CButtonCS::emTop);
	m_btnOperCycleStop.Init_Ctrl("Segoe UI", 40, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xD7, 0xD7, 0xD7), NULL, CButtonCS::emTop);
	m_btnOperTest.Init_Ctrl("Segoe UI", 30, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xD7, 0xD7, 0xD7), NULL, CButtonCS::emTop);
	m_btnOperInitial.Init_Ctrl("Segoe UI", 22, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xD7, 0xD7, 0xD7), NULL, CButtonCS::emTop);
	m_btnOperWork.Init_Ctrl("Segoe UI", 22, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xD7, 0xD7, 0xD7), NULL, CButtonCS::emTop);
	m_btnOperManual.Init_Ctrl("Segoe UI", 22, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xD7, 0xD7, 0xD7), NULL, CButtonCS::emTop);
	m_btnOperSetup.Init_Ctrl("Segoe UI", 22, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xD7, 0xD7, 0xD7), NULL, CButtonCS::emTop);
	m_btnOperMonitor.Init_Ctrl("Segoe UI", 22, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xD7, 0xD7, 0xD7), NULL, CButtonCS::emTop);
	m_btnOperExit.Init_Ctrl("Segoe UI", 22, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xD7, 0xD7, 0xD7), NULL, CButtonCS::emTop);
}

void COperatorDlg::Initial_Grid(CGridCS *pGrid, int nRows, int nCols, int nNoDir)
{
	pGrid->Set_RowCount(nRows);
	pGrid->Set_ColCount(nCols);
	pGrid->Set_FixRowCount(0);
	pGrid->Set_FixColCount(0);

	long lTotalW = pGrid->Get_GridWidth();
	long lTotalH = pGrid->Get_GridHeight();
	long lCellW = lTotalW / nCols;
	long lCellH = lTotalH / nRows;

	CString str;
	for (int i = 0; i < nRows; i++) {
		pGrid->Set_RowHeight(i, lCellH);

		for (int j = 0; j < nCols; j++) {
			if (i == 0) pGrid->Set_ColWidth(j, lCellW);

			pGrid->Set_CellAlign(i, j, CELL_ALIGN_CENTER);
			if (nNoDir == 1) { str.Format(_T("%d"), j * nRows + i + 1); }
			else			 { str.Format(_T("%d"), i * nCols + j + 1); }
			pGrid->Set_CellText(i, j, str);
		}
	}
}

void COperatorDlg::Enable_OpModeButton(BOOL bEnable)
{
	m_btnOperInitial.EnableWindow(bEnable);
	m_btnOperManual.EnableWindow(bEnable);
	m_btnOperSetup.EnableWindow(bEnable);
}

void COperatorDlg::Display_Status()
{
	CTime datetime = CTime::GetCurrentTime();

	for (int i = 0; i < 3; i++) m_stcOperAlmTime[i].SetWindowText(gData.sAlarmTime[i]);
	for (int i = 0; i < 3; i++) m_stcOperAlmList[i].SetWindowText(gData.sAlarmList[i]);

	if (g_dlgWork.m_rdoWorkStart.GetCheck()) {
		if (m_btnOperInitial.IsWindowEnabled()) {
			m_btnOperStart.Set_Color(RGB(0x00, 0x00, 0x00), RGB(0x00, 0xFF, 0x00));
			m_btnOperStop.Set_Color(RGB(0x00, 0x00, 0x00), RGB(0xD7, 0xD7, 0xD7));
			Enable_OpModeButton(FALSE);
		}
	}

	if (g_dlgWork.m_rdoWorkStop.GetCheck()) {
		if (!m_btnOperInitial.IsWindowEnabled()) {
			m_btnOperStop.Set_Color(RGB(0x00, 0x00, 0x00), RGB(0xFF, 0x00, 0x00));
			m_btnOperStart.Set_Color(RGB(0x00, 0x00, 0x00), RGB(0xD7, 0xD7, 0xD7));
			Enable_OpModeButton(TRUE);
		}
	}

	int nNo = gData.nLPNo-1;
	if (nNo < 0) nNo = 0;

	COLORREF crCycleStop = (gData.bCycleStop ? RGB(0x80, 0xFF, 0xFF) : RGB(0xD7, 0xD7, 0xD7));
	m_btnOperCycleStop.Set_Color(RGB(0x00, 0x00, 0x00), crCycleStop);

	CString strText;
	strText.Format("%d", gLot.nGoodCount[nNo]);
	m_stcOperResult[0].SetWindowText(strText);

	strText.Format("%d", gLot.nNgCount[nNo]);
	m_stcOperResult[1].SetWindowText(strText);

	int nTotal = gLot.nGoodCount[nNo] + gLot.nNgCount[nNo];
	if (nTotal < 1) strText = "0.0%";
	else strText.Format("%0.1lf%%", gLot.nGoodCount[nNo] * 100.0 / nTotal);
	m_stcOperResult[2].SetWindowText(strText);
}

void COperatorDlg::Update_TrayInfo(int nTray, int nNo)
{
	CString strText;

	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	if (nTray == 0 || nTray == 1) {		// Load Tray
		int nPNo = gData.nLPNo - 1;
		if (nPNo < 0) nPNo = 0;
		strText.Format("%d", gData.nLoadTrayCount[nPNo]);
		m_stcOperLoadTray.SetWindowText(strText);

		int nTrayX = pEquipData->nLoadTrayX;
		int nTrayY = pEquipData->nLoadTrayY;
		for (int i = 0; i < nTrayY; i++) {
			for (int j = 0; j < nTrayX; j++) {
				if		(gData.InfoLoadTray[i][j] == 9) m_grdOperLoadTray.Set_CellBackClr(i, j, RGB(0x00, 0xFF, 0xFF));	// Reserve
				else if (gData.InfoLoadTray[i][j] == 0) m_grdOperLoadTray.Set_CellBackClr(i, j, RGB(0xFF, 0xFF, 0xFF));	// Empty
				else									m_grdOperLoadTray.Set_CellBackClr(i, j, RGB(0x80, 0x80, 0x80));	// Error
			}
		}
	}
	if (nTray == 0 || nTray == 2) {		// Cap Tray

		int nTrayX = pEquipData->nCapTrayX;
		int nTrayY = pEquipData->nCapTrayY;
		for (int i = 0; i < nTrayY; i++) {
			for (int j = 0; j < nTrayX; j++) {
				if		(gData.InfoCapTray[i][j] == 9) m_grdOperCapTray.Set_CellBackClr(i, j, RGB(0x00, 0xFF, 0xFF));	// Reserve
				else if (gData.InfoCapTray[i][j] == 0) m_grdOperCapTray.Set_CellBackClr(i, j, RGB(0xFF, 0xFF, 0xFF));	// Empty
				else								   m_grdOperCapTray.Set_CellBackClr(i, j, RGB(0x80, 0x80, 0x80));	// Error
			}
		}
	}

	if (nTray == 0 || nTray == 3) {		// Unload Tray
		int nTrayX = pEquipData->nUnloadTrayX;
		int nTrayY = pEquipData->nUnloadTrayY;
		for (int i = 0; i < nTrayY; i++) {
			for (int j = 0; j < nTrayX; j++) {
				if		(gData.InfoUnloadTray[i][j] >  1) m_grdOperUnloadTray.Set_CellBackClr(i, j, RGB(0x70, 0x70, 0x70));	// Not Use
				else if	(gData.InfoUnloadTray[i][j] == 1) m_grdOperUnloadTray.Set_CellBackClr(i, j, RGB(0x00, 0xFF, 0x00));	// Good
				else if (gData.InfoUnloadTray[i][j] == 0) m_grdOperUnloadTray.Set_CellBackClr(i, j, RGB(0xFF, 0xFF, 0xFF));	// Empty
				else									  m_grdOperUnloadTray.Set_CellBackClr(i, j, RGB(0x80, 0x80, 0x80));	// Error
			}
		}
	}
}

void COperatorDlg::Set_GridModelChange()
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	Initial_Grid(&m_grdOperLoadTray, pEquipData->nLoadTrayY, pEquipData->nLoadTrayX);
	Initial_Grid(&m_grdOperCapTray, pEquipData->nCapTrayY, pEquipData->nCapTrayX);
	Initial_Grid(&m_grdOperUnloadTray, pEquipData->nUnloadTrayY, pEquipData->nUnloadTrayX);
}

///////////////////////////////////////////////////////////////////////////////

void COperatorDlg::OnBnClickedBtnOperTest()
{
}

///////////////////////////////////////////////////////////////////////////////
