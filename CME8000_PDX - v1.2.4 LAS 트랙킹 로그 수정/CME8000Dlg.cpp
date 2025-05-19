// CME8000Dlg.cpp : 구현 파일
//
#include "stdafx.h"
#include "CME8000.h"
#include "CME8000Dlg.h"
#include "afxdialogex.h"

#include "SplashDlg.h"

#include "LogFile.h"
#include "AJinAXL.h"
#include "DataManager.h"
#include "Common.h"

#include "OperatorDlg.h"
#include "InitialDlg.h"
#include "WorkDlg.h"
#include "ManualDlg.h"
#include "SetupDlg.h"
#include "ProhibitDlg.h"
#include "MonitorDlg.h"
#include "ErrorDlg.h"
#include "AlarmDlg.h"
#include "VersionDlg.h"
#include "NoWorkDlg.h"

#include "Inspector.h"
#include "BarcodeLot.h"
#include "LoadCell.h"
#include "MESInterface.h"
#include "AviUDP.h"

#include "SequenceInit.h"
#include "SequenceMain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CCME8000Dlg 대화 상자

CCME8000Dlg::CCME8000Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCME8000Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCME8000Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IMG_LG_LOGO, m_imgLgLogo);
	DDX_Control(pDX, IDC_PIC_MAIN_LOGO, m_picMainLogo);
	DDX_Control(pDX, IDC_STC_MAIN_EQUIP, m_stcMainEquip);
	DDX_Control(pDX, IDC_STC_MAIN_MODE, m_stcMainMode);
	DDX_Control(pDX, IDC_STC_MAIN_OP_ENG, m_stcMainOpEng);
	DDX_Control(pDX, IDC_STC_MAIN_DATE, m_stcMainDate);
	DDX_Control(pDX, IDC_STC_MAIN_TIME, m_stcMainTime);
	DDX_Control(pDX, IDC_STC_MAIN_VER, m_stcMainVer);
	for (int i = 0; i < 3; i++) DDX_Control(pDX, IDC_STC_MAIN_TOWER_0 + i, m_stcMainTower[i]);
	DDX_Control(pDX, IDC_PIC_MODE_BACK, m_picModeBack);
	DDX_Control(pDX, IDC_BTN_MAIN_OPERATOR, m_btnMainOperator);
	DDX_Control(pDX, IDC_RDO_MAIN_PARAM, m_rdoMainParm);
	DDX_Control(pDX, IDC_RDO_MAIN_WORK, m_rdoMainWork);
	DDX_Control(pDX, IDC_RDO_MAIN_MANUAL, m_rdoMainManual);
	DDX_Control(pDX, IDC_RDO_MAIN_SETUP, m_rdoMainSetup);
	DDX_Control(pDX, IDC_RDO_MAIN_PROHIBIT, m_rdoMainProhibit);
	DDX_Control(pDX, IDC_BTN_MAIN_MONITOR, m_btnMainMonitor);
	DDX_Control(pDX, IDC_BTN_MAIN_EXIT, m_btnMainExit);
	DDX_Control(pDX, IDC_BTN_MAIN_IO, m_btnMainIO);
	DDX_Control(pDX, IDC_BTN_MAIN_ALARM, m_btnMainAlarm);
}

BEGIN_MESSAGE_MAP(CCME8000Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_MAIN_OPERATOR, &CCME8000Dlg::OnBnClickedBtnMainOperator)
	ON_BN_CLICKED(IDC_RDO_MAIN_PARAM, &CCME8000Dlg::OnBnClickedRdoMainParam)
	ON_BN_CLICKED(IDC_RDO_MAIN_WORK, &CCME8000Dlg::OnBnClickedRdoMainWork)
	ON_BN_CLICKED(IDC_RDO_MAIN_MANUAL, &CCME8000Dlg::OnBnClickedRdoMainManual)
	ON_BN_CLICKED(IDC_RDO_MAIN_SETUP, &CCME8000Dlg::OnBnClickedRdoMainSetup)
	ON_BN_CLICKED(IDC_RDO_MAIN_PROHIBIT, &CCME8000Dlg::OnBnClickedRdoMainProhibit)
	ON_BN_CLICKED(IDC_BTN_MAIN_MONITOR, &CCME8000Dlg::OnBnClickedBtnMainMonitor)
	ON_BN_CLICKED(IDC_BTN_MAIN_IO, &CCME8000Dlg::OnBnClickedBtnMainIO)
	ON_BN_CLICKED(IDC_BTN_MAIN_ALARM, &CCME8000Dlg::OnBnClickedBtnMainAlarm)
	ON_BN_CLICKED(IDC_BTN_MAIN_EXIT, &CCME8000Dlg::OnBnClickedBtnMainExit)
	ON_STN_CLICKED(IDC_STC_MAIN_VER, &CCME8000Dlg::OnStnClickedStcMainVer)
	ON_STN_DBLCLK(IDC_STC_MAIN_EQUIP, &CCME8000Dlg::OnDblclkStcMainEquip)
END_MESSAGE_MAP()

// CCME8000Dlg 메시지 처리기

BOOL CCME8000Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	SetWindowText("CME8000_Handler");

	Initial_Controls();

	m_bInsideLight = FALSE;
	m_bTowerOn = FALSE;
	m_bBuzzerOn = FALSE;

	m_bScreenOff = FALSE;
	m_dwScreenTime = m_dwNoWorkTime = GetTickCount();

	gData.nTrayX = 1;
	gData.nTrayY = 1;

	gData.bTempLot[0] = FALSE;
	gData.bTempLot[1] = FALSE;

	gData.bContinueLot = FALSE;

	g_objCommon.Create(NULL, NULL, WS_CHILD, CRect(0,0,0,0), this, 0);
	g_objInspector.Create(NULL, NULL, WS_CHILD, CRect(0,0,0,0), this, 0);
	g_objBarcodeLot.Create(NULL, NULL, WS_CHILD, CRect(0,0,0,0), this, 0);
	g_objLoadCell.Create(NULL, NULL, WS_CHILD, CRect(0,0,0,0), this, 0);
	g_objAviUDP.Create(NULL, NULL, WS_CHILD, CRect(0,0,0,0), this, 0);

	g_dlgOperator.Create(COperatorDlg::IDD, this);
	g_dlgInitial.Create(CInitialDlg::IDD, this);
	g_dlgWork.Create(CWorkDlg::IDD, this);
	g_dlgManual.Create(CManualDlg::IDD, this);
	g_dlgSetup.Create(CSetupDlg::IDD, this);
	g_dlgProhibit.Create(CProhibitDlg::IDD, this);
	g_dlgMonitor.Create(CMonitorDlg::IDD, this);
	g_dlgError.Create(CErrorDlg::IDD, this);
	g_dlgAlarm.Create(CAlarmDlg::IDD, this);
	g_dlgVersion.Create(CVersionDlg::IDD, this);
	g_dlgNoWork.Create(CNoWorkDlg::IDD, this);

	CString strLog;
	strLog.Format("[Main Dialog] Program Begin [%s]", MAIN_VERSION);
	g_objLogFile.Save_HandlerLog(strLog);

	SetTimer(TIMER_DATE_TIME, 500, NULL);
	SetTimer(TIMER_DOOR_LOCK, 1000, NULL);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CCME8000Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CCME8000Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CCME8000Dlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	if (pMsg->message == WM_KEYDOWN || pMsg->message == WM_MOUSEMOVE || pMsg->message == WM_LBUTTONDOWN) Set_ScreenOn();

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CCME8000Dlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	g_objInspector.Set_LightOff();
	g_objInspector.Set_StatusUpdate(0);
	g_objAviUDP.Set_StatusUpdate(0);
	
	g_dlgVersion.DestroyWindow();
	g_dlgAlarm.DestroyWindow();
	g_dlgError.DestroyWindow();
	g_dlgMonitor.DestroyWindow();
	g_dlgProhibit.DestroyWindow();
	g_dlgSetup.DestroyWindow();
	g_dlgManual.DestroyWindow();
	g_dlgWork.DestroyWindow();
	g_dlgInitial.DestroyWindow();
	g_dlgOperator.DestroyWindow();
	g_dlgNoWork.DestroyWindow();

	g_objInspector.DestroyWindow();
	g_objBarcodeLot.DestroyWindow();
	g_objLoadCell.DestroyWindow();
	g_objAviUDP.DestroyWindow();
	g_objCommon.DestroyWindow();
}

void CCME8000Dlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (!bShow) return;

	Gloval_Initial();

	CSplashDlg dlgSplash;
	dlgSplash.Create(IDD_SPLASH_DLG, NULL);
	dlgSplash.ShowWindow(SW_SHOW);

	CIniFileCS INI(gsCurrentDir + "\\System\\EquipData.ini");
	INI.Set_Bool("EQUIPMENT", "DOOR_LOCK", TRUE);

	if (!g_objDataManager.Read_ModelData()) { Exit_System(EXIT_SYSTEM_MODEL); return; }	// Model Read 실패하면 프로그램 종료.
	g_objDataManager.Read_EquipData();
	g_objDataManager.Read_MoveData();

	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	if (INI.Check_File()) {
		pEquipData->nCappingCnt = INI.Get_Integer("LOAD_CELL", "CAPPING_CNT", 0);
	} else { AfxMessageBox("EquipData.ini File Not Found!!!"); }

	if (!g_objAJinAXL.Initialize()) { Exit_System(EXIT_SYSTEM_AJIN); return; }

	g_objInspector.Initialize();
	g_objAviUDP.Initialize();
	
	gData.sOperID = "";
	g_objMES.Initialize(pEquipData->bUseMes);

	if (!g_objLoadCell.Initialize()) { AfxMessageBox("LoadCell Connect Fail!!!"); }
	if (!g_objBarcodeLot.Initialize()) { Exit_System(EXIT_SYSTEM_BARCODE); return; }

// 	m_stcMainLabel.SetWindowText("Auto Vision Inspection - Initial Mode");

	char myCom[256];
	gethostname(myCom, sizeof(myCom));
	gData.sComName.Format("%s", myCom);

	CString strVersion;
	strVersion.Format("%s", MAIN_VERSION);
	m_stcMainVer.SetWindowText(strVersion);

	g_objInspector.Set_StatusUpdate(0);
	g_objAviUDP.Set_StatusUpdate(0);
	g_objInspector.Set_LightOff();
	g_objCommon.Read_CapShipPoatNo();

	theApp.uSleep(1000);

	dlgSplash.DestroyWindow();

	Set_CurrentMode(MODE_INITIAL);
	Set_CurrentState(STATE_NONE);
	g_dlgWork.PostMessage(UM_MODEL_CHANGE, NULL, NULL);

	m_dwEquipProcTime = GetTickCount();
	Set_LotErrorLog("PROGRAM BEGIN", 900, "Program Begin");
	g_dlgWork.MachineStopLog("PROGRAM_BEGIN");
}

void CCME8000Dlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent) {
	case TIMER_DATE_TIME:
		Display_DateTime();
		Set_InsideLight();
		Set_ScreenOff();
		Set_NoWork();
		break;
	case TIMER_TOWER_FLKR:
		Set_TowerFlicker(TRUE);
		break;
	case TIMER_BUZZER_FLKR:
		Set_BuzzerFlicker(TRUE);
		break;
	case TIMER_LOAD1_LAMP_FLKR:
		{
			BOOL bLoad1 = g_objSequenceMain.Get_LotLoadEnable(0);
			if (bLoad1 && gData.bLoadLampOn[0])	{ Set_LampFlicker_Load1(TRUE); /*Set_BuzzerFlicker(TRUE);*/ }
		}
		break;
	case TIMER_LOAD2_LAMP_FLKR:
		{
			BOOL bLoad2 = g_objSequenceMain.Get_LotLoadEnable(1);
			if (bLoad2 && gData.bLoadLampOn[1])	{ Set_LampFlicker_Load2(TRUE); /*Set_BuzzerFlicker(TRUE);*/ }

			if (gData.bLoadPort3Wait)	{ Set_LampFlicker_Load3(TRUE); /*Set_BuzzerFlicker(TRUE);*/ }
		}
		break;
	case TIMER_NG_LAMP_FLKR:
		if (gData.bCapPort1Wait)	{ Set_LampFlicker_Cap1(TRUE); /*Set_BuzzerFlicker(TRUE);*/ }
		if (gData.bCapPort2Wait)	{ Set_LampFlicker_Cap2(TRUE); /*Set_BuzzerFlicker(TRUE);*/ }
		break;
	case TIMER_GOOD_LAMP_FLKR:
		if (gData.bUnloadPort1Wait) { Set_LampFlicker_Unload1(TRUE);	/*Set_BuzzerFlicker(TRUE);*/ }
		if (gData.bUnloadPort2Wait) { Set_LampFlicker_Unload2(TRUE);	/*Set_BuzzerFlicker(TRUE);*/ }
		break;
	case TIMER_DOOR_LOCK:
		Set_DoorLock();
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CCME8000Dlg::OnBnClickedBtnMainOperator()
{
	int nMode = theApp.Get_MainMode();
	if (nMode == MODE_OPERATOR) {
#ifdef DRY_RUN_TEST
		g_dlgSetup.Set_LoginUser(2);	// SI
		Set_CurrentMode(MODE_WORK);
#else
		int nUser = 0;
		if (g_objCommon.Show_Password(nUser) != IDOK) return;
		g_dlgSetup.Set_LoginUser(nUser);
		Set_CurrentMode(MODE_WORK);
#endif
	}
	else Set_CurrentMode(MODE_OPERATOR);
}

void CCME8000Dlg::OnBnClickedRdoMainParam()
{
#ifdef DRY_RUN_TEST
	g_dlgSetup.Set_LoginUser(2);	// SI
	Set_CurrentMode(MODE_SETUP);
#else
	int nUser = 0;
	if (g_objCommon.Show_Password(nUser) == IDOK) {
		theApp.bParamMode = TRUE;
		g_dlgSetup.Set_LoginUser(nUser);
		Set_CurrentMode(MODE_PARAM);

	} else {
		int nMode = theApp.Get_MainMode();
		if (nMode == MODE_WORK) m_rdoMainWork.SetCheck(TRUE);
		if (nMode == MODE_MANUAL) m_rdoMainManual.SetCheck(TRUE);
		if (nMode == MODE_SETUP) m_rdoMainSetup.SetCheck(TRUE);
		if (nMode == MODE_PROHIBIT) m_rdoMainProhibit.SetCheck(TRUE);
		if (nMode == MODE_PARAM) m_rdoMainParm.SetCheck(TRUE);
	}
#endif
}

void CCME8000Dlg::OnBnClickedRdoMainWork()
{
	Set_CurrentMode(MODE_WORK);
}

void CCME8000Dlg::OnBnClickedRdoMainManual()
{
	if (gData.bManualPassword) {
		int nUser = 0;
		if (g_objCommon.Show_Password(nUser) == IDOK) {
			Set_CurrentMode(MODE_MANUAL);

		} else {
			int nMode = theApp.Get_MainMode();
			if (nMode == MODE_WORK) m_rdoMainWork.SetCheck(TRUE);
			if (nMode == MODE_MANUAL) m_rdoMainManual.SetCheck(TRUE);
			if (nMode == MODE_SETUP) m_rdoMainSetup.SetCheck(TRUE);
			if (nMode == MODE_PROHIBIT) m_rdoMainProhibit.SetCheck(TRUE);
			if (nMode == MODE_PARAM) m_rdoMainParm.SetCheck(TRUE);
		}
	} else {
		Set_CurrentMode(MODE_MANUAL);
	}
}

void CCME8000Dlg::OnBnClickedRdoMainSetup()
{
#ifdef DRY_RUN_TEST
	g_dlgSetup.Set_LoginUser(2);	// SI
	Set_CurrentMode(MODE_SETUP);
#else
	int nUser = 0;
	if (g_objCommon.Show_Password(nUser) == IDOK) {
		theApp.bParamMode = FALSE;
		g_dlgSetup.Set_LoginUser(nUser);
		Set_CurrentMode(MODE_SETUP);

	} else {
		int nMode = theApp.Get_MainMode();
		if (nMode == MODE_WORK) m_rdoMainWork.SetCheck(TRUE);
		if (nMode == MODE_MANUAL) m_rdoMainManual.SetCheck(TRUE);
		if (nMode == MODE_SETUP) m_rdoMainSetup.SetCheck(TRUE);
		if (nMode == MODE_PROHIBIT) m_rdoMainProhibit.SetCheck(TRUE);
		if (nMode == MODE_PARAM) m_rdoMainParm.SetCheck(TRUE);
	}
#endif
}

void CCME8000Dlg::OnBnClickedRdoMainProhibit()
{
	Set_CurrentMode(MODE_PROHIBIT);
}

void CCME8000Dlg::OnBnClickedBtnMainMonitor()
{
	theApp.bIoMode = FALSE;
	theApp.bAlarmMode = FALSE;
	if (g_dlgMonitor.IsWindowVisible()) g_dlgMonitor.ShowWindow(SW_HIDE);
	else g_dlgMonitor.ShowWindow(SW_SHOW);	
}

void CCME8000Dlg::OnBnClickedBtnMainIO()
{
	theApp.bIoMode = TRUE;
	theApp.bAlarmMode = FALSE;
	if (g_dlgMonitor.IsWindowVisible()) g_dlgMonitor.ShowWindow(SW_HIDE);
	g_dlgMonitor.ShowWindow(SW_SHOW);	
}

void CCME8000Dlg::OnBnClickedBtnMainAlarm()
{
	theApp.bIoMode = FALSE;
	theApp.bAlarmMode = TRUE;
	if (g_dlgMonitor.IsWindowVisible()) g_dlgMonitor.ShowWindow(SW_HIDE);
	g_dlgMonitor.ShowWindow(SW_SHOW);	
}

void CCME8000Dlg::OnBnClickedBtnMainExit()
{
	if (g_objCommon.Show_MsgBox(2, "Do you want to exit the program?") != IDOK) return;
	Exit_System(EXIT_SYSTEM_NONE);
}

void CCME8000Dlg::OnStnClickedStcMainVer()
{
	if (g_dlgVersion.IsWindowVisible()) {
		g_dlgVersion.ShowWindow(FALSE);

	} else {
		int nMode = theApp.Get_MainMode();
		int nState = theApp.Get_MainState();
		if (nMode != MODE_WORK) return;
		if (nState != STATE_NONE && nState != STATE_STOP) return;
		g_dlgVersion.ShowWindow(TRUE);
	}
}

void CCME8000Dlg::OnDblclkStcMainEquip()
{
	CWnd *pDeskTopWnd = GetDesktopWindow();
	if (!pDeskTopWnd) return;
	CWindowDC DeskTopDC(pDeskTopWnd);

	CImage imgScreen;
	int nX = GetSystemMetrics(SM_CXSCREEN);
	int nY = GetSystemMetrics(SM_CYSCREEN);
	if (!imgScreen.Create(nX, nY, 32)) return;

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strLog, strMsg, strFile, strPath = "D:\\Screen";
	g_objLogFile.Create_Folder(strPath);
	strFile.Format("%s\\%04d%02d%02d_%02d%02d%02d.png", strPath, time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);

	HDC hDC = imgScreen.GetDC();
	BitBlt(hDC, 0, 0, nX, nY, DeskTopDC, 0, 0, SRCCOPY);

	imgScreen.Save(strFile, Gdiplus::ImageFormatPNG);
	imgScreen.ReleaseDC();

	strLog.Format("%s Screen Capture Completed.", strFile);
	g_objLogFile.Save_HandlerLog(strLog);

	strMsg.Format("%s\n\nScreen Capture Completed.", strFile);
	g_objCommon.Show_MsgBox(1, strMsg);
}

/////////////////////////////////////////////////////////////////////////////
// User Functions

void CCME8000Dlg::Initial_Controls() 
{
	m_bmpLgLogo.LoadBitmap(IDB_LGINNOTEK_LOGO);
	m_imgLgLogo.SetBitmap(m_bmpLgLogo);
	m_imgLgLogo.SetWindowPos(this, 5, 8, 315, 60, SWP_NOZORDER);

	m_picMainLogo.Init_Ctrl(COLOR_DEFAULT, RGB(0xFF, 0xFF, 0xFF));
	m_stcMainEquip.Init_Ctrl("Segoe UI", 14, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xE6, 0xE6, 0xE6));
	m_stcMainMode.Init_Ctrl("Segoe UI", 14, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xE6, 0xE6, 0xE6));
	m_stcMainOpEng.Init_Ctrl("Segoe UI", 14, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xE6, 0xE6, 0xE6));
	m_stcMainDate.Init_Ctrl("Segoe UI", 14, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xE6, 0xE6, 0xE6));
	m_stcMainTime.Init_Ctrl("Segoe UI", 14, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xE6, 0xE6, 0xE6));
	m_stcMainVer.Init_Ctrl("Segoe UI", 14, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xE6, 0xE6, 0xE6));
	for (int i = 0; i < 3; i++) m_stcMainTower[i].Init_Ctrl("Segoe UI", 14, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xFF, 0xFF));

#ifdef DRY_RUN_TEST
	m_stcMainEquip.Init_Ctrl("Segoe UI", 14, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0xFF, 0x00, 0x00));
#endif
#ifdef VISION_REPEAT
	m_stcMainEquip.Init_Ctrl("Segoe UI", 14, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x00, 0xFF));
#endif

	m_picModeBack.Init_Ctrl(COLOR_DEFAULT, COLOR_DEFAULT);
	m_btnMainOperator.Init_Ctrl("Segoe UI", 14, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	m_rdoMainParm.Init_Ctrl("Segoe UI", 14, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	m_rdoMainWork.Init_Ctrl("Segoe UI", 14, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	m_rdoMainManual.Init_Ctrl("Segoe UI", 14, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	m_rdoMainSetup.Init_Ctrl("Segoe UI", 14, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	m_rdoMainProhibit.Init_Ctrl("Segoe UI", 14, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	m_btnMainMonitor.Init_Ctrl("Segoe UI", 14, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	m_btnMainIO.Init_Ctrl("Segoe UI", 14, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	m_btnMainAlarm.Init_Ctrl("Segoe UI", 14, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	m_btnMainExit.Init_Ctrl("Segoe UI", 14, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
}

void CCME8000Dlg::Hide_ModeWindows(int nMode, int nPreMode)
{
	m_rdoMainParm.Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
	m_rdoMainWork.Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
	m_rdoMainManual.Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
	m_rdoMainSetup.Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
	m_rdoMainProhibit.Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);

	int nShow = (nMode == MODE_OPERATOR ? SW_HIDE : SW_SHOW);
	if (nShow == SW_SHOW) nShow = (nMode == MODE_INITIAL ? SW_HIDE : SW_SHOW);

	m_rdoMainParm.ShowWindow(nShow);
	m_rdoMainWork.ShowWindow(nShow);
	m_rdoMainManual.ShowWindow(nShow);
	m_rdoMainSetup.ShowWindow(nShow);
	m_rdoMainProhibit.ShowWindow(nShow);
	m_btnMainMonitor.ShowWindow(nShow);
	m_btnMainIO.ShowWindow(nShow);
	m_btnMainAlarm.ShowWindow(nShow);
	if (nMode == MODE_OPERATOR) m_btnMainExit.ShowWindow(FALSE);
	else						m_btnMainExit.ShowWindow(TRUE);

	if (nPreMode == MODE_OPERATOR) g_dlgOperator.ShowWindow(SW_HIDE);
	if (nPreMode == MODE_INITIAL) g_dlgInitial.ShowWindow(SW_HIDE);
	if (nPreMode == MODE_PARAM) g_dlgSetup.ShowWindow(SW_HIDE);
	if (nPreMode == MODE_WORK) g_dlgWork.ShowWindow(SW_HIDE);
	if (nPreMode == MODE_MANUAL) g_dlgManual.ShowWindow(SW_HIDE);
	if (nPreMode == MODE_SETUP) g_dlgSetup.ShowWindow(SW_HIDE);
	if (nPreMode == MODE_PROHIBIT) g_dlgProhibit.ShowWindow(SW_HIDE);
}

///////////////////////////////////////////////////////////////////////////////

void CCME8000Dlg::Set_CurrentMode(int nMode)
{
	int nPreMode = theApp.Get_MainMode();
	if (nMode == nPreMode) return;

	Hide_ModeWindows(nMode, nPreMode);

	if (nMode == MODE_OPERATOR) m_btnMainOperator.SetWindowText("Engineer");
	else m_btnMainOperator.SetWindowText("Operator");

	if (nMode == MODE_OPERATOR) {
		g_dlgOperator.ShowWindow(SW_SHOW);
		m_stcMainMode.SetWindowText("Operator");
		m_stcMainOpEng.SetWindowText("Operator Mode");
		g_objLogFile.Save_HandlerLog("[Main Dialog] Operator Mode start");

	} else if (nMode == MODE_INITIAL) {
		g_dlgInitial.ShowWindow(SW_SHOW);
		m_stcMainMode.SetWindowText("Initial");
		m_stcMainOpEng.SetWindowText("Engineer Mode");
		g_objLogFile.Save_HandlerLog("[Main Dialog] Initial Mode start");

	} else if (nMode == MODE_WORK) {
		g_dlgWork.ShowWindow(SW_SHOW);
		m_stcMainMode.SetWindowText("Work");
		m_stcMainOpEng.SetWindowText("Engineer Mode");
		m_rdoMainWork.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);
		if (!m_rdoMainWork.GetCheck()) m_rdoMainWork.SetCheck(TRUE);
		g_objLogFile.Save_HandlerLog("[Main Dialog] Work Mode start");

	} else if (nMode == MODE_MANUAL) {
		g_dlgManual.ShowWindow(SW_SHOW);
		m_stcMainMode.SetWindowText("Manual");
		m_stcMainOpEng.SetWindowText("Engineer Mode");
		m_rdoMainManual.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);
		if (!m_rdoMainManual.GetCheck()) m_rdoMainManual.SetCheck(TRUE);
		g_objLogFile.Save_HandlerLog("[Main Dialog] Manual Mode start");

	} else if (nMode == MODE_SETUP) {
		g_dlgSetup.ShowWindow(SW_SHOW);
		m_stcMainMode.SetWindowText("Setup");
		m_stcMainOpEng.SetWindowText("Engineer Mode");
		m_rdoMainSetup.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);
		if (!m_rdoMainSetup.GetCheck()) m_rdoMainSetup.SetCheck(TRUE);
		g_objLogFile.Save_HandlerLog("[Main Dialog] Setup Mode start");

	} else if (nMode == MODE_PROHIBIT) {
		g_dlgProhibit.ShowWindow(SW_SHOW);
		m_stcMainMode.SetWindowText("Prohibit");
		m_stcMainOpEng.SetWindowText("Engineer Mode");
		m_rdoMainProhibit.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);
		if (!m_rdoMainProhibit.GetCheck()) m_rdoMainProhibit.SetCheck(TRUE);
		g_objLogFile.Save_HandlerLog("[Main Dialog] Prohibit Start");
	} else if (nMode == MODE_PARAM) {
		g_dlgSetup.ShowWindow(SW_SHOW);
		m_stcMainMode.SetWindowText("Setup");
		m_stcMainOpEng.SetWindowText("Engineer Mode");
		m_rdoMainParm.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);
		if (!m_rdoMainParm.GetCheck()) m_rdoMainParm.SetCheck(TRUE);
		g_objLogFile.Save_HandlerLog("[Main Dialog] Param Mode start");
	}

	theApp.Set_MainMode(nMode);
}

void CCME8000Dlg::Set_CurrentState(int nState)
{
	KillTimer(TIMER_TOWER_FLKR);
	KillTimer(TIMER_BUZZER_FLKR);
//	KillTimer(TIMER_LAMP_FLKR);

	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	DY_DATA_12 *pDY12 = g_objAJinAXL.Get_pDY12();

	// Start, Stop, Reset SW
	switch (nState) {
	case STATE_NONE:
		pDY12->oStartLamp1 = pDY12->oStartLamp2 = FALSE;
		pDY12->oStopLamp1  = pDY12->oStopLamp2 = FALSE;
		pDY12->oResetLamp1 = pDY12->oResetLamp2 = FALSE;
		break;
	case STATE_INIT:
		pDY12->oStartLamp1 = pDY12->oStartLamp2 = TRUE;
		pDY12->oStopLamp1  = pDY12->oStopLamp2 = TRUE;
		pDY12->oResetLamp1 = pDY12->oResetLamp2 = FALSE;
		break;
	case STATE_RUN:
		pDY12->oStartLamp1 = pDY12->oStartLamp2 = TRUE;
		pDY12->oStopLamp1  = pDY12->oStopLamp2 = FALSE;
		pDY12->oResetLamp1 = pDY12->oResetLamp2 = FALSE;
		break;
	case STATE_STOP:
		pDY12->oStartLamp1 = pDY12->oStartLamp2 = FALSE;
		pDY12->oStopLamp1  = pDY12->oStopLamp2 = TRUE;
		pDY12->oResetLamp1 = pDY12->oResetLamp2 = FALSE;
		break;
	case STATE_ALARM:
	case STATE_ERROR:
	case STATE_LOTEND:
	case STATE_SHIPTRAY:
	case STATE_CAPTRAY:
		pDY12->oStartLamp1 = pDY12->oStartLamp2 = FALSE;
		pDY12->oStopLamp1  = pDY12->oStopLamp2 = FALSE;
		pDY12->oResetLamp1 = pDY12->oResetLamp2 = TRUE;
		break;
	}
	g_objAJinAXL.Write_Output(12);
	Set_LotStateTime();
	// Tower
	m_bTowerOn = TRUE;
	pDY12->oTowerGreen = pEquipData->bTower[nState][0];
	pDY12->oTowerYellow = pEquipData->bTower[nState][1];
	pDY12->oTowerRed = pEquipData->bTower[nState][2];

	// Tower Flicker
	if (pEquipData->bTower[nState][3]) SetTimer(TIMER_TOWER_FLKR, 500, NULL);

	// Buzzer
	if (nState == STATE_ALARM || nState == STATE_ERROR ||
		nState == STATE_LOTEND || nState == STATE_CAPTRAY || nState == STATE_SHIPTRAY) {
		m_bBuzzerOn = TRUE;
#ifndef DRY_RUN_TEST	// 시끄러워서 막음
		EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
// temp
// 		if (gData.bEmptyFull) {
// 			pDY12->oBuzzerBit0 = TRUE;	//pEquipData->bBuzzer[1][0];
// 			pDY12->oBuzzerBit1 = TRUE;	//pEquipData->bBuzzer[1][1];
// 			pDY12->oBuzzerBit2 = FALSE;	//pEquipData->bBuzzer[1][2];
// 			pDY12->oBuzzerBit3 = FALSE;	//pEquipData->bBuzzer[1][3];
// 			pDY12->oBuzzerBit4 = FALSE;	//pEquipData->bBuzzer[1][4];
// 		} else {
			pDY12->oBuzzerBit0 = pEquipData->bBuzzer[nState - STATE_ALARM][0];
			pDY12->oBuzzerBit1 = pEquipData->bBuzzer[nState - STATE_ALARM][1];
			pDY12->oBuzzerBit2 = pEquipData->bBuzzer[nState - STATE_ALARM][2];
			pDY12->oBuzzerBit3 = pEquipData->bBuzzer[nState - STATE_ALARM][3];
			pDY12->oBuzzerBit4 = pEquipData->bBuzzer[nState - STATE_ALARM][4];
//		}
#endif
		// Buzzer Flicker
		if (pEquipData->bBuzzer[nState - STATE_ALARM][5]) SetTimer(TIMER_BUZZER_FLKR, 500, NULL);
	}
	g_objAJinAXL.Write_Output(12);

	// Load/Unload Lamp
	if (nState == STATE_RUN) {
		SetTimer(TIMER_LOAD1_LAMP_FLKR, 500, NULL);
		SetTimer(TIMER_LOAD2_LAMP_FLKR, 500, NULL);
		SetTimer(TIMER_NG_LAMP_FLKR, 500, NULL);
		SetTimer(TIMER_GOOD_LAMP_FLKR, 500, NULL);
		SetTimer(TIMER_EMPTY_LAMP_FLKR, 500, NULL);
	}

	theApp.Set_MainState(nState);
	g_dlgWork.Set_State(nState);
}

void CCME8000Dlg::Set_InsideLight()
{
	DX_DATA_13 *pDX13 = g_objAJinAXL.Get_pDX13();
	DY_DATA_13 *pDY13 = g_objAJinAXL.Get_pDY13();

	int nDoorState = (pDX13->nValue >> 0);
	BOOL bLoad = g_objSequenceMain.Get_LotLoadEnable(0);

	if (nDoorState == 0 || (bLoad && nDoorState == 3)) {
		if (m_bInsideLight) {
			m_bInsideLight = FALSE;
			pDY13->oInsideLight = FALSE;
			g_objAJinAXL.Write_Output(13);
		}
	} else {
		if (!m_bInsideLight) {
			m_bInsideLight = TRUE;
			pDY13->oInsideLight = TRUE;
			g_objAJinAXL.Write_Output(13);
		}
	}
}

void CCME8000Dlg::Set_TowerFlicker(BOOL bEnable)
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	DY_DATA_12 *pDY12 = g_objAJinAXL.Get_pDY12();

	if (m_bTowerOn || !bEnable) {
		m_bTowerOn = FALSE;
		pDY12->oTowerGreen = FALSE;
		pDY12->oTowerYellow = FALSE;
		pDY12->oTowerRed = FALSE;
		if (!bEnable) KillTimer(TIMER_TOWER_FLKR);

	} else {
		m_bTowerOn = TRUE;
		int nState = theApp.Get_MainState();
		pDY12->oTowerGreen = pEquipData->bTower[nState][0];
		pDY12->oTowerYellow = pEquipData->bTower[nState][1];
		pDY12->oTowerRed = pEquipData->bTower[nState][2];
	}
	g_objAJinAXL.Write_Output(12);
}

void CCME8000Dlg::Set_BuzzerFlicker(BOOL bEnable)
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	DY_DATA_12 *pDY12 = g_objAJinAXL.Get_pDY12();

	if (m_bBuzzerOn || !bEnable) {
		m_bBuzzerOn = FALSE;
		pDY12->oBuzzerBit0 = FALSE;
		pDY12->oBuzzerBit1 = FALSE;
		pDY12->oBuzzerBit2 = FALSE;
		pDY12->oBuzzerBit3 = FALSE;
		pDY12->oBuzzerBit4 = FALSE;
		if (!bEnable) KillTimer(TIMER_BUZZER_FLKR);

	} else {
		m_bBuzzerOn = TRUE;
#ifndef DRY_RUN_TEST	// 시끄러워서 막음
		int nState = theApp.Get_MainState();
		pDY12->oBuzzerBit0 = pEquipData->bBuzzer[nState-STATE_ALARM][0];
		pDY12->oBuzzerBit1 = pEquipData->bBuzzer[nState-STATE_ALARM][1];
		pDY12->oBuzzerBit2 = pEquipData->bBuzzer[nState-STATE_ALARM][2];
		pDY12->oBuzzerBit3 = pEquipData->bBuzzer[nState-STATE_ALARM][3];
		pDY12->oBuzzerBit4 = pEquipData->bBuzzer[nState-STATE_ALARM][4];
#endif
	}
	g_objAJinAXL.Write_Output(12);
}

void CCME8000Dlg::Set_LampFlicker_Load1(BOOL bEnable)
{
	DY_DATA_12 *pDY12 = g_objAJinAXL.Get_pDY12();

	if (m_bLampOnLoad1 || !bEnable) {
		m_bLampOnLoad1 = FALSE;
		pDY12->oLoad1Lamp = FALSE;
//		if (!bEnable) KillTimer(TIMER_LOAD1_LAMP_FLKR);

	} else {
		m_bLampOnLoad1 = TRUE;
		pDY12->oLoad1Lamp = TRUE;
	}
	g_objAJinAXL.Write_Output(12);
}

void CCME8000Dlg::Set_LampFlicker_Load2(BOOL bEnable)
{
	DY_DATA_12 *pDY12 = g_objAJinAXL.Get_pDY12();

	if (m_bLampOnLoad2 || !bEnable) {
		m_bLampOnLoad2 = FALSE;
		pDY12->oLoad2Lamp = FALSE;
//		if (!bEnable) KillTimer(TIMER_LOAD2_LAMP_FLKR);

	} else {
		m_bLampOnLoad2 = TRUE;
		pDY12->oLoad2Lamp = TRUE;
	}
	g_objAJinAXL.Write_Output(12);
}

void CCME8000Dlg::Set_LampFlicker_Load3(BOOL bEnable)
{
	DY_DATA_12 *pDY12 = g_objAJinAXL.Get_pDY12();

	if (m_bLampOnLoad3 || !bEnable) {
		m_bLampOnLoad3 = FALSE;
		pDY12->oLoad3Lamp = FALSE;
//		if (!bEnable) KillTimer(TIMER_LOAD2_LAMP_FLKR);

	} else {
		m_bLampOnLoad3 = TRUE;
		pDY12->oLoad3Lamp = TRUE;
	}
	g_objAJinAXL.Write_Output(12);
}

void CCME8000Dlg::Set_LampFlicker_Cap1(BOOL bEnable)
{
	DY_DATA_12 *pDY12 = g_objAJinAXL.Get_pDY12();

	if (m_bLampOnCap1 || !bEnable) {
		m_bLampOnCap1 = FALSE;
		pDY12->oCap1Lamp = FALSE;
//		if (!bEnable) KillTimer(TIMER_NG_LAMP_FLKR);

	} else {
		m_bLampOnCap1 = TRUE;
		pDY12->oCap1Lamp = TRUE;
	}
	g_objAJinAXL.Write_Output(12);
}

void CCME8000Dlg::Set_LampFlicker_Cap2(BOOL bEnable)
{
	DY_DATA_12 *pDY12 = g_objAJinAXL.Get_pDY12();

	if (m_bLampOnCap2 || !bEnable) {
		m_bLampOnCap2 = FALSE;
		pDY12->oCap2Lamp = FALSE;
//		if (!bEnable) KillTimer(TIMER_NG_LAMP_FLKR);

	} else {
		m_bLampOnCap2 = TRUE;
		pDY12->oCap2Lamp = TRUE;
	}
	g_objAJinAXL.Write_Output(12);
}

void CCME8000Dlg::Set_LampFlicker_Unload1(BOOL bEnable)
{
	DY_DATA_12 *pDY12 = g_objAJinAXL.Get_pDY12();

	if (m_bLampOnUnload1 || !bEnable) {
		m_bLampOnUnload1 = FALSE;
		pDY12->oUnload1Lamp = FALSE;
//		if (!bEnable) KillTimer(TIMER_GOOD_LAMP_FLKR);

	} else {
		m_bLampOnUnload1 = TRUE;
		pDY12->oUnload1Lamp = TRUE;
	}
	g_objAJinAXL.Write_Output(12);
}

void CCME8000Dlg::Set_LampFlicker_Unload2(BOOL bEnable)
{
	DY_DATA_12 *pDY12 = g_objAJinAXL.Get_pDY12();

	if (m_bLampOnUnload2 || !bEnable) {
		m_bLampOnUnload2 = FALSE;
		pDY12->oUnload2Lamp = FALSE;
//		if (!bEnable) KillTimer(TIMER_GOOD_LAMP_FLKR);

	} else {
		m_bLampOnUnload2 = TRUE;
		pDY12->oUnload2Lamp = TRUE;
	}
	g_objAJinAXL.Write_Output(12);
}

void CCME8000Dlg::Set_NoWork()
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	// 	if (pEquipData->nNoWorkTime < 1 || gMes.bNoWorkStart){ m_dwNoWorkTime = GetTickCount(); return; }
	if (pEquipData->nNoWorkTime < 1 || g_dlgNoWork.m_bStart){ m_dwNoWorkTime = GetTickCount(); return; }

	int nState = theApp.Get_MainState();
	if (nState != STATE_NONE && nState != STATE_STOP) { m_dwNoWorkTime = GetTickCount(); return; }

	if (g_dlgNoWork.IsWindowVisible()) { m_dwNoWorkTime = GetTickCount(); return; }

	int nTerm = (int)(GetTickCount() - m_dwNoWorkTime);
	if (nTerm < pEquipData->nNoWorkTime * 1000) return;	// 초 -> 밀리초

	g_dlgNoWork.m_bStart = TRUE;

	CTime CurTime = CTime::GetCurrentTime(); g_dlgNoWork.m_dwStartTime = GetTickCount();
	g_dlgNoWork.m_strStartTime.Format("%04d%02d%02d%02d%02d%02d", CurTime.GetYear(), CurTime.GetMonth(), CurTime.GetDay(), CurTime.GetHour(), CurTime.GetMinute(), CurTime.GetSecond());

	CString strLog;
	strLog.Format("비가동 집계 시작\t%s", g_dlgNoWork.m_strStartTime);
	g_objLogFile.Save_HandlerLog(strLog);

	g_dlgNoWork.ShowWindow(TRUE);
}

void CCME8000Dlg::Enable_ModeButton(BOOL bEnable)
{
	m_btnMainOperator.EnableWindow(bEnable);
	m_rdoMainWork.EnableWindow(bEnable);
	m_rdoMainManual.EnableWindow(bEnable);
	m_rdoMainSetup.EnableWindow(bEnable);
	m_rdoMainProhibit.EnableWindow(bEnable);
	m_rdoMainParm.EnableWindow(bEnable);
}

void CCME8000Dlg::Display_EquipName()
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	CString strText = pEquipData->sEquipName + " ( " + gData.sRecipeName + " )";
	m_stcMainEquip.SetWindowText(strText);
}

void CCME8000Dlg::Display_DateTime()
{
	CTime datetime = CTime::GetCurrentTime();

	CString strDate = datetime.Format("%Y-%m-%d");
	m_stcMainDate.SetWindowText(strDate);

	CString strTime = datetime.Format("%H:%M:%S");
	m_stcMainTime.SetWindowText(strTime);

	DY_DATA_12 *pDY12 = g_objAJinAXL.Get_pDY12();
	COLORREF crTowerR = (pDY12->oTowerRed ? RGB(0xFF, 0x00, 0x00) : RGB(0xF0, 0xF0, 0xF0));
	COLORREF crTowerY = (pDY12->oTowerYellow ? RGB(0xFF, 0xFF, 0x00) : RGB(0xF0, 0xF0, 0xF0));
	COLORREF crTowerG = (pDY12->oTowerGreen ? RGB(0x00, 0xFF, 0x00) : RGB(0xF0, 0xF0, 0xF0));
	m_stcMainTower[0].Set_Color(COLOR_DEFAULT, crTowerR);
	m_stcMainTower[1].Set_Color(COLOR_DEFAULT, crTowerY);
	m_stcMainTower[2].Set_Color(COLOR_DEFAULT, crTowerG);

	static int nUphClear = 0;
	if (datetime.GetHour() == 7) {	// 클리어
		if (nUphClear == 1) return;
		nUphClear = 1;
		for (int i = 0; i < 24; i++) gUph.nLotCount[i] = 0;
		for (int i = 0; i < 2; i++) gUph.nCmCount[i] = 0;
		Save_EquipRunTime();
		Clear_EquipRunTime();
	} else nUphClear = 0;

	 if (gIt.nWriteHH != datetime.GetHour() && gData.sComName.GetLength() > 0) {
		g_objLogFile.Save_Interlock(1);
	 }
}

void CCME8000Dlg::Exit_System(int nExitNo) 
{
	if (nExitNo == EXIT_SYSTEM_AJIN) g_objCommon.Show_MsgBox(1, "Fail to motion initialization.");
	if (nExitNo == EXIT_SYSTEM_BARCODE) g_objCommon.Show_MsgBox(1, "Fail to barcode initialization.");
	if (nExitNo == EXIT_SYSTEM_MODEL) g_objCommon.Show_MsgBox(1, "Fail to Model Load.");

	Save_EquipCappingCnt();	// Capping한 수량 저장
	g_objLogFile.Save_HandlerLog("[Main Dialog] Program Exit");
	g_dlgWork.MachineStopLog("PROGRAM_EXIT");

	Set_LotErrorLog("PROGRAM EXIT", 909, "Program Exit");	// 종료시 마지막 로그 기록하기 위해...

	Set_CurrentState(STATE_NONE);

	DY_DATA_13 *pDY13 = g_objAJinAXL.Get_pDY13();
	pDY13->oInsideLight = FALSE;
	g_objAJinAXL.Write_Output(13);

	g_objBarcodeLot.Terminate();
	g_objLoadCell.Terminate();
	g_objAviUDP.Terminate();
	g_objInspector.Terminate();
	g_objAJinAXL.Terminate();

	KillTimer(TIMER_BUZZER_FLKR);
	KillTimer(TIMER_TOWER_FLKR);
	KillTimer(TIMER_DATE_TIME);
	KillTimer(TIMER_DOOR_LOCK);

	DestroyWindow();
}

void CCME8000Dlg::Gloval_Initial()
{
	gUph.dTaktTime = 0.0;
	for (int i = 0; i < 24; i++) for (int j = 0; j < 50; j++) gUph.dTakt[i][j] = 0.0;
	for (int i = 0; i < 24; i++) gUph.nLotCount[i] = 0;
	for (int i = 0; i < 2; i++) gUph.nCmCount[i] = 0;
}

void CCME8000Dlg::Set_EquipRunStart()
{
	m_dwEquipRunStart = GetTickCount();
	g_objLogFile.Save_HandlerLog("[Equipment Run Time] Start Run Time");
}

void CCME8000Dlg::Save_EquipRunTime()
{
	if (m_dwEquipRunStart == 0) return;

	CString strTemp;
	DWORD dwTime;
	int nRunTime = 0;
	
	strTemp = "\\System\\EquipData.ini";
	CIniFileCS INIA(gsCurrentDir + strTemp);
	nRunTime = INIA.Get_Integer("EQUIPMENT", "RUN_TIME", 0);
	dwTime = GetTickCount() - m_dwEquipRunStart;
	nRunTime += (int)(dwTime);
	INIA.Set_Integer("EQUIPMENT", "RUN_TIME", nRunTime);	

	CString strLog;
	int nHour = nRunTime / 3600000;
	int nMin = (nRunTime - nHour * 3600000) / 60000;
	int nSec = (nRunTime - nHour * 3600000 - nMin * 60000) / 1000;
	strLog.Format("[Equipment Run Time] Save Run Time ==> %02d:%02d:%02d", nHour, nMin, nSec);
	g_objLogFile.Save_HandlerLog(strLog);
	g_objLogFile.Save_SaveRunTimeLog(strLog);

	m_dwEquipRunStart = 0;
}

void CCME8000Dlg::Clear_EquipRunTime()
{
	CString strTemp;
	strTemp = "\\System\\EquipData.ini";
	CIniFileCS INI(gsCurrentDir + strTemp);
	INI.Set_Integer("EQUIPMENT", "RUN_TIME", 0);
	
	m_dwEquipRunStart = 0;

	g_objLogFile.Save_HandlerLog("[Equipment Run Time] Clear Run Time");
}

void CCME8000Dlg::Save_EquipCappingCnt()
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	CIniFileCS INIA(gsCurrentDir + "\\System\\EquipData.ini");
	INIA.Set_Integer("LOAD_CELL", "CAPPING_CNT", pEquipData->nCappingCnt);

	CString strKey;
	for (int i = 0; i < gData.nPickerUseCnt; i++) {
		strKey.Format("%d", i); INIA.Set_Double("ASSY_LOAD_CELL", strKey, gData.dAssyLoadCell[i]);
	}

	CString strLog;
	strLog.Format("[Equipment Capping Count] %d", pEquipData->nCappingCnt);
	g_objLogFile.Save_HandlerLog(strLog);
}

void CCME8000Dlg::Clear_EquipCapTrayCnt()
{
	CString strTemp;
	strTemp = "\\System\\EquipData.ini";
	CIniFileCS INI(gsCurrentDir + strTemp);
	INI.Set_Integer("EQUIPMENT", "CAP_TRAY_CNT", 0);

	g_objLogFile.Save_HandlerLog("[Equipment Cap Tray] Clear Cap Tray Count");
}

void CCME8000Dlg::Set_ScreenOff()
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	if (pEquipData->nScreenOff < 1 || m_bScreenOff) { m_dwScreenTime = GetTickCount(); return; }

	int nMode = theApp.Get_MainMode();
	int nState = theApp.Get_MainState();
	if (nMode != MODE_WORK && nMode != MODE_PROHIBIT) { m_dwScreenTime = GetTickCount(); return; }
	if (nState != STATE_NONE && nState != STATE_STOP) { m_dwScreenTime = GetTickCount(); return; }

	int nTerm = (int)(GetTickCount() - m_dwScreenTime);
	if (nTerm < pEquipData->nScreenOff * 60 * 1000) return;	// 분 -> 밀리초
	//if (GetTickCount() - m_dwScreenTime < 10000) return;	// 테스트 (10초)

	m_bScreenOff = TRUE;
	g_objLogFile.Save_HandlerLog("Monitor Screen Auto Off.");
	PostMessage(WM_SYSCOMMAND, SC_MONITORPOWER, (LPARAM)2);	// Off
	Set_CurrentMode(MODE_PROHIBIT);
}

void CCME8000Dlg::Set_ScreenOn()
{
	m_dwScreenTime = GetTickCount();
	if (!m_bScreenOff) return;

	m_bScreenOff = FALSE;
	g_objLogFile.Save_HandlerLog("Monitor Screen Auto On.");
	//PostMessage(WM_SYSCOMMAND, SC_MONITORPOWER, (LPARAM)-1);	// ON
}

void CCME8000Dlg::Set_LotErrorLog(CString sEvent, int nErrCode, CString sMessage, int nPNo)
{
	CString strModel, strAction, strLog;
	int nNo = nPNo-1;
	if (nNo < 0) {
		nNo = gData.nULPNo-1;
		if (nNo < 0) nNo = gData.nLPNo-1;
		if (nNo < 0) nNo = 0;
	}


	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	// 	strModel = (pEquipData->nEquipModel == 3 ? "Iago" : (pEquipData->nEquipModel == 2 ? "Titus" : (pEquipData->nEquipModel == 1 ? "Romeo-F" : "Romeo-P")));
	strModel = "EOL";

	if (gData.sLotID[nNo] == "") gData.sLotID[nNo] = "CLOT_ID";

	strAction.Format("%0.3lf", (GetTickCount() - m_dwEquipProcTime) / 1000.0);
	m_dwEquipProcTime = GetTickCount();

	m_strEquipEvent.Format("%s,%04d,%s", sEvent, nErrCode, sMessage);
	strLog.Format("%s,%s,%s,%s,%s", strModel, MAIN_VERSION, gData.sLotID[nNo], m_strEquipEvent, strAction);
	g_objLogFile.Save_LotError(strLog, nPNo);
}

///////////////////////////////////////////////////////////////////////////////
void CCME8000Dlg::Set_LotStateTime()
{
	// 	if (!g_objSequenceMain.Get_IsAutoRun())return;
	if (m_dwSetTimer == 0) { m_dwSetTimer = GetTickCount(); return; }

	int nPreState = theApp.Get_MainState();
	DWORD dwTime = GetTickCount() - m_dwSetTimer;

	switch(nPreState) {
	case STATE_RUN:
		gLot.dwRunTime += dwTime; break;
	case STATE_ALARM:
	case STATE_ERROR:
		/*gLot.dwErrorTime += dwTime;	gLot.nErrorCount++; break;*/
		break;
	default:
		if (!gAlm.bBegin) gLot.dwStopTime += dwTime; break;
	}
	m_dwSetTimer = GetTickCount();
}

void CCME8000Dlg::Set_DoorLock()
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	if (pEquipData->bUseDoorLock) return;
	if (gData.dwDoorStartTime <= 0) gData.dwDoorStartTime = GetTickCount();

	DWORD dwCurrentTime = GetTickCount();
	DWORD dwDoorEndTime = DWORD(gData.nDoorLockTime) * 60 * 1000;	//분
	if (dwCurrentTime - gData.dwDoorStartTime >= dwDoorEndTime) {
		CIniFileCS INI(gsCurrentDir + "\\System\\EquipData.ini");
		if (!INI.Check_File()) return;
		INI.Set_Bool("EQUIPMENT", "DOOR_LOCK", TRUE);

		g_objLogFile.Save_HandlerLog("Door Lock을 Auto로 설정 하였습니다...");

		gData.dwDoorStartTime = 0;
		g_objDataManager.Read_EquipData();

		g_dlgWork.PostMessage(UM_SHOW_MSG, 99, NULL);
	}
}
