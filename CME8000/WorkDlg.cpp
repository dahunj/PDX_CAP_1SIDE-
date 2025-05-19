// WorkDlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CME8000.h"
#include "WorkDlg.h"
#include "afxdialogex.h"

#include "LogFile.h"
#include "Common.h"

#include "Inspector.h"
#include "AviUDP.h"
#include "BarcodeLot.h"
#include "LoadCell.h"
#include "MESInterface.h"
#include "SequenceInit.h"
#include "SequenceMain.h"

#include "OperatorDlg.h"
#include "CME8000Dlg.h"
#include "NoWorkDlg.h"

// CWorkDlg 대화 상자입니다.
CWorkDlg g_dlgWork;

IMPLEMENT_DYNAMIC(CWorkDlg, CDialogEx)

CWorkDlg::CWorkDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWorkDlg::IDD, pParent)
{
}

CWorkDlg::~CWorkDlg()
{
}

void CWorkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for (int i = 0; i < 10; i++) DDX_Control(pDX, IDC_GROUP_0 + i, m_Group[i]);
	for (int i = 0; i < 3; i++) DDX_Control(pDX, IDC_LABEL_0 + i, m_Label[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_GRP_LOT + i, m_grpLot[i]);
	for (int i = 0; i < 6; i++) DDX_Control(pDX, IDC_LBL_LOT_0 + i, m_lblLot[i]);
	DDX_Control(pDX, IDC_BTN_CAP_CLEAR, m_btnCapClear);
	DDX_Control(pDX, IDC_BTN_SHIP_CLEAR, m_btnShipClear);
	DDX_Control(pDX, IDC_BTN_MES_CANCEL, m_btnMesCancel);
	DDX_Control(pDX, IDC_STC_LOT_ID_R, m_stcLotIdR);
	DDX_Control(pDX, IDC_STC_TRAY_COUNT_R, m_stcTrayCountR);
	DDX_Control(pDX, IDC_STC_CM_COUNT_R, m_stcCmCountR);
	DDX_Control(pDX, IDC_STC_LOT_ID_S, m_stcLotIdS);
	DDX_Control(pDX, IDC_STC_TRAY_COUNT_S, m_stcTrayCountS);
	DDX_Control(pDX, IDC_STC_CM_COUNT_S, m_stcCmCountS);
	DDX_Control(pDX, IDC_IMG_EQUIPMENT, m_imgEquipment);
	DDX_Control(pDX, IDC_PIC_UPH_BACK, m_picUphBack);

	for (int i = 0; i < 6; i++) DDX_Control(pDX, IDC_LBL_CAP_LOT_0 + i, m_lblCapLot[i]);
	DDX_Control(pDX, IDC_STC_CAP_LOT_ID, m_stcCapLotId);
	DDX_Control(pDX, IDC_STC_CAP_TRAY_COUNT, m_stcCapTrayCnt);
	DDX_Control(pDX, IDC_STC_CAP_COUNT, m_stcCapCnt);
	DDX_Control(pDX, IDC_STC_CAP_LOT_ID2, m_stcCapLotIdS);
	DDX_Control(pDX, IDC_STC_CAP_TRAY_COUNT2, m_stcCapTrayCntS);
	DDX_Control(pDX, IDC_STC_CAP_COUNT2, m_stcCapCntS);

	DDX_Control(pDX, IDC_STC_SHIP_LOT_ID, m_stcShipLotId);
	DDX_Control(pDX, IDC_STC_SHIP_TRAY_COUNT, m_stcShipTrayCnt);
	DDX_Control(pDX, IDC_STC_SHIP_COUNT, m_stcShipCnt);
	DDX_Control(pDX, IDC_STC_SHIP_LOT_ID2, m_stcShipLotIdS);
	DDX_Control(pDX, IDC_STC_SHIP_TRAY_COUNT2, m_stcShipTrayCntS);
	DDX_Control(pDX, IDC_STC_SHIP_COUNT2, m_stcShipCntS);

	DDX_Control(pDX, IDC_STC_LOAD_CELL_COUNT, m_stcLoadCellCnt);
	DDX_Control(pDX, IDC_BTN_LOAD_CELL_EDIT, m_btnLoadCellChkEdit);

	for (int i = 0; i < 10; i++) DDX_Control(pDX, IDC_LED_EQUIP_OPTION_0 + i, m_ledEquipOption[i]);

	for (int i = 0; i < 6; i++) DDX_Control(pDX, IDC_STC_LOAD_NO_0 + i, m_stcLoadNo[i]);
	for (int i = 0; i < 6; i++) DDX_Control(pDX, IDC_STC_ASSY_NO_0 + i, m_stcAssyNo[i]);
	for (int i = 0; i < 6; i++) DDX_Control(pDX, IDC_STC_TRANS_NO_0 + i, m_stcTransNo[i]);
	for (int i = 0; i < 6; i++) DDX_Control(pDX, IDC_STC_TSTAGE_NO_0 + i, m_stcTStageNo[i]);
	for (int i = 0; i < 6; i++) DDX_Control(pDX, IDC_STC_UNLOAD_NO_0 + i, m_stcUnloadNo[i]);

	for (int i = 0; i < 3; i++) DDX_Control(pDX, IDC_LED_INDEX_DONE_0 + i, m_ledIndexDone[i]);
	DDX_Control(pDX, IDC_STC_INDEX_POS, m_stcIndexPos);

	DDX_Control(pDX, IDC_STC_CYCLE_TIME, m_stcCycleTime);

	DDX_Control(pDX, IDC_RDO_WORK_START, m_rdoWorkStart);
	DDX_Control(pDX, IDC_RDO_WORK_STOP, m_rdoWorkStop);
	DDX_Control(pDX, IDC_LED_INIT_COMPLETE, m_ledInitComplete);
	DDX_Control(pDX, IDC_LED_LOT_LOAD_ENABLE, m_ledLotLoadEnable);
	DDX_Control(pDX, IDC_CHK_CYCLE_STOP, m_chkCycleStop);
	DDX_Control(pDX, IDC_CHK_MES_USE, m_chkMesUse);
	for (int i = 0; i < 3; i++) DDX_Control(pDX, IDC_PIC_TRAY_BACK_0 + i, m_picTrayBack[i]);

	for (int i = 0; i < 1; i++) DDX_Control(pDX, IDC_STC_LOAD_TRAY_COUNT_0 + i, m_stcLoadTrayCount[i]);
	for (int i = 0; i < 1; i++) DDX_Control(pDX, IDC_STC_CAP_TRAY_COUNT_0 + i, m_stcCapTrayCount[i]);
	for (int i = 0; i < 1; i++) DDX_Control(pDX, IDC_STC_UNLOAD_TRAY_COUNT_0 + i, m_stcUnloadTrayCount[i]);

	for (int i = 0; i < 1; i++) DDX_Control(pDX, IDC_GRD_LOAD_TRAY_0+ i, m_grdLoadTray[i]);
	for (int i = 0; i < 1; i++) DDX_Control(pDX, IDC_GRD_CAP_TRAY_0+ i, m_grdCapTray[i]);
	for (int i = 0; i < 1; i++) DDX_Control(pDX, IDC_GRD_UNLOAD_TRAY_0 + i , m_grdUnloadTray[i]);

	for (int i = 0; i < 16; i++) DDX_Control(pDX, IDC_STC_WORK_CASE_0 + i, m_stcWorkCase[i]);
	for (int i = 0; i < 12; i++) DDX_Control(pDX, IDC_STC_PORT_NO_0 + i, m_stcPortNo[i]);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_STC_TAKT_0 + i, m_stcTakt[i]);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_STC_UPH_0 + i, m_stcUph[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_STC_DAY_0 + i, m_stcDay[i]);
	for (int i = 0; i < 5; i++) DDX_Control(pDX, IDC_LED_VISION_STATUS_0 + i, m_ledVisionStatus[i]);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_STC_VISION_RESULT_0 + i, m_stcVisionResult[i]);
}

BEGIN_MESSAGE_MAP(CWorkDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_STN_CLICKED(IDC_STC_LOT_ID_R, &CWorkDlg::OnStnClickedLotIdR)
	ON_STN_CLICKED(IDC_STC_CM_COUNT_R, &CWorkDlg::OnStnClickedCmCountR)
	ON_STN_CLICKED(IDC_STC_LOT_ID_S, &CWorkDlg::OnStnClickedLotIdS)
	ON_STN_CLICKED(IDC_STC_CM_COUNT_S, &CWorkDlg::OnStnClickedCmCountS)
	ON_STN_CLICKED(IDC_STC_CAP_LOT_ID2, &CWorkDlg::OnStnClickedCapLotId)
	ON_STN_CLICKED(IDC_STC_CAP_TRAY_COUNT2, &CWorkDlg::OnStnClickedCapTrayCount)
	ON_STN_CLICKED(IDC_STC_SHIP_LOT_ID2, &CWorkDlg::OnStnClickedShipLotId)
	ON_STN_CLICKED(IDC_STC_SHIP_TRAY_COUNT2, &CWorkDlg::OnStnClickedShipTrayCount)
	ON_BN_CLICKED(IDC_RDO_WORK_START, &CWorkDlg::OnBnClickedRdoWorkStart)
	ON_BN_CLICKED(IDC_RDO_WORK_STOP, &CWorkDlg::OnBnClickedRdoWorkStop)
	ON_BN_CLICKED(IDC_CHK_CYCLE_STOP, &CWorkDlg::OnBnClickedChkCycleStop)
	ON_BN_CLICKED(IDC_CHK_MES_USE, &CWorkDlg::OnBnClickedChkMesUse)
	ON_MESSAGE(UM_UPDATE_MODEL, &CWorkDlg::OnUpdateModel)
	ON_MESSAGE(UM_UPDATE_TRAY_INFO, &CWorkDlg::OnUpdateTrayInfo)
	ON_MESSAGE(UM_UPDATE_BARCODE, &CWorkDlg::OnUpdateBarcode)
	ON_MESSAGE(UM_UPDATE_LOADCELL, &CWorkDlg::OnUpdateLoadCell)
	ON_MESSAGE(UM_RESET_CYCLE_STOP, &CWorkDlg::OnResetCycleStop)
	ON_MESSAGE(UM_LOT_START_END, &CWorkDlg::OnLotStartEnd)
	ON_MESSAGE(UM_UPDATE_UPH, &CWorkDlg::OnUpdateUph)
	ON_MESSAGE(UM_VISION_RESULT, &CWorkDlg::OnVisionResult)
	ON_MESSAGE(UM_INDEX_TACK, &CWorkDlg::OnIndexTack)
	ON_MESSAGE(UM_MODEL_CHANGE, &CWorkDlg::OnModelChange)
	ON_MESSAGE(UM_SHOW_MSG, &CWorkDlg::OnShowMsg)
	ON_BN_CLICKED(IDC_BUTTON1, &CWorkDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CWorkDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BTN_CAP_CLEAR, &CWorkDlg::OnBnClickedCapClear)
	ON_BN_CLICKED(IDC_BTN_SHIP_CLEAR, &CWorkDlg::OnBnClickedShipClear)
	ON_BN_CLICKED(IDC_BTN_MES_CANCEL, &CWorkDlg::OnBnClickedMesCancel)
	ON_STN_CLICKED(IDC_STC_LOAD_CELL_COUNT, &CWorkDlg::OnStnClickedLoadCellCnt)
	ON_BN_CLICKED(IDC_BTN_LOAD_CELL_EDIT, &CWorkDlg::OnBnClickedLoadCellEdit)
	ON_STN_CLICKED(IDC_LBL_LOT_0, &CWorkDlg::OnStnClickedLblLot0)
	ON_STN_CLICKED(IDC_LBL_LOT_3, &CWorkDlg::OnStnClickedLblLot3)
	ON_STN_CLICKED(IDC_LBL_CAP_LOT_0, &CWorkDlg::OnStnClickedLblCapLot0)
	ON_STN_CLICKED(IDC_LBL_CAP_LOT_3, &CWorkDlg::OnStnClickedLblShipLot0)
END_MESSAGE_MAP()

// CWorkDlg 메시지 처리기입니다.

BOOL CWorkDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(this, 0, 75, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	Initial_Controls();

	m_pWorkInfoDlg = new CWorkInfoDlg(this);
	m_pWorkInfoDlg->Create(IDD_WORK_INFO_DLG, this);

	m_bAutoRunning = FALSE;
	m_nGroupNo = 0;

	gData.bBuffTrayChange = FALSE;
	gData.bContinueLotEnd = FALSE;
	gData.bMesFirstLot = FALSE;
	gData.bMesContinueLot = FALSE;
	gData.bLotEndBeep = FALSE;
	gData.bFirstLotStart = FALSE;

	m_rdoWorkStop.SetCheck(TRUE);
	m_rdoWorkStop.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);

	m_bCapLotIdInput = FALSE;
	m_bShipLotIdInput = FALSE;

	n_TimerCnt = 0;

	CIniFileCS INI(gsCurrentDir + "\\System\\CapShipData.ini");
	if (INI.Check_File()) {
		gData.sCapLotID = INI.Get_String("CAP_DATA", "LOTID", "");
		gData.nCapTrayUseCount = INI.Get_Integer("CAP_DATA", "TRAYCOUNT", 0);
		gData.nCapUseCount = INI.Get_Integer("CAP_DATA", "CMCOUNT", 0);
		gData.nCapTrayCount = INI.Get_Integer("CAP_DATA", "USETYCOUNT", 999);

		gData.sShipLotID = INI.Get_String("SHIP_DATA", "LOTID", "");
		gData.nShipTrayUseCount = INI.Get_Integer("SHIP_DATA", "TRAYCOUNT", 0);
		gData.nShipUseCount = INI.Get_Integer("SHIP_DATA", "CMCOUNT", 0);
		gData.nShipTrayCount = INI.Get_Integer("SHIP_DATA", "USETYCOUNT", 999);

		CString sTxt;
		m_stcCapLotId.SetWindowText(gData.sCapLotID);
		sTxt.Format("%d", gData.nCapTrayUseCount);	m_stcCapTrayCnt.SetWindowText(sTxt);
		sTxt.Format("%d", gData.nCapUseCount);		m_stcCapCnt.SetWindowText(sTxt);

		m_stcShipLotId.SetWindowText(gData.sShipLotID);
		sTxt.Format("%d", gData.nShipTrayUseCount);	m_stcShipTrayCnt.SetWindowText(sTxt);
		sTxt.Format("%d", gData.nShipUseCount);		m_stcShipCnt.SetWindowText(sTxt);
	} else {
		gData.sCapLotID = gData.sShipLotID = "";
		gData.nCapTrayUseCount = gData.nShipTrayUseCount = 0;
		gData.nCapUseCount = gData.nShipUseCount = 0;
		gData.nCapTrayCount = gData.nShipTrayCount = 999;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CWorkDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	if (pMsg->message == WM_MOUSEMOVE) {
		CCME8000Dlg *pMainDlg = (CCME8000Dlg*)AfxGetApp()->GetMainWnd();
		pMainDlg->Set_ScreenOn();
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CWorkDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	
	KillTimer(0);

	m_pWorkInfoDlg->DestroyWindow();
	delete m_pWorkInfoDlg;
	m_pWorkInfoDlg = NULL;
}

void CWorkDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow) {
		// 화면전환시 AVI 와 CAP 간 연결 변수 FALSE로 만들기, 연결되어 있으면 다시 LED에 불 들어옴
		g_objAviUDP.Set_ConnectStatus (FALSE); 
		g_objAviUDP.Set_ConnectRequest();

		EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
		CString strText;
		
		m_pWorkInfoDlg->ShowWindow(SW_SHOW);

#ifndef DRY_RUN_TEST
		m_bmpEquipment.DeleteObject();
		if (pEquipData->bUseDoorLock) m_bmpEquipment.LoadBitmap(IDB_EQUIP_WORK);
		else m_bmpEquipment.LoadBitmap(IDB_EQUIP_DOOR);
		m_imgEquipment.SetBitmap(m_bmpEquipment);
#endif

		m_stcLotIdR.GetWindowText(strText);
		if (strText.GetLength() < 1) {
			if (gData.sLotID[0].GetLength() < 1) gData.sLotID[0] = "CLOT_ID";
			m_stcLotIdR.SetWindowText(gData.sLotID[0]);
		}

		strText.Format("%d", gData.nTrayUseCount[0]);
		m_stcTrayCountR.SetWindowText(strText);

		strText.Format("%d", gData.nCmUseCount[0]);
		m_stcCmCountR.SetWindowText(strText);

		m_stcLotIdS.GetWindowText(strText);
		if (strText.GetLength() < 1) {
			if (gData.sLotID[1].GetLength() < 1) gData.sLotID[1] = "CLOT_ID";
			m_stcLotIdS.SetWindowText(gData.sLotID[1]);
		}

		strText.Format("%d", gData.nTrayUseCount[1]);
		m_stcTrayCountS.SetWindowText(strText);

		strText.Format("%d", gData.nCmUseCount[1]);
		m_stcCmCountS.SetWindowText(strText);

		strText.Format("%d", pEquipData->nLoadCellChkCnt);
		m_stcLoadCellCnt.SetWindowText(strText);
		m_stcLoadCellCnt.EnableWindow(FALSE);

		m_chkMesUse.SetCheck(pEquipData->bUseMes);

		m_ledEquipOption[0].Set_On(pEquipData->bUseVisionCmAlign);
		m_ledEquipOption[1].Set_On(pEquipData->bUseVisionCapAlign);
		m_ledEquipOption[2].Set_On(pEquipData->bUseTrayPickerTurn);
		m_ledEquipOption[3].Set_On(pEquipData->bUseCapPickerTurn);
		m_ledEquipOption[4].Set_On(pEquipData->bUseInlineMode);


		m_grpLot[0].SetWindowText("Port 1 Lot Info");
		m_grpLot[1].SetWindowText("Port 2 Lot Info");
		m_stcLotIdR.Set_Color(COLOR_DEFAULT, RGB(0x80, 0xF0, 0xF0));
		m_stcCmCountR.Set_Color(COLOR_DEFAULT, RGB(0x80, 0xF0, 0xF0));

		m_stcLotIdS.Set_Color(COLOR_DEFAULT, RGB(0x80, 0xF0, 0xF0));
		m_stcCmCountS.Set_Color(COLOR_DEFAULT, RGB(0x80, 0xF0, 0xF0));

		g_objCommon.Locking_MainDoor(FALSE);

		SetTimer(0, 100, NULL);
	


	} else {
		KillTimer(0);
		m_pWorkInfoDlg->ShowWindow(SW_HIDE);
	}
}

void CWorkDlg::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(0);
	
	CCME8000Dlg *pMainDlg = (CCME8000Dlg*)AfxGetApp()->GetMainWnd();
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	DX_DATA_12 *pDX12 = g_objAJinAXL.Get_pDX12();
	if (pDX12->iStartSw && !m_rdoWorkStart.GetCheck()) {
		g_objLogFile.Save_HandlerLog("[Work Mode] START S/W push");
		m_rdoWorkStart.SetCheck(TRUE);
		pMainDlg->Set_LotErrorLog("START", 903, "Start");
	} else if (pDX12->iStopSw && !m_rdoWorkStop.GetCheck()) {
		g_objLogFile.Save_HandlerLog("[Work Mode] STOP S/W push");
		MachineStopLog("STOP_BUTTON_PUSH");
		m_rdoWorkStop.SetCheck(TRUE);
		pMainDlg->Set_LotErrorLog("STOP", 904, "Stop");
	}

	if (pDX12->iResetSw) {
		g_objCommon.Show_Alarm("", STATE_ALARM, FALSE);
	}

	Check_Lamp();	// Load1/2, NG, Good Port Lamp Switch
	Display_Status();

	n_TimerCnt++;
	if(n_TimerCnt == 10)
	{
		n_TimerCnt = 0;
		g_objAviUDP.Set_ConnectStatus(FALSE);
		g_objAviUDP.Set_ConnectRequest();

	}

	if (m_rdoWorkStart.GetCheck()) {
		if (!m_bAutoRunning) {		// First AutoRun
			if (!Work_Start()) { SetTimer(0, 100, NULL); m_rdoWorkStop.SetCheck(TRUE); return; }
			if (g_objSequenceInit.Get_InitComplete()) {

				m_bAutoRunning = TRUE;
				BOOL bLoad1 = g_objSequenceMain.Get_LotLoadEnable(0);
				BOOL bLoad2 = g_objSequenceMain.Get_LotLoadEnable(1);
				if (!bLoad1)					g_objCommon.Locking_PortSlide(TRUE, 1);
				if (!bLoad2)					g_objCommon.Locking_PortSlide(TRUE, 2);
				if (!gData.bLoadPort3Wait)		g_objCommon.Locking_PortSlide(TRUE, 3);
				if (!gData.bCapPort1Wait)		g_objCommon.Locking_PortSlide(TRUE, 4);
				if (!gData.bCapPort2Wait)		g_objCommon.Locking_PortSlide(TRUE, 5);
				if (!gData.bUnloadPort1Wait)	g_objCommon.Locking_PortSlide(TRUE, 6);
				if (!gData.bUnloadPort2Wait)	g_objCommon.Locking_PortSlide(TRUE, 7);
					
				g_objCommon.Locking_MainDoor(TRUE, TRUE);
				pMainDlg->Enable_ModeButton(FALSE);
				if (gAlm.bBegin) Reset_AlarmLog();
				pMainDlg->Set_CurrentState(STATE_RUN);

				g_objSequenceMain.Begin_MainRunThread();
				pMainDlg->Set_EquipRunStart();
				MachineStopLog("RUN_START");

 				g_objMES.m_bStart = TRUE;
// 				//처음 시작할때만 MES 체크 해준다.
// 				if (g_objMES.m_bMESUse == FALSE || g_objMES.m_nMESSequence == 3) {
// 					
// 					if (g_objMES.m_bMESUse==FALSE) g_objMES.m_nMESSequence = 3;
// 					if (gData.nCapTrayCount == 0) { Set_CapLotIDChange(); }	// Cap Lot 처음 시작할때 Cap Lot Mes 정상 등록 후 정보를 옮겨준다.
// 					g_objSequenceMain.Begin_MainRunThread();
// 					pMainDlg->Set_EquipRunStart();
// 					MachineStopLog("RUN_START");
// 
// 				} else {
// 					if (gData.bMesFirstLot == FALSE) { 
// 						gData.bMesFirstLot = TRUE;
// 						g_objMES.m_nMESSequence = 0;
// 						g_objMES.Set_CapLotChangeRequest(gData.sCapLotID, gData.sOperID);
// 						CString strLog;
// 						strLog.Format("[Work Timer] Set_CapLotChangeRequest. (LotID:%s)", gData.sCapLotID);
// 						g_objLogFile.Save_MesAgentLog(strLog);
// 
// 					} else if (g_objMES.m_bMESUse==TRUE && g_objMES.m_nMESSequence==1) {
// 						g_objCommon.Show_MsgBox(1, "MES 등록이 정상적으로 되지 않았습니다. 다시 시도해주시길 바랍니다.");
// 
// 						g_objMES.m_nMESSequence = 0;
// 						if (gData.bMesFirstLot == TRUE) gData.bMesFirstLot = FALSE;
// 						pMainDlg->Enable_ModeButton(TRUE);
// 						SetTimer(0, 100, NULL); m_rdoWorkStop.SetCheck(TRUE); return;
// 					}
// 				}

				//g_objSequenceMain.Begin_MainRunThread();
				//pMainDlg->Set_EquipRunStart();

			} else {
				g_objCommon.Show_Error(40);		// 초기화 완료 에러
			}

		} else {				// Auto Running
			if (g_objMES.m_bMESUse==FALSE) {
				if (!g_objSequenceMain.Is_MainThreadRun()) {
					g_objLogFile.Save_HandlerLog("[Work Mode] Auto STOP");
					pMainDlg->Set_CurrentState(STATE_STOP);
				}
			}
		}

	} else if (m_rdoWorkStop.GetCheck()) {
		if (m_bAutoRunning) {	// First AutoStop
			m_bAutoRunning = FALSE;

			g_objSequenceMain.End_MainRunThread();

			g_objMES.m_bStart = FALSE;

			int nState = theApp.Get_MainState();
			if (nState != STATE_ALARM && nState != STATE_ERROR) pMainDlg->Set_CurrentState(STATE_STOP);

			m_rdoWorkStart.Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
			m_rdoWorkStop.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);

			pMainDlg->Enable_ModeButton(TRUE);
			g_objCommon.Locking_MainDoor(FALSE);
			g_objCommon.Locking_PortSlide(FALSE);

			pMainDlg->Save_EquipRunTime();
			g_objCommon.Save_MotionPos();

		} else {				// Stop
			int nState = theApp.Get_MainState();
			if (nState != STATE_ERROR) g_objCommon.Check_MainEmgAir();
		}
	}

	int nMode = theApp.Get_MainMode();
	if (nMode == MODE_OPERATOR || nMode == MODE_WORK) SetTimer(0, 100, NULL);
	else											  KillTimer(0);

	CDialogEx::OnTimer(nIDEvent);
}

void CWorkDlg::OnStnClickedLblLot0()
{
	m_nGroupNo = 0;

	m_Group[0].Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);
	m_Group[1].Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
	m_Group[6].Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
	m_Group[9].Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
	m_bCapLotIdInput = FALSE;
	m_bShipLotIdInput = FALSE;
}

void CWorkDlg::OnStnClickedLblLot3()
{
	m_nGroupNo = 1;

	m_Group[0].Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
	m_Group[1].Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);
	m_Group[6].Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
	m_Group[9].Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
	m_bCapLotIdInput = FALSE;
	m_bShipLotIdInput = FALSE;
}

void CWorkDlg::OnStnClickedLblCapLot0()
{
	m_Group[0].Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
	m_Group[1].Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
	m_Group[6].Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);
	m_Group[9].Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
	m_bCapLotIdInput = TRUE;
	m_bShipLotIdInput = FALSE;
}

void CWorkDlg::OnStnClickedLblShipLot0()
{
	m_Group[0].Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
	m_Group[1].Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
	m_Group[6].Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
	m_Group[9].Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);
	m_bCapLotIdInput = FALSE;
	m_bShipLotIdInput = TRUE;
}

void CWorkDlg::OnBnClickedCapClear()
{
	if (g_objCommon.Show_MsgBox(2, "Cap Lot 정보를 초기화 하시겠습니까?") != IDOK) return;

	m_stcCapLotId.SetWindowText("");
	m_stcCapCnt.SetWindowText("");
	m_stcCapTrayCnt.SetWindowText("");

	m_stcCapLotIdS.SetWindowText("");
	m_stcCapCntS.SetWindowText("");
	m_stcCapTrayCntS.SetWindowText("");

	gData.sCapLotID.Empty();
	gData.nCapTrayUseCount = 0;
	gData.nCapUseCount = 0;
	gData.nCapTrayCount = 999;	// Cap 정보 초기화 할때 같이 초기화 해준다.
	g_objCommon.Save_CapShiipData(1);
	g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 0, NULL);
}

void CWorkDlg::OnBnClickedShipClear()
{
	if (g_objCommon.Show_MsgBox(2, "Ship Lot 정보를 초기화 하시겠습니까?") != IDOK) return;

	m_stcShipLotId.SetWindowText("");
	m_stcShipCnt.SetWindowText("");
	m_stcShipTrayCnt.SetWindowText("");

	m_stcShipLotIdS.SetWindowText("");
	m_stcShipCntS.SetWindowText("");
	m_stcShipTrayCntS.SetWindowText("");

	gData.sShipLotID.Empty();
	gData.nShipUseCount = 0;
	gData.nShipTrayUseCount = 0;
	gData.nShipTrayCount = 999;
	g_objCommon.Save_CapShiipData(2);
	g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 0, NULL);
}

void CWorkDlg::OnBnClickedMesCancel()
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	if (m_rdoWorkStart.GetCheck()) {
		g_objCommon.Show_MsgBox(1, "Auto Run 중엔 취소할 수 없습니다........");
		return;
	}

	CString sLog;sLog.Format("[Work Mode] MES Cancel Clicked");
	g_objLogFile.Save_HandlerLog(sLog);

	g_objMES.m_nMESCapSequence = 0;
	g_objMES.m_nMESShipSequence = 0;
	if (gData.bMesFirstLot == TRUE) gData.bMesFirstLot = FALSE;	
}

void CWorkDlg::OnStnClickedLotIdR()
{
	CString strKey;
	if (g_objCommon.Show_KeyPad(strKey) != IDOK) return;

	m_stcLotIdR.SetWindowText(strKey);

	CString sLog;sLog.Format("[Work Mode] Lot ID Clicked, %s", strKey);
	g_objLogFile.Save_HandlerLog(sLog);


//	g_dlgOperator.m_stcOperLotId.SetWindowText(strKey);
}

void CWorkDlg::OnStnClickedCmCountR()
{
	CString strOld, strNew, strValue;

	m_stcCmCountR.GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	int nCmCnt = atoi(strNew);

	int nTrayUseCount = nCmCnt / gData.nCmMaxCount;
	if (nCmCnt % gData.nCmMaxCount) nTrayUseCount++;
	strValue.Format("%d", nTrayUseCount);
	m_stcTrayCountR.SetWindowText(strValue);
// 	g_dlgOperator.m_stcOperTrayCount.SetWindowText(strValue);

	strValue.Format("%d", nCmCnt);
	m_stcCmCountR.SetWindowText(strValue);
// 	g_dlgOperator.m_stcOperCmCount.SetWindowText(strValue);
}

void CWorkDlg::OnStnClickedLotIdS()
{
	CString strKey;
	if (g_objCommon.Show_KeyPad(strKey) != IDOK) return;

	m_stcLotIdS.SetWindowText(strKey);
	g_dlgOperator.m_stcOperLotId.SetWindowText(strKey);

	CString sLog;sLog.Format("[Work Mode] Lot ID Clicked, %s", strKey);
	g_objLogFile.Save_HandlerLog(sLog);
}

void CWorkDlg::OnStnClickedCmCountS()
{
	CString strOld, strNew, strValue;

	m_stcCmCountS.GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	int nCmCnt = atoi(strNew);

	int nTrayUseCount = nCmCnt / gData.nCmMaxCount;
	if (nCmCnt % gData.nCmMaxCount) nTrayUseCount++;
	strValue.Format("%d", nTrayUseCount);
	m_stcTrayCountS.SetWindowText(strValue);
	g_dlgOperator.m_stcOperTrayCount.SetWindowText(strValue);

	strValue.Format("%d", nCmCnt);
	m_stcCmCountS.SetWindowText(strValue);
	g_dlgOperator.m_stcOperCmCount.SetWindowText(strValue);
}

void CWorkDlg::OnStnClickedCapLotId()
{
	CString strKey;
	if (g_objCommon.Show_KeyPad(strKey) != IDOK) return;

	int nCapCnt = Check_CapShipLotID(1, strKey);
	if (nCapCnt < 1) return;
	m_stcCapLotIdS.SetWindowText(strKey);


	CString sLog;sLog.Format("[Work Mode] Cap Lot ID Clicked, %s", strKey);
	g_objLogFile.Save_HandlerLog(sLog);
}

void CWorkDlg::OnStnClickedCapTrayCount()
{
	CString strOld, strNew, strValue;

	m_stcCapTrayCntS.GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	int nCapTrayCnt = atoi(strNew);
	if (nCapTrayCnt < 1 || nCapTrayCnt >= 999) return;

	strValue.Format("%d", nCapTrayCnt);
	m_stcCapTrayCntS.SetWindowText(strValue);

	strValue.Format("%d", nCapTrayCnt * gData.nCapMaxCount);
	m_stcCapCntS.SetWindowText(strValue);


	CString sLog;sLog.Format("[Work Mode] Cap Tray Count Clicked, %s", strValue);
	g_objLogFile.Save_HandlerLog(sLog);
}

void CWorkDlg::OnStnClickedShipLotId()
{
	CString strKey;
	if (g_objCommon.Show_KeyPad(strKey) != IDOK) return;

	int nCapCnt = Check_CapShipLotID(2, strKey);
	if (nCapCnt < 1) return;
	m_stcShipLotIdS.SetWindowText(strKey);

	CString sLog;sLog.Format("[Work Mode] Ship Lot ID Clicked, %s", strKey);
	g_objLogFile.Save_HandlerLog(sLog);
}

void CWorkDlg::OnStnClickedShipTrayCount()
{
	CString strOld, strNew, strValue;

	m_stcShipTrayCntS.GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	int nShipTrayCnt = atoi(strNew);
	if (nShipTrayCnt < 1 || nShipTrayCnt >= 999) return;

	strValue.Format("%d", nShipTrayCnt);
	m_stcShipTrayCntS.SetWindowText(strValue);

	strValue.Format("%d", nShipTrayCnt * gData.nShipMaxCount);
	m_stcShipCntS.SetWindowText(strValue);

	CString sLog;sLog.Format("[Work Mode] Ship Tray Count Clicked, %s", strValue);
	g_objLogFile.Save_HandlerLog(sLog);
}

void CWorkDlg::OnBnClickedRdoWorkStart()
{
	g_objLogFile.Save_HandlerLog("[Work Mode] START button push");
	CCME8000Dlg *pMainDlg = (CCME8000Dlg*)AfxGetApp()->GetMainWnd();

	pMainDlg->Set_LotErrorLog("START", 903, "Start");
}

void CWorkDlg::OnBnClickedRdoWorkStop()
{
	g_objLogFile.Save_HandlerLog("[Work Mode] STOP button push");
	MachineStopLog("STOP_BUTTON_PUSH");
	CCME8000Dlg *pMainDlg = (CCME8000Dlg*)AfxGetApp()->GetMainWnd();

	pMainDlg->Set_LotErrorLog("STOP", 904, "Stop");
}

void CWorkDlg::OnBnClickedChkCycleStop()
{
	if (m_chkCycleStop.GetCheck()) {
		m_chkCycleStop.Set_Color(RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x00, 0xFF));
		if (g_objCommon.Show_MsgBox(2, "Are you sure you want to Cycle Stop?") == IDOK) {
			gData.bCycleStop = TRUE;
		} else {
			m_chkCycleStop.Set_Color(RGB(0x00, 0x00, 0x00), RGB(0xF0, 0xF0, 0xF0));
			m_chkCycleStop.SetCheck(FALSE);
		}
	} else {
		m_chkCycleStop.Set_Color(RGB(0x00, 0x00, 0x00), RGB(0xF0, 0xF0, 0xF0));
		gData.bCycleStop = FALSE;
	}
}

void CWorkDlg::OnBnClickedChkMesUse()
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	if (m_rdoWorkStart.GetCheck()) {
		g_objCommon.Show_MsgBox(1, "Can't change in Auto Run........");

		m_chkMesUse.SetCheck(pEquipData->bUseMes);
		return;
	}

	CIniFileCS INI("System/EquipData.ini");
	if (!INI.Check_File()) {
		AfxMessageBox("EquipData.ini File Not Found!!!");
		return;
	}
	INI.Set_Bool("OPTION", "MES_USE", m_chkMesUse.GetCheck());
	g_objDataManager.Read_EquipData();
	g_objMES.Set_MESUse(m_chkMesUse.GetCheck());
}

void CWorkDlg::OnStnClickedLoadCellCnt()
{
	CString strOld, strNew;
	m_stcLoadCellCnt.GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	m_stcLoadCellCnt.SetWindowText(strNew);

	CIniFileCS INI(gsCurrentDir + "\\System\\EquipData.ini");
	if (!INI.Check_File()) { AfxMessageBox("EquipData.ini File Not Found!!!"); return; }
	
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	int	nCnt;

	nCnt = atoi(strNew); INI.Set_Integer("LOAD_CELL", "CHECK_COUNT", nCnt);
	pEquipData->nLoadCellChkCnt = nCnt;

	m_stcLoadCellCnt.EnableWindow(FALSE);
}

void CWorkDlg::OnBnClickedLoadCellEdit()
{
	if (m_stcLoadCellCnt.IsWindowEnabled())
		m_stcLoadCellCnt.EnableWindow(FALSE);
	else
		m_stcLoadCellCnt.EnableWindow(TRUE);
}

///////////////////////////////////////////////////////////////////////////////
// User Functions

BOOL CWorkDlg::Work_Start()
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	CString strTemp, strTemp2;

	int nState = theApp.Get_MainState();
	if (nState == STATE_ERROR || nState == STATE_ALARM) return FALSE;

	if (!g_objCommon.Check_MainDoor()) return FALSE;

	DX_DATA_00 *pDX00 = g_objAJinAXL.Get_pDX00();
	DX_DATA_01 *pDX01 = g_objAJinAXL.Get_pDX01();
	DX_DATA_02 *pDX02 = g_objAJinAXL.Get_pDX02();
	DX_DATA_03 *pDX03 = g_objAJinAXL.Get_pDX03();

// #ifndef DRY_RUN_TEST
// 	if (gData.sOperID.GetLength() < 5) {
// 		m_rdoWorkStop.SetCheck(TRUE);
// 		g_objCommon.Show_MsgBox(1, "Please Check Operator ID.......");
// 		return FALSE;
// 	}
// #endif

	if (!pDX00->iLoadPort1SlideClose)	{ g_objCommon.Show_MsgBox(1, "Load Port 1번 Slide Close 센서가 감지 되지 않습니다. Slide를 끝까지 밀어주십시오."); return FALSE; }
	if (!pDX01->iLoadPort2SlideClose)	{ g_objCommon.Show_MsgBox(1, "Load Port 2번 Slide Close 센서가 감지 되지 않습니다. Slide를 끝까지 밀어주십시오."); return FALSE; }
	if (!pDX01->iLoadPort3SlideClose)	{ g_objCommon.Show_MsgBox(1, "Load Port 3번 Slide Close 센서가 감지 되지 않습니다. Slide를 끝까지 밀어주십시오."); return FALSE; }
	if (!pDX02->iCapPort1SlideClose)	{ g_objCommon.Show_MsgBox(1, "Cap Port1 Slide Close 센서가 감지 되지 않습니다. Slide를 끝까지 밀어주십시오."); return FALSE; }
	if (!pDX02->iCapPort2SlideClose)	{ g_objCommon.Show_MsgBox(1, "Cap Port2 Slide Close 센서가 감지 되지 않습니다. Slide를 끝까지 밀어주십시오."); return FALSE; }
	if (!pDX03->iUnloadPort1SlideClose)	{ g_objCommon.Show_MsgBox(1, "Unload Port1 Slide Close 센서가 감지 되지 않습니다. Slide를 끝까지 밀어주십시오."); return FALSE; }
	if (!pDX03->iUnloadPort2SlideClose)	{ g_objCommon.Show_MsgBox(1, "Unload Port2 Slide Close 센서가 감지 되지 않습니다. Slide를 끝까지 밀어주십시오."); return FALSE; }

	int nMotionNo = g_objCommon.Check_MotionPos();
	if (nMotionNo < 99) {
		double dCurrentPos = g_objAJinAXL.Get_Position(nMotionNo);
		CString strName = g_objAJinAXL.Get_AxisName(nMotionNo);
		strTemp.Format("Motion(%s) 위치를 Check 하세요.\n이전위치(%0.3lf) != 현재위치(%0.3lf)", strName, gAlm.dMotionPos[nMotionNo], dCurrentPos);
		g_objLogFile.Save_HandlerLog(strTemp);

		g_objCommon.Show_MsgBox(1, strTemp);
		m_rdoWorkStop.SetCheck(TRUE);
		return FALSE;
	}

	m_stcCapLotId.GetWindowText(strTemp);
	m_stcCapLotIdS.GetWindowText(strTemp2);
	if (strTemp.GetLength() < 2 && strTemp2.GetLength() < 2) { g_objCommon.Show_MsgBox(1, "Cap LOT ID를 입력하여 주십시오."); return FALSE; }

	m_stcCapCnt.GetWindowText(strTemp);
	m_stcCapCntS.GetWindowText(strTemp2);
	int nTempCnt1 = atoi(strTemp);
	int nTempCnt2 = atoi(strTemp2);
	if (nTempCnt1 < 1 && nTempCnt2 < 1) {	g_objCommon.Show_MsgBox(1, "Cap 수량을 확인하여 주십시오."); return FALSE; }

	m_stcShipLotId.GetWindowText(strTemp);
	m_stcShipLotIdS.GetWindowText(strTemp2);
	if (strTemp.GetLength() < 2 && strTemp2.GetLength() < 2) { g_objCommon.Show_MsgBox(1, "Ship LOT ID를 입력하여 주십시오."); return FALSE; }

	m_stcShipCnt.GetWindowText(strTemp);
	m_stcShipCntS.GetWindowText(strTemp2);
	nTempCnt1 = atoi(strTemp);
	nTempCnt2 = atoi(strTemp2);
	if (nTempCnt1 < 1 && nTempCnt2 < 1) {	g_objCommon.Show_MsgBox(1, "Ship 수량을 확인하여 주십시오."); return FALSE; }

#ifndef DRY_RUN_TEST
// 	if (!pEquipData->bUseVisionCmAlign || !pEquipData->bUseVisionCapAlign) {
// 		if (g_objCommon.Show_MsgBox(2, "Vision Option을 끄고 진행하시겠습니까?") != IDOK) return FALSE;
// 	}
#endif

	if (g_objSequenceMain.Get_IsAutoRun()) return TRUE;	// Auto Run이면 스킵

	if(pEquipData->bUseInlineMode && !g_objAviUDP.Is_Connected())
	{
		g_objCommon.Show_MsgBox(1, "CAP과 AVI 통신 연결되어 있지 않습니다."); return FALSE;
	}

	if (pEquipData->bUseInlineMode) {
		//g_objSequenceMain.Set_ClearRunData(1);	// 시점 변경 (LotEnd->LotStart)
		gData.bFirstLotStart = TRUE;

		g_dlgWork.Enable_UserInput(1, FALSE);
		g_dlgWork.Enable_UserInput(2, FALSE);

	} else {
#ifdef AJIN_BOARD_USE
		int nLoadTray1 = g_objSequenceMain.Get_MainRunCase(AUTO_LOAD_STAGE1);
		int nLoadTray2 = g_objSequenceMain.Get_MainRunCase(AUTO_LOAD_STAGE2);
		if ((nLoadTray1 == 0 || nLoadTray2 == 0) && !pDX00->iLoadPort1LowCheck && gData.nLoadTrayCount[0] == 0) {
			g_objCommon.Show_MsgBox(1, "Please Check the Load Port."); return FALSE;
		}
#endif
		//g_objSequenceMain.Set_ClearRunData(1);	// 시점 변경 (LotEnd->LotStart)
		gData.bFirstLotStart = TRUE;

		m_stcLotIdR.GetWindowText(strTemp);		// Lot ID
		gData.sLotID[0] = strTemp;
		if (strTemp.GetLength() < 2) { g_objCommon.Show_MsgBox(1, "Port1번에 LOT ID를 입력하여 주십시오."); return FALSE; }

		m_stcCmCountR.GetWindowText(strTemp);	// CM 수량
		int nTempCnt = atoi(strTemp);
		gData.nCmUseCount[0] = nTempCnt;
		if (nTempCnt < 1 || nTempCnt > 2400) {	g_objCommon.Show_MsgBox(1, "Port1번의 CM 수량을 확인하여 주십시오. (1 ~ 2400)"); return FALSE; }

		m_stcTrayCountR.GetWindowText(strTemp);	// Tray Count
		gData.nTrayUseCount[0] = atoi(strTemp);

		m_stcLotIdS.GetWindowText(strTemp2);		// Lot ID
	
		gData.sLotID[1] = strTemp2;

		m_stcCmCountS.GetWindowText(strTemp2);	// CM 수량
		int nTempCnt2 = atoi(strTemp2);
		gData.nCmUseCount[1] = nTempCnt2;

		m_stcTrayCountS.GetWindowText(strTemp2);	// Tray Count
		gData.nTrayUseCount[1] = atoi(strTemp2);

		if (strTemp2.GetLength() > 0 && nTempCnt2 > 0) { Enable_UserInput(2, TRUE); }
		else { gData.bLoadLampOn[1] = TRUE; g_objSequenceMain.Set_LotLoadEnable(TRUE, 1); }
	}
	return TRUE;
}

void CWorkDlg::Initial_Controls() 
{
	CString strText;
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	for (int i = 0; i < 10; i++) m_Group[i].Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT);
	for (int i = 0; i < 3; i++) m_Label[i].Init_Ctrl("바탕", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x80, 0x80, 0x80));		// Tray
	for (int i = 0; i < 2; i++) m_grpLot[i].Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT);
	for (int i = 0; i < 3; i++) m_lblLot[i].Init_Ctrl("바탕", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x20, 0x20, 0x80));		// Lot Run
	for (int i = 3; i < 6; i++) m_lblLot[i].Init_Ctrl("바탕", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x40, 0x00, 0x80));		// Lot Standby
	m_stcLotIdR.Init_Ctrl("Arial", 10, TRUE, COLOR_DEFAULT, RGB(0x80, 0xF0, 0xF0));
	m_stcTrayCountR.Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, RGB(0xD0, 0xD0, 0xD0));
	m_stcCmCountR.Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, RGB(0x80, 0xF0, 0xF0));
	m_stcLotIdS.Init_Ctrl("Arial", 10, TRUE, COLOR_DEFAULT, RGB(0x80, 0xF0, 0xF0));
	m_stcTrayCountS.Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, RGB(0xD0, 0xD0, 0xD0));
	m_stcCmCountS.Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, RGB(0x80, 0xF0, 0xF0));

	for (int i = 0; i < 6; i++) m_lblCapLot[i].Init_Ctrl("바탕", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x20, 0x20, 0x80));
	m_stcCapLotId.Init_Ctrl("Arial", 9, TRUE, COLOR_DEFAULT, RGB(0xD0, 0xD0, 0xD0));
	m_stcCapTrayCnt.Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, RGB(0xD0, 0xD0, 0xD0));
	m_stcCapCnt.Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, RGB(0xD0, 0xD0, 0xD0));
	m_stcCapLotIdS.Init_Ctrl("Arial", 9, TRUE, COLOR_DEFAULT, RGB(0x80, 0xF0, 0xF0));
	m_stcCapTrayCntS.Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, RGB(0x80, 0xF0, 0xF0));
	m_stcCapCntS.Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, RGB(0xD0, 0xD0, 0xD0));

	m_stcShipLotId.Init_Ctrl("Arial", 9, TRUE, COLOR_DEFAULT, RGB(0xD0, 0xD0, 0xD0));
	m_stcShipTrayCnt.Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, RGB(0xD0, 0xD0, 0xD0));
	m_stcShipCnt.Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, RGB(0xD0, 0xD0, 0xD0));
	m_stcShipLotIdS.Init_Ctrl("Arial", 9, TRUE, COLOR_DEFAULT, RGB(0x80, 0xF0, 0xF0));
	m_stcShipTrayCntS.Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, RGB(0x80, 0xF0, 0xF0));
	m_stcShipCntS.Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, RGB(0xD0, 0xD0, 0xD0));

	m_bmpEquipment.LoadBitmap(IDB_EQUIP_WORK);
	m_imgEquipment.SetBitmap(m_bmpEquipment);
	m_picUphBack.Init_Ctrl(COLOR_DEFAULT, COLOR_DEFAULT);
	for (int i = 0; i < 10; i++) m_ledEquipOption[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emBlue, CLedCS::em16);
	for (int i = 0; i < 6; i++) m_stcLoadNo[i].Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0xFF, 0x00), RGB(0x00, 0x00, 0x00));
	for (int i = 0; i < 6; i++) m_stcAssyNo[i].Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0xFF, 0x00), RGB(0x00, 0x00, 0x00));
	for (int i = 0; i < 6; i++) m_stcTransNo[i].Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0xFF, 0x00), RGB(0x00, 0x00, 0x00));
	for (int i = 0; i < 6; i++) m_stcTStageNo[i].Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0xFF, 0x00), RGB(0x00, 0x00, 0x00));
	for (int i = 0; i < 6; i++) m_stcUnloadNo[i].Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0xFF, 0x00), RGB(0x00, 0x00, 0x00));

	for (int i = 0; i < 3; i++) m_ledIndexDone[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em24);
	m_stcIndexPos.Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0xFF, 0x00), RGB(0x00, 0x00, 0x00));

	m_stcCycleTime.Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0xFF, 0x00), RGB(0x00, 0x00, 0x00));
	m_rdoWorkStart.Init_Ctrl("바탕", 20, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	m_rdoWorkStop.Init_Ctrl("바탕", 20, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	m_ledInitComplete.Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em24);
	m_ledLotLoadEnable.Init_Ctrl("바탕", 10, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	m_chkCycleStop.Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	m_chkMesUse.Init_Ctrl("Arial", 12, TRUE, RGB(0xFF, 0xFF, 0x00), RGB(0xC0, 0x10, 0x30), CCheckCS::emRed, CCheckCS::emRight);
	for (int i = 0; i < 3; i++) m_picTrayBack[i].Set_Color(COLOR_DEFAULT, RGB(0xF0, 0xF0, 0x80));
	for (int i = 0; i < 1; i++) m_stcLoadTrayCount[i].Init_Ctrl("바탕", 11, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x00, 0x00));
	for (int i = 0; i < 1; i++) m_stcCapTrayCount[i].Init_Ctrl("바탕", 11, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x00, 0x00));
	for (int i = 0; i < 1; i++) m_stcUnloadTrayCount[i].Init_Ctrl("바탕", 11, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x00, 0x00));
	for (int i = 0; i < 1; i++)Initial_Grid(&m_grdLoadTray[i], 3, 5);
	for (int i = 0; i < 1; i++)Initial_Grid(&m_grdCapTray[i], 4, 8);
	for (int i = 0; i < 1; i++)Initial_Grid(&m_grdUnloadTray[i], 6, 5);
	for (int i = 0; i < 16; i++) m_stcWorkCase[i].Init_Ctrl("바탕", 11, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x40, 0x40, 0x40));
	for (int i = 0; i < 12; i++) m_stcPortNo[i].Init_Ctrl("바탕", 11, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x40, 0x40, 0x40));
	for (int i = 0; i < 4; i++) m_stcTakt[i].Init_Ctrl("Arial", 9, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x40, 0x40, 0x40));
	for (int i = 0; i < 4; i++) m_stcUph[i].Init_Ctrl("Arial", 9, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x40, 0x40, 0x40));
	for (int i = 0; i < 2; i++) m_stcDay[i].Init_Ctrl("Arial", 9, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x40, 0x40, 0x40));
	for (int i = 0; i < 5; i++) m_ledVisionStatus[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i < 4; i++) m_stcVisionResult[i].Init_Ctrl("바탕", 11, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x00, 0x00));
	m_stcLoadCellCnt.Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, RGB(0x80, 0xF0, 0xF0));
}

void CWorkDlg::Initial_Grid(CGridCS *pGrid, int nRows, int nCols, int nNoDir)
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
			if (nNoDir == 1) { str.Format(_T("%d"), (nCols-1-j) * nRows + i + 1); }
			else			 { str.Format(_T("%d"), i * nCols + j + 1); }
			
			pGrid->Set_CellText(i, j, str);
		}
	}
}

void CWorkDlg::Check_Lamp()
{
	CCME8000Dlg *pMainDlg = (CCME8000Dlg*)AfxGetApp()->GetMainWnd();
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	DX_DATA_00 *pDX00 = g_objAJinAXL.Get_pDX00(); DY_DATA_00 *pDY00 = g_objAJinAXL.Get_pDY00();
	DX_DATA_01 *pDX01 = g_objAJinAXL.Get_pDX01(); DY_DATA_01 *pDY01 = g_objAJinAXL.Get_pDY01();
	DX_DATA_02 *pDX02 = g_objAJinAXL.Get_pDX02(); DY_DATA_02 *pDY02 = g_objAJinAXL.Get_pDY02();
	DX_DATA_03 *pDX03 = g_objAJinAXL.Get_pDX03(); DY_DATA_03 *pDY03 = g_objAJinAXL.Get_pDY03();
	DX_DATA_07 *pDX07 = g_objAJinAXL.Get_pDX07(); DY_DATA_07 *pDY07 = g_objAJinAXL.Get_pDY07();
	DX_DATA_09 *pDX09 = g_objAJinAXL.Get_pDX09(); DY_DATA_09 *pDY09 = g_objAJinAXL.Get_pDY09();
	DX_DATA_12 *pDX12 = g_objAJinAXL.Get_pDX12(); DY_DATA_12 *pDY12 = g_objAJinAXL.Get_pDY12();
	DX_DATA_13 *pDX13 = g_objAJinAXL.Get_pDX13(); DY_DATA_13 *pDY13 = g_objAJinAXL.Get_pDY13();

	BOOL bLoad1 = g_objSequenceMain.Get_LotLoadEnable(0);
	BOOL bLoad2 = g_objSequenceMain.Get_LotLoadEnable(1);

	if (g_objSequenceMain.Get_IsAutoRun()) {	// m_rdoWorkStart.GetCheck()
		// Load Port1
		if (pDX12->iLoad1Sw && bLoad1) {
			if (pDX00->iLoadPort1SlideClose && (!pEquipData->bUseDoorLock || !pDX13->iDoor01Unlock)) {	// 안전 확인.
				
				CString sTemp;
				m_stcCmCountR.GetWindowText(sTemp);	// CM 수량
				int nCnt = atoi(sTemp);
				m_stcLotIdR.GetWindowText(sTemp);		// Lot ID				
				// Port1 CM 수량 확인 및 LotID 확인
				if (nCnt > 0 && gLot.sLotID[0] != sTemp && gLot.sLotID[1] != sTemp) {
					gData.nCmUseCount[0] = nCnt;
					gData.sLotID[0] = sTemp;
					m_stcTrayCountR.GetWindowText(sTemp);	// Tray Count
					gData.nTrayUseCount[0] = atoi(sTemp);

					gData.bLoadLampOn[0] = FALSE;
					pMainDlg->Set_LampFlicker_Load1(FALSE);
					g_objSequenceMain.Set_LotLoadEnable(FALSE, 0);
					pDY00->oLoadPort1SlideLock = TRUE; pDY00->oLoadPort1SlideUnlock = FALSE;
					g_objAJinAXL.Write_Output(0);
					if (pEquipData->bUseDoorLock) pDY13->oDoor01Unlock = FALSE; 
					g_objAJinAXL.Write_Output(13);
				} else {
					g_objCommon.Show_MsgBox(1, "Port1 Lot 정보를 확인해 주십시오.");
				}
			}
		}
		// Load Port2
		if (pDX12->iLoad2Sw && bLoad2) {
			if (pDX01->iLoadPort2SlideClose && (!pEquipData->bUseDoorLock || !pDX13->iDoor02Unlock)) {	// 안전 확인.
				CString sTemp;
				m_stcCmCountS.GetWindowText(sTemp);	// CM 수량
				int nCnt = atoi(sTemp);
				m_stcLotIdS.GetWindowText(sTemp);		// Lot ID
				// Port1 CM 수량 확인 및 LotID 확인
				if (nCnt > 0 && gLot.sLotID[0] != sTemp && gLot.sLotID[1] != sTemp) {
					gData.nCmUseCount[1] = nCnt;					
					gData.sLotID[1] = sTemp;					
					m_stcTrayCountS.GetWindowText(sTemp);	// Tray Count
					gData.nTrayUseCount[1] = atoi(sTemp);
					
					gData.bLoadLampOn[1] = FALSE;
					pMainDlg->Set_LampFlicker_Load2(FALSE);
					g_objSequenceMain.Set_LotLoadEnable(FALSE, 1);
					pDY01->oLoadPort2SlideLock = TRUE; pDY01->oLoadPort2SlideUnlock = FALSE;
					g_objAJinAXL.Write_Output(0);
					if (pEquipData->bUseDoorLock) pDY13->oDoor02Unlock = FALSE; 
					g_objAJinAXL.Write_Output(13);
				} else {
					g_objCommon.Show_MsgBox(1, "Port2 Lot 정보를 확인해 주십시오.");
				}
			}
		}
		// Load Port3
		if (pDX12->iLoad3Sw) {
			if (pDX01->iLoadPort3SlideClose && (!pEquipData->bUseDoorLock || !pDX13->iDoor04Unlock)) {	// 안전 확인.
				if (!gData.bLoadPort3Wait && !m_bLoadSw3On) {
					pMainDlg->Set_LampFlicker_Load3(TRUE);
					pDY01->oLoadPort3SlideLock = FALSE; pDY01->oLoadPort3SlideUnlock = TRUE;
					g_objAJinAXL.Write_Output(1);
					if (pEquipData->bUseDoorLock) pDY13->oDoor04Unlock = TRUE; 
					g_objAJinAXL.Write_Output(13);

					gData.bLoadPort3Wait = TRUE;
					m_bLoadSw3On = TRUE;

				} else if (gData.bLoadPort3Wait && !m_bLoadSw3On) {
					pMainDlg->Set_LampFlicker_Load3(FALSE);
					pDY01->oLoadPort3SlideLock = TRUE; pDY01->oLoadPort3SlideUnlock = FALSE;
					g_objAJinAXL.Write_Output(1);
					if (pEquipData->bUseDoorLock) pDY13->oDoor04Unlock = FALSE; 
					g_objAJinAXL.Write_Output(13);

					gData.bLoadPort3Wait = FALSE;
					m_bLoadSw3On = TRUE;
				}
			}
		} else {
			m_bLoadSw3On = FALSE;
		}
		// Cap Port
		if (pDX12->iCap1Sw) {
			if (pDX02->iCapPort1SlideClose && (!pEquipData->bUseDoorLock || !pDX13->iDoor11Unlock)) {	// 안전 확인.
				if (!gData.bCapPort1Wait && !m_bCapSw1On) {
					pMainDlg->Set_LampFlicker_Cap1(TRUE);
					pDY02->oCapPort1SlideLock = FALSE; pDY02->oCapPort1SlideUnlock = TRUE;
					g_objAJinAXL.Write_Output(2);
					if (pEquipData->bUseDoorLock) pDY13->oDoor11Unlock = TRUE; 
					g_objAJinAXL.Write_Output(13);

					gData.bCapPort1Wait = TRUE;
					m_bCapSw1On = TRUE;

				} else if (gData.bCapPort1Wait && !m_bCapSw1On) {
					pMainDlg->Set_LampFlicker_Cap1(FALSE);
					pDY02->oCapPort1SlideLock = TRUE; pDY02->oCapPort1SlideUnlock = FALSE;
					g_objAJinAXL.Write_Output(2);
					if (pEquipData->bUseDoorLock) pDY13->oDoor11Unlock = FALSE; 
					g_objAJinAXL.Write_Output(13);

					gData.bCapPort1Wait = FALSE;
					m_bCapSw1On = TRUE;
				}
			}
		} else {
			m_bCapSw1On = FALSE;
		}

		if (pDX12->iCap2Sw) {
			if (pDX02->iCapPort2SlideClose && (!pEquipData->bUseDoorLock || !pDX13->iDoor12Unlock)) {	// 안전 확인.
				if (!gData.bCapPort2Wait && !m_bCapSw2On) {
					pMainDlg->Set_LampFlicker_Cap2(TRUE);
					pDY02->oCapPort2SlideLock = FALSE; pDY02->oCapPort2SlideUnlock = TRUE;
					g_objAJinAXL.Write_Output(2);
					if (pEquipData->bUseDoorLock) pDY13->oDoor12Unlock = TRUE; 
					g_objAJinAXL.Write_Output(13);

					gData.bCapPort2Wait = TRUE;
					m_bCapSw2On = TRUE;
				}
				else if (gData.bCapPort2Wait && !m_bCapSw2On) {
					pMainDlg->Set_LampFlicker_Cap2(FALSE);
					pDY02->oCapPort2SlideLock = TRUE; pDY02->oCapPort2SlideUnlock = FALSE;
					g_objAJinAXL.Write_Output(2);
					if (pEquipData->bUseDoorLock) pDY13->oDoor12Unlock = FALSE; 
					g_objAJinAXL.Write_Output(13);

					gData.bCapPort2Wait = FALSE;
					m_bCapSw2On = TRUE;
				}
			}
		} else {
			m_bCapSw2On = FALSE;
		}
		// Unload Tray
		if (pDX12->iUnload1Sw) {
			if (pDX03->iUnloadPort1SlideClose && (!pEquipData->bUseDoorLock || !pDX13->iDoor10Unlock)) {
				if (!gData.bUnloadPort1Wait && !m_bUnloadSw1On) {
					pMainDlg->Set_LampFlicker_Unload1(TRUE);
					pDY03->oUnloadPort1SlideLock = FALSE; pDY03->oUnloadPort1SlideUnlock = TRUE;
					g_objAJinAXL.Write_Output(3);
					if (pEquipData->bUseDoorLock) pDY13->oDoor10Unlock = TRUE; 
					g_objAJinAXL.Write_Output(13);

					gData.bUnloadPort1Wait = TRUE;
					m_bUnloadSw1On = TRUE;
				}
				else if (gData.bUnloadPort1Wait && !m_bUnloadSw1On) {
					pMainDlg->Set_LampFlicker_Unload1(FALSE);
					pDY03->oUnloadPort1SlideLock = TRUE; pDY03->oUnloadPort1SlideUnlock = FALSE;
					g_objAJinAXL.Write_Output(3);
					if (pEquipData->bUseDoorLock) pDY13->oDoor10Unlock = FALSE; 
					g_objAJinAXL.Write_Output(13);

					gData.bUnloadPort1Wait = FALSE;
					m_bUnloadSw1On = TRUE;
				}
			}
		} else {
			m_bUnloadSw1On = FALSE;
		}

		if (pDX12->iUnload2Sw && gData.bUnloadPort2Wait) {
			if (pDX03->iUnloadPort2SlideClose && (!pEquipData->bUseDoorLock || (!pDX13->iDoor05Unlock && !pDX13->iDoor06Unlock))) {	// 안전 확인.
				pMainDlg->Set_LampFlicker_Unload2(FALSE);
				pDY03->oUnloadPort2SlideLock = TRUE; pDY03->oUnloadPort2SlideUnlock = FALSE;
				g_objAJinAXL.Write_Output(3);
				if (pEquipData->bUseDoorLock) {
					pDY13->oDoor05Unlock = FALSE; pDY13->oDoor06Unlock = FALSE;
					g_objAJinAXL.Write_Output(13);
				}
				gData.nPNoUnloadPort = 0;
				gData.bUnloadPort2Wait = FALSE;
				gData.bUnloadTrayLotEnd[0] = FALSE;
				gData.bUnloadTrayLotEnd[1] = FALSE;
				//gData.nGoodTrayCount = 0;
			}
		}
	}
	//Cap Port1, 2, Unload Port1 버튼 누르면 도어락 풀리고 작업중 Flag Set
	//Flag Set 상태에서 버튼 누르면 도어락 걸리고 Flag Clear
	//Cap Port1, Unload Port1 트레이 없으면 알람
	//Cap Port2, Full Sensor 감지되면 알람
	//Load Port는 연속랏 테스트할때 정리하자....
}

void CWorkDlg::Display_Status()
{
	CString strTemp, strText;
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

// 	if (g_objMesAgent.Is_Connected()) { m_stcMesConnect.Set_Text("Connected"); m_stcMesConnect.Set_Color(RGB(0x00, 0x00, 0x00), RGB(0x00, 0xFF, 0x00)); }
// 	else { m_stcMesConnect.Set_Text("Disconnected"); m_stcMesConnect.Set_Color(RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x00, 0x00)); }
// 
// 	if (g_objMesAgent.Is_HostOnline()) { m_stcMesOnline.Set_Text("Online"); m_stcMesOnline.Set_Color(RGB(0x00, 0x00, 0x00), RGB(0x00, 0xFF, 0x00)); }
// 	else { m_stcMesOnline.Set_Text("Offline"); m_stcMesOnline.Set_Color(RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x00, 0x00)); }

	for (int i = 0; i < 3; i++) m_ledIndexDone[i].Set_On(gData.IndexDone[i]);

	DX_DATA_11 *pDX11 = g_objAJinAXL.Get_pDX11();
	int nIndexPos = pDX11->iIndexPosition0 + (pDX11->iIndexPosition1 << 1);
	strText.Format("%d", nIndexPos + 1);
	m_stcIndexPos.SetWindowText(strText);

	BOOL bInitComplete = g_objSequenceInit.Get_InitComplete();
	m_ledInitComplete.Set_On(bInitComplete);
	
	BOOL bLotLoadEnable = g_objSequenceMain.Get_LotLoadEnable(0);
	m_ledLotLoadEnable.Set_On(bLotLoadEnable);

	if (m_nGroupNo == 1) {
		m_grpLot[0].Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
		m_grpLot[1].Init_Ctrl("바탕", 12, TRUE, RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);

	} else {
		m_grpLot[0].Init_Ctrl("바탕", 12, TRUE, RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);
		m_grpLot[1].Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
	}

	for (int i = 0; i < 6; i++) { strText.Format("%d-%d", gData.nTNoIndex[0][i], gData.nCNoIndex[0][i]); m_stcLoadNo[i].Set_Text(strText); }
	for (int i = 0; i < 6; i++) { strText.Format("%d-%d", gData.nTNoIndex[1][i], gData.nCNoIndex[1][i]); m_stcAssyNo[i].Set_Text(strText); }
	for (int i = 0; i < 6; i++) { strText.Format("%d-%d", gData.nTNoIndex[2][i], gData.nCNoIndex[2][i]); m_stcTransNo[i].Set_Text(strText); }
	for (int i = 0; i < 6; i++) { strText.Format("%d-%d", gData.nTNoTransStage[i], gData.nCNoTransStage[i]); m_stcTStageNo[i].Set_Text(strText); }
	for (int i = 0; i < 6; i++) { strText.Format("%d-%d", gData.nTNoUnloadPick[i], gData.nCNoUnloadPick[i]); m_stcUnloadNo[i].Set_Text(strText); }



	m_ledVisionStatus[0].Set_On(g_objInspector.Get_VisionStatus());
	m_ledVisionStatus[1].Set_On(pEquipData->bUseInlineMode && g_objAviUDP.Is_Connected());
	//m_ledVisionStatus[1].Set_On(pEquipData->bUseInlineMode && g_objAviUDP.Is_Opened());

	int *pCase = g_objSequenceMain.Get_pMainRunCase();
	for (int i = 0; i < AUTO_COUNT; i++) { strText.Format("%02d", *(pCase + i)); m_stcWorkCase[i].Set_Text(strText); }
	
	// Debug
	strText.Format("%d-%d", gData.nPNoAviPort, gData.nTNoAviPort);			m_stcPortNo[ 0].Set_Text(strText);
	strText.Format("%d-%d", gData.nPNoTrayPick, gData.nTNoTrayPick);		m_stcPortNo[ 1].Set_Text(strText);
	strText.Format("%d-%d", gData.nPNoLoadPort, gData.nTNoLoadPort);		m_stcPortNo[ 2].Set_Text(strText);
	strText.Format("%d-%d", gData.nPNoLoadTray[0], gData.nTNoLoadTray[0]);	m_stcPortNo[ 3].Set_Text(strText);
	strText.Format("%d-%d", gData.nPNoLoadTray[1], gData.nTNoLoadTray[1]);	m_stcPortNo[ 4].Set_Text(strText);
	strText.Format("%d-%d", gData.nPNoLoadPick, gData.nTNoLoadPick[0]);		m_stcPortNo[ 5].Set_Text(strText);
	strText.Format("%d-%d", gData.nPNoIndex[0], gData.nTNoIndex[0][0]);		m_stcPortNo[ 6].Set_Text(strText);
	strText.Format("%d-%d", gData.nPNoIndex[1], gData.nTNoIndex[1][0]);		m_stcPortNo[ 7].Set_Text(strText);
	strText.Format("%d-%d", gData.nPNoIndex[2], gData.nTNoIndex[2][0]);		m_stcPortNo[ 8].Set_Text(strText);
	strText.Format("%d-%d", gData.nPNoTransStage, gData.nTNoTransStage[0]);	m_stcPortNo[ 9].Set_Text(strText);
	strText.Format("%d-%d", gData.nPNoUnloadPick, gData.nTNoUnloadPick[0]);	m_stcPortNo[10].Set_Text(strText);
	strText.Format("%d", gData.nPNoUnloadTray);								m_stcPortNo[11].Set_Text(strText);

}

void CWorkDlg::Reset_AlarmLog()
{
	CString strLog, strErrNo;
	SYSTEMTIME time;

	gAlm.bBegin = FALSE;
	GetLocalTime(&time);

	gAlm.dwEndTime = GetTickCount();
	gAlm.sEndTime.Format("%04d%02d%02d_%02d%02d%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	gAlm.dwProcTime = gAlm.dwEndTime - gAlm.dwStartTime;
	
	gLot.dwErrorTime += gAlm.dwProcTime; gLot.nErrorCount++;

	strLog.Format("%s,%04d,%s,%s,%s,%d", gAlm.sLotID, gAlm.nAlmNo, gAlm.sAlmMsg, gAlm.sStartTime, gAlm.sEndTime, gAlm.dwProcTime);
	g_objLogFile.Save_AlarmResetLog(strLog);	// Alarm Reset

	strErrNo.Format("%04d", gAlm.nAlmNo);
//	g_objMesAgent.Set_ErrorUpdate(0, strErrNo);

	g_objLogFile.Save_ECMLog(1, strLog);
}

void CWorkDlg::MachineStopLog(CString sType, CString sMsg)
{
	CString strLog, strErrNo;
	SYSTEMTIME time;

	GetLocalTime(&time);

	if (sType == "STOP_BUTTON_PUSH" || sType == "ALARM" || sType == "PROGRAM_BEGIN") {
		if (gData.dwMStopStartTime > 1) {
			gData.dwMStopEndTime = GetTickCount();
			gData.dwMStopProcTime = gData.dwMStopEndTime - gData.dwMStopStartTime;

			gData.dwMStopStartTime = GetTickCount();
			if (sType == "ALARM")	strLog.Format("%s: %s,%d", sType, sMsg, gData.dwMStopProcTime);
			else					strLog.Format("%s,%d", sType, gData.dwMStopProcTime);

		} else {
			gData.dwMStopStartTime = GetTickCount();
			if (sType == "ALARM")	strLog.Format("%s: %s", sType, sMsg);
			else					strLog.Format("%s", sType);
		}
	} else if (sType == "RUN_START" || sType == "PROGRAM_EXIT") {
		gData.dwMStopEndTime = GetTickCount();
		if (gData.dwMStopStartTime < 1) gData.dwMStopProcTime = 0;
		else							gData.dwMStopProcTime = gData.dwMStopEndTime - gData.dwMStopStartTime;

		strLog.Format("%s,%d", sType, gData.dwMStopProcTime);

		gData.dwMStopStartTime = 0;
	}

	g_objLogFile.Save_MachineStopLog(strLog);
}

void CWorkDlg::Set_AutoRun(BOOL bAutoRun)
{
	if (bAutoRun) m_rdoWorkStart.SetCheck(TRUE);
	else m_rdoWorkStop.SetCheck(TRUE);
}

void CWorkDlg::Set_State(int nState)
{
	switch (nState) {
	case STATE_NONE:
	case STATE_INIT:
	case STATE_STOP:	// Stop
		m_rdoWorkStart.Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
		m_rdoWorkStop.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);
		m_rdoWorkStop.SetCheck(TRUE);
		m_chkCycleStop.EnableWindow(TRUE);
		break;
	case STATE_RUN:		// Start
		m_rdoWorkStart.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);
		m_rdoWorkStop.Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
		m_chkCycleStop.EnableWindow(FALSE);
		break;
	case STATE_ALARM:
	case STATE_ERROR:	// Error
		m_rdoWorkStart.Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
		m_rdoWorkStop.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);
		m_rdoWorkStop.SetCheck(TRUE);
		break;
	}
}

void CWorkDlg::Enable_UserInput(int nNo, BOOL bEnable)
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	if (nNo == 1) {
		m_stcLotIdR.EnableWindow(bEnable);
		m_stcTrayCountR.EnableWindow(bEnable);
		m_stcCmCountR.EnableWindow(bEnable);
	} 
	if (nNo == 2) {
		m_stcLotIdS.EnableWindow(bEnable);
		m_stcTrayCountS.EnableWindow(bEnable);
		m_stcCmCountS.EnableWindow(bEnable);
	}
}

void CWorkDlg::Get_LotInfo(int nNo)
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	CString strTemp;
	if (nNo == 1) {
		m_stcLotIdR.GetWindowText(strTemp);		// Lot ID
		gData.sLotID[0] = strTemp;

		m_stcCmCountR.GetWindowText(strTemp);	// CM 수량
		gData.nCmUseCount[0] = atoi(strTemp);

		m_stcTrayCountR.GetWindowText(strTemp);	// Tray Count
		gData.nTrayUseCount[0] = atoi(strTemp);
	} 
	if (nNo == 2) {
		m_stcLotIdS.GetWindowText(strTemp);		// Lot ID
		gData.sLotID[1] = strTemp;

		m_stcCmCountS.GetWindowText(strTemp);	// CM 수량
		gData.nCmUseCount[1] = atoi(strTemp);

		m_stcTrayCountS.GetWindowText(strTemp);	// Tray Count
		gData.nTrayUseCount[1] = atoi(strTemp);
	}
}

void CWorkDlg::Set_LotInfo(int nNo)
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	CString strTemp;
	if (nNo == 1) {
		m_stcLotIdR.SetWindowText(gData.sLotID[0]);		// Lot ID

		strTemp.Format("%d", gData.nCmUseCount[0]);
		m_stcCmCountR.SetWindowText(strTemp);	// CM 수량

		int nCmCnt = gData.nCmUseCount[0];
		int nTrayUseCount = gData.nCmUseCount[0] / gData.nCmMaxCount;		
		if (nCmCnt % gData.nCmMaxCount) nTrayUseCount++;
		strTemp.Format("%d", nTrayUseCount);
		m_stcTrayCountR.SetWindowText(strTemp);

	} 
	if (nNo == 2) {
		m_stcLotIdS.SetWindowText(gData.sLotID[1]);		// Lot ID

		strTemp.Format("%d", gData.nCmUseCount[1]);
		m_stcCmCountS.SetWindowText(strTemp);	// CM 수량

		int nCmCnt = gData.nCmUseCount[1];
		int nTrayUseCount = gData.nCmUseCount[1] / gData.nCmMaxCount;		
		if (nCmCnt % gData.nCmMaxCount) nTrayUseCount++;
		strTemp.Format("%d", nTrayUseCount);
		m_stcTrayCountS.SetWindowText(strTemp);
	}
}

void CWorkDlg::Get_CapLotID()
{
	CString strTemp;
	m_stcCapLotIdS.GetWindowText(strTemp);		// Lot ID
	if (strTemp.GetLength() < 2) return;
	gData.sCapLotID = strTemp;	
}

void CWorkDlg::Get_ShipLotID()
{
	CString strTemp;
	m_stcShipLotIdS.GetWindowText(strTemp);
	if (strTemp.GetLength() < 2) return;
	gData.sShipLotID = strTemp;
}

BOOL CWorkDlg::Check_CapLotID()
{
	CString strTemp;
	int nTempCnt;

	m_stcCapLotIdS.GetWindowText(strTemp);		// Lot ID
	if (strTemp.GetLength() < 2) { return FALSE; }

	m_stcCapCntS.GetWindowText(strTemp);	// CM 수량
	nTempCnt = atoi(strTemp);
	if (nTempCnt < 1) { return FALSE; }

	return TRUE;
}

BOOL CWorkDlg::Check_ShipLotID()
{
	CString strRead;

	m_stcShipLotIdS.GetWindowText(strRead);
	if (strRead.GetLength() < 2) return FALSE;

	m_stcShipCntS.GetWindowText(strRead);
	if (strRead.GetLength() < 1 || atoi(strRead) < 1) return FALSE;

	return TRUE;
}

BOOL CWorkDlg::Set_CapLotIDChange()
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	CString strTemp;

	m_stcCapLotIdS.GetWindowText(strTemp);		// Lot ID
	if (strTemp.GetLength() < 2) { return FALSE; }
	m_stcCapLotId.SetWindowText(strTemp);
	m_stcCapLotIdS.SetWindowText("");
	gData.sCapLotID = strTemp;

	m_stcCapCntS.GetWindowText(strTemp);	// CM 수량
	m_stcCapCnt.SetWindowText(strTemp);
	m_stcCapCntS.SetWindowText("0");
	gData.nCapUseCount = atoi(strTemp);

	m_stcCapTrayCntS.GetWindowText(strTemp);	// Tray Count
	m_stcCapTrayCnt.SetWindowText(strTemp);
	m_stcCapTrayCntS.SetWindowText("0");
	gData.nCapTrayUseCount = atoi(strTemp);

	return TRUE;
}

BOOL CWorkDlg::Set_ShipLotIDChange()
{
	CString strTemp;

	m_stcShipLotIdS.GetWindowText(strTemp);		// Lot ID
	if (strTemp.GetLength() < 2) { return FALSE; }
	m_stcShipLotId.SetWindowText(strTemp);
	m_stcShipLotIdS.SetWindowText("");
	gData.sShipLotID = strTemp;

	m_stcShipCntS.GetWindowText(strTemp);	// CM 수량
	m_stcShipCnt.SetWindowText(strTemp);
	m_stcShipCntS.SetWindowText("0");
	gData.nShipUseCount = atoi(strTemp);		//gData.nShipUseCount

	m_stcShipTrayCntS.GetWindowText(strTemp);	// Tray Count
	m_stcShipTrayCnt.SetWindowText(strTemp);
	m_stcShipTrayCntS.SetWindowText("0");
	gData.nShipTrayUseCount = atoi(strTemp);	//gData.nShipTrayUseCount

	return TRUE;
}

int CWorkDlg::Check_CapShipLotID(int nType, CString sBarID)
{
	char chSep = '/';
	CString sPart, sLotId, sCnt, sTemp;

	AfxExtractSubString(sPart, sBarID, 0, chSep);	sPart.Trim();
	AfxExtractSubString(sCnt, sBarID, 1, chSep);	sCnt.Trim();
	AfxExtractSubString(sLotId, sBarID, 2, chSep);	sLotId.Trim();
	int nCount = atoi(sCnt);

	//'/' 구분 바코드 자릿수 체크 
	if(sPart.GetLength() < 10) { // 바코드 part number 자릿수 10 이하 체크 
		sTemp.Format("Part number를 체크해 주십시오\nPart number(%s)/수량/LotID", sPart);
		g_objCommon.Show_MsgBox(1, sTemp);
		return 0;
	}
	if (g_objCommon.Check_CapShipPoatNo(nType, sPart) == FALSE) {
		if (nType == 1) sTemp.Format("Cap Parts가 아닙니다. 확인해 주세요.\nPart number(%s)", sPart);
		if (nType == 2) sTemp.Format("Ship Parts가 아닙니다. 확인해 주세요.\nPart number(%s)", sPart);
		g_objCommon.Show_MsgBox(1, sTemp);
		return 0;
	}

	if(sCnt.GetLength() < 2) { // 모듈 수량 자릿수 2 이하 체크 
		sTemp.Format("수량을 체크해 주십시오\nPart number/수량(%s)/LotID", sCnt);
		g_objCommon.Show_MsgBox(1, sTemp);
		return 0;
	}
	if(nCount < 1) {
		sTemp.Format("수량을 체크해 주십시오\nPart number/수량(%d)/LotID", nCount);
		g_objCommon.Show_MsgBox(1, sTemp);
		return 0;
	}
	if(sLotId.GetLength() < 11) { // 모듈 수량 자릿수 11 이하 체크 
		sTemp.Format("Lot ID를 체크해 주십시오\nPart number/수량/LotID(%s)", sLotId);
		g_objCommon.Show_MsgBox(1, sTemp);
		return 0;
	}

	if (nType == 1) {	//Cap만 중복체크
		BOOL ret = g_objLogFile.Check_BarcodeLog(sBarID);
		if(!ret){
			sTemp.Format("2주 내에 같은 바코드를 사용하였습니다.체크 바랍니다.\n(%s)", sBarID);
			g_objCommon.Show_MsgBox(1, sTemp);
			return 0;
		}
	}

	return nCount;
}

///////////////////////////////////////////////////////////////////////////////
// Message

LRESULT CWorkDlg::OnUpdateModel(WPARAM wParam, LPARAM lParam)
{
	CString strText;

	strText.Format("%d", gData.nTrayUseCount[0]);
	m_stcTrayCountR.SetWindowText(strText);

	strText.Format("%d", gData.nCmUseCount[0]);
	m_stcCmCountR.SetWindowText(strText);

	strText.Format("%d", gData.nTrayUseCount[1]);
	m_stcTrayCountS.SetWindowText(strText);

	strText.Format("%d", gData.nCmUseCount[1]);
	m_stcCmCountS.SetWindowText(strText);

	return 0;
}

LRESULT CWorkDlg::OnUpdateTrayInfo(WPARAM nTray, LPARAM lParam)
{
	CString strText;
	int nNo = (int)lParam;

	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	if (nTray == 0 || nTray == 1) {		// Load Tray
		strText.Format("%d", gData.nTNoLoadTray[nNo]);
		m_stcLoadTrayCount[0].SetWindowText(strText);

		int nTrayX = pEquipData->nLoadTrayX;
		int nTrayY = pEquipData->nLoadTrayY;
		for (int i = 0; i < nTrayY; i++) {
			for (int j = 0; j < nTrayX; j++) {
				if		(gData.InfoLoadTray[i][j] == 9) m_grdLoadTray[0].Set_CellBackClr(i, j, RGB(0xFF, 0xFF, 0x00));	// Reserve
				else if (gData.InfoLoadTray[i][j] == 0) m_grdLoadTray[0].Set_CellBackClr(i, j, RGB(0xFF, 0xFF, 0xFF));	// Empty
				else									m_grdLoadTray[0].Set_CellBackClr(i, j, RGB(0x80, 0x80, 0x80));	// Error
			}
		}
		g_dlgOperator.Update_TrayInfo(nTray);
	}
	if (nTray == 0 || nTray == 2) {		// Cap Tray
		if (gData.nCapTrayCount >= 999) strText.Format("0");
		else						    strText.Format("%d", gData.nCapTrayCount);
		m_stcCapTrayCount[0].SetWindowText(strText);

		int nTrayX = pEquipData->nCapTrayX;
		int nTrayY = pEquipData->nCapTrayY;
		for (int i = 0; i < nTrayY; i++) {
			for (int j = 0; j < nTrayX; j++) {
				if		(gData.InfoCapTray[i][j] == 9) m_grdCapTray[0].Set_CellBackClr(i, j, RGB(0x00, 0xFF, 0xFF));	// Reserve
				else if (gData.InfoCapTray[i][j] == 0) m_grdCapTray[0].Set_CellBackClr(i, j, RGB(0xFF, 0xFF, 0xFF));	// Empty
				else								   m_grdCapTray[0].Set_CellBackClr(i, j, RGB(0x80, 0x80, 0x80));	// Error
			}
		}
		g_dlgOperator.Update_TrayInfo(nTray, nNo);
	}

	if (nTray == 0 || nTray == 3) {		// Unload Tray
//		strText.Format("%d", gData.nTNoUnloadTray);
		if (gData.nShipTrayCount >= 999) strText.Format("0");
		else							 strText.Format("%d", gData.nShipTrayCount);
		m_stcUnloadTrayCount[0].SetWindowText(strText);
		
		int nTrayX = pEquipData->nUnloadTrayX;
		int nTrayY = pEquipData->nUnloadTrayY;
		for (int i = 0; i < nTrayY; i++) {
			for (int j = 0; j < nTrayX; j++) {
				if		(gData.InfoUnloadTray[i][j] >  1) m_grdUnloadTray[0].Set_CellBackClr(i, j, RGB(0x70, 0x70, 0x70));	// Not Use
				else if	(gData.InfoUnloadTray[i][j] == 1) m_grdUnloadTray[0].Set_CellBackClr(i, j, RGB(0x00, 0xFF, 0x00));	// Good
				else if (gData.InfoUnloadTray[i][j] == 0) m_grdUnloadTray[0].Set_CellBackClr(i, j, RGB(0xFF, 0xFF, 0xFF));	// Empty
				else									  m_grdUnloadTray[0].Set_CellBackClr(i, j, RGB(0x80, 0x80, 0x80));	// Error
			}
		}
		g_dlgOperator.Update_TrayInfo(nTray, nNo);
	}

	return 0;
}

LRESULT CWorkDlg::OnUpdateBarcode(WPARAM wParam, LPARAM lParam)
{
	CString sData = g_objBarcodeLot.Get_BarcodeLot();
	sData.Replace("\r","");
	sData.Replace("\n","");
	if (sData.GetLength() < 1) return 0;

	int nType = 0;
	if		(m_bCapLotIdInput)  nType = 1;
	else if (m_bShipLotIdInput) nType = 2;
	else						return 0;
	int nCapCnt = Check_CapShipLotID(nType, sData);
	if (nCapCnt < 1) return 0;

	CString strTemp;
	if (m_bCapLotIdInput) {
		gData.sCapLotID = sData;
		m_stcCapLotIdS.SetWindowText(gData.sCapLotID);

		gData.nCapUseCount = nCapCnt;	//Cap수량
		strTemp.Format("%d", gData.nCapUseCount);
		m_stcCapCntS.SetWindowText(strTemp);

		gData.nCapTrayUseCount = nCapCnt / gData.nCapMaxCount;
		if (nCapCnt % gData.nCapMaxCount) gData.nCapTrayUseCount++;
		strTemp.Format("%d", gData.nCapTrayUseCount);
		m_stcCapTrayCntS.SetWindowText(strTemp);
	}

	if (m_bShipLotIdInput) {
		gData.sShipLotID = sData;
		m_stcShipLotIdS.SetWindowText(gData.sShipLotID);

		gData.nShipTrayUseCount = nCapCnt;	//Tray수량
		strTemp.Format("%d", gData.nShipTrayUseCount);
		m_stcShipTrayCntS.SetWindowText(strTemp);

		gData.nShipUseCount = gData.nShipTrayUseCount * gData.nShipMaxCount;
		strTemp.Format("%d", gData.nShipUseCount);
		m_stcShipCntS.SetWindowText(strTemp);
	}

	return 0;
}

LRESULT CWorkDlg::OnUpdateLoadCell(WPARAM wParam, LPARAM lParam)
{
	CString strTemp;
	int nNo = (int)wParam;
	double dData;
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	if (nNo == pEquipData->nAssyLoadCellPort) dData = g_objLoadCell.Get_AssyLoadCell();
	if (nNo == pEquipData->nUnloadLoadCellPort) dData = g_objLoadCell.Get_UnloadLoadCell();

	if (dData < 0) return 0;

	if (nNo == pEquipData->nAssyLoadCellPort) {
		gData.dLoadCell[0] = dData;
	} else {
		gData.dLoadCell[1] = dData;
	}
	gData.bLoadCellComplete = TRUE;
	return 0;
}

LRESULT CWorkDlg::OnResetCycleStop(WPARAM wParam, LPARAM lParam)
{
	gData.bCycleStop = FALSE;
	m_chkCycleStop.SetCheck(FALSE);
	m_chkCycleStop.Set_Color(RGB(0x00, 0x00, 0x00), RGB(0xF0, 0xF0, 0xF0));
	//if (wParam == 1) g_objSequenceInit.Set_InitComplete(FALSE);	// 초기화 필요

	return 0;
}

LRESULT CWorkDlg::OnUpdateUph(WPARAM wParam, LPARAM lParam)
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	int nHour = time.wHour;
	double	dDispTakt[4] = { 0.0 };
	double	dCycleTime = 0.0;
	CString strText;

	if (g_objSequenceMain.m_dwULCycleTime != 0) {
//		dCycleTime = g_objSequenceMain.m_dwULCycleTime / (double)(gData.nTrayX * gData.nTrayY) / 1000.0;
// 		if (!gData.bR04B) dCycleTime = g_objSequenceMain.m_dwULCycleTime / (double)(gData.nTrayX * gData.nTrayY) / 1000.0;
// 		if (gData.bR04B) dCycleTime = g_objSequenceMain.m_dwULCycleTime / (double)(10.0) / 1000.0;
	}
// 	if (dCycleTime == 0.0) strText = " Cycle Time :";
// 	else strText.Format(" Cycle Time : %0.5f", dCycleTime);
// 	m_stcCycleTime.SetWindowText(strText);
// 	strText.Format("LotID,%s,Cycle Time,%0.6lf", gLot.sLotID[gData.nULPNo-1], dCycleTime);
// 	g_objLogFile.Save_HandlerLog(strText);
	if (wParam == 1) return 0;

	dDispTakt[0] = gUph.dTaktTime;		// 현재 Lot

	int nCount = gUph.nLotCount[nHour];
	for (int i = 0; i < nCount; i++) dDispTakt[1] += gUph.dTakt[nHour][i];
	dDispTakt[1] /= nCount;				// 1시간

	int nTotalCount = 0;
	if (nHour >= 7 && nHour < 19) {
		for (int i = 7; i < 19; i++) {
			nCount = gUph.nLotCount[i];
			for (int j = 0; j < nCount; j++) dDispTakt[2] += gUph.dTakt[i][j];
			nTotalCount += nCount;
		}
	} else {
		for (int i = 0; i < 7; i++) {
			nCount = gUph.nLotCount[i];
			for (int j = 0; j < nCount; j++) dDispTakt[2] += gUph.dTakt[i][j];
			nTotalCount += nCount;
		}

		for (int i = 19; i < 24; i++) {
			nCount = gUph.nLotCount[i];
			for (int j = 0; j < nCount; j++) dDispTakt[2] += gUph.dTakt[i][j];
			nTotalCount += nCount;
		}
	}
	dDispTakt[2] /= nTotalCount;		// 12시간

	nTotalCount = 0;
	for (int i = 0; i < 24; i++) {
		nCount = gUph.nLotCount[i];
		for (int j = 0; j < nCount; j++) dDispTakt[3] += gUph.dTakt[i][j];
		nTotalCount += nCount;
	}
	dDispTakt[3] /= nTotalCount;		// 1일

	// Takt & UPH
	for (int i = 0; i < 4; i++) {
		if (dDispTakt[i] == 0) strText = "";
		else strText.Format("%0.5f", dDispTakt[i]);
		m_stcTakt[i].SetWindowText(strText);

		if (dDispTakt[i] == 0) strText = "";
		else strText.Format("%d", int(3600 / dDispTakt[i]));
		m_stcUph[i].SetWindowText(strText);
	}

	//생산량
	for (int i = 0; i < 2; i++) {
		if (gUph.nCmCount[i] == 0) strText = "";
		else strText.Format("%d", gUph.nCmCount[i]);
		m_stcDay[i].SetWindowText(strText);
	}

	return 0;
}

LRESULT CWorkDlg::OnVisionResult(WPARAM wParam, LPARAM lParam)
{
	CString strText;
	int nTotal = gLot.nCmCount[0];

	if (wParam == 1 || wParam == 0) {
		strText.Format("%d", nTotal);	m_stcVisionResult[0].SetWindowText(strText);

		m_stcVisionResult[2].Init_Ctrl("바탕", 11, TRUE, RGB(0x00, 0xFF, 0x00), RGB(0x00, 0x00, 0x00));
		m_stcVisionResult[3].Init_Ctrl("바탕", 11, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x00, 0x00));
		m_stcVisionResult[2].SetWindowText(gLot.sLotID[0]);
		m_stcVisionResult[3].SetWindowText(gLot.sLotID[1]);	
	}
	if (wParam == 2 || wParam == 0) {
		nTotal = gLot.nCmCount[1];
		strText.Format("%d", nTotal);	m_stcVisionResult[1].SetWindowText(strText);

		m_stcVisionResult[2].Init_Ctrl("바탕", 11, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x00, 0x00));
		m_stcVisionResult[3].Init_Ctrl("바탕", 11, TRUE, RGB(0x00, 0xFF, 0x00), RGB(0x00, 0x00, 0x00));
		m_stcVisionResult[2].SetWindowText(gLot.sLotID[0]);
		m_stcVisionResult[3].SetWindowText(gLot.sLotID[1]);
	}
	return 0;
}

LRESULT CWorkDlg::OnLotStartEnd(WPARAM wParam, LPARAM lParam)
{
	CCME8000Dlg *pMainDlg = (CCME8000Dlg*)AfxGetApp()->GetMainWnd();
	int nNo = lParam;
	if (wParam == 1) {
		pMainDlg->Set_LotErrorLog("LOT START", 901, "Lot Start", nNo);
	} else if (wParam == 2) {
		pMainDlg->Set_LotErrorLog("LOT END", 902, "Lot End", nNo);
	}
	return 0;
}

LRESULT CWorkDlg::OnIndexTack(WPARAM wParam, LPARAM lParam)
{
	CString strTemp;
	strTemp.Format("Index Tack : %0.3lf", (double)(wParam / 1000.0));
	m_stcCycleTime.SetWindowText(strTemp);
	return 0;
}

LRESULT CWorkDlg::OnModelChange(WPARAM wParam, LPARAM lParam)
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	Initial_Grid(&m_grdLoadTray[0], pEquipData->nLoadTrayY, pEquipData->nLoadTrayX);
	Initial_Grid(&m_grdCapTray[0], pEquipData->nCapTrayY, pEquipData->nCapTrayX);
	Initial_Grid(&m_grdUnloadTray[0], pEquipData->nUnloadTrayY, pEquipData->nUnloadTrayX);

	g_dlgOperator.Set_GridModelChange();

	return 0;
}

LRESULT CWorkDlg::OnShowMsg(WPARAM wParam, LPARAM lParam)
{
	if(wParam == 99) {
		EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
		m_bmpEquipment.DeleteObject();
		if (pEquipData->bUseDoorLock) m_bmpEquipment.LoadBitmap(IDB_EQUIP_WORK);
		else m_bmpEquipment.LoadBitmap(IDB_EQUIP_DOOR);
		m_imgEquipment.SetBitmap(m_bmpEquipment);
		return 0;
	}

	//g_dlgWork.PostMessage(UM_LOT_END_MSG, NULL, NULL);
	if(wParam == 1) g_objCommon.Show_MsgBox(1, "NG Tray Full 상태입니다.\nNG Tray 교체 해주십시오.");
	else if(wParam == 2) g_objCommon.Show_MsgBox(1, "현재 Lot이 종료 되었습니다. Tray 배출 해주십시오.");
	else if(wParam == 3) g_objCommon.Show_Alarm("Cap Tray 자재 정보를 입력해 주십시오.", STATE_CAPTRAY);
	else if(wParam == 4) g_objCommon.Show_Alarm("Cap Tray 투입 해주십시오.", STATE_CAPTRAY);
	else if(wParam == 5) g_objCommon.Show_Alarm("Ship Tray 투입 해주십시오.", STATE_SHIPTRAY);
	else if(wParam == 6) g_objCommon.Show_Alarm("Ship Tray 자재 정보를 입력해 주십시오.", STATE_SHIPTRAY);
	else 	g_objCommon.Show_MsgBox(1, "Lot의 마지막 트레이 작업 중입니다.\n배출 준비 해주십시오.");

	return 0;
}

///////////////////////////////////////////////////////////////////////////////

void CWorkDlg::OnBnClickedButton1()
{
	g_dlgWork.PostMessage(UM_UPDATE_BARCODE, NULL, NULL);
	g_dlgWork.PostMessage(UM_SHOW_MSG, 6, NULL);

	CString strTemp;
	m_stcCapLotIdS.GetWindowText(strTemp);		// Lot ID
	int nLength = strTemp.GetLength();
	if (nLength < 2) { AfxMessageBox(strTemp); return; }
// 	int nCpWorkTray = 0;		// 현재 작업중인 트레이
// 
// 	int nCpPosX = 0;			// Tray X
// 	int nCpPosY = 0;			// Tray Y
// 	int nCpStart = 0;		// Picker Start No
// 	int nCpPickCnt = 0;		// Picker가 잡을수 있는 수
// 	int nCpDownSu = 0;		// 최종적으로 Picker가 Down되는 수
// 
// 	int nCpTrayLine = 0;		// Cap Tray Line
// 	int nCpTrayCnt = 0;		// Tray Cnt	
// 
// 	int m_nCapPickMultiCnt = 0;
// 
// 	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
// 
// 	if (g_objSequenceMain.Check_CapPickerFull()) {
// 		for (int i=0; i<6; i++) gData.InfoCapPick[i] = 0;
// 		return;
// 	}
// 
// 	if (g_objSequenceMain.Select_CapPickerPos(nCpStart, nCpPickCnt)) {
// 		if (g_objSequenceMain.Select_CapTrayPos(nCpPosX, nCpPosY, nCpTrayCnt)){
// 
// 			// Cap 건너뛰어 PickUp						
// 			nCpDownSu = ((nCpPickCnt < nCpTrayCnt) ? nCpPickCnt : nCpTrayCnt);
// 			m_nCapPickMultiCnt = nCpDownSu;
// 
// 			int nIndex = nCpStart + ((nCpWorkTray-1)*6);	// Cap Picker Y축 MoveData Index
// 		}
// 	}
// 
// 	int nTX, nTY;
// 	for (int i=0; i<nCpDownSu; i++) {
// 		nTX = nCpPosX+(i*2);
// 		nTY = nCpPosY;
// 		gData.InfoCapPick[nCpStart+i] = gData.InfoCapTray[nTY][nTX]; gData.InfoCapTray[nTY][nTX] = 0;
// 		gData.nCapNoCapPicker[nCpStart+i] = (nCpPosY * pEquipData->nCapTrayX) + nTX + 1;
// 		gData.nCapTNoCapPicker[nCpStart+i] = gData.nCapTrayCount;
// 	}
// 	g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 2, 0);



// 	for (int i=0; i<6; i++) gData.InfoIndex[1][i] = 9;
// 	gData.bCapAlignNg[0] = 0;
// 	gData.bCapAlignNg[1] = 1;
// 	gData.bCapAlignNg[2] = 1;
// 	gData.bCapAlignNg[3] = 0;
// 	gData.bCapAlignNg[4] = 1;
// 	gData.bCapAlignNg[5] = 1;
// 
// 	DX_DATA_09 *pDX09 = g_objAJinAXL.Get_pDX09();
// 	pDX09->iAssyPickerTilt01 = TRUE;
// 	pDX09->iAssyPickerTilt02 = FALSE;
// 	pDX09->iAssyPickerTilt03 = TRUE;
// 	pDX09->iAssyPickerTilt04 = FALSE;
// 	pDX09->iAssyPickerTilt05 = FALSE;
// 	pDX09->iAssyPickerTilt06 = TRUE;
// 
// 	g_objCommon.Show_Error(4229);
// 	g_objCommon.Show_Alarm("d", STATE_CAPTRAY);
//	g_objCommon.Show_Alarm("e", STATE_SHIPTRAY);
//	g_objCommon.Show_Alarm("c", STATE_LOTEND);
// 	SYSTEMTIME time;
// 	GetLocalTime(&time);
// 	CString sCapLot = "3PRC00105A/400/H940115C001";
// 	gLot.sLotID[0] = "LOT_1234";
// 	gMes.sBarID[0][0][0] = "123456789",
// 	gData.sLotCapStart[0][0].Format("%04d-%02d-%02d %02d:%02d:%02d:%03d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
// 	gData.sLotCapEnd[0][0].Format("%04d-%02d-%02d %02d:%02d:%02d:%03d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
// 	gData.nINoUnloadPicker = 1;
// 	gData.nLNoUnloadPicker = 0;
// 	gData.dwCmTactStart[0][0] = GetTickCount();
// 
// 	theApp.uSleep(4000);
// 
// 	g_objLogFile.Save_CapLasLog(gData.sCapLotID, 1, 1, 1, 1);

// 	SYSTEMTIME time;
// 	GetLocalTime(&time);

// 	CWaitCursor wait;
// 
// 	DWORD dwStart = GetTickCount();
// 	while (TRUE) {
// 		if (GetTickCount() - dwStart > 10000) { AfxMessageBox("Time Over."); break; }
// 		//theApp.DoEvents();
// 	}
// 
// 	wait.Restore();

}

void CWorkDlg::OnBnClickedButton2()
{
// 	g_objSequenceMain.Init_CapTray();
// 
// 	for (int y = 0; y < 1; y++) {
// 		for (int x = 0; x < 2; x++) {
// 			gData.InfoCapTray[y][x] = 0;
// 		}
// 	}

// 	CCME8000Dlg *pMainDlg = (CCME8000Dlg*)AfxGetApp()->GetMainWnd();
// 	pMainDlg->Set_LampFlicker(FALSE, 5);
}
