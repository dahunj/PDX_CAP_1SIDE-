// SequenceInit.cpp : 구현 파일
//
#include "stdafx.h"
#include "CME8000.h"
#include "SequenceInit.h"
#include "math.h"

#include "LogFile.h"
#include "Common.h"
#include "SequenceMain.h"
#include "MESInterface.h"
#include "WorkDlg.h"

const double ANGLE_STAGE_LENGTH = 250.0;	// Good Tray 1/2 간섭없는 간격

CSequenceInit g_objSequenceInit;

CSequenceInit::CSequenceInit()
{
	for (int i = 0; i < AXIS_COUNT; i++) m_pStatus[i] = g_objAJinAXL.Get_pStatus(i);
	for (int i = 0; i < AXIS_COUNT; i++) m_pParam[i] = g_objAJinAXL.Get_pParam(i);
	
	m_pDX00 = g_objAJinAXL.Get_pDX00();
	m_pDX01 = g_objAJinAXL.Get_pDX01();
	m_pDX02 = g_objAJinAXL.Get_pDX02();
	m_pDX03 = g_objAJinAXL.Get_pDX03();
	m_pDX04 = g_objAJinAXL.Get_pDX04();
	m_pDX05 = g_objAJinAXL.Get_pDX05();
	m_pDX06 = g_objAJinAXL.Get_pDX06();
	m_pDX07 = g_objAJinAXL.Get_pDX07();
	m_pDX08 = g_objAJinAXL.Get_pDX08();
	m_pDX09 = g_objAJinAXL.Get_pDX09();
	m_pDX10 = g_objAJinAXL.Get_pDX10();
	m_pDX11 = g_objAJinAXL.Get_pDX11();
	m_pDX12 = g_objAJinAXL.Get_pDX12();
	m_pDX13 = g_objAJinAXL.Get_pDX13();

	m_pDY00 = g_objAJinAXL.Get_pDY00();
	m_pDY01 = g_objAJinAXL.Get_pDY01();
	m_pDY02 = g_objAJinAXL.Get_pDY02();
	m_pDY03 = g_objAJinAXL.Get_pDY03();
	m_pDY04 = g_objAJinAXL.Get_pDY04();
	m_pDY05 = g_objAJinAXL.Get_pDY05();
	m_pDY06 = g_objAJinAXL.Get_pDY06();
	m_pDY07 = g_objAJinAXL.Get_pDY07();
	m_pDY08 = g_objAJinAXL.Get_pDY08();
	m_pDY09 = g_objAJinAXL.Get_pDY09();
	m_pDY10 = g_objAJinAXL.Get_pDY10();
	m_pDY11 = g_objAJinAXL.Get_pDY11();
	m_pDY12 = g_objAJinAXL.Get_pDY12();
	m_pDY13 = g_objAJinAXL.Get_pDY13();

	m_pEquipData = g_objDataManager.Get_pEquipData();
	m_pMoveData = g_objDataManager.Get_pMoveData();
	
	m_bThreadInitial = FALSE;
	m_pThreadInitial = NULL;
	
	Reset_InitialCase();

	m_bInitComplete = FALSE;
}

CSequenceInit::~CSequenceInit()
{
	if (m_pThreadInitial) {
		m_bThreadInitial = FALSE;
		WaitForSingleObject(m_pThreadInitial->m_hThread, INFINITE);
	}
}

int *CSequenceInit::Get_InitialCase()
{
	static int nCase[14] = { 0 };

	nCase[0]  = m_niMainInitCase;		//  0. (Error : 1000)
	nCase[1]  = m_niLoadPickCase;		//  1. (Error : 1100)
	nCase[2]  = m_niLoadStageCase;		//  2. (Error : 1200)
	nCase[3]  = m_niMainIndexCase;		//  3. (Error : 1300)
	nCase[4]  = m_niAssyPickCase;		//  4. (Error : 1400)
	nCase[5]  = m_niCapPickCase;		//  5. (Error : 1500)
	nCase[6]  = m_niCapBufferCase; 		//  6. (Error : 1600)
	nCase[7]  = m_niCapStageCase;		//  7. (Error : 1700)
	nCase[8]  = m_niTransStageCase;		//  8. (Error : 1800)
	nCase[9]  = m_niUnloadPickCase;		//  9. (Error : 1900)
	nCase[10] = m_niUnloadStageCase;	// 10. (Error : 2000)

	return nCase;
}

void CSequenceInit::Begin_InitialThread()
{
	m_bInitComplete = FALSE;

	Init_Replay();

	if (m_niMainInitCase == 0) m_niMainInitCase = 1;

	if (m_pThreadInitial) End_InitialThread();
	m_bThreadInitial = TRUE;
	m_pThreadInitial = AfxBeginThread(Thread_Initial, NULL);
}

void CSequenceInit::End_InitialThread()
{
	if (m_pThreadInitial) {
		m_bThreadInitial = FALSE;
		WaitForSingleObject(m_pThreadInitial->m_hThread, INFINITE);
	}
}

void CSequenceInit::Init_Replay()
{
	m_tiMainInitLoop.Set_LoopTime(20000);		//  0. (Error : 1000)
	m_tiLoadPickLoop.Set_LoopTime(20000);		//  1. (Error : 1100)
	m_tiLoadStageLoop.Set_LoopTime(20000);		//  2. (Error : 1200)
	m_tiMainIndexLoop.Set_LoopTime(20000);		//  3. (Error : 1300)
	m_tiAssyPickLoop.Set_LoopTime(20000);		//  4. (Error : 1400)
	m_tiCapPickLoop.Set_LoopTime(20000);		//  5. (Error : 1500)
	m_tiCapBufferLoop.Set_LoopTime(20000);		//  6. (Error : 1600)
	m_tiCapStageLoop.Set_LoopTime(20000); 		//  7. (Error : 1700)
	m_tiTransStageLoop.Set_LoopTime(20000);		//  8. (Error : 1800)
	m_tiUnloadPickLoop.Set_LoopTime(20000);		//  9. (Error : 1900)
	m_tiUnloadStageLoop.Set_LoopTime(20000);	// 10. (Error : 2000)
}

void CSequenceInit::Reset_InitialCase()
{
	m_niMainInitCase	= 0;	//  0. (Error : 1000)
	m_niLoadPickCase	= 0;	//  1. (Error : 1100)
	m_niLoadStageCase	= 0;	//  2. (Error : 1200)
	m_niMainIndexCase	= 0;	//  3. (Error : 1300)
	m_niAssyPickCase	= 0;	//  4. (Error : 1400)
	m_niCapPickCase		= 0;	//  5. (Error : 1500)
	m_niCapBufferCase 	= 0;	//  6. (Error : 1600)
	m_niCapStageCase	= 0;	//  7. (Error : 1700)
	m_niTransStageCase	= 0;	//  8. (Error : 1800)
	m_niUnloadPickCase	= 0;	//  9. (Error : 1900)
	m_niUnloadStageCase	= 0;	// 10. (Error : 2000)
}

/////////////////////////////////////////////////////////////////////////////
// Initial Thread Function 

UINT CSequenceInit::Thread_Initial(LPVOID lpVoid)
{
	while (g_objSequenceInit.m_bThreadInitial) {
		if (!g_objCommon.Check_MainEmgAir()) break;
		if (!g_objCommon.Check_MainDoor()) break;
		if (!g_objCommon.Check_TrayFull()) break;
		if (!g_objCommon.Check_ServoOn()) break;
		if (!g_objCommon.Check_DirveAlarm()) break;

		if (!g_objSequenceInit.Initial_MainInit()) break;		//  0. (Error : 1000)
		if (!g_objSequenceInit.Initial_LoadPicker()) break;		//  1. (Error : 1100)
		if (!g_objSequenceInit.Initial_LoadStage()) break;		//  2. (Error : 1200)
		if (!g_objSequenceInit.Initial_MainIndex()) break;		//  3. (Error : 1300)
		if (!g_objSequenceInit.Initial_AssyPicker()) break;		//  4. (Error : 1400)
		if (!g_objSequenceInit.Initial_CapPicker()) break;		//  5. (Error : 1500)
		if (!g_objSequenceInit.Initial_CapBuffer()) break;		//  6. (Error : 1600)
		if (!g_objSequenceInit.Initial_CapStage()) break;		//  7. (Error : 1700)
		if (!g_objSequenceInit.Initial_TransStage()) break;		//  8. (Error : 1800)
		if (!g_objSequenceInit.Initial_UnloadPicker()) break;	//  9. (Error : 1900)
		if (!g_objSequenceInit.Initial_UnloadStage()) break;	// 10. (Error : 2000)

		Sleep(10);
	}
	g_objSequenceInit.m_bThreadInitial = FALSE;
	g_objSequenceInit.m_pThreadInitial = NULL;

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// 0. (Error : 1000)

BOOL CSequenceInit::Initial_MainInit()
{
	switch (m_niMainInitCase) {
	case 0:		// Wait
		return TRUE;

	case 1:		// Initial Start
		g_objLogFile.Save_HandlerLog("[Initial Sequence] MainInit Start");
		gLot.sLotID[0] = gLot.sLotID[1] = "";
		m_niMainInitCase++; m_tiMainInitLoop.Set_LoopTime(5000);
		break;

	case 2:		// Tray Picker Exist Check
		if (!m_pDX00->iTrayPickerExist) {
			m_niMainInitCase++; m_tiMainInitLoop.Set_LoopTime(5000);
		}
		break;
	case 3:		// Load Stage1 Exist Check
		if (!m_pDX04->iLoadStage1Exist) {
			m_niMainInitCase++; m_tiMainInitLoop.Set_LoopTime(5000);
		}
 		break;
 	case 4:		// Load Stage2 Exist Check
		if (!m_pDX04->iLoadStage2Exist) {
			m_niMainInitCase++; m_tiMainInitLoop.Set_LoopTime(5000);
		}
		break;
	case 5:		// Cap Stage1 Exist Check
		if (!m_pDX04->iCapStage1Exist) {
			m_niMainInitCase++; m_tiMainInitLoop.Set_LoopTime(5000);
		}
		break;
	case 6:		// Cap Stage2 Exist Check
		if (!m_pDX04->iCapStage2Exist) {
			m_niMainInitCase++; m_tiMainInitLoop.Set_LoopTime(5000);
		}
		break;
	case 7:		// Cap Clean Nozzle Down
		m_pDY09->oCapCleanUp = FALSE;
		g_objAJinAXL.Write_Output(9);

		m_niMainInitCase++; m_tiMainInitLoop.Set_LoopTime(5000);
		break;
	case 8:		// Load Port1 Tray Check
		if (!m_pDX00->iLoadPort1LowCheck) {
			m_niMainInitCase = 11; m_tiMainInitLoop.Set_LoopTime(5000);
		}
		break;

	case 11:	// Load Picker Exist Check
		if (!m_pDX06->iLoadPickerCMCheck01 && !m_pDX06->iLoadPickerCMCheck02 && !m_pDX06->iLoadPickerCMCheck03 &&
			!m_pDX06->iLoadPickerCMCheck06 && !m_pDX06->iLoadPickerCMCheck05 && !m_pDX06->iLoadPickerCMCheck06) {
			m_niMainInitCase++; m_tiMainInitLoop.Set_LoopTime(5000);
		}
		break;
	case 12:	// Cap Picker Exist Check
		if (g_objCommon.Get_CapPickerVacOff(0)) {
			m_niMainInitCase++; m_tiMainInitLoop.Set_LoopTime(5000);
		}
		break;
	case 13:	// Assy Picker Exist Check
		if (g_objCommon.Get_AssyPickerVacOff(0)) {
			m_niMainInitCase++; m_tiMainInitLoop.Set_LoopTime(5000);
		}
		break;
	case 14:	// Unload Picker Exist Check
		if (g_objCommon.Get_UnloadPickerVacOff(0)) {
			m_niMainInitCase = 21; m_tiMainInitLoop.Set_LoopTime(5000);
		}
		break;

	case 21:	// Trans Stage Exist Check
		if (g_objCommon.Get_TransStageVacOff(0)) {
			m_niMainInitCase++; m_tiMainInitLoop.Set_LoopTime(5000);
		}
		break;
	case 22:	// Unload Stage1 Exist Check
		if (!m_pDX05->iUnloadStage1Exist) {
			m_niMainInitCase++; m_tiMainInitLoop.Set_LoopTime(5000);
		}
		break;
	case 23:	// Unload Stage2 Exist Check
		if (!m_pDX05->iUnloadStage2Exist) {
			m_niMainInitCase = 50; m_tiMainInitLoop.Set_LoopTime(5000);
		}
		break;
		
	case 50:	// 각 파트 초기화 진행
		if (m_niLoadPickCase == 0)		m_niLoadPickCase = 1;	//  1. (Error : 1100)
		if (m_niAssyPickCase == 0)		m_niAssyPickCase = 1;	//  4. (Error : 1400)
		if (m_niCapPickCase == 0)		m_niCapPickCase = 1;	//  5. (Error : 1500)		
		if (m_niUnloadPickCase == 0)	m_niUnloadPickCase = 1;	//  9. (Error : 1900)
		m_niMainInitCase++;
		break;

	case 51:	// Wait 
		if (m_niLoadPickCase == 90 && m_niLoadStageCase == 90 && m_niMainIndexCase == 90 &&
			m_niAssyPickCase == 90 && m_niCapPickCase == 90 && m_niCapBufferCase == 90 &&
			m_niCapStageCase == 90 && m_niTransStageCase == 90 && m_niUnloadPickCase == 90 &&
			m_niUnloadStageCase == 90) {
			m_niMainInitCase++; m_tiMainInitLoop.Set_LoopTime(5000);
		}
		return TRUE;
	case 52:	// Port Lamp Off
		m_pDY12->oLoad1Lamp = FALSE;
		m_pDY12->oLoad2Lamp = FALSE;
		m_pDY12->oLoad3Lamp = FALSE;
		m_pDY12->oCap1Lamp = FALSE;
		m_pDY12->oCap2Lamp = FALSE;
		m_pDY12->oUnload1Lamp = FALSE;
		m_pDY12->oUnload2Lamp = FALSE;
		g_objAJinAXL.Write_Output(12);
		m_niMainInitCase = 90; m_tiMainInitLoop.Set_LoopTime(5000);
		break;
		
	case 90:	// Initial Complete
		m_bInitComplete = TRUE;

		Reset_InitialCase();

		// 변수 초기화
		g_objSequenceMain.Reset_MainRunCase();
		g_objSequenceMain.Set_ClearRunData(0);
		if (gData.bCycleStop) g_dlgWork.PostMessage(UM_RESET_CYCLE_STOP, NULL, NULL);

		g_objLogFile.Save_HandlerLog("[Initial Sequence] MainInit Complete");
		m_niMainInitCase = 0;
		return FALSE;	// Thread_Initial 종료
	}

	// 1. (Error : 1000)
	if (m_tiMainInitLoop.Over_LoopTime()) {
		g_objCommon.Show_Error(1000 + m_niMainInitCase);
		return FALSE;
	}
	return TRUE;
}

// /////////////////////////////////////////////////////////////////////////////
// 1. (Error : 1100)
BOOL CSequenceInit::Initial_LoadPicker()
{
	static double dILpPickY = 0.0;	// Load Picker Y

	switch (m_niLoadPickCase) {
	case 0:		// Wait
		return TRUE;

	case 1:		// Load Picker Initial Start
		g_objLogFile.Save_HandlerLog("[Initial Sequence] - Load Picker Start");
		m_niLoadPickCase++; m_tiLoadPickLoop.Set_LoopTime(5000);
		break;
	case 2:		// Tray Picker Open
		if (!m_pDX00->iTrayPickerExist) {
			m_pDY00->oTrayPickerGrip12Open = TRUE; m_pDY00->oTrayPickerGrip12Close = FALSE;
			m_pDY00->oTrayPickerGrip34Open = TRUE; m_pDY00->oTrayPickerGrip34Close = FALSE;
			g_objAJinAXL.Write_Output(0);
			m_niLoadPickCase++; m_tiLoadPickLoop.Set_LoopTime(5000);
		}
		break;
	case 3:		// Picker Up/Open
		if (g_objCommon.Get_TrayPickerGripOpen()) {
			if (!m_pDX06->iLoadPickerCMCheck01 && !m_pDX06->iLoadPickerCMCheck02 && !m_pDX06->iLoadPickerCMCheck03 &&
				!m_pDX06->iLoadPickerCMCheck06 && !m_pDX06->iLoadPickerCMCheck05 && !m_pDX06->iLoadPickerCMCheck06) {
				g_objCommon.Set_LoadPickerUp();
				g_objCommon.Set_LoadPickerOpen();
				m_niLoadPickCase++; m_tiLoadPickLoop.Set_LoopTime(5000);
			}
		}
		break;
	case 4:		// Tray/Load Picker Z Home
		if (g_objCommon.Get_LoadPickerUp() && g_objCommon.Get_LoadPickerOpen()) {
			g_objAJinAXL.Home_Search(AX_TRAY_PICKER_Z);
			g_objAJinAXL.Home_Search(AX_LOAD_PICKER_Z);
			m_niLoadPickCase++; m_tiLoadPickLoop.Set_LoopTime(30000);
		}
		break;
	case 5:		// Home Check
		if (g_objAJinAXL.Is_Home(AX_TRAY_PICKER_Z) && g_objAJinAXL.Is_Home(AX_LOAD_PICKER_Z)) {
			m_niLoadPickCase = 11; m_tiLoadPickLoop.Set_LoopTime(5000);
		}
		break;

	case 11:	// Tray Picker X/R, Load Picker Y/P Home
		if (g_objAJinAXL.Is_Home(AX_TRAY_PICKER_Z) && g_objAJinAXL.Is_Home(AX_LOAD_PICKER_Z)) {
			m_tiLoadPickLoop.Waiting_Time(500);
			g_objAJinAXL.Home_Search(AX_TRAY_PICKER_X);
			g_objAJinAXL.Home_Search(AX_LOAD_PICKER_Y);
			g_objAJinAXL.Home_Search(AX_LOAD_PICKER_P);
			g_objAJinAXL.Home_Search(AX_VISION_CM_ALIGN_X);
			m_niLoadPickCase++; m_tiLoadPickLoop.Set_LoopTime(50000);
		}
		break;
	case 12:	// Picker Z Ready Position
		if (g_objAJinAXL.Is_Home(AX_TRAY_PICKER_X) && g_objAJinAXL.Is_Home(AX_VISION_CM_ALIGN_X) &&
			g_objAJinAXL.Is_Home(AX_LOAD_PICKER_Y) && g_objAJinAXL.Is_Home(AX_LOAD_PICKER_P)) {
			g_objCommon.Move_Position(AX_TRAY_PICKER_Z, 0);	// Ready Up
			g_objCommon.Move_Position(AX_LOAD_PICKER_Z, 0);	// Ready
			m_niLoadPickCase++; m_tiLoadPickLoop.Set_LoopTime(10000);
		}
	case 13:	// Tray Picker X/R, Load Picker Y/P Ready Position
		if (g_objCommon.Check_Position(AX_TRAY_PICKER_Z, 0) && g_objCommon.Check_Position(AX_LOAD_PICKER_Z, 0)) {
			g_objCommon.Move_Position(AX_TRAY_PICKER_X, 2);	// Home Search Position
			g_objCommon.Move_Position(AX_LOAD_PICKER_Y, 0);	// Load Stage1
			g_objCommon.Move_Position(AX_LOAD_PICKER_P, 0);	// Load Tray
			g_objCommon.Move_Position(AX_VISION_CM_ALIGN_X, 0);	// Ready
			m_niLoadPickCase++; m_tiLoadPickLoop.Set_LoopTime(10000);
		}
		break;
	case 14:	// Tray Picker Home
		if (g_objCommon.Check_Position(AX_TRAY_PICKER_X, 2) && g_objCommon.Check_Position(AX_VISION_CM_ALIGN_X, 0) &&
			g_objCommon.Check_Position(AX_LOAD_PICKER_Y, 0) && g_objCommon.Check_Position(AX_LOAD_PICKER_P, 0)) {
			g_objAJinAXL.Home_Search(AX_TRAY_PICKER_R);
			m_niLoadPickCase++; m_tiLoadPickLoop.Set_LoopTime(50000);
		}
		break;
	case 15:	// Tray Picker R Move to Ready Position
		if (g_objAJinAXL.Is_Home(AX_TRAY_PICKER_R)) {
			g_objCommon.Move_Position(AX_TRAY_PICKER_R, 0);	// Ready Position
			m_niLoadPickCase++; m_tiLoadPickLoop.Set_LoopTime(10000);
		}
		break;
	case 16:	// Tray Picker X Move to Load1 Position
		if (g_objCommon.Check_Position(AX_TRAY_PICKER_R, 0)) {
			//g_objCommon.Move_Position(AX_TRAY_PICKER_X, 1);	// Load1 Position
			m_niLoadPickCase++; m_tiLoadPickLoop.Set_LoopTime(10000);
		}
		break;
	case 17:	// Position Check
// 		if (g_objCommon.Check_Position(AX_TRAY_PICKER_X, 1)) {
		if (g_objCommon.Check_Position(AX_TRAY_PICKER_R, 0)) {
			g_objLogFile.Save_HandlerLog("[Initial Sequence] - Load Picker Complete");
			m_niLoadPickCase = 90; m_tiLoadPickLoop.Set_LoopTime(5000);
		}
		break;

	case 90:	// Initial Complete
		return TRUE;
	}

	// 1. (Error : 1100)
	if (m_tiLoadPickLoop.Over_LoopTime()) {
		g_objCommon.Show_Error(1100 + m_niLoadPickCase);
		return FALSE;
	}
	return TRUE;
}

// /////////////////////////////////////////////////////////////////////////////
// 2. (Error : 1200)
BOOL CSequenceInit::Initial_LoadStage()
{
	switch (m_niLoadStageCase) {
	case 0:		// Wait
		if (m_niLoadPickCase > 10) m_niLoadStageCase++;
		return TRUE;

	case 1:		// Loading Initial Start
		g_objLogFile.Save_HandlerLog("[Initial Sequence] - Load Stage Start");
		m_niLoadStageCase++; m_tiLoadStageLoop.Set_LoopTime(5000);
		break;

	case 2:		// Port1/2/3 Support In
		if (/*m_pDX00->iLoadPort1SlideClose && !m_pDX00->iLoadPort1SlideOpen &&*/	//Temp 간섭이 있다.
			m_pDX01->iLoadPort2SlideClose && !m_pDX01->iLoadPort2SlideOpen &&
			m_pDX01->iLoadPort3SlideClose && !m_pDX01->iLoadPort3SlideOpen) {
			g_objCommon.Set_LoadPortSupportIn(1);
			g_objCommon.Set_LoadPortSupportIn(2);
			m_niLoadStageCase++; m_tiLoadStageLoop.Set_LoopTime(5000);
		}
		break;
	case 3:		// Stage1 Master/Slave Out
		m_pDY04->oLoadStage1MasterIn = FALSE; m_pDY04->oLoadStage1SlaveIn = FALSE;
		g_objAJinAXL.Write_Output(4);
		m_niLoadStageCase++; m_tiLoadStageLoop.Set_LoopTime(5000);
		break;
	case 4:		// Stage2 Master/Slave Out
		if (!m_pDX04->iLoadStage1MasterIn && m_pDX04->iLoadStage1MasterOut &&
			!m_pDX04->iLoadStage1SlaveIn && m_pDX04->iLoadStage1SlaveOut) {
			m_pDY04->oLoadStage2MasterIn = FALSE; m_pDY04->oLoadStage2SlaveIn = FALSE;
			g_objAJinAXL.Write_Output(4);
			m_niLoadStageCase++; m_tiLoadStageLoop.Set_LoopTime(5000);
		}
		break;
	case 5:		// Tray2 Support In
		if (!m_pDX04->iLoadStage2MasterIn && m_pDX04->iLoadStage2MasterOut &&
			!m_pDX04->iLoadStage2SlaveIn && m_pDX04->iLoadStage2SlaveOut) {
			m_niLoadStageCase = 11; m_tiLoadStageLoop.Set_LoopTime(5000);
		}
		break;

	case 11:	// Z1/Z2 Move Up or Down Position
		if (g_objAJinAXL.Get_Position(AX_LOAD_STAGE1_Z) < g_objAJinAXL.Get_Position(AX_LOAD_STAGE2_Z)) {
			g_objCommon.Move_Position(AX_LOAD_STAGE1_Z, 0);	// Moving Down
			g_objCommon.Move_Position(AX_LOAD_STAGE2_Z, 1);	// Moving Up
			m_niLoadStageCase = 12; m_tiLoadStageLoop.Set_LoopTime(10000);
		} else {
			g_objCommon.Move_Position(AX_LOAD_STAGE2_Z, 0);	// Moving Down
			g_objCommon.Move_Position(AX_LOAD_STAGE1_Z, 1);	// Moving Up
			m_niLoadStageCase = 12; m_tiLoadStageLoop.Set_LoopTime(10000);
		}
		break;
	case 12:	// Stage1,2 X Home Search
		if ((g_objCommon.Check_Position(AX_LOAD_STAGE1_Z, 0) && g_objCommon.Check_Position(AX_LOAD_STAGE2_Z, 1)) ||
			(g_objCommon.Check_Position(AX_LOAD_STAGE1_Z, 1) && g_objCommon.Check_Position(AX_LOAD_STAGE2_Z, 0))) {
				g_objAJinAXL.Home_Search(AX_LOAD_STAGE1_X);
				g_objAJinAXL.Home_Search(AX_LOAD_STAGE2_X);
				m_niLoadStageCase = 13; m_tiLoadStageLoop.Set_LoopTime(60000);
		}
		break;
	case 13:	// Stage1 X Load Position, Stage2 X Unload Position
		if (g_objAJinAXL.Is_Home(AX_LOAD_STAGE1_X) && g_objAJinAXL.Is_Home(AX_LOAD_STAGE2_X)) {
			g_objCommon.Move_Position(AX_LOAD_STAGE1_X, 0);	// Load Position
			g_objCommon.Move_Position(AX_LOAD_STAGE2_X, 3);	// Unload Position
			m_niLoadStageCase = 14; m_tiLoadStageLoop.Set_LoopTime(30000);
		}
		break;
	case 14:	// Stage1,2 Z Home Search
		if (g_objCommon.Check_Position(AX_LOAD_STAGE1_X, 0) && g_objCommon.Check_Position(AX_LOAD_STAGE2_X, 3)) {
			m_tiLoadStageLoop.Waiting_Time(500);
			g_objAJinAXL.Home_Search(AX_LOAD_STAGE1_Z);
			g_objAJinAXL.Home_Search(AX_LOAD_STAGE2_Z);
			m_niLoadStageCase = 15; m_tiLoadStageLoop.Set_LoopTime(50000);
		}
		break;
	case 15:	// Stage1 Z Move Up, Stage2 Z Move Down
		if (g_objAJinAXL.Is_Home(AX_LOAD_STAGE1_Z) && g_objAJinAXL.Is_Home(AX_LOAD_STAGE2_Z)) {
			g_objCommon.Move_Position(AX_LOAD_STAGE1_Z, 1);	// Up Position
			g_objCommon.Move_Position(AX_LOAD_STAGE2_Z, 0);	// Down Position
			m_niLoadStageCase = 16; m_tiLoadStageLoop.Set_LoopTime(20000);
		}
		break;
	case 16:	// Port Slide Unlock
		if (g_objCommon.Check_Position(AX_LOAD_STAGE1_Z, 1) && g_objCommon.Check_Position(AX_LOAD_STAGE2_Z, 0)) {
			m_pDY00->oLoadPort1SlideLock = FALSE; m_pDY00->oLoadPort1SlideUnlock = TRUE;
			m_pDY01->oLoadPort2SlideLock = FALSE; m_pDY01->oLoadPort2SlideUnlock = TRUE;
			m_pDY01->oLoadPort3SlideLock = FALSE; m_pDY01->oLoadPort3SlideUnlock = TRUE;
			g_objAJinAXL.Write_Output(0);
			g_objAJinAXL.Write_Output(1);
			m_niLoadStageCase = 17; m_tiLoadStageLoop.Set_LoopTime(10000);
		}
		break;
	case 17:	// Port Slide Check
		if (!m_pDX00->iLoadPort1SlideLock && m_pDX00->iLoadPort1SlideUnlock &&
			!m_pDX01->iLoadPort2SlideLock && m_pDX01->iLoadPort2SlideUnlock &&
			!m_pDX01->iLoadPort3SlideLock && m_pDX01->iLoadPort3SlideUnlock) {
			g_objLogFile.Save_HandlerLog("[Initial Sequence] - Load Stage Complete");
			m_niLoadStageCase = 90; m_tiLoadStageLoop.Set_LoopTime(5000);
		}
		break;

	case 90:	// Initial Complete
		return TRUE;
	}

	// 2. (Error : 1200)
	if (m_tiLoadStageLoop.Over_LoopTime()) {
		g_objCommon.Show_Error(1200 + m_niLoadStageCase);
		return FALSE;
	}
	return TRUE;
}

// /////////////////////////////////////////////////////////////////////////////
// 3. (Error : 1300)
BOOL CSequenceInit::Initial_MainIndex()
{
	switch (m_niMainIndexCase) {
	case 0:		// Wait
		if (m_niLoadPickCase > 10 && m_niAssyPickCase > 10 && m_niTransStageCase > 10) m_niMainIndexCase++;
		return TRUE;

	case 1:		// Main Index Initial Start
		g_objLogFile.Save_HandlerLog("[Initial Sequence] - Main Index Start");
		m_niMainIndexCase++; m_tiMainIndexLoop.Set_LoopTime(5000);
		break;
	case 2:		// Main Index Vacuum Off
		g_objCommon.Set_IndexLoadVacuumOff(0);	// All Off
		g_objCommon.Set_IndexAssyVacuumOff(0);	// All Off
		m_niMainIndexCase++; m_tiMainIndexLoop.Set_LoopTime(5000);
		break;
	case 3:		// Main Index Vac Down
		if (g_objCommon.Get_IndexLoadVacuumOff(0) && g_objCommon.Get_IndexAssyVacuumOff(0)) {
			m_pDY11->oIndexLoadVacUp = FALSE;
			m_pDY11->oIndexAssyVacUp = FALSE;
			g_objAJinAXL.Write_Output(11);
			m_niMainIndexCase++; m_tiMainIndexLoop.Set_LoopTime(5000);
		}
		break;
	case 4:		// Main Index Align In
		if (!m_pDX11->iIndexLoadVacUp && m_pDX11->iIndexLoadVacDown &&
			!m_pDX11->iIndexAssyVacUp && m_pDX11->iIndexAssyVacDown) {
			m_pDY11->oIndexLoadAlignOut = FALSE;
			m_pDY11->oIndexAssyAlignOut = FALSE;
			m_pDY11->oIndexTransAlignOut = FALSE;
			g_objAJinAXL.Write_Output(11);
			m_niMainIndexCase++; m_tiMainIndexLoop.Set_LoopTime(10000);
		}
		break;
	case 5:		// Check Picker Z Home
		if (m_pDX11->iIndexLoadAlignIn && !m_pDX11->iIndexLoadAlignOut &&
			m_pDX11->iIndexAssyAlignIn && !m_pDX11->iIndexAssyAlignOut &&
			m_pDX11->iIndexTransAlignIn && !m_pDX11->iIndexTransAlignOut) {
			g_objAJinAXL.Home_Search(AX_INDEX_R);
			m_niMainIndexCase++; m_tiMainIndexLoop.Set_LoopTime(30000);
		}
		break;
	case 6:
		if (g_objAJinAXL.Is_Home(AX_INDEX_R)) {
			m_niMainIndexCase = 90; m_tiMainIndexLoop.Set_LoopTime(5000);
		}
		break;

	case 90:	// Initial Complete
		return TRUE;
	}

	// 3. (Error : 1300)
	if (m_tiMainIndexLoop.Over_LoopTime()) {
		g_objCommon.Show_Error(1300 + m_niMainIndexCase);
		return FALSE;
	}
	return TRUE;
}

// /////////////////////////////////////////////////////////////////////////////
// 4. (Error : 1400)
BOOL CSequenceInit::Initial_AssyPicker()
{
	switch (m_niAssyPickCase) {
	case 0:		// Wait
		return TRUE;

	case 1:		// Btm2 Picker Initial Start
		g_objLogFile.Save_HandlerLog("[Initial Sequence] - Assembly Picker Start");
		m_niAssyPickCase++; m_tiAssyPickLoop.Set_LoopTime(5000);
		break;
	case 2:		// Picker Vacuum Off
		g_objCommon.Set_AssyPickerVacOff(0);	// All Off
		m_niAssyPickCase++; m_tiAssyPickLoop.Set_LoopTime(5000);
		break;
	case 3:		// Picker Vacuum Air Off
		if (g_objCommon.Get_AssyPickerVacOff(0)) {
			if (!m_tiAssyPickLoop.Waiting_Time(500)) break;
			g_objCommon.Set_AssyPickerAirOff(0);	// All Off
			m_niAssyPickCase++; m_tiAssyPickLoop.Set_LoopTime(5000);
		}
		break;
	case 4:		// Picker Up
 		if (g_objCommon.Get_AssyPickerVacOff(0)) {
			g_objCommon.Set_AssyPickerUp(0);
			m_pDY08->oAssyPickerTiltDown = FALSE;
			g_objAJinAXL.Write_Output(8);
			m_niAssyPickCase++; m_tiAssyPickLoop.Set_LoopTime(5000);
		}
		break;
	case 5:		// Picker Z Home
		if (g_objCommon.Get_AssyPickerUp(0) && m_pDX08->iAssyPickerTiltUp) {
			g_objAJinAXL.Home_Search(AX_ASSY_PICKER_Z);
			m_niAssyPickCase++; m_tiAssyPickLoop.Set_LoopTime(30000);
		}
		break;
	case 6:		// Check Picker Z Home
		if (g_objAJinAXL.Is_Home(AX_ASSY_PICKER_Z)) {
			m_niAssyPickCase = 11; m_tiAssyPickLoop.Set_LoopTime(5000);
		}
		break;

	case 11:	// Picker X, Y Home
		if (g_objAJinAXL.Is_Home(AX_ASSY_PICKER_Z)) {
			g_objAJinAXL.Home_Search(AX_ASSY_PICKER_X);
			g_objAJinAXL.Home_Search(AX_ASSY_PICKER_Y);
			g_objAJinAXL.Home_Search(AX_VISION_CAP_ALIGN_Y);
			m_niAssyPickCase++; m_tiAssyPickLoop.Set_LoopTime(50000);
		}
		break;
	case 12:	//  Picker Z Move to Ready Up Position
		if (g_objAJinAXL.Is_Home(AX_ASSY_PICKER_X) && g_objAJinAXL.Is_Home(AX_ASSY_PICKER_Y) &&
			g_objAJinAXL.Is_Home(AX_VISION_CAP_ALIGN_Y)) {
			if (!m_tiAssyPickLoop.Waiting_Time(500)) break;
			g_objCommon.Move_Position(AX_ASSY_PICKER_Z, 0);	// Ready Up
			m_niAssyPickCase++; m_tiAssyPickLoop.Set_LoopTime(10000);
		}
		break;
	case 13:	// Picker X, Y Move to Ready Up Position
		if (g_objCommon.Check_Position(AX_ASSY_PICKER_Z, 0)) {
			g_objCommon.Move_Position(AX_ASSY_PICKER_X, 0);	// Cap Buffer
			g_objCommon.Move_Position(AX_ASSY_PICKER_Y, 0);	// Cap Buffer
			g_objCommon.Move_Position(AX_VISION_CAP_ALIGN_Y, 0);	// Inspect
			m_niAssyPickCase++; m_tiAssyPickLoop.Set_LoopTime(10000);
		}
		break;
	case 14:	// Check Picker X, Y Ready Check
		if (g_objCommon.Check_Position(AX_ASSY_PICKER_X, 0) &&
			g_objCommon.Check_Position(AX_ASSY_PICKER_Y, 0) &&
			g_objCommon.Check_Position(AX_VISION_CAP_ALIGN_Y, 0)) {
			g_objLogFile.Save_HandlerLog("[Initial Sequence] - Assembly Picker Complete");
			m_niAssyPickCase = 90; m_tiAssyPickLoop.Set_LoopTime(5000);
		}
		break;

	case 90:	// Initial Complete
		return TRUE;

	}

	// 4. (Error : 1400)
	if (m_tiAssyPickLoop.Over_LoopTime()) {
		g_objCommon.Show_Error(1400 + m_niAssyPickCase);
		return FALSE;
	}
	return TRUE;
}

// /////////////////////////////////////////////////////////////////////////////
// 5. (Error : 1500)
BOOL CSequenceInit::Initial_CapPicker()
{
	switch (m_niCapPickCase) {
	case 0:		// Wait
		return TRUE;

	case 1:		// Cap Picker Initial Start
		g_objLogFile.Save_HandlerLog("[Initial Sequence] - Cap Picker Start");
		m_niCapPickCase++; m_tiCapPickLoop.Set_LoopTime(5000);
		break;
	case 2:		// Picker Vacuum Off
		g_objCommon.Set_CapPickerVacOff(0);	// All Off
		m_niCapPickCase++; m_tiCapPickLoop.Set_LoopTime(5000);
		break;
	case 3:		// Picker Vacuum Air Off
		if (g_objCommon.Get_CapPickerVacOff(0)) {
			if (!m_tiCapPickLoop.Waiting_Time(500)) break;
			g_objCommon.Set_CapPickerAirOff(0);	// All Off
			m_niCapPickCase++; m_tiCapPickLoop.Set_LoopTime(5000);
		}
		break;
	case 4:		// Picker Up
		if (g_objCommon.Get_CapPickerVacOff(0)) {
			g_objCommon.Set_CapPickerUp(0);	// All Up
			m_niCapPickCase++; m_tiCapPickLoop.Set_LoopTime(5000);
		}
		break;
	case 5:		// Picker Z Home
		if (g_objCommon.Get_CapPickerUp(0)) {
			g_objAJinAXL.Home_Search(AX_CAP_PICKER_Z);
			m_niCapPickCase++; m_tiCapPickLoop.Set_LoopTime(30000);
		}
		break;
	case 6:		// Picker Z Home Check
		if (g_objAJinAXL.Is_Home(AX_CAP_PICKER_Z)) {
			m_niCapPickCase = 11; m_tiCapPickLoop.Set_LoopTime(5000);
		}
		break;

	case 11:	// Picker Y, P Home
		if (g_objAJinAXL.Is_Home(AX_CAP_PICKER_Z)) {
			if (!m_tiCapPickLoop.Waiting_Time(500)) break;
			g_objAJinAXL.Home_Search(AX_CAP_PICKER_Y);
			g_objAJinAXL.Home_Search(AX_CAP_PICKER_P);
			m_niCapPickCase++; m_tiCapPickLoop.Set_LoopTime(50000);
		}
		break;
	case 12:	// Picker Z Move to Ready Up Position
		if (g_objAJinAXL.Is_Home(AX_CAP_PICKER_Y) && g_objAJinAXL.Is_Home(AX_CAP_PICKER_P)) {
			g_objCommon.Move_Position(AX_CAP_PICKER_Z, 0);	// Ready Up
			m_niCapPickCase++; m_tiCapPickLoop.Set_LoopTime(10000);
		}
		break;
	case 13:	// Picker Y, P Move to Ready Position
		if (g_objCommon.Check_Position(AX_CAP_PICKER_Z, 0)) {
			g_objCommon.Move_Position(AX_CAP_PICKER_Y, 0);	// Cap Stage1
			g_objCommon.Move_Position(AX_CAP_PICKER_P, 0);	// Cap Tray
			m_niCapPickCase++; m_tiCapPickLoop.Set_LoopTime(10000);
		}
		break;
	case 14:	// Picker Normal
		if (g_objCommon.Check_Position(AX_CAP_PICKER_Y, 0) && g_objCommon.Check_Position(AX_CAP_PICKER_P, 0)) {
			m_pDY07->oCapPickerTurn01 = FALSE;
			m_pDY07->oCapPickerTurn02 = FALSE;
			m_pDY07->oCapPickerTurn03 = FALSE;
			m_pDY07->oCapPickerTurn04 = FALSE;
			m_pDY07->oCapPickerTurn05 = FALSE;
			m_pDY07->oCapPickerTurn06 = FALSE;
			g_objAJinAXL.Write_Output(7);
			m_niCapPickCase++; m_tiCapPickLoop.Set_LoopTime(5000);
		}
		break;
	case 15:	// Check Picker X Ready Main Position
		if (g_objCommon.Check_Position(AX_CAP_PICKER_Y, 0) && g_objCommon.Check_Position(AX_CAP_PICKER_P, 0)) {
			g_objLogFile.Save_HandlerLog("[Initial Sequence] - Cap Picker Complete");
			m_niCapPickCase = 90; m_tiCapPickLoop.Set_LoopTime(5000);
		}
		break;

	case 90:	// Initial Complete
		return TRUE;
	}

	// 5. (Error : 1500)
	if (m_tiCapPickLoop.Over_LoopTime()) {
		g_objCommon.Show_Error(1500 + m_niCapPickCase);
		return FALSE;
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 6. (Error : 1600)
BOOL CSequenceInit::Initial_CapBuffer()
{
	switch (m_niCapBufferCase) {
	case 0:
		if (m_niCapPickCase > 10 && m_niAssyPickCase > 10) m_niCapBufferCase++;
		return TRUE;

	case 1:		// Cap Buffer Initial Start
		g_objLogFile.Save_HandlerLog("[Initial Sequence] - Cap Buffer Start");
		m_niCapBufferCase++; m_tiCapBufferLoop.Set_LoopTime(5000);
		break;
	case 2:		// Cap Buffer Cover Up
		m_pDY09->oCapCoverDown = FALSE;
		g_objAJinAXL.Write_Output(9);
		m_niCapBufferCase++; m_tiCapBufferLoop.Set_LoopTime(5000);
		break;
	case 3:		// Cap Buffer Cover Out
		if (1/*m_pDX09->iCapCoverUp && !m_pDX09->iCapCoverDown*/) {	//Temp 안달려있다.
			if (!m_tiCapBufferLoop.Waiting_Time(100)) break;
			m_pDY09->oCapCoverIn = FALSE;
			g_objAJinAXL.Write_Output(9);
			m_niCapBufferCase++; m_tiCapBufferLoop.Set_LoopTime(5000);
		}
		break;
	case 4:		// Cap Clean Air Off & Nozzle Down
		if (1/*!m_pDX09->iCapCoverIn && m_pDX09->iCapCoverOut*/) {		//Temp 안달려있다.
			if (!m_tiCapBufferLoop.Waiting_Time(100)) break;
			g_objCommon.Set_CapCleanAirOff();
			m_pDY09->oCapCleanUp = FALSE;
			g_objAJinAXL.Write_Output(9);
			m_niCapBufferCase++; m_tiCapBufferLoop.Set_LoopTime(5000);
		}
		break;
	case 5:		// Cap Buffer Align Out
		if (!m_pDX09->iCapCleanUp && m_pDX09->iCapCleanDown) {
			m_pDY09->oCapBufferAlignIn = FALSE;
			g_objAJinAXL.Write_Output(9);
			m_niCapBufferCase++; m_tiCapBufferLoop.Set_LoopTime(5000);
		}
		break;
	case 6:		// Cap Buffer Align Out Check
		if (!m_pDX09->iCapBufferStageAlignIn && m_pDX09->iCapBufferStageAlignOut) {
			m_niCapBufferCase = 11; m_tiCapBufferLoop.Set_LoopTime(5000);
		}
		break;

	case 11:	// Stage Y Home Search
		if (!m_pDX09->iCapBufferStageAlignIn && m_pDX09->iCapBufferStageAlignOut) {
			g_objAJinAXL.Home_Search(AX_CAP_BUFFER_STAGE_Y);
			m_niCapBufferCase++; m_tiCapBufferLoop.Set_LoopTime(20000);
		}
		break;
	case 12:	// Stage Z Move to Ready Down Position
		if (g_objAJinAXL.Is_Home(AX_CAP_BUFFER_STAGE_Y)) {
			if (!m_tiCapBufferLoop.Waiting_Time(500)) break;
			g_objCommon.Move_Position(AX_CAP_BUFFER_STAGE_Y, 0);	// Load
			m_niCapBufferCase++; m_tiCapBufferLoop.Set_LoopTime(10000);
		}
		break;
	case 13:	// Stage X Move to Load Position
		if (g_objCommon.Check_Position(AX_CAP_BUFFER_STAGE_Y, 0)) {
			g_objLogFile.Save_HandlerLog("[Initial Sequence] - Cap Buffer Complete");
			m_niCapBufferCase = 90; m_tiCapBufferLoop.Set_LoopTime(5000);
		}
		break;

	case 90:	// Initial Complete
		return TRUE;
	}

	// 6. (Error : 1600)
	if (m_tiCapBufferLoop.Over_LoopTime()) {
		g_objCommon.Show_Error(1600 + m_niCapBufferCase);
		return FALSE;
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 7. (Error : 1700)
BOOL CSequenceInit::Initial_CapStage()
{
	switch (m_niCapStageCase) {
	case 0:
		if (m_niCapPickCase > 10) m_niCapStageCase++;
		return TRUE;

	case 1:		// Cap Stage Initial Start
		g_objLogFile.Save_HandlerLog("[Initial Sequence] - Cap Stage Start");
		m_niCapStageCase++; m_tiCapStageLoop.Set_LoopTime(5000);
		break;

	case 2:		// Port1/2 Support In
		if (m_pDX02->iCapPort1SlideClose && !m_pDX02->iCapPort1SlideOpen &&
			m_pDX02->iCapPort2SlideClose && !m_pDX02->iCapPort2SlideOpen) {
			g_objCommon.Set_CapPortSupportIn(1);
			g_objCommon.Set_CapPortSupportIn(2);
			m_niCapStageCase++; m_tiCapStageLoop.Set_LoopTime(5000);
		}
		break;
	case 3:		// Stage1 Master/Slave Out
		m_pDY04->oCapStage1MasterIn = FALSE; m_pDY04->oCapStage1SlaveIn = FALSE;
		g_objAJinAXL.Write_Output(4);
		m_niCapStageCase++; m_tiCapStageLoop.Set_LoopTime(5000);
		break;
	case 4:		// Stage2 Master/Slave Out
		if (!m_pDX04->iCapStage1MasterIn && m_pDX04->iCapStage1MasterOut &&
			!m_pDX04->iCapStage1SlaveIn  && m_pDX04->iCapStage1SlaveOut) {
			m_pDY04->oCapStage2MasterIn = FALSE; m_pDY04->oCapStage2SlaveIn = FALSE;
			g_objAJinAXL.Write_Output(4);
			m_niCapStageCase++; m_tiCapStageLoop.Set_LoopTime(5000);
		}
		break;
	case 5:		// Master/Slave Check
		if (!m_pDX04->iCapStage2MasterIn && m_pDX04->iCapStage2MasterOut &&
			!m_pDX04->iCapStage2SlaveIn  && m_pDX04->iCapStage2SlaveOut) {
			m_niCapStageCase = 11; m_tiCapStageLoop.Set_LoopTime(5000);
		}
		break;

	case 11:	// Z1/Z2 Move Up or Down Position
		if (g_objAJinAXL.Get_Position(AX_CAP_STAGE1_Z) < g_objAJinAXL.Get_Position(AX_CAP_STAGE2_Z)) {
			g_objCommon.Move_Position(AX_CAP_STAGE1_Z, 0);	// Moving Down
			g_objCommon.Move_Position(AX_CAP_STAGE2_Z, 1);	// Moving Up
			m_niCapStageCase = 12; m_tiCapStageLoop.Set_LoopTime(10000);
		} else {
			g_objCommon.Move_Position(AX_CAP_STAGE2_Z, 0);	// Moving Down
			g_objCommon.Move_Position(AX_CAP_STAGE1_Z, 1);	// Moving Up
			m_niCapStageCase = 12; m_tiCapStageLoop.Set_LoopTime(10000);
		}
		break;
	case 12:	// Stage1,2 X Home Search
		if ((g_objCommon.Check_Position(AX_CAP_STAGE1_Z, 0) && g_objCommon.Check_Position(AX_CAP_STAGE2_Z, 1)) ||
			(g_objCommon.Check_Position(AX_CAP_STAGE1_Z, 1) && g_objCommon.Check_Position(AX_CAP_STAGE2_Z, 0))) {
			g_objAJinAXL.Home_Search(AX_CAP_STAGE1_X);
			g_objAJinAXL.Home_Search(AX_CAP_STAGE2_X);
			m_niCapStageCase = 13; m_tiCapStageLoop.Set_LoopTime(60000);
		}
		break;
	case 13:	// Stage1 X Load Position, Stage2 X Unload Position
		if (g_objAJinAXL.Is_Home(AX_CAP_STAGE1_X) && g_objAJinAXL.Is_Home(AX_CAP_STAGE2_X)) {
			m_tiCapStageLoop.Waiting_Time(500);
			g_objCommon.Move_Position(AX_CAP_STAGE1_X, 0);	// Load Position
			g_objCommon.Move_Position(AX_CAP_STAGE2_X, 2);	// Unload Position
			m_niCapStageCase = 14; m_tiCapStageLoop.Set_LoopTime(30000);
		}
		break;
	case 14:	// Stage1,2 Z Home Search
		if (g_objCommon.Check_Position(AX_CAP_STAGE1_X, 0) && g_objCommon.Check_Position(AX_CAP_STAGE2_X, 2)) {
			g_objAJinAXL.Home_Search(AX_CAP_STAGE1_Z);
			g_objAJinAXL.Home_Search(AX_CAP_STAGE2_Z);
			m_niCapStageCase = 15; m_tiCapStageLoop.Set_LoopTime(30000);
		}
		break;
	case 15:	// Stage1 Z Move Up, Stage2 Z Move Down
		if (g_objAJinAXL.Is_Home(AX_CAP_STAGE1_Z) && g_objAJinAXL.Is_Home(AX_CAP_STAGE2_Z)) {
			m_tiCapStageLoop.Waiting_Time(500);
			g_objCommon.Move_Position(AX_CAP_STAGE1_Z, 0);	// Down Position
			g_objCommon.Move_Position(AX_CAP_STAGE2_Z, 1);	// Up Position
			m_niCapStageCase = 16; m_tiCapStageLoop.Set_LoopTime(20000);
		}
		break;
	case 16:	// Port Slide Unlock
		if (g_objCommon.Check_Position(AX_CAP_STAGE1_Z, 0) && g_objCommon.Check_Position(AX_CAP_STAGE2_Z, 1)) {
			m_pDY02->oCapPort1SlideLock = FALSE; m_pDY02->oCapPort1SlideUnlock = TRUE;
			m_pDY02->oCapPort2SlideLock = FALSE; m_pDY02->oCapPort2SlideUnlock = TRUE;
			g_objAJinAXL.Write_Output(2);
			m_niCapStageCase = 17; m_tiCapStageLoop.Set_LoopTime(10000);
		}
		break;
	case 17:	// Port Slide Check
		if (!m_pDX02->iCapPort1SlideLock && m_pDX02->iCapPort1SlideUnlock &&
			!m_pDX02->iCapPort2SlideLock && m_pDX02->iCapPort2SlideUnlock) {
			g_objLogFile.Save_HandlerLog("[Initial Sequence] - Cap Stage Complete");
			m_niCapStageCase = 90; m_tiCapStageLoop.Set_LoopTime(5000);
		}
		break;

	case 90:	// Initial Complete
		return TRUE;
	}

	// 17. (Error : 1700)
	if (m_tiCapStageLoop.Over_LoopTime()) {
		g_objCommon.Show_Error(1700 + m_niCapStageCase);
		return FALSE;
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 8. (Error : 1800)
BOOL CSequenceInit::Initial_TransStage()
{
	switch (m_niTransStageCase) {
	case 0:		// Wait for Load Picker & Unload Picker
		if (m_niUnloadPickCase > 10) m_niTransStageCase++;
		return TRUE;

	case 1:		// Trans Stage Initial Start
		g_objLogFile.Save_HandlerLog("[Initial Sequence] - Trans Stage Start");
		m_niTransStageCase++; m_tiTransStageLoop.Set_LoopTime(5000);
		break;
	case 2:		// Stage Vac Off
		g_objCommon.Set_TransStageVacOff(0);
		m_niTransStageCase++; m_tiTransStageLoop.Set_LoopTime(10000);
		break;
	case 3:		// Stage Clamp Off
		if (g_objCommon.Get_TransStageVacOff(0)) {
			g_objCommon.Set_TransStageAirOff(0);
			g_objCommon.Set_TransStageClampOff();
			m_niTransStageCase++; m_tiTransStageLoop.Set_LoopTime(10000);
		}
		break;
	case 4:		// Stage Z Home Search
		if (g_objCommon.Get_TransStageClampOff()) {
			g_objAJinAXL.Home_Search(AX_TRANS_STAGE_Z);
			m_niTransStageCase++; m_tiTransStageLoop.Set_LoopTime(30000);
		}
		break;
 	case 5:		// Home Check
		if (g_objAJinAXL.Is_Home(AX_TRANS_STAGE_Z)) {
			m_niTransStageCase = 11; m_tiTransStageLoop.Set_LoopTime(5000);
		}
		break;

	case 11:	// Stage X/T Home Search
		if (g_objAJinAXL.Is_Home(AX_TRANS_STAGE_Z)) {
			if (!m_tiTransStageLoop.Waiting_Time(500)) break;
			g_objAJinAXL.Home_Search(AX_TRANS_STAGE_X);
			g_objAJinAXL.Home_Search(AX_TRANS_STAGE_T);
			m_niTransStageCase++; m_tiTransStageLoop.Set_LoopTime(50000);
		}
		break;
	case 12:	// Stage Z Ready Up Position
		if (g_objAJinAXL.Is_Home(AX_TRANS_STAGE_X) && g_objAJinAXL.Is_Home(AX_TRANS_STAGE_T)) {
			g_objCommon.Move_Position(AX_TRANS_STAGE_Z, 0);	// Ready Up
			m_niTransStageCase++; m_tiTransStageLoop.Set_LoopTime(10000);
		}
		break;
	case 13:	// Check Stage1/2/3 X Position
		if (g_objCommon.Check_Position(AX_TRANS_STAGE_Z, 0)) {
			g_objCommon.Move_Position(AX_TRANS_STAGE_X, 0);	// Index
			g_objCommon.Move_Position(AX_TRANS_STAGE_T, 0);	// Load
			m_niTransStageCase++; m_tiTransStageLoop.Set_LoopTime(20000);
		}
		break;
	case 14:
		if (g_objCommon.Check_Position(AX_TRANS_STAGE_X, 0) && g_objCommon.Check_Position(AX_TRANS_STAGE_T, 0)) {
			g_objLogFile.Save_HandlerLog("[Initial Sequence] - Inspect Stage Complete");
			m_niTransStageCase = 90; m_tiTransStageLoop.Set_LoopTime(5000);
		}
		break;

	case 90:	// Initial Complete
		return TRUE;
	}

	// 8. (Error : 1800)
	if (m_tiTransStageLoop.Over_LoopTime()) {
		g_objCommon.Show_Error(1800 + m_niTransStageCase);
		return FALSE;
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 9. (Error : 1900)
BOOL CSequenceInit::Initial_UnloadPicker()
{
	switch (m_niUnloadPickCase) {
	case 0:
		return TRUE;

	case 1:		// Unload Picker Initial Start
		g_objLogFile.Save_HandlerLog("[Initial Sequence] - Unload Picker Start");
		m_niUnloadPickCase++; m_tiUnloadPickLoop.Set_LoopTime(5000);
		break;
	case 2:		// Picker Vacuum Off
		g_objCommon.Set_UnloadPickerVacOff(0);	// All Off
		m_niUnloadPickCase++; m_tiUnloadPickLoop.Set_LoopTime(5000);
		break;
	case 3:		// Picker Vacuum Air Off
		if (g_objCommon.Get_UnloadPickerVacOff(0)) {
			if (!m_tiUnloadPickLoop.Waiting_Time(500)) break;
			g_objCommon.Set_UnloadPickerAirOff(0);	// All Off
			m_niUnloadPickCase++; m_tiUnloadPickLoop.Set_LoopTime(5000);
		}
		break;
	case 4:		// Picker Up
		if (g_objCommon.Get_UnloadPickerVacOff(0)) {
			g_objCommon.Set_UnloadPickerUp(0);	// All Up
			m_niUnloadPickCase++; m_tiUnloadPickLoop.Set_LoopTime(5000);
		}
		break;
	case 5:		// Picker Z Home
		if (g_objCommon.Get_UnloadPickerUp(0)) {
			g_objAJinAXL.Home_Search(AX_UNLOAD_PICKER_Z);
			m_niUnloadPickCase++; m_tiUnloadPickLoop.Set_LoopTime(20000);
		}
		break;
	case 6:		// Picker Z Home Check
		if (g_objAJinAXL.Is_Home(AX_UNLOAD_PICKER_Z)) {
			m_niUnloadPickCase = 11; m_tiUnloadPickLoop.Set_LoopTime(5000);
		}
		break;

	case 11:	// Picker X, P Home
		if (g_objAJinAXL.Is_Home(AX_UNLOAD_PICKER_Z)) {
			if (!m_tiUnloadPickLoop.Waiting_Time(500)) break;
			g_objAJinAXL.Home_Search(AX_UNLOAD_PICKER_X);
			g_objAJinAXL.Home_Search(AX_UNLOAD_PICKER_P);
			m_niUnloadPickCase++; m_tiUnloadPickLoop.Set_LoopTime(60000);
		}
		break;
	case 12:	// Picker Z Move to Ready Up Position
		if (g_objAJinAXL.Is_Home(AX_UNLOAD_PICKER_X) && g_objAJinAXL.Is_Home(AX_UNLOAD_PICKER_P)) {
			g_objCommon.Move_Position(AX_UNLOAD_PICKER_Z, 0);	// Ready Up
			m_niUnloadPickCase++; m_tiUnloadPickLoop.Set_LoopTime(10000);
		}
		break;
	case 13:	// Picker X, P Move to Ready Position
		if (g_objCommon.Check_Position(AX_UNLOAD_PICKER_Z, 0)) {
			g_objCommon.Move_Position(AX_UNLOAD_PICKER_X, 0);	// Trans Stage
			g_objCommon.Move_Position(AX_UNLOAD_PICKER_P, 0);	// Trans Stage
			m_niUnloadPickCase++; m_tiUnloadPickLoop.Set_LoopTime(10000);
		}
		break;
	case 14:	// Picker Normal
		if (g_objCommon.Check_Position(AX_UNLOAD_PICKER_X, 0) && g_objCommon.Check_Position(AX_UNLOAD_PICKER_P, 0)) {
			g_objLogFile.Save_HandlerLog("[Initial Sequence] - Unload Picker Complete");
			m_niUnloadPickCase = 90; m_tiUnloadPickLoop.Set_LoopTime(5000);
		}
		break;

	case 90:	// Initial Complete
		return TRUE;
	}

	// 9. (Error : 1900)
	if (m_tiUnloadPickLoop.Over_LoopTime()) {
		g_objCommon.Show_Error(1900 + m_niUnloadPickCase);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 10. (Error : 2000)
BOOL CSequenceInit::Initial_UnloadStage()
{
	switch (m_niUnloadStageCase) {
	case 0:		// Wait
		if (m_niUnloadPickCase > 10) m_niUnloadStageCase++;
		return TRUE;

	case 1:		// Unloading Initial Start
		g_objLogFile.Save_HandlerLog("[Initial Sequence] - Unload Stage Start");
		m_niUnloadStageCase++; m_tiUnloadStageLoop.Set_LoopTime(5000);
		break;

	case 2:		// Port1/2 Support In
		if (m_pDX03->iUnloadPort1SlideClose && !m_pDX03->iUnloadPort1SlideOpen &&
			m_pDX03->iUnloadPort2SlideClose && !m_pDX03->iUnloadPort2SlideOpen) {
			g_objCommon.Set_UnloadPortSupportIn(1);
			g_objCommon.Set_UnloadPortSupportIn(2);
			m_niUnloadStageCase++; m_tiUnloadStageLoop.Set_LoopTime(5000);
		}
		break;
	case 3:		// Stage1 Master/Slave Out
		m_pDY05->oUnloadStage1MasterIn = FALSE; m_pDY05->oUnloadStage1SlaveIn = FALSE;
		g_objAJinAXL.Write_Output(5);
		m_niUnloadStageCase++; m_tiUnloadStageLoop.Set_LoopTime(5000);
		break;
	case 4:		// Stage2 Master/Slave Out
		if (!m_pDX05->iUnloadStage1MasterIn && m_pDX05->iUnloadStage1MasterOut &&
			!m_pDX05->iUnloadStage1SlaveIn  && m_pDX05->iUnloadStage1SlaveOut) {
			m_pDY05->oUnloadStage2MasterIn = FALSE; m_pDY05->oUnloadStage2SlaveIn = FALSE;
			g_objAJinAXL.Write_Output(5);
			m_niUnloadStageCase++; m_tiUnloadStageLoop.Set_LoopTime(5000);
		}
		break;
	case 5:		// Master/Slave Check
		if (!m_pDX05->iUnloadStage2MasterIn && m_pDX05->iUnloadStage2MasterOut &&
			!m_pDX05->iUnloadStage2SlaveIn  && m_pDX05->iUnloadStage2SlaveOut) {
			m_niUnloadStageCase = 11; m_tiUnloadStageLoop.Set_LoopTime(5000);
		}
		break;

	case 11:	// Z1/Z2 Move Up or Down Position
		if (g_objAJinAXL.Get_Position(AX_UNLOAD_STAGE1_Z) < g_objAJinAXL.Get_Position(AX_UNLOAD_STAGE2_Z)) {
			g_objCommon.Move_Position(AX_UNLOAD_STAGE1_Z, 0);	// Moving Down
			g_objCommon.Move_Position(AX_UNLOAD_STAGE2_Z, 1);	// Moving Up
			m_niUnloadStageCase = 12; m_tiUnloadStageLoop.Set_LoopTime(10000);
		} else {
			g_objCommon.Move_Position(AX_UNLOAD_STAGE2_Z, 0);	// Moving Down
			g_objCommon.Move_Position(AX_UNLOAD_STAGE1_Z, 1);	// Moving Up
			m_niUnloadStageCase = 12; m_tiUnloadStageLoop.Set_LoopTime(10000);
		}
		break;
	case 12:	// Stage1,2 X Home Search
		if ((g_objCommon.Check_Position(AX_UNLOAD_STAGE1_Z, 0) && g_objCommon.Check_Position(AX_UNLOAD_STAGE2_Z, 1)) ||
			(g_objCommon.Check_Position(AX_UNLOAD_STAGE1_Z, 1) && g_objCommon.Check_Position(AX_UNLOAD_STAGE2_Z, 0))) {
			g_objAJinAXL.Home_Search(AX_UNLOAD_STAGE1_Y);
			g_objAJinAXL.Home_Search(AX_UNLOAD_STAGE2_Y);
			m_niUnloadStageCase = 13; m_tiUnloadStageLoop.Set_LoopTime(60000);
		}
		break;
	case 13:	// Stage1 X Load Position, Stage2 X Unload Position
		if (g_objAJinAXL.Is_Home(AX_UNLOAD_STAGE1_Y) && g_objAJinAXL.Is_Home(AX_UNLOAD_STAGE2_Y)) {
			m_tiUnloadStageLoop.Waiting_Time(500);
			g_objCommon.Move_Position(AX_UNLOAD_STAGE1_Y, 0);	// Load Position
			g_objCommon.Move_Position(AX_UNLOAD_STAGE2_Y, 2);	// Unload Position
			m_niUnloadStageCase = 14; m_tiUnloadStageLoop.Set_LoopTime(30000);
		}
		break;
	case 14:	// Stage1,2 Z Home Search
		if (g_objCommon.Check_Position(AX_UNLOAD_STAGE1_Y, 0) && g_objCommon.Check_Position(AX_UNLOAD_STAGE2_Y, 2)) {
			g_objAJinAXL.Home_Search(AX_UNLOAD_STAGE1_Z);
			g_objAJinAXL.Home_Search(AX_UNLOAD_STAGE2_Z);
			m_niUnloadStageCase = 15; m_tiUnloadStageLoop.Set_LoopTime(30000);
		}
		break;
	case 15:	// Stage1 Z Move Up, Stage2 Z Move Down
		if (g_objAJinAXL.Is_Home(AX_UNLOAD_STAGE1_Z) && g_objAJinAXL.Is_Home(AX_UNLOAD_STAGE2_Z)) {
			m_tiUnloadStageLoop.Waiting_Time(500);
			g_objCommon.Move_Position(AX_UNLOAD_STAGE1_Z, 1);	// Up Position
			g_objCommon.Move_Position(AX_UNLOAD_STAGE2_Z, 0);	// Down Position
			m_niUnloadStageCase = 16; m_tiUnloadStageLoop.Set_LoopTime(20000);
		}
		break;
	case 16:	// Port Slide Unlock
		if (g_objCommon.Check_Position(AX_UNLOAD_STAGE1_Z, 1) && g_objCommon.Check_Position(AX_UNLOAD_STAGE2_Z, 0)) {
			m_pDY03->oUnloadPort1SlideLock = FALSE; m_pDY03->oUnloadPort1SlideUnlock = TRUE;
			m_pDY03->oUnloadPort2SlideLock = FALSE; m_pDY03->oUnloadPort2SlideUnlock = TRUE;
			g_objAJinAXL.Write_Output(3);
			m_niUnloadStageCase = 17; m_tiUnloadStageLoop.Set_LoopTime(10000);
		}
		break;
	case 17:	// Port Slide Check
		if (!m_pDX03->iUnloadPort1SlideLock && m_pDX03->iUnloadPort1SlideUnlock &&
			!m_pDX03->iUnloadPort2SlideLock && m_pDX03->iUnloadPort2SlideUnlock) {
			g_objLogFile.Save_HandlerLog("[Initial Sequence] - Unload Stage Complete");
			m_niUnloadStageCase = 90; m_tiUnloadStageLoop.Set_LoopTime(5000);
		}
		break;

	case 90:	// Initial Complete
		return TRUE;
	}

	// 10. (Error : 2000)
	if (m_tiUnloadStageLoop.Over_LoopTime()) {
		g_objCommon.Show_Error(2000 + m_niUnloadStageCase);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
