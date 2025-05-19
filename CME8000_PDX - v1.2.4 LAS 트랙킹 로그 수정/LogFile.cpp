// LogFile.cpp: implementation of the CLogFile class.
//
#include "stdafx.h"
#include "LogFile.h"
#include "DataManager.h"
#include "Common.h"
#include "AJinAXL.h"
#include "SequenceMain.h"

// CLogFile
CLogFile g_objLogFile;

CCriticalSection g_csAlarmLog;
CCriticalSection g_csAlarmResetLog;
CCriticalSection g_csHandlerLog;
CCriticalSection g_csRunTimeLog;
CCriticalSection g_csInspectorLog;
CCriticalSection g_csJobListLog;
CCriticalSection g_csAviUDPLog;
CCriticalSection g_csECMLog;
CCriticalSection g_csLoadCellLog;
CCriticalSection g_csMachineStopLog;
CCriticalSection g_csMesAgentLog;
CCriticalSection g_csCmTrackingLog;
CCriticalSection g_csBarcodeLog;

CLogFile::CLogFile()
{
}

CLogFile::~CLogFile()
{
}

///////////////////////////////////////////////////////////////////////////////

void CLogFile::Create_Folder(CString sPath)
{
	if (sPath == _T("")) return;
	if (sPath.Right(1) == _T("\\")) sPath = sPath.Left(sPath.GetLength() - 1);
	if (GetFileAttributes(sPath) != -1) return;	// Directory Exist!!!

	int nFound = sPath.ReverseFind('\\');
	Create_Folder(sPath.Left(nFound));

	CreateDirectory(sPath, NULL);
}

void CLogFile::MakeFolder(CString sPath)	//"D:\\Run\\Log\\LotData\\YYYY\\MM\\DD\\";
{
	CString sTemp;
	int nIdx = 0;

	while(true) {
		nIdx = sPath.Find("\\");
		sTemp += sPath.Mid(0, nIdx) + "\\";
		CreateDirectory(sTemp,FALSE);
		sPath = sPath.Mid(nIdx+1, sPath.GetLength());
		if (nIdx<0) break;
	}
}

///////////////////////////////////////////////////////////////////////////////

void CLogFile::Save_AlarmLog(CString sLog)
{
	g_csAlarmLog.Lock();

	CString strPath = gsCurrentDir + "\\LOG\\Alarm";

	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strFile, strSave;
	strFile.Format("%s\\%04d%02d%02d.txt", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			strSave.Format("%02d:%02d:%02d %03d,%s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);	// , 추가

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	g_csAlarmLog.Unlock();
}

void CLogFile::Save_AlarmResetLog(CString sLog)
{
	g_csAlarmResetLog.Lock();

	CString strPath = gsCurrentDir + "\\LOG\\AlarmReset";

	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strFile, strSave;
	strFile.Format("%s\\%04d%02d%02d_AlarmReset.txt", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			strSave.Format("%02d:%02d:%02d %03d,%s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);	// , 추가

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	g_csAlarmResetLog.Unlock();
}

void CLogFile::Save_HandlerLog(CString sLog)
{
	g_csHandlerLog.Lock();

	CString strPath = gsCurrentDir + "\\LOG\\Handler";

	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strFile, strSave;
	strFile.Format("%s\\%04d%02d%02d.txt", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			strSave.Format("[%02d:%02d:%02d.%03d], %s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	g_csHandlerLog.Unlock();

	Save_ECMLog(4, sLog);
}

void CLogFile::Save_SaveRunTimeLog(CString sLog)
{
	g_csRunTimeLog.Lock();

	CString strPath = gsCurrentDir + "\\LOG\\RunTime";

	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strFile, strSave;
	strFile.Format("%s\\%04d%02d%02d.txt", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			strSave.Format("[%02d:%02d:%02d.%03d], %s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	g_csRunTimeLog.Unlock();
}

void CLogFile::Save_InspectorLog(CString sLog)
{
	g_csInspectorLog.Lock();

	CString strPath = gsCurrentDir + "\\LOG\\Inspector";

	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strFile, strSave;
	strFile.Format("%s\\%04d%02d%02d.txt", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			strSave.Format("[%02d:%02d:%02d.%03d],%s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	g_csInspectorLog.Unlock();
}

void CLogFile::Save_JobListLog(CString sLog, BOOL bMode)
{
	g_csJobListLog.Lock();

	CString strPath = gsCurrentDir + "\\LOG\\JobList";

	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString sTitle, strFile, strSave;
	sTitle.Format("Time,lotNum,Start_Time,End_Time,Run_Time,Tray_Count,CM_Count,Tack,Good_Count,NG_Count,NG1_Count,NG2_Count,MESNG_Count\r\n");
	strFile.Format("%s\\%04d%02d%02d_JobList.txt", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();
			if (file.GetLength() < 1) file.Write(sTitle, sTitle.GetLength());

			strSave.Format("[%02d:%02d:%02d.%03d], %s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	g_csJobListLog.Unlock();

	if (bMode == TRUE) Save_ECMLog(2, sLog);
}

void CLogFile::Save_LotResult(int nPNo, CString sLog)
{
	CString strPath1 = "D:\\EVMS\\TP\\Log";
	CString strPath2 = "D:\\EVMS\\TP\\Backup";
	CString strPath3;

	Create_Folder(strPath1);
	Create_Folder(strPath2);

	CTime tNow = CTime::GetCurrentTime();
	CTime tLog = tNow - CTimeSpan(7 * 60 * 60);

	strPath3.Format("%s\\LOG\\SPC\\%04d\\%02d\\%02d\\", gsCurrentDir, tLog.GetYear(), tLog.GetMonth(), tLog.GetDay());
	MakeFolder(strPath3);

	CString strFile1, strFile2, strFile3, strTitle, strDate, strSave;
	strFile1.Format("%s\\%s_%04d%02d%02d07_CapLot_Result.csv", strPath1, gData.sLotID[nPNo], tLog.GetYear(), tLog.GetMonth(), tLog.GetDay());
	strFile2.Format("%s\\%s_%04d%02d%02d07_CapLot_Result.csv", strPath2, gData.sLotID[nPNo], tLog.GetYear(), tLog.GetMonth(), tLog.GetDay());
	strFile3.Format("%s%s_CapLot_Result.csv", strPath3, gData.sLotID[nPNo]);

	CFile file;
	if (!file.Open(strFile1, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) return;

	strTitle.Format("날짜,Lot Start,Lot End,Term,호기,Lot ID,투입수\r\n");

	try {
		file.SeekToEnd();

		if (file.GetLength() < 1) file.Write(strTitle, strTitle.GetLength());

		strDate.Format("%02d/%02d", tNow.GetMonth(), tNow.GetDay());

		strSave.Format("%s,%s\r\n", strDate, sLog);

		file.Write(strSave, strSave.GetLength());
		file.Close();

		CopyFile(strFile1, strFile2, FALSE);	// Backup
		CopyFile(strFile1, strFile3, FALSE);	// SPC

	} catch (CFileException *pEx) {
		pEx->Delete();
	}
}

void CLogFile::Save_AviUDPLog(CString sLog)
{
	g_csAviUDPLog.Lock();

	CString strPath = gsCurrentDir + "\\LOG\\AviUDP";

	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strFile, strSave;
	strFile.Format("%s\\%04d%02d%02d.txt", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			strSave.Format("[%02d:%02d:%02d %03d] %s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	g_csAviUDPLog.Unlock();
}

void CLogFile::Save_LotTime(int nPNo, CString sLog)
{
	CString strPath1 = "D:\\EVMS\\TP\\Log";
	CString strPath2 = "D:\\EVMS\\TP\\Backup";
	CString strPath3;

	Create_Folder(strPath1);
	Create_Folder(strPath2);

	SYSTEMTIME time;
	GetLocalTime(&time);

	strPath3.Format("%s\\LOG\\SPC\\%04d\\%02d\\%02d\\", gsCurrentDir, time.wYear, time.wMonth, time.wDay);
	MakeFolder(strPath3);

	CString strFile1, strFile2, strFile3, strTitle, strTime, strPcName, strSave;
	strFile1.Format("%s\\%s_%04d%02d%02d%02d_LOT_TIME.csv", strPath1, gData.sLotID[nPNo-1], time.wYear, time.wMonth, time.wDay, time.wHour);
	strFile2.Format("%s\\%s_%04d%02d%02d%02d_LOT_TIME.csv", strPath2, gData.sLotID[nPNo-1], time.wYear, time.wMonth, time.wDay, time.wHour);
	strFile3.Format("%s%s_LOT_TIME.csv", strPath3, gData.sLotID[nPNo-1]);

	CFile file;
	if (!file.Open(strFile1, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) return;

	//m_pEquipData->sEquipName, MAIN_VERSION, gData.sLotID[nPNo-1], nTrayNo, nCmNo, gMes.sBarID[nPNo-1][nTrayNo-1][nCmNo-1], strCode, strResult
	strTitle.Format("Time,Station,Machine,Version,LotID,TrayNo,CmNo,Barcode,AG,B1,T1,T2,B2,Result\r\n");

	try {
		file.SeekToEnd();

		if (file.GetLength() < 1) file.Write(strTitle, strTitle.GetLength());

		strTime.Format("%04d-%02d-%02d %02d:%02d:%02d:%03d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);

		char szPcName[MAX_COMPUTERNAME_LENGTH + 1];
		DWORD dwNameSize = MAX_COMPUTERNAME_LENGTH + 1;
		GetComputerName(szPcName, &dwNameSize);

		strSave.Format("%s,%s,%s\r\n", strTime, szPcName, sLog);

		file.Write(strSave, strSave.GetLength());
		file.Close();

		CopyFile(strFile1, strFile2, FALSE);	// Backup
		CopyFile(strFile1, strFile3, FALSE);	// SPC

	} catch (CFileException *pEx) {
		pEx->Delete();
	}
}

void CLogFile::Save_LotError(CString sLog, int nPNo)
{
	CString strPath1 = "D:\\EVMS\\TP\\Log";
	CString strPath2 = "D:\\EVMS\\TP\\Backup";
	CString strPath3;

	Create_Folder(strPath1);
	Create_Folder(strPath2);

	SYSTEMTIME time;
	GetLocalTime(&time);

	strPath3.Format("%s\\LOG\\SPC\\%04d\\%02d\\%02d\\", gsCurrentDir, time.wYear, time.wMonth, time.wDay);
	MakeFolder(strPath3);

	int nNo = nPNo-1;
	if (nNo < 0) {
		nNo = gData.nULPNo-1;
		if (nNo < 0) nNo = gData.nLPNo-1;
		if (nNo < 0) nNo = 0;
	}

	CString strFile1, strFile2, strFile3, strTitle, strDateTime, strPcName, strSave;
	strFile1.Format("%s\\%s_%04d%02d%02d%02d_CapAVIError.csv", strPath1, gData.sLotID[nNo], time.wYear, time.wMonth, time.wDay, time.wHour);
	strFile2.Format("%s\\%s_%04d%02d%02d%02d_CapAVIError.csv", strPath2, gData.sLotID[nNo], time.wYear, time.wMonth, time.wDay, time.wHour);
	strFile3.Format("%s%s_CapAVIError.csv", strPath3, gData.sLotID[nNo]);

	CFile file;
	if (!file.Open(strFile1, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) return;

	strTitle.Format("Time,Station,Model,Version,LotNum,Event,Error Code,event,ActionTime\r\n");

	try {
		file.SeekToEnd();

		if (file.GetLength() < 1) file.Write(strTitle, strTitle.GetLength());

		strDateTime.Format("%04d-%02d-%02d %02d:%02d:%02d:%03d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);

		char szPcName[MAX_COMPUTERNAME_LENGTH + 1];
		DWORD dwNameSize = MAX_COMPUTERNAME_LENGTH + 1;
		GetComputerName(szPcName, &dwNameSize);

		strSave.Format("%s,%s,%s\r\n", strDateTime, szPcName, sLog);

		file.Write(strSave, strSave.GetLength());
		file.Close();

		CopyFile(strFile1, strFile2, FALSE);	// Backup
		CopyFile(strFile1, strFile3, FALSE);	// SPC

	} catch (CFileException *pEx) {
		pEx->Delete();
	}
}

void CLogFile::Save_CapLasLog(CString sShipLotId, CString sCapLotID, int nPort, int nTNo, int nCNo, int nPickNo)
{
	CString sPart, sQty, sLotID, sMaker, sCapPgm, sCapTool, sYear, sMonth, sDay, sSerial;
	CString sLog, sTime, sLotID_1, sLotID_2, sLotID_3, sLotID_4, sLotID_5, sLotID_6, sLotID_7;

	char chSep = '/';

	AfxExtractSubString(sPart, sCapLotID, 0, chSep);
	AfxExtractSubString(sQty, sCapLotID, 1, chSep);
	AfxExtractSubString(sLotID, sCapLotID, 2, chSep);

	sPart.Trim();
	sQty.Trim();
	sLotID.Trim();

	sLotID_1 = sLotID.Mid(0, 1); // Maker: H, D, T
	sLotID_2 = sLotID.Mid(1, 2); // Cap Program: 89, 94
	sLotID_3 = sLotID.Mid(3, 2); // Cap Tool: 01~ 99
	sLotID_4 = sLotID.Mid(5, 1); // Year: 0, 1, 2
	sLotID_5 = sLotID.Mid(6, 1); // Month: 1~9, A, B, C
	sLotID_6 = sLotID.Mid(7, 1); // Day: 1~9, A~Z except for 'I', 'O'
	sLotID_7 = sLotID.Mid(8, 3); // Serial: 001~999

	if (sLotID_1 == "H") { sMaker = "HS"; } 
	else if (sLotID_1 == "D") { sMaker = "DH"; }
	else if (sLotID_1 == "T") {	sMaker = "TS"; }

	if (sLotID_2 == "89") { sCapPgm = "i89"; } 
	else if (sLotID_2 == "94") { sCapPgm = "i94"; }
	else { sCapPgm = sLotID_2; }

	sCapTool = sLotID_3;
	sYear = Get_CapYear(sLotID_4);
	sMonth = Get_CapMonth(sLotID_5);
	sDay = Get_CapDay(sLotID_6);
	sSerial = sLotID_7;

	SYSTEMTIME time;
	GetLocalTime(&time);
	sTime.Format("%04d-%02d-%02d %02d:%02d:%02d:%03d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);

	char szPcName[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD dwNameSize = MAX_COMPUTERNAME_LENGTH + 1;
	GetComputerName(szPcName, &dwNameSize);
	
	int nINo = gData.nINoUnloadPicker;
	MOVE_DATA *pMoveData = g_objDataManager.Get_pMoveData();
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	DWORD dwTime = GetTickCount() - gData.dwCmTactStart[nTNo-1][gData.nLNoUnloadPicker];
	double dTactTime = dwTime / 1000.0 / gData.nPickerUseCnt;	//pEquipData->nLoadTrayX;	// Floating-point inexact result

	//SWversion,site,moduleConfig,LotNum,barcode,Cap Lot Num,Start,End,Cap Part No,Cap Qty,Cap Maker,Cap Program Info,Cap Tool Info,Year,Month,Day,Cap Serial No,
	//Index No,Head No,Cap Picker Table X Pos,Turn Table Work Pos,Cap Picker Table Z Pos,Cap Attach Force,Alarm Code,Tact Time,UPH
	sLog.Format("%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%d,%d,%0.3lf,%0.3lf,%0.3lf,%0.3lf,%s,%0.3lf,%0.3lf",
		MAIN_VERSION, "C3", gData.sRecipeName, gLot.sLotID[nPort-1], gMes.sBarID[nPort-1][nTNo-1][nCNo-1], sShipLotId, sCapLotID,
		gData.sCapAttachStart[nTNo-1][nCNo-1], gData.sCapAttachEnd[nTNo-1][nCNo-1], sPart, sQty, sMaker, sCapPgm, sCapTool, sYear, sMonth, sDay, sSerial,
		nINo+1, nPickNo, pMoveData->dAssyPickerX[2+nINo], pMoveData->dIndexR[0], pMoveData->dAssyPickerZ[4], gData.dAssyLoadCell[nPickNo-1], "0-0-0-0-0-0-0-0-0", dTactTime, 3600 / dTactTime);

	Save_ECMLog(5, sLog);
}

void CLogFile::Save_ECMLog(int nType, CString strLog)	//nType:1[Alarm], 2[Joblist] 3[Inspect]
{
	g_csECMLog.Lock();

	CString strFile, strFile2, sTitle, strTime, strSave;

	CString strPath = "D:\\EVMS\\TP\\Log\\";
	CString strPath2 = "D:\\EVMS\\TP\\Backup\\";

	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);
	strTime.Format("%04d-%02d-%02d %02d:%02d:%02d:%03d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);

	int nNo = gData.nULPNo-1;
	if (nNo < 0) nNo = gData.nLPNo-1;
	if (nNo < 0) nNo = 0;

	if (nType == 1) sTitle.Format("Time,Station,Type,lotNum,Error Code,Error,Start_Time,End_Time,Lead_Time\r\n");
	if (nType == 2) sTitle.Format("Time,Station,Type,lotNum,Start_Time,End_Time,Tack_Time,Tray_Count,CM_Count,Tack,Capping Fail count\r\n");
	if (nType == 3) sTitle.Format("Time,Station,Type,lotNum,Load_Pick,Inspect,Barcode,NG_Pick,Good_Pick,Trans_Pick\r\n");
	if (nType == 4) sTitle.Format("Time,Station,Type\r\n");
	if (nType == 5) sTitle.Format("Time,Station,SWversion,site,moduleConfig,LotNum,barcode,Ship Lot Num,Cap Lot Num,Start,End,Cap Part No,Cap Qty,Cap Maker,Cap Program Info,Cap Tool Info,Year,Month,Day,Cap Serial No,Index No,Head No,Cap Picker Table X Pos,Turn Table Work Pos,Cap Picker Table Z Pos,Cap Attach Force,Alarm Code,Tact Time,UPH\r\n");

	if (nType == 1) strFile.Format("%s%s_%04d%02d%02d%02d_CapAlarm.csv", strPath, gAlm.sLotID, time.wYear, time.wMonth, time.wDay, time.wHour);
	if (nType == 2) strFile.Format("%s%s_%04d%02d%02d%02d_CapJobList.csv", strPath, gLot.sLotID[nNo], time.wYear, time.wMonth, time.wDay, time.wHour);
	if (nType == 3) strFile.Format("%s%s_%04d%02d%02d%02d_Inspector.csv", strPath, gLot.sLotID[nNo], time.wYear, time.wMonth, time.wDay, time.wHour);
	if (nType == 4) strFile.Format("%s%s_%04d%02d%02d%02d_CapHandler.csv", strPath, gLot.sLotID[nNo], time.wYear, time.wMonth, time.wDay, time.wHour);
	if (nType == 5) { 
		strFile.Format("%s%s_%04d%02d%02d%02d_CapAttach.csv", strPath, gLot.sLotID[nNo], time.wYear, time.wMonth, time.wDay, time.wHour);
		strFile2.Format("%s%s_%04d%02d%02d%02d_CapAttach.csv", strPath2, gLot.sLotID[nNo], time.wYear, time.wMonth, time.wDay, time.wHour);
	}


	CFile file;
	if (!file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) return;

	try {
		file.SeekToEnd();

		if (file.GetLength() < 1) file.Write(sTitle, sTitle.GetLength());

		if (nType == 5) { strSave.Format("%s,%s,%s\r\n", strTime, gData.sComName, strLog); }
		else			{ strSave.Format("%s,%s,%s,%s\r\n", strTime, gData.sComName, gData.sRecipeName, strLog); }

		file.Write(strSave, strSave.GetLength());
		file.Close();

		if (nType == 5) { CopyFile(strFile, strFile2, FALSE); }	// Backup

	} catch (CFileException *pEx) {
		pEx->Delete();
	}

	g_csECMLog.Unlock();
}

void CLogFile::Save_CapAssembly(int nPNo, CString sLog)
{
	CString strPath = gsCurrentDir + "\\LOG\\CapAssy";

	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString sTitle, strFile, strSave, strLotID;

	strLotID = gLot.sLotID[nPNo-1];
	if (strLotID == "") strLotID = "CLOT_ID";

	sTitle.Format("TIME,LOT_ID,CAP_TRAY,CAP_NO,-,PORT_NO,CM_TRAY,CM_NO,BARCODE,CAP_OFFSET_X,CAP_OFFSET_Y,CM_OFFSET_X,CM_OFFSET_Y,OFFSET_X,OFFSET_Y,CAP_SIZE_X,CAP_SIZE_Y,CM_SIZE_X,CM_SIZE_Y\r\n");
	strFile.Format("%s\\%04d%02d%02d.csv", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();
			if (file.GetLength() < 1) file.Write(sTitle, sTitle.GetLength());

			strSave.Format("[%02d:%02d:%02d.%03d],%s,%s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, strLotID, sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////

void CLogFile::Save_TestLog(CString sLog)
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strPath = gsCurrentDir + "\\LOG\\TEST";

	Create_Folder(strPath);

	CString strFile, strSave;
	strFile.Format("%s\\%04d%02d%02d.txt", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			strSave.Format("[%02d:%02d:%02d %03d] %s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
}

void CLogFile::Save_LotLog(int nPNo)
{
	CString sFileName, strFile, strCreDate, sCreFile, sTemp, sData, sInsResult, sBarResult;
	SYSTEMTIME time;
	GetLocalTime(&time);
	int nNo = nPNo-1;
	if (nNo < 0) nNo = 0;

	if (gLot.sLotID[nNo] == "") gLot.sLotID[nNo] = "LOT_ID_DEFAULT";
	strCreDate.Format("%04d%02d%02d_%02d%02d%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	strFile.Format("LOG/LotData/%04d/%02d/%02d/%s_%s.txt", time.wYear, time.wMonth, time.wDay, gLot.sLotID[nNo], strCreDate);
	sFileName = strFile;

	sCreFile.Format("%s\\LOG\\LotData\\%04d\\%02d\\%02d\\", gsCurrentDir, time.wYear, time.wMonth, time.wDay);
	MakeFolder(sCreFile);

	CFile file;
	if (!file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) return;

	sTemp.Format("LotID,%s,Start_Time,%s,End_Time,%s,Tray_Count,%02d,CM_Count,%04d,Tack,%0.7lf,\r\n\r\n", gLot.sLotID[nNo], gLot.sStartTime[nNo], gLot.sEndTime[nNo], gLot.nTrayCount[nNo], gLot.nCmCount[nNo], gLot.dTackTime);
	sData.Format("%sTray_No,Pocket_No,ID,Inspection Result,ID Result,\r\n", sTemp);
	file.Write(sData, sData.GetLength());

	try {
		int	nCmCount = 0;
		for (int i = 0; i < 30; i++) {
			if (nCmCount > gLot.nCmCount[nNo]) break;

			for (int j = 0; j < 40; j++) {
				nCmCount++;
				if (nCmCount > gLot.nCmCount[nNo]) break;

				file.SeekToEnd();

				sData.Format("%d,%d,%s,%s,%s,\r\n", i+1, j+1, gMes.sBarID[nNo][i][j], gMes.sJudge[nNo][i][j], gMes.sNGCode[nNo][i][j]);
				file.Write(sData, sData.GetLength());
			}
		}

		file.Close();

	} catch (CFileException *pEx) {
		pEx->Delete();
	}
}

void CLogFile::Save_OperatingRatio(CString sLog)	// 가동률 작업 중
{
	CString strPath = gsCurrentDir + "\\LOG\\OperatingRatio";

	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strFile, strSave, strTitle;
	strFile.Format("%s\\%04d%02d%02d.csv", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			if (file.GetLength() < 1) {
				strTitle.Format("Time,호기,Lot ID,Lot Start,Lot End,Cycle Time,CM(EA),Run Time,Stop Time,Error Time,Error Count,수율,UPH,MTB\r\n");
				file.Write(strTitle, strTitle.GetLength());
			}

			char szPcName[MAX_COMPUTERNAME_LENGTH + 1];
			DWORD dwNameSize = MAX_COMPUTERNAME_LENGTH + 1;
			GetComputerName(szPcName, &dwNameSize);

			strSave.Format("[%02d:%02d:%02d %03d],%s,%s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, szPcName, sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
}

void CLogFile::Save_MachineStopLog(CString sLog)
{
	g_csMachineStopLog.Lock();

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strPath, strFile, strTitle, strSave;
	strPath.Format("%s\\LOG\\MachineStop", gsCurrentDir);
	Create_Folder(strPath);

	strFile.Format("%s\\%04d%02d%02d_MachineStop.csv", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			if (file.GetLength() < 1) {
				strTitle.Format("Time,Comment,Lead_Time\r\n");
				file.Write(strTitle, strTitle.GetLength());
			}

			strSave.Format("%02d:%02d:%02d %03d,%s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	g_csMachineStopLog.Unlock();
}

void CLogFile::Save_MesAgentLog(CString sLog)
{
	g_csMesAgentLog.Lock();

	CString strPath = gsCurrentDir + "\\LOG\\MESAgent";

	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strFile, strSave;
	strFile.Format("%s\\%04d%02d%02d.txt", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			strSave.Format("[%02d:%02d:%02d %03d], %s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	g_csMesAgentLog.Unlock();
}

void CLogFile::Save_CmTrackingLog(CString strOut, int nTrayCount, int nPosX, int nPosY, int nPortNo, int nTrayNo, int nCmNo)
{
	if (nTrayNo < 1 || nTrayNo > 30 || nCmNo < 1 || nCmNo > 50) return;

	g_csCmTrackingLog.Lock();

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strPath, strFile, strTitle, strSave, strJudge;
	strPath.Format("%s\\LOG\\Tracking\\%04d-%02d-%02d", gsCurrentDir, time.wYear, time.wMonth, time.wDay);
	Create_Folder(strPath);

	if (gLot.sLotID[nPortNo-1] == "") gLot.sLotID[nPortNo-1] = "LOT_ID";
	strFile.Format("%s\\%s_CapTracking.csv", strPath, gLot.sLotID[nPortNo-1]);

	CFile file;
	if (!file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) return;

	strTitle.Format("Time,Barcode,Judge,Port No,Tray No,CM No,Load Stage,Load Picker,Index Load,Index Load Jig,Unload Picker,NG Tray,NG Y,NG X,Ship Tray,Ship Y,Ship X\r\n");

	try {
		file.SeekToEnd();

		if (file.GetLength() < 1) file.Write(strTitle, strTitle.GetLength());

		//검사결과 (0:Empty, 1:Good, 2:NG)
		int nJudge = 1;
		strJudge = (nJudge == 1 ? "G" : (nJudge == 2 ? "N" : " "));

		int nLdStageNo, nLdPick, nIdxLdNo, nIdxLdJig, nUlPick;
		nLdStageNo	= gData.nCmJigNo[nPortNo-1][nTrayNo-1][nCmNo-1][LOAD_STAGE];
		nLdPick		= gData.nCmJigNo[nPortNo-1][nTrayNo-1][nCmNo-1][LOAD_PICK];
		nIdxLdNo	= gData.nCmJigNo[nPortNo-1][nTrayNo-1][nCmNo-1][INDEX_LOAD_NO];
		nIdxLdJig	= gData.nCmJigNo[nPortNo-1][nTrayNo-1][nCmNo-1][INDEX_LOAD_JIG];
		nUlPick		= gData.nCmJigNo[nPortNo-1][nTrayNo-1][nCmNo-1][UNLOAD_PICK];
		
		if (strOut == "GOOD") {
			strSave.Format("%02d:%02d:%02d,%s,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n", 
				time.wHour, time.wMinute, time.wSecond, gMes.sBarID[nPortNo-1][nTrayNo-1][nCmNo-1], strJudge, nPortNo, nTrayNo, nCmNo, 
				nLdStageNo, nLdPick, nIdxLdNo, nIdxLdJig, nUlPick,
				0, 0, 0, nTrayCount, nPosY+1, nPosX+1);
		} else {
			strSave.Format("%02d:%02d:%02d,%s,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n", 
				time.wHour, time.wMinute, time.wSecond, gMes.sBarID[nPortNo-1][nTrayNo-1][nCmNo-1], strJudge, nPortNo, nTrayNo, nCmNo, 
				nLdStageNo, nLdPick, nIdxLdNo, nIdxLdJig, nUlPick,
				nTrayCount, nPosY+1, nPosX+1, 0, 0, 0);
		}
		file.Write(strSave, strSave.GetLength());
		file.Close();

	} catch (CFileException *pEx) {
		pEx->Delete();
	}
	Save_ECMTracking(strSave, nTrayCount, nPosX, nPosY, nPortNo, nTrayNo, nCmNo);

	g_csCmTrackingLog.Unlock();
}

void CLogFile::Save_ECMTracking(CString sLog, int nTrayCount, int nPosX, int nPosY, int nPortNo, int nTrayNo, int nCmNo)
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strPath, strFile, strTitle;
	strPath = "D:\\EVMS\\TP\\Log";
	Create_Folder(strPath);

	if (gLot.sLotID[nPortNo-1] == "") gLot.sLotID[nPortNo-1] = "LOT_ID";
	strFile.Format("%s\\%s_%04d%02d%02d%02d_CapTracking.csv", strPath, gLot.sLotID[nPortNo-1], time.wYear, time.wMonth, time.wDay, time.wHour);

//	strTitle.Format("Time,Barcode,Judge,Port No,Tray No,CM No,Load Stage,Load Picker,Index Load,Index Load Jig,NG Picker,NG Stage,Good Picker,Index Good No,Index Good Jig,Transfer Picker,NG Tray,NG Y,NG X,Ship Tray,Ship Y,Ship X\r\n");
	strTitle.Format("Time,Barcode,Judge,Port No,Tray No,CM No,Load Stage,Load Picker,Index Load,Index Load Jig,Unload Picker,NG Tray,NG Y,NG X,Ship Tray,Ship Y,Ship X\r\n");

	CFile file;
	if (!file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) return;

	try {
		file.SeekToEnd();

		if (file.GetLength() < 1) file.Write(strTitle, strTitle.GetLength());

		file.Write(sLog, sLog.GetLength());
		file.Close();

	} catch (CFileException *pEx) {
		pEx->Delete();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// v1.0.1.1 Test
void CLogFile::Save_JobListExcel(CString sLog)
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strPath, strFile, strTitle, strSave, strJudge;
	strPath.Format("%s\\LOG\\LotJobList\\%04d-%02d-%02d", gsCurrentDir, time.wYear, time.wMonth, time.wDay);
	Create_Folder(strPath);
	int nNo = gData.nLPNo-1;
	if (nNo < 0) nNo = 0;


	if (gLot.sLotID[nNo] == "") gLot.sLotID[nNo] = "CLOT_ID";
	strFile.Format("%s\\%s.csv", strPath, gLot.sLotID[nNo]);	

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();
// 			if (file.GetLength() < 1) {
// 				strTitle.Format("Time,호기,Lot ID,Lot Start,Lot End,Cycle Time,LM(EA),Run Time,Stop Time,Error Time,Error Count,수율,UPH,ROS Time\r\n");
// 				file.Write(strTitle, strTitle.GetLength());
// 			}
			strSave.Format("%02d:%02d:%02d %03d,%s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
}

void CLogFile::Save_AverageCycle(int nPNo)
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strPath, strFile, strLog, strMsg = "";
	strPath.Format("%s\\LOG\\LotJobList\\%04d-%02d-%02d", gsCurrentDir, time.wYear, time.wMonth, time.wDay);
	Create_Folder(strPath);

	strFile.Format("%s\\%s_Average.csv", strPath, gLot.sLotID[nPNo]);	

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			for (int i = 1; i <= 28; i++) {
				for (int j = 1; j <= 14; j++) {
					strMsg = Job_Msg(i, j);
					if (strMsg != "") {
						double dTemp = gLot.dAverageCycle[i-1][1][j-1] == 0 ? 0.0 : gLot.dAverageCycle[i-1][1][j-1] / gLot.dAverageCycle[i-1][0][j-1];
						strLog.Format("%02d:%02d:%02d %03d,%s,%0.3lf\r\n",time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, strMsg, dTemp);
						file.Write(strLog, strLog.GetLength());
					} else 
						break;
				}
			}
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
}

void CLogFile::Save_LoadCellLog(int nPNo, CString sLog)
{
	g_csLoadCellLog.Lock();

	CString strFile, sTitle, strTime, strSave;

	CString strPath = "D:\\EVMS\\TP\\Log\\";

	Create_Folder(strPath);

	int nNo = nPNo - 1;
	if (nNo < 0) {
		nNo = gData.nULPNo - 1;
		if (nNo < 0) nNo = gData.nLPNo - 1;
		if (nNo < 0) nNo = 0;
	}

	SYSTEMTIME time;
	GetLocalTime(&time);
	strTime.Format("%04d-%02d-%02d %02d:%02d:%02d:%03d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);

	strFile.Format("%s%s_%04d%02d%02d%02d_CapAutoIPQC.csv", strPath, gLot.sLotID[nNo], time.wYear, time.wMonth, time.wDay, time.wHour);
	CFile file;
	if (!file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) return;

	try {
		file.SeekToEnd();
		sTitle.Format("Time,Station,SWversion,Model,LotNum,Start,End,Head No,Cap Attach Force\r\n");
		if (file.GetLength() < 1) file.Write(sTitle, sTitle.GetLength());
		strSave.Format("%s,%s,%s,%s,%s,%s\r\n", strTime, gData.sComName, MAIN_VERSION, gData.sRecipeName, gLot.sLotID[nNo], sLog);
		file.Write(strSave, strSave.GetLength());
		file.Close();
	} catch (CFileException *pEx) {
		pEx->Delete();
	}

	g_csLoadCellLog.Unlock();
}

void CLogFile::Save_BarcodeChkLog(CString sLog)
{
	g_csBarcodeLog.Lock();

	CString strPath = gsCurrentDir + "\\LOG\\Barcode";

	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strFile, strSave;
	strFile.Format("%s\\%04d-%02d-%02d.txt", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			strSave.Format("%s\r\n", sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	g_csBarcodeLog.Unlock();

}

// 2주 이내 같은 바코드 기록 있는지 체크 , 있으면 FALSE, 없으면 TRUE
// 파일 읽기 실패, FALSE
BOOL CLogFile::Check_BarcodeLog(CString sBarcode) 
{
	int nPoint = 0;
	int nCnt = 0;

	CString strFileName,strFilePath, strData, strTemp, strTime, strBarcode;
	CString strPath = gsCurrentDir + "\\LOG\\Barcode\\*.*";

	CFileFind finder;
	BOOL bRes = finder.FindFile(strPath);

	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);

	CTime tempTime;
	CTime curTime(sysTime);
	CTimeSpan diffTime; 

	while (bRes) {
		bRes = finder.FindNextFile();
		if (finder.IsDirectory()) continue;
		strFileName = finder.GetFileName();
		strFilePath = finder.GetFilePath();

        int nLogYear = atoi(strFileName.Left(4));
        int nLogMonth = atoi(strFileName.Mid(5, 2));
        int nLogDay = atoi(strFileName.Mid(8, 2));

		if(strFileName.GetLength() != 14 || nLogYear < 2000 || nLogMonth < 1 || nLogMonth > 12 || nLogDay < 1 || nLogDay > 31) {
			DeleteFile(strFilePath);
			continue;
		}

		CTime tempTime(nLogYear, nLogMonth , nLogDay,0,0,0);
		diffTime = curTime -  tempTime;

		// 2주전 기록은 삭제, 2주 이내는 FALSE
		if(diffTime.GetDays() > 14) {
			DeleteFile(strFilePath);
			continue;
		}
	}

	bRes = finder.FindFile(strPath);
	CFile file;

	while (bRes) {
		bRes = finder.FindNextFile();
		if (finder.IsDirectory()) continue;
		strFileName = finder.GetFileName();
		strFilePath = finder.GetFilePath();

		if (!file.Open(strFilePath, CFile::modeRead)) return FALSE;

		int nSize = (int)file.GetLength();

		char *pBuff = new char[nSize + 1];
		pBuff[nSize] = '\0';

		file.Read(pBuff, nSize);
		strData.Format("%s", pBuff);

		file.Close();
		delete pBuff;

		if(strData.Find(sBarcode) != -1 )
		{
			return FALSE;
		}
	}
	return TRUE;

}

CString CLogFile::Job_Msg(int nFun, int nId)
{
	CString strFun, strLog, strMsg = "";
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
	if (strMsg == "") return "";
	CString strTemp;
	strTemp.Format("%s,%d,%s", strFun, nId, strMsg);
	return strTemp;
}

CString CLogFile::Get_CapYear(CString sYear)
{
	CString sY;
	if (sYear == "0")		{ sY = "Y20"; } 
	else if (sYear == "1")  { sY = "Y21"; }
	else if (sYear == "2")  { sY = "Y22"; }

	return sY;
}

CString CLogFile::Get_CapMonth(CString sMonth)
{
	CString sM;

	if (sMonth == "1")		 { sM = "1"; }
	else if (sMonth == "2")  { sM = "2"; }
	else if (sMonth == "3")  { sM = "3"; }
	else if (sMonth == "4")  { sM = "4"; }
	else if (sMonth == "5")  { sM = "5"; }
	else if (sMonth == "6")  { sM = "6"; }
	else if (sMonth == "7")  { sM = "7"; }
	else if (sMonth == "8")  { sM = "8"; }
	else if (sMonth == "9")  { sM = "9"; }
	else if (sMonth == "A")  { sM = "10"; }
	else if (sMonth == "B")  { sM = "11"; }
	else if (sMonth == "C")  { sM = "12"; }

	return sM;
}

CString CLogFile::Get_CapDay(CString sDay)
{
	CString sD;
	if (sDay == "1")	   { sD = "1"; }
	else if (sDay == "2")  { sD = "2"; }
	else if (sDay == "3")  { sD = "3"; }
	else if (sDay == "4")  { sD = "4"; }
	else if (sDay == "5")  { sD = "5"; }
	else if (sDay == "6")  { sD = "6"; }
	else if (sDay == "7")  { sD = "7"; }
	else if (sDay == "8")  { sD = "8"; }
	else if (sDay == "9")  { sD = "9"; }
	else if (sDay == "A")  { sD = "10"; }
	else if (sDay == "B")  { sD = "11"; }
	else if (sDay == "C")  { sD = "12"; }
	else if (sDay == "D")  { sD = "13"; }
	else if (sDay == "E")  { sD = "14"; }
	else if (sDay == "F")  { sD = "15"; }
	else if (sDay == "G")  { sD = "16"; }
	else if (sDay == "H")  { sD = "17"; }
	else if (sDay == "J")  { sD = "18"; }
	else if (sDay == "K")  { sD = "19"; }
	else if (sDay == "L")  { sD = "20"; }
	else if (sDay == "M")  { sD = "21"; }
	else if (sDay == "N")  { sD = "22"; }
	else if (sDay == "P")  { sD = "23"; }
	else if (sDay == "Q")  { sD = "24"; }
	else if (sDay == "R")  { sD = "25"; }
	else if (sDay == "S")  { sD = "26"; }
	else if (sDay == "T")  { sD = "27"; }
	else if (sDay == "U")  { sD = "28"; }
	else if (sDay == "V")  { sD = "29"; }
	else if (sDay == "W")  { sD = "30"; }
	else if (sDay == "X")  { sD = "31"; }
	else if (sDay == "Y")  { sD = "32"; }
	else if (sDay == "Z")  { sD = "33"; }

	return sD;
}

void CLogFile::Save_Interlock(int nType)	//nType:1[정시], 2[해제] 3[설정]
{
	CString strFile, sTitle, sTitle1, strTime, strSave, strSave1, strSave2, strSave3, strSave4, strLotID, strDoor[20], sInterUse;

	SYSTEMTIME time;
	GetLocalTime(&time);

	if (nType == 1)	strTime.Format("%04d-%02d-%02d %02d:00:00.000", time.wYear, time.wMonth, time.wDay, time.wHour);
	else			strTime.Format("%04d-%02d-%02d %02d:%02d:%02d.%03d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);

	sTitle1 = "Door I/L 11,Door I/L 12,Door I/L 13,Door I/L 14,Door I/L 15,Door I/L 16,Door I/L 17,Door I/L 18,Door I/L 19,Door I/L 20,Door I/L 21,Door I/L 22,Door I/L 23,Door I/L 24,Door I/L 25,Door I/L 26,Door I/L 27,Door I/L 28,Door I/L 29,Door I/L 30";
	sTitle.Format("time,station,lotNum,barcode,SWversion,State,site,Line,Machine,operator,result,Interlock,Interlock off Time,Interlock Coverage,Door I/L 1,Door I/L 2,Door I/L 3,Door I/L 4,Door I/L 5,Door I/L 6,Door I/L 7,Door I/L 8,Door I/L 9,Door I/L 10,%s\r\n", sTitle1);
	strFile.Format("%sGSY827ADOOR1_%04d%02d%02d%02d_InterlockResult.csv", ECM_LOG, time.wYear, time.wMonth, time.wDay, time.wHour);

	strLotID = "";
	if (g_objSequenceMain.Get_IsAutoRun()) {
		strLotID = gData.sLotID[0];
		if (gData.sLotID[1].GetLength() > 1 && gData.sLotID[0].GetLength() < 1) strLotID = gData.sLotID[1];
	}

	int	   nNGTime;
	double dTime, dPer;
	if (nType == 1) {
		if (gIt.nOpenStart == 1) {
			nNGTime = (GetTickCount() - gIt.dwOpenStartTime) / 1000;
			gIt.nOpenTime = gIt.nOpenTime + nNGTime;
		}
		if (gIt.nOpenTime > 0) {
			if (gIt.nOpenTime > 3600) gIt.nOpenTime = 3600;
			dTime = gIt.nOpenTime / 60.0;
			dPer = ((3600.0 - gIt.nOpenTime) / 3600.0) * 100.0;
		} else {
			dTime = 0.0; dPer = 100.0;
		}
		gIt.dwOpenStartTime = GetTickCount();
		gIt.nOpenTime = 0;
		gIt.nWriteHH = time.wHour;

		EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
		if (pEquipData->bUseDoorLock) sInterUse = "OK";
		else						  sInterUse = "NG";
	}
	if (nType == 2) {
		sInterUse = "NG";
		gIt.nOpenStart = 1;
		gIt.dwOpenStartTime = GetTickCount();
	}
	if (nType == 3) {
		sInterUse = "OK";
		gIt.nOpenStart = 0;
		nNGTime = (GetTickCount() - gIt.dwOpenStartTime) / 1000;
		gIt.nOpenTime = gIt.nOpenTime + nNGTime;
	}
	if (nType == 4) {
		sInterUse = "OK";
		gIt.nOpenStart = gIt.nOpenTime = 0;
	}

	CFile file;
	if (!file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) return;

	try {
		file.SeekToEnd();

		if (file.GetLength() < 1) file.Write(sTitle, sTitle.GetLength());
		else {
			if (nType == 1) { file.Close(); Save_Interlock(4); return; }
		}

		strSave1.Format("%s,%s,%s,,%s,MP,Gumi Campus 3 Area,Campus 3 FOL,Cosmetic AVI,%s", strTime, gData.sComName, strLotID, MAIN_VERSION, gData.sOperID);
		if (nType == 1) strSave2.Format("A1,%s,%0.2lf,%0.1lf", sInterUse, dTime, dPer);
		if (nType == 2) strSave2.Format("F,NG,,");
		if (nType >= 3) strSave2.Format("A2,OK,,");

		DX_DATA_13 *pDX13 = g_objAJinAXL.Get_pDX13();
		if (pDX13->iDoor01Unlock) strDoor[0] = "Open"; else strDoor[0] = "Close";
		if (pDX13->iDoor02Unlock) strDoor[1] = "Open"; else strDoor[1] = "Close";
		if (pDX13->iDoor03Unlock) strDoor[2] = "Open"; else strDoor[2] = "Close";
	 	if (pDX13->iDoor04Unlock) strDoor[3] = "Open"; else strDoor[3] = "Close";
	 	if (pDX13->iDoor05Unlock) strDoor[4] = "Open"; else strDoor[4] = "Close";
		if (pDX13->iDoor06Unlock) strDoor[5] = "Open"; else strDoor[5] = "Close";
		if (pDX13->iDoor07Unlock) strDoor[6] = "Open"; else strDoor[6] = "Close";
		if (pDX13->iDoor08Unlock) strDoor[7] = "Open"; else strDoor[7] = "Close";
		if (pDX13->iDoor09Unlock) strDoor[8] = "Open"; else strDoor[8] = "Close";
		if (pDX13->iDoor10Unlock) strDoor[9] = "Open"; else strDoor[9] = "Close";
		if (pDX13->iDoor11Unlock) strDoor[10] = "Open"; else strDoor[10] = "Close";
		if (pDX13->iDoor12Unlock) strDoor[11] = "Open"; else strDoor[11] = "Close";
		if (pDX13->iDoor13Unlock) strDoor[12] = "Open"; else strDoor[12] = "Close";
		strSave3.Format("%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s", strDoor[0], strDoor[1], strDoor[2], strDoor[3], strDoor[4], strDoor[5], strDoor[6], strDoor[7], strDoor[8], strDoor[9], strDoor[10], strDoor[11], strDoor[12]);
		strSave4 = "-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-";

		strSave.Format("%s,%s,%s,%s\r\n", strSave1, strSave2, strSave3, strSave4);
		file.Write(strSave, strSave.GetLength());
		file.Close();

	} catch (CFileException *pEx) {
		pEx->Delete();
	}
}



void CLogFile::Save_PCLog(int nPNo, CString sLog)
{
	CString strPath1 = "D:\\EVMS\\TP\\Log";
	CString strPath2 = "D:\\EVMS\\TP\\Backup";

	Create_Folder(strPath1);
	Create_Folder(strPath2);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strFile1, strFile2, strTitle, strTime, strPcName, strSave;
	strFile1.Format("%s\\%s_%04d%02d%02d%02d_PC.csv", strPath1, gData.sLotID[nPNo-1], time.wYear, time.wMonth, time.wDay, time.wHour);
	strFile2.Format("%s\\%s_%04d%02d%02d%02d_PC.csv", strPath2, gData.sLotID[nPNo-1], time.wYear, time.wMonth, time.wDay, time.wHour);

	CFile file;
	if (!file.Open(strFile1, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) return;

	strTitle.Format("Time,Station,LotID,ModuleConfig,CPU,RAM,HardDisk\r\n");

	try {
		file.SeekToEnd();

		if (file.GetLength() < 1) file.Write(strTitle, strTitle.GetLength());

		strTime.Format("%04d-%02d-%02d %02d:%02d:%02d:%03d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);

		char szPcName[MAX_COMPUTERNAME_LENGTH + 1];
		DWORD dwNameSize = MAX_COMPUTERNAME_LENGTH + 1;
		GetComputerName(szPcName, &dwNameSize);

		strSave.Format("%s,%s,%s\r\n", strTime, szPcName, sLog);

		file.Write(strSave, strSave.GetLength());
		file.Close();

		CopyFile(strFile1, strFile2, FALSE);	// Backup

	} catch (CFileException *pEx) {
		pEx->Delete();
	}


}