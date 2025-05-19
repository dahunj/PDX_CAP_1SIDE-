// LoopTimer.cpp : 구현 파일
//
#include "StdAfx.h"
#include "LoopTimer.h"

#include "LogFile.h"
#include "DataManager.h"

CLoopTimer::CLoopTimer()
{
	m_dwLoopStart = 0;
	m_dwLoopTerm = 0;

	m_bWaitingRun = FALSE;
	m_dwWaitingStart = 0;
}

CLoopTimer::~CLoopTimer()
{
}

void CLoopTimer::Set_LoopTime(DWORD msec)
{
	m_dwLoopStart = GetTickCount();
	m_dwLoopTerm = msec;
}

BOOL CLoopTimer::Over_LoopTime()
{
	DWORD dwTerm = GetTickCount() - m_dwLoopStart;
	if (dwTerm >= m_dwLoopTerm)
		return TRUE;
	else
		return FALSE;
}

BOOL CLoopTimer::Delay_LoopTime(DWORD msec)
{
	DWORD dwTerm = GetTickCount() - m_dwLoopStart;
	if (dwTerm >= msec) return TRUE;
	else return FALSE;
}

BOOL CLoopTimer::Waiting_Time(DWORD msec)
{
	if (!m_bWaitingRun) {
		m_bWaitingRun = TRUE;
		m_dwWaitingStart = GetTickCount();
	}

	DWORD dwTerm = GetTickCount() - m_dwWaitingStart;
	if (dwTerm < msec) return FALSE;
	
	m_bWaitingRun = FALSE;
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////

void CLoopTimer::Takt_Start()
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	if (!pEquipData->bJobListTaktLog) return;

	m_dwTakt = GetTickCount();
}

void CLoopTimer::Takt_Save(int nFun, int nId)
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	if (!pEquipData->bJobListTaktLog) return;

	CString strFun, strLog, strMsg;
	switch (nFun) {
	case 1:		// Tray Picker
		strFun = "TrayPicker";
		switch (nId) {
		case  1: strMsg = "Z Axis Move to Avi Down Position"; break;
		case  2: strMsg = "Tray Picker Grip Close"; break;
		case  3: strMsg = "Z Axis Move to Ready Up Position"; break;
		case  4: strMsg = "X Axis Move to Load1 Position(R Axis Turn)"; break;
		case  5: strMsg = "Z Axis Move to Load1 Down Position"; break;
		case  6: strMsg = "Tray Picker Grip Open"; break;
		case  7: strMsg = "Z Axis Move to Ready Up Position"; break;
		case  8: strMsg = "X Axis Move to Avi Position(R Axis Ready)"; break;
		}
		break;
	case 2:		// Load Stage1
		strFun = "LoadStage1";
		switch (nId) {
		case  1: strMsg = "Z Axis Move to Support Up Position"; break;
		case  2: strMsg = "Port Support Out"; break;
		case  3: strMsg = "Z Axis Move to Support Down Position"; break;
		case  4: strMsg = "Port Support In"; break;
		case  5: strMsg = "Z Axis Move to Moving Up Position"; break;
		case  6: strMsg = "Master/Slave In"; break;
		case  7: strMsg = "X Axis Move to Work Position"; break;
		case  8: strMsg = "X Axis Move to Unload Position"; break;
		case  9: strMsg = "Z Axis Move to Unload Up Position"; break;
		case 10: strMsg = "Master/Slave Out"; break;
		case 11: strMsg = "Z Axis Move to Moving Down Position"; break;
		case 12: strMsg = "X Axis Move to Load Position"; break;
		}
		break;
	case 3:		// Load Stage2
		strFun = "LoadStage2";
		switch (nId) {
		case  1: strMsg = "Z Axis Move to Support Up Position"; break;
		case  2: strMsg = "Port Support Out"; break;
		case  3: strMsg = "Z Axis Move to Support Down Position"; break;
		case  4: strMsg = "Port Support In"; break;
		case  5: strMsg = "Z Axis Move to Moving Up Position"; break;
		case  6: strMsg = "Master/Slave In"; break;
		case  7: strMsg = "X Axis Move to Work Position"; break;
		case  8: strMsg = "X Axis Move to Unload Position"; break;
		case  9: strMsg = "Z Axis Move to Unload Up Position"; break;
		case 10: strMsg = "Master/Slave Out"; break;
		case 11: strMsg = "Z Axis Move to Moving Down Position"; break;
		case 12: strMsg = "X Axis Move to Load Position"; break;
		}
		break;
	case 4:		// Load Picker
		strFun = "LoadPicker";
		switch (nId) {
		case 1: strMsg = "Y Axis Move to Pickup position"; break; 
		case 2: strMsg = "Z Axis Move to Tray Down. Cylinder Down"; break;
		case 3: strMsg = "Load Picker Grip Close"; break;
		case 4: strMsg = "Z Axis Move to Tray Up. Cylinder Up"; break;
		case 5: strMsg = "Y/P Axis Move to Index Position"; break;
		case 6: strMsg = "Z Axis Move to Index Down. Cylinder Down"; break;
		case 7: strMsg = "Load Picker Grip Open"; break;
		case 8: strMsg = "Z Axis Move to Index Up. Cylinder Up"; break;
		case 9: strMsg = "Y/P Axis Move to Tray position"; break;
		}
		break;
	case 5:		// Vision Cm Align
		strFun = "CmAlign";
		switch (nId) {
		case  1: strMsg = "X Axis Move to Inspect Position(LoadComplete)"; break;
		case  2: strMsg = "CM Scan Complete"; break;
		case  3: strMsg = "X Axis Move to Ready Position"; break;
		}
		break;
	case 6:		// Vision Cap Align
		strFun = "CapAlign";
		switch (nId) {
		case  1: strMsg = "Y Axis Move to Inspect Position(LoadComplete)"; break;
		case  2: strMsg = "Cap Scan Complete"; break;
		case  3: strMsg = "Y Axis Move to Ready Position"; break;
		}
		break;
	case 7:		// Cap Stage1
		strFun = "CapStage1";
		switch (nId) {
		case  1: strMsg = "Z Axis Move to Support Up Position"; break;
		case  2: strMsg = "Port Support Out"; break;
		case  3: strMsg = "Z Axis Move to Support Down Position"; break;
		case  4: strMsg = "Port Support In"; break;
		case  5: strMsg = "Z Axis Move to Moving Down Position"; break;
		case  6: strMsg = "Master/Slave In"; break;
		case  7: strMsg = "X Axis Move to Work Position"; break;
		case  8: strMsg = "Z Axis Move to Moving Up(Work) Position"; break;
		case  9: strMsg = "X Axis Move to Unload Position"; break;
		case 10: strMsg = "Z Axis Move to Support Down Position"; break;
		case 11: strMsg = "Port Support Out"; break;
		case 12: strMsg = "Z Axis Move to Support Up Position"; break;
		case 13: strMsg = "Port Support In"; break;
		case 14: strMsg = "Master/Slave Out"; break;
		case 15: strMsg = "Z Axis Move to Moving Up Position"; break;
		case 16: strMsg = "X Axis Move to Load Position"; break;
		}
		break;
	case 8:		// Cap Stage2
		strFun = "CapStage2";
		switch (nId) {
		case  1: strMsg = "Z Axis Move to Support Up Position"; break;
		case  2: strMsg = "Port Support Out"; break;
		case  3: strMsg = "Z Axis Move to Support Down Position"; break;
		case  4: strMsg = "Port Support In"; break;
		case  5: strMsg = "Z Axis Move to Moving Down Position"; break;
		case  6: strMsg = "Master/Slave In"; break;
		case  7: strMsg = "X Axis Move to Work Position"; break;
		case  8: strMsg = "Z Axis Move to Moving Up(Work) Position"; break;
		case  9: strMsg = "X Axis Move to Unload Position"; break;
		case 10: strMsg = "Z Axis Move to Support Down Position"; break;
		case 11: strMsg = "Port Support Out"; break;
		case 12: strMsg = "Z Axis Move to Support Up Position"; break;
		case 13: strMsg = "Port Support In"; break;
		case 14: strMsg = "Master/Slave Out"; break;
		case 15: strMsg = "Z Axis Move to Moving Up Position"; break;
		case 16: strMsg = "X Axis Move to Load Position"; break;
		}
		break;
	case 9:		// Cap Picker
		strFun = "CapPicker";
		switch (nId) {
		case  1: strMsg = "X/Y Axis Move to Cap Pickup Position"; break;
		case  2: strMsg = "Cap Pickup (Down + VacOn + Up) In Cap Tray"; break;
		case  3: strMsg = "X/Y Axis Move to Cap Buffer Position"; break;
		case  4: strMsg = "Cap Buffer Align Out"; break;
		case  5: strMsg = "Cap Unloading (Down + VacOff + Up) In Cap Buffer"; break;
		case  6: strMsg = "X/Y Axis Move to Cap Tray Position"; break;
		}
		break;
	case 10:	// Cap Buffer
		strFun = "CapBuffer";
		switch (nId) {
		case  1: strMsg = "Move to Cleaning Position"; break;
		case  2: strMsg = "Cap Cover In"; break;
		case  3: strMsg = "Cap Cover Down/Cap Clean Up + Cap Clean Air On/Off + Cap Cover Up/Cap Clean Down"; break;
		case  4: strMsg = "Cap Cover Out"; break;
		case  5: strMsg = "Move to Cap Unload Position"; break;
		case  6: strMsg = "Move to Cap Load Position"; break;
		}
		break;
	case 11:	// Assy Picker
		strFun = "Assy Picker";
		switch (nId) {
		case 1: strMsg = "Cap Pickup (Down + VacOn + Up) In Cap Buffer"; break;
		case 2: strMsg = "X/Y/Z Axis Move to Inspect Position"; break;
		case 3: strMsg = "X/Y Axis Move to Index Position"; break;
		case 4: strMsg = "Index Assy Vac Up"; break;
		case 5: strMsg = "Index Assy Vac On"; break;
		case 6: strMsg = "Cap Assembly (Down + Assembly + Tilt Check Up)"; break;
		case 7: strMsg = "Tilt Cylinder Down"; break;
		case 8: strMsg = "Tilt Cylinder Up/Index Assy Vac Down"; break;
		case 9: strMsg = "X/Y Axis Move to Cap Load Position"; break;
		}
		break;
	case 12:	// Trans Stage
		strFun = "TransStage";
		switch (nId) {
		case 1: strMsg = "Z Axis Move to Index Down"; break;
		case 2: strMsg = "Index Trans Align Out"; break;
		case 3: strMsg = "Clamp On"; break;
		case 4: strMsg = "Z Axis Move to Ready Up"; break;
		case 5: strMsg = "Index Trans Align In"; break;
		case 6: strMsg = "X/T Axis Move to Unload Position"; break;
		case 7: strMsg = "Z Axis Move to Unload Up Position"; break;
		case 8: strMsg = "Z Axis Move to Ready Up Position"; break;
		case 9: strMsg = "X/T Axis Move to Load Position"; break;
		}
		break;
	case 13:	// UnloadPicker
		strFun = "UnloadPicker";
		switch (nId) {
		case  1: strMsg = "Z Axis Move to Trans Down"; break;
		case  2: strMsg = "Trans Stage Clamp Off"; break;
		case  3: strMsg = "Z Axis Move to Ready Up"; break;
		case  4: strMsg = "X/P Axis Move to Unload Tray"; break;
		case  5: strMsg = "X/Y Axis Move to Unload Down Position"; break;
		case  6: strMsg = "Module Unloading (Down + VacuumOff + Up) In Unload Tray"; break;
		case  7: strMsg = "Z Axis Move to Ready Up"; break;
		case  8: strMsg = "X/P Axis Move to Trans Stage Position"; break;
		}
		break;
	case 14:	// UnloadStage1
		strFun = "UnloadStage1";
		switch (nId) {
		case  1: strMsg = "Z Axis Move to Support Up Position"; break;
		case  2: strMsg = "Port Support Out"; break;
		case  3: strMsg = "Z Axis Move to Support Down Position"; break;
		case  4: strMsg = "Port Support In"; break;
		case  5: strMsg = "Z Axis Move to Moving Up Position"; break;
		case  6: strMsg = "Master/Slave In"; break;
		case  7: strMsg = "Y Axis Move to Work Position"; break;
		case  8: strMsg = "Y Axis Move to Unload Position"; break;
		case  9: strMsg = "Z Axis Move to Support Down Position"; break;
		case 10: strMsg = "Port Support Out"; break;
		case 11: strMsg = "Z Axis Move to Support Up Position"; break;
		case 12: strMsg = "Port Support In"; break;
		case 13: strMsg = "Master/Slave Out"; break;
		case 14: strMsg = "Z Axis Move to Moving Down Position"; break;
		case 15: strMsg = "Y Axis Move to Load Position"; break;
		case 16: strMsg = "Z Axis Move to Moving Up Position"; break;
		}
		break;
	case 15:	// UnloadStage2
		strFun = "UnloadStage2";
		switch (nId) {
		case  1: strMsg = "Z Axis Move to Support Up Position"; break;
		case  2: strMsg = "Port Support Out"; break;
		case  3: strMsg = "Z Axis Move to Support Down Position"; break;
		case  4: strMsg = "Port Support In"; break;
		case  5: strMsg = "Z Axis Move to Moving Up Position"; break;
		case  6: strMsg = "Master/Slave In"; break;
		case  7: strMsg = "Y Axis Move to Work Position"; break;
		case  8: strMsg = "Y Axis Move to Unload Position"; break;
		case  9: strMsg = "Z Axis Move to Support Down Position"; break;
		case 10: strMsg = "Port Support Out"; break;
		case 11: strMsg = "Z Axis Move to Support Up Position"; break;
		case 12: strMsg = "Port Support In"; break;
		case 13: strMsg = "Master/Slave Out"; break;
		case 14: strMsg = "Z Axis Move to Moving Down Position"; break;
		case 15: strMsg = "Y Axis Move to Load Position"; break;
		case 16: strMsg = "Z Axis Move to Moving Up Position"; break;
		}
		break;
	}

	if (strMsg != "") {
		gLot.dAverageCycle[nFun-1][0][nId-1]++;
		gLot.dAverageCycle[nFun-1][1][nId-1] += (GetTickCount() - m_dwTakt) / 1000.0;
	}

	strLog.Format("MCC,(%02d) %s,(%02d) %s,%0.3lf", nFun, strFun, nId, strMsg, (GetTickCount() - m_dwTakt) / 1000.0);
	//g_objLogFile.Save_JobListLog(strLog);
	g_objLogFile.Save_HandlerLog(strLog);

	strLog.Format("%s,%d,%s,%0.3lf", strFun, nId, strMsg,(GetTickCount() - m_dwTakt) / 1000.0);
	g_objLogFile.Save_JobListExcel(strLog);
}

///////////////////////////////////////////////////////////////////////////////
