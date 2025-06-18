// SequenceMain.cpp : 구현 파일
//
#include "stdafx.h"
#include "CME8000.h"
#include "CME8000Dlg.h"
#include "SequenceMain.h"
#include <math.h>

#include "LogFile.h"
#include "Common.h"
#include "Inspector.h"
#include "MESInterface.h"
#include "AviUDP.h"
#include "WorkDlg.h"
#include "LoadCell.h"

CSequenceMain g_objSequenceMain;

///////////////////////////////////////////////////////////////////////////////

CSequenceMain::CSequenceMain()
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

	m_strLog = "";

	m_bThreadMainRun = FALSE;
	m_pThreadMainRun = NULL;

	m_nCapPickMultiCnt = 0;
	m_nUnloadPickMultiCnt = 0;

	m_nUnloadLotCmCnt = 0;
	m_nUnloadLotIndex = 0;

	Reset_MainRunCase();
}

CSequenceMain::~CSequenceMain()
{
	if (m_pThreadMainRun) {
		m_bThreadMainRun = FALSE;
		WaitForSingleObject(m_pThreadMainRun->m_hThread, INFINITE);
	}
}

void CSequenceMain::Reset_MainRunCase()
{
	m_nTrayPickCase			= 0;		//  1. (Error : 3100)
	m_nLoadStage1Case		= 0;		//  2. (Error : 3200)
	m_nLoadStage2Case		= 50;		//  3. (Error : 3300)
	m_nLoadPickCase			= 0;		//  4. (Error : 3400)
	m_nMainIndexCase		= 0;		//  5. (Error : 3500)
	m_nVisCmAlignCase		= 0;		//  6. (Error : 3600)
	m_nVisCapAlignCase		= 0;		//  7. (Error : 3700)
	m_nCapStage1Case		= 0;		//  8. (Error : 3800)
	m_nCapStage2Case		= 50;		//  9. (Error : 3900)
	m_nCapPickCase			= 0;		// 10. (Error : 4000)
	m_nCapBufferCase		= 0;		// 11. (Error : 4100)
	m_nAssyPickCase			= 0;		// 12. (Error : 4200)
	m_nTransStageCase		= 0;		// 13. (Error : 4300)
	m_nUnloadPickCase		= 0;		// 14. (Error : 4400)
	m_nUnloadStage1Case		= 0;		// 15. (Error : 4500)	
	m_nUnloadStage2Case		= 50;		// 16. (Error : 4600) 
}

int *CSequenceMain::Get_pMainRunCase()
{
	static int nCase[16] = { 0 };

	nCase[0]  = m_nTrayPickCase;		//  1. (Error : 3100)
	nCase[1]  = m_nLoadStage1Case;		//  2. (Error : 3200)
	nCase[2]  = m_nLoadStage2Case;		//  3. (Error : 3300)
	nCase[3]  = m_nLoadPickCase;		//  4. (Error : 3400)
	nCase[4]  = m_nMainIndexCase;		//  5. (Error : 3500)
	nCase[5]  = m_nVisCmAlignCase;		//  6. (Error : 3600)
	nCase[6]  = m_nVisCapAlignCase;		//  7. (Error : 3700)
	nCase[7]  = m_nCapStage1Case;		//  8. (Error : 3800)
	nCase[8]  = m_nCapStage2Case;		//  9. (Error : 3900)
	nCase[9]  = m_nCapPickCase;			// 10. (Error : 4000)
	nCase[10] = m_nCapBufferCase;		// 11. (Error : 4100)
	nCase[11] = m_nAssyPickCase;		// 12. (Error : 4200)
	nCase[12] = m_nTransStageCase;		// 13. (Error : 4300)
	nCase[13] = m_nUnloadPickCase;		// 14. (Error : 4400)
	nCase[14] = m_nUnloadStage1Case;		// 15. (Error : 4500)
	nCase[15] = m_nUnloadStage2Case;		// 16. (Error : 4600)

	return nCase;
}

int CSequenceMain::Get_MainRunCase(int nRun)
{
	if		(nRun == AUTO_TRAY_PICKER)		return m_nTrayPickCase;			//  1. (Error : 3100)
	else if (nRun == AUTO_LOAD_STAGE1)		return m_nLoadStage1Case;		//  2. (Error : 3200)
	else if (nRun == AUTO_LOAD_STAGE2)		return m_nLoadStage2Case;		//  3. (Error : 3300)
	else if (nRun == AUTO_LOAD_PICKER)		return m_nLoadPickCase;			//  4. (Error : 3400)
	else if (nRun == AUTO_MAIN_INDEX)		return m_nMainIndexCase;		//  5. (Error : 3500)
	else if (nRun == AUTO_VISION_CM)		return m_nVisCmAlignCase;		//  6. (Error : 3600)
	else if (nRun == AUTO_VISION_CAP)		return m_nVisCapAlignCase;		//  7. (Error : 3700)
	else if (nRun == AUTO_CAP_STAGE1)		return m_nCapStage1Case;		//  8. (Error : 3800)
	else if (nRun == AUTO_CAP_STAGE2)		return m_nCapStage2Case;		//  9. (Error : 3900)
	else if (nRun == AUTO_CAP_PICKER)		return m_nCapPickCase;			// 10. (Error : 4000)
	else if (nRun == AUTO_CAP_BUFFER)		return m_nCapBufferCase;		// 11. (Error : 4100)
	else if (nRun == AUTO_ASSY_PICKER)		return m_nAssyPickCase;			// 12. (Error : 4200)
	else if (nRun == AUTO_TRANS_STAGE)		return m_nTransStageCase;		// 13. (Error : 4300)
	else if (nRun == AUTO_UNLOAD_PICKER)	return m_nUnloadPickCase;		// 14. (Error : 4400)
	else if (nRun == AUTO_UNLOAD_STAGE1)	return m_nUnloadStage1Case;		// 15. (Error : 4500)
	else if (nRun == AUTO_UNLOAD_STAGE2)	return m_nUnloadStage2Case;		// 16. (Error : 4600)

	return 0;
}

void CSequenceMain::Set_MainRunCase(int nRun, int nCase)
{
	if		(nRun == AUTO_TRAY_PICKER)		m_nTrayPickCase			= nCase;	//  1. (Error : 3100)
	else if (nRun == AUTO_LOAD_STAGE1)		m_nLoadStage1Case		= nCase;	//  2. (Error : 3200)
	else if (nRun == AUTO_LOAD_STAGE2)		m_nLoadStage2Case		= nCase;	//  3. (Error : 3300)
	else if (nRun == AUTO_LOAD_PICKER)		m_nLoadPickCase			= nCase;	//  4. (Error : 3400)
	else if (nRun == AUTO_MAIN_INDEX)		m_nMainIndexCase		= nCase;	//  5. (Error : 3500)
	else if (nRun == AUTO_VISION_CM)		m_nVisCmAlignCase		= nCase;	//  6. (Error : 3600)
	else if (nRun == AUTO_VISION_CAP)		m_nVisCapAlignCase		= nCase;	//  7. (Error : 3700)
	else if (nRun == AUTO_CAP_STAGE1)		m_nCapStage1Case		= nCase;	//  8. (Error : 3800)
	else if (nRun == AUTO_CAP_STAGE2)		m_nCapStage2Case		= nCase;	//  9. (Error : 3900)
	else if (nRun == AUTO_CAP_PICKER)		m_nCapPickCase			= nCase;	// 10. (Error : 4000)
	else if (nRun == AUTO_CAP_BUFFER)		m_nCapBufferCase		= nCase;	// 11. (Error : 4100)
	else if (nRun == AUTO_ASSY_PICKER)		m_nAssyPickCase			= nCase;	// 12. (Error : 4200)
	else if (nRun == AUTO_TRANS_STAGE)		m_nTransStageCase		= nCase;	// 13. (Error : 4300)
	else if (nRun == AUTO_UNLOAD_PICKER)	m_nUnloadPickCase		= nCase;	// 14. (Error : 4400)
	else if (nRun == AUTO_UNLOAD_STAGE1)	m_nUnloadStage1Case		= nCase;	// 15. (Error : 4500)
	else if (nRun == AUTO_UNLOAD_STAGE2)	m_nUnloadStage2Case		= nCase;	// 16. (Error : 4600)
}

void CSequenceMain::Set_MainRunLoop(int nRun, int nLoop)
{
	if		(nRun == AUTO_TRAY_PICKER)		m_tTrayPickLoop.Set_LoopTime(nLoop);	//  1. (Error : 3100)
	else if (nRun == AUTO_LOAD_STAGE1)		m_tLoadStage1Loop.Set_LoopTime(nLoop);	//  2. (Error : 3200)
	else if (nRun == AUTO_LOAD_STAGE2)		m_tLoadStage2Loop.Set_LoopTime(nLoop);	//  3. (Error : 3300)
	else if (nRun == AUTO_LOAD_PICKER)		m_tLoadPickLoop.Set_LoopTime(nLoop);	//  4. (Error : 3400)
	else if (nRun == AUTO_MAIN_INDEX)		m_tMainIndexLoop.Set_LoopTime(nLoop);	//  5. (Error : 3500)
	else if (nRun == AUTO_VISION_CM)		m_tVisCmAlignLoop.Set_LoopTime(nLoop);	//  6. (Error : 3600)
	else if (nRun == AUTO_VISION_CAP)		m_tVisCapAlignLoop.Set_LoopTime(nLoop);	//  7. (Error : 3700)
	else if (nRun == AUTO_CAP_STAGE1)		m_tCapStage1Loop.Set_LoopTime(nLoop);	//  8. (Error : 3800)
	else if (nRun == AUTO_CAP_STAGE2)		m_tCapStage2Loop.Set_LoopTime(nLoop);	//  9. (Error : 3900)
	else if (nRun == AUTO_CAP_PICKER)		m_tCapPickLoop.Set_LoopTime(nLoop);		// 10. (Error : 4000)
	else if (nRun == AUTO_CAP_BUFFER)		m_tCapBufferLoop.Set_LoopTime(nLoop);	// 11. (Error : 4100)
	else if (nRun == AUTO_ASSY_PICKER)		m_tAssyPickLoop.Set_LoopTime(nLoop);	// 12. (Error : 4200)
	else if (nRun == AUTO_TRANS_STAGE)		m_tTransStageLoop.Set_LoopTime(nLoop);	// 13. (Error : 4300)
	else if (nRun == AUTO_UNLOAD_PICKER)	m_tUnloadPickLoop.Set_LoopTime(nLoop);	// 14. (Error : 4400)
	else if (nRun == AUTO_UNLOAD_STAGE1)	m_tUnloadStage1Loop.Set_LoopTime(nLoop);	// 15. (Error : 4500)
	else if (nRun == AUTO_UNLOAD_STAGE2)	m_tUnloadStage2Loop.Set_LoopTime(nLoop);	// 16. (Error : 4600)
}

BOOL CSequenceMain::Get_IsAutoRun()
{
	int *pCase = Get_pMainRunCase();
	for (int i = 0; i < AUTO_COUNT; i++) {
		if (i == AUTO_TRAY_PICKER && m_nTrayPickCase == 1) continue;	// temp
		if (i == AUTO_LOAD_STAGE1 && m_nLoadStage1Case == 50) continue;
		if (i == AUTO_LOAD_STAGE2 && m_nLoadStage2Case == 50) continue;
		if (i == AUTO_CAP_STAGE1 && (m_nCapStage1Case == 16 || m_nCapStage1Case == 20  || m_nCapStage1Case == 50)) continue;
		if (i == AUTO_CAP_STAGE2 && (m_nCapStage2Case == 16 || m_nCapStage2Case == 20  || m_nCapStage2Case == 50)) continue;
		if (i == AUTO_CAP_PICKER && m_nCapPickCase == 10) continue;
		if (i == AUTO_CAP_BUFFER && m_nCapBufferCase == 10) continue;
		if (i == AUTO_ASSY_PICKER && m_nAssyPickCase == 1) continue;
		if (i == AUTO_UNLOAD_STAGE1 && (m_nUnloadStage1Case == 20 || m_nUnloadStage1Case == 50)) continue;
		if (i == AUTO_UNLOAD_STAGE2 && (m_nUnloadStage2Case == 20 || m_nUnloadStage2Case == 50)) continue;
		if (*(pCase + i) > 0) return TRUE;
	}

	// Temp
	if (!Check_LoadPickerEmpty()) return TRUE;
	if (!Check_IndexEmpty(-1)) return TRUE;
	if (!Check_TransStageEmpty()) return TRUE;
	if (!Check_UnloadPickerEmpty()) return TRUE;

	return FALSE;
}

void CSequenceMain::Begin_MainRunThread()
{
	Run_Replay();

	if (m_nTrayPickCase == 0)	m_nTrayPickCase = 1;

	if (m_nLoadStage1Case == 0)	m_nLoadStage1Case = 1;
	if (m_nLoadStage2Case == 0)	m_nLoadStage2Case = 1;

	if (m_nCapStage1Case == 0)	m_nCapStage1Case = 1;
	if (m_nCapStage2Case == 0)	m_nCapStage2Case = 1;

	if (m_nUnloadStage1Case == 0)	m_nUnloadStage1Case = 1;
	if (m_nUnloadStage2Case == 0)	m_nUnloadStage2Case = 1;

	g_objInspector.Set_StatusUpdate(1);
	g_objAviUDP.Set_StatusUpdate(1);
	
	if (m_pThreadMainRun) End_MainRunThread();
	m_bThreadMainRun = TRUE;
	m_pThreadMainRun = AfxBeginThread(Thread_MainRun, NULL);
}

void CSequenceMain::End_MainRunThread()
{
	if (m_pThreadMainRun) {
		m_bThreadMainRun = FALSE;
		WaitForSingleObject(m_pThreadMainRun->m_hThread, INFINITE);
	}
}

void CSequenceMain::Run_Replay()
{
	for (int i = 0; i < AUTO_COUNT; i++) Set_MainRunLoop(i, 5000);

	if (m_nLoadStage1Case == 17) m_tLoadStage1Loop.Set_LoopTime(30000);
	if (m_nLoadStage2Case == 17) m_tLoadStage2Loop.Set_LoopTime(30000);
	if (m_nVisCmAlignCase  == 10)  m_tVisCmAlignLoop.Set_LoopTime(30000);
	if (m_nVisCapAlignCase == 10)  m_tVisCapAlignLoop.Set_LoopTime(30000);
}

/////////////////////////////////////////////////////////////////////////////
// Main Thread Function 
UINT CSequenceMain::Thread_MainRun(LPVOID lpVoid)
{
	while (g_objSequenceMain.m_bThreadMainRun) 
	{

		if (!g_objSequenceMain.Check_Mode()) break;
		if (!g_objCommon.Check_MainEmgAir()) break;
		if (!g_objCommon.Check_MainDoor(TRUE)) break;
		//if (!g_objCommon.Check_PortArea(TRUE)) break;
		if (!g_objCommon.Check_TrayFull()) break;
		if (!g_objCommon.Check_ServoOn()) break;
		if (!g_objCommon.Check_DirveAlarm()) break;
		if (!g_objCommon.Check_EndLimit()) break;
		if (!g_objCommon.Check_HomeDone()) break;

		if (!g_objSequenceMain.TrayPicker_Run()) break;		//  1. (Error : 3100)
		if (!g_objSequenceMain.LoadStage1_Run()) break;		//  2. (Error : 3200)
		if (!g_objSequenceMain.LoadStage2_Run()) break;		//  3. (Error : 3300)
		if (!g_objSequenceMain.LoadPicker_Run()) break;		//  4. (Error : 3400)
		if (!g_objSequenceMain.MainIndex_Run()) break;		//  5. (Error : 3500)
		if (!g_objSequenceMain.VisionCM_Run()) break;		//  6. (Error : 3600)
		if (!g_objSequenceMain.VisionCap_Run()) break;		//  7. (Error : 3700)
		if (!g_objSequenceMain.CapStage1_Run()) break;		//  8. (Error : 3800)
		if (!g_objSequenceMain.CapStage2_Run()) break;		//  9. (Error : 3900)
		if (!g_objSequenceMain.CapPicker_Run()) break;		// 10. (Error : 4000)
		if (!g_objSequenceMain.CapBuffer_Run()) break;		// 11. (Error : 4100)
		if (!g_objSequenceMain.AssyPicker_Run()) break;		// 12. (Error : 4200)
		if (!g_objSequenceMain.TransStage_Run()) break;		// 13. (Error : 4300)
		if (!g_objSequenceMain.UnloadPicker_Run()) break;	// 14. (Error : 4400)
		if (!g_objSequenceMain.UnloadStage1_Run()) break;	// 15. (Error : 4500)
		if (!g_objSequenceMain.UnloadStage2_Run()) break;	// 16. (Error : 4600)

		if (g_objSequenceMain.LotEnd_Run()) break;

		if (gData.bLotEndBeep) {
			if (g_objSequenceMain.m_pThreadBeep == NULL) {
				g_objSequenceMain.m_pThreadBeep = AfxBeginThread(Thread_Beep, (LPVOID)(2000));
				gData.bLotEndBeep = FALSE;
			}
		}
		
		Sleep(5);
	}

	g_objInspector.Set_StatusUpdate(0);
	g_objAviUDP.Set_StatusUpdate(0);
	
	g_objSequenceMain.m_bThreadMainRun = FALSE;
	g_objSequenceMain.m_pThreadMainRun = NULL;

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// Picker Vacuum Thread Function

UINT CSequenceMain::Thread_Vacuum_Cap(LPVOID lpVoid)
{
	g_objSequenceMain.Vacuum_CapPicker((int)lpVoid);
	g_objSequenceMain.m_pThreadVacuumCap = NULL;
	return 0;
}

UINT CSequenceMain::Thread_Vacuum_Assy(LPVOID lpVoid)
{
	g_objSequenceMain.Vacuum_AssyPicker((int)lpVoid);
	g_objSequenceMain.m_pThreadVacuumAssy = NULL;
	return 0;
}

UINT CSequenceMain::Thread_Vacuum_Unload(LPVOID lpVoid)
{
	g_objSequenceMain.Vacuum_UnloadPicker((int)lpVoid);
	g_objSequenceMain.m_pThreadVacuumUnload = NULL;
	return 0;
}

UINT CSequenceMain::Thread_Vacuum_Cap_Multi(LPVOID lpVoid)
{
	g_objSequenceMain.Vacuum_CapPickerMulti((int)lpVoid);
	g_objSequenceMain.m_pThreadVacuumCapMulti = NULL;
	return 0;
}

UINT CSequenceMain::Thread_Vacuum_Unload_Multi(LPVOID lpVoid)
{
	g_objSequenceMain.Vacuum_UnloadPickerMulti((int)lpVoid);
	g_objSequenceMain.m_pThreadVacuumUnloadMulti = NULL;
	return 0;
}

UINT CSequenceMain::Thread_Beep(LPVOID lpVoid)
{
	g_objSequenceMain.Beep_Post(0, (int)lpVoid);
	g_objSequenceMain.m_pThreadBeep = NULL;
	return 0;
}


///////////////////////////////////////////////////////////////////////////////

void CSequenceMain::Vacuum_CapPicker(int nSub)
{
	int nCount = 0;
	while (TRUE) {
		g_objCommon.Set_CapPickerVacOff(nSub);
		Sleep(m_pEquipData->nVacOffDelay[0]);

		g_objCommon.Set_CapPickerAirOff(nSub);
		if (++nCount >= m_pEquipData->nVacOffRepeat[0]) return;
		Sleep(m_pEquipData->nVacOffDelay[0]);
	}
}

void CSequenceMain::Vacuum_AssyPicker(int nSub)
{
	int nCount = 0;
	while (TRUE) {
		g_objCommon.Set_AssyPickerVacOff(nSub);
		Sleep(m_pEquipData->nVacOffDelay[1]);

		g_objCommon.Set_AssyPickerAirOff(nSub);
		if (++nCount >= m_pEquipData->nVacOffRepeat[1]) return;
		Sleep(m_pEquipData->nVacOffDelay[1]);
	}
}

void CSequenceMain::Vacuum_UnloadPicker(int nSub)
{
	int nCount = 0;
	while (TRUE) {
		g_objCommon.Set_UnloadPickerVacOff(nSub);
		Sleep(m_pEquipData->nVacOffDelay[2]);

		g_objCommon.Set_UnloadPickerAirOff(nSub);
		if (++nCount >= m_pEquipData->nVacOffRepeat[2]) return;
		Sleep(m_pEquipData->nVacOffDelay[2]);
	}
}

void CSequenceMain::Vacuum_CapPickerMulti(int nSub)
{
	int nCount = 0;
	while (TRUE) {
		g_objCommon.Set_CapPickerVacOffMulti(nSub, m_nCapPickMultiCnt);
		Sleep(m_pEquipData->nVacOffDelay[0]);

		g_objCommon.Set_CapPickerAirOffMulti(nSub, m_nCapPickMultiCnt);
		if (++nCount >= m_pEquipData->nVacOffRepeat[0]) return;
		Sleep(m_pEquipData->nVacOffDelay[0]);
	}
}

void CSequenceMain::Vacuum_UnloadPickerMulti(int nSub)
{
	int nCount = 0;
	while (TRUE) {
		g_objCommon.Set_UnloadPickerVacOffMulti(nSub, m_nUnloadPickMultiCnt);
		Sleep(m_pEquipData->nVacOffDelay[2]);

		g_objCommon.Set_UnloadPickerAirOffMulti(nSub, m_nUnloadPickMultiCnt);
		if (++nCount >= m_pEquipData->nVacOffRepeat[2]) return;
		Sleep(m_pEquipData->nVacOffDelay[2]);
	}
}

///////////////////////////////////////////////////////////////////////////////

BOOL CSequenceMain::LotEnd_Run()
{
//	if (!m_bLoadLotEnd) return FALSE;
	if (!m_bUnloadLotEnd) return FALSE;

	if (Get_IsAutoRun()) return FALSE;

	int nPNo = gData.nULPNo - 1;	// 맨마지막 공정인 Good Tray Port No 활용.
	if (nPNo < 0) nPNo = 0;
//	Job_LotEnd(gData.nULPNo);
// 	Beep_Post(1000);
	if (gData.bCycleStop) g_dlgWork.PostMessage(UM_RESET_CYCLE_STOP, NULL, NULL);

	gMes.nLotStatus = 0;
	gData.bFirstLotStart = FALSE;

	gData.bLoadPort3Wait = FALSE;
	gData.bCapPort1Wait = FALSE;
	gData.bCapPort2Wait = FALSE;
	gData.bUnloadPort1Wait = FALSE;
	gData.bUnloadPort2Wait = FALSE;
	gData.bLoadLampOn[0] = FALSE;
	gData.bLoadLampOn[1] = FALSE;

	CCME8000Dlg *pMainDlg = (CCME8000Dlg*)AfxGetApp()->GetMainWnd();
	pMainDlg->Set_LampFlicker_Load1(FALSE);
	pMainDlg->Set_LampFlicker_Load2(FALSE);
	pMainDlg->Set_LampFlicker_Load3(FALSE);
	pMainDlg->Set_LampFlicker_Cap1(FALSE);
	pMainDlg->Set_LampFlicker_Cap2(FALSE);
	pMainDlg->Set_LampFlicker_Unload1(FALSE);
	pMainDlg->Set_LampFlicker_Unload2(FALSE);

	CString strMsg;

// 	int nSum = gLot.nGoodCount[nPNo] + gLot.nNgCount[nPNo];
// 	double dRate = (nSum == 0 ? 0.0 : gLot.nGoodCount[nPNo] * 100.0 / nSum);
	strMsg.Format("Lot End ==> ID(%s)", gLot.sLotID[nPNo]);
	g_objLogFile.Save_HandlerLog(strMsg);

	strMsg.Format("Lot End.\n\n");

	gData.nPNoUnloadPort = 0;
	if (m_nAssyPickCase == 1) m_nAssyPickCase = 0;
	Set_ClearRunData(1);

	g_dlgWork.m_stcLotIdR.SetWindowText("");
	g_dlgWork.m_stcCmCountR.SetWindowText("0");
	g_dlgWork.m_stcTrayCountR.SetWindowText("0");
	
	g_dlgWork.m_stcLotIdS.SetWindowText("");
	g_dlgWork.m_stcCmCountS.SetWindowText("0");
	g_dlgWork.m_stcTrayCountS.SetWindowText("0");
	
	g_objCommon.Show_Alarm(strMsg, STATE_LOTEND);

	m_pEquipData->bResultTestUse = FALSE;	// LOT 끝나면 Reset

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////

void CSequenceMain::Set_ClearRunData(int nType)
{
	memset(gData.bScanDone, 0x00, sizeof(BOOL) * 2);
	memset(gData.nCmInspectInfo, 0x00, sizeof(int) * 2 * 30 * 40);
	memset(gData.nCapInspectInfo, 0x00, sizeof(int) * 2 * 100 * 40);

	memset(gData.IndexDone, 0x00, sizeof(int) * 3);
	memset(gData.InfoLoadTray, 0x00, sizeof(int) * 20 * 20);
//	memset(gData.InfoUnloadTray, 0x00, sizeof(int) * 20 * 20);
	Init_UnloadTray();

	if (nType == 0) { memset(gData.InfoCapTray, 0x00, sizeof(int) * 20 * 20); }
	g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 0, NULL);

	memset(gData.InfoIndex, 0x00, sizeof(int) * 3 * 6);
	memset(gData.InfoTransStage, 0x00, sizeof(int) * 6);
	if (nType == 0) { memset(gData.InfoCapBuffer, 0x00, sizeof(int) * 6); }
	
	memset(gData.InfoLoadPick, 0x00, sizeof(int) * 6);
	memset(gData.InfoUnloadPick, 0x00, sizeof(int) * 6);
	if (nType == 0) {
		memset(gData.InfoCapPick, 0x00, sizeof(int) * 6);
		memset(gData.InfoAssyPick, 0x00, sizeof(int) * 6);
	}

	if (nType == 0) {
		memset(gData.nCapTNoCapPicker, 0x00, sizeof(int) * 6);
		memset(gData.nCapTNoCapBuffer, 0x00, sizeof(int) * 6);
		memset(gData.nCapTNoAssyPicker, 0x00, sizeof(int) * 6);

		memset(gData.nCapNoCapPicker, 0x00, sizeof(int) * 6);
		memset(gData.nCapNoCapBuffer, 0x00, sizeof(int) * 6);
		memset(gData.nCapNoAssyPicker, 0x00, sizeof(int) * 6);
	}

	gData.nPNoLoadPick = gData.nPNoUnloadPick = 0;
	gData.nPNoTransStage = gData.nPNoUnloadTray = 0;
	gData.nPNoUnloadPort = 0;
	gData.nLPNo = gData.nULPNo = 0;
	memset(gData.nPNoLoadTray, 0x00, sizeof(int) * 2);
	memset(gData.nPNoIndex, 0x00, sizeof(int) * 3 );

	gData.nTNoTrayPick = gData.nTNoLoadPort = gData.nTNoUnloadTray = 0;
	memset(gData.nTNoLoadTray, 0x00, sizeof(int) * 2);
	memset(gData.nTNoLoadPick, 0x00, sizeof(int) * 6);
	memset(gData.nTNoIndex, 0x00, sizeof(int) * 3 * 6);
	memset(gData.nTNoTransStage, 0x00, sizeof(int) * 6);
	memset(gData.nTNoUnloadPick, 0x00, sizeof(int) * 6);

	memset(gData.nCNoLoadPick, 0x00, sizeof(int) * 6);
	memset(gData.nCNoIndex, 0x00, sizeof(int) * 3 * 6);
	memset(gData.nCNoTransStage, 0x00, sizeof(int) * 6);
	memset(gData.nCNoUnloadPick, 0x00, sizeof(int) * 6);

	memset(gLot.dAverageCycle, 0x00, sizeof(double) * 28 * 2 * 14);

	// Process 변수 초기화
	m_bLoadLotEnd = FALSE;
	m_bUnloadLotEnd = FALSE;

	//AHN
	m_dwFirstLoad = 0;
	m_dwLastUnLoad = 0;
	m_dwULCycleTime = 0;
	gLot.nErrorCount = 0;
	gLot.dwRunTime = gLot.dwErrorTime = gLot.dwStopTime = 0;
	gLot.bLotEndComplete[0] = FALSE;
	gLot.bLotEndComplete[1] = FALSE;
	gLot.nCapFailCount[0] = gLot.nCapFailCount[1] = 0;
	gData.bFirstLotStart = FALSE;

	gData.nLoadTrayCount[0] = 0; gData.nLoadTrayCount[1] = 0;
	gData.nUnloadTrayCount = 0;

	gData.nTrayUseCount[0] = gData.nTrayUseCount[1] = 0;
	gData.nCmUseCount[0] = gData.nCmUseCount[1] = 0;

	gData.bAviTrayLoad = FALSE;
	gData.bAviLotEnd[0] = gData.bAviLotEnd[1] = FALSE;
	gData.nLastTrayNo[0] = gData.nLastTrayNo[1] = 0;

	gData.bContinueLotEnd = FALSE;
	gData.bLoadPort3Wait = FALSE;
	gData.bCapPort1Wait = FALSE;
	gData.bCapPort2Wait = FALSE;
	gData.bUnloadPort1Wait = FALSE;
	gData.bUnloadPort2Wait = FALSE;
	gData.bUnloadTrayLotEnd[0] = FALSE;
	gData.bUnloadTrayLotEnd[1] = FALSE;

	gData.bLoadLampOn[0] = FALSE;
	gData.bLoadLampOn[1] = FALSE;

	m_nUnloadLotIndex = 0;
	m_nUnloadLotCmCnt = 0;

	m_bLoadPortTrayExist = FALSE;

	if (nType == 0) {
		gData.bTempLot[0] = FALSE;
		gData.bTempLot[1] = FALSE;
	}
	
	// Align Error 변수 초기화
	memset(gData.bCmAlignErr, 0x00, sizeof(BOOL) * 6);
	memset(gData.bCapAlignErr, 0x00, sizeof(BOOL) * 6);
	
	for (int p = 0; p < 2; p++) for (int i = 0; i < 6; i++) gLot.nSNgCount[p][i] = 0;	// Special NG
	gLot.nGoodCount[0] = gLot.nGoodCount[1] = gLot.nNgCount[0] = gLot.nNgCount[1] = 0;
	for (int p = 0; p < 2; p++) { gLot.nRosRequest[p] = gLot.nRosGood[p] = gLot.nRosNg[p] = gLot.nRosRepair[p] = gLot.nRosTimeOut[p] = 0; }
	g_dlgWork.PostMessage(UM_VISION_RESULT, NULL, NULL);

	for (int p = 0; p <  2; p++) for (int i = 0; i < 50; i++) for (int j = 0; j < 40; j++) gMes.sBarID[p][i][j] = gMes.sJudge[p][i][j] = gMes.sNGCode[p][i][j] = "";
	for (int p = 0; p <  2; p++) for (int i = 0; i < 100; i++) for (int j = 0; j < 40; j++) for (int k = 0; k < 2; k++) gData.sNGData[p][i][j][k] = "";
	memset(gMes.nCmResult, 0x00, sizeof(int) * 50 * 40);	// Result Clear

	for (int i = 0; i < 4; i++) { 
		for (int j = 0; j < 6; j++) {
			gData.dIndexOffsetX[i][j] = gData.dIndexOffsetY[i][j] = 0.0;
			gData.dIndexCmSizeX[i][j] = gData.dIndexCmSizeY[i][j] = 0.0;
		}
	}
	for (int i = 0; i < 6; i++) { 
		gData.dAssyPickOffsetX[i] = gData.dAssyPickOffsetY[i] = 0.0;
		gData.dAssyPickCapSizeX[i] = gData.dAssyPickCapSizeY[i] = 0.0;
		gData.dAlignDiffX[i] = gData.dAlignDiffY[i] = 0.0;
	}

	m_bLotLoadEnable[0] = FALSE;
	m_bLotLoadEnable[1] = FALSE;

	g_dlgWork.Enable_UserInput(1, TRUE);
	g_dlgWork.Enable_UserInput(2, TRUE);

	gData.bMesContinueLot = FALSE;

	CCME8000Dlg *pMainDlg = (CCME8000Dlg*)AfxGetApp()->GetMainWnd();
	pMainDlg->Set_LampFlicker_Load1(FALSE);
	pMainDlg->Set_LampFlicker_Load2(FALSE);
	pMainDlg->Set_LampFlicker_Load3(FALSE);
	pMainDlg->Set_LampFlicker_Cap1(FALSE);
	pMainDlg->Set_LampFlicker_Cap2(FALSE);
	pMainDlg->Set_LampFlicker_Unload1(FALSE);
	pMainDlg->Set_LampFlicker_Unload2(FALSE);

	if (nType == 0) m_pEquipData->bResultTestUse = FALSE;	// LOT 끝나면 Reset

	g_dlgWork.PostMessage(UM_UPDATE_MODEL, NULL, NULL);
}

///////////////////////////////////////////////////////////////////////////////
void CSequenceMain::Init_LoadTray(int nPNo, int nSNo)
{
	int nNo = nPNo-1;
	if (nNo < 0) nNo = 0;

	memset(gData.InfoLoadTray, 0x00, sizeof(int) * 20 * 20);	// Empty

	int nCmMax = gData.nCmMaxCount;
	int nTrayCnt = gData.nTNoLoadPort;
	int nTrayUse = gData.nTrayUseCount[nNo];
	int nCmUse   = gData.nCmUseCount[nNo];
	int nCmCnt	 = gData.nCmCntLoadStage[nSNo-1];

	if (!m_pEquipData->bUseInlineMode){
		nTrayCnt = gData.nLoadTrayCount[nNo];
		nCmCnt = (nTrayCnt < nTrayUse ? nCmMax : (nCmUse - (nTrayCnt - 1) * nCmMax));
	}	

	int nTrayX = m_pEquipData->nLoadTrayX;
	int nTrayY = m_pEquipData->nLoadTrayY;

	if (m_pEquipData->bSortDirRight) {
		for (int i = 0; i < nCmCnt; i++) gData.InfoLoadTray[i / nTrayX][i % nTrayX] = 9;
	} else {
		for (int i = 0; i < nCmCnt; i++) gData.InfoLoadTray[i % nTrayY][i / nTrayY] = 9;
	}

	g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 1, nSNo-1);
}

void CSequenceMain::Init_CapTray()
{
	int nTrayX = m_pEquipData->nCapTrayX;
	int nTrayY = m_pEquipData->nCapTrayY;

	for (int y = 0; y < nTrayY; y++) {
		for (int x = 0; x < nTrayX; x++) {
			gData.InfoCapTray[y][x] = 9;		
		}
	}

	g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 2, NULL);
}

void CSequenceMain::Init_UnloadTray()
{
	memset(gData.InfoUnloadTray, 0x00, sizeof(int) * 20 * 20);	// Empty
	if (m_pEquipData->bShipTrayOneEmpty) {
		int x = m_pEquipData->nUnloadTrayX - 1;
		int y = m_pEquipData->nUnloadTrayY - 1;
//		gData.InfoUnloadTray[y][x] = 2;	//우하(21)
		gData.InfoUnloadTray[0][x] = 2;	//우상(3)
	}
	// 테스트 (시간 단축하기 위해 10줄 채운 상태로 진행)
	//for (int i = 0; i < 10; i++) for (int j = 0; j < 3; j++) gData.InfoGoodTray[i][j] = 1;
	g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 3, NULL);
}

///////////////////////////////////////////////////////////////////////////////
BOOL CSequenceMain::Check_AviTrayPickup()
{
	if (gData.bAviTrayLoad) return TRUE;
	return FALSE;
}

BOOL CSequenceMain::Check_LoadTrayEmpty(int nNo)
{
	int nSNo = 0; int nENo = 0;
	if (nNo == 0) { nSNo = 0; nENo = 2; }
	else		  { nSNo = nNo-1; nENo = nNo; }

	int nTrayX = m_pEquipData->nLoadTrayX;
	int nTrayY = m_pEquipData->nLoadTrayY;

//	for (int s = nSNo; s < nENo; s++) {
		for (int y = 0; y < nTrayY; y++) {
			for (int x = 0; x < nTrayX; x++) {
				if (gData.InfoLoadTray[y][x] > 0) return FALSE;
			}
		}
//	}
	return TRUE;
}

BOOL CSequenceMain::Check_LoadTrayLineEmpty(int nLine)
{
	int nTrayX = m_pEquipData->nLoadTrayX;
	for (int x = 0; x < nTrayX; x++) {
		if (gData.InfoLoadTray[nLine-1][x] > 0) return FALSE;
	}
	return TRUE;
}

BOOL CSequenceMain::Check_CapTrayEmpty()
{
	int nTrayX = m_pEquipData->nCapTrayX;
	int nTrayY = m_pEquipData->nCapTrayY;

	for (int y = 0; y < nTrayY; y++) {
		for (int x = 0; x < nTrayX; x++) {
			if (gData.InfoCapTray[y][x] > 0) return FALSE;
		}
	}
	return TRUE;
}

BOOL CSequenceMain::Check_CapTrayLineEmpty(int nLine)
{
	int nTrayX = m_pEquipData->nCapTrayX;
	for (int x = 0; x < nTrayX; x++) {
		if (gData.InfoCapTray[nLine-1][x] > 0) return FALSE;
	}
	return TRUE;
}

BOOL CSequenceMain::Check_UnloadTrayEmpty()
{
	int nTrayX = m_pEquipData->nUnloadTrayX;
	int nTrayY = m_pEquipData->nUnloadTrayY;

	for (int y = 0; y < nTrayY; y++) {
		for (int x = 0; x < nTrayX; x++) {
			if (gData.InfoUnloadTray[y][x] > 0) return FALSE;
		}
	}
	return TRUE;
}

BOOL CSequenceMain::Check_LoadPickerEmpty()
{
	for (int i = 0; i < 6; i++) { 
		if (gData.InfoLoadPick[i] > 0) return FALSE; 
	}
	return TRUE;
}

BOOL CSequenceMain::Check_CapPickerEmpty()
{
	for (int i = 0; i < 6; i++) { 
		if (gData.InfoCapPick[i] > 0) return FALSE; 
	}
	return TRUE;
}

BOOL CSequenceMain::Check_AssyPickerEmpty()
{
	for (int i = 0; i < 6; i++) { 
		if (gData.InfoAssyPick[i] > 0) return FALSE; 
	}
	return TRUE;
}

BOOL CSequenceMain::Check_UnloadPickerEmpty()
{
	for (int i = 0; i < 6; i++) { 
		if (gData.InfoUnloadPick[i] > 0) return FALSE; 
	}
	return TRUE;
}


BOOL CSequenceMain::Check_CapBufferEmpty()
{
	for (int i = 0; i < 6; i++) {
		if (gData.InfoCapBuffer[i] > 0) return FALSE;
	}
	return TRUE;
}

BOOL CSequenceMain::Check_TransStageEmpty()
{
	for (int i = 0; i < 6; i++) {
		if (gData.InfoTransStage[i] > 0) return FALSE;
	}
	return TRUE;
}

BOOL CSequenceMain::Check_LoadPickerFull()
{
	for (int i = 0; i < gData.nPickerUseCnt; i++) { 
		if (gData.InfoLoadPick[i] == 0) return FALSE; 
	}
	return TRUE;
}

BOOL CSequenceMain::Check_CapPickerFull()
{
	for (int i = 0; i < gData.nPickerUseCnt; i++) { 
		if (gData.InfoCapPick[i] == 0) return FALSE; 
	}
	return TRUE;
}

BOOL CSequenceMain::Check_CapBufferFull()
{
	int nTrayX = gData.nPickerUseCnt;
	if (gData.InfoCapBuffer[nTrayX - 1] > 0) return TRUE;
	
	return FALSE;
}

BOOL CSequenceMain::Check_UnloadTrayFull()
{
	int nTrayY = m_pEquipData->nUnloadTrayY;
	int nTrayX = m_pEquipData->nUnloadTrayX;
//	if (gData.InfoUnloadTray[nTrayY - 1][nTrayX - 1] > 0) return TRUE;

	for(int y = 0; y < nTrayY; y++) {
		for (int x = 0; x < nTrayX; x++) {
			if (gData.InfoUnloadTray[y][x] == 0) return FALSE;
		}
	}
	return TRUE;
}

BOOL CSequenceMain::Check_IndexEmpty(int nPos)
{
	// nPos (0:Load, 1:Assembly, 2:Unload, -1:All)
	int nNo = 6;

	int nS = (nPos == -1 ? 0 : nPos);
	int nE = (nPos == -1 ? 2 : nPos);

	for (int i = nS; i < nE + 1; i++) {
		for (int j = 0; j < nNo; j++) {
			if (gData.InfoIndex[i][j] > 0) return FALSE;
		}
	}
	return TRUE;
}

BOOL CSequenceMain::Check_IndexModule()
{
	// nPos (0:Load, 1:Assembly, 2:Unload, -1:All)
	int nNo = 6;

	for (int j = 0; j < nNo; j++) {
		if (gData.InfoIndex[0][j] > 0) return TRUE;	// Use : 9
		if (gData.InfoIndex[1][j] > 1) return TRUE; // 조립전 : 9, 조립후 : 1
	}
	return FALSE;
}

BOOL CSequenceMain::Check_PickerEmpty(int nPos)
{
	if (nPos == 0) {		// Lot End
// 		for(int i = 0; i < 3; i++) if (gData.InfoLoadPick[i] > 0) return FALSE;
// 		for(int i = 0; i < 3; i++) if (gData.InfoNgPick[i] > 0) return FALSE;
// 		for(int i = 0; i < 3; i++) if (gData.InfoGoodPick[i] > 0) return FALSE;
// 		for(int i = 0; i < 3; i++) if (gData.InfoTransPick[i] > 0) return FALSE;
// 		//for(int i = 0; i < 3; i++) if (gData.InfoAssemPick[i] > 0) return FALSE;	// 가지고 있는 경우 있음
// 		//for(int i = 0; i < 3; i++) if (gData.InfoCapPick[i] > 0) return FALSE;	// 가지고 있는 경우 있음
// 		for(int i = 0; i < 3; i++) if (gData.InfoShipPick[i] > 0) return FALSE;

	} else if (nPos == 1) {		// Index Run
		for(int i = 0; i < 6; i++) if (gData.InfoLoadPick[i] > 0) return FALSE;

	} else if (nPos == 2) {		// Assembly End
// 		for(int i = 0; i < 3; i++) if (gData.InfoLoadPick[i] > 0) return FALSE;
// 		for(int i = 0; i < 3; i++) if (gData.InfoNgPick[i] > 0) return FALSE;
// 		for(int i = 0; i < 3; i++) if (gData.InfoGoodPick[i] > 0) return FALSE;
// 		for(int i = 0; i < 3; i++) if (gData.InfoTransPick[i] > 0) return FALSE;
	}

	return TRUE;
}

BOOL CSequenceMain::Check_CapInspAllGood()
{
// 	for (int i = 0; i < gData.nPickerUseCnt; i++) { 
// 		if (gData.InfoAssyPick[i] == 2) return FALSE; 
// 	}
	// CM/Cap Vision, Align Offset 중 하나라도 꺼져있으면 Offset 기능 사용하면 안된다.
	if (!m_pEquipData->bUseAlignOffset || !m_pEquipData->bUseVisionCmAlign || !m_pEquipData->bUseVisionCapAlign) {
		for (int i = 0; i < gData.nPickerUseCnt; i++) { 
			gData.dAlignDiffX[i] = gData.dAlignDiffY[i] = 0.0;
			gData.dAssyPickOffsetX[i] = gData.dAssyPickOffsetY[i] = 0.0;
			gData.bCapAlignNg[i] = FALSE;
		}
		for (int i = 0; i < 4; i++) { for (int j = 0; j < 6; j++) { gData.dIndexOffsetX[i][j] = gData.dIndexOffsetY[i][j] = 0.0; } }
		return TRUE;
	}

	// 모듈 이미지를 180도 회전해야 모듈이 제대로 보이고 인덱스 회전까지 합하면 총 270도
	// 캡 이미지를 -90도 회전해야 캡이 제대로 보인다.
	int nIndexNo = g_objCommon.Get_IndexPos(2);
	double dDiff_X = 0.0;
	double dDiff_Y = 0.0;
	double dTolerance = m_pEquipData->dAlignOffset;
	for (int i = 0; i < gData.nPickerUseCnt; i++) { 
		dDiff_Y = gData.dIndexOffsetX[nIndexNo][i] - gData.dAssyPickOffsetX[i];
		dDiff_X = gData.dIndexOffsetY[nIndexNo][i] + gData.dAssyPickOffsetY[i];
		if ((gData.InfoIndex[1][i] > 0) && ((fabs(dDiff_X) > dTolerance) || (fabs(dDiff_Y) > dTolerance)))
		{
			if (fabs(dDiff_X) > dTolerance) gData.dAlignDiffX[i] = dDiff_X;
			else							gData.dAlignDiffX[i] = 0.0;
			if (fabs(dDiff_Y) > dTolerance) gData.dAlignDiffY[i] = dDiff_Y;
			else							gData.dAlignDiffY[i] = 0.0;
			gData.InfoAssyPick[i] = 2;
			gData.bCapAlignNg[i] = TRUE;
		} else {
// 			if (fabs(dDiff_X) < dTolerance) gData.dAlignDiffX[i] = 0.0;
// 			if (fabs(dDiff_Y) < dTolerance) gData.dAlignDiffY[i] = 0.0;
			gData.dAlignDiffX[i] = 0.0;
			gData.dAlignDiffY[i] = 0.0;
			gData.bCapAlignNg[i] = FALSE;
		}
	}

	for (int i = 0; i < gData.nPickerUseCnt; i++) { 
		if (gData.InfoAssyPick[i] == 2) return FALSE; 
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
int CSequenceMain::Get_CmScanCnt()
{
	int nCnt = 0;
	int nScanCnt = -1;

	for (int i=0; i<gData.nPickerUseCnt; i++) {
		if (gData.InfoIndex[0][i] > 0) nCnt++;
	}

	if (nCnt >= 3)		nScanCnt = 2;
	else if (nCnt >= 2)	nScanCnt = 1;
	else if (nCnt >= 1)	nScanCnt = 0;

	return nScanCnt;
}

int CSequenceMain::Get_CapScanCnt()
{
	int nCnt = 0;
	int nScanCnt = -1;

	for (int i=0; i<gData.nPickerUseCnt; i++) {
		if (gData.InfoAssyPick[i] > 0) nCnt++;
	}

	if (nCnt >= 3)		nScanCnt = 2;
	else if (nCnt >= 2)	nScanCnt = 1;
	else if (nCnt >= 1)	nScanCnt = 0;

	return nScanCnt;
}
 
// BOOL CSequenceMain::Select_BtmScanPos(int nPos, int nScanCnt)
// {
// 	int *pInfo = NULL;
// 
// 	if (pInfo == NULL) return FALSE;
// 
// 	if (gData.bR04C) {	// Btm Picker 4개만 사용.
// 		if (nScanCnt == 0) {
// 			if( pInfo[2] == 9 || pInfo[3] == 9 ||
// 				pInfo[7] == 9 || pInfo[8] == 9) { return TRUE; }
// 		}
// 		if (nScanCnt == 1) {
// 			if( pInfo[0] == 9 || pInfo[1] == 9 ||
// 				pInfo[5] == 9 || pInfo[6] == 9) { return TRUE; }
// 		}
// 
// 	} else {
// 		if (nScanCnt == 0) {
// 			if( pInfo[3] == 9 || pInfo[4] == 9 ||
// 				pInfo[8] == 9 || pInfo[9] == 9) { return TRUE; }
// 		}
// 		if (nScanCnt == 1) {
// 			if( pInfo[1] == 9 || pInfo[2] == 9 ||
// 				pInfo[6] == 9 || pInfo[7] == 9) { return TRUE; }
// 		}
// 		if (nScanCnt == 2) {
// 			if( pInfo[0] == 9 || pInfo[5] == 9) { return TRUE; }
// 		}
// 	}
// 	return FALSE;
// }
// 
// BOOL CSequenceMain::Select_Top1ScanPos(int nPos, int nScanCnt)
// {
// 	int *pInfo = NULL;
// // 	if (nPos == AUTO_INSPECTION1) pInfo = gData.InfoInspect[0];
// // 	if (nPos == AUTO_INSPECTION2) pInfo = gData.InfoInspect[1];
// // 	if (nPos == AUTO_INSPECTION3) pInfo = gData.InfoInspect[2];
// 
// 	if (pInfo == NULL) return FALSE;
// 
// 	if (gData.bR04C) {	// Btm Picker 4개만 사용.
// 		if (nScanCnt == 0) {
// 			if( pInfo[2] == 9 || pInfo[3] == 9 ||
// 				pInfo[7] == 9 || pInfo[8] == 9) { return TRUE; }
// 		}
// 		if (nScanCnt == 1) {
// 			if( pInfo[0] == 9 || pInfo[1] == 9 ||
// 				pInfo[5] == 9 || pInfo[6] == 9) { return TRUE; }
// 		}
// 
// 	} else {
// 		if (nScanCnt == 0) {
// 			if( pInfo[3] == 9 || pInfo[4] == 9 ||
// 				pInfo[8] == 9 || pInfo[9] == 9) { return TRUE; }
// 		}
// 		if (nScanCnt == 1) {
// 			if( pInfo[1] == 9 || pInfo[2] == 9 ||
// 				pInfo[6] == 9 || pInfo[7] == 9) { return TRUE; }
// 		}
// 		if (nScanCnt == 2) {
// 			if( pInfo[0] == 9 || pInfo[5] == 9) { return TRUE; }
// 		}
// 	}
// 	return FALSE;
// }
// 
// BOOL CSequenceMain::Select_Top2ScanPos(int nPos, int nScanCnt)
// {
// 	int *pInfo = NULL;
// // 	if (nPos == AUTO_INSPECTION1) pInfo = gData.InfoInspect[0];
// // 	if (nPos == AUTO_INSPECTION2) pInfo = gData.InfoInspect[1];
// // 	if (nPos == AUTO_INSPECTION3) pInfo = gData.InfoInspect[2];
// 
// 	if (pInfo == NULL) return FALSE;
// 
// 	if (gData.bR04C) {	// Btm Picker 4개만 사용.
// 		if (nScanCnt == 0) {
// 			if( pInfo[3] == 9 || pInfo[8] == 9) { return TRUE; }
// 		}
// 		if (nScanCnt == 1) {
// 			if( pInfo[2] == 9 || pInfo[7] == 9) { return TRUE; }
// 		}
// 		if (nScanCnt == 2) {
// 			if( pInfo[1] == 9 || pInfo[6] == 9) { return TRUE; }
// 		}
// 		if (nScanCnt == 3) {
// 			if( pInfo[0] == 9 || pInfo[5] == 9) { return TRUE; }
// 		}
// 
// 	} else {
// 		if (nScanCnt == 0) {
// 			if( pInfo[4] == 9 || pInfo[9] == 9) { return TRUE; }
// 		}
// 		if (nScanCnt == 1) {
// 			if( pInfo[3] == 9 || pInfo[8] == 9) { return TRUE; }
// 		}
// 		if (nScanCnt == 2) {
// 			if( pInfo[2] == 9 || pInfo[7] == 9) { return TRUE; }
// 		}
// 		if (nScanCnt == 3) {
// 			if( pInfo[1] == 9 || pInfo[6] == 9) { return TRUE; }
// 		}
// 		if (nScanCnt == 4) {
// 			if( pInfo[0] == 9 || pInfo[5] == 9) { return TRUE; }
// 		}
// 	}
// 	return FALSE;
// }
// 
// 
BOOL CSequenceMain::Select_LoadTrayPos(int &nTrayPosX, int &nTrayPosY)
{
	nTrayPosX = nTrayPosY = -1;
	
	if (Check_LoadTrayEmpty()) return FALSE;
	
	if (m_pEquipData->bSortDirRight) {
		if (gData.nPickerUseCnt < 6) {
			for (int y = 0; y < m_pEquipData->nLoadTrayY; y++) {
				if (gData.InfoLoadTray[y][0] > 0) { nTrayPosY = y; break;}
			}
			if (nTrayPosY == -1) return FALSE;

		}  else {
			// Picker 6ea
			int nTrayY = m_pEquipData->nLoadTrayY;	//8
			int nTrayX = m_pEquipData->nLoadTrayX;	//4
			for (int y = 0; y < nTrayY; y++) {
				for (int x = 0; x < nTrayX; x++) {
					if (gData.InfoLoadTray[y][x] > 0) { nTrayPosX = x; nTrayPosY = y; return TRUE; }
				}
			}
			if (nTrayPosX == -1 || nTrayPosY == -1) return FALSE;
		}

	} else {
		int nTrayY = m_pEquipData->nLoadTrayY;	//8
		int nTrayX = m_pEquipData->nLoadTrayX;	//4
		for (int y = 0; y < nTrayY; y++) {
			for (int x = 0; x < nTrayX; x++) {
				if (gData.InfoLoadTray[y][x] > 0) { nTrayPosX = x; nTrayPosY = y; return TRUE; }
			}
		}
		if (nTrayPosX == -1 || nTrayPosY == -1) return FALSE;
	}

	return TRUE;
}

BOOL CSequenceMain::Select_CapTrayPos(int &nTrayPosX, int &nTrayPosY, int &nCnt)
{
	nTrayPosX = nTrayPosY = -1;
	if (Check_CapTrayEmpty()) return FALSE;

	int nTrayY = m_pEquipData->nCapTrayY;	//8
	int nTrayX = m_pEquipData->nCapTrayX;	//4

	if (m_pEquipData->bCapPickUpMulti) {
		for (int y = 0; y < nTrayY; y++) {
			for (int x = 0; x < nTrayX; x++) {
				if (gData.InfoCapTray[y][x] == 9) { nTrayPosX = x; nTrayPosY = y; return TRUE; }
			}
		}
	} else {
		int nMaxCnt, nENo, nTX, nTY, nTCnt;
		nTCnt = 0;
		// Cap을 최대한 집을수 있는 개수
		nMaxCnt= m_pEquipData->nCapTrayX / 2;
		if (m_pEquipData->nCapTrayX % 2) nMaxCnt++;

		for (int y = 0; y < nTrayY; y++) {
			for (int x = 0; x < nTrayX; x++) {
				if (gData.InfoCapTray[y][x] == 9) {
					//하나씩 건너뛰어 캡이 있는지 확인
					if (x == 0) nENo = nMaxCnt;
					else		nENo = nMaxCnt - (x / 2);
					
					for (int i=0; i < nENo; i++) {
						nTX = x+(i*2);
						nTY = y;
						if (nTX >= m_pEquipData->nCapTrayX) break;
						if (gData.InfoCapTray[nTY][nTX] == 9) nTCnt++;
					}

					nTrayPosX = x; nTrayPosY = y; nCnt = nTCnt;
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

BOOL CSequenceMain::Select_UnloadTrayPos(int &nTrayPosX, int &nTrayPosY, int &nCnt)
{
	nTrayPosX = nTrayPosY = -1; nCnt = 0;
	if (Check_UnloadTrayFull()) return FALSE;

	int nTrayY = m_pEquipData->nUnloadTrayY;	// 6, 7
	int nTrayX = m_pEquipData->nUnloadTrayX;	// 5, 3
	for (int y = 0; y < nTrayY; y++) {
		for (int x = 0; x < nTrayX; x++) {
			if (gData.InfoUnloadTray[y][x] == 0) {
				nTrayPosX = x; nTrayPosY = y;
				for (int z = nTrayPosX; z < nTrayX; z++) {
					if (gData.InfoUnloadTray[y][z] == 0) nCnt++;
					else								 break;
				}
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL CSequenceMain::Select_LoadPickerPos(int &nPos, int &nCnt)
{
	nPos = -1; nCnt = 0;

	for (int i = 0; i < gData.nPickerUseCnt; i++) { 
		if (gData.InfoLoadPick[i] == 0) { nPos = i; break; } 
	}

	for (int j = nPos; j < gData.nPickerUseCnt; j++) { 
		if (gData.InfoLoadPick[j] == 0)	nCnt++;
		else							break;
	}
	if (nPos == -1 || nCnt == 0) return FALSE;

	return TRUE;
}

BOOL CSequenceMain::Select_CapPickerPos(int &nPos, int &nCnt)
{
	nPos = -1; nCnt = 0;

	for (int i = 0; i < gData.nPickerUseCnt; i++) { 
		if (gData.InfoCapPick[i] == 0) { nPos = i; break; } 
	}

	for (int j = nPos; j < gData.nPickerUseCnt; j++) { 
		if (gData.InfoCapPick[j] == 0)	nCnt++;
		else							break;
	}
	if (nPos == -1 || nCnt == 0) return FALSE;

	// Cap Tray에서 Cap을 하나씩 건너뛰어 집기 때문에 Pick 전체를 한꺼번에 내릴수 없다.
	if (m_pEquipData->bCapPickUpMulti == FALSE) {
		int nMaxCnt = m_pEquipData->nCapTrayX / 2;
		if (m_pEquipData->nCapTrayX % 2) nMaxCnt++;
		if (nCnt > nMaxCnt) nCnt = nMaxCnt;
	}

	return TRUE;
}

BOOL CSequenceMain::Select_AssyPickerPos(int &nPos, int &nCnt)
{
	nPos = -1;  nCnt = 0;

	for (int i = 0; i < gData.nPickerUseCnt; i++) { 
		if (gData.InfoAssyPick[i] == 0) { nPos = i; break; } 
	}

	for (int j = nPos; j < gData.nPickerUseCnt; j++) { 
		if (gData.InfoAssyPick[j] == 0)	nCnt++;
		else							break;
	}
	if (nPos == -1 || nCnt == 0) return FALSE;

	return TRUE;
}

BOOL CSequenceMain::Select_AssyPickerNgPos(int &nPos)
{
	nPos = -1;

	for (int i = 0; i < gData.nPickerUseCnt; i++) { 
		if (gData.InfoAssyPick[i] == 2) { nPos = i; break; } 
	}

	if (nPos == -1) return FALSE;

	return TRUE;
}

BOOL CSequenceMain::Select_UnloadPickerPos(int &nPos, int &nCnt)
{
	nPos = -1; nCnt = 0;

	for (int i = 0; i < gData.nPickerUseCnt; i++) { 
		if (gData.InfoUnloadPick[i] > 0) { nPos = i; break; } 
	}

	for (int j = nPos; j < gData.nPickerUseCnt; j++) { 
		if (gData.InfoUnloadPick[j] > 0)	nCnt++;
		else								break;
	}
	if (nPos == -1 || nCnt == 0) return FALSE;

	return TRUE;
}


void CSequenceMain::Set_InspectJigNo(int nType, int nScanCnt, int &nINo1, int &nINo2)
{
	if (nType == 0) {	// CM
		if (nScanCnt == 0) { nINo1 = 0; nINo2 =  3; }
		if (nScanCnt == 1) { nINo1 = 1; nINo2 =  4; }
		if (nScanCnt == 2) { nINo1 = 3; nINo2 = -1; }
	}
	if (nType == 1) {	// Cap
		if (nScanCnt == 0) { nINo1 = 0; nINo2 =  3; }
		if (nScanCnt == 1) { nINo1 = 1; nINo2 =  4; }
		if (nScanCnt == 2) { nINo1 = 2; nINo2 = -1; }
	}
}
///////////////////////////////////////////////////////////////////////////////
BOOL CSequenceMain::Check_CmAlignDone()
{
	if (!m_pEquipData->bUseVisionCmAlign) return TRUE;

// 	for (int i=0; i<6; i++) {
// 		if (gData.InfoIndex[0][i] > 0) {
// 			if (gData.InfoIndex[0][i] != 1) return FALSE;
// 		}
// 	}
	if (gData.bScanDone[0]) return TRUE;
	return TRUE;	// All Inspect Done
}
BOOL CSequenceMain::Check_CapAlignDone()
{
	if (!m_pEquipData->bUseVisionCapAlign) return TRUE;

// 	for (int i=0; i<6; i++) {
// 		if (gData.InfoAssyPick[i] > 0) {
// 			if (gData.InfoAssyPick[i] != 1) return FALSE;
// 		}
// 	}
	if (gData.bScanDone[1]) return TRUE;
	return TRUE;	// All Inspect Done
}

// 한번에 2Lot 이상 들어가면 안된다.
BOOL CSequenceMain::Check_LoadTrayLoading(int nPNo)
{
	if (gData.nPNoLoadPick == nPNo) return FALSE;
	if (gData.nPNoIndex[0] == nPNo) return FALSE;
	if (gData.nPNoIndex[1] == nPNo) return FALSE;
	if (gData.nPNoIndex[2] == nPNo) return FALSE;
	if (gData.nPNoTransStage == nPNo) return FALSE;
 	if (gData.nPNoUnloadPick == nPNo) return FALSE;
// 	if (gData.nPNoUnloadTray == nPNo) return FALSE;

	return TRUE;
}

BOOL CSequenceMain::Check_LoadLotEnd(int nPNo, int nMode)
{
	// 로드가 다끝나고 앞공정에 모듈이 없으면 TRUE
	if (!gData.bCycleStop && (gData.nLoadTrayCount[nPNo-1] < gData.nTrayUseCount[nPNo-1])) return FALSE;
	if (gData.nPNoLoadTray[0] == nPNo || gData.nPNoLoadTray[1] == nPNo) return FALSE;
// 	if (!Check_AngleTrayEmpty()) return FALSE;
	if (nMode == 0 && !Check_LoadPickerEmpty()) return FALSE;

	return TRUE;
}

BOOL CSequenceMain::Check_UnloadLotEnd(int nMode)
{
//  	if (!m_bLoadLotEnd) return FALSE;
	if (m_nLoadStage1Case != 60 && m_nLoadStage1Case != 50) return FALSE;
	if (m_nLoadStage2Case != 60 && m_nLoadStage2Case != 50) return FALSE;
	if (!Check_IndexEmpty(-1)) return FALSE;
	if (!Check_LoadPickerEmpty()) return FALSE;
	if (!Check_TransStageEmpty()) return FALSE;
	if (!Check_UnloadPickerEmpty()) return FALSE;

	return TRUE;
}


BOOL CSequenceMain::Get_VisionInspectUse()
{
	BOOL bUse = (m_pEquipData->bUseVisionCmAlign || m_pEquipData->bUseVisionCapAlign);
	return bUse;
}

void CSequenceMain::Set_TaktLog(int nNo)
{
	// 0:Load, 1:Assembly, 2:Unload
	if (!gData.IndexDone[nNo]) return;
	gLot.dwTaktData[nNo] = GetTickCount() - gLot.dwTaktData[nNo];
	gLot.bTaktDone[nNo] = TRUE;
}

void CSequenceMain::Set_IndexEnd()
{
	int i = 0;
	int j = 0;	
	CString ErrMsg;
// 	for (i = 0; i < 2; i++) {
// 		ErrMsg.Format("1.InfoIndex%d ==> InfoIndex[0]:%d InfoIndex[1]:%d InfoIndex[2]:%d", i, gData.InfoIndex[i][0], gData.InfoIndex[i][1], gData.InfoIndex[i][2]);
// 		g_objLogFile.Save_HandlerLog(ErrMsg);
// 	}
	memmove(gData.nPNoIndex + 1, gData.nPNoIndex, sizeof(int) * 2);
	memmove(gData.nTNoIndex + 1, gData.nTNoIndex, sizeof(int) * 2 * 6);
	memmove(gData.nCNoIndex[1], gData.nCNoIndex[0], sizeof(int) * 2 * 6);
	memmove(gData.InfoIndex[1], gData.InfoIndex[0], sizeof(int) * 2 * 6);

	gData.nPNoIndex[0] = 0;
	memset(gData.nTNoIndex[0], 0x00, sizeof(int) * 6);
	memset(gData.InfoIndex[0], 0x00, sizeof(int) * 6);
	memset(gData.nCNoIndex[0], 0x00, sizeof(int) * 6);
	memset(gData.IndexDone, 0x00, sizeof(BOOL) * 3);

	for (i = 1; i >= 0; i--) {
		for (j = 0; j < 6; j++) {
			gData.sCIDIndex[i+1][j] = gData.sCIDIndex[i][j];
		}
	}
	for (i = 0; i < 6; i++) {
		gData.sCIDIndex[0][i].Empty();
	}

	if (gData.nCNoIndex[2][0] > 0) {
		int nLineNo = gData.nCNoIndex[2][0] / gData.nPickerUseCnt;
		int nTNo = gData.nTNoIndex[2][0] - 1;
		gData.dwCmTactStart[nTNo][nLineNo] = GetTickCount();
	}

// 	for (i = 0; i < 2; i++) {
// 		ErrMsg.Format("1.InfoIndex%d ==> InfoIndex[0]:%d InfoIndex[1]:%d InfoIndex[2]:%d", i, gData.InfoIndex[i][0], gData.InfoIndex[i][1], gData.InfoIndex[i][2]);
// 		g_objLogFile.Save_HandlerLog(ErrMsg);
// 	}

	// Index Takt Log /////////////////////////////////////////////////////////
	gLot.dwTaktData[3] = GetTickCount() - gLot.dwTaktData[3];	// Index Takt

	CString	sLog;
	sLog.Format("[Index Table] Index[%d] LP[%d] Assy[%d] TP[%d]",
		gLot.dwTaktData[3], gLot.dwTaktData[0], gLot.dwTaktData[1], gLot.dwTaktData[2]);
	g_objLogFile.Save_HandlerLog(sLog);	// MCC로그 따로 저장해준다.

	g_dlgWork.PostMessage(UM_INDEX_TACK, (WPARAM)gLot.dwTaktData[3], NULL);

	memset(gLot.bTaktDone, 0x00, sizeof(BOOL) * 3);
	for (int i = 0; i < 4; i++) gLot.dwTaktData[i] = GetTickCount();
	///////////////////////////////////////////////////////////////////////////
}
///////////////////////////////////////////////////////////////////////////////

void CSequenceMain::Job_LotStart(int nPortNo)
{
	int nLPNo = nPortNo - 1;
	if (Get_VisionInspectUse()) g_objInspector.Set_LotStart(gData.sLotID[nLPNo], nPortNo, "", "");
	gLot.bLotEndComplete[nLPNo] = FALSE;

	SYSTEMTIME time;
	GetLocalTime(&time);

	gLot.sLotID[nLPNo] = gData.sLotID[nLPNo];
	gLot.sStartTime[nLPNo].Format("%04d%02d%02d_%02d%02d%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	m_dwFirstLoad = gLot.dwLotStart[nLPNo] = GetTickCount();
	gLot.nTrayCount[nLPNo] = gData.nTrayUseCount[nLPNo];
	gLot.nCmCount[nLPNo] = gData.nCmUseCount[nLPNo];

	gLot.nCapCount[nLPNo] = 0;
 	gLot.nErrorCount = 0;
 	if (gData.bFirstLotStart == TRUE) {
		gData.bFirstLotStart = FALSE;
		gLot.dwRunTime = gLot.dwErrorTime = gLot.dwStopTime = 0;
	}

	g_dlgWork.PostMessage(UM_LOT_START_END, (WPARAM)1, nPortNo);	// LotStart
}

void CSequenceMain::Job_LotEnd(int nPortNo)
{
	int nLPNo = nPortNo - 1;
	if (gLot.nCmCount[nLPNo] < 1) return;	// Error
	if (gLot.bLotEndComplete[nLPNo] == TRUE) return;

	//2주내 바코드 중복 체크 
	g_objLogFile.Save_BarcodeChkLog(gData.sCapLotID);
	g_objLogFile.Save_BarcodeChkLog(gData.sShipLotID);

	gLot.bLotEndComplete[nLPNo] = TRUE;

	SYSTEMTIME time;
	GetLocalTime(&time);
	gLot.dwLotEnd[nLPNo] = GetTickCount();
	gLot.sEndTime[nLPNo].Format("%04d%02d%02d_%02d%02d%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);

	if (Get_VisionInspectUse()) g_objInspector.Set_LotEnd(gData.sLotID[nLPNo], nPortNo);
	if (m_pEquipData->bUseInlineMode) g_objAviUDP.Set_LotEnd(nPortNo);
	gData.bMesRegistered[nLPNo] = FALSE;

	int nTrayCnt = gData.nLastTrayNo[nLPNo];
	int nCmCnt = gLot.nCapCount[nLPNo];

	DWORD dwTime = gLot.dwLotEnd[nLPNo] - gLot.dwLotStart[nLPNo];
	gLot.dTackTime = dwTime / 1000.0 / nCmCnt;	//gLot.nCmCount[nLPNo];	// Floating-point inexact result
	m_dwLastUnLoad = gLot.dwLotEnd[nLPNo] - m_dwLastUnLoad;

	m_strLog.Format("LotID,%s,Start_Time,%s,End_Time,%s,Time,%d,Tray_Count,%02d,CM_Count,%04d,Tack,%0.7lf",
		gLot.sLotID[nLPNo], gLot.sStartTime[nLPNo], gLot.sEndTime[nLPNo], dwTime, nTrayCnt/*gLot.nTrayCount[nLPNo]*/, nCmCnt/*gLot.nCmCount[nLPNo]*/, gLot.dTackTime);
	g_objLogFile.Save_HandlerLog(m_strLog);

// 	double dOne = (double)(dwTime - m_dwFirstLoad - m_dwLastUnLoad) / (gLot.nCmCount[nLPNo] - 1);
// 	m_strLog.Format("LotID(%s), Load(%0.3lf), Unload(%0.3lf), Takt as 600 EA Run(%0.6lf)", gLot.sLotID[nLPNo], (double)m_dwFirstLoad / 1000, (double)m_dwLastUnLoad / 1000, (dOne * 599 + m_dwFirstLoad + m_dwLastUnLoad) / 600000.0);
// 	g_objLogFile.Save_HandlerLog(m_strLog);

	
	m_strLog.Format("%s,%s,%s,%d,%02d,%04d,%0.7lf,%d",
		gLot.sLotID[nLPNo], gLot.sStartTime[nLPNo], gLot.sEndTime[nLPNo], dwTime, nTrayCnt/*gLot.nTrayCount[nLPNo]*/, nCmCnt/*gLot.nCmCount[nLPNo]*/, gLot.dTackTime, gLot.nCapFailCount[nLPNo]);
	g_objLogFile.Save_JobListLog(m_strLog, TRUE);

	g_dlgWork.PostMessage(UM_LOT_START_END, (WPARAM)2, nPortNo);	// LotEnd

	// UPH & 생산량
	//if (gLot.sLotID.GetLength() < 12) return;		// Lot ID

	gUph.dTaktTime = gLot.dTackTime;

	if (time.wHour >= 7 && time.wHour < 19) gUph.nCmCount[0] += nCmCnt;	//gLot.nCmCount[nLPNo];
	else gUph.nCmCount[1] += nCmCnt;	//gLot.nCmCount[nLPNo];

	int i = (int)time.wHour;
	int j = gUph.nLotCount[i];
	if (j > 49) return;		// 1시간에 LOT 수량 MAX 50개

	gUph.dTakt[i][j] = gLot.dTackTime;
	gUph.nLotCount[i] = j + 1;

	int nSum = nCmCnt;	//gLot.nCmCount[nLPNo]; //gLot.nGoodCount[nLPNo] + gLot.nNgCount[nLPNo];
	double dRate = 0.0;	//(nSum == 0 ? 0.0 : gLot.nGoodCount[nLPNo] * 100.0 / nSum);
	// Lot Result [날짜,Lot Start,Lot End,Term,호기,Lot ID,투입수];
	CString strMsg, strStart, strEnd, strTerm, strEquip;

	strStart.Format("%s:%s:%s", gLot.sStartTime[nLPNo].Mid(9, 2), gLot.sStartTime[nLPNo].Mid(11, 2), gLot.sStartTime[nLPNo].Mid(13, 2));
	strEnd.Format("%s:%s:%s", gLot.sEndTime[nLPNo].Mid(9, 2), gLot.sEndTime[nLPNo].Mid(11, 2), gLot.sEndTime[nLPNo].Mid(13, 2));
	int nSec = (int)(gLot.dwLotEnd[nLPNo] - gLot.dwLotStart[nLPNo]) / 1000;
	int nH = nSec / 60 / 60;
	int nM = (nSec - nH * 60 * 60) / 60;
	int nS = (nSec - nH * 60 * 60) % 60;
	strTerm.Format("%02d:%02d:%02d", nH, nM, nS);
	strEquip = m_pEquipData->sEquipName;
// 	double dNgRate = (nSum == 0 ? 0.0 : gLot.nNgCount[nLPNo] * 100.0 / nSum);

	strMsg.Format("%s,%s,%s,%s,%s,%d",
		strStart, strEnd, strTerm, strEquip, gLot.sLotID[nLPNo], nSum);

	g_objLogFile.Save_LotResult(nLPNo, strMsg);

	//Time, Lot_ID, Total, Good, NG, RosRequest, RosGood, RosNg, RosRepair, Filtering1, Filtering2, RateAvi, Final, Tack
// 	double dFiltering1 = (gLot.nRosRequest[nLPNo] == 0 ? 0.0 : (gLot.nRosGood[nLPNo] * 100.0) / gLot.nRosRequest[nLPNo]);
// 	double dFiltering2 = (nSum == 0 ? 0.0 : (gLot.nRosGood[nLPNo] * 100.0) / nSum);
// 	double dRateAvi = 0.0;	//(nSum == 0 ? 0.0 : ((gLot.nGoodCount[nLPNo] - gLot.nRosGood[nLPNo]) * 100.0) / nSum);

	CCME8000Dlg *pMainDlg = (CCME8000Dlg*)AfxGetApp()->GetMainWnd();
	pMainDlg->Set_LotStateTime();
	strMsg.Format("%s,%s,%s,%0.3lf,%d,%0.3lf,%0.3lf,%0.3lf,%d,%0.2lf,%0.3lf,%0.3lf,%0.2lf",
		gLot.sLotID[nLPNo], strStart, strEnd, gLot.dTackTime, nSum, (double)(gLot.dwRunTime) / 1000,
		(double)(gLot.dwStopTime) / 1000, (double)(gLot.dwErrorTime) / 1000, gLot.nErrorCount, dRate, 3600 / gLot.dTackTime ,(double)(gLot.dwRunTime + gLot.dwErrorTime + gLot.dwStopTime) / gLot.nErrorCount);
	g_objLogFile.Save_OperatingRatio(strMsg);
	gLot.dwRunTime = gLot.dwErrorTime = gLot.dwStopTime = 0;

	g_objLogFile.Save_AverageCycle(nLPNo);
	g_objLogFile.Save_LotLog(nPortNo);	//gjcs

	// 해당포트 판정 완료, 결과 초기화
	gLot.nCapFailCount[nLPNo] = 0;

	memset(gData.nCmInspectInfo, 0x00, sizeof(int) * 2 * 30 * 40);
	memset(gData.nCapInspectInfo, 0x00, sizeof(int) * 2 * 100 * 40);
	memset(gData.nCmJigNo[nLPNo], 0x00, sizeof(int) * 30 * 40 * 5);	// Module Tracking 초기화
	gData.nLoadTrayCount[nLPNo] = 0;
	gData.nUnloadTrayCount = 0;

	//과검랏 관련 변수 초기화
	m_nUnloadLotIndex = 0;
	m_nUnloadLotCmCnt = 0;

	if (!m_pEquipData->bUseInlineMode) {
		gData.sLotID[nLPNo] = "";
		gData.nCmUseCount[nLPNo] = 0;
		gData.nTrayUseCount[nLPNo] = 0;	
	}

	gData.bAviLotEnd[nLPNo] = FALSE;
	gData.bLoadLampOn[nLPNo] = TRUE;

	if (!m_pEquipData->bUseInlineMode) {
		if (nLPNo == 0) {
			g_dlgWork.m_stcLotIdR.SetWindowText("");
			g_dlgWork.m_stcCmCountR.SetWindowText("0");
			g_dlgWork.m_stcTrayCountR.SetWindowText("0");
		} else {
			g_dlgWork.m_stcLotIdS.SetWindowText("");
			g_dlgWork.m_stcCmCountS.SetWindowText("0");
			g_dlgWork.m_stcTrayCountS.SetWindowText("0");
		}
	}
	
	if (!m_pEquipData->bUseInlineMode)	g_dlgWork.Enable_UserInput(nPortNo, TRUE);

	g_dlgWork.PostMessage(UM_UPDATE_UPH, NULL, NULL);

//	Beep_Post(1000);
}

void CSequenceMain::Beep_Post(int nState, int nTime)
{
	if (nTime < 500) nTime = 500;

	if (nState == 0) {	// Lot End (Alarm state)
		m_pDY12->oBuzzerBit0 = m_pEquipData->bBuzzer[0][0];
		m_pDY12->oBuzzerBit1 = m_pEquipData->bBuzzer[0][1];
		m_pDY12->oBuzzerBit2 = m_pEquipData->bBuzzer[0][2];
		m_pDY12->oBuzzerBit3 = m_pEquipData->bBuzzer[0][3];
		m_pDY12->oBuzzerBit4 = m_pEquipData->bBuzzer[0][4];

	} else if (nState == 1) {	// NG Full
		m_pDY12->oBuzzerBit0 = TRUE;	//m_pEquipData->bBuzzer[1][0];
		m_pDY12->oBuzzerBit1 = FALSE;	//m_pEquipData->bBuzzer[1][1];
		m_pDY12->oBuzzerBit2 = FALSE;	//m_pEquipData->bBuzzer[1][2];
		m_pDY12->oBuzzerBit3 = FALSE;	//m_pEquipData->bBuzzer[1][3];
		m_pDY12->oBuzzerBit4 = FALSE;	//m_pEquipData->bBuzzer[1][4];
	}
	g_objAJinAXL.Write_Output(12);
	theApp.uSleep(nTime);
	m_pDY12->oBuzzerBit0 = FALSE;
	m_pDY12->oBuzzerBit1 = FALSE;
	m_pDY12->oBuzzerBit2 = FALSE;
	m_pDY12->oBuzzerBit3 = FALSE;
	m_pDY12->oBuzzerBit4 = FALSE;
	g_objAJinAXL.Write_Output(12);
}

BOOL CSequenceMain::Check_MainIndexCylinder()
{
	if (m_pDX11->iIndexLoadAlignIn && !m_pDX11->iIndexLoadAlignOut &&
		m_pDX11->iIndexAssyAlignIn && !m_pDX11->iIndexAssyAlignOut &&
		m_pDX11->iIndexTransAlignIn && !m_pDX11->iIndexTransAlignOut &&
		m_pDX11->iIndexLoadVacDown && !m_pDX11->iIndexLoadVacUp &&
		m_pDX11->iIndexAssyVacDown && !m_pDX11->iIndexAssyVacUp) {
		return TRUE;
	}
	return FALSE;
}

BOOL CSequenceMain::Check_Mode()
{
	if(theApp.Get_MainMode() == MODE_MANUAL
		|| theApp.Get_MainMode() == MODE_SETUP
		|| theApp.Get_MainMode() == MODE_PARAM
		|| theApp.Get_MainMode() == MODE_PROHIBIT
		|| theApp.Get_MainMode() == MODE_INITIAL) return FALSE;
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
// 1. (Error : 3100)
BOOL CSequenceMain::TrayPicker_Run()
{
	switch (m_nTrayPickCase) {
	case 0:		// Start 시 1로 진행
		return TRUE;

	case 1:		// AVI Tray Check.
		if (m_bUnloadLotEnd || gData.bCycleStop || !m_pEquipData->bUseInlineMode) {
			m_nTrayPickCase = 30;	// 사이클스탑이나 InlineMode가 아니면 종료.

		} else if (m_pEquipData->bUseInlineMode) {
			m_dwTrayPick = GetTickCount();
			m_nTrayPickCase++; m_tTrayPickLoop.Set_LoopTime(5000);
		}
		return TRUE;

	case 2:		// Tray Picker Grip Check
		if (g_objCommon.Get_TrayPickerGripOpen() &&
			g_objCommon.Check_Position(AX_TRAY_PICKER_Z, 0) &&
			g_objCommon.Check_Position(AX_TRAY_PICKER_R, 0))
		{
			g_objCommon.Move_Position(AX_TRAY_PICKER_X, 0);	// AVI Position
			m_nTrayPickCase++; m_tTrayPickLoop.Set_LoopTime(5000);
		}		
		break;
	case 3:		// Z Move to AVI Down
		if (g_objCommon.Check_Position(AX_TRAY_PICKER_X, 0) &&
			g_objCommon.Check_Position(AX_TRAY_PICKER_Z, 0) &&
			g_objCommon.Check_Position(AX_TRAY_PICKER_R, 0)) {

			if (!Check_AviTrayPickup()) return TRUE;
			g_objCommon.Move_Position(AX_TRAY_PICKER_Z, 1);
			m_tTrayPickLoop.Takt_Start();
			m_nTrayPickCase++; m_tTrayPickLoop.Set_LoopTime(10000);
		}		
		break;
	case 4:		// Grip Close
		if (g_objCommon.Check_Position(AX_TRAY_PICKER_Z, 1)) {
			m_tTrayPickLoop.Takt_Save(1, 1);
			m_tTrayPickLoop.Takt_Start();
			g_objCommon.Set_TrayPickerGripClose();
			m_nTrayPickCase++; m_tTrayPickLoop.Set_LoopTime(5000);
		}		
		break;
	case 5:		// Z Move to Ready Up
		if (g_objCommon.Get_TrayPickerGripClose() && m_pDX00->iTrayPickerExist) {
			m_tTrayPickLoop.Takt_Save(1, 2);
			m_tTrayPickLoop.Takt_Start();

			gData.nPNoTrayPick = gData.nPNoAviPort;
			gData.nTNoTrayPick = gData.nTNoAviPort;
			gData.nCmCntTrayPick = gData.nCmCntAviPort;
			g_objCommon.Move_Position(AX_TRAY_PICKER_Z, 0);
			m_nTrayPickCase++; m_tTrayPickLoop.Set_LoopTime(10000);
		}		
		break;
	case 6:		// X Move to Load1 Port & AVI Tray Pickup Complete Send
		if (g_objCommon.Check_Position(AX_TRAY_PICKER_Z, 0) && m_pDX00->iTrayPickerExist) {
			m_tTrayPickLoop.Takt_Save(1, 3);
			m_tTrayPickLoop.Takt_Start();
			
			if (m_pEquipData->bUseInlineMode) g_objAviUDP.Set_TrayUnload();
			gData.bAviTrayLoad = FALSE;
			if (m_pEquipData->bUseTrayPickerTurn) {
				g_objCommon.Move_Position(AX_TRAY_PICKER_X, 2);	// Rotate Position				
				m_nTrayPickCase++;

			} else {
				g_objCommon.Move_Position(AX_TRAY_PICKER_X, 1);	// Load1 Position
				m_nTrayPickCase = 10;
			}
			m_tTrayPickLoop.Set_LoopTime(10000);
		}		
		break;
	case 7:		// Tray Rotation
		if (g_objCommon.Check_Position(AX_TRAY_PICKER_X, 2)) {
			g_objCommon.Move_Position(AX_TRAY_PICKER_R, 1);	// Rotate
			m_nTrayPickCase++; m_tTrayPickLoop.Set_LoopTime(10000);
		}
		break;
	case 8:		// Rotation Check
		if (g_objCommon.Check_Position(AX_TRAY_PICKER_R, 1)) {
			g_objCommon.Move_Position(AX_TRAY_PICKER_X, 1);	// Load1 Position
			m_nTrayPickCase = 10; m_tTrayPickLoop.Set_LoopTime(10000);
		}
		break;

	case 10:	// Check Position
		if (g_objCommon.Check_Position(AX_TRAY_PICKER_X, 1) &&
			((!m_pEquipData->bUseTrayPickerTurn && g_objCommon.Check_Position(AX_TRAY_PICKER_R, 0)) ||
			 ( m_pEquipData->bUseTrayPickerTurn && g_objCommon.Check_Position(AX_TRAY_PICKER_R, 1))))
		{
			m_tTrayPickLoop.Takt_Save(1, 4);
			m_nTrayPickCase++; m_tTrayPickLoop.Set_LoopTime(5000);
		}
		break;
	case 11:		// Inter Lock - Load Port1 Check, Tray가 있으면 대기한다.
		if (!m_pDX00->iLoadPort1LowCheck && !m_bLoadPortTrayExist &&
			((m_nLoadStage1Case < 3 || m_nLoadStage1Case > 9) && 
			 (m_nLoadStage2Case < 3 || m_nLoadStage2Case > 9)))	//Temp
		{
			m_nTrayPickCase++; m_tTrayPickLoop.Set_LoopTime(5000);
		}
		return TRUE;
	case 12:		// Z Move to Load1 Down
		if (g_objCommon.Check_Position(AX_TRAY_PICKER_X, 1) &&
			((!m_pEquipData->bUseTrayPickerTurn && g_objCommon.Check_Position(AX_TRAY_PICKER_R, 0)) ||
			 ( m_pEquipData->bUseTrayPickerTurn && g_objCommon.Check_Position(AX_TRAY_PICKER_R, 1))))
		{
			m_tTrayPickLoop.Takt_Start();
			g_objCommon.Move_Position(AX_TRAY_PICKER_Z, 2);
			m_nTrayPickCase++; m_tTrayPickLoop.Set_LoopTime(10000);
		}
		break;
	case 13:	// Grip Open
		if (g_objCommon.Check_Position(AX_TRAY_PICKER_Z, 2)) {
			m_tTrayPickLoop.Takt_Save(1, 5);
			m_tTrayPickLoop.Takt_Start();
			g_objCommon.Set_TrayPickerGripOpen();
			m_nTrayPickCase++; m_tTrayPickLoop.Set_LoopTime(5000);
		}
		break;
	case 14:	// 정보전달, Z Move to Ready up
		if (g_objCommon.Get_TrayPickerGripOpen()) {
			m_tTrayPickLoop.Takt_Save(1, 6);
			m_tTrayPickLoop.Takt_Start();

			m_bLoadPortTrayExist = TRUE;
			gData.nPNoLoadPort = gData.nPNoTrayPick;
			gData.nTNoLoadPort = gData.nTNoTrayPick;
			gData.nCmCntLoadPort = gData.nCmCntTrayPick;
			g_objCommon.Move_Position(AX_TRAY_PICKER_Z, 0);
			//if (g_objAviUDP.Get_AviStatus()){
				m_nTrayPickCase++; m_tTrayPickLoop.Set_LoopTime(10000);

// 			} else {
// 				// AVI 멈춰 있으면 대기 한다.
// 				m_nTrayPickCase = 40; m_tTrayPickLoop.Set_LoopTime(10000);
// 			}
		}
		break;
	case 15:	// X Move to AVI Position
		if (g_objCommon.Check_Position(AX_TRAY_PICKER_Z, 0) && !m_pDX00->iTrayPickerExist) {
			m_tTrayPickLoop.Takt_Save(1, 7);
			m_tTrayPickLoop.Takt_Start();

			if (m_pEquipData->bUseTrayPickerTurn)	{
				g_objCommon.Move_Position(AX_TRAY_PICKER_X, 2);
				m_nTrayPickCase++; m_tTrayPickLoop.Set_LoopTime(10000);

			} else {
				g_objCommon.Move_Position(AX_TRAY_PICKER_X, 0);
				m_nTrayPickCase = 20; m_tTrayPickLoop.Set_LoopTime(10000);
			}			
		}
		break;
	case 16:
		if (g_objCommon.Check_Position(AX_TRAY_PICKER_X, 2)) {
			g_objCommon.Move_Position(AX_TRAY_PICKER_R, 0);	// Rotate
			m_nTrayPickCase++; m_tTrayPickLoop.Set_LoopTime(10000);
		}
		break;
	case 17:		// Rotation Check
		if (g_objCommon.Check_Position(AX_TRAY_PICKER_R, 0)) {
			g_objCommon.Move_Position(AX_TRAY_PICKER_X, 0);	// AVI Position
			m_nTrayPickCase = 20; m_tTrayPickLoop.Set_LoopTime(10000);
		}
		break;

	case 20:	// Position Check
		if (g_objCommon.Check_Position(AX_TRAY_PICKER_X, 0) &&
			g_objCommon.Check_Position(AX_TRAY_PICKER_R, 0)) 
		{
			m_tTrayPickLoop.Takt_Save(1, 8);
			m_nTrayPickCase = 1; m_tTrayPickLoop.Set_LoopTime(10000);

			m_strLog.Format("TrayPicker, %d", GetTickCount() - m_dwTrayPick);
			g_objLogFile.Save_TestLog(m_strLog);			
		}
		break;

		//종료
	case 30:
		if (g_objCommon.Get_TrayPickerGripOpen() &&
			g_objCommon.Check_Position(AX_TRAY_PICKER_Z, 0) &&
			g_objCommon.Check_Position(AX_TRAY_PICKER_R, 0))
		{
			g_objCommon.Move_Position(AX_TRAY_PICKER_X, 2);
			m_nTrayPickCase++; m_tTrayPickLoop.Set_LoopTime(10000);
		}
		break;
	case 31:
		if (g_objCommon.Check_Position(AX_TRAY_PICKER_X, 2)) {
			m_nTrayPickCase = 0; m_tTrayPickLoop.Set_LoopTime(10000);
		}
		break;

		// AVI 정지 상태이면 대기
	case 40:
		if (g_objCommon.Get_TrayPickerGripOpen() && !m_pDX00->iTrayPickerExist &&
			g_objCommon.Check_Position(AX_TRAY_PICKER_Z, 0))
		{
			//if (g_objAviUDP.Get_AviStatus()){
				m_nTrayPickCase = 15; m_tTrayPickLoop.Set_LoopTime(10000);

			//} else { return TRUE; }
		}
		break;
	}

	// 1. (Error : 3100)
	if (m_tTrayPickLoop.Over_LoopTime()) {
		g_objCommon.Show_Error(3100 + m_nTrayPickCase);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 2. (Error : 3200)
BOOL CSequenceMain::LoadStage1_Run()
{
	static int nLs1WorkPort = 0;		// 작업중인 Port (1 ~ 2)
	static int nLs1AviPort = 0;			// Inline Mode 일때 Avi Port 번호 받는다.

	if (gData.bLoadPort3Wait && m_nLoadStage1Case > 22 && m_nLoadStage1Case < 50) return TRUE;

	if (!m_pEquipData->bUseInlineMode) {
		// LJH 1번 투입시 2번에 Tray 있으면 가져가야한다...
		// Load Tray Lot 투입시 도어락, 버튼 확인하여 Tray Loading 작업 못하게 막아준다.
		if (((m_bLotLoadEnable[0] == TRUE && ((m_pEquipData->bUseDoorLock && m_pDX13->iDoor01Unlock) || !m_pEquipData->bUseDoorLock) && (nLs1WorkPort == 1 && m_nLoadStage1Case > 3 && m_nLoadStage1Case < 10)) ||
			 (m_bLotLoadEnable[1] == TRUE && ((m_pEquipData->bUseDoorLock && m_pDX13->iDoor02Unlock) || !m_pEquipData->bUseDoorLock) && (nLs1WorkPort == 2 && m_nLoadStage1Case > 3 && m_nLoadStage1Case < 10)))) return TRUE;
	}

	switch (m_nLoadStage1Case) {
	case 0:		// Start 시 1로 진행
		nLs1WorkPort = 0;
		nLs1AviPort = 0;
		return TRUE;

	case 1:		// Load Tray, Port 확인.
		if (g_objCommon.Check_Position(AX_LOAD_STAGE1_Z, 1)) {	// 안전 확인
			if (m_bUnloadLotEnd || gData.bCycleStop) {
				nLs1WorkPort = 0;
				m_nLoadStage1Case = 0;	// 사이클스탑이면 종료.

			} else {
				if (m_pEquipData->bUseInlineMode) {
					// Inline Mode
					// Tray, Tray Picker Check
					if (m_pDX00->iLoadPort1LowCheck && !gData.bCycleStop && m_bLoadPortTrayExist &&
						(m_nTrayPickCase < 12 || m_nTrayPickCase > 15))
					{
						nLs1WorkPort = 1;
						nLs1AviPort = gData.nPNoLoadPort;
						m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(5000);
					}
					return TRUE;
				} else {
					// 단독 모드
					if (m_bLoadLotEnd) {
						m_nLoadStage1Case = 0;

					} else if ((m_pDX00->iLoadPort1LowCheck || m_pDX01->iLoadPort2LowCheck) && !gData.bCycleStop) {
						// 맨처음 시작할때 2개 모두 적재 되어있으면 1번부터 가져간다.
						// 작업할 포트에 트레이 유무와 입력수량확인.
						if ((nLs1WorkPort == 0 || nLs1WorkPort == 2) && m_pDX00->iLoadPort1LowCheck && (gData.nCmUseCount[0] > 0)) { nLs1WorkPort = 1; nLs1AviPort = 1; }
						else if ((nLs1WorkPort == 0 || nLs1WorkPort == 1) && m_pDX01->iLoadPort2LowCheck && (gData.nCmUseCount[1] > 0)) { nLs1WorkPort = 2; nLs1AviPort = 2; }

						//if (!Check_LoadTrayLoading(nLs1WorkPort)) return TRUE;	// 같은 Port Lot을 동시에 검사하지 않는다.
						m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(5000);
					}
				}
			}
		}
		break;
	case 2:		// Tray X Move to Load Position
		if (gData.bCycleStop) {
			nLs1WorkPort = 0;
			nLs1AviPort = 0;
			m_nLoadStage1Case = 0;	// 사이클스탑이면 종료.
		}
		if (gData.nLoadTrayCount[nLs1AviPort-1] <= gData.nTrayUseCount[nLs1AviPort-1] && !gData.bCycleStop) {
			if ((nLs1WorkPort == 1 && !m_pDX00->iLoadPort1LowCheck) || (nLs1WorkPort == 2 && !m_pDX01->iLoadPort2LowCheck)) break;
			//if (gData.bTempLot[nLs1AviPort-1]) return TRUE;

			if (nLs1WorkPort == 1 && !g_objCommon.Check_Position(AX_LOAD_STAGE1_X, 0)) g_objCommon.Move_Position(AX_LOAD_STAGE1_X, 0);	// Port1
			if (nLs1WorkPort == 2 && !g_objCommon.Check_Position(AX_LOAD_STAGE1_X, 1)) g_objCommon.Move_Position(AX_LOAD_STAGE1_X, 1);	// Port2
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(5000);
		} 
		break;
	case 3:		// Check Position, Lot Start
		if ((nLs1WorkPort == 1 && g_objCommon.Check_Position(AX_LOAD_STAGE1_X, 0)) || 
			(nLs1WorkPort == 2 && g_objCommon.Check_Position(AX_LOAD_STAGE1_X, 1))) {
			m_dwLoadStage1 = GetTickCount();
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(30000);
		} 
		break;
	case 4:		// Tray Z Move to Support Up Position (Load)
		if (g_objCommon.Get_LoadTrayMasterSlaveOut(1) && !m_pDX04->iLoadStage1Exist && m_pDX00->iLoadPort1SlideClose && m_pDX01->iLoadPort2SlideClose) {
			m_tLoadStage1Loop.Takt_Start();
			gData.nLPNo = nLs1WorkPort;
			g_dlgWork.Enable_UserInput(nLs1WorkPort, FALSE);
			g_objCommon.Move_Position(AX_LOAD_STAGE1_Z, 2);	// Support Up
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(10000);
		}
		break;
	case 5:		// Port Support Out
		if (g_objCommon.Check_Position(AX_LOAD_STAGE1_Z, 2)) {
			m_tLoadStage1Loop.Takt_Save(2, 1);
			m_tLoadStage1Loop.Takt_Start();
			g_objCommon.Set_LoadPortSupportOut(nLs1WorkPort);
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 6:		// Tray Z Move to Support Down Position
		if (g_objCommon.Get_LoadPortSupportOut(nLs1WorkPort)) {
			m_tLoadStage1Loop.Takt_Save(2, 2);
			m_tLoadStage1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_LOAD_STAGE1_Z, 3);	// Support Down
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(10000);
		}
		break;
	case 7:		// Port Support In
		if (g_objCommon.Check_Position(AX_LOAD_STAGE1_Z, 3)) {
			m_tLoadStage1Loop.Takt_Save(2, 3);
			m_tLoadStage1Loop.Takt_Start();
			if (m_pEquipData->bUseInlineMode) {
				gData.nPNoLoadTray[0] = gData.nPNoLoadPort;
				gData.nTNoLoadTray[0] = gData.nTNoLoadPort;
				gData.nLoadTrayCount[nLs1AviPort-1] = gData.nTNoLoadPort;
				gData.nCmCntLoadStage[0] = gData.nCmCntLoadPort;


				//PC Information Log 파일 생성
				CString strLog, strCPU, strRAM, strHardDisk;
				g_objCommon.Get_CPUInfo(strCPU);
				g_objCommon.Get_RAMSize(strRAM);
				g_objCommon.Get_HardInfo(strHardDisk);

				strLog.Format("%s,%s,%s,%s,%s",gData.sLotID[gData.nPNoLoadTray[0]-1],gData.sRecipeName, strCPU,strRAM,strHardDisk);
				g_objLogFile.Save_PCLog(gData.nPNoLoadTray[0], strLog);

			} else {
				//gData.nLoadTrayCount[nLs1WorkPort-1]++;
				gData.nPNoLoadTray[0] = nLs1WorkPort;
				//gData.nTNoLoadTray[0] = gData.nLoadTrayCount[nLs1WorkPort-1];
			}
			g_objCommon.Set_LoadPortSupportIn(nLs1WorkPort);
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 8:		// Tray Z Move to Move Up Position
		if (g_objCommon.Get_LoadPortSupportIn(nLs1WorkPort) && m_pDX04->iLoadStage1Exist) {
			m_tLoadStage1Loop.Takt_Save(2, 4);
			m_tLoadStage1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_LOAD_STAGE1_Z, 1);	// Move Up (Work)
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(10000);
		}
		break;
	case 9:		// Tray Master In
		if (g_objCommon.Check_Position(AX_LOAD_STAGE1_Z, 1) && m_pDX04->iLoadStage1Exist) {
			m_tLoadStage1Loop.Takt_Save(2, 5);
			m_tLoadStage1Loop.Takt_Start();

			m_bLoadPortTrayExist = FALSE;	// Port에 Tray가 없다고 알려준다.

			m_pDY04->oLoadStage1MasterIn = TRUE;
			g_objAJinAXL.Write_Output(4);
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 10:	// Tray Slave In
		if (m_pDX04->iLoadStage1MasterIn && !m_pDX04->iLoadStage1MasterOut) {
			m_pDY04->oLoadStage1SlaveIn = TRUE;
			g_objAJinAXL.Write_Output(4);
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 11:	// Check Tray Master/Slave In
		if (g_objCommon.Get_LoadTrayMasterSlaveIn(1) && m_pDX04->iLoadStage1Exist) {
			m_tLoadStage1Loop.Takt_Save(2, 6);
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 12:	// 안전확인
		if (m_nLoadStage2Case >= 23) {
			m_nLoadStage1Case = 15; m_tLoadStage1Loop.Set_LoopTime(5000);

		} else if (nLs1WorkPort == 1 && m_nLoadStage2Case == 20 && m_pEquipData->bSortDirRight) {
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(5000);
		}
		return TRUE;
	case 13:	// X Move to Load2 Position
		if (g_objCommon.Check_Position(AX_LOAD_STAGE1_Z, 1)) {
			g_objCommon.Move_Position(AX_LOAD_STAGE1_X, 1);	// Port2
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 14:	// Position Check
		if (g_objCommon.Check_Position(AX_LOAD_STAGE1_X, 1)) {
			if (m_nLoadStage2Case > 22) {
				m_nLoadStage1Case = 15; m_tLoadStage1Loop.Set_LoopTime(5000);
			}
			return TRUE;			
		}
		break;

	case 15:	// Tray X Move to Work Position, Tray Init
		if (g_objCommon.Check_Position(AX_LOAD_STAGE1_Z, 1)) {
			m_tLoadStage1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_LOAD_STAGE1_X, 2);	// Work
			if (!m_pEquipData->bUseInlineMode) {
				gData.nLoadTrayCount[nLs1WorkPort-1]++;
				gData.nTNoLoadTray[0] = gData.nLoadTrayCount[nLs1WorkPort-1];
			}
			Init_LoadTray(nLs1AviPort, 1);
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(20000);
		}
		break;
	case 16:	// Position Check
		if (g_objCommon.Check_Position(AX_LOAD_STAGE1_X, 2)) {
			m_tLoadStage1Loop.Takt_Save(2, 7);
			if(gData.nLoadTrayCount[nLs1AviPort-1] == 1) {
				g_dlgWork.Enable_UserInput(nLs1WorkPort, FALSE);
				Job_LotStart(nLs1AviPort);
				m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(30000);

			} else {
				m_nLoadStage1Case = 20; m_tLoadStage1Loop.Set_LoopTime(10000);
			}
		}
		break;
	case 17:	// Check Lot Ready
		if (g_objInspector.Check_LotReady()) {
			m_nLoadStage1Case = 20; m_tLoadStage1Loop.Set_LoopTime(5000);
		}
		break;

	case 20:	// Working
		return TRUE;


	case 21:	// 안전 확인 
		if (m_nLoadStage2Case > 51 || m_nLoadStage2Case <= 20) {
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(5000);
		}
		return TRUE;
	case 22:	// X Move to Unload Port Position
		if (g_objCommon.Check_Position(AX_LOAD_STAGE1_Z, 1)) {
			m_tLoadStage1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_LOAD_STAGE1_X, 3);
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(10000);
		}
		break;
	case 23:	// Z Move to Unload Up Position
		if (g_objCommon.Check_Position(AX_LOAD_STAGE1_X, 3)  && m_pDX01->iLoadPort3SlideClose) {
			m_tLoadStage1Loop.Takt_Save(2, 8);
			m_tLoadStage1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_LOAD_STAGE1_Z, 4);	// Unload Up
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(10000);
		}
		break;
	case 24:	// Slave Out
		if (g_objCommon.Check_Position(AX_LOAD_STAGE1_Z, 4)) {
			m_tLoadStage1Loop.Takt_Save(2, 9);
			m_tLoadStage1Loop.Takt_Start();
			m_pDY04->oLoadStage1SlaveIn = FALSE;
			g_objAJinAXL.Write_Output(4);
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 25:	// Master Out
		if (!m_pDX04->iLoadStage1SlaveIn && m_pDX04->iLoadStage1SlaveOut) {
			m_pDY04->oLoadStage1MasterIn = FALSE;
			g_objAJinAXL.Write_Output(4);
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 26:	// Z Axis Move to Move Down Position
		if (g_objCommon.Get_LoadTrayMasterSlaveOut(1)) {
			m_tLoadStage1Loop.Takt_Save(2, 10);
			m_tLoadStage1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_LOAD_STAGE1_Z, 0);	// Move Down
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(10000);
		}
		break;
	case 27:	// Position Check
		if (g_objCommon.Check_Position(AX_LOAD_STAGE1_Z, 0)) {
			m_tLoadStage1Loop.Takt_Save(2, 11);
			m_nLoadStage1Case = 50; m_tLoadStage1Loop.Set_LoopTime(5000);

			m_strLog.Format("LoadTray1, %d", GetTickCount() - m_dwLoadStage1);
			g_objLogFile.Save_TestLog(m_strLog);
		}
		break;


	case 50:	//	Interlock
		if (m_nLoadStage2Case > 11 && m_nLoadStage2Case < 50) {
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(5000);
		}
		return TRUE;

	case 51:	// Tray X Move to Load1 Position
		if (g_objCommon.Check_Position(AX_LOAD_STAGE1_Z, 0)) {
			if (g_objCommon.Check_Position(AX_LOAD_STAGE2_Z, 0)) break;	// 인터락
			m_tLoadStage1Loop.Takt_Start();
			if (m_pEquipData->bUseInlineMode) {
				nLs1WorkPort = 1;
			} else {
				//앞에 작업 중인 Port 번호를 받는다.
				if (nLs1WorkPort == 0) nLs1WorkPort = gData.nPNoLoadTray[1];
				if (nLs1AviPort == 0) nLs1AviPort = gData.nPNoLoadTray[1];
			}

			if (nLs1WorkPort == 1) { g_objCommon.Move_Position(AX_LOAD_STAGE1_X, 0); }	// Port1
			if (nLs1WorkPort == 2) { g_objCommon.Move_Position(AX_LOAD_STAGE1_X, 1); }	// Port2

			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(10000);
		}
		break;
	case 52:	// Check End or Go
		if ((nLs1WorkPort == 1 && g_objCommon.Check_Position(AX_LOAD_STAGE1_X, 0)) ||
			(nLs1WorkPort == 2 && g_objCommon.Check_Position(AX_LOAD_STAGE1_X, 1)))
		{
			m_tLoadStage1Loop.Takt_Save(2, 12);
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(10000);			
		}
		break;
	case 53:	// 안전확인
		if (m_nLoadStage2Case >= 20) {
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(10000);
		}
		return TRUE;
	case 54:	// Z Move to Move Up Position 
		if ((nLs1WorkPort == 1 && g_objCommon.Check_Position(AX_LOAD_STAGE1_X, 0)) ||
			(nLs1WorkPort == 2 && g_objCommon.Check_Position(AX_LOAD_STAGE1_X, 1)))
		{
			g_objCommon.Move_Position(AX_LOAD_STAGE1_Z, 1);	// Move Up
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(10000);
		}
		break;
	case 55:	//
		if (g_objCommon.Check_Position(AX_LOAD_STAGE1_Z, 1)) {
			if (m_pEquipData->bUseInlineMode) {
				// 인라인 모드는 무조건 Port 1번으로 가고 case도 1번에서 새로 시작한다.
				m_nLoadStage1Case = 1; m_tLoadStage1Loop.Set_LoopTime(10000);

			} else {
				// 단독 모드는 연속랏이 되어야하기 때문에 Port확인 해준다.
				if (gData.nLoadTrayCount[nLs1WorkPort-1] >= gData.nTrayUseCount[nLs1WorkPort-1]) {
					m_bLotLoadEnable[nLs1WorkPort-1] = TRUE;	// Port에서 트레이 로드 완료
// 					m_bLoadLotEnd = TRUE;	//temp 연속랏은 좀더 생각해보자.
					if (gData.nTNoLoadTray[0] >= gData.nTrayUseCount[nLs1WorkPort-1]) gData.nLoadTrayCount[nLs1WorkPort-1] = 0;

					if (nLs1WorkPort == 1) { 
						m_pDY00->oLoadPort1SlideLock = FALSE; m_pDY00->oLoadPort1SlideUnlock = TRUE; 
						//m_pDY13->oDoor02Unlock = TRUE; 
						nLs1WorkPort = 2;
						nLs1AviPort = 2;

					} else if (nLs1WorkPort == 2) {
						m_pDY01->oLoadPort2SlideLock = FALSE; m_pDY01->oLoadPort2SlideUnlock = TRUE; 
						//m_pDY13->oDoor03Unlock = TRUE; 
						nLs1WorkPort = 1;
						nLs1AviPort = 1;
					}
					g_objAJinAXL.Write_Output(0);
					g_objAJinAXL.Write_Output(1);

					m_nLoadStage1Case = 60; m_tLoadStage1Loop.Set_LoopTime(10000);	//temp 연속랏 고민좀 해봐야한다.
// 					m_nLoadStage1Case = 0; m_tLoadStage1Loop.Set_LoopTime(10000);

				} else {
					m_nLoadStage1Case = 2; m_tLoadStage1Loop.Set_LoopTime(10000);
				}
			}
		}
		break;
	case 60:	// Wait, Port Select
		if (gData.nLoadTrayCount[nLs1WorkPort-1] == 0 && !Check_LoadTrayLoading(nLs1WorkPort)) return TRUE;	// 같은 Port에서 연속으로 2Lot은 검사할 수 없다.

		if ((nLs1WorkPort == 1 && m_pDX00->iLoadPort1LowCheck && m_bLotLoadEnable[0] == FALSE) ||
			(nLs1WorkPort == 2 && m_pDX01->iLoadPort2LowCheck && m_bLotLoadEnable[1] == FALSE))
		{
			g_dlgWork.Get_LotInfo(nLs1WorkPort);
			m_nLoadStage1Case = 2; m_tLoadStage1Loop.Set_LoopTime(10000);

		} else {
			if (m_bUnloadLotEnd) {
				m_nLoadStage1Case = 0; m_tLoadStage1Loop.Set_LoopTime(10000);	// 작업 종료
			}
		}		
		return TRUE;

	}

	// 2. (Error : 3200)
	if (m_tLoadStage1Loop.Over_LoopTime()) {
		g_objCommon.Show_Error(3200 + m_nLoadStage1Case);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 3. (Error : 3300)
BOOL CSequenceMain::LoadStage2_Run()
{
	static int nLs2WorkPort = 0;		// 작업중인 Port (1 ~ 2)
	static int nLs2AviPort = 0;			// Inline Mode 일때 Avi Port 번호 받는다.

	if (gData.bLoadPort3Wait && m_nLoadStage2Case > 22 && m_nLoadStage2Case < 50) return TRUE;

	if (!m_pEquipData->bUseInlineMode) {
		// LJH 1번 투입시 2번에 Tray 있으면 가져가야한다...
		// Load Tray Lot 투입시 도어락, 버튼 확인하여 Tray Loading 작업 못하게 막아준다.
		if (((m_bLotLoadEnable[0] == TRUE && ((m_pEquipData->bUseDoorLock && m_pDX13->iDoor01Unlock) || !m_pEquipData->bUseDoorLock) && (nLs2WorkPort == 1 && m_nLoadStage2Case > 3 && m_nLoadStage2Case < 10)) ||
			 (m_bLotLoadEnable[1] == TRUE && ((m_pEquipData->bUseDoorLock && m_pDX13->iDoor02Unlock) || !m_pEquipData->bUseDoorLock) && (nLs2WorkPort == 2 && m_nLoadStage2Case > 3 && m_nLoadStage2Case < 10)))) return TRUE;
	}

	switch (m_nLoadStage2Case) {
	case 0:		// Start 시 1로 진행
		nLs2WorkPort = 0;
		nLs2AviPort = 0;
		return TRUE;

	case 1:		// Load Tray, Port 확인.
		if (g_objCommon.Check_Position(AX_LOAD_STAGE2_Z, 1)) {	// 안전 확인
			if (m_bUnloadLotEnd || gData.bCycleStop) {
				nLs2WorkPort = 0;
				m_nLoadStage2Case = 0;	// 사이클스탑이면 종료.

			} else {
				if (m_pEquipData->bUseInlineMode) {
					// Inline Mode
					// Tray, Tray Picker Check
					if (m_pDX00->iLoadPort1LowCheck && !gData.bCycleStop && m_bLoadPortTrayExist &&
						(m_nTrayPickCase < 12 || m_nTrayPickCase > 15))
					{
						nLs2WorkPort = 1;
						nLs2AviPort = gData.nPNoLoadPort;
						m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(5000);
					}
					return TRUE;
				} else {
					// 단독 모드
					if (m_bLoadLotEnd) {
						m_nLoadStage2Case = 0;

					} else if ((m_pDX00->iLoadPort1LowCheck || m_pDX01->iLoadPort2LowCheck) && !gData.bCycleStop) {
						// 맨처음 시작할때 2개 모두 적재 되어있으면 1번부터 가져간다.
						// 작업할 포트에 트레이 유무와 입력수량확인.
						if ((nLs2WorkPort == 0 || nLs2WorkPort == 2) && m_pDX00->iLoadPort1LowCheck && (gData.nCmUseCount[0] > 0)) { nLs2WorkPort = 1; nLs2AviPort = 1; }
						else if ((nLs2WorkPort == 0 || nLs2WorkPort == 1) && m_pDX01->iLoadPort2LowCheck && (gData.nCmUseCount[1] > 0)) { nLs2WorkPort = 2; nLs2AviPort = 2; }

						if (!Check_LoadTrayLoading(nLs2WorkPort)) return TRUE;	// 같은 Port Lot을 동시에 검사하지 않는다.
						m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(5000);
					}
				}
			}
		}
		break;
	case 2:		// Tray X Move to Load Position
		if (gData.bCycleStop) {
			nLs2WorkPort = 0;
			nLs2AviPort = 0;
			m_nLoadStage2Case = 0;	// 사이클스탑이면 종료.
		}
		if (gData.nLoadTrayCount[nLs2AviPort-1] <= gData.nTrayUseCount[nLs2AviPort-1] && !gData.bCycleStop) {
			if ((nLs2WorkPort == 1 && !m_pDX00->iLoadPort1LowCheck) || (nLs2WorkPort == 2 && !m_pDX01->iLoadPort2LowCheck)) break;
			//if (gData.bTempLot[nLs2AviPort-1]) return TRUE;

			if (nLs2WorkPort == 1 && !g_objCommon.Check_Position(AX_LOAD_STAGE2_X, 0)) g_objCommon.Move_Position(AX_LOAD_STAGE2_X, 0);	// Port1
			if (nLs2WorkPort == 2 && !g_objCommon.Check_Position(AX_LOAD_STAGE2_X, 1)) g_objCommon.Move_Position(AX_LOAD_STAGE2_X, 1);	// Port2
			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(5000);
		} 
		break;
	case 3:		// Check Position, Lot Start
		if ((nLs2WorkPort == 1 && g_objCommon.Check_Position(AX_LOAD_STAGE2_X, 0)) || 
			(nLs2WorkPort == 2 && g_objCommon.Check_Position(AX_LOAD_STAGE2_X, 1)))
		{
			m_dwLoadStage2 = GetTickCount();
			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(30000);
		} 
		break;
	case 4:		// Tray Z Move to Support Up Position (Load)
		if (g_objCommon.Get_LoadTrayMasterSlaveOut(2) && !m_pDX04->iLoadStage2Exist && m_pDX00->iLoadPort1SlideClose && m_pDX01->iLoadPort2SlideClose) {
			m_tLoadStage2Loop.Takt_Start();
			gData.nLPNo = nLs2WorkPort;
			g_dlgWork.Enable_UserInput(nLs2WorkPort, FALSE);
			g_objCommon.Move_Position(AX_LOAD_STAGE2_Z, 2);	// Support Up
			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(10000);
		}
		break;
	case 5:		// Port Support Out
		if (g_objCommon.Check_Position(AX_LOAD_STAGE2_Z, 2)) {
			m_tLoadStage2Loop.Takt_Save(3, 1);
			m_tLoadStage2Loop.Takt_Start();
			g_objCommon.Set_LoadPortSupportOut(nLs2WorkPort);
			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 6:		// Tray Z Move to Support Down Position
		if (g_objCommon.Get_LoadPortSupportOut(nLs2WorkPort)) {
			m_tLoadStage2Loop.Takt_Save(3, 2);
			m_tLoadStage2Loop.Takt_Start();
			g_objCommon.Move_Position(AX_LOAD_STAGE2_Z, 3);	// Support Down
			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(10000);
		}
		break;
	case 7:		// 정보전달, Port Support In
		if (g_objCommon.Check_Position(AX_LOAD_STAGE2_Z, 3)) {
			m_tLoadStage2Loop.Takt_Save(3, 3);
			m_tLoadStage2Loop.Takt_Start();
			if (m_pEquipData->bUseInlineMode) {
				gData.nPNoLoadTray[1] = gData.nPNoLoadPort;
				gData.nTNoLoadTray[1] = gData.nTNoLoadPort;
				gData.nLoadTrayCount[nLs2AviPort-1] = gData.nTNoLoadPort;
				gData.nCmCntLoadStage[1] = gData.nCmCntLoadPort;
				
				//PC Information Log 파일 생성
				CString strLog, strCPU, strRAM, strHardDisk;
				g_objCommon.Get_CPUInfo(strCPU);
				g_objCommon.Get_RAMSize(strRAM);
				g_objCommon.Get_HardInfo(strHardDisk);

				strLog.Format("%s,%s,%s,%s,%s",gData.sLotID[gData.nPNoLoadTray[1]-1],gData.sRecipeName, strCPU,strRAM,strHardDisk);
				g_objLogFile.Save_PCLog(gData.nPNoLoadTray[1], strLog);

			} else {
				//gData.nLoadTrayCount[nLs2WorkPort-1]++;
				gData.nPNoLoadTray[1] = nLs2WorkPort;
				//gData.nTNoLoadTray[1] = gData.nLoadTrayCount[nLs2WorkPort-1];
			}
			g_objCommon.Set_LoadPortSupportIn(nLs2WorkPort);
			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 8:		// Tray Z Move to Move Up Position
		if (g_objCommon.Get_LoadPortSupportIn(nLs2WorkPort) && m_pDX04->iLoadStage2Exist) {
			m_tLoadStage2Loop.Takt_Save(3, 4);
			m_tLoadStage2Loop.Takt_Start();
			g_objCommon.Move_Position(AX_LOAD_STAGE2_Z, 1);	// Move Up (Work)
			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(10000);
		}
		break;
	case 9:		// Tray Master In
		if (g_objCommon.Check_Position(AX_LOAD_STAGE2_Z, 1) && m_pDX04->iLoadStage2Exist) {
			m_tLoadStage2Loop.Takt_Save(3, 5);
			m_tLoadStage2Loop.Takt_Start();

			m_bLoadPortTrayExist = FALSE;	// Port에 Tray가 없다고 알려준다.

			m_pDY04->oLoadStage2MasterIn = TRUE;
			g_objAJinAXL.Write_Output(4);
			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 10:	// Tray Slave In
		if (m_pDX04->iLoadStage2MasterIn && !m_pDX04->iLoadStage2MasterOut) {
			m_pDY04->oLoadStage2SlaveIn = TRUE;
			g_objAJinAXL.Write_Output(4);
			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 11:	// Check Tray Master/Slave In
		if (g_objCommon.Get_LoadTrayMasterSlaveIn(2) && m_pDX04->iLoadStage2Exist) {
			m_tLoadStage2Loop.Takt_Save(3, 6);
			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 12:	// 안전확인
		if (m_nLoadStage1Case >= 23) {
			m_nLoadStage2Case = 15; m_tLoadStage2Loop.Set_LoopTime(5000);

		} else if (nLs2WorkPort == 1 && m_nLoadStage1Case == 20 && m_pEquipData->bSortDirRight) {
			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(5000);
		}
		return TRUE;
	case 13:	// X Move to Load2 Position
		if (g_objCommon.Check_Position(AX_LOAD_STAGE2_Z, 1)) {
			g_objCommon.Move_Position(AX_LOAD_STAGE2_X, 1);	// Port2
			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 14:	// Position Check
		if (g_objCommon.Check_Position(AX_LOAD_STAGE2_X, 1)) {
			if (m_nLoadStage1Case > 22) {
				m_nLoadStage2Case = 15; m_tLoadStage2Loop.Set_LoopTime(5000);
			}
			return TRUE;			
		}
		break;

	case 15:	// Tray X Move to Work Position, Tray Init
		if (g_objCommon.Check_Position(AX_LOAD_STAGE2_Z, 1)) {
			m_tLoadStage2Loop.Takt_Start();
			g_objCommon.Move_Position(AX_LOAD_STAGE2_X, 2);	// Work
			if (!m_pEquipData->bUseInlineMode) {
				gData.nLoadTrayCount[nLs2WorkPort-1]++;
				gData.nTNoLoadTray[1] = gData.nLoadTrayCount[nLs2WorkPort-1];
			}
			Init_LoadTray(nLs2AviPort, 2);
			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(20000);
		}
		break;
	case 16:	// Position Check
		if (g_objCommon.Check_Position(AX_LOAD_STAGE2_X, 2)) {
			m_tLoadStage2Loop.Takt_Save(3, 7);
			if(gData.nLoadTrayCount[nLs2AviPort-1] == 1) {
				g_dlgWork.Enable_UserInput(nLs2WorkPort, FALSE);
				Job_LotStart(nLs2AviPort);
				m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(30000);

			} else {
				m_nLoadStage2Case = 20; m_tLoadStage2Loop.Set_LoopTime(10000);
			}
		}
		break;
	case 17:	// Check Lot Ready
		if (g_objInspector.Check_LotReady()) {
			m_nLoadStage2Case = 20; m_tLoadStage2Loop.Set_LoopTime(5000);
		}
		break;

	case 20:	// Working
		return TRUE;


	case 21:	// 안전 확인 
		if (m_nLoadStage1Case > 51 || m_nLoadStage1Case <= 20) {
			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(5000);
		}
		return TRUE;
	case 22:	// X Move to Unload Port Position
		if (g_objCommon.Check_Position(AX_LOAD_STAGE2_Z, 1)) {
			m_tLoadStage2Loop.Takt_Start();
			g_objCommon.Move_Position(AX_LOAD_STAGE2_X, 3);
			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(10000);
		}
		break;
	case 23:	// Z Move to Unload Up Position
		if (g_objCommon.Check_Position(AX_LOAD_STAGE2_X, 3) && m_pDX01->iLoadPort3SlideClose) {
			m_tLoadStage2Loop.Takt_Save(3, 8);
			m_tLoadStage2Loop.Takt_Start();
			g_objCommon.Move_Position(AX_LOAD_STAGE2_Z, 4);	// Unload Up
			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(10000);
		}
		break;
	case 24:	// Slave Out
		if (g_objCommon.Check_Position(AX_LOAD_STAGE2_Z, 4)) {
			m_tLoadStage2Loop.Takt_Save(3, 9);
			m_tLoadStage2Loop.Takt_Start();
			m_pDY04->oLoadStage2SlaveIn = FALSE;
			g_objAJinAXL.Write_Output(4);
			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 25:	// Master Out
		if (!m_pDX04->iLoadStage2SlaveIn && m_pDX04->iLoadStage2SlaveOut) {
			m_pDY04->oLoadStage2MasterIn = FALSE;
			g_objAJinAXL.Write_Output(4);
			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 26:	// Z Axis Move to Move Down Position
		if (g_objCommon.Get_LoadTrayMasterSlaveOut(2)) {
			m_tLoadStage2Loop.Takt_Save(3, 10);
			m_tLoadStage2Loop.Takt_Start();
			g_objCommon.Move_Position(AX_LOAD_STAGE2_Z, 0);	// Move Down
			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(10000);
		}
		break;
	case 27:	// Position Check
		if (g_objCommon.Check_Position(AX_LOAD_STAGE2_Z, 0)) {
			m_tLoadStage2Loop.Takt_Save(3, 11);
			m_nLoadStage2Case = 50; m_tLoadStage2Loop.Set_LoopTime(5000);

			m_strLog.Format("LoadTray2, %d", GetTickCount() - m_dwLoadStage2);
			g_objLogFile.Save_TestLog(m_strLog);
		}
		break;


	case 50:	//	Interlock
		if (m_nLoadStage1Case > 11 && m_nLoadStage1Case < 50) {
			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(5000);
		}
		return TRUE;

	case 51:	// Tray X Move to Load1 Position
		if (g_objCommon.Check_Position(AX_LOAD_STAGE2_Z, 0)) {
			if (g_objCommon.Check_Position(AX_LOAD_STAGE1_Z, 0)) break;	// 인터락
			m_tLoadStage2Loop.Takt_Start();
			if (m_pEquipData->bUseInlineMode) {
				nLs2WorkPort = 1;
			} else {
				//앞에 작업 중인 Port 번호를 받는다.
				if (nLs2WorkPort == 0) nLs2WorkPort = gData.nPNoLoadTray[0];
				if (nLs2AviPort == 0) nLs2AviPort = gData.nPNoLoadTray[0];
			}

			if (nLs2WorkPort == 1) { g_objCommon.Move_Position(AX_LOAD_STAGE2_X, 0); }	// Port1
			if (nLs2WorkPort == 2) { g_objCommon.Move_Position(AX_LOAD_STAGE2_X, 1); }	// Port2

			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(10000);
		}
		break;
	case 52:	// Check End or Go
		if ((nLs2WorkPort == 1 && g_objCommon.Check_Position(AX_LOAD_STAGE2_X, 0)) ||
			(nLs2WorkPort == 2 && g_objCommon.Check_Position(AX_LOAD_STAGE2_X, 1)))
		{
			m_tLoadStage2Loop.Takt_Save(3, 12);
			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(10000);			
		}
		break;
	case 53:	// 안전확인
		if (m_nLoadStage1Case >= 20) {
			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(10000);
		}
		return TRUE;
	case 54:	// Z Move to Move Up Position 
		if ((nLs2WorkPort == 1 && g_objCommon.Check_Position(AX_LOAD_STAGE2_X, 0)) ||
			(nLs2WorkPort == 2 && g_objCommon.Check_Position(AX_LOAD_STAGE2_X, 1)))
		{
			g_objCommon.Move_Position(AX_LOAD_STAGE2_Z, 1);	// Move Up
			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(10000);
		}
		break;
	case 55:	//
		if (g_objCommon.Check_Position(AX_LOAD_STAGE2_Z, 1)) {
			if (m_pEquipData->bUseInlineMode) {
				// 인라인 모드는 무조건 Port 1번으로 가고 case도 1번에서 새로 시작한다.
				m_nLoadStage2Case = 1; m_tLoadStage2Loop.Set_LoopTime(10000);

			} else {
				// 단독 모드는 연속랏이 되어야하기 때문에 Port확인 해준다.
				if (gData.nLoadTrayCount[nLs2WorkPort-1] >= gData.nTrayUseCount[nLs2WorkPort-1]) {
					m_bLotLoadEnable[nLs2WorkPort-1] = TRUE;	// Port에서 트레이 로드 완료
// 					m_bLoadLotEnd = TRUE;	//temp 연속랏은 좀더 생각해보자.

					if (gData.nTNoLoadTray[1] >= gData.nTrayUseCount[nLs2WorkPort-1]) gData.nLoadTrayCount[nLs2WorkPort-1] = 0;

					if (nLs2WorkPort == 1) { 
						m_pDY00->oLoadPort1SlideLock = FALSE; m_pDY00->oLoadPort1SlideUnlock = TRUE; 
						//m_pDY13->oDoor02Unlock = TRUE; 
						nLs2WorkPort = 2;
						nLs2AviPort = 2;

					} else if (nLs2WorkPort == 2) {
						m_pDY01->oLoadPort2SlideLock = FALSE; m_pDY01->oLoadPort2SlideUnlock = TRUE; 
						//m_pDY13->oDoor03Unlock = TRUE; 
						nLs2WorkPort = 1;
						nLs2AviPort = 1;
					}
					g_objAJinAXL.Write_Output(0);
					g_objAJinAXL.Write_Output(1);

					m_nLoadStage2Case = 60; m_tLoadStage2Loop.Set_LoopTime(10000);
// 					m_nLoadStage2Case = 0; m_tLoadStage2Loop.Set_LoopTime(10000);
				} else {
					m_nLoadStage2Case = 2; m_tLoadStage2Loop.Set_LoopTime(10000);
				}
			}
		}
		break;
	case 60:	// Wait, Port Select
		if (gData.nLoadTrayCount[nLs2WorkPort-1] == 0 && !Check_LoadTrayLoading(nLs2WorkPort)) return TRUE;	// 같은 Port에서 연속으로 2Lot은 검사할 수 없다.

		if ((nLs2WorkPort == 1 && m_pDX00->iLoadPort1LowCheck && m_bLotLoadEnable[0] == FALSE) ||
			(nLs2WorkPort == 2 && m_pDX01->iLoadPort2LowCheck && m_bLotLoadEnable[1] == FALSE))
		{
			g_dlgWork.Get_LotInfo(nLs2WorkPort);
			m_nLoadStage2Case = 2; m_tLoadStage2Loop.Set_LoopTime(10000);

		} else {
			if (m_bUnloadLotEnd) {
				m_nLoadStage2Case = 0; m_tLoadStage2Loop.Set_LoopTime(10000);	// 작업 종료
			}
		}		
		return TRUE;

	}

	// 3. (Error : 3300)
	if (m_tLoadStage2Loop.Over_LoopTime()) {
		g_objCommon.Show_Error(3300 + m_nLoadStage2Case);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 4. (Error : 3400)
BOOL CSequenceMain::LoadPicker_Run()
{
	static int	nLpWorkTray = 0;
	static int	nLpTrayPosY = 0;
	static int	nLpTrayPosX = 0;

	static double	dLpY = 0.0;
	static double	dLpX = 0.0;

	static int nLpStart = 0;		// Picker Start No
	static int nLpPickCnt = 0;		// Picker가 잡을수 있는 수
	static int nLpDownSu = 0;		// 최종적으로 Picker가 Down되는 수
	static int nLpTrayCnt = 0;		// Tray Cnt
	

	switch (m_nLoadPickCase) {
	case 0:		// Tray가 Work 위치에 오면 시작
		if (/*m_nMainIndexCase <= 10 && */!gData.IndexDone[0]) {
			if (m_nLoadStage1Case == 20 || m_nLoadStage2Case == 20) {
				m_nLoadPickCase++; m_tLoadPickLoop.Set_LoopTime(5000);
			}
			if (m_pEquipData->bUseInlineMode) {
				// Index에 모듈이 있을때 Tray가 안넘어오면 Index는 돌아가게 해준다.
				
				if (!Check_IndexEmpty(-1) && !gData.IndexDone[0] && m_nVisCmAlignCase  == 0 &&
					(((m_nLoadStage1Case == 0 || m_nLoadStage1Case == 1) && m_nLoadStage2Case == 50) ||
					(m_nLoadStage1Case == 50 && (m_nLoadStage2Case == 0 || m_nLoadStage2Case == 1))))
				{
					gData.IndexDone[0] = TRUE;
				}
			} else {
				if (!gData.IndexDone[0] && m_nVisCmAlignCase  == 0 &&
					(((m_nLoadStage1Case == 0 || m_nLoadStage1Case == 60) && m_nLoadStage2Case == 50) ||
					  (m_nLoadStage1Case == 50 && (m_nLoadStage2Case == 0 || m_nLoadStage2Case == 60))) )
				{
					gData.IndexDone[0] = TRUE;
				}
			}
		}
		return TRUE;

	case 1:		// 안전 확인(Grip Open, Cylinder Up, Picker Z Ready Up, Picker Pitch Tray), Y Axis Move to Stage Position
		if (g_objCommon.Get_LoadPickerUp() && g_objCommon.Get_LoadPickerOpen()) {
			if (g_objCommon.Check_Position(AX_LOAD_PICKER_Z, 0) && g_objCommon.Check_Position(AX_LOAD_PICKER_P, 0)) {

				if (m_nLoadStage1Case == 20) { nLpWorkTray = 1; dLpX = m_pMoveData->dLoadStage1X[2]; }
				if (m_nLoadStage2Case == 20) { nLpWorkTray = 2; dLpX = m_pMoveData->dLoadStage2X[2]; }

				dLpY = m_pMoveData->dLoadPickerY[nLpWorkTray-1];	//Stage1/2 Position
				g_objAJinAXL.Move_Absolute(AX_LOAD_PICKER_Y, dLpY);
				m_nLoadPickCase++; m_tLoadPickLoop.Set_LoopTime(10000);
			}
		}
		break;
	case 2:		// X/Y Move to Pickup Position
		if (gData.nPickerUseCnt < 6 && m_pEquipData->bSortDirRight) {
			if (g_objAJinAXL.Is_MoveDone(AX_LOAD_PICKER_Y, dLpY) &&
				((nLpWorkTray == 1 && g_objCommon.Check_Position(AX_LOAD_STAGE1_X, 2)) ||
				(nLpWorkTray == 2 && g_objCommon.Check_Position(AX_LOAD_STAGE2_X, 2))))
			{
				if (Select_LoadTrayPos(nLpTrayPosX, nLpTrayPosY)){
					m_dwLoadPick = GetTickCount();
					m_tLoadPickLoop.Takt_Start();
					dLpY = m_pMoveData->dLoadPickerY[nLpWorkTray-1] - (m_pEquipData->dLoadTrayPitchY * nLpTrayPosY);
					g_objAJinAXL.Move_Absolute(AX_LOAD_PICKER_Y, dLpY);
					m_nLoadPickCase++; m_tLoadPickLoop.Set_LoopTime(10000);
				}
			}
		} else {
			// Picker 6ea or Load Tray 잔량정렬 아래방향
			if (m_nLoadStage1Case != 20 && m_nLoadStage2Case != 20) return TRUE;
// 			if (m_nLoadStage1Case != 20 && m_nLoadStage2Case != 20 &&
// 				((m_nLoadStage1Case > 1 && m_nLoadStage1Case < 20) ||
// 				 (m_nLoadStage2Case > 1 && m_nLoadStage2Case < 20)) )
// 			{
// 				// Tray Loading 하고 있으면 대기 
// 				return TRUE;
// 
// 			} else if (m_nLoadStage1Case != 20 && m_nLoadStage2Case != 20) {
// 				//AVI에서 Tray 안넘어 오고 있으면 case 0으로
// 				m_nLoadPickCase = 0;
// 				return TRUE;
// 			}

			if (g_objAJinAXL.Is_MoveDone(AX_LOAD_PICKER_Y, dLpY) &&
				((m_nLoadStage1Case == 20 && (g_objAJinAXL.Is_MoveDone(AX_LOAD_STAGE1_X, dLpX)|| g_objCommon.Check_Position(AX_LOAD_STAGE1_X, 2))) ||
				 (m_nLoadStage2Case == 20 && (g_objAJinAXL.Is_MoveDone(AX_LOAD_STAGE2_X, dLpX)|| g_objCommon.Check_Position(AX_LOAD_STAGE2_X, 2)))))
			{
				if (m_nLoadStage1Case == 20) nLpWorkTray = 1;
				if (m_nLoadStage2Case == 20) nLpWorkTray = 2;

				if (Select_LoadPickerPos(nLpStart, nLpPickCnt)) {
					if (Select_LoadTrayPos(nLpTrayPosX, nLpTrayPosY)){

						m_dwLoadPick = GetTickCount();
						m_tLoadPickLoop.Takt_Start();
						nLpTrayCnt = m_pEquipData->nLoadTrayX - nLpTrayPosX;
						int nTempDownSu = ((nLpPickCnt < nLpTrayCnt) ? nLpPickCnt : nLpTrayCnt);

						if (m_pEquipData->bSortDirRight) nLpDownSu = nTempDownSu;
						else							 nLpDownSu = g_objCommon.Get_InfoLoadPickerDownCnt(nLpTrayPosY, nLpTrayPosX, nTempDownSu);
						m_nLoadPickMultiCnt = nLpDownSu;

						if (nLpWorkTray == 1) dLpX = m_pMoveData->dLoadStage1X[2] - (nLpTrayPosX - nLpStart) * m_pEquipData->dLoadTrayPitchX;
						if (nLpWorkTray == 2) dLpX = m_pMoveData->dLoadStage2X[2] - (nLpTrayPosX - nLpStart) * m_pEquipData->dLoadTrayPitchX;
						dLpY = m_pMoveData->dLoadPickerY[nLpWorkTray-1] - (m_pEquipData->dLoadTrayPitchY * nLpTrayPosY);

						if (nLpWorkTray == 1) g_objAJinAXL.Move_Absolute(AX_LOAD_STAGE1_X, dLpX);
						if (nLpWorkTray == 2) g_objAJinAXL.Move_Absolute(AX_LOAD_STAGE2_X, dLpX);
						g_objAJinAXL.Move_Absolute(AX_LOAD_PICKER_Y, dLpY);		
						m_nLoadPickCase++; m_tLoadPickLoop.Set_LoopTime(10000);
					}
				}
			}
		}
		break;
	case 3:		// Z Axis Move to Tray Down
		if (gData.nPickerUseCnt < 6 && m_pEquipData->bSortDirRight) {
			if (g_objCommon.Get_LoadPickerOpen() &&
				g_objAJinAXL.Is_MoveDone(AX_LOAD_PICKER_Y, dLpY) && g_objCommon.Check_Position(AX_LOAD_PICKER_P, 0))
			{
				m_tLoadPickLoop.Takt_Save(4, 1);
				m_tLoadPickLoop.Takt_Start();

				g_objCommon.Set_InfoLoadPickerDown(INFO_STAGE, nLpTrayPosY);
				g_objCommon.Move_Position(AX_LOAD_PICKER_Z, nLpWorkTray);
				m_nLoadPickCase++; m_tLoadPickLoop.Set_LoopTime(10000);
			}

		} else {	// Picker 6ea or Load Tray 잔량정렬 아래방향
			if (g_objCommon.Get_InfoLoadPickerGripOpen(INFO_PICK) && g_objAJinAXL.Is_MoveDone(AX_LOAD_PICKER_Y, dLpY) &&
				((nLpWorkTray == 1 && g_objAJinAXL.Is_MoveDone(AX_LOAD_STAGE1_X, dLpX)) ||
				 (nLpWorkTray == 2 && g_objAJinAXL.Is_MoveDone(AX_LOAD_STAGE2_X, dLpX))) &&
				g_objCommon.Check_Position(AX_LOAD_PICKER_P, 0))
			{
				m_tLoadPickLoop.Takt_Save(4, 1);
				m_tLoadPickLoop.Takt_Start();

				g_objCommon.Set_LoadPickerDownMulti(nLpStart+1, nLpDownSu);
				g_objCommon.Move_Position(AX_LOAD_PICKER_Z, nLpWorkTray);
				m_nLoadPickCase++; m_tLoadPickLoop.Set_LoopTime(10000);
			}
		}
		break;
	case 4:		// 정보전달 Picker Grip
		if (g_objCommon.Check_Position(AX_LOAD_PICKER_Z, nLpWorkTray) && 
			(((gData.nPickerUseCnt < 6) && m_pEquipData->bSortDirRight && g_objCommon.Get_InfoLoadPickerDown(INFO_STAGE, nLpTrayPosY)) ||
			 ((gData.nPickerUseCnt == 6 || !m_pEquipData->bSortDirRight) && g_objCommon.Get_LoadPickerDownMulti(nLpStart+1, nLpDownSu))) )
		{
			if (!m_tLoadPickLoop.Waiting_Time(m_pEquipData->nDelayAdd[0])) break;

			m_tLoadPickLoop.Takt_Save(4, 2);
			m_tLoadPickLoop.Takt_Start();
			
			if (gData.nPickerUseCnt < 6 && m_pEquipData->bSortDirRight) {
				for (int i = 0; i < m_pEquipData->nLoadTrayX; i++) {
					gData.InfoLoadPick[i] = gData.InfoLoadTray[nLpTrayPosY][i]; gData.InfoLoadTray[nLpTrayPosY][i] = 0;
					gData.nTNoLoadPick[i] = gData.nTNoLoadTray[nLpWorkTray-1];
					if (gData.InfoLoadPick[i] > 0) {
						gData.nCNoLoadPick[i] = nLpTrayPosY * m_pEquipData->nLoadTrayX + i + 1;

						//모듈트랙킹
						int nPNo = gData.nPNoLoadTray[nLpWorkTray-1] - 1;
						int nTNo = gData.nTNoLoadTray[nLpWorkTray-1] - 1;
						int nCNo = gData.nCNoLoadPick[i] - 1;
						gData.nCmJigNo[nPNo][nTNo][nCNo][LOAD_STAGE] = nLpWorkTray;	// Stage No.
						gData.nCmJigNo[nPNo][nTNo][nCNo][LOAD_PICK]  = i + 1;		// Btm1 Jig No.
					}
				}
				gData.nPNoLoadPick = gData.nPNoLoadTray[nLpWorkTray-1];
				g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 1, nLpWorkTray-1);

			} else {
				// Picker 6ea or Load Tray 잔량정렬 아래방향
				// Single은 연속랏이 아니기 때문에 Port 상관없지만
				// EOL 6면 검사기는 연속랏으로 넘어오기 때문에 Port 구분하여 Lot 혼입 안되게 처리해줘야한다.
				// 피커 6개 사용하는건 현재 Single 밖에 없어 처리하지 않았다. R12A
				for (int i=0; i<nLpDownSu; i++) {
					gData.InfoLoadPick[nLpStart+i] = gData.InfoLoadTray[nLpTrayPosY][nLpTrayPosX+i]; gData.InfoLoadTray[nLpTrayPosY][nLpTrayPosX+i] = 0;
					if (gData.InfoLoadPick[nLpStart+i] > 0) {
						gData.nCNoLoadPick[nLpStart+i] = nLpTrayPosY * m_pEquipData->nLoadTrayX + nLpTrayPosX + i + 1; 
						gData.nTNoLoadPick[nLpStart+i] = gData.nTNoLoadTray[nLpWorkTray-1];

						//모듈트랙킹
						int nPNo = gData.nPNoLoadTray[nLpWorkTray-1] - 1;
						int nTNo = gData.nTNoLoadTray[nLpWorkTray-1] - 1;
						int nCNo = gData.nCNoLoadPick[i] - 1;
						gData.nCmJigNo[nPNo][nTNo][nCNo][LOAD_STAGE] = nLpWorkTray;		 // Stage No.
						gData.nCmJigNo[nPNo][nTNo][nCNo][LOAD_PICK]  = nLpStart + i + 1; // Btm1 Jig No.
					}
				}
				gData.nPNoLoadPick = gData.nPNoLoadTray[nLpWorkTray-1];
				g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 1, gData.nPNoLoadPick-1);
			}
			g_objCommon.Set_InfoLoadPickerGrip(INFO_PICK, nLpTrayPosY);
			m_nLoadPickCase++; m_tLoadPickLoop.Set_LoopTime(5000);			
		}
		break;
	case 5:		// Picker Up
		if (g_objCommon.Get_InfoLoadPickerGrip(INFO_PICK, nLpTrayPosY)) {
			if (!m_tLoadPickLoop.Waiting_Time(m_pEquipData->nDelayAdd[0])) break;

			if (gData.nPickerUseCnt < 6 && m_pEquipData->bSortDirRight) {
				m_tLoadPickLoop.Takt_Save(4, 3);
				m_tLoadPickLoop.Takt_Start();
				g_objCommon.Set_LoadPickerUp();
				g_objCommon.Move_Position(AX_LOAD_PICKER_Z, 0);			
				m_nLoadPickCase++; m_tLoadPickLoop.Set_LoopTime(5000);

			} else {
				// Picker 6ea or Load Tray 잔량정렬 아래방향
				m_tLoadPickLoop.Takt_Save(4, 3);
				m_tLoadPickLoop.Takt_Start();
				g_objCommon.Set_LoadPickerUp();
				m_nLoadPickCase = 7; m_tLoadPickLoop.Set_LoopTime(5000);
			}			
		}
		break;
	case 6:		// CM Check & Tray Empty Check
		if (g_objCommon.Check_Position(AX_LOAD_PICKER_Z, 0) &&
			g_objCommon.Get_LoadPickerUp() && g_objCommon.Get_InfoLoadPickerCmCheck())
		{
			m_tLoadPickLoop.Takt_Save(4, 4);
			if (Check_LoadTrayEmpty()) {
				if (nLpWorkTray == 1) m_nLoadStage1Case = 21;	// Unloading
				if (nLpWorkTray == 2) m_nLoadStage2Case = 21;	// Unloading

				//gData.nTNoLoadTray[nLpWorkTray-1] = 0;
				gData.nPNoLoadTray[nLpWorkTray-1] = 0;
				nLpWorkTray = 0;
			}
			m_nLoadPickCase = 10; m_tLoadPickLoop.Set_LoopTime(5000);
		}
		break;

		
	case 7:	// Picker Check
		if (g_objCommon.Get_LoadPickerUp() && g_objCommon.Get_InfoLoadPickerCmCheck())
		{
			m_tLoadPickLoop.Takt_Save(4, 4);
			if (Check_LoadPickerFull() || Check_LoadTrayEmpty()) {

				if (Check_LoadPickerFull()) {
					g_objCommon.Move_Position(AX_LOAD_PICKER_Z, 0);
					m_nLoadPickCase = 10; m_tLoadPickLoop.Set_LoopTime(5000);
				} else {
					int nPNo = gData.nPNoLoadTray[nLpWorkTray-1];
					if (m_pEquipData->bUseInlineMode) {	// Inline Mode
						// 트레이가 비어있고 마지막 트레이면 Index로 넘어간다.
						if (Check_LoadTrayEmpty() && (gData.nTNoLoadTray[nLpWorkTray-1] == gData.nLastTrayNo[nPNo-1])) {
							g_objCommon.Move_Position(AX_LOAD_PICKER_Z, 0);
							m_nLoadPickCase = 10; m_tLoadPickLoop.Set_LoopTime(5000);
						} else {
							m_nLoadPickCase = 2; m_tLoadPickLoop.Set_LoopTime(5000);
						}
					} else {	// 단독 모드
						// 트레이가 비어있고 마지막 트레이면 Index로 넘어간다.
						if (Check_LoadTrayEmpty() && (gData.nLoadTrayCount[nPNo-1] >= gData.nTrayUseCount[nPNo-1])) {
							g_objCommon.Move_Position(AX_LOAD_PICKER_Z, 0);
							m_nLoadPickCase = 10; m_tLoadPickLoop.Set_LoopTime(5000);
						} else {
							m_nLoadPickCase = 2; m_tLoadPickLoop.Set_LoopTime(5000);
						}
					}
				}

				if (Check_LoadTrayEmpty()) {
					if (nLpWorkTray == 1) m_nLoadStage1Case = 21;	// Unloading
					if (nLpWorkTray == 2) m_nLoadStage2Case = 21;	// Unloading
					int nPNo = gData.nPNoLoadTray[nLpWorkTray-1];

					//gData.nTNoLoadTray[nLpWorkTray-1] = 0;
					gData.nPNoLoadTray[nLpWorkTray-1] = 0;
					nLpWorkTray = 0;
				}
			} else {
				m_nLoadPickCase = 2; m_tLoadPickLoop.Set_LoopTime(5000);				
			}
			
		}
		break;


	case 10:	// Interlock Vision CM
		if (m_nVisCmAlignCase == 0 && g_objCommon.Check_Position(AX_VISION_CM_ALIGN_X, 0)) {
			m_nLoadPickCase++; m_tLoadPickLoop.Set_LoopTime(5000);
		}
		return TRUE;

	case 11:	// Y/P Move to Index Position
		if (g_objCommon.Check_Position(AX_LOAD_PICKER_Z, 0) && g_objCommon.Get_InfoLoadPickerCmCheck())
		{
			m_tLoadPickLoop.Takt_Start();
			g_objCommon.Move_Position(AX_LOAD_PICKER_Y, 2);	// Index Position
			g_objCommon.Move_Position(AX_LOAD_PICKER_P, 1);	// Index Pitch
			m_nLoadPickCase++; m_tLoadPickLoop.Set_LoopTime(5000);
		}
	case 12:	// Position Check
		if (g_objCommon.Get_InfoLoadPickerCmCheck() &&
			g_objCommon.Check_Position(AX_LOAD_PICKER_Y, 2) && g_objCommon.Check_Position(AX_LOAD_PICKER_P, 1))
		{
			m_tLoadPickLoop.Takt_Save(4, 5);
			m_nLoadPickCase = 15; m_tLoadPickLoop.Set_LoopTime(5000);
		}
		break;

	case 15:	// index Check
		if ((m_nMainIndexCase == 2 || m_nMainIndexCase == 5) && !gData.IndexDone[0]) {
			m_nLoadPickCase++; m_tLoadPickLoop.Set_LoopTime(5000);
		}
		return TRUE;

	case 16:	// Index Down
		if (g_objCommon.Get_InfoLoadPickerCmCheck() &&
			g_objCommon.Check_Position(AX_LOAD_PICKER_Y, 2) && g_objCommon.Check_Position(AX_LOAD_PICKER_P, 1) &&
			!m_pDX11->iIndexLoadAlignIn && m_pDX11->iIndexLoadAlignOut)
		{
			m_tLoadPickLoop.Takt_Start();
			g_objCommon.Set_InfoLoadPickerDown(INFO_PICK);
			g_objCommon.Move_Position(AX_LOAD_PICKER_Z, 3);	// Index Down
			m_nLoadPickCase++; m_tLoadPickLoop.Set_LoopTime(10000);
		}
		break;
	case 17:	// 정보전달, Grip Open
		if (g_objCommon.Check_Position(AX_LOAD_PICKER_Z, 3) && g_objCommon.Get_InfoLoadPickerDown(INFO_PICK)) {
			if (!m_tLoadPickLoop.Waiting_Time(100)) break;
			m_tLoadPickLoop.Takt_Save(4, 6);
			m_tLoadPickLoop.Takt_Start();

			for (int i = 0; i < gData.nPickerUseCnt; i++) {
				gData.InfoIndex[0][i] = gData.InfoLoadPick[i]; gData.InfoLoadPick[i] = 0;
				gData.nCNoIndex[0][i] = gData.nCNoLoadPick[i]; gData.nCNoLoadPick[i] = 0;
				gData.nTNoIndex[0][i] = gData.nTNoLoadPick[i]; gData.nTNoLoadPick[i] = 0;

				//모듈트랙킹
				int nPNo = gData.nPNoLoadPick - 1;
				int nTNo = gData.nTNoIndex[0][i] - 1;
				int nCNo = gData.nCNoIndex[0][i] - 1;
				if (nCNo > 0) {
					int nIdxNo = 0;
					nIdxNo = g_objCommon.Get_IndexPos(1);	// Load

					gData.nCmJigNo[nPNo][nTNo][nCNo][INDEX_LOAD_NO]	 = nIdxNo;	// Index No
					//gData.nCmJigNo[nPNo][nTNo][nCNo][INDEX_LOAD_NO]	 = i + 1;	// Index Jig
					gData.nCmJigNo[nPNo][nTNo][nCNo][INDEX_LOAD_JIG] = i + 1;	// Index Jig
				}
			}
			gData.nPNoIndex[0] = gData.nPNoLoadPick; gData.nPNoLoadPick = 0;
			
			g_objCommon.Set_LoadPickerOpen();
			//if (m_pEquipData->bUseIndexLoadVac) g_objCommon.Set_InfoIndexLoadVacuumOn(0);
			g_objCommon.Set_InfoIndexLoadVacuumOn(0);
			m_nLoadPickCase++; m_tLoadPickLoop.Set_LoopTime(5000);
		}
		break;
	case 18:	// Picker Up
		if (g_objCommon.Get_LoadPickerOpen()) {
			if (!m_tLoadPickLoop.Waiting_Time(m_pEquipData->nDelayAdd[0])) break;
			m_tLoadPickLoop.Takt_Save(4, 7);
			m_tLoadPickLoop.Takt_Start();
			g_objCommon.Set_LoadPickerUp();
			g_objCommon.Move_Position(AX_LOAD_PICKER_Z, 0);	// Ready Up
			g_objCommon.Set_IndexLoadVacuumOff(0);

			m_nLoadPickCase++; m_tLoadPickLoop.Set_LoopTime(5000);
		}
		break;
	case 19:	// return
		//if (m_pDX11->iIndexLoadAlignIn && !m_pDX11->iIndexLoadAlignOut) {	//Main Index에서 확인해준다.
			if (g_objCommon.Check_Position(AX_LOAD_PICKER_Z, 0) &&
				g_objCommon.Get_LoadPickerUp() && g_objCommon.Get_LoadPickerCmCheckOff())
			{
				if(!m_tLoadPickLoop.Waiting_Time(300)) break;
				m_tLoadPickLoop.Takt_Save(4, 8);
				m_tLoadPickLoop.Takt_Start();

				if (Select_LoadTrayPos(nLpTrayPosX, nLpTrayPosY)){
					if (nLpWorkTray > 0) {
						if (nLpWorkTray == 1) dLpY = m_pMoveData->dLoadPickerY[0] - (m_pEquipData->dLoadTrayPitchY * nLpTrayPosY);	//Stage1 Position
						if (nLpWorkTray == 2) dLpY = m_pMoveData->dLoadPickerY[1] - (m_pEquipData->dLoadTrayPitchY * nLpTrayPosY);	//Stage2 Position
					
					} else {
						if (m_nLoadStage2Case == 20) dLpY = m_pMoveData->dLoadPickerY[1];	//Stage2 Position
						else						 dLpY = m_pMoveData->dLoadPickerY[0];	//Stage1 Position
					}
				} else {
					if (m_nLoadStage2Case == 20) dLpY = m_pMoveData->dLoadPickerY[1];	//Stage2 Position
					else						 dLpY = m_pMoveData->dLoadPickerY[0];	//Stage1 Position
				}
				m_pDY11->oIndexLoadAlignOut = FALSE;
				g_objAJinAXL.Write_Output(11);

				g_objAJinAXL.Move_Absolute(AX_LOAD_PICKER_Y, dLpY);
				g_objCommon.Move_Position(AX_LOAD_PICKER_P, 0);	// Tray Pitch
				m_nLoadPickCase++; m_tLoadPickLoop.Set_LoopTime(5000);
			}
		//}
		break;
	case 20:	//Position Check & Vision Start
		if (g_objAJinAXL.Is_MoveDone(AX_LOAD_PICKER_Y, dLpY) && g_objCommon.Check_Position(AX_LOAD_PICKER_P, 0)) {
			m_tLoadPickLoop.Takt_Save(4, 9);
			m_tLoadPickLoop.Takt_Start();

			if (m_nVisCmAlignCase == 0) m_nVisCmAlignCase = 1;

			if (nLpWorkTray > 0) m_nLoadPickCase = 2;
			else				 m_nLoadPickCase = 0;
			m_tLoadPickLoop.Set_LoopTime(5000);

			m_strLog.Format("LoadPicker, %d", GetTickCount() - m_dwLoadPick);
			g_objLogFile.Save_TestLog(m_strLog);
		}
		break;

	}

	// 4. (Error : 3400)
	if (m_tLoadPickLoop.Over_LoopTime()) {
		if (m_nLoadPickCase == 0) return TRUE;
		g_objCommon.Show_Error(3400 + m_nLoadPickCase);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 5. (Error : 3500)
BOOL CSequenceMain::MainIndex_Run()
{
	static int nIndexVacuumRetry = 0;

	for (int i = 0; i < 3; i++) if (!gLot.bTaktDone[i]) Set_TaktLog(i);

	switch (m_nMainIndexCase) {
	case 0:
		if (Check_IndexEmpty(-1) && Check_PickerEmpty(1)) return TRUE;

		if (m_nLoadPickCase == 0 &&
			(((m_nLoadStage1Case == 0 || m_nLoadStage1Case == 1 || m_nLoadStage1Case == 60) && m_nLoadStage2Case == 50) ||
			 ((m_nLoadStage2Case == 0 || m_nLoadStage2Case == 1 || m_nLoadStage2Case == 60) && m_nLoadStage1Case == 50)))
		{
			m_nMainIndexCase = 10; m_tMainIndexLoop.Set_LoopTime(5000);
		} else if (m_nLoadPickCase > 4 && m_nLoadPickCase <= 15) {	// Loading
			m_nMainIndexCase++; m_tMainIndexLoop.Set_LoopTime(5000);
		}
		return TRUE;

	case 1:	// Index Align1 Out & Vacuum Pad Up
		m_pDY11->oIndexLoadAlignOut = TRUE;
		m_pDY11->oIndexLoadVacUp = TRUE;
		g_objAJinAXL.Write_Output(11);
		m_nMainIndexCase = 2; m_tMainIndexLoop.Set_LoopTime(5000);
		break;

	case 2:	// Check Index Align1 Out & Vacuum Pad Up
		if (!m_pDX11->iIndexLoadAlignIn && m_pDX11->iIndexLoadAlignOut && m_pDX11->iIndexLoadVacUp && !m_pDX11->iIndexLoadVacDown) {
			m_nMainIndexCase = 5; m_tMainIndexLoop.Set_LoopTime(5000);
		}
		break;

	case 5:	// Wait for Load Picker Done
		// 		if (gData.IndexDone[0]) {
		if (m_nLoadPickCase > 19) {	// LoadPicker Up 동작 후
			nIndexVacuumRetry = 0;	// Clear
			g_objCommon.Set_InfoIndexLoadVacuumOn(0);
			m_nMainIndexCase = 6; m_tMainIndexLoop.Set_LoopTime(5000);
		}
		return TRUE;

	case 6:	// Retry (Align Out) or Vacuum Off
		if (m_pDX11->iIndexLoadAlignIn && !m_pDX11->iIndexLoadAlignOut) {
			if (g_objCommon.Get_InfoIndexLoadVacuumOn(0)) {
				g_objCommon.Set_IndexLoadVacuumOff(0);
				m_nMainIndexCase = 8; m_tMainIndexLoop.Set_LoopTime(5000);
			} else {
				if (nIndexVacuumRetry < 1) {	// Retry 1회
					if (!m_tMainIndexLoop.Delay_LoopTime(1000)) break;	// 1초 기다리고 Retry (Align1 Out)
					m_pDY11->oIndexLoadAlignOut = TRUE;
					g_objAJinAXL.Write_Output(11);
					m_nMainIndexCase = 7; m_tMainIndexLoop.Set_LoopTime(5000);
				}
			}
		}
		break;
	case 7:	// Align1 In (Retry)
		if (!m_pDX11->iIndexLoadAlignIn && m_pDX11->iIndexLoadAlignOut) {
			nIndexVacuumRetry++;
			m_pDY11->oIndexLoadAlignOut = FALSE;
			g_objAJinAXL.Write_Output(11);
			m_nMainIndexCase = 6; m_tMainIndexLoop.Set_LoopTime(5000);
		}
		break;
	case 8:	// Vacuum Pad Down
		if (g_objCommon.Get_IndexLoadVacuumOff(0)) {
			m_pDY11->oIndexLoadVacUp = FALSE;
			g_objAJinAXL.Write_Output(11);
			m_nMainIndexCase = 9; m_tMainIndexLoop.Set_LoopTime(5000);
		}
		break;
	case 9:	// Check Vacuum Pad Down
		if (!m_pDX11->iIndexLoadVacUp && m_pDX11->iIndexLoadVacDown) {
			m_nMainIndexCase = 10; m_tMainIndexLoop.Set_LoopTime(5000);
		}
		break;

	case 10:	// Index R Move 45 Degree
		if (gData.IndexDone[0] && gData.IndexDone[1] && gData.IndexDone[2]) {
			if (m_pDX11->iIndexLoadAlignIn && !m_pDX11->iIndexLoadAlignOut && m_pDX11->iIndexAssyAlignIn && !m_pDX11->iIndexAssyAlignOut && m_pDX11->iIndexTransAlignIn && !m_pDX11->iIndexTransAlignOut &&
				!m_pDX11->iIndexLoadVacUp && m_pDX11->iIndexLoadVacDown && !m_pDX11->iIndexAssyVacUp && m_pDX11->iIndexAssyVacDown) 
			{
				if (Check_IndexEmpty(-1)) { 
					gData.IndexDone[0] = FALSE;
					m_nMainIndexCase = 0; 
					return TRUE;
				}
				if ((g_objAJinAXL.Get_Position(AX_LOAD_PICKER_Y) <= 200.0 || (g_objAJinAXL.Get_Position(AX_LOAD_PICKER_Y) > 200.0 && g_objCommon.Get_LoadPickerUp())) &&
					(g_objAJinAXL.Get_Position(AX_ASSY_PICKER_X) <= 200.0 || (g_objAJinAXL.Get_Position(AX_ASSY_PICKER_X) > 200.0 && g_objCommon.Get_AssyPickerUp(0))) &&
					((g_objAJinAXL.Get_Position(AX_TRANS_STAGE_X) >= 200.0 || (g_objAJinAXL.Get_Position(AX_TRANS_STAGE_X) < 200.0) && 
					 (g_objCommon.Check_Position(AX_TRANS_STAGE_Z, 0) || g_objCommon.Check_Position(AX_TRANS_STAGE_Z, 2))))) {

						g_objAJinAXL.Move_Relative(AX_INDEX_R, m_pMoveData->dIndexR[0]);
						m_nMainIndexCase = 11; m_tMainIndexLoop.Set_LoopTime(10000);
				}
			}
		}
		return TRUE;
	case 11:	// Check R Movd Done and Index End
		if (g_objAJinAXL.Is_MoveDone(AX_INDEX_R, m_pMoveData->dIndexR[0])) {
			Set_IndexEnd();
			m_nMainIndexCase = 0; m_tMainIndexLoop.Set_LoopTime(5000);
		}
		break;

		// Lot End Home
	case 21:		// Index R Home
		if (m_pDX11->iIndexLoadAlignIn && !m_pDX11->iIndexLoadAlignOut && m_pDX11->iIndexAssyAlignIn && !m_pDX11->iIndexAssyAlignOut && m_pDX11->iIndexTransAlignIn && !m_pDX11->iIndexTransAlignOut &&
			!m_pDX11->iIndexLoadVacUp && m_pDX11->iIndexLoadVacDown && !m_pDX11->iIndexAssyVacUp && m_pDX11->iIndexAssyVacDown) 
		{
			if ((g_objAJinAXL.Get_Position(AX_LOAD_PICKER_Y) <= 200.0 || (g_objAJinAXL.Get_Position(AX_LOAD_PICKER_Y) > 200.0 && g_objCommon.Get_LoadPickerUp())) &&
				(g_objAJinAXL.Get_Position(AX_ASSY_PICKER_X) <= 200.0 || (g_objAJinAXL.Get_Position(AX_ASSY_PICKER_X) > 200.0 && g_objCommon.Get_AssyPickerUp(0))) &&
				(g_objAJinAXL.Get_Position(AX_TRANS_STAGE_X) >= 200.0 || (g_objAJinAXL.Get_Position(AX_TRANS_STAGE_X) < 200.0 && 
				(g_objCommon.Check_Position(AX_TRANS_STAGE_X, 0) || g_objCommon.Check_Position(AX_TRANS_STAGE_X, 2))))) {

					g_objAJinAXL.Home_Search(AX_INDEX_R);
					m_nMainIndexCase = 22; m_tMainIndexLoop.Set_LoopTime(10000);
			}
		}
		break;
	case 22:		// Check R Home Done
		if (g_objAJinAXL.Is_Home(AX_INDEX_R)) {
			m_nMainIndexCase = 0;
		}
		break;
	}

	// 5. (Error : 3500)
	if (m_tMainIndexLoop.Over_LoopTime()) {
// 		if (m_nAssemPickCase == 144 || m_nAssemPickCase == 145) return TRUE;	// Flipping Servo Off 방지
// 
// 		if (m_nIndexTableCase == 101) g_objCommon.Err_InfoIndexLoadVacuumOn(0);
// 		if (m_nIndexTableCase == 103) g_objCommon.Err_IndexLoadVacuumOff();

		g_objCommon.Show_Error(3500 + m_nMainIndexCase);
		return FALSE;
	} else {
		return TRUE;
	}
}

///////////////////////////////////////////////////////////////////////////////
// 6. (Error : 3600)
BOOL CSequenceMain::VisionCM_Run()
{
	static int nCmScanCnt = 0;	// 총 Scan 횟수 
	static int nCmScanNo = 0;	// 현재 Scan 번호

	static double dCmX = 0.0;

	switch (m_nVisCmAlignCase) {
	case 0:		// Load Picker에서 Start
		return TRUE;

	case 1:		// X Move Inspect Position
		if (g_objCommon.Check_Position(AX_VISION_CM_ALIGN_X, 0)) {
			if (m_pEquipData->bUseVisionCmAlign) {
				m_dwVisCmAlign = GetTickCount();
				m_tVisCmAlignLoop.Takt_Start();
				nCmScanCnt = 0;
				nCmScanNo = 0;
				dCmX = m_pMoveData->dVisionCMAlignX[1];	// Inspect Position
				g_objAJinAXL.Move_Absolute(AX_VISION_CM_ALIGN_X, dCmX);
				m_nVisCmAlignCase++; m_tVisCmAlignLoop.Set_LoopTime(5000);

			} else {
				// Align 검사를 안하면 종료한다.
				gData.IndexDone[0] = TRUE;
				m_nVisCmAlignCase = 0; m_tVisCmAlignLoop.Set_LoopTime(5000);
			}
		}
		break;
	case 2:		// Load Complete Send
		if (g_objAJinAXL.Is_MoveDone(AX_VISION_CM_ALIGN_X, dCmX)) {
			nCmScanCnt = Get_CmScanCnt();
			if (gData.nCmAlignSkip == TRUE) {
				gData.nCmAlignSkip = FALSE;
				nCmScanNo++;
				m_nVisCmAlignCase = 10; m_tVisCmAlignLoop.Set_LoopTime(30000);
				break;
			}

			m_tVisCmAlignLoop.Takt_Save(5, 1);
			if (nCmScanNo <= nCmScanCnt) {
				m_tVisCmAlignLoop.Takt_Start();
				int nPNo = gData.nPNoIndex[0] - 1;
				int nTNo1 = gData.nTNoIndex[0][nCmScanNo];
				int nTNo2 = gData.nTNoIndex[0][nCmScanNo+3];
				int nCNo1 = gData.nCNoIndex[0][nCmScanNo];
				int nCNo2 = gData.nCNoIndex[0][nCmScanNo+3];
				gData.nCmInspPickNo1 = nCmScanNo + 1;
				gData.nCmInspPickNo2 = nCmScanNo + 4;

				gData.nCmAlignSkip = FALSE;
				if (m_pEquipData->bUseInlineMode) {
					g_objInspector.Set_LoadComplete("T1", gLot.sLotID[nPNo], gData.nPNoIndex[0], nTNo1, nTNo2, nCNo1, nCNo2, gData.nCmInspPickNo1, gData.nCmInspPickNo2);
				} else {
					g_objInspector.Set_LoadComplete("T1", gLot.sLotID[nPNo], gData.nPNoIndex[0], nTNo1, nTNo2, nCNo1, nCNo2, gData.nCmInspPickNo1, gData.nCmInspPickNo2);
				}
				

				nCmScanNo++;
				m_nVisCmAlignCase = 5; m_tVisCmAlignLoop.Set_LoopTime(30000);

			} else {
				m_nVisCmAlignCase = 15; m_tVisCmAlignLoop.Set_LoopTime(5000);
			}
		}
		break;

	case 5:	// Wait for CM inspection(30sec)
		if (!m_pEquipData->bUseVisionCmAlign) {
			m_nVisCmAlignCase = 10; m_tVisCmAlignLoop.Set_LoopTime(5000);
		}
		break;

	case 6:	// Error
		nCmScanNo--;
		m_nVisCmAlignCase = 2; m_tVisCmAlignLoop.Set_LoopTime(5000);
		g_objCommon.Show_Error(3606);
		return FALSE;

	case 10:	// 다음 검사 위치 피치 이동
		if (!m_pEquipData->bUseVisionCmAlign) { if(!m_tVisCmAlignLoop.Waiting_Time(300)) break; }	// Delay

		if (g_objAJinAXL.Is_Done(AX_VISION_CM_ALIGN_X)) {
			m_tVisCmAlignLoop.Takt_Save(5, 2);
			if (nCmScanNo <= nCmScanCnt) {				
				m_tVisCmAlignLoop.Takt_Start();
				dCmX = m_pMoveData->dVisionCMAlignX[1] + (m_pEquipData->dIndexPitch * nCmScanNo);
				g_objAJinAXL.Move_Absolute(AX_VISION_CM_ALIGN_X, dCmX);
				m_nVisCmAlignCase = 2; m_tVisCmAlignLoop.Set_LoopTime(10000);

			} else {
				m_nVisCmAlignCase = 15; m_tVisCmAlignLoop.Set_LoopTime(10000);
			}
		}
		break;
	case 15:	// X Move to Ready Position
		if (g_objAJinAXL.Is_Done(AX_VISION_CM_ALIGN_X)) {
			m_tVisCmAlignLoop.Takt_Start();
			
			g_objCommon.Move_Position(AX_VISION_CM_ALIGN_X, 0);
			m_nVisCmAlignCase++; m_tVisCmAlignLoop.Set_LoopTime(10000);
		}
		break;
	case 16:	// Check Position
		if (g_objCommon.Check_Position(AX_VISION_CM_ALIGN_X, 0)) {
			if (Check_CmAlignDone()) {
				m_tVisCmAlignLoop.Takt_Save(5,3);
				gData.IndexDone[0] = TRUE;
// 				if (gData.IndexDone[0] && Check_CmAlignDone())	{ m_nVisCmAlignCase = 0; m_tVisCmAlignLoop.Set_LoopTime(30000); }
// 				else											{ m_nVisCmAlignCase++; m_tVisCmAlignLoop.Set_LoopTime(30000); }

				m_nVisCmAlignCase = 0; m_tVisCmAlignLoop.Set_LoopTime(30000);
				m_strLog.Format("VisionCmAlign, %d", GetTickCount() - m_dwVisCmAlign);
				g_objLogFile.Save_TestLog(m_strLog);
			}
		}
		break;
	case 17:	// 검사 완료 확인.
		if (!gData.IndexDone[0] && Check_CmAlignDone()) {
			gData.IndexDone[0] = TRUE;
			m_nVisCmAlignCase = 0; m_tVisCmAlignLoop.Set_LoopTime(30000);
		}
		break;
	}

	// 6. (Error : 3600)
	if (m_tVisCmAlignLoop.Over_LoopTime()) {
		g_objCommon.Show_Error(3600 + m_nVisCmAlignCase);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 7. (Error : 3700)
BOOL CSequenceMain::VisionCap_Run()
{
	static int nCapScanCnt = 0;	// 총 Scan 횟수 
	static int nCapScanNo = 0;	// 현재 Scan 번호

	static double dCapY = 0.0;

	switch (m_nVisCapAlignCase) {
	case 0:		// Assembly Picker가 검사 위치에 오면 시각
		if (m_nAssyPickCase == 10) {
			m_nVisCapAlignCase = 1;
			 m_tVisCapAlignLoop.Set_LoopTime(30000);
		}
		return TRUE;

	case 1:		// X Move Inspect Position
		if (g_objCommon.Check_Position(AX_VISION_CAP_ALIGN_Y, 0)) {
			if (m_pEquipData->bUseVisionCapAlign) {
				if (!g_objInspector.Check_LotReady()) return TRUE;
				m_dwVisCapAlign = GetTickCount();
				m_tVisCapAlignLoop.Takt_Start();
				nCapScanCnt = 0;
				nCapScanNo = 0;
				dCapY = m_pMoveData->dVisionCapAlignY[0];	// Inspect Position
				g_objAJinAXL.Move_Absolute(AX_VISION_CAP_ALIGN_Y, dCapY);
				m_nVisCapAlignCase++; m_tVisCapAlignLoop.Set_LoopTime(5000);

			} else {
				// Align 검사를 안하면 종료한다.
				if (m_nAssyPickCase == 10) m_nAssyPickCase = 11;
				m_nVisCapAlignCase = 0; m_tVisCapAlignLoop.Set_LoopTime(5000);
			}
		}
		break;
	case 2:		// Load Complete Send
		if (g_objAJinAXL.Is_MoveDone(AX_VISION_CAP_ALIGN_Y, dCapY)) {
			nCapScanCnt = Get_CapScanCnt();
			if (gData.nCapAlignSkip == TRUE) {
				gData.nCapAlignSkip = FALSE;
				nCapScanNo++;
				m_nVisCapAlignCase = 10; m_tVisCapAlignLoop.Set_LoopTime(30000);
				break;
			}

			m_tVisCapAlignLoop.Takt_Save(6, 1);
			if (nCapScanNo <= nCapScanCnt) {
				m_tVisCapAlignLoop.Takt_Start();
				int nCapNo1 = 0;
				int nCapNo2 = 0;
				int nCapTNo1 = gData.nCapTNoAssyPicker[nCapScanNo];
				int nCapTNo2 = gData.nCapTNoAssyPicker[nCapScanNo+3];

				if (gData.InfoAssyPick[nCapScanNo] > 0)		{ nCapNo1 = gData.nCapNoAssyPicker[nCapScanNo]; }
				if (gData.InfoAssyPick[nCapScanNo+3] > 0)	{ nCapNo2 = gData.nCapNoAssyPicker[nCapScanNo+3]; }

				if (gData.InfoAssyPick[nCapScanNo] == 0)	{ nCapTNo1 = -1; nCapNo1 = -1;}
				if (gData.InfoAssyPick[nCapScanNo+3] == 0)	{ nCapTNo2 = -1; nCapNo2 = -1; }

				gData.nCapInspPickNo1 = nCapScanNo+1;
				gData.nCapInspPickNo2 = nCapScanNo+4;

				gData.nCapAlignSkip = FALSE;
				// Cap 검사는 Tray 번호와 CM 번호가 없다.
				CString sCapLotID;
				sCapLotID = gData.sCIDAssyPicker[nCapScanNo];
				sCapLotID.Replace("/", "_");
				g_objInspector.Set_LoadComplete("B1", sCapLotID, 1, nCapTNo1, nCapTNo2, nCapNo1, nCapNo2, gData.nCapInspPickNo1, gData.nCapInspPickNo2);

				nCapScanNo++;
				m_nVisCapAlignCase = 5; m_tVisCapAlignLoop.Set_LoopTime(30000);

			} else {
				m_nVisCapAlignCase = 15; m_tVisCapAlignLoop.Set_LoopTime(5000);
			}
		}
		break;

	case 5:	// Wait for Cap inspection(30sec)
		if (!m_pEquipData->bUseVisionCapAlign) {
			m_nVisCapAlignCase = 10; m_tVisCapAlignLoop.Set_LoopTime(5000);
		}
		break;
	case 6:	// Error
		nCapScanNo--;
		m_nVisCapAlignCase = 2; m_tVisCapAlignLoop.Set_LoopTime(5000);
		g_objCommon.Show_Error(3706);
		return FALSE;

	case 10:	// 다음 검사 위치 피치 이동
		if (!m_pEquipData->bUseVisionCapAlign) { if(!m_tVisCapAlignLoop.Waiting_Time(300)) break; }	// Delay

		if (g_objAJinAXL.Is_Done(AX_VISION_CAP_ALIGN_Y)) {
			m_tVisCapAlignLoop.Takt_Save(6, 2);
			if (nCapScanNo <= nCapScanCnt) {
				m_tVisCapAlignLoop.Takt_Start();
				dCapY = m_pMoveData->dVisionCapAlignY[0] + (m_pEquipData->dIndexPitch * nCapScanNo);
				g_objAJinAXL.Move_Absolute(AX_VISION_CAP_ALIGN_Y, dCapY);
				m_nVisCapAlignCase = 2; m_tVisCapAlignLoop.Set_LoopTime(10000);

			} else {
				m_nVisCapAlignCase = 15; m_tVisCapAlignLoop.Set_LoopTime(10000);
			}
		}
		break;
	case 15:	// X Move to Ready Position
		if (g_objAJinAXL.Is_Done(AX_VISION_CAP_ALIGN_Y)) {
			if (m_nAssyPickCase == 10) m_nAssyPickCase = 11;
			g_objCommon.Move_Position(AX_VISION_CAP_ALIGN_Y, 0);
			m_nVisCapAlignCase++; m_tVisCapAlignLoop.Set_LoopTime(10000);
		}
		break;
	case 16:	// Check Position
		if (g_objCommon.Check_Position(AX_VISION_CAP_ALIGN_Y, 0)) {
			m_tVisCapAlignLoop.Takt_Save(6, 3);
			m_nVisCapAlignCase = 0; m_tVisCapAlignLoop.Set_LoopTime(10000);

			m_strLog.Format("VisionCapAlign, %d", GetTickCount() - m_dwVisCapAlign);
			g_objLogFile.Save_TestLog(m_strLog);
		}
		break;
	}

	// 7. (Error : 3700)
	if (m_tVisCapAlignLoop.Over_LoopTime()) {
		g_objCommon.Show_Error(3700 + m_nVisCapAlignCase);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 8. (Error : 3800)
BOOL CSequenceMain::CapStage1_Run()
{
	if (gData.bCapPort1Wait && m_nCapStage1Case > 0 && m_nCapStage1Case <10) return TRUE;
	if (gData.bCapPort2Wait && m_nCapStage1Case >22 && m_nCapStage1Case <50) return TRUE;

	switch (m_nCapStage1Case) {
	case 0:		// Start시 1로 바꿈
		return TRUE;

	case 1:		// 안전확인, Z Move to Move Up
		if (m_bUnloadLotEnd) {
			m_nCapStage1Case = 0; m_tCapStage1Loop.Set_LoopTime(5000);
		}
		if (m_pDX02->iCapPort1LowCheck && !m_pDX04->iCapStage1Exist && g_objCommon.Check_Position(AX_CAP_STAGE1_X, 0)) {
			g_objCommon.Move_Position(AX_CAP_STAGE1_Z, 1);	// Move Up
			m_nCapStage1Case++; m_tCapStage1Loop.Set_LoopTime(5000);

		} else {
			if (!m_pDX02->iCapPort1LowCheck) { g_dlgWork.PostMessage(UM_SHOW_MSG, 4, NULL); return FALSE;}
		}
		break;

	case 2:		// Cap Count 확인 및 MES 자재 등록.
		if (m_pDX02->iCapPort1SlideClose && m_pDX02->iCapPort1LowCheck && !m_pDX04->iCapStage1Exist && g_objCommon.Get_CapTrayMasterSlaveOut(1) &&
			g_objCommon.Check_Position(AX_CAP_STAGE1_X, 0) && g_objCommon.Check_Position(AX_CAP_STAGE1_Z, 1))
		{
			if (gData.nCapTrayCount >= gData.nCapTrayUseCount) {
				// Cap 정보 업데이트
				m_nCapStage1Case++; m_tCapStage1Loop.Set_LoopTime(5000); break;
			
			} else {
				// Support Up
				gData.nCapTrayCount++; g_objCommon.Save_CapShiipData(1);
				gData.sCIDCapStage[0] = gData.sCapLotID;

				m_dwCapStage1 = GetTickCount();
				m_tCapStage1Loop.Takt_Start();
				g_objCommon.Move_Position(AX_CAP_STAGE1_Z, 2);
				m_nCapStage1Case = 6; m_tCapStage1Loop.Set_LoopTime(5000);
			}

		} else {
			if (!m_pDX02->iCapPort1LowCheck) { g_dlgWork.PostMessage(UM_SHOW_MSG, 4, NULL); return FALSE;}
		}
		break;
	case 3:	// Cap 정보 확인 및 MES 자재 등록.
		if (g_dlgWork.Check_CapLotID()) {
			if (m_pEquipData->bUseMes && m_pEquipData->bUseMesCapReg) {	// MES Cap 자재등록 사용
				g_objMES.m_nMESCapSequence = 0;
				g_dlgWork.Get_CapLotID();
				g_objMES.Set_CapLotChangeRequest(gData.sCapLotID, gData.sOperID);	// 자재 등록 요청
			}
			m_nCapStage1Case++; m_tCapStage1Loop.Set_LoopTime(30000);
		} else {
			g_dlgWork.PostMessage(UM_SHOW_MSG, 3, NULL);
			return FALSE;
		}
		break;
	case 4:	// MES 결과 확인.
		if (g_objMES.m_nMESCapSequence == 1 || !m_pEquipData->bUseMes || !m_pEquipData->bUseMesCapReg) {
			g_dlgWork.Set_CapLotIDChange();	// 정보 이동.
			gData.nCapTrayCount = 0;  g_objCommon.Save_CapShiipData(1);
			m_nCapStage1Case++; m_tCapStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 5:		// Z Move to Support Up
		if (m_pDX02->iCapPort1SlideClose && m_pDX02->iCapPort1LowCheck && !m_pDX04->iCapStage1Exist && g_objCommon.Get_CapTrayMasterSlaveOut(1) &&
			g_objCommon.Check_Position(AX_CAP_STAGE1_X, 0) && g_objCommon.Check_Position(AX_CAP_STAGE1_Z, 1))
		{
			gData.sCIDCapStage[0] = gData.sCapLotID;

			m_dwCapStage1 = GetTickCount();
			m_tCapStage1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_CAP_STAGE1_Z, 2);
			m_nCapStage1Case++; m_tCapStage1Loop.Set_LoopTime(5000);

		} else {
			if (!m_pDX02->iCapPort1LowCheck) { g_dlgWork.PostMessage(UM_SHOW_MSG, 4, NULL); return FALSE;}
		}
		break;

	case 6:		// Support Out
		if (g_objCommon.Check_Position(AX_CAP_STAGE1_Z, 2)) {
			m_tCapStage1Loop.Takt_Save(7, 1);
			m_tCapStage1Loop.Takt_Start();
			g_objCommon.Set_CapPortSupportOut(1);
			m_nCapStage1Case++; m_tCapStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 7:		// Z1 Move to Support Down Position
		if (g_objCommon.Get_CapPortSupportOut(1)) {
			m_tCapStage1Loop.Takt_Save(7, 2);
			m_tCapStage1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_CAP_STAGE1_Z, 3);	// Support Down Pos
			m_nCapStage1Case++; m_tCapStage1Loop.Set_LoopTime(10000);
		}
		break;
	case 8:		// Support In
		if (g_objCommon.Check_Position(AX_CAP_STAGE1_Z, 3)) {
			m_tCapStage1Loop.Takt_Save(7, 3);
			m_tCapStage1Loop.Takt_Start();
			g_objCommon.Set_CapPortSupportIn(1);
			m_nCapStage1Case++; m_tCapStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 9:	// Z1 Move to Moving Down Position
		if (g_objCommon.Get_CapPortSupportIn(1)) {
			m_tCapStage1Loop.Takt_Save(7, 4);
			m_tCapStage1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_CAP_STAGE1_Z, 0);	// Move Down
			m_nCapStage1Case++; m_tCapStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 10:	// Z1 Master In
		if (g_objCommon.Check_Position(AX_CAP_STAGE1_Z, 0)) {
			m_tCapStage1Loop.Takt_Save(7, 5);
			m_tCapStage1Loop.Takt_Start();
			//if (m_pDX04->iCapStage1Exist) {
				m_pDY04->oCapStage1MasterIn = TRUE;
				g_objAJinAXL.Write_Output(4);
				m_nCapStage1Case++; m_tCapStage1Loop.Set_LoopTime(5000);
			//}
		}
		break;
	case 11:		// Slave In
		if (m_pDX04->iCapStage1MasterIn && !m_pDX04->iCapStage1MasterOut) {
			if (!m_tCapStage1Loop.Waiting_Time(300)) break;
			m_pDY04->oCapStage1SlaveIn = TRUE;
			g_objAJinAXL.Write_Output(4);
			m_nCapStage1Case++; m_tCapStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 12:		// Cap Tray Count 증가
		if (g_objCommon.Get_CapTrayMasterSlaveIn(1)) {
			if (m_pDX04->iCapStage1Exist) {
				m_tCapStage1Loop.Takt_Save(7, 6);
// 				m_nCapStage1Case++; m_tCapStage1Loop.Set_LoopTime(5000);
				m_nCapStage1Case = 14; m_tCapStage1Loop.Set_LoopTime(5000);
			}			
		}
		break;

// 	case 13:	// InterLock
// 		if (g_objCommon.Check_Position(AX_CAP_STAGE1_Z, 0)) {
// 			double dPos = g_objAJinAXL.Get_Position(AX_CAP_STAGE2_Z);
// 			if (m_pMoveData->dCapStage2Z[1] >= dPos) {
// 				m_nCapStage1Case++; m_tCapStage1Loop.Set_LoopTime(5000);
// 			}
// 		}
// 		break;

	case 14:	// X1 Move to Start Position
		if (m_pDX04->iCapStage1Exist && g_objCommon.Check_Position(AX_CAP_STAGE1_Z, 0)) {
			if (m_nCapStage2Case < 20) return TRUE;
			if (g_objCommon.Check_Position(AX_CAP_STAGE2_Z, 0)) break;	// 인터락

			m_tCapStage1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_CAP_STAGE1_X, 1);	// Work Pos
			m_nCapStage1Case++; m_tCapStage1Loop.Set_LoopTime(10000);
		}
		break;
	case 15:	// Check X1 Start Position
		if (m_pDX04->iCapStage1Exist && g_objCommon.Check_Position(AX_CAP_STAGE1_X, 1)) {
			m_tCapStage1Loop.Takt_Save(7, 7);
			m_nCapStage1Case++; m_tCapStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 16:	// Wait
		if (m_nCapStage2Case < 14 || m_nCapStage2Case > 23) {
			m_nCapStage1Case++; m_tCapStage1Loop.Set_LoopTime(5000);
		}
		return TRUE;
	case 17:	// Move Up
		if (m_pDX04->iCapStage1Exist && g_objCommon.Check_Position(AX_CAP_STAGE1_Z, 0)) {
			m_tCapStage1Loop.Takt_Start();

			Init_CapTray();
			g_objCommon.Move_Position(AX_CAP_STAGE1_Z, 1);
			m_nCapStage1Case++; m_tCapStage1Loop.Set_LoopTime(10000);
		}
		break;
	case 18:	// Position Check
		if (m_pDX04->iCapStage1Exist && g_objCommon.Check_Position(AX_CAP_STAGE1_Z, 1)) {
			m_tCapStage1Loop.Takt_Save(7, 8);
			m_nCapStage1Case = 20; m_tCapStage1Loop.Set_LoopTime(10000);
		}
		break;

	case 20:	// Work
		return TRUE;
		
	case 21:	// 안전 확인
		if (m_nCapStage2Case > 51 || m_nCapStage2Case <= 16) {
			m_nCapStage1Case++; m_tCapStage1Loop.Set_LoopTime(10000);
		}
		return TRUE;
	case 22:	// X Move to Unload Position
		if (g_objCommon.Check_Position(AX_CAP_STAGE1_Z, 1)) {
			m_tCapStage1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_CAP_STAGE1_X, 2);	// Unload Pos
			m_nCapStage1Case++; m_tCapStage1Loop.Set_LoopTime(10000);
		}
		break;
	case 23:	// Z Move to Support Down
		if (m_pDX02->iCapPort2SlideClose && g_objCommon.Check_Position(AX_CAP_STAGE1_X, 2)) {
			m_tCapStage1Loop.Takt_Save(7, 9);
			m_tCapStage1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_CAP_STAGE1_Z, 3);	// Support Down
			m_nCapStage1Case++; m_tCapStage1Loop.Set_LoopTime(10000);
		}
		break;
	case 24:	// Port Support Out
		if (g_objCommon.Check_Position(AX_CAP_STAGE1_Z, 3)) {
			m_tCapStage1Loop.Takt_Save(7, 10);
			m_tCapStage1Loop.Takt_Start();
			g_objCommon.Set_CapPortSupportOut(2);
			m_nCapStage1Case++; m_tCapStage1Loop.Set_LoopTime(10000);
		}
		break;
	case 25:	// Z Move to Support Up
		if (g_objCommon.Get_CapPortSupportOut(2)) {
			m_tCapStage1Loop.Takt_Save(7, 11);
			m_tCapStage1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_CAP_STAGE1_Z, 2);	// Support Up
			m_nCapStage1Case++; m_tCapStage1Loop.Set_LoopTime(10000);
		}
		break;
	case 26:	// Port Support In
		if (g_objCommon.Check_Position(AX_CAP_STAGE1_Z, 2)) {
			m_tCapStage1Loop.Takt_Save(7, 12);
			m_tCapStage1Loop.Takt_Start();
			g_objCommon.Set_CapPortSupportIn(2);
			m_nCapStage1Case++; m_tCapStage1Loop.Set_LoopTime(10000);
		}
		break;
	case 27:	// Master/Slave Out
		if (g_objCommon.Get_CapPortSupportIn(2)) {
			m_tCapStage1Loop.Takt_Save(7, 13);
			m_tCapStage1Loop.Takt_Start();
			m_pDY04->oCapStage1MasterIn = FALSE;
			m_pDY04->oCapStage1SlaveIn = FALSE;
			g_objAJinAXL.Write_Output(4);
			m_nCapStage1Case++; m_tCapStage1Loop.Set_LoopTime(10000);
		}
		break;
	case 28:	// Z Move to Move Up Position
		if (g_objCommon.Get_CapTrayMasterSlaveOut(1))
		{
			m_tCapStage1Loop.Takt_Save(7, 14);
			m_tCapStage1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_CAP_STAGE1_Z, 1);	// Move Up
			m_nCapStage1Case++; m_tCapStage1Loop.Set_LoopTime(10000);
		}
		break;
	case 29:	// Position Check
		if (g_objCommon.Check_Position(AX_CAP_STAGE1_Z, 1)) {
			m_tCapStage1Loop.Takt_Save(7, 15);
			m_nCapStage1Case = 50; m_tCapStage1Loop.Set_LoopTime(10000);

			m_strLog.Format("CapStage1, %d", GetTickCount() - m_dwCapStage1);
			g_objLogFile.Save_TestLog(m_strLog);
		}
		break;


	case 50:	// Stage2 Move Down 이후
		if (m_nCapStage2Case > 14 && m_nCapStage2Case < 50) {	
			m_nCapStage1Case++; m_tCapStage1Loop.Set_LoopTime(10000);
		}
		return TRUE;
	case 51:	// Move to Load Position
		if (!m_pDX04->iCapStage1Exist && g_objCommon.Check_Position(AX_CAP_STAGE1_Z, 1)) {
			m_tCapStage1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_CAP_STAGE1_X, 0);
			m_nCapStage1Case++; m_tCapStage1Loop.Set_LoopTime(10000);
		}
		break;
	case 52:
		if (!m_pDX04->iCapStage1Exist &&
			g_objCommon.Check_Position(AX_CAP_STAGE1_Z, 1) && g_objCommon.Check_Position(AX_CAP_STAGE1_X, 0))
		{
			m_tCapStage1Loop.Takt_Save(7, 16);
			m_nCapStage1Case = 2; m_tCapStage1Loop.Set_LoopTime(10000);
		}
		break;

	}

	// 8. (Error : 3800)
	if (m_tCapStage1Loop.Over_LoopTime()) {
		g_objCommon.Show_Error(3800 + m_nCapStage1Case);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 9. (Error : 3900)
BOOL CSequenceMain::CapStage2_Run()
{
	if (gData.bCapPort1Wait && m_nCapStage2Case > 0 && m_nCapStage2Case <10) return TRUE;
	if (gData.bCapPort2Wait && m_nCapStage2Case >22 && m_nCapStage2Case <50) return TRUE;

	switch (m_nCapStage2Case) {
	case 0:		// Start시 1로 바꿈
		return TRUE;

	case 1:		// 안전확인, Z Move to Move Up
		if (m_bUnloadLotEnd) {
			m_nCapStage2Case = 0; m_tCapStage2Loop.Set_LoopTime(5000);
		}
		if (m_pDX02->iCapPort1LowCheck && !m_pDX04->iCapStage2Exist && g_objCommon.Check_Position(AX_CAP_STAGE2_X, 0)) {
			g_objCommon.Move_Position(AX_CAP_STAGE2_Z, 1);	// Move Up
			m_nCapStage2Case++; m_tCapStage2Loop.Set_LoopTime(5000);

		} else {
			if (!m_pDX02->iCapPort1LowCheck) { g_dlgWork.PostMessage(UM_SHOW_MSG, 4, NULL); return FALSE;}
		}
		break;

	case 2:		// Cap Count 확인 및 MES 자재 등록.
		if (m_pDX02->iCapPort1SlideClose && m_pDX02->iCapPort1LowCheck && !m_pDX04->iCapStage2Exist && g_objCommon.Get_CapTrayMasterSlaveOut(2) &&
			g_objCommon.Check_Position(AX_CAP_STAGE2_X, 0) && g_objCommon.Check_Position(AX_CAP_STAGE2_Z, 1))
		{
			if (gData.nCapTrayCount >= gData.nCapTrayUseCount) {
				// Cap 정보 업데이트
				m_nCapStage2Case++; m_tCapStage2Loop.Set_LoopTime(5000); break;

			} else {
				// Support Up
				gData.nCapTrayCount++; g_objCommon.Save_CapShiipData(1);
				gData.sCIDCapStage[1] = gData.sCapLotID;

				m_dwCapStage2 = GetTickCount();
				m_tCapStage2Loop.Takt_Start();
				g_objCommon.Move_Position(AX_CAP_STAGE2_Z, 2);
				m_nCapStage2Case = 6; m_tCapStage2Loop.Set_LoopTime(5000);
			}

		} else {
			if (!m_pDX02->iCapPort1LowCheck) { g_dlgWork.PostMessage(UM_SHOW_MSG, 4, NULL); return FALSE;}
		}
		break;
	case 3:	// Cap 정보 확인 및 MES 자재 등록.
		if (g_dlgWork.Check_CapLotID()) {
			if (m_pEquipData->bUseMes && m_pEquipData->bUseMesCapReg) {	// MES Cap 자재등록 사용
				g_objMES.m_nMESCapSequence = 0;
				g_dlgWork.Get_CapLotID();
				g_objMES.Set_CapLotChangeRequest(gData.sCapLotID, gData.sOperID);	// 자재 등록 요청
			}
			m_nCapStage2Case++; m_tCapStage2Loop.Set_LoopTime(30000);
		} else {
			g_dlgWork.PostMessage(UM_SHOW_MSG, 3, NULL);
			return FALSE;
		}
		break;
	case 4:	// MES 결과 확인.
		if (g_objMES.m_nMESCapSequence == 1 || !m_pEquipData->bUseMes || !m_pEquipData->bUseMesCapReg) {
			g_dlgWork.Set_CapLotIDChange();	// 정보 이동.
			gData.nCapTrayCount = 0; g_objCommon.Save_CapShiipData(1);
			m_nCapStage2Case++; m_tCapStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 5:		// Z Move to Support Up
		if (m_pDX02->iCapPort1SlideClose && m_pDX02->iCapPort1LowCheck && !m_pDX04->iCapStage2Exist && g_objCommon.Get_CapTrayMasterSlaveOut(2) &&
			g_objCommon.Check_Position(AX_CAP_STAGE2_X, 0) && g_objCommon.Check_Position(AX_CAP_STAGE2_Z, 1))
		{
			gData.sCIDCapStage[1] = gData.sCapLotID;

			m_dwCapStage2 = GetTickCount();
			m_tCapStage2Loop.Takt_Start();
			g_objCommon.Move_Position(AX_CAP_STAGE2_Z, 2);
			m_nCapStage2Case++; m_tCapStage2Loop.Set_LoopTime(5000);

		} else {
			if (!m_pDX02->iCapPort1LowCheck) { g_dlgWork.PostMessage(UM_SHOW_MSG, 4, NULL); return FALSE;}
		}
		break;

	case 6:		// Support Out
		if (g_objCommon.Check_Position(AX_CAP_STAGE2_Z, 2)) {
			m_tCapStage2Loop.Takt_Save(8, 1);
			m_tCapStage2Loop.Takt_Start();
			g_objCommon.Set_CapPortSupportOut(1);
			m_nCapStage2Case++; m_tCapStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 7:		// Z1 Move to Support Down Position
		if (g_objCommon.Get_CapPortSupportOut(1)) {
			m_tCapStage2Loop.Takt_Save(8, 2);
			m_tCapStage2Loop.Takt_Start();
			g_objCommon.Move_Position(AX_CAP_STAGE2_Z, 3);	// Support Down Pos
			m_nCapStage2Case++; m_tCapStage2Loop.Set_LoopTime(10000);
		}
		break;
	case 8:		// Support In
		if (g_objCommon.Check_Position(AX_CAP_STAGE2_Z, 3)) {
			m_tCapStage2Loop.Takt_Save(8, 3);
			m_tCapStage2Loop.Takt_Start();
			g_objCommon.Set_CapPortSupportIn(1);
			m_nCapStage2Case++; m_tCapStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 9:	// Z1 Move to Moving Down Position
		if (g_objCommon.Get_CapPortSupportIn(1)) {
			m_tCapStage2Loop.Takt_Save(8, 4);
			m_tCapStage2Loop.Takt_Start();
			g_objCommon.Move_Position(AX_CAP_STAGE2_Z, 0);	// Move Down
			m_nCapStage2Case++; m_tCapStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 10:	// Z1 Master In
		if (g_objCommon.Check_Position(AX_CAP_STAGE2_Z, 0)) {
			m_tCapStage2Loop.Takt_Save(8, 5);
			m_tCapStage2Loop.Takt_Start();
			//if (m_pDX04->iCapStage2Exist) {
				m_pDY04->oCapStage2MasterIn = TRUE;
				g_objAJinAXL.Write_Output(4);
				m_nCapStage2Case++; m_tCapStage2Loop.Set_LoopTime(5000);
			//}
		}
		break;
	case 11:		// Slave In
		if (m_pDX04->iCapStage2MasterIn && !m_pDX04->iCapStage2MasterOut) {
			if (!m_tCapStage2Loop.Waiting_Time(300)) break;
			m_pDY04->oCapStage2SlaveIn = TRUE;
			g_objAJinAXL.Write_Output(4);
			m_nCapStage2Case++; m_tCapStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 12:		// Cap Tray Count 증가
		if (g_objCommon.Get_CapTrayMasterSlaveIn(2)) {
			if (m_pDX04->iCapStage2Exist) {
				m_tCapStage2Loop.Takt_Save(8, 6);
// 				m_nCapStage2Case++; m_tCapStage2Loop.Set_LoopTime(5000);
				m_nCapStage2Case = 14; m_tCapStage2Loop.Set_LoopTime(5000);
			}			
		}
		break;

// 	case 13:	// InterLock
// 		if (g_objCommon.Check_Position(AX_CAP_STAGE2_Z, 0)) {
// 			double dPos = g_objAJinAXL.Get_Position(AX_CAP_STAGE1_Z);
// 			if (m_pMoveData->dCapStage1Z[1] >= dPos) {
// 				m_nCapStage2Case++; m_tCapStage2Loop.Set_LoopTime(5000);
// 			}
// 		}
// 		break;

	case 14:	// X1 Move to Start Position
		if (m_pDX04->iCapStage2Exist && g_objCommon.Check_Position(AX_CAP_STAGE2_Z, 0)) {
			if (m_nCapStage1Case < 20) return TRUE;
			if (g_objCommon.Check_Position(AX_CAP_STAGE1_Z, 0)) break;	// 인터락

			m_tCapStage2Loop.Takt_Start();
			g_objCommon.Move_Position(AX_CAP_STAGE2_X, 1);	// Work Pos
			m_nCapStage2Case++; m_tCapStage2Loop.Set_LoopTime(10000);
		}
		break;
	case 15:	// Check X2 Start Position
		if (m_pDX04->iCapStage2Exist && g_objCommon.Check_Position(AX_CAP_STAGE2_X, 1)) {
			m_tCapStage2Loop.Takt_Save(8, 7);
			m_nCapStage2Case++; m_tCapStage2Loop.Set_LoopTime(10000);
		}
		break;
	case 16:	// Wait
		if (m_nCapStage1Case < 14 || m_nCapStage1Case > 23) {
			m_nCapStage2Case++; m_tCapStage2Loop.Set_LoopTime(5000);
		}
		return TRUE;
	case 17:	// Move Up
		if (m_pDX04->iCapStage2Exist && g_objCommon.Check_Position(AX_CAP_STAGE2_Z, 0)) {
			m_tCapStage2Loop.Takt_Start();

			Init_CapTray();
			g_objCommon.Move_Position(AX_CAP_STAGE2_Z, 1);
			m_nCapStage2Case++; m_tCapStage2Loop.Set_LoopTime(10000);
		}
		break;
	case 18:	// Position Check
		if (m_pDX04->iCapStage2Exist && g_objCommon.Check_Position(AX_CAP_STAGE2_Z, 1)) {
			m_tCapStage2Loop.Takt_Save(8, 8);
			m_nCapStage2Case = 20; m_tCapStage2Loop.Set_LoopTime(10000);
		}
		break;

	case 20:	// Work
		return TRUE;

	case 21:	// 안전 확인
		if (m_nCapStage1Case > 51 || m_nCapStage1Case <= 16) {
			m_nCapStage2Case++; m_tCapStage2Loop.Set_LoopTime(10000);
		}
		return TRUE;
	case 22:	// X Move to Unload Position
		if (g_objCommon.Check_Position(AX_CAP_STAGE2_Z, 1)) {
			m_tCapStage2Loop.Takt_Start();
			g_objCommon.Move_Position(AX_CAP_STAGE2_X, 2);	// Unload Pos
			m_nCapStage2Case++; m_tCapStage2Loop.Set_LoopTime(10000);
		}
		break;
	case 23:	// Z Move to Support Down
		if (m_pDX02->iCapPort2SlideClose && g_objCommon.Check_Position(AX_CAP_STAGE2_X, 2)) {
			m_tCapStage2Loop.Takt_Save(8, 9);
			m_tCapStage2Loop.Takt_Start();
			g_objCommon.Move_Position(AX_CAP_STAGE2_Z, 3);	// Support Down
			m_nCapStage2Case++; m_tCapStage2Loop.Set_LoopTime(10000);
		}
		break;
	case 24:	// Port Support Out
		if (g_objCommon.Check_Position(AX_CAP_STAGE2_Z, 3)) {
			m_tCapStage2Loop.Takt_Save(8, 10);
			m_tCapStage2Loop.Takt_Start();
			g_objCommon.Set_CapPortSupportOut(2);
			m_nCapStage2Case++; m_tCapStage2Loop.Set_LoopTime(10000);
		}
		break;
	case 25:	// Z Move to Support Up
		if (g_objCommon.Get_CapPortSupportOut(2)) {
			m_tCapStage2Loop.Takt_Save(8, 11);
			m_tCapStage2Loop.Takt_Start();
			g_objCommon.Move_Position(AX_CAP_STAGE2_Z, 2);	// Support Up
			m_nCapStage2Case++; m_tCapStage2Loop.Set_LoopTime(10000);
		}
		break;
	case 26:	// Port Support In
		if (g_objCommon.Check_Position(AX_CAP_STAGE2_Z, 2)) {
			m_tCapStage2Loop.Takt_Save(8, 12);
			m_tCapStage2Loop.Takt_Start();
			g_objCommon.Set_CapPortSupportIn(2);
			m_nCapStage2Case++; m_tCapStage2Loop.Set_LoopTime(10000);
		}
		break;
	case 27:	// Master/Slave Out
		if (g_objCommon.Get_CapPortSupportIn(2)) {
			m_tCapStage2Loop.Takt_Save(8, 13);
			m_tCapStage2Loop.Takt_Start();
			m_pDY04->oCapStage2MasterIn = FALSE;
			m_pDY04->oCapStage2SlaveIn = FALSE;
			g_objAJinAXL.Write_Output(4);
			m_nCapStage2Case++; m_tCapStage2Loop.Set_LoopTime(10000);
		}
		break;
	case 28:	// Z Move to Move Up Position
		if (g_objCommon.Get_CapTrayMasterSlaveOut(2))
		{
			m_tCapStage2Loop.Takt_Save(8, 14);
			m_tCapStage2Loop.Takt_Start();
			g_objCommon.Move_Position(AX_CAP_STAGE2_Z, 1);	// Move Up
			m_nCapStage2Case++; m_tCapStage2Loop.Set_LoopTime(10000);
		}
		break;
	case 29:	// Position Check
		if (g_objCommon.Check_Position(AX_CAP_STAGE2_Z, 1)) {
			m_tCapStage2Loop.Takt_Save(8, 15);
			m_nCapStage2Case = 50; m_tCapStage2Loop.Set_LoopTime(10000);

			m_strLog.Format("CapStage2, %d", GetTickCount() - m_dwCapStage2);
			g_objLogFile.Save_TestLog(m_strLog);
		}
		break;


	case 50:	// Stage2 Move Down 이후
		if (m_nCapStage1Case > 14 && m_nCapStage1Case < 50) {
			m_nCapStage2Case++; m_tCapStage2Loop.Set_LoopTime(10000);
		}
		return TRUE;
	case 51:	// Move to Load Position
		if (!m_pDX04->iCapStage2Exist && g_objCommon.Check_Position(AX_CAP_STAGE2_Z, 1)) {
			m_tCapStage2Loop.Takt_Start();
			g_objCommon.Move_Position(AX_CAP_STAGE2_X, 0);
			m_nCapStage2Case++; m_tCapStage2Loop.Set_LoopTime(10000);
		}
		break;
	case 52:
		if (!m_pDX04->iCapStage2Exist &&
			g_objCommon.Check_Position(AX_CAP_STAGE2_Z, 1) && g_objCommon.Check_Position(AX_CAP_STAGE2_X, 0))
		{
			m_tCapStage2Loop.Takt_Save(8, 16);
			m_nCapStage2Case = 2; m_tCapStage2Loop.Set_LoopTime(10000);
		}
		break;
	}

	// 9. (Error : 3900)
	if (m_tCapStage2Loop.Over_LoopTime()) {
		g_objCommon.Show_Error(3900 + m_nCapStage2Case);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 10. (Error : 4000)
BOOL CSequenceMain::CapPicker_Run()
{
	static int nCpWorkTray = 0;		// 현재 작업중인 트레이

	static int nCpPosX = 0;			// Tray X
	static int nCpPosY = 0;			// Tray Y
	static int nCpStart = 0;		// Picker Start No
	static int nCpPickCnt = 0;		// Picker가 잡을수 있는 수
	static int nCpDownSu = 0;		// 최종적으로 Picker가 Down되는 수

	static int nCpTrayLine = 0;		// Cap Tray Line
	static int nCpTrayCnt = 0;		// Tray Cnt	

	static double dCpY = 0.0;		// Picker Y
	static double dCpX = 0.0;		// Tray X

	switch (m_nCapPickCase) {
	case 0:
		if (m_nCapStage1Case == 20 || m_nCapStage2Case == 20) m_nCapPickCase++;
		return TRUE;

	case 1:		// 안전확인
		if ((g_objCommon.Check_Position(AX_CAP_PICKER_Y, 0) || g_objCommon.Check_Position(AX_CAP_PICKER_Y, 6)) &&
			g_objCommon.Check_Position(AX_CAP_PICKER_Z, 0) && g_objCommon.Check_Position(AX_CAP_PICKER_P, 0) &&
			g_objCommon.Get_CapPickerUp(0) && g_objCommon.Get_InfoCapPickerNormal())
		{
			if (m_nCapStage1Case != 20 && m_nCapStage2Case != 20) return TRUE;	//break;
			m_dwCapPick = GetTickCount();

			if (m_nCapStage1Case == 20) { nCpWorkTray = 1; dCpX = g_objAJinAXL.Get_Position(AX_CAP_STAGE1_X); }
			if (m_nCapStage2Case == 20) { nCpWorkTray = 2; dCpX = g_objAJinAXL.Get_Position(AX_CAP_STAGE2_X); }
			dCpY = g_objAJinAXL.Get_Position(AX_CAP_PICKER_Y);

			m_nCapPickCase++; m_tCapPickLoop.Set_LoopTime(5000);
		}
		break;
		
	case 2:		// Stage X Picker Y Move to Cap Pickup Position
		if ( ((nCpWorkTray == 1 && g_objAJinAXL.Is_MoveDone(AX_CAP_STAGE1_X, dCpX)) ||
			  (nCpWorkTray == 2 && g_objAJinAXL.Is_MoveDone(AX_CAP_STAGE2_X, dCpX))) &&
			  g_objAJinAXL.Is_MoveDone(AX_CAP_PICKER_Y, dCpY))
		{
			if (m_nCapStage1Case != 20 && m_nCapStage2Case != 20) return TRUE;	//break;

			// Picker가 Cap을 가지고 있을 수 있어 비어있는 피커만 내릴수 있도록 한다.
			if (Select_CapPickerPos(nCpStart, nCpPickCnt)) {
				if (Select_CapTrayPos(nCpPosX, nCpPosY, nCpTrayCnt)){

					m_tCapPickLoop.Takt_Start();
					if (m_pEquipData->bCapPickUpMulti) {
						nCpTrayCnt = m_pEquipData->nCapTrayX - nCpPosX;
						nCpDownSu = ((nCpPickCnt < nCpTrayCnt) ? nCpPickCnt : nCpTrayCnt);
						m_nCapPickMultiCnt = nCpDownSu;

						int nIndex = (nCpWorkTray-1)*6;	// Cap Picker Y축 MoveData Index, Stage1 Picker1 = 0, Stage2 Picker1 = 6
						if (nCpWorkTray == 1) dCpX = m_pMoveData->dCapStage1X[1] - nCpPosY * m_pEquipData->dCapTrayPitchY;
						if (nCpWorkTray == 2) dCpX = m_pMoveData->dCapStage2X[1] - nCpPosY * m_pEquipData->dCapTrayPitchY;
						dCpY = m_pMoveData->dCapPickerY[nIndex] + (nCpPosX - nCpStart) * m_pEquipData->dCapTrayPitchX;

					} else {
						// Cap 건너뛰어 PickUp						
						nCpDownSu = ((nCpPickCnt < nCpTrayCnt) ? nCpPickCnt : nCpTrayCnt);
						m_nCapPickMultiCnt = nCpDownSu;

						int nIndex = nCpStart + ((nCpWorkTray-1)*6);	// Cap Picker Y축 MoveData Index
						if (nCpWorkTray == 1) dCpX = m_pMoveData->dCapStage1X[1] - nCpPosY * m_pEquipData->dCapTrayPitchY;
						if (nCpWorkTray == 2) dCpX = m_pMoveData->dCapStage2X[1] - nCpPosY * m_pEquipData->dCapTrayPitchY;
						dCpY = m_pMoveData->dCapPickerY[nIndex] + (nCpPosX - nCpStart) * m_pEquipData->dCapTrayPitchX;
					}

					if (nCpWorkTray == 1) g_objAJinAXL.Move_Absolute(AX_CAP_STAGE1_X, dCpX);
					if (nCpWorkTray == 2) g_objAJinAXL.Move_Absolute(AX_CAP_STAGE2_X, dCpX);
					g_objAJinAXL.Move_Absolute(AX_CAP_PICKER_Y, dCpY);		
					m_nCapPickCase++; m_tCapPickLoop.Set_LoopTime(10000);
				}
			}
		}
		break;
	case 3:		// Z Move to Tray Down
		if (((nCpWorkTray == 1 && g_objAJinAXL.Is_MoveDone(AX_CAP_STAGE1_X, dCpX)) ||
			(nCpWorkTray == 2 && g_objAJinAXL.Is_MoveDone(AX_CAP_STAGE2_X, dCpX))) &&
			g_objAJinAXL.Is_MoveDone(AX_CAP_PICKER_Y, dCpY))
		{
			m_tCapPickLoop.Takt_Save(9, 1);
			m_tCapPickLoop.Takt_Start();
			g_objCommon.Move_Position(AX_CAP_PICKER_Z, nCpWorkTray);	//Cap Tray Down
			g_objCommon.Set_CapPickerDownMulti(nCpStart+1, nCpDownSu);
			m_nCapPickCase++; m_tCapPickLoop.Set_LoopTime(10000);
		}
		break;
	case 4:		// Vac On
		if (g_objCommon.Check_Position(AX_CAP_PICKER_Z, nCpWorkTray) && g_objCommon.Get_CapPickerDownMulti(nCpStart+1, nCpDownSu)) {
			g_objCommon.Set_CapPickerVacOnMulti(nCpStart+1, nCpDownSu);
			m_nCapPickCase++; m_tCapPickLoop.Set_LoopTime(90000);
		}
		break;
	case 5:		// Z Move to Ready Up
		if (!m_tCapPickLoop.Waiting_Time(m_pEquipData->nDelayAdd[1])) break;

		 g_objCommon.Move_Position(AX_CAP_PICKER_Z, 0);	//Ready Up
		 g_objCommon.Set_CapPickerUp(0);	// All Up
		 m_nCapPickCase++; m_tCapPickLoop.Set_LoopTime(10000);
		 break;
	case 6:		// 정보전달, Vac Check & Picker Full Check
		if (g_objCommon.Get_InfoCapPickerVacOn(0) && 
			g_objCommon.Check_Position(AX_CAP_PICKER_Z, 0) && g_objCommon.Get_CapPickerUp(0))
		{
			m_tCapPickLoop.Takt_Save(9, 2);
			if (m_pEquipData->bCapPickUpMulti) {
				for (int i=0; i<nCpDownSu; i++) {
					gData.InfoCapPick[nCpStart+i] = gData.InfoCapTray[nCpPosY][nCpPosX+i]; gData.InfoCapTray[nCpPosY][nCpPosX+i] = 0;
					gData.nCapNoCapPicker[nCpStart+i] = (nCpPosY * m_pEquipData->nCapTrayX) + nCpPosX + i + 1;
					gData.nCapTNoCapPicker[nCpStart+i] = gData.nCapTrayCount;
					gData.sCIDCapPicker[nCpStart+i] = gData.sCIDCapStage[nCpWorkTray-1];
				}
			} else {
				int nTX, nTY;
				for (int i=0; i<nCpDownSu; i++) {
					nTX = nCpPosX+(i*2);
					nTY = nCpPosY;
					gData.InfoCapPick[nCpStart+i] = gData.InfoCapTray[nTY][nTX]; gData.InfoCapTray[nTY][nTX] = 0;
					gData.nCapNoCapPicker[nCpStart+i] = (nCpPosY * m_pEquipData->nCapTrayX) + nTX + 1;
					gData.nCapTNoCapPicker[nCpStart+i] = gData.nCapTrayCount;
					gData.sCIDCapPicker[nCpStart+i] = gData.sCIDCapStage[nCpWorkTray-1];
				}
			}
			g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 2, 0);

			if (Check_CapTrayEmpty()) {
				if (m_nCapStage1Case == 20) m_nCapStage1Case = 21;
				if (m_nCapStage2Case == 20) m_nCapStage2Case = 21;
			}
			if (Check_CapPickerFull()) { m_nCapPickCase = 9; m_tCapPickLoop.Set_LoopTime(10000); }
			else { m_nCapPickCase++; m_tCapPickLoop.Set_LoopTime(10000); }
			
		}
		break;
	case 7:		// Tray 교체 중이면 대기
		if (g_objCommon.Get_InfoCapPickerVacOn(0) && 
			g_objCommon.Check_Position(AX_CAP_PICKER_Z, 0) && g_objCommon.Get_CapPickerUp(0))
		{
			if (m_nCapStage1Case != 20 && m_nCapStage2Case != 20) return TRUE;	//break;

			if (m_nCapStage1Case == 20) { nCpWorkTray = 1; dCpX = g_objAJinAXL.Get_Position(AX_CAP_STAGE1_X); }
			if (m_nCapStage2Case == 20) { nCpWorkTray = 2; dCpX = g_objAJinAXL.Get_Position(AX_CAP_STAGE2_X); }
			dCpY = g_objAJinAXL.Get_Position(AX_CAP_PICKER_Y);

			m_nCapPickCase = 2; m_tCapPickLoop.Set_LoopTime(5000);
		}
		break;

	case 9:	// Y/P Move to Cap Buffer Position
		if (g_objCommon.Get_InfoCapPickerVacOn(0) && 
			g_objCommon.Check_Position(AX_CAP_PICKER_Z, 0) && g_objCommon.Get_CapPickerUp(0))
		{
			m_tCapPickLoop.Takt_Start();
			 g_objCommon.Move_Position(AX_CAP_PICKER_Y, 12);	// Cap Buffer
			 g_objCommon.Move_Position(AX_CAP_PICKER_P, 1);	// Cap Buffer
			 if (m_pEquipData->bUseCapPickerTurn) {
				 g_objCommon.Set_CapPickerTurn(0);
			 }
			 m_nCapPickCase++; m_tCapPickLoop.Set_LoopTime(10000);
		}
		break;
	case 10:	// Cap Buffer Check
		if (g_objCommon.Check_Position(AX_CAP_PICKER_Y, 12) && g_objCommon.Check_Position(AX_CAP_PICKER_P, 1) &&
			((m_pEquipData->bUseCapPickerTurn && g_objCommon.Get_CapPickerTurn(0)) || !m_pEquipData->bUseCapPickerTurn))
		{
			if (m_nCapBufferCase == 0) { m_tCapPickLoop.Takt_Save(9, 3); m_nCapPickCase++; m_tCapPickLoop.Set_LoopTime(10000); }
			return TRUE;
		}
		break;

	case 11:	// Buffer Align Out
		if (g_objCommon.Check_Position(AX_CAP_BUFFER_STAGE_Y, 0)) {
			m_tCapPickLoop.Takt_Start();
			g_objCommon.Set_CapBufferAlign(FALSE);
			m_nCapPickCase++; m_tCapPickLoop.Set_LoopTime(10000);
		}
		break;
	case 12:	//Z Move to Buffer Down 
		if (g_objCommon.Check_Position(AX_CAP_BUFFER_STAGE_Y, 0) && g_objCommon.Get_CapBufferAlign(FALSE) &&
			g_objCommon.Check_Position(AX_CAP_PICKER_Y, 12) && g_objCommon.Check_Position(AX_CAP_PICKER_P, 1))
		{
			if (!m_tCapPickLoop.Waiting_Time(500)) break;
			m_tCapPickLoop.Takt_Save(9, 4);
			m_tCapPickLoop.Takt_Start();
			g_objCommon.Set_InfoCapPickerDown(0);
			g_objCommon.Move_Position(AX_CAP_PICKER_Z, 3);
			
			// Cap Clean Nozzle Up
			m_pDY09->oCapCleanUp = TRUE;
			g_objAJinAXL.Write_Output(9);
			m_nCapPickCase++; m_tCapPickLoop.Set_LoopTime(10000);
		}
		break;
	case 13:	// Picker Vac Off
		if (g_objCommon.Get_InfoCapPickerDown(0) && g_objCommon.Check_Position(AX_CAP_PICKER_Z, 3) &&
			!m_pDX09->iCapCleanDown && m_pDX09->iCapCleanUp)
		{
			if (!m_tCapPickLoop.Waiting_Time(100)) break;
			g_objCommon.Set_InfoCapPickerVacOff(0);
			g_objCommon.Set_CapBufferAlign(TRUE);

			//g_objCommon.Set_InfoCapCleanAirOn();
			m_nCapPickCase++; m_tCapPickLoop.Set_LoopTime(10000);
		}
		break;
	case 14:	// 정보전달, Picker Up
		if (g_objCommon.Get_CapBufferAlign(TRUE) && g_objCommon.Get_InfoCapPickerDown(0) && g_objCommon.Check_Position(AX_CAP_PICKER_Z, 3)) {
			if (!m_tCapPickLoop.Waiting_Time(200)) break;

			for (int i=0; i<gData.nPickerUseCnt; i++) {
				if (gData.InfoCapBuffer[i] == 0) {
					gData.InfoCapBuffer[i] = gData.InfoCapPick[i]; gData.InfoCapPick[i] = 0;

					gData.nCapNoCapBuffer[i]  = gData.nCapNoCapPicker[i];  gData.nCapNoCapPicker[i]  = 0;
					gData.nCapTNoCapBuffer[i] = gData.nCapTNoCapPicker[i]; gData.nCapTNoCapPicker[i] = 0;

					gData.sCIDCapBuffer[i] = gData.sCIDCapPicker[i];
				}
			}
			//g_objCommon.Set_CapBufferAlign(TRUE);
			g_objCommon.Set_InfoCapCleanAirOn();

			m_nCapPickCase++; m_tCapPickLoop.Set_LoopTime(10000);
		}
		break;
	case 15:	// Vac Check & Cap Buffer Go
		if (g_objCommon.Get_CapBufferAlign(TRUE) && g_objCommon.Get_InfoCapPickerVacOff(0)) {
			if (!m_tCapPickLoop.Waiting_Time(m_pEquipData->nVacOffDelay[0])) break;

			g_objCommon.Set_CapCleanAirOff();	// All Off
			m_pDY09->oCapCleanUp = FALSE;
			g_objAJinAXL.Write_Output(9);
			m_nCapPickCase++; m_tCapPickLoop.Set_LoopTime(10000);
		}
		break;
	case 16:
		if (g_objCommon.Get_CapBufferAlign(TRUE) && g_objCommon.Get_InfoCapPickerVacOff(0)) {
			g_objCommon.Set_CapPickerUp(0);
			g_objCommon.Move_Position(AX_CAP_PICKER_Z, 0);
			if (m_nCapBufferCase == 0) { m_nCapBufferCase = 1; m_tCapBufferLoop.Set_LoopTime(10000); }
			m_nCapPickCase++; m_tCapPickLoop.Set_LoopTime(10000);
		}
		break;
	case 17:	// return
		if (g_objCommon.Check_Position(AX_CAP_PICKER_Z, 0) && g_objCommon.Get_CapPickerUp(0)) {
			m_tCapPickLoop.Takt_Save(9, 5);
			m_tCapPickLoop.Takt_Start();
			
			if (m_nCapStage2Case == 20)	{ g_objCommon.Move_Position(AX_CAP_PICKER_Y, 6); }
			else						{ g_objCommon.Move_Position(AX_CAP_PICKER_Y, 0); }
			g_objCommon.Move_Position(AX_CAP_PICKER_P, 0); // Tray Pitch
			g_objCommon.Set_InfoCapPickerNormal();	//g_objCommon.Set_CapPickerNormal(0);
			g_objCommon.Set_InfoCapPickerAirOff(0);

			m_nCapPickCase++; m_tCapPickLoop.Set_LoopTime(10000);
		}
		break;
	case 18:	// Check
		if ((g_objCommon.Check_Position(AX_CAP_PICKER_Y, 0) || g_objCommon.Check_Position(AX_CAP_PICKER_Y, 6)) &&
			g_objCommon.Check_Position(AX_CAP_PICKER_Z, 0) && g_objCommon.Check_Position(AX_CAP_PICKER_P, 0) &&
			g_objCommon.Get_CapPickerUp(0) && g_objCommon.Get_InfoCapPickerNormal())
		{
			m_tCapPickLoop.Takt_Save(9, 6);
			m_nCapPickCase = 1; m_tCapPickLoop.Set_LoopTime(10000);

			m_strLog.Format("Cap Picker, %d", GetTickCount() - m_dwCapPick);
			g_objLogFile.Save_TestLog(m_strLog);
		}
		break;

	}

	// 10. (Error : 4000)
	if (m_tCapPickLoop.Over_LoopTime()) {
		g_objCommon.Show_Error(4000 + m_nCapPickCase);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 11. (Error : 4100)
BOOL CSequenceMain::CapBuffer_Run()
{
	switch (m_nCapBufferCase) {
	case 0:		// Cap Picker에서 Start
		return TRUE;

	case 1:		// 안전확인, Y Move to Clean Position
		if (g_objCommon.Get_CapBufferAlign(TRUE) && g_objCommon.Check_Position(AX_CAP_BUFFER_STAGE_Y, 0) &&
			/*!m_pDX09->iCapCoverIn && m_pDX09->iCapCoverOut && !m_pDX09->iCapCoverDown && m_pDX09->iCapCoverUp &&*/
			m_pDX09->iCapCleanDown && !m_pDX09->iCapCleanUp)
		{
			m_dwCapBuffer = GetTickCount();
			m_tCapBufferLoop.Takt_Start();

// 			g_objCommon.Move_Position(AX_CAP_BUFFER_STAGE_Y, 1);
			m_nCapBufferCase++; m_tCapBufferLoop.Set_LoopTime(5000);
		}
		break;
		// Cap Corver 기구물 장착예정 없음.
		// Cap Clean은 Cap Picker Down 상태에서 하기때문에 기존 시퀀스 삭제.
	case 2:		// Cap Cover In
		if (g_objCommon.Check_Position(AX_CAP_BUFFER_STAGE_Y, 0)) {
// 			m_tCapBufferLoop.Takt_Save(10, 1);
// 			m_tCapBufferLoop.Takt_Start();
// 			m_pDY09->oCapCoverIn = TRUE;
// 			g_objAJinAXL.Write_Output(9);
			m_nCapBufferCase++; m_tCapBufferLoop.Set_LoopTime(5000);
		}
		break;
	case 3:		// Cap Clean Up, Cover Down
//		if (m_pDX09->iCapCoverIn && !m_pDX09->iCapCoverOut) {
// 			m_tCapBufferLoop.Takt_Save(10, 2);
// 			m_tCapBufferLoop.Takt_Start();
// 			m_pDY09->oCapCoverDown = TRUE;
// 			m_pDY09->oCapCleanUp = TRUE;
// 			g_objAJinAXL.Write_Output(9);
			m_nCapBufferCase++; m_tCapBufferLoop.Set_LoopTime(5000);
//		}
		break;
	case 4:		// Cap Clean Air On
// 		if (m_pDX09->iCapCoverDown && !m_pDX09->iCapCoverUp &&
// 			!m_pDX09->iCapCleanDown && m_pDX09->iCapCleanUp)
//		{
// 			g_objCommon.Set_InfoCapCleanAirOn();
			m_nCapBufferCase++; m_tCapBufferLoop.Set_LoopTime(5000);
// 		}
		break;
	case 5:		// Cap Clean Air Delay, Air Off
// 		if (!m_tCapBufferLoop.Waiting_Time(300)) break;
// 		g_objCommon.Set_CapCleanAirOff();	// All Off
		m_nCapBufferCase++; m_tCapBufferLoop.Set_LoopTime(5000);
		break;
	case 6:		// Cap Clean Down, Cover Up
// 		if (m_pDX09->iCapCoverDown && !m_pDX09->iCapCoverUp &&
// 			!m_pDX09->iCapCleanDown && m_pDX09->iCapCleanUp)
// 		{
// 			m_pDY09->oCapCoverDown = FALSE;
// 			m_pDY09->oCapCleanUp = FALSE;
// 			g_objAJinAXL.Write_Output(9);
			m_nCapBufferCase++; m_tCapBufferLoop.Set_LoopTime(5000);
// 		}
		break;
	case 7:		// Cap Cover Out
// 		if (!m_pDX09->iCapCoverDown && m_pDX09->iCapCoverUp &&
// 			m_pDX09->iCapCleanDown && !m_pDX09->iCapCleanUp)
// 		{
// 			m_tCapBufferLoop.Takt_Save(10, 3);
// 			m_tCapBufferLoop.Takt_Start();
// 			m_pDY09->oCapCoverIn = FALSE;
// 			g_objAJinAXL.Write_Output(9);
			m_nCapBufferCase++; m_tCapBufferLoop.Set_LoopTime(5000);
// 		}
		break;
	case 8:		// Y Move to Unload Position
		if (m_pDX09->iCapCleanDown && !m_pDX09->iCapCleanUp) {
			m_tCapBufferLoop.Takt_Save(10, 4);
			m_tCapBufferLoop.Takt_Start();
			g_objCommon.Move_Position(AX_CAP_BUFFER_STAGE_Y, 2);
			m_nCapBufferCase++; m_tCapBufferLoop.Set_LoopTime(10000);
		}
		break;
	case 9:		// Position Check
		if (g_objCommon.Check_Position(AX_CAP_BUFFER_STAGE_Y, 2)) {
			m_tCapBufferLoop.Takt_Save(10, 5);
			m_nCapBufferCase = 10; m_tCapBufferLoop.Set_LoopTime(5000);
		}
		break;

	case 10:	// Unloading
		return TRUE;

	case 11:	// Cap Align In
		g_objCommon.Set_CapBufferAlign(TRUE);
		m_nCapBufferCase++; m_tCapBufferLoop.Set_LoopTime(5000);
		break;
	case 12:	// 안전확인, return
		if (g_objCommon.Get_CapBufferAlign(TRUE) && /*g_objCommon.Check_Position(AX_CAP_BUFFER_STAGE_Y, 2) &&*/
			/*!m_pDX09->iCapCoverIn && m_pDX09->iCapCoverOut && !m_pDX09->iCapCoverDown && m_pDX09->iCapCoverUp &&*/
			m_pDX09->iCapCleanDown && !m_pDX09->iCapCleanUp)
		{
			m_tCapBufferLoop.Takt_Start();
			g_objCommon.Move_Position(AX_CAP_BUFFER_STAGE_Y, 0);
			m_nCapBufferCase++; m_tCapBufferLoop.Set_LoopTime(10000);
		}
		break;
	case 13:	// Position Check
		if (g_objCommon.Check_Position(AX_CAP_BUFFER_STAGE_Y, 0)) {
			m_tCapBufferLoop.Takt_Save(10, 6);
			m_nCapBufferCase = 0; m_tCapBufferLoop.Set_LoopTime(5000);

			m_strLog.Format("CapBuffer, %d", GetTickCount() - m_dwCapBuffer);
			g_objLogFile.Save_TestLog(m_strLog);
		}
		break;

	}

	// 11. (Error : 4100)
	if (m_tCapBufferLoop.Over_LoopTime()) {
		g_objCommon.Show_Error(4100 + m_nCapBufferCase);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 12. (Error : 4200)
BOOL CSequenceMain::AssyPicker_Run()
{
	static int	nIndexNo = 0;
	static int	nApJobNo = 0;

	static double dApX = 0.0;
	static double dApY = 0.0;
	static double dApZ = 0.0;

	switch (m_nAssyPickCase) {
	case 0:		// Cap Buffer Check
		if (Check_IndexEmpty(1) && !gData.IndexDone[1]) gData.IndexDone[1] = TRUE;
		else if (m_nCapBufferCase == 10 /*&& !Check_AssyPickerFull()*/) m_nAssyPickCase++;
		return TRUE;

	case 1:		// 안전확인 및 인덱스에 모듈이 있는지 확인
		if (g_objCommon.Check_Position(AX_ASSY_PICKER_X, 0) && g_objCommon.Check_Position(AX_ASSY_PICKER_Y, 0) && g_objCommon.Check_Position(AX_ASSY_PICKER_Z, 0) &&
			g_objCommon.Get_AssyPickerUp(0) && m_pDX08->iAssyPickerTiltUp && !m_pDX08->iAssyPickerTiltDown) {
			if (Check_IndexModule()) {
				m_dwAssyPick = GetTickCount();
				m_nAssyPickCase++; m_tAssyPickLoop.Set_LoopTime(5000);
			} else {
				if (Check_IndexEmpty(1) && !gData.IndexDone[1]) gData.IndexDone[1] = TRUE;
				return TRUE;
			}
		}
		break;
	case 2:		// Z Move to Buffer Down, Picker Vac On
		if (g_objCommon.Check_Position(AX_ASSY_PICKER_X, 0) && g_objCommon.Check_Position(AX_ASSY_PICKER_Y, 0) && g_objCommon.Check_Position(AX_ASSY_PICKER_Z, 0) &&
			g_objCommon.Get_AssyPickerUp(0) && m_pDX08->iAssyPickerTiltUp && !m_pDX08->iAssyPickerTiltDown) {
			m_tAssyPickLoop.Takt_Start();
			g_objCommon.Move_Position(AX_ASSY_PICKER_Z, 1);
			g_objCommon.Set_InfoAssyPickerDown(0);
			g_objCommon.Set_InfoAssyPickerVacOn(0);
			m_nAssyPickCase++; m_tAssyPickLoop.Set_LoopTime(5000);
		}
		break;
	case 3:		// Buffer Align Out
		if (g_objCommon.Check_Position(AX_ASSY_PICKER_Z, 1) && g_objCommon.Get_InfoAssyPickerDown(0)) {
			if (!m_tAssyPickLoop.Waiting_Time(m_pEquipData->nDelayAdd[2])) break;
			g_objCommon.Set_CapBufferAlign(FALSE);
			m_nAssyPickCase++; m_tAssyPickLoop.Set_LoopTime(5000);
		}
		break;
	case 4:		// 정보전달, Vac On Delay, Z Move to Ready Up 
		if (g_objCommon.Get_CapBufferAlign(FALSE)) {
			for (int i=0; i<gData.nPickerUseCnt; i++) {
				if (gData.InfoAssyPick[i] == 0) {
					gData.InfoAssyPick[i] = gData.InfoCapBuffer[i]; gData.InfoCapBuffer[i] = 0;
					gData.nCapNoAssyPicker[i]  = gData.nCapNoCapBuffer[i];  gData.nCapNoCapBuffer[i]  = 0;
					gData.nCapTNoAssyPicker[i] = gData.nCapTNoCapBuffer[i]; gData.nCapTNoCapBuffer[i] = 0;
					gData.sCIDAssyPicker[i] = gData.sCIDCapBuffer[i];
				} else {
					if (gData.InfoAssyPick[i] > 0 && gData.InfoAssyPick[i] < 9) gData.InfoAssyPick[i] = 9;
				}
			}
			g_objCommon.Move_Position(AX_ASSY_PICKER_Z, 0);
			g_objCommon.Set_AssyPickerUp(0);
			m_nAssyPickCase++; m_tAssyPickLoop.Set_LoopTime(10000);
		}
		break;
	case 5:		// X/Y/Z Move to Inspection Position
		if (g_objCommon.Check_Position(AX_ASSY_PICKER_Z, 0) && g_objCommon.Get_AssyPickerUp(0) && g_objCommon.Get_InfoAssyPickerVacOn(0)) {
			m_tAssyPickLoop.Takt_Save(11, 1);
			m_tAssyPickLoop.Takt_Start();

			//Loadcell 측정 해야하는지 확인.
			if (m_pEquipData->nLoadCellChkCnt != 0 && m_pEquipData->nCappingCnt >= m_pEquipData->nLoadCellChkCnt) {
				nApJobNo = 0;	// 측정하는 피커 시작 번호
				dApY = m_pMoveData->dCapBufferStageY[3];
				g_objAJinAXL.Move_Absolute(AX_CAP_BUFFER_STAGE_Y, dApY);	// Load Cell Position	
				m_nAssyPickCase = 70; m_tAssyPickLoop.Set_LoopTime(10000);
			} else {
				if (m_nCapBufferCase == 10) m_nCapBufferCase = 11;
				g_objCommon.Move_Position(AX_ASSY_PICKER_X, 1);
				g_objCommon.Move_Position(AX_ASSY_PICKER_Y, 1);
				g_objCommon.Move_Position(AX_ASSY_PICKER_Z, 2);
				m_nAssyPickCase++; m_tAssyPickLoop.Set_LoopTime(10000);
			}			
		}
		break;
	case 6:		// Position Check
		if (g_objCommon.Check_Position(AX_ASSY_PICKER_X, 1) && g_objCommon.Check_Position(AX_ASSY_PICKER_Y, 1) && g_objCommon.Check_Position(AX_ASSY_PICKER_Z, 2)) {
			m_tAssyPickLoop.Takt_Save(11, 2);
			m_nAssyPickCase = 10; m_tAssyPickLoop.Set_LoopTime(5000);
		}
		break;

	case 10:	// Waiting
		return TRUE;

	case 11:	// X/Y Move to Index Position
		if (g_objCommon.Check_Position(AX_ASSY_PICKER_X, 1) && g_objCommon.Check_Position(AX_ASSY_PICKER_Y, 1) && g_objCommon.Check_Position(AX_ASSY_PICKER_Z, 2)) {
			m_tAssyPickLoop.Takt_Start();
			nIndexNo = g_objCommon.Get_IndexPos(2);
			g_objCommon.Move_Position(AX_ASSY_PICKER_X, 2+nIndexNo);
			g_objCommon.Move_Position(AX_ASSY_PICKER_Y, 2+nIndexNo);
			g_objCommon.Move_Position(AX_ASSY_PICKER_Z, 0);
			m_nAssyPickCase++; m_tAssyPickLoop.Set_LoopTime(30000);
		}
		break;
	case 12:	// Position Check
		if (g_objCommon.Check_Position(AX_ASSY_PICKER_X, 2+nIndexNo) && g_objCommon.Check_Position(AX_ASSY_PICKER_Y, 2+nIndexNo) &&
			g_objCommon.Check_Position(AX_ASSY_PICKER_Z, 0) && Check_CapAlignDone()) {
			m_nAssyPickCase = 15; m_tAssyPickLoop.Set_LoopTime(5000);
		}
		break;
	case 15:	// Index Check
		if (m_nMainIndexCase <= 10 && !gData.IndexDone[1]) {
			if (Check_IndexEmpty(1) && !gData.IndexDone[1]) { gData.IndexDone[1] = TRUE; }
			else											{ m_nAssyPickCase = 16; m_tTransStageLoop.Set_LoopTime(5000); }
		}
		return TRUE;

	// Cap Index Down
	case 16:	// Index Assy Vac Up
		m_tAssyPickLoop.Takt_Save(11, 3);
		m_tAssyPickLoop.Takt_Start();

		nIndexNo = g_objCommon.Get_IndexPos(2);
		g_objCommon.Move_Position(AX_ASSY_PICKER_X, 2+nIndexNo);
		g_objCommon.Move_Position(AX_ASSY_PICKER_Y, 2+nIndexNo);

		m_pDY11->oIndexAssyVacUp = TRUE;
		g_objAJinAXL.Write_Output(11);
		m_nAssyPickCase++; m_tAssyPickLoop.Set_LoopTime(5000);
		break;
	case 17:	// Index Assy Vac On
		if (m_pDX11->iIndexAssyVacUp && !m_pDX11->iIndexAssyVacDown) {
			m_tAssyPickLoop.Takt_Save(11, 4);
			m_tAssyPickLoop.Takt_Start();
			g_objCommon.Set_InfoIndexAssyVacuumOn();
			m_nAssyPickCase++; m_tAssyPickLoop.Set_LoopTime(5000);
		}
		break;
	case 18:	// Index Assy Align Out
		//if (g_objCommon.Get_InfoIndexAssyVacuumOn()) {	// 버큠 확인안하고 바로조립하도록한다.
			m_tAssyPickLoop.Takt_Save(11, 5);
			//m_pDY11->oIndexAssyAlignOut = TRUE;
			//g_objAJinAXL.Write_Output(11);
			if (Check_CapInspAllGood()) {
				m_nAssyPickCase++; m_tAssyPickLoop.Set_LoopTime(5000);
			} else {
				m_nAssyPickCase = 50; m_tAssyPickLoop.Set_LoopTime(5000);
			}
		//}
		break;
	case 19:	// Z Move to Index Down
		//if (!m_pDX11->iIndexAssyAlignIn && m_pDX11->iIndexAssyAlignOut) {
		if (m_pDX11->iIndexAssyAlignIn && !m_pDX11->iIndexAssyAlignOut && g_objCommon.Check_Position(AX_ASSY_PICKER_X, 2+nIndexNo) &&
			g_objCommon.Check_Position(AX_ASSY_PICKER_Y, 2+nIndexNo)) {
			SYSTEMTIME time;
			GetLocalTime(&time);

			for (int i=0; i<gData.nPickerUseCnt; i++) {
				if (gData.InfoIndex[1][i] > 0) {
					int nTNo = gData.nTNoIndex[1][i] - 1;
					int nCNo = gData.nCNoIndex[1][i] - 1;
					gData.sCapAttachStart[nTNo][nCNo].Format("%04d-%02d-%02d %02d:%02d:%02d:%03d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
				}
			}

			m_tAssyPickLoop.Takt_Start();
			g_objCommon.Set_InfoAssyPickerDown(1);
			g_objCommon.Move_Position(AX_ASSY_PICKER_Z, 3);	// Assembly Down Position
 			if (!m_pEquipData->bUseIndexAssyVac) g_objCommon.Set_IndexAssyVacuumOff(0);
			m_nAssyPickCase = 25; m_tAssyPickLoop.Set_LoopTime(5000);
		}
		break;

	// Cap Press & Tilt Check
	case 25:	// Z Move to Cap Press
		if (g_objCommon.Check_Position(AX_ASSY_PICKER_Z, 3) && g_objCommon.Get_InfoAssyPickerDown(1)) { 
			if (!m_tAssyPickLoop.Waiting_Time(m_pEquipData->nDelayAdd[5])) break;	// Assy Picker Cap Press Delay
			dApZ = m_pMoveData->dAssyPickerZ[4];	// Cap Press
			g_objAJinAXL.Move_AbsSlow(AX_ASSY_PICKER_Z, dApZ, 0.5);	// Slow
			m_nAssyPickCase++; m_tAssyPickLoop.Set_LoopTime(5000);
		}
		break;
	case 26:	// Assy Picker Vac Off
		if (g_objAJinAXL.Is_MoveDone(AX_ASSY_PICKER_Z, dApZ) && g_objCommon.Get_InfoAssyPickerDown(1)) {
			if(!m_tAssyPickLoop.Waiting_Time(100)) break;	//2021.06.23c
			g_objCommon.Set_InfoAssyPickerVacOff(0);
			m_nAssyPickCase++; m_tAssyPickLoop.Set_LoopTime(5000);
		}
		break;
	case 27:	// Z Move to Tilt Check
		if (g_objCommon.Check_Position(AX_ASSY_PICKER_Z, 4)) {
			if (!m_tAssyPickLoop.Waiting_Time(m_pEquipData->nVacOffDelay[1])) break;
			if (m_pEquipData->bUseAssyPickerTiltChk) {
				g_objCommon.Move_Position(AX_ASSY_PICKER_Z, 5);	// Tilt Position
				g_objCommon.Set_AssyPickerUp(0);
				m_nAssyPickCase++; m_tAssyPickLoop.Set_LoopTime(5000);
			} else {
				g_objCommon.Move_Position(AX_ASSY_PICKER_Z, 0);	// Ready Position
				g_objCommon.Set_AssyPickerUp(0);
				g_objCommon.Set_InfoAssyPickerAirOff(0);
				m_nAssyPickCase = 30; m_tAssyPickLoop.Set_LoopTime(5000);
			}			
		}
		break;
	case 28:	// Vac Off Check & Air Off, X/Y Axis Move to Tilt Check Position
		if (g_objCommon.Check_Position(AX_ASSY_PICKER_Z, 5) && g_objCommon.Get_AssyPickerUp(0) && g_objCommon.Get_InfoAssyPickerVacOff(0)) {
			m_tAssyPickLoop.Takt_Save(11, 6);
			m_tAssyPickLoop.Takt_Start();
			g_objCommon.Set_InfoAssyPickerAirOff(0);
			g_objCommon.Move_Position(AX_ASSY_PICKER_X, 6);	// Tilt Position
			g_objCommon.Move_Position(AX_ASSY_PICKER_Y, 6);	// Tilt Position
			m_nAssyPickCase++; m_tAssyPickLoop.Set_LoopTime(5000);
		} else {
			if (!m_pEquipData->bUseAssyPickerTiltChk) {
				g_objCommon.Move_Position(AX_ASSY_PICKER_Z, 0);	// Ready Position
				g_objCommon.Set_AssyPickerUp(0);
				g_objCommon.Set_InfoAssyPickerAirOff(0);
				m_nAssyPickCase = 30; m_tAssyPickLoop.Set_LoopTime(5000);
			}
		}
		break;
	case 29:	// Tilt Check Cylinder Down
		if (g_objCommon.Check_Position(AX_ASSY_PICKER_Z, 5) && g_objCommon.Check_Position(AX_ASSY_PICKER_X, 6) && g_objCommon.Check_Position(AX_ASSY_PICKER_Y, 6) &&
			g_objCommon.Get_AssyPickerUp(0) && g_objCommon.Get_InfoAssyPickerVacOff(0)) {
			m_pDY08->oAssyPickerTiltDown = TRUE;
			g_objAJinAXL.Write_Output(8);
			m_nAssyPickCase++; m_tAssyPickLoop.Set_LoopTime(5000);
		} else {
			if (!m_pEquipData->bUseAssyPickerTiltChk) {
				g_objCommon.Move_Position(AX_ASSY_PICKER_Z, 0);	// Ready Position
				g_objCommon.Set_AssyPickerUp(0);
				g_objCommon.Set_InfoAssyPickerAirOff(0);
				m_nAssyPickCase++; m_tAssyPickLoop.Set_LoopTime(5000);
			}
		}
		break;
	case 30:	// Tilt Check
		if ((m_pEquipData->bUseAssyPickerTiltChk && m_pDX08->iAssyPickerTiltDown && !m_pDX08->iAssyPickerTiltUp &&
			!m_pDX09->iAssyPickerTilt01 && !m_pDX09->iAssyPickerTilt02 && !m_pDX09->iAssyPickerTilt03 &&
			!m_pDX09->iAssyPickerTilt04 && !m_pDX09->iAssyPickerTilt05 && !m_pDX09->iAssyPickerTilt06/*&& !m_pDX09->iAssyPickerCrossTilt*/)
			|| !m_pEquipData->bUseAssyPickerTiltChk) {
			if(!m_tAssyPickLoop.Waiting_Time(150)) break;
			m_tAssyPickLoop.Takt_Save(11, 7);
			CString sLog;
			SYSTEMTIME time;
			GetLocalTime(&time);

			for (int i=0; i<gData.nPickerUseCnt; i++) {
				if (gData.InfoIndex[1][i] > 0) {
					int nPNo = gData.nPNoIndex[1];
					int nTNo = gData.nTNoIndex[1][i];
					int nCNo = gData.nCNoIndex[1][i];
					sLog.Format("%d,%d,-,%d,%d,%d,%s,%0.3lf,%0.3lf,%0.3lf,%0.3lf,%0.3lf,%0.3lf,%0.3lf,%0.3lf,%0.3lf,%0.3lf",
						gData.nCapTNoAssyPicker[i], gData.nCapNoAssyPicker[i], nPNo, nTNo, nCNo, gMes.sBarID[nPNo-1][nTNo-1][nCNo-1],
						gData.dAssyPickOffsetX[i], gData.dAssyPickOffsetY[i], gData.dIndexOffsetX[nIndexNo][i], gData.dIndexOffsetY[nIndexNo][i],
						gData.dAlignDiffX[i], gData.dAlignDiffY[i],
						gData.dAssyPickCapSizeX[i], gData.dAssyPickCapSizeY[i], gData.dIndexCmSizeX[nIndexNo][i], gData.dIndexCmSizeY[nIndexNo][i]);
					g_objLogFile.Save_CapAssembly(gData.nPNoIndex[1], sLog);

					if (m_pEquipData->nLoadCellChkCnt != 0) m_pEquipData->nCappingCnt++;
					
					gLot.nCapCount[nPNo-1]++;

					if (gData.InfoIndex[1][i] == 9) gData.InfoIndex[1][i] = 1;
					gData.InfoAssyPick[i] = 0;
					gData.nCapNoAssyPicker[i]  = 0;
					gData.nCapTNoAssyPicker[i] = 0;

					gData.sCapAttachEnd[nTNo-1][nCNo-1].Format("%04d-%02d-%02d %02d:%02d:%02d:%03d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
					gData.sCIDIndex[1][i] = gData.sCIDAssyPicker[i];
					//gData.sCIDCapBuffer[i].Empty();

					// CM/Cap SizeX, Y 초기화
					gData.dAssyPickCapSizeX[i] = gData.dAssyPickCapSizeY[i] = 0.0;
					gData.dIndexCmSizeX[nIndexNo][i] = gData.dIndexCmSizeY[nIndexNo][i] = 0.0;
				}
			}
			g_objCommon.Set_IndexAssyVacuumOff(0);
			m_nAssyPickCase = 35; m_tAssyPickLoop.Set_LoopTime(5000);
		}
		break;

	// Picker return
	case 35:	// Picker Up
		if (((m_pEquipData->bUseAssyPickerTiltChk && m_pDX08->iAssyPickerTiltDown && !m_pDX08->iAssyPickerTiltUp && g_objCommon.Check_Position(AX_ASSY_PICKER_Z, 5)) || 
			(!m_pEquipData->bUseAssyPickerTiltChk && !m_pDX08->iAssyPickerTiltDown && m_pDX08->iAssyPickerTiltUp && g_objCommon.Check_Position(AX_ASSY_PICKER_Z, 0))) &&
			g_objCommon.Get_AssyPickerUp(0)) {
			m_tAssyPickLoop.Takt_Start();
			m_pDY08->oAssyPickerTiltDown = FALSE;
			m_pDY11->oIndexAssyVacUp = FALSE;
			m_pDY11->oIndexAssyAlignOut = FALSE;
			g_objAJinAXL.Write_Output(8);
			g_objAJinAXL.Write_Output(11);
			g_objCommon.Move_Position(AX_ASSY_PICKER_Z, 0);
			m_nAssyPickCase++; m_tAssyPickLoop.Set_LoopTime(5000);
		}
		break;
	case 36:	// return
		if (!m_pDX08->iAssyPickerTiltDown && m_pDX08->iAssyPickerTiltUp && !m_pDX11->iIndexAssyVacUp && m_pDX11->iIndexAssyVacDown &&
			!m_pDX11->iIndexAssyAlignOut  && m_pDX11->iIndexAssyAlignIn && g_objCommon.Check_Position(AX_ASSY_PICKER_Z, 0) && g_objCommon.Get_AssyPickerUp(0)) {
			m_tAssyPickLoop.Takt_Save(11, 8);
			m_tAssyPickLoop.Takt_Start();

			if (!gData.IndexDone[1]) gData.IndexDone[1] = TRUE;

			g_objCommon.Move_Position(AX_ASSY_PICKER_X, 0);
			g_objCommon.Move_Position(AX_ASSY_PICKER_Y, 0);
			m_nAssyPickCase++; m_tAssyPickLoop.Set_LoopTime(5000);
		}
		break;
	case 37:	// Position Check
		if (g_objCommon.Check_Position(AX_ASSY_PICKER_X, 0) && g_objCommon.Check_Position(AX_ASSY_PICKER_Y, 0) && g_objCommon.Check_Position(AX_ASSY_PICKER_Z, 0) &&
			g_objCommon.Get_AssyPickerUp(0) && !m_pDX08->iAssyPickerTiltDown && m_pDX08->iAssyPickerTiltUp) {
			m_tAssyPickLoop.Takt_Save(11, 9);
			m_nAssyPickCase = 0; m_tAssyPickLoop.Set_LoopTime(5000);

			m_strLog.Format("AssyPicker, %d", GetTickCount() - m_dwAssyPick);
			g_objLogFile.Save_TestLog(m_strLog);
		}
		break;

	case 50:	// Z Move to Index Down, Good Cap Down
		//if (!m_pDX11->iIndexAssyAlignIn && m_pDX11->iIndexAssyAlignOut) {
		if (m_pDX11->iIndexAssyAlignIn && !m_pDX11->iIndexAssyAlignOut && g_objCommon.Check_Position(AX_ASSY_PICKER_X, 2+nIndexNo) &&
			g_objCommon.Check_Position(AX_ASSY_PICKER_Y, 2+nIndexNo)) {
			SYSTEMTIME time;
			GetLocalTime(&time);

			for (int i=0; i<gData.nPickerUseCnt; i++) {
				if (gData.InfoIndex[1][i] > 0 && gData.InfoAssyPick[i] == 9) {
					int nTNo = gData.nTNoIndex[1][i] - 1;
					int nCNo = gData.nCNoIndex[1][i] - 1;
					gData.sCapAttachStart[nTNo][nCNo].Format("%04d-%02d-%02d %02d:%02d:%02d:%03d",
						time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
				}
			}

			m_tAssyPickLoop.Takt_Start();
			//g_objCommon.Move_Position(AX_ASSY_PICKER_Z, 3);
			g_objCommon.Move_Position(AX_ASSY_PICKER_Z, 4);	//2021.06.23c
			g_objCommon.Set_InfoAssyPickerGoodDown();	// Good Down

			if (!m_pEquipData->bUseIndexAssyVac) g_objCommon.Set_IndexAssyVacuumOff(0);
			//m_nAssyPickCase++; m_tAssyPickLoop.Set_LoopTime(5000);
			m_nAssyPickCase = 56; m_tAssyPickLoop.Set_LoopTime(5000);	//2021.06.23c
		}
		break;
	case 51:	// Position Check
		if (g_objCommon.Check_Position(AX_ASSY_PICKER_Z, 3) && g_objCommon.Get_InfoAssyPickerGoodDown()) {
			if(!m_tAssyPickLoop.Waiting_Time(100)) break;
			if (!m_pEquipData->bUseAssyPickerVac) g_objCommon.Set_InfoAssyPickerGoodVacOff();
			m_nAssyPickCase = 55; m_tAssyPickLoop.Set_LoopTime(5000);
		}
		break;

		// Cap Press & Tilt Check
	case 55:	// Z Move to Cap Press
		if (g_objCommon.Check_Position(AX_ASSY_PICKER_Z, 3) && g_objCommon.Get_InfoAssyPickerGoodDown()) {
			dApZ = m_pMoveData->dAssyPickerZ[4];	// Cap Press
			g_objAJinAXL.Move_AbsSlow(AX_ASSY_PICKER_Z, dApZ, 0.5);	// Slow
			m_nAssyPickCase++; m_tAssyPickLoop.Set_LoopTime(5000);
		}
		break;
	case 56:	// Assy Picker Vac Off
		if (g_objAJinAXL.Is_MoveDone(AX_ASSY_PICKER_Z, dApZ) && g_objCommon.Get_InfoAssyPickerGoodDown()) {
			if(!m_tAssyPickLoop.Waiting_Time(100)) break;	//2021.06.23c
			g_objCommon.Set_InfoAssyPickerGoodVacOff();	// Good Vac Off
			m_nAssyPickCase++; m_tAssyPickLoop.Set_LoopTime(5000);
		}
		break;
	case 57:	// Z Move to Index Down, Picker Up
		if (g_objCommon.Check_Position(AX_ASSY_PICKER_Z, 4)) {
			if (!m_tAssyPickLoop.Waiting_Time(m_pEquipData->nVacOffDelay[1])) break;

			if (gData.InfoIndex[1][0] > 0 && gData.InfoAssyPick[0] == 1) { gData.InfoAssyPick[0] = 9; }
			if (gData.InfoIndex[1][1] > 0 && gData.InfoAssyPick[1] == 1) { gData.InfoAssyPick[1] = 9; }
			if (gData.InfoIndex[1][2] > 0 && gData.InfoAssyPick[2] == 1) { gData.InfoAssyPick[2] = 9; }
			if (gData.InfoIndex[1][3] > 0 && gData.InfoAssyPick[3] == 1) { gData.InfoAssyPick[3] = 9; }
			if (gData.InfoIndex[1][4] > 0 && gData.InfoAssyPick[4] == 1) { gData.InfoAssyPick[4] = 9; }
			if (gData.InfoIndex[1][5] > 0 && gData.InfoAssyPick[5] == 1) { gData.InfoAssyPick[5] = 9; }

			g_objCommon.Move_Position(AX_ASSY_PICKER_Z, 3);	// Index Down
			g_objCommon.Set_AssyPickerUp(0);
			g_objCommon.Set_InfoAssyPickerGoodAirOff();	// Good Air Off
			m_nAssyPickCase = 60; m_tAssyPickLoop.Set_LoopTime(5000);			
		}
		break;

	case 60:	// NG Check, X, Y Move
		// Align Offset X, Y 방향을 알아야한다. 카메라 기준이라면 X, Y 옵셋 값이 반대일수 있다.
		if (Select_AssyPickerNgPos(nApJobNo)) {
			nIndexNo = g_objCommon.Get_IndexPos(2);
// 			dApX = m_pMoveData->dAssyPickerX[2+nIndexNo] - gData.dAssyPickOffsetY[nApJobNo];
// 			dApY = m_pMoveData->dAssyPickerY[2+nIndexNo] + gData.dAssyPickOffsetX[nApJobNo];
			dApX = m_pMoveData->dAssyPickerX[2+nIndexNo] + gData.dAlignDiffX[nApJobNo];
			dApY = m_pMoveData->dAssyPickerY[2+nIndexNo] + gData.dAlignDiffY[nApJobNo];
			g_objAJinAXL.Move_Absolute(AX_ASSY_PICKER_X, dApX);
			g_objAJinAXL.Move_Absolute(AX_ASSY_PICKER_Y, dApY);

			m_nAssyPickCase++; m_tAssyPickLoop.Set_LoopTime(5000);
		} else {
			if (g_objAJinAXL.Is_Done(AX_ASSY_PICKER_Z)) {
				if (m_pEquipData->bUseAssyPickerTiltChk) {
					g_objCommon.Move_Position(AX_ASSY_PICKER_Z, 5);	// Tilt Position
					g_objCommon.Set_AssyPickerUp(0);
// 					m_pDY08->oAssyPickerTiltDown = TRUE;
// 					g_objAJinAXL.Write_Output(8);
					m_nAssyPickCase = 28; m_tAssyPickLoop.Set_LoopTime(5000);
				} else {
					g_objCommon.Move_Position(AX_ASSY_PICKER_Z, 0);	// Ready Position
					g_objCommon.Set_AssyPickerUp(0);
					g_objCommon.Set_InfoAssyPickerAirOff(0);
					m_nAssyPickCase = 30; m_tAssyPickLoop.Set_LoopTime(5000);
				}
			}
		}
		break;
	case 61:	// Picker Down
		if (m_pDX11->iIndexAssyAlignIn && !m_pDX11->iIndexAssyAlignOut && g_objAJinAXL.Is_Done(AX_ASSY_PICKER_Z) &&
			g_objAJinAXL.Is_MoveDone(AX_ASSY_PICKER_X, dApX) && g_objAJinAXL.Is_MoveDone(AX_ASSY_PICKER_Y, dApY)) {
			SYSTEMTIME time;
			GetLocalTime(&time);
			//for (int i=0; i<gData.nPickerUseCnt; i++) {
				if (gData.InfoAssyPick[nApJobNo] == 2) {
					int nTNo = gData.nTNoIndex[1][nApJobNo] - 1;
					int nCNo = gData.nCNoIndex[1][nApJobNo] - 1;
					gData.sCapAttachStart[nTNo][nCNo].Format("%04d-%02d-%02d %02d:%02d:%02d:%03d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
				}
			//}

			m_tAssyPickLoop.Takt_Start();
			//g_objCommon.Move_Position(AX_ASSY_PICKER_Z, 3);
			g_objCommon.Move_Position(AX_ASSY_PICKER_Z, 4);	//2021.06.23c
			g_objCommon.Set_AssyPickerDown(nApJobNo+1);

			if (!m_pEquipData->bUseIndexAssyVac) g_objCommon.Set_IndexAssyVacuumOff(0);
			//m_nAssyPickCase++; m_tAssyPickLoop.Set_LoopTime(5000);	//2021.06.23c
			m_nAssyPickCase = 66; m_tAssyPickLoop.Set_LoopTime(5000);
		}
		break;
	case 62:	// Position Check
		if (g_objCommon.Check_Position(AX_ASSY_PICKER_Z, 3) && g_objCommon.Get_AssyPickerDown(nApJobNo+1)) {
			if(!m_tAssyPickLoop.Waiting_Time(100)) break;
			if (!m_pEquipData->bUseAssyPickerVac) g_objCommon.Set_AssyPickerVacOff(nApJobNo+1);
			m_nAssyPickCase = 65; m_tAssyPickLoop.Set_LoopTime(5000);
		}
		break;

	// Cap Press & Tilt Check
	case 65:	// Z Move to Cap Press
		if (g_objCommon.Check_Position(AX_ASSY_PICKER_Z, 3) && g_objCommon.Get_AssyPickerDown(nApJobNo+1)) {
			dApZ = m_pMoveData->dAssyPickerZ[4];	// Cap Press
			g_objAJinAXL.Move_AbsSlow(AX_ASSY_PICKER_Z, dApZ, 0.5);	// Slow
			m_nAssyPickCase++; m_tAssyPickLoop.Set_LoopTime(5000);
		}
		break;
	case 66:	// Assy Picker Vac Off
		if (g_objAJinAXL.Is_MoveDone(AX_ASSY_PICKER_Z, dApZ) && g_objCommon.Get_AssyPickerDown(nApJobNo+1)) {
			if(!m_tAssyPickLoop.Waiting_Time(100)) break;	//2021.06.23c
			g_objCommon.Set_AssyPickerVacOff(nApJobNo+1);
			m_nAssyPickCase++; m_tAssyPickLoop.Set_LoopTime(5000);
		}
		break;
	case 67:	// Z Move to Index Down, Picker Up
		if (g_objCommon.Check_Position(AX_ASSY_PICKER_Z, 4)) {
			if (!m_tAssyPickLoop.Waiting_Time(m_pEquipData->nVacOffDelay[1])) break;

			if (gData.InfoIndex[1][nApJobNo] > 0 && gData.InfoAssyPick[nApJobNo] == 2) { gData.InfoAssyPick[nApJobNo] = 9; }
			g_objCommon.Move_Position(AX_ASSY_PICKER_Z, 3);	// Index Down
			g_objCommon.Set_AssyPickerUp(0);
			m_nAssyPickCase = 60; m_tAssyPickLoop.Set_LoopTime(5000);			
		}
		break;

	// Load Cell 측정.
	case 70:	// Assy Picker Cylinder Down
		if (g_objCommon.Get_AssyPickerUp(0) && g_objCommon.Check_Position(AX_ASSY_PICKER_Z, 0) && g_objAJinAXL.Is_MoveDone(AX_CAP_BUFFER_STAGE_Y, dApY)) {
			nApJobNo++;
			if (nApJobNo > gData.nPickerUseCnt) {
				nApJobNo = 0;
				m_pEquipData->nCappingCnt = 0;

				CCME8000Dlg *pMainDlg = (CCME8000Dlg*)AfxGetApp()->m_pMainWnd;
				pMainDlg->Save_EquipCappingCnt();

				g_objCommon.Move_Position(AX_ASSY_PICKER_Z, 0);	// Ready Up
				g_objCommon.Set_AssyPickerUp(0);

				m_nAssyPickCase = 5; m_tAssyPickLoop.Set_LoopTime(10000);
			} else {
				SYSTEMTIME time;
				GetLocalTime(&time);
				m_strLoadCellStart.Format("%04d-%02d-%02d %02d:%02d:%02d:%03d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);

				g_objCommon.Set_AssyPickerDown(nApJobNo);
				m_nAssyPickCase++; m_tAssyPickLoop.Set_LoopTime(10000);
			}
		}
		break;
	case 71:	// Assy Picker Z Move to Load Cell Position
		if (g_objCommon.Get_AssyPickerDown(nApJobNo) && g_objAJinAXL.Is_MoveDone(AX_CAP_BUFFER_STAGE_Y, dApY)) {
			dApZ = m_pMoveData->dAssyPickerZ[6];	// Load Cell Down
			g_objAJinAXL.Move_AbsSlow(AX_ASSY_PICKER_Z, dApZ, 0.5);	// Load Cell Down Position
			m_nAssyPickCase++; m_tAssyPickLoop.Set_LoopTime(10000);
		}
		break;
	case 72:	// 일정시간 Dealy 후 Load Cell Check
		if (g_objCommon.Get_AssyPickerDown(nApJobNo) && g_objAJinAXL.Is_MoveDone(AX_ASSY_PICKER_Z, dApZ)) {
			if (!m_tAssyPickLoop.Waiting_Time(m_pEquipData->nDelayAdd[4])) break;	// 측정 전 대기시간
			gData.dLoadCell[0] = 0.0;
			g_objLoadCell.Get_Weight(ASSY_LOAD_CELL);
			m_nAssyPickCase++; m_tAssyPickLoop.Set_LoopTime(10000);
		}
		break;
	case 73:	// 측정값 확인 및 Picker Up
		if (gData.bLoadCellComplete) {
			if (gData.dLoadCell[0] >= m_pEquipData->dAssyPickForce[0] && gData.dLoadCell[0] <= m_pEquipData->dAssyPickForce[1]) {
				gData.dAssyLoadCell[nApJobNo-1] = gData.dLoadCell[0];

				SYSTEMTIME time;
				GetLocalTime(&time);
				m_strLoadCellEnd.Format("%04d-%02d-%02d %02d:%02d:%02d:%03d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);

				CString strLog;
				strLog.Format("%s,%s,%d,%0.3lf", m_strLoadCellStart, m_strLoadCellEnd, nApJobNo, gData.dLoadCell[0]);
				g_objLogFile.Save_LoadCellLog(gData.nPNoIndex[1], strLog);

				g_objCommon.Move_Position(AX_ASSY_PICKER_Z, 0);
				g_objCommon.Set_AssyPickerUp(0);
				m_nAssyPickCase++; m_tAssyPickLoop.Set_LoopTime(10000);
			}
		}
		break;
	case 74:	// Buffer Stage Y Pitch Move
		if (g_objCommon.Get_AssyPickerUp(0) && g_objCommon.Check_Position(AX_ASSY_PICKER_Z, 0)) {
			if (nApJobNo < gData.nPickerUseCnt) {
				dApY = m_pMoveData->dCapBufferStageY[3] + nApJobNo * m_pEquipData->dIndexPitch;
				g_objAJinAXL.Move_Absolute(AX_CAP_BUFFER_STAGE_Y, dApY);
			}
			m_nAssyPickCase = 70; m_tAssyPickLoop.Set_LoopTime(10000);
		}
		break;

	case 75:	// Error 후 처리
		if (g_objCommon.Get_AssyPickerUp(0) && g_objCommon.Check_Position(AX_ASSY_PICKER_Z, 0)) {
			if (m_pEquipData->nLoadCellChkCnt == 0) {	// Load Cell 측정 사용 안함. (
				m_pEquipData->nCappingCnt = 0;
				CCME8000Dlg *pMainDlg = (CCME8000Dlg*)AfxGetApp()->m_pMainWnd;
				pMainDlg->Save_EquipCappingCnt();
				m_nAssyPickCase = 5; m_tAssyPickLoop.Set_LoopTime(10000);
			} else {
				nApJobNo = 0;
				dApY = m_pMoveData->dCapBufferStageY[3];
				g_objAJinAXL.Move_Absolute(AX_CAP_BUFFER_STAGE_Y, dApY);	// Load Cell Position	
				m_nAssyPickCase = 70; m_tAssyPickLoop.Set_LoopTime(10000);
			}
		}
		break;
	}

	// 12. (Error : 4200)
	if (m_tAssyPickLoop.Over_LoopTime()) {
		g_objCommon.Show_Error(4200 + m_nAssyPickCase);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 13. (Error : 4300)
BOOL CSequenceMain::TransStage_Run()
{
	switch (m_nTransStageCase) {
	case 0:		// Index Check
		if (m_nMainIndexCase <= 10 && !gData.IndexDone[2]) {
			m_nTransStageCase = 1; m_tTransStageLoop.Set_LoopTime(5000);
		}
		return TRUE;

	case 1:		// 안전확인
		// Index가 비어 있으면 True로 바꿔준다.
		if (Check_IndexEmpty(2) && !gData.IndexDone[2]) {
			gData.IndexDone[2] = TRUE;
			m_nTransStageCase = 0; m_tTransStageLoop.Set_LoopTime(5000);

		} else if (g_objCommon.Check_Position(AX_TRANS_STAGE_X, 0) &&
			g_objCommon.Check_Position(AX_TRANS_STAGE_Z, 0) &&
			g_objCommon.Check_Position(AX_TRANS_STAGE_T, 0))
		{
			m_dwTransStage = GetTickCount();
			m_nTransStageCase++; m_tTransStageLoop.Set_LoopTime(5000);
		}
		break;
	case 2:		// Clamp Off Check & Z Move to Index Down, Vac On
		if (g_objCommon.Get_TransStageClampOff()) {
			m_tTransStageLoop.Takt_Start();

			g_objCommon.Move_Position(AX_TRANS_STAGE_Z, 1);	// Index Down
			m_nTransStageCase++; m_tTransStageLoop.Set_LoopTime(5000);
		}
		break;
	case 3:		// Index Align Out
		if (g_objCommon.Check_Position(AX_TRANS_STAGE_Z, 1)) {
			m_tTransStageLoop.Takt_Save(12, 1);
			m_tTransStageLoop.Takt_Start();
			g_objCommon.Set_InfoTransStageVacOn(0);	// Vac 사용안함
			//g_objCommon.Set_TransStageVacOff(0);	// Blower 사용 할꺼면 주석 삭제 
			m_pDY11->oIndexTransAlignOut = TRUE;
			g_objAJinAXL.Write_Output(11);
			m_nTransStageCase++; m_tTransStageLoop.Set_LoopTime(5000);
		}
		break;
	case 4:		// 정보전달, Clamp On
		if (m_pDX11->iIndexTransAlignOut && !m_pDX11->iIndexTransAlignIn) {
			m_tTransStageLoop.Takt_Save(12, 2);
			m_tTransStageLoop.Takt_Start();
			for (int i=0; i<gData.nPickerUseCnt; i++) {
				gData.InfoTransStage[i] = gData.InfoIndex[2][i]; gData.InfoIndex[2][i] = 0;
				gData.nTNoTransStage[i] = gData.nTNoIndex[2][i]; gData.nTNoIndex[2][i] = 0;
				gData.nCNoTransStage[i] = gData.nCNoIndex[2][i]; gData.nCNoIndex[2][i] = 0;

				gData.sCIDTransStage[i] = gData.sCIDIndex[2][i];
			}
			gData.nPNoTransStage = gData.nPNoIndex[2];
			gData.nPNoIndex[2] = 0;
			gData.nINoTransStage = g_objCommon.Get_IndexPos(3);

// 			if (gData.nCNoIndex[4][0] > 0) {
// 				int nLineNo = gData.nCNoIndex[4][0] / m_pEquipData->nLoadTrayX;
// 				int nTNo = gData.nTNoIndex[2] - 1;
// 				gData.dwCmTactStart[nTNo][nLineNo] = GetTickCount();
// 			}

			g_objCommon.Set_TransStageClampOn();
			m_nTransStageCase++; m_tTransStageLoop.Set_LoopTime(5000);
		}
		break;
	case 5:		// Z Move to Ready Up
		if (g_objCommon.Get_TransStageClampOn()) {
			if (!m_tTransStageLoop.Waiting_Time(300)) break;
			m_tTransStageLoop.Takt_Save(12, 3);
			m_tTransStageLoop.Takt_Start();
			g_objCommon.Move_Position(AX_TRANS_STAGE_Z, 0);	//Ready Up
			m_nTransStageCase++; m_tTransStageLoop.Set_LoopTime(30000);
		}
		break;
	case 6:		// Index Align In & X, T Move to Unload Position
		if (/*g_objCommon.Get_InfoTransStageVacOn(1) && */g_objCommon.Check_Position(AX_TRANS_STAGE_Z, 0)) {
			m_tTransStageLoop.Takt_Save(12, 4);
			m_tTransStageLoop.Takt_Start();
			g_objCommon.Move_Position(AX_TRANS_STAGE_X, 1);	// Unload
			g_objCommon.Move_Position(AX_TRANS_STAGE_T, 1);	// Unload

			m_pDY11->oIndexTransAlignOut = FALSE;
			g_objAJinAXL.Write_Output(11);
			m_nTransStageCase++; m_tTransStageLoop.Set_LoopTime(20000);
		}
		break;
	case 7:		// Align In Check & Index Done
		if (!m_pDX11->iIndexTransAlignOut && m_pDX11->iIndexTransAlignIn) {
			m_tTransStageLoop.Takt_Save(12, 5);
			gData.IndexDone[2] = TRUE;
			m_nTransStageCase++; m_tTransStageLoop.Set_LoopTime(5000);
		}
		break;
	case 8:		// X, T Position Check & Z Move to Unload Up
		if (g_objCommon.Check_Position(AX_TRANS_STAGE_X, 1) && g_objCommon.Check_Position(AX_TRANS_STAGE_T, 1)) {
			m_tTransStageLoop.Takt_Save(12, 6);
			m_tTransStageLoop.Takt_Start();
			//g_objCommon.Set_TransStageVacOff(0);
			g_objCommon.Move_Position(AX_TRANS_STAGE_Z, 2);
			m_nTransStageCase++; m_tTransStageLoop.Set_LoopTime(5000);
		}
		break;
	case 9:		// Position Check
		if (g_objCommon.Check_Position(AX_TRANS_STAGE_Z, 2)) {
			m_tTransStageLoop.Takt_Save(12, 7);
			m_nTransStageCase++; m_tTransStageLoop.Set_LoopTime(5000);
		}
		break;
	case 10:	// Unloading
		return TRUE;

	case 11:	// Z Move to Ready Up
		if (g_objCommon.Check_Position(AX_TRANS_STAGE_Z, 2) && g_objCommon.Get_TransStageClampOff()) {
			m_tTransStageLoop.Takt_Start();
			g_objCommon.Move_Position(AX_TRANS_STAGE_Z, 0);
			m_nTransStageCase++; m_tTransStageLoop.Set_LoopTime(5000);
		}
		break;
	case 12:	// return
		if (g_objCommon.Check_Position(AX_TRANS_STAGE_Z, 0)) {
			m_tTransStageLoop.Takt_Save(12, 8);
			m_tTransStageLoop.Takt_Start();
			g_objCommon.Move_Position(AX_TRANS_STAGE_X, 0);	// Load
			g_objCommon.Move_Position(AX_TRANS_STAGE_T, 0);	// Load
			m_nTransStageCase++; m_tTransStageLoop.Set_LoopTime(5000);
		}
		break;
	case 13:	// Position Check
		if (g_objCommon.Check_Position(AX_TRANS_STAGE_X, 0) && g_objCommon.Check_Position(AX_TRANS_STAGE_T, 0)) {
			m_tTransStageLoop.Takt_Save(12, 9);
			m_nTransStageCase = 0; m_tTransStageLoop.Set_LoopTime(5000);

			m_strLog.Format("TransStage, %d", GetTickCount() - m_dwTransStage);
			g_objLogFile.Save_TestLog(m_strLog);
		}
		break;
	}

	// 13. (Error : 4300)
	if (m_tTransStageLoop.Over_LoopTime()) {
		g_objCommon.Show_Error(4300 + m_nTransStageCase);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 14. (Error : 4400)
BOOL CSequenceMain::UnloadPicker_Run()
{
	static int	nUpWorkTray= 0;
	
	static int nUpPosX = 0;			// Tray X
	static int nUpPosY = 0;			// Tray Y
	static int nUpStart = 0;		// Picker Start No
	static int nUpPickCnt = 0;		// Picker가 잡을수 있는 수
	static int nUpPosCnt = 0;		// Tray 작업가능수
	static int nUpDownSu = 0;		// 최종적으로 Picker가 Down되는 수

	static int nUpTrayLine = 0;		// Tray Line
	static int nUpTrayCnt = 0;		// Tray Cnt

	static double dUpY = 0.0;		// Picker Y
	static double dUpX = 0.0;		// Tray X

	switch (m_nUnloadPickCase) {
	case 0:		// Trans Stage Check
		if (m_nTransStageCase == 10) {
			m_nUnloadPickCase++; m_tUnloadPickLoop.Set_LoopTime(5000);
		}

		if (m_pEquipData->bUseInlineMode) {
			// Avi Lotend가 늦게 들어오거나 타이밍이 안맞아 Unload Picker가 지나쳤을 경우 대기하는데에서 확인해준다.
			if (gData.nTNoUnloadTray != 0 && (gData.nTNoUnloadTray == gData.nLastTrayNo[gData.nPNoUnloadTray-1]) && gData.nLastTrayNo[gData.nPNoUnloadTray-1] > 0 &&
				/*!gData.bCapLotEnd[gData.nPNoUnloadTray-1] &&*/ Check_LoadTrayLoading(gData.nPNoUnloadTray))
			{

				Job_LotEnd(gData.nPNoUnloadTray);
				gData.nLastTrayNo[gData.nPNoUnloadTray-1] = 0;
				if (m_pThreadBeep == NULL) {
					m_pThreadBeep = AfxBeginThread(Thread_Beep, (LPVOID)(2000));
				}
				gData.nTNoUnloadTray = 0;
				g_dlgWork.PostMessage(UM_SHOW_MSG, 2, NULL);		// 2020.09.14 khs
				if (m_nUnloadStage1Case == 20) { gData.bUnloadTrayLotEnd[0] = TRUE; m_nUnloadStage1Case = 21; }
				if (m_nUnloadStage2Case == 20) { gData.bUnloadTrayLotEnd[1] = TRUE; m_nUnloadStage2Case = 21; }

				// 배출중일때 초기화 할수있도록 gData.bUnloadTrayLotEnd = TRUE
				if (gData.bUnloadTrayLotEnd[0] == FALSE && m_nUnloadStage1Case > 20 && m_nUnloadStage1Case < 30) { gData.bUnloadTrayLotEnd[0] = TRUE; } 
				if (gData.bUnloadTrayLotEnd[1] == FALSE && m_nUnloadStage2Case > 20 && m_nUnloadStage2Case < 30) { gData.bUnloadTrayLotEnd[1] = TRUE; } 

				// 마지막 Tray 배출 이후 다른 스테이지 Empty Tray가 대기위치에 없을때는 여기에서 초기화 해준다.
				if ((m_nUnloadStage1Case >= 30 && m_nUnloadStage2Case < 20) ||
					(m_nUnloadStage2Case >= 30 && m_nUnloadStage1Case < 20) ) {

					m_pDY03->oUnloadPort2SlideLock = FALSE; m_pDY03->oUnloadPort2SlideUnlock = TRUE;
					g_objAJinAXL.Write_Output(3);
					gData.bUnloadPort2Wait = TRUE;
					gData.nPNoUnloadTray = 0;
				}
			}
		}
		return TRUE;

	case 1:		// 안전확인 & Move to Trans Down
		if (g_objCommon.Check_Position(AX_UNLOAD_PICKER_X, 0) && g_objCommon.Check_Position(AX_UNLOAD_PICKER_Z, 0) &&
			g_objCommon.Check_Position(AX_UNLOAD_PICKER_P, 0) && g_objCommon.Get_UnloadPickerUp(0) &&
			g_objCommon.Check_Position(AX_TRANS_STAGE_Z, 2))
		{
			m_dwUnloadPick = GetTickCount();
			m_tUnloadPickLoop.Takt_Start();

			g_objCommon.Move_Position(AX_UNLOAD_PICKER_Z, 1);
			g_objCommon.Set_InfoUnloadPickerDown(0);
// 			g_objCommon.Set_InfoUnloadPickerVacOn(0);
			m_nUnloadPickCase++; m_tUnloadPickLoop.Set_LoopTime(5000);
		}		
		break;
	case 2:		// Trans Stage Clamp Off
		if (g_objCommon.Check_Position(AX_UNLOAD_PICKER_Z, 1) && g_objCommon.Get_InfoUnloadPickerDown(0))
		{
			m_tUnloadPickLoop.Takt_Save(13, 1);
			m_tUnloadPickLoop.Takt_Start();
			g_objCommon.Set_TransStageClampOff();
			g_objCommon.Set_TransStageVacOff(0);
			m_nUnloadPickCase++; m_tUnloadPickLoop.Set_LoopTime(5000);
		}
		break;
	case 3:		// 정보전달, Picker Vac On
		if (g_objCommon.Get_TransStageClampOff()) {
// 			if (!m_tUnloadPickLoop.Waiting_Time(m_pEquipData->nDelayAdd[3])) break;
			m_tUnloadPickLoop.Takt_Save(13, 2);
			m_tUnloadPickLoop.Takt_Start();

			for (int i=0; i<gData.nPickerUseCnt; i++) {
				gData.InfoUnloadPick[i] = gData.InfoTransStage[i]; gData.InfoTransStage[i] = 0;
				gData.nTNoUnloadPick[i] = gData.nTNoTransStage[i]; gData.nTNoTransStage[i] = 0;
				gData.nCNoUnloadPick[i] = gData.nCNoTransStage[i]; gData.nCNoTransStage[i] = 0;

				gData.sCIDUnloadPicker[i] = gData.sCIDTransStage[i];
			}
			gData.nPNoUnloadPick = gData.nPNoTransStage; gData.nPNoTransStage = 0;
			gData.nINoUnloadPicker = gData.nINoTransStage;
			gData.nLNoUnloadPicker = gData.nCNoUnloadPick[0] / gData.nPickerUseCnt;
			//gData.nLNoUnloadPicker = gData.nCNoUnloadPick[0] / m_pEquipData->nLoadTrayX;

			// g_objCommon.Set_TransStageAirOff(0);
			g_objCommon.Set_InfoUnloadPickerVacOn(1);
// 			g_objCommon.Move_Position(AX_UNLOAD_PICKER_Z, 0);
// 			g_objCommon.Set_UnloadPickerUp(0);
			m_nUnloadPickCase++; m_tUnloadPickLoop.Set_LoopTime(5000);
		}
		break;
	case 4:		// Vac On Delay, Picker Up
// 		if (g_objCommon.Get_InfoUnloadPickerVacOn(1) && g_objCommon.Get_UnloadPickerUp(0) && g_objCommon.Check_Position(AX_UNLOAD_PICKER_Z, 0)) {
		if (g_objCommon.Get_TransStageClampOff()) {
			if (!m_tUnloadPickLoop.Waiting_Time(m_pEquipData->nDelayAdd[3])) break;

			m_tUnloadPickLoop.Takt_Save(13, 3);
// 			if (m_nTransStageCase == 10) m_nTransStageCase = 11;

			g_objCommon.Move_Position(AX_UNLOAD_PICKER_Z, 0);
			g_objCommon.Set_UnloadPickerUp(0);
			g_objCommon.Set_TransStageAirOff(0);
			
			m_nUnloadPickCase++; m_tUnloadPickLoop.Set_LoopTime(5000);		
		}
		break;
	case 5:		// Check Picker Up, Move to Unload Stage1 Position
		if (g_objCommon.Get_InfoUnloadPickerVacOn(1) && g_objCommon.Get_UnloadPickerUp(0) && g_objCommon.Check_Position(AX_UNLOAD_PICKER_Z, 0)) {
			m_tUnloadPickLoop.Takt_Start();

			if (m_nTransStageCase == 10) m_nTransStageCase = 11;	// Trans Stage Loading 위치로 이동.
			if (m_nUnloadStage1Case == 20 || m_nUnloadStage2Case == 20) {
				m_nUnloadPickCase = 10; m_tUnloadPickLoop.Set_LoopTime(5000);

			} else {
				// Unload Stage 대기위치에 없으면 언로드 대기 위치로 이동.
				g_objCommon.Move_Position(AX_UNLOAD_PICKER_X, 1);
				g_objCommon.Move_Position(AX_UNLOAD_PICKER_P, 1);
				m_nUnloadPickCase++; m_tUnloadPickLoop.Set_LoopTime(5000);
			}	
		}
		break;

	case 6:		// wait
		if (g_objCommon.Get_InfoUnloadPickerVacOn(1) && g_objCommon.Get_InfoUnloadPickerCapCheck() &&
			g_objCommon.Check_Position(AX_UNLOAD_PICKER_X, 1) && g_objCommon.Check_Position(AX_UNLOAD_PICKER_P, 1)) {
			if (m_nUnloadStage1Case != 20 && m_nUnloadStage2Case != 20) return TRUE;

			m_tUnloadPickLoop.Takt_Save(13, 4);
			m_nUnloadPickCase = 10; m_tUnloadPickLoop.Set_LoopTime(5000);
		}
		break;

	case 10:		
		if (g_objCommon.Get_InfoUnloadPickerVacOn(1) && g_objCommon.Get_UnloadPickerUp(0) &&
			(g_objCommon.Check_Position(AX_UNLOAD_PICKER_Z, 0) || g_objCommon.Check_Position(AX_UNLOAD_PICKER_Z, nUpWorkTray+1)))
		{
			
			if (gData.nPNoUnloadPick != gData.nPNoUnloadTray && gData.nPNoUnloadTray > 0) {
				if (m_pEquipData->bUseInlineMode) {
					// Avi Lotend가 늦게 들어오거나 타이밍이 안맞아 Unload Picker가 지나쳤을 경우 대기하는데에서 확인해준다.
					if (gData.nTNoUnloadTray != 0 && (gData.nTNoUnloadTray == gData.nLastTrayNo[gData.nPNoUnloadTray-1]) && gData.nLastTrayNo[gData.nPNoUnloadTray-1] > 0 &&
						/*!gData.bCapLotEnd[gData.nPNoUnloadTray-1] &&*/ Check_LoadTrayLoading(gData.nPNoUnloadTray))
					{
						Job_LotEnd(gData.nPNoUnloadTray);
						gData.nLastTrayNo[gData.nPNoUnloadTray-1] = 0;
						if (m_pThreadBeep == NULL) {
							m_pThreadBeep = AfxBeginThread(Thread_Beep, (LPVOID)(2000));
						}
						gData.nTNoUnloadTray = 0;
						g_dlgWork.PostMessage(UM_SHOW_MSG, 2, NULL);		// 2020.09.14 khs
						if (m_nUnloadStage1Case == 20) { gData.bUnloadTrayLotEnd[0] = TRUE; m_nUnloadStage1Case = 21; }
						if (m_nUnloadStage2Case == 20) { gData.bUnloadTrayLotEnd[1] = TRUE; m_nUnloadStage2Case = 21; }

						// 배출중일때 초기화 할수있도록 gData.bUnloadTrayLotEnd = TRUE
						if (gData.bUnloadTrayLotEnd[0] == FALSE && m_nUnloadStage1Case > 20 && m_nUnloadStage1Case < 30) { gData.bUnloadTrayLotEnd[0] = TRUE; } 
						if (gData.bUnloadTrayLotEnd[1] == FALSE && m_nUnloadStage2Case > 20 && m_nUnloadStage2Case < 30) { gData.bUnloadTrayLotEnd[1] = TRUE; } 

						// 마지막 Tray 배출 이후 다른 스테이지 Empty Tray가 대기위치에 없을때는 여기에서 초기화 해준다.
						if ((m_nUnloadStage1Case >= 30 && m_nUnloadStage2Case < 20) ||
							(m_nUnloadStage2Case >= 30 && m_nUnloadStage1Case < 20) ) {

							m_pDY03->oUnloadPort2SlideLock = FALSE; m_pDY03->oUnloadPort2SlideUnlock = TRUE;
							g_objAJinAXL.Write_Output(3);
							gData.bUnloadPort2Wait = TRUE;
							gData.nPNoUnloadTray = 0;
						}
					}
				}
				return TRUE;
			}
			if (m_nUnloadStage1Case != 20 && m_nUnloadStage2Case != 20) return TRUE;

			if (m_nUnloadStage1Case == 20) { nUpWorkTray = 1; dUpY = g_objAJinAXL.Get_Position(AX_UNLOAD_STAGE1_Y); }
			if (m_nUnloadStage2Case == 20) { nUpWorkTray = 2; dUpY = g_objAJinAXL.Get_Position(AX_UNLOAD_STAGE2_Y); }
			dUpX = g_objAJinAXL.Get_Position(AX_UNLOAD_PICKER_X);

			m_nUnloadPickCase++; m_tUnloadPickLoop.Set_LoopTime(5000);
		}
		break;
	case 11:	// Move to Unload Tray, Tray Position Check
		if (((nUpWorkTray == 1 && g_objAJinAXL.Is_MoveDone(AX_UNLOAD_STAGE1_Y, dUpY)) ||
			 (nUpWorkTray == 2 && g_objAJinAXL.Is_MoveDone(AX_UNLOAD_STAGE2_Y, dUpY))) &&
			g_objAJinAXL.Is_MoveDone(AX_UNLOAD_PICKER_X, dUpX))
		{
			if (m_nUnloadStage1Case != 20 && m_nUnloadStage2Case != 20) break;

			if (Select_UnloadPickerPos(nUpStart, nUpPickCnt)) {	//0,3
				if (Select_UnloadTrayPos(nUpPosX, nUpPosY, nUpPosCnt)){	//0,0,3
					m_tUnloadPickLoop.Takt_Start();

					if (nUpPosCnt == 0) nUpTrayCnt = m_pEquipData->nUnloadTrayX - nUpPosX;
					else				nUpTrayCnt = nUpPosCnt;
					nUpDownSu = ((nUpPickCnt < nUpTrayCnt) ? nUpPickCnt : nUpTrayCnt);
					m_nUnloadPickMultiCnt = nUpDownSu;

					dUpX = m_pMoveData->dUnloadPickerX[nUpWorkTray] - (nUpPosX - nUpStart) * m_pEquipData->dUnloadTrayPitchX;
					if (nUpWorkTray == 1) dUpY = m_pMoveData->dUnloadStage1Y[1] + nUpPosY * m_pEquipData->dUnloadTrayPitchY;
					if (nUpWorkTray == 2) dUpY = m_pMoveData->dUnloadStage2Y[1] + nUpPosY * m_pEquipData->dUnloadTrayPitchY;					

					g_objAJinAXL.Move_Absolute(AX_UNLOAD_PICKER_X, dUpX);		
					if (nUpWorkTray == 1) g_objAJinAXL.Move_Absolute(AX_UNLOAD_STAGE1_Y, dUpY);
					if (nUpWorkTray == 2) g_objAJinAXL.Move_Absolute(AX_UNLOAD_STAGE2_Y, dUpY);

					g_objCommon.Move_Position(AX_UNLOAD_PICKER_P, 1);

					m_nUnloadPickCase++; m_tUnloadPickLoop.Set_LoopTime(10000);
				}
			}
		}
		break;
	case 12:	// Picker Down
		if (((nUpWorkTray == 1 && g_objAJinAXL.Is_MoveDone(AX_UNLOAD_STAGE1_Y, dUpY)) ||
			(nUpWorkTray == 2 && g_objAJinAXL.Is_MoveDone(AX_UNLOAD_STAGE2_Y, dUpY))) &&
			g_objAJinAXL.Is_MoveDone(AX_UNLOAD_PICKER_X, dUpX) && g_objCommon.Check_Position(AX_UNLOAD_PICKER_P, 1) &&
			g_objCommon.Get_InfoUnloadPickerCapCheck()) {
			m_tUnloadPickLoop.Takt_Save(13, 5);
			m_tUnloadPickLoop.Takt_Start();

			g_objCommon.Move_Position(AX_UNLOAD_PICKER_Z, nUpWorkTray+1);
			g_objCommon.Set_UnloadPickerDownMulti(nUpStart+1, nUpDownSu);
			m_nUnloadPickCase++; m_tUnloadPickLoop.Set_LoopTime(10000);
		}
		break;
	case 13:	// Vac Off
		if (g_objCommon.Check_Position(AX_UNLOAD_PICKER_Z, nUpWorkTray+1) && g_objCommon.Get_UnloadPickerDownMulti(nUpStart+1, nUpDownSu)) {
			g_objCommon.Set_UnloadPickerVacOffMulti(nUpStart+1, nUpDownSu);
			m_nUnloadPickCase++; m_tUnloadPickLoop.Set_LoopTime(10000);
		}
		break;
	case 14:	// 정보전달, Picker Up
		if (g_objCommon.Check_Position(AX_UNLOAD_PICKER_Z, nUpWorkTray+1) && g_objCommon.Get_UnloadPickerDownMulti(nUpStart+1, nUpDownSu)) {
			if (!m_tUnloadPickLoop.Waiting_Time(m_pEquipData->nVacOffDelay[2])) break;

			gData.nULPNo = gData.nPNoUnloadPick;
			for (int i=0; i<nUpDownSu; i++) {
				//모듈트랙킹
				int nPNo = gData.nPNoUnloadPick - 1;
				int nTNo = gData.nTNoUnloadPick[nUpStart+i] - 1;
				int nCNo = gData.nCNoUnloadPick[nUpStart+i] - 1;
				if (nCNo > 0) {
					gData.nCmJigNo[nPNo][nTNo][nCNo][UNLOAD_PICK] = nUpStart + i + 1;	// Unload Pick
					g_objLogFile.Save_CmTrackingLog("GOOD", nUpWorkTray, nUpPosX + i + 1, nUpPosY + 1, gData.nPNoUnloadPick, gData.nTNoUnloadPick[nUpStart+i], gData.nCNoUnloadPick[nUpStart+i]);
				}
				g_objLogFile.Save_CapLasLog(gData.sShipLotID, gData.sCIDUnloadPicker[i], gData.nPNoUnloadPick, gData.nTNoUnloadPick[nUpStart+i], gData.nCNoUnloadPick[nUpStart+i], nUpStart+i+1);

				gData.InfoUnloadTray[nUpPosY][nUpPosX+i] = gData.InfoUnloadPick[nUpStart+i]; gData.InfoUnloadPick[nUpStart+i] = 0;
				gData.nTNoUnloadTray = gData.nTNoUnloadPick[nUpStart+i];
				gData.nTNoUnloadPick[nUpStart+i] = gData.nCNoUnloadPick[nUpStart+i] = 0;
				m_nUnloadLotCmCnt++;
			}
			gData.nPNoUnloadTray = gData.nPNoUnloadPick;

			g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 3, gData.nPNoUnloadTray-1);
			g_dlgWork.PostMessage(UM_VISION_RESULT, gData.nPNoUnloadTray, NULL);
			if (Check_UnloadPickerEmpty()) { gData.nPNoUnloadPick = 0; }

			g_objCommon.Set_UnloadPickerAirOffMulti(nUpStart+1, nUpDownSu);
			g_objCommon.Set_UnloadPickerUp(0);
			m_nUnloadPickCase++; m_tUnloadPickLoop.Set_LoopTime(10000);
		}
		break;
	case 15:	// Position Check, Unload Tray Full Check
		if (g_objCommon.Get_UnloadPickerUp(0) && g_objCommon.Get_InfoUnloadPickerVacOff(0)) {
			m_tUnloadPickLoop.Takt_Save(13, 6);
			m_tUnloadPickLoop.Takt_Start();
			if (Check_UnloadPickerEmpty() || Check_UnloadTrayFull()) {
				if (m_pEquipData->bUseInlineMode) {
					if ((gData.nTNoUnloadTray == gData.nLastTrayNo[gData.nPNoUnloadTray-1]) && gData.nLastTrayNo[gData.nPNoUnloadTray-1] > 0 &&
						Check_LoadTrayLoading(gData.nPNoUnloadTray)) {

						Job_LotEnd(gData.nPNoUnloadTray);
						gData.nLastTrayNo[gData.nPNoUnloadTray-1] = 0;
						if (m_pThreadBeep == NULL) {
							m_pThreadBeep = AfxBeginThread(Thread_Beep, (LPVOID)(2000));
						}
						gData.nTNoUnloadTray = 0;
						g_dlgWork.PostMessage(UM_SHOW_MSG, 2, NULL);		// 2020.09.14 khs
						if (m_nUnloadStage1Case == 20) { gData.bUnloadTrayLotEnd[0] = TRUE; m_nUnloadStage1Case = 21; }
						if (m_nUnloadStage2Case == 20) { gData.bUnloadTrayLotEnd[1] = TRUE; m_nUnloadStage2Case = 21; }
					}

					if (gData.bContinueLot && gData.nCmsUseCnt[m_nUnloadLotIndex] == m_nUnloadLotCmCnt) {
						m_nUnloadLotCmCnt = 0;
						m_nUnloadLotIndex++;
						if (m_nUnloadStage1Case == 20) { m_nUnloadStage1Case = 21; }
						if (m_nUnloadStage2Case == 20) { m_nUnloadStage2Case = 21; }
					}
				} else {
					if (Check_LoadTrayLoading(gData.nPNoUnloadTray)) {

						Job_LotEnd(gData.nPNoUnloadTray);
						if (m_pThreadBeep == NULL) {
							m_pThreadBeep = AfxBeginThread(Thread_Beep, (LPVOID)(2000));
						}
						gData.nTNoUnloadTray = 0;
						g_dlgWork.PostMessage(UM_SHOW_MSG, 2, NULL);		// 2020.09.14 khs
						if (m_nUnloadStage1Case == 20) { gData.bUnloadTrayLotEnd[0] = TRUE; m_nUnloadStage1Case = 21; }
						if (m_nUnloadStage2Case == 20) { gData.bUnloadTrayLotEnd[1] = TRUE; m_nUnloadStage2Case = 21; }
					}
					if (Check_UnloadLotEnd() && !m_bUnloadLotEnd) { m_bUnloadLotEnd = TRUE; }
				}

				if (Check_UnloadTrayFull()) {
					if (m_nUnloadStage1Case == 20) m_nUnloadStage1Case = 21;
					if (m_nUnloadStage2Case == 20) m_nUnloadStage2Case = 21;
				}

				if (Check_UnloadPickerEmpty()) {
					g_objCommon.Move_Position(AX_UNLOAD_PICKER_Z, 0);	// Ready Up
					m_nUnloadPickCase++; m_tUnloadPickLoop.Set_LoopTime(10000);
				} else {
					m_nUnloadPickCase = 10; m_tUnloadPickLoop.Set_LoopTime(30000);
				}
			} else {
				m_nUnloadPickCase = 10; m_tUnloadPickLoop.Set_LoopTime(30000);
			}
		}
		break;
	case 16:	// return
		if (g_objCommon.Get_UnloadPickerUp(0) && g_objCommon.Get_InfoUnloadPickerVacOff(0) &&
			g_objCommon.Check_Position(AX_UNLOAD_PICKER_Z, 0))
		{
			m_tUnloadPickLoop.Takt_Save(13, 7);
			m_tUnloadPickLoop.Takt_Start();
			g_objCommon.Move_Position(AX_UNLOAD_PICKER_X, 0);
			g_objCommon.Move_Position(AX_UNLOAD_PICKER_P, 0);
			m_nUnloadPickCase++; m_tUnloadPickLoop.Set_LoopTime(10000);
		}
		break;
	case 17:	// Position Check
		if (g_objCommon.Check_Position(AX_UNLOAD_PICKER_X, 0) && g_objCommon.Check_Position(AX_UNLOAD_PICKER_P, 0))
		{
			m_tUnloadPickLoop.Takt_Save(13, 8);
			m_nUnloadPickCase = 0; m_tUnloadPickLoop.Set_LoopTime(10000);

			m_strLog.Format("UnloadPicker, %d", GetTickCount() - m_dwUnloadPick);
			g_objLogFile.Save_TestLog(m_strLog);
		}
		break;

	}

	// 14. (Error : 4400)
	if (m_tUnloadPickLoop.Over_LoopTime()) {
		g_objCommon.Show_Error(4400 + m_nUnloadPickCase);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 15. (Error : 4500)
BOOL CSequenceMain::UnloadStage1_Run()
{
	if (gData.bUnloadPort1Wait && m_nUnloadStage1Case >  1 && m_nUnloadStage1Case < 10) return TRUE;
	if (gData.bUnloadPort2Wait && m_nUnloadStage1Case > 22 && m_nUnloadStage1Case < 50) return TRUE;

	switch (m_nUnloadStage1Case) {
	case 0:		// Start시 1로 바꿈
		return TRUE;

	case 1:		// 안전확인
		if (g_objCommon.Check_Position(AX_UNLOAD_STAGE1_Y, 0) && g_objCommon.Check_Position(AX_UNLOAD_STAGE1_Z, 1) &&
			g_objCommon.Get_UnloadTrayMasterSlaveOut(1) && !m_pDX05->iUnloadStage1Exist &&
			m_pDX03->iUnloadPort1SlideClose && m_pDX03->iUnlaodPort1LowCheck)
		{
			m_dwUnloadStage1 = GetTickCount();
			m_nUnloadStage1Case++; m_tUnloadStage1Loop.Set_LoopTime(5000);

		} else {
			if (!m_pDX03->iUnlaodPort1LowCheck) { g_dlgWork.PostMessage(UM_SHOW_MSG, 5, NULL); return FALSE;}
		}
		break;
	case 2:		// Ship Tray Check, Z Move to Support Up
		if (g_objCommon.Check_Position(AX_UNLOAD_STAGE1_Y, 0) && g_objCommon.Check_Position(AX_UNLOAD_STAGE1_Z, 1) &&
			g_objCommon.Get_UnloadTrayMasterSlaveOut(1) && !m_pDX05->iUnloadStage1Exist &&
			m_pDX03->iUnloadPort1SlideClose && m_pDX03->iUnlaodPort1LowCheck)
		{
			m_tUnloadStage1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_UNLOAD_STAGE1_Z, 2);				
			m_nUnloadStage1Case++; m_tUnloadStage1Loop.Set_LoopTime(5000);

		} else {
			if (!m_pDX03->iUnlaodPort1LowCheck) { g_dlgWork.PostMessage(UM_SHOW_MSG, 5, NULL); return FALSE;}
		}
		break;
	case 3:		// Support Out
		if (g_objCommon.Check_Position(AX_UNLOAD_STAGE1_Z, 2)) {
			m_tUnloadStage1Loop.Takt_Save(14, 1);
			m_tUnloadStage1Loop.Takt_Start();
			m_pDY05->oUnloadStage1MasterIn = TRUE;
			g_objAJinAXL.Write_Output(5);
			m_nUnloadStage1Case++; m_tUnloadStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 4:		// stage Slave In
		if (m_pDX05->iUnloadStage1MasterIn && !m_pDX05->iUnloadStage1MasterOut) {
			m_pDY05->oUnloadStage1SlaveIn = TRUE;
			g_objAJinAXL.Write_Output(5);
			m_nUnloadStage1Case++; m_tUnloadStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 5:		// Master/Slave Check
		if (g_objCommon.Get_UnloadTrayMasterSlaveIn(1)) {
			m_tUnloadStage1Loop.Takt_Save(14, 6);	// Master/slave In
			m_tUnloadStage1Loop.Takt_Start();
			g_objCommon.Set_UnloadPortSupportOut(1);
			m_nUnloadStage1Case++; m_tUnloadStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 6:		// Z Move to Support Down
		if (g_objCommon.Get_UnloadPortSupportOut(1)) {
			m_tUnloadStage1Loop.Takt_Save(14, 2);	// Support Out
			m_tUnloadStage1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_UNLOAD_STAGE1_Z, 3);				
			m_nUnloadStage1Case++; m_tUnloadStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 7:		// Support In
		if (g_objCommon.Check_Position(AX_UNLOAD_STAGE1_Z, 3)) {
			if (!m_tUnloadStage1Loop.Waiting_Time(500)) break;
			m_tUnloadStage1Loop.Takt_Save(14, 3);	// Support Down
			m_tUnloadStage1Loop.Takt_Start();
			g_objCommon.Set_UnloadPortSupportIn(1);
			m_nUnloadStage1Case++; m_tUnloadStage1Loop.Set_LoopTime(5000);
		}
		break;
	
	case 8:		// Tray Check, Stage Master In
		if (g_objCommon.Get_UnloadPortSupportIn(1)) {
			if (!m_tUnloadStage1Loop.Waiting_Time(500)) break;
			m_tUnloadStage1Loop.Takt_Save(14, 4);	// Support In
			m_tUnloadStage1Loop.Takt_Start();
			// Port와 Tray가 아슬아슬하여 부딪힐때가 있어 MoveDown에서 이동하도록 한다.
			g_objCommon.Move_Position(AX_UNLOAD_STAGE1_Z, 0);	// Move Down
			m_nUnloadStage1Case++; m_tUnloadStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 9:		// Z Move to Move Up
		if (g_objCommon.Check_Position(AX_UNLOAD_STAGE1_Z, 0) && m_pDX05->iUnloadStage1Exist) {
			m_tUnloadStage1Loop.Takt_Save(14, 14);
			m_nUnloadStage1Case++; m_tUnloadStage1Loop.Set_LoopTime(5000);
		}
		break;

	case 10:	// 안전 확인 
		if (m_nUnloadStage2Case > 22) {
			m_nUnloadStage1Case++; m_tUnloadStage1Loop.Set_LoopTime(5000);
		}
		return TRUE;

	case 11:	// Y Move to Work Position
		if (g_objCommon.Check_Position(AX_UNLOAD_STAGE1_Z, 0) && m_pDX05->iUnloadStage1Exist) {
			m_tUnloadStage1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_UNLOAD_STAGE1_Y, 1);	// Work Position
			m_nUnloadStage1Case++; m_tUnloadStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 12:	// Position check
		if (g_objCommon.Check_Position(AX_UNLOAD_STAGE1_Y, 1)) {
			m_tUnloadStage1Loop.Takt_Save(14, 7);
			m_tUnloadStage1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_UNLOAD_STAGE1_Z, 1);	// Move Up
			m_nUnloadStage1Case++; m_tUnloadStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 13:
		if (g_objCommon.Check_Position(AX_UNLOAD_STAGE1_Z, 1) && m_pDX05->iUnloadStage1Exist) {
			m_tUnloadStage1Loop.Takt_Save(14, 5);
			Init_UnloadTray();
			m_nUnloadStage1Case++; m_tUnloadStage1Loop.Set_LoopTime(5000);
		}
		break;

	case 14:	// Ship 정보 확인 및 MES 자재 등록.
		if (gData.nShipTrayCount >= gData.nShipTrayUseCount) {
			m_nUnloadStage1Case++; m_tUnloadStage1Loop.Set_LoopTime(5000);
		} else {
			gData.nShipTrayCount++; g_objCommon.Save_CapShiipData(2);
			m_nUnloadStage1Case = 20; m_tUnloadStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 15:	// Ship 정보 확인 및 MES 자재 등록.
		if (g_dlgWork.Check_ShipLotID()) {
			if (m_pEquipData->bUseMes && m_pEquipData->bUseMesShipReg) {	// MES Ship 자재등록 사용
				g_objMES.m_nMESShipSequence = 0;
				g_dlgWork.Get_ShipLotID();
				g_objMES.Set_ShipLotChangeRequest(gData.sShipLotID, gData.sOperID);	// 자재 등록 요청
			}
			m_nUnloadStage1Case++; m_tUnloadStage1Loop.Set_LoopTime(30000);
		} else {
//			g_objCommon.Show_Alarm("Ship Tray 자재 정보를 입력해 주십시오.");
			g_dlgWork.PostMessage(UM_SHOW_MSG, 6, NULL);
			return FALSE;
		}
		break;
	case 16:	// MES 결과 확인.
		if (g_objMES.m_nMESShipSequence == 1 || !m_pEquipData->bUseMes || !m_pEquipData->bUseMesShipReg) {
			g_dlgWork.Set_ShipLotIDChange();	// 정보 이동.
			gData.nShipTrayCount = 0; g_objCommon.Save_CapShiipData(2);
			m_nUnloadStage1Case = 20; m_tUnloadStage1Loop.Set_LoopTime(5000);
		}
		break;

	case 20:	// Working
		return TRUE;

	case 21:	// 안전 확인
		if (m_nUnloadStage2Case <= 10 || m_nUnloadStage2Case > 52) {
			if (g_objAJinAXL.Is_Done(AX_UNLOAD_STAGE1_Y)) {
				m_tUnloadStage1Loop.Takt_Start();
				// Port와 Tray가 아슬아슬하여 부딪힐때가 있어 MoveDown에서 이동하도록 한다.
				g_objCommon.Move_Position(AX_UNLOAD_STAGE1_Z, 0);	// Move Down
				m_nUnloadStage1Case++; m_tUnloadStage1Loop.Set_LoopTime(5000);
			}
		}
		return TRUE;
	case 22:	// Y Move to Unload Port Position
		if (g_objCommon.Check_Position(AX_UNLOAD_STAGE1_Z, 0)) {
			m_tUnloadStage1Loop.Takt_Save(14, 14);
			m_tUnloadStage1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_UNLOAD_STAGE1_Y, 2);
			m_nUnloadStage1Case++; m_tUnloadStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 23:	// Z Move to Support Down
		if (g_objCommon.Check_Position(AX_UNLOAD_STAGE1_Y, 2) && m_pDX05->iUnloadStage1Exist && m_pDX03->iUnloadPort2SlideClose) {
			if ((gData.nPNoUnloadTray != gData.nPNoUnloadPort) && gData.nPNoUnloadPort != 0) return TRUE;

			m_tUnloadStage1Loop.Takt_Save(14, 8);
			m_tUnloadStage1Loop.Takt_Start();
			gData.nPNoUnloadPort = gData.nPNoUnloadTray;
			g_objCommon.Move_Position(AX_UNLOAD_STAGE1_Z, 5);	// Support Down
			m_nUnloadStage1Case++; m_tUnloadStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 24:	// Support Out
		if (g_objCommon.Check_Position(AX_UNLOAD_STAGE1_Z, 5)) {
			m_tUnloadStage1Loop.Takt_Save(14, 9);
			m_tUnloadStage1Loop.Takt_Start();
			g_objCommon.Set_UnloadPortSupportOut(2);
			m_nUnloadStage1Case++; m_tUnloadStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 25:	// Z Move to Support Up
		if (g_objCommon.Get_UnloadPortSupportOut(2)) {
			m_tUnloadStage1Loop.Takt_Save(14, 10);
			m_tUnloadStage1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_UNLOAD_STAGE1_Z, 4);	// Support Up				
			m_nUnloadStage1Case++; m_tUnloadStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 26:	// Support In
		if (g_objCommon.Check_Position(AX_UNLOAD_STAGE1_Z, 4)) {
			m_tUnloadStage1Loop.Takt_Save(14, 11);
			m_tUnloadStage1Loop.Takt_Start();
			g_objCommon.Set_UnloadPortSupportIn(2);
			m_nUnloadStage1Case++; m_tUnloadStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 27:	// Slave Out
		if (g_objCommon.Get_UnloadPortSupportIn(2) && g_objCommon.Check_Position(AX_UNLOAD_STAGE1_Z, 4)) {
			m_tUnloadStage1Loop.Takt_Save(14, 12);
			m_tUnloadStage1Loop.Takt_Start();
			m_pDY05->oUnloadStage1SlaveIn = FALSE;
			g_objAJinAXL.Write_Output(5);
			m_nUnloadStage1Case++; m_tUnloadStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 28:	// Master Out
		if (!m_pDX05->iUnloadStage1SlaveIn && m_pDX05->iUnloadStage1SlaveOut) {
			m_pDY05->oUnloadStage1MasterIn = FALSE;
			g_objAJinAXL.Write_Output(5);
			m_nUnloadStage1Case++; m_tUnloadStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 29:	// Stage2 Check, Z Move to Move Down
		if (g_objCommon.Get_UnloadTrayMasterSlaveOut(1)) {
			m_tUnloadStage1Loop.Takt_Save(14, 13);
			m_tUnloadStage1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_UNLOAD_STAGE1_Z, 0);	// Move Down				
			m_nUnloadStage1Case++; m_tUnloadStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 30:	// Position Check
		if (g_objCommon.Check_Position(AX_UNLOAD_STAGE1_Z, 0) && !m_pDX05->iUnloadStage1Exist) {
			m_tUnloadStage1Loop.Takt_Save(14, 14);
			if (gData.bUnloadTrayLotEnd[0] && m_bUnloadLotEnd) {	// 도어락 오픈 후처리 확인.
				m_pDY03->oUnloadPort2SlideLock = FALSE; m_pDY03->oUnloadPort2SlideUnlock = TRUE;
				//m_pDY13->oDoor05Unlock = TRUE;
				//m_pDY13->oDoor06Unlock = TRUE;
				g_objAJinAXL.Write_Output(3);
				g_objAJinAXL.Write_Output(13);
				gData.bUnloadPort2Wait = TRUE;
				gData.nPNoUnloadTray = 0;
			} else if (gData.bUnloadTrayLotEnd[0] && !m_bUnloadLotEnd) {	// 연속랏
				m_pDY03->oUnloadPort2SlideLock = FALSE; m_pDY03->oUnloadPort2SlideUnlock = TRUE;
				//m_pDY13->oDoor05Unlock = TRUE;
				//m_pDY13->oDoor06Unlock = TRUE;
				g_objAJinAXL.Write_Output(3);
				g_objAJinAXL.Write_Output(13);
				gData.bUnloadPort2Wait = TRUE;
				gData.nPNoUnloadTray = 0;
			}
			m_nUnloadStage1Case = 50; m_tUnloadStage1Loop.Set_LoopTime(5000);

			m_strLog.Format("UnloadStage1, %d", GetTickCount() - m_dwUnloadStage1);
			g_objLogFile.Save_TestLog(m_strLog);
		}
		break;

	case 50:	// Stage2 Check
		if (!m_bUnloadLotEnd && m_nUnloadStage2Case > 13 && g_objCommon.Check_Position(AX_UNLOAD_STAGE2_Z, 1)) {
			m_nUnloadStage1Case++; m_tUnloadStage1Loop.Set_LoopTime(5000);

		} else if (m_bUnloadLotEnd && m_nUnloadStage2Case > 20 && g_objCommon.Check_Position(AX_UNLOAD_STAGE2_Z, 1)) {
			// Lot 종료 시점이더라도 잔량있는 트레이 배출할 수 있게 해준다.
			m_nUnloadStage1Case++; m_tUnloadStage1Loop.Set_LoopTime(5000);
		}
		return TRUE;
	case 51:	// 안전 확인, Y Move to Load Port Position
		if (g_objCommon.Check_Position(AX_UNLOAD_STAGE1_Z, 0) && !m_pDX05->iUnloadStage1Exist) {
			if (g_objCommon.Check_Position(AX_UNLOAD_STAGE2_Z, 0)) break;	// 인터락
			m_tUnloadStage1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_UNLOAD_STAGE1_Y, 0);
			m_nUnloadStage1Case++; m_tUnloadStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 52:	// stage2 Check
		if (g_objCommon.Check_Position(AX_UNLOAD_STAGE1_Y, 0)) {
			if (m_nUnloadStage2Case >= 20) {
				m_tUnloadStage1Loop.Takt_Save(14, 15);
				m_nUnloadStage1Case++; m_tUnloadStage1Loop.Set_LoopTime(5000);
			}
			return TRUE;
		}
		break;
		
	case 53:	// Z Move to Move Up
		if (g_objCommon.Check_Position(AX_UNLOAD_STAGE1_Y, 0)) {
			m_tUnloadStage1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_UNLOAD_STAGE1_Z, 1);
			m_nUnloadStage1Case++; m_tUnloadStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 54:	// Position Check
		if (g_objCommon.Check_Position(AX_UNLOAD_STAGE1_Z, 1)) {
			m_tUnloadStage1Loop.Takt_Save(14, 16);
			m_nUnloadStage1Case = 1; m_tUnloadStage1Loop.Set_LoopTime(5000);
		}
		break;
	}

	// 15. (Error : 4500)
	if (m_tUnloadStage1Loop.Over_LoopTime()) {
		g_objCommon.Show_Error(4500 + m_nUnloadStage1Case);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 16. (Error : 4600)
BOOL CSequenceMain::UnloadStage2_Run()
{
	if (gData.bUnloadPort1Wait && m_nUnloadStage2Case >  1 && m_nUnloadStage2Case < 10) return TRUE;
	if (gData.bUnloadPort2Wait && m_nUnloadStage2Case > 22 && m_nUnloadStage2Case < 50) return TRUE;

	switch (m_nUnloadStage2Case) {
	case 0:		// Start시 1로 바꿈
		return TRUE;

	case 1:		// 안전확인
		if (g_objCommon.Check_Position(AX_UNLOAD_STAGE2_Y, 0) && g_objCommon.Check_Position(AX_UNLOAD_STAGE2_Z, 1) &&
			g_objCommon.Get_UnloadTrayMasterSlaveOut(2) && !m_pDX05->iUnloadStage2Exist &&
			m_pDX03->iUnloadPort1SlideClose && m_pDX03->iUnlaodPort1LowCheck)
		{
			m_dwUnloadStage2 = GetTickCount();
			m_nUnloadStage2Case++; m_tUnloadStage2Loop.Set_LoopTime(5000);

		} else {
			if (!m_pDX03->iUnlaodPort1LowCheck) { g_dlgWork.PostMessage(UM_SHOW_MSG, 5, NULL); return FALSE;}
		}
		break;
	case 2:		// Ship Tray Check, Z Move to Support Up
		if (g_objCommon.Check_Position(AX_UNLOAD_STAGE2_Y, 0) && g_objCommon.Check_Position(AX_UNLOAD_STAGE2_Z, 1) &&
			g_objCommon.Get_UnloadTrayMasterSlaveOut(2) && !m_pDX05->iUnloadStage2Exist &&
			m_pDX03->iUnloadPort1SlideClose && m_pDX03->iUnlaodPort1LowCheck)
		{
			m_tUnloadStage2Loop.Takt_Start();
			g_objCommon.Move_Position(AX_UNLOAD_STAGE2_Z, 2);				
			m_nUnloadStage2Case++; m_tUnloadStage2Loop.Set_LoopTime(5000);

		} else {
			if (!m_pDX03->iUnlaodPort1LowCheck) { g_dlgWork.PostMessage(UM_SHOW_MSG, 5, NULL); return FALSE;}
		}
		break;
	case 3:		// 
		if (g_objCommon.Check_Position(AX_UNLOAD_STAGE2_Z, 2)) {
			m_tUnloadStage2Loop.Takt_Save(15, 1);
			m_tUnloadStage2Loop.Takt_Start();
			m_pDY05->oUnloadStage2MasterIn = TRUE;
			g_objAJinAXL.Write_Output(5);
			m_nUnloadStage2Case++; m_tUnloadStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 4:		// stage Slave In
		if (m_pDX05->iUnloadStage2MasterIn && !m_pDX05->iUnloadStage2MasterOut) {
			m_pDY05->oUnloadStage2SlaveIn = TRUE;
			g_objAJinAXL.Write_Output(5);
			m_nUnloadStage2Case++; m_tUnloadStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 5:		// Master/Slave Check
		if (g_objCommon.Get_UnloadTrayMasterSlaveIn(2)) {
			m_tUnloadStage2Loop.Takt_Save(15, 6);	// Master/Slave In
			m_tUnloadStage2Loop.Takt_Start();
			g_objCommon.Set_UnloadPortSupportOut(1);
			m_nUnloadStage2Case++; m_tUnloadStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 6:		// Z Move to Support Down
		if (g_objCommon.Get_UnloadPortSupportOut(1)) {
			m_tUnloadStage2Loop.Takt_Save(15, 2);
			m_tUnloadStage2Loop.Takt_Start();
			g_objCommon.Move_Position(AX_UNLOAD_STAGE2_Z, 3);				
			m_nUnloadStage2Case++; m_tUnloadStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 7:		// Support In
		if (g_objCommon.Check_Position(AX_UNLOAD_STAGE2_Z, 3)) {
			if (!m_tUnloadStage2Loop.Waiting_Time(500)) break;
			m_tUnloadStage2Loop.Takt_Save(15, 3);
			m_tUnloadStage2Loop.Takt_Start();
			g_objCommon.Set_UnloadPortSupportIn(1);
			m_nUnloadStage2Case++; m_tUnloadStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 8:		// Tray Check, Stage Master In
		if (g_objCommon.Get_UnloadPortSupportIn(1)) {
			if (!m_tUnloadStage2Loop.Waiting_Time(500)) break;
			m_tUnloadStage2Loop.Takt_Save(15, 4);
			m_tUnloadStage2Loop.Takt_Start();
			// Port와 Tray가 아슬아슬하여 부딪힐때가 있어 MoveDown에서 이동하도록 한다.
			g_objCommon.Move_Position(AX_UNLOAD_STAGE2_Z, 0);	// Move Down
			m_nUnloadStage2Case++; m_tUnloadStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 9:		// Z Move to Move Up
		if (g_objCommon.Check_Position(AX_UNLOAD_STAGE2_Z, 0) && m_pDX05->iUnloadStage2Exist) {
			m_tUnloadStage2Loop.Takt_Save(15, 14);							
			m_nUnloadStage2Case++; m_tUnloadStage2Loop.Set_LoopTime(5000);
		}
		break;

	case 10:	// 안전 확인 
		if (m_nUnloadStage1Case > 22) {
			m_nUnloadStage2Case++; m_tUnloadStage2Loop.Set_LoopTime(5000);
		}
		return TRUE;

	case 11:	// Y Move to Work Position
		if (g_objCommon.Check_Position(AX_UNLOAD_STAGE2_Z, 0) && m_pDX05->iUnloadStage2Exist) {
			m_tUnloadStage2Loop.Takt_Start();
			g_objCommon.Move_Position(AX_UNLOAD_STAGE2_Y, 1);	// Work Position
			m_nUnloadStage2Case++; m_tUnloadStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 12:	// Position check
		if (g_objCommon.Check_Position(AX_UNLOAD_STAGE2_Y, 1)) {
			m_tUnloadStage2Loop.Takt_Save(15, 7);
			m_tUnloadStage2Loop.Takt_Start();
			g_objCommon.Move_Position(AX_UNLOAD_STAGE2_Z, 1);	// Move Up
			m_nUnloadStage2Case++; m_tUnloadStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 13:
		if (g_objCommon.Check_Position(AX_UNLOAD_STAGE2_Z, 1) && m_pDX05->iUnloadStage2Exist) {
			m_tUnloadStage2Loop.Takt_Save(15, 5);
			m_tUnloadStage2Loop.Takt_Start();
			Init_UnloadTray();
			m_nUnloadStage2Case++; m_tUnloadStage2Loop.Set_LoopTime(5000);
		}
		break;

	case 14:	// Ship 정보 확인 및 MES 자재 등록.
		if (gData.nShipTrayCount >= gData.nShipTrayUseCount) {
			m_nUnloadStage2Case++; m_tUnloadStage2Loop.Set_LoopTime(5000);
		} else {
			gData.nShipTrayCount++; g_objCommon.Save_CapShiipData(2);
			m_nUnloadStage2Case = 20; m_tUnloadStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 15:	// Ship 정보 확인 및 MES 자재 등록.
		if (g_dlgWork.Check_ShipLotID()) {
			if (m_pEquipData->bUseMes && m_pEquipData->bUseMesShipReg) {	// MES Ship 자재등록 사용
				g_objMES.m_nMESShipSequence = 0;
				g_dlgWork.Get_ShipLotID();
				g_objMES.Set_ShipLotChangeRequest(gData.sShipLotID, gData.sOperID);	// 자재 등록 요청
			}
			m_nUnloadStage2Case++; m_tUnloadStage2Loop.Set_LoopTime(30000);
		} else {
//			g_objCommon.Show_Alarm("Ship Tray 자재 정보를 입력해 주십시오.");
			g_dlgWork.PostMessage(UM_SHOW_MSG, 6, NULL);
			return FALSE;
		}
		break;
	case 16:	// MES 결과 확인.
		if (g_objMES.m_nMESShipSequence == 1 || !m_pEquipData->bUseMes || !m_pEquipData->bUseMesShipReg) {
			g_dlgWork.Set_ShipLotIDChange();	// 정보 이동.
			gData.nShipTrayCount = 0; g_objCommon.Save_CapShiipData(2);
			m_nUnloadStage2Case = 20; m_tUnloadStage2Loop.Set_LoopTime(5000);
		}
		break;

	case 20:	// Working
		return TRUE;

	case 21:	// 안전 확인
		if (m_nUnloadStage1Case <= 10 || m_nUnloadStage1Case > 52) {
			if (g_objAJinAXL.Is_Done(AX_UNLOAD_STAGE2_Y)) {
				m_tUnloadStage2Loop.Takt_Start();
				// Port와 Tray가 아슬아슬하여 부딪힐때가 있어 MoveDown에서 이동하도록 한다.
				g_objCommon.Move_Position(AX_UNLOAD_STAGE2_Z, 0);	// Move Down
				m_nUnloadStage2Case++; m_tUnloadStage2Loop.Set_LoopTime(5000);
			}
		}
		return TRUE;
	case 22:	// Y Move to Unload Port Position
		if (g_objCommon.Check_Position(AX_UNLOAD_STAGE2_Z, 0)) {
			m_tUnloadStage2Loop.Takt_Save(15, 14);
			m_tUnloadStage2Loop.Takt_Start();
			g_objCommon.Move_Position(AX_UNLOAD_STAGE2_Y, 2);
			m_nUnloadStage2Case++; m_tUnloadStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 23:	// Z Move to Support Down
		if (g_objCommon.Check_Position(AX_UNLOAD_STAGE2_Y, 2) && m_pDX05->iUnloadStage2Exist && m_pDX03->iUnloadPort2SlideClose) {
			if ((gData.nPNoUnloadTray != gData.nPNoUnloadPort) && gData.nPNoUnloadPort != 0) return TRUE;

			m_tUnloadStage2Loop.Takt_Save(15, 8);
			m_tUnloadStage2Loop.Takt_Start();
			gData.nPNoUnloadPort = gData.nPNoUnloadTray;
			g_objCommon.Move_Position(AX_UNLOAD_STAGE2_Z, 5);	// Support Down
			m_nUnloadStage2Case++; m_tUnloadStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 24:	// Support Out
		if (g_objCommon.Check_Position(AX_UNLOAD_STAGE2_Z, 5)) {
			m_tUnloadStage2Loop.Takt_Save(15, 9);
			m_tUnloadStage2Loop.Takt_Start();
			g_objCommon.Set_UnloadPortSupportOut(2);
			m_nUnloadStage2Case++; m_tUnloadStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 25:	// Z Move to Support Up
		if (g_objCommon.Get_UnloadPortSupportOut(2)) {
			m_tUnloadStage2Loop.Takt_Save(15, 10);
			m_tUnloadStage2Loop.Takt_Start();
			g_objCommon.Move_Position(AX_UNLOAD_STAGE2_Z, 4);	// Support Up				
			m_nUnloadStage2Case++; m_tUnloadStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 26:	// Support In
		if (g_objCommon.Check_Position(AX_UNLOAD_STAGE2_Z, 4)) {
			m_tUnloadStage2Loop.Takt_Save(15, 11);
			m_tUnloadStage2Loop.Takt_Start();
			g_objCommon.Set_UnloadPortSupportIn(2);
			m_nUnloadStage2Case++; m_tUnloadStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 27:	// Slave Out
		if (g_objCommon.Get_UnloadPortSupportIn(2) && g_objCommon.Check_Position(AX_UNLOAD_STAGE2_Z, 4)) {
			m_tUnloadStage2Loop.Takt_Save(15, 12);
			m_tUnloadStage2Loop.Takt_Start();

			m_pDY05->oUnloadStage2SlaveIn = FALSE;
			g_objAJinAXL.Write_Output(5);
			m_nUnloadStage2Case++; m_tUnloadStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 28:	// Master Out
		if (!m_pDX05->iUnloadStage2SlaveIn && m_pDX05->iUnloadStage2SlaveOut) {
			m_pDY05->oUnloadStage2MasterIn = FALSE;
			g_objAJinAXL.Write_Output(5);
			m_nUnloadStage2Case++; m_tUnloadStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 29:	// Stage2 Check, Z Move to Move Down
		if (g_objCommon.Get_UnloadTrayMasterSlaveOut(2)) {
			m_tUnloadStage2Loop.Takt_Save(15, 13);
			m_tUnloadStage2Loop.Takt_Start();
			g_objCommon.Move_Position(AX_UNLOAD_STAGE2_Z, 0);	// Move Down				
			m_nUnloadStage2Case++; m_tUnloadStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 30:	// Position Check
		if (g_objCommon.Check_Position(AX_UNLOAD_STAGE2_Z, 0) && !m_pDX05->iUnloadStage2Exist) {
			m_tUnloadStage2Loop.Takt_Save(15, 14);
			if (gData.bUnloadTrayLotEnd[1] && m_bUnloadLotEnd) {	// 도어락 오픈 후처리 확인.
				m_pDY03->oUnloadPort2SlideLock = FALSE; m_pDY03->oUnloadPort2SlideUnlock = TRUE;
				//m_pDY13->oDoor05Unlock = TRUE;
				//m_pDY13->oDoor06Unlock = TRUE;
				g_objAJinAXL.Write_Output(3);
				g_objAJinAXL.Write_Output(13);
				gData.bUnloadPort2Wait = TRUE;
				gData.nPNoUnloadTray = 0;
			} else if (gData.bUnloadTrayLotEnd[1] && !m_bUnloadLotEnd) {	// 연속랏
				m_pDY03->oUnloadPort2SlideLock = FALSE; m_pDY03->oUnloadPort2SlideUnlock = TRUE;
				//m_pDY13->oDoor05Unlock = TRUE;
				//m_pDY13->oDoor06Unlock = TRUE;
				g_objAJinAXL.Write_Output(3);
				g_objAJinAXL.Write_Output(13);
				gData.bUnloadPort2Wait = TRUE;
				gData.nPNoUnloadTray = 0;
			}
			m_nUnloadStage2Case = 50; m_tUnloadStage2Loop.Set_LoopTime(5000);

			m_strLog.Format("UnloadStage2, %d", GetTickCount() - m_dwUnloadStage2);
			g_objLogFile.Save_TestLog(m_strLog);
		}
		break;

	case 50:	// Stage2 Check
		if (!m_bUnloadLotEnd && m_nUnloadStage1Case > 13 && g_objCommon.Check_Position(AX_UNLOAD_STAGE1_Z, 1)) {
			m_nUnloadStage2Case++; m_tUnloadStage2Loop.Set_LoopTime(5000);

		} else if (m_bUnloadLotEnd && m_nUnloadStage1Case > 20 && g_objCommon.Check_Position(AX_UNLOAD_STAGE1_Z, 1)) {
			// Lot 종료 시점이더라도 잔량있는 트레이 배출할 수 있게 해준다.
			m_nUnloadStage2Case++; m_tUnloadStage2Loop.Set_LoopTime(5000);
		}
		return TRUE;
	case 51:	// 안전 확인, Y Move to Load Port Position
		if (g_objCommon.Check_Position(AX_UNLOAD_STAGE2_Z, 0) && !m_pDX05->iUnloadStage2Exist) {
			if (g_objCommon.Check_Position(AX_UNLOAD_STAGE1_Z, 0)) break;	// 인터락
			m_tUnloadStage2Loop.Takt_Start();
			g_objCommon.Move_Position(AX_UNLOAD_STAGE2_Y, 0);
			m_nUnloadStage2Case++; m_tUnloadStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 52:	// stage2 Check
		if (g_objCommon.Check_Position(AX_UNLOAD_STAGE2_Y, 0)) {
			if (m_nUnloadStage1Case >= 20) {
				m_tUnloadStage2Loop.Takt_Save(15, 15);
				m_nUnloadStage2Case++; m_tUnloadStage2Loop.Set_LoopTime(5000);
			}
			return TRUE;
		}
		break;

	case 53:	// Z Move to Move Up
		if (g_objCommon.Check_Position(AX_UNLOAD_STAGE2_Y, 0)) {
			m_tUnloadStage2Loop.Takt_Start();
			g_objCommon.Move_Position(AX_UNLOAD_STAGE2_Z, 1);
			m_nUnloadStage2Case++; m_tUnloadStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 54:	// Position Check
		if (g_objCommon.Check_Position(AX_UNLOAD_STAGE2_Z, 1)) {
			m_tUnloadStage2Loop.Takt_Save(15, 16);
			m_nUnloadStage2Case = 1; m_tUnloadStage2Loop.Set_LoopTime(5000);
		}
		break;
	}

	// 16. (Error : 4600)
	if (m_tUnloadStage2Loop.Over_LoopTime()) {
		g_objCommon.Show_Error(4600 + m_nUnloadStage2Case);
		return FALSE;
	}
	return TRUE;
}



