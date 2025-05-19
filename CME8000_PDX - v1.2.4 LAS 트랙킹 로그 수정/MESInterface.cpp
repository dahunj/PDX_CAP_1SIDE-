// MESInterface.cpp : 구현 파일
//
#include "stdafx.h"
#include <vector>
#include "io.h"

#include "MESInterface.h"
#include "CME8000.h"
#include "CME8000Dlg.h"
#include "Common.h"
#include "WorkDlg.h"
#include "LogFile.h"
#include "DataManager.h"

#define RESULT_FOLDER	"D:\\MES\\VALIDATION\\"
#define MAINLOG_FOLDER	"D:\\MES\\LOG\\"
#define STATUS_FOLDER	"D:\\MES\\STATUS\\"
#define APD_FOLDER		"D:\\MES\\DATA\\"
#define MES_FOLDER_RMS	":\\MES\\Recipe\\"			// 네트워크 드라이브 설정, Handler PC에 있는 파일에 저장한다.
#define EQUIP_TYPE		"S"	//Single:S, Dual:D"
#define APD_COUNT		100	//전송Max수량
#define APD_MAX_COUNT	500	//전송Max수량
#define MES_TIME_OUT	20	//20*500=20000(10초)

CMESInterface	 g_objMES;
CCriticalSection g_csMesThread;
CCriticalSection g_csMesLog;
CCriticalSection g_csMesUse;

CMESInterface::CMESInterface(void)
{
	m_pThreadMES = NULL;
	m_bThreadMES = FALSE;
}

CMESInterface::~CMESInterface(void)
{
}

void CMESInterface::Initialize(BOOL bMESUse)
{
	MakeFolder(RESULT_FOLDER);
	MakeFolder(MAINLOG_FOLDER);
	MakeFolder(STATUS_FOLDER);
	MakeFolder(APD_FOLDER);

	m_nTCount = m_nSNo = 0;
	g_objMES.m_nMESCapSequence = 0;
	g_objMES.m_nMESShipSequence = 0;

	Set_MESUse(bMESUse);		//On-Off Line Set
	Set_Status(3);				//Idle Set
}

/////////////////////////////////////////////////////////////////////////////
// Thread Function 
UINT CMESInterface::Thread_MES(LPVOID lpVoid)
{
	//if (g_objMES.m_nMESSequence == 0) g_objMES.Clear_Result();	//수신 Folder All File 삭제

	CString strLog;

	strLog.Format("[MESInterface] Thread_MES. Start.");
	g_objLogFile.Save_MesAgentLog(strLog);

	g_objMES.m_nReadCnt = 0;
	while (g_objMES.m_bThreadMES) {
		Sleep(500);
		g_objMES.Read_Result(1);

		g_objMES.m_nReadCnt++;
		if (g_objMES.m_nReadCnt >= MES_TIME_OUT) break;
		if (g_objMES.m_bStart==FALSE) break;
	}
	if (g_objMES.m_bStart==FALSE) {
		g_objMES.m_bThreadMES = FALSE; g_objMES.m_pThreadMES = NULL;
		strLog.Format("[MESInterface] Thread_MES. m_bStart = FALSE.");
		g_objLogFile.Save_MesAgentLog(strLog);
		return 0;
	}

	if (g_objMES.m_nReadCnt >= MES_TIME_OUT) {			//TimeOver
		g_objMES.m_nMESCapSequence = 0; g_objMES.m_pThreadMES = NULL;
		if (gData.bMesFirstLot == TRUE) gData.bMesFirstLot = FALSE;
		strLog.Format("[MESInterface] Thread_MES. Timeout. ErrNo:991, OperLotId(%s), OperCmCnt(%d)", g_objMES.m_sOperLotID, g_objMES.m_nOperCount);
		g_objLogFile.Save_MesAgentLog(strLog);
		g_objCommon.Show_Error(991);
		return 0;
	}
	if (g_objMES.m_sMESResult == "1") {				// 착공실패
		g_objMES.m_nMESCapSequence = 0; g_objMES.m_pThreadMES = NULL;
		if (gData.bMesFirstLot == TRUE) gData.bMesFirstLot = FALSE;
		strLog.Format("[MESInterface] Thread_MES. Cancel. ErrNo:992, OperLotId(%s), OperCmCnt(%d)", g_objMES.m_sOperLotID, g_objMES.m_nOperCount);
		g_objLogFile.Save_MesAgentLog(strLog);
		g_objCommon.Show_Error(992);
		return 0;
	}

	strLog.Format("[MESInterface] Thread_MES. OK.");
	g_objLogFile.Save_MesAgentLog(strLog);

	g_objMES.m_nMESCapSequence = 1;
//	g_dlgWork.Set_AutoRunStatus(FALSE);

	g_objMES.m_bThreadMES = FALSE; g_objMES.m_pThreadMES = NULL;
	g_objMES.Clear_Result();	//수신 Folder All File 삭제

	return 0;
}
/////////////////////////////////////////////////////////////////////////////

void CMESInterface::Begin_MESThread()
{
	if (m_pThreadMES) return;

	m_bThreadMES = TRUE;
	m_pThreadMES = AfxBeginThread(Thread_MES, NULL);
}

void CMESInterface::End_MESThread()
{
	if (m_pThreadMES) {
		m_bThreadMES = FALSE;
		WaitForSingleObject(m_pThreadMES->m_hThread, INFINITE);
	}
}

void CMESInterface::Create_Folder(CString sPath)
{
	if (sPath == _T("")) return;
	if (sPath.Right(1) == _T("\\")) sPath = sPath.Left(sPath.GetLength() - 1);
	if (GetFileAttributes(sPath) != -1) return;

	int nFound = sPath.ReverseFind('\\');
	Create_Folder(sPath.Left(nFound));

	CreateDirectory(sPath, NULL);
}

void CMESInterface::MakeFolder(CString sPath)	//"D:\\Run\\Log\\LotData\\YYYY\\MM\\DD\\";
{
	CString sTemp;
	int nIdx = 0;

	while(true) {
		nIdx = sPath.Find("\\");
		sTemp += sPath.Mid(0, nIdx) + "\\";
		CreateDirectory(sTemp,FALSE);
		sPath = sPath.Mid(nIdx+1, sPath.GetLength());
		if(nIdx<0) break;
	}
}

///////////////////////////////////////////////////////////////////////////////

void CMESInterface::Clear_Result()
{
	m_sMESResult = m_sMESLotID = m_sReasonCode = m_sReasonText = "";
	m_nMESCount = 0;
	m_nTCount = m_nSNo = m_nReadCnt = 0;

	FileAllDelete(RESULT_FOLDER);
}

void CMESInterface::Read_Result(int nType)
{
	CString strLog, strFile, strData, sDataA, sResult[10];

	strLog.Format("[MES] Read_Result(%d)", nType);
	g_objLogFile.Save_HandlerLog(strLog);

	ResultFileSearch();

	strLog.Format("[MES] Read_Result(%d) - %s", nType, m_sResultFileName);
	g_objLogFile.Save_HandlerLog(strLog);
	if(m_sResultFileName.GetLength() < 4) return;

	strFile = m_sResultFileName;
	CFile file;
	if (!file.Open(strFile, CFile::modeRead)) return;

	int nSize = (int)file.GetLength();
	char *pBuff = new char[nSize + 1];
	pBuff[nSize] = '\0';

	file.Read(pBuff, nSize);
	strData.Format("%s", pBuff);
	strData.Replace("\r\n", ",");

	file.Close();
	delete pBuff;

	char chSepA = '=';
	char chSepB = ',';
	for(int i=0; i<8; i++) {
		AfxExtractSubString(sDataA, strData, i, chSepA);
		if (i>0) {
			AfxExtractSubString(sResult[i-1], sDataA, 0, chSepB);
		}
	}

	m_sMESResult = sResult[1];
	m_sReasonCode = sResult[2];
//	m_sReasonText = sResult[3];
	m_sReasonText.Empty();
	m_sReasonText.Insert(0, UTF8ToANSI(sResult[3]));
	m_sMESLotID = sResult[4];
	m_nMESCount = atoi(sResult[6]);

	if (nType == 1) g_objMES.m_bThreadMES = FALSE;
	if (nType == 2) g_objMES.m_bThreadMESShip = FALSE;
}

char* CMESInterface::UTF8ToANSI(const char *pszCode)
{
	BSTR    bstrWide;
	char*   pszAnsi;
	int     nLength;

	nLength = MultiByteToWideChar(CP_UTF8, 0, pszCode, lstrlen(pszCode) + 1, NULL, NULL);
	bstrWide = SysAllocStringLen(NULL, nLength);

	MultiByteToWideChar(CP_UTF8, 0, pszCode, lstrlen(pszCode) + 1, bstrWide, nLength);

	nLength = WideCharToMultiByte(CP_ACP, 0, bstrWide, -1, NULL, 0, NULL, NULL);
	pszAnsi = new char[nLength];

	WideCharToMultiByte(CP_ACP, 0, bstrWide, -1, pszAnsi, nLength, NULL, NULL);
	SysFreeString(bstrWide);

	return pszAnsi;
}


///////////////////////////////////////////////////////////////////////////////

void CMESInterface::Set_MESUse(BOOL bMESUse)
{
	int nStatus;

	g_csMesUse.Lock();

	m_bMESUse = bMESUse;
	if(m_bMESUse) nStatus = 1;
	else		  nStatus = 0;

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strPath, strFile, strSave;
	strPath.Format("%s%04d%02d%02d", STATUS_FOLDER, time.wYear, time.wMonth, time.wDay);
	strFile.Format("%s\\ValidationSet_%04d%02d%02d.txt", strPath, time.wYear, time.wMonth, time.wDay);
	Create_Folder(strPath);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			strSave.Format("[%04d/%02d/%02d %02d:%02d:%02d],CONTROL_STATE=%d\r\n",
				time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, nStatus);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}

	g_csMesUse.Unlock();
}

void CMESInterface::Set_Status(int nStste)	//1:Run, 2:Stop, 3:Idle
{
	if(!g_objMES.m_bMESUse) return;

	g_csMesLog.Lock();

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strPath, strFile, strSave, sState;
	strPath.Format("%s%04d%02d%02d", MAINLOG_FOLDER, time.wYear, time.wMonth, time.wDay);
	strFile.Format("%s\\%04d%02d%02d.txt", strPath, time.wYear, time.wMonth, time.wDay);
	Create_Folder(strPath);

	if		(nStste == 1) sState = "RUN";
	else if (nStste == 2) sState = "STOP";
	else				  sState = "IDLE";

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			strSave.Format("[%04d/%02d/%02d %02d:%02d:%02d],StateChange,UNITID=1000,WorkMode=%s\r\n",
				time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, sState);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}

	g_csMesLog.Unlock();
}

void CMESInterface::FileAllDelete(CString sPath)
{
	BOOL bRes;
    CString path, file_path, file_name;

    path.Format("%s*.*", sPath);
    CFileFind finder;
    bRes = finder.FindFile(path);

    while(bRes) {
        bRes = finder.FindNextFile();
        if(!finder.IsDirectory()) {
            file_name = finder.GetFileName();
            file_path.Format("%s%s", sPath, file_name);

			DeleteFile(file_path);
        }
    }
}

void CMESInterface::ResultFileSearch()
{
	BOOL bRes;
    CString path, file_path, file_name;

	m_sResultFileName = "";
    path.Format("%s*.*", RESULT_FOLDER);
    CFileFind finder;
    bRes = finder.FindFile(path);

    while(bRes) {
        bRes = finder.FindNextFile();
        if(!finder.IsDirectory()) {
            file_name = finder.GetFileName();
            file_path.Format("%s%s", RESULT_FOLDER, file_name);

			m_sResultFileName = file_path;
			return;
        }
    }
}

void CMESInterface::Save_AviRmsData(CString sKey, CString sValue)
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	CString strFile = pEquipData->sRmsNetDrive + (CString)MES_FOLDER_RMS + "Current_Recipe.txt";

	CIniFileCS INI(strFile);
	if (!INI.Check_File()) return;

	INI.Set_String("Current_Recipe", "Recipe_Name_Handler", gData.sRecipeName);	// Model
	INI.Set_String("Current_Recipe", sKey, sValue);	// Change Data
}

void CMESInterface::Set_CapLotChangeRequest(CString sLotID, CString sOperID)
{
	char chSep = '/';
	CString sPart, sMLotID, sCapCnt;

	Clear_Result();	//수신 Folder All File 삭제

	AfxExtractSubString(sPart, sLotID, 0, chSep);
	AfxExtractSubString(sCapCnt, sLotID, 1, chSep);
	AfxExtractSubString(sMLotID, sLotID, 2, chSep);

	sPart.Trim();
	sCapCnt.Trim();
	sMLotID.Trim();

	m_sOperPartNo = sPart; m_sOperLotID = sMLotID;	m_nOperCount = atoi(sCapCnt);	m_sOperID = sOperID;

	//Set_Status(1);	//Run Set
	Begin_MESThread();
// 	if (g_objMES.m_nMESSequence!=0) return;

	g_csMesLog.Lock();

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strPath, strFile, strSave, sState;
	strPath.Format("%s%04d%02d%02d", MAINLOG_FOLDER, time.wYear, time.wMonth, time.wDay);
	strFile.Format("%s\\%04d%02d%02d.txt", strPath, time.wYear, time.wMonth, time.wDay);
	Create_Folder(strPath);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			// MTRLTYPE=KMODC01: Cap, ChangeCode=1: 재료소진
			sState = "MaterialExchangeConfirmRequest";
			strSave.Format("%04d-%02d-%02d %02d:%02d:%02d,LOTSTATUS=%s,MTRLTYPE=KMODC01,PARTNO=%s,MLOTID=%s,SLOT=1,QUANTITY=%d,CHANGECODE=#0001,OPID=%s,APPLY_AMOUNT=0\r\n",
				time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond,
				sState, m_sOperPartNo, m_sOperLotID, m_nOperCount, m_sOperID);
			file.Write(strSave, strSave.GetLength());

			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	g_objMES.m_nMESCapSequence = 1;

	g_csMesLog.Unlock();
}

//////////////////////////////////////////////////////////////////////////////////

void CMESInterface::Set_ShipLotChangeRequest(CString sLotID, CString sOperID)
{
	char chSep = '/';
	CString strPart, strShipCnt, strLotId;

	AfxExtractSubString(strPart, sLotID, 0, chSep);	   strPart.Trim();
	AfxExtractSubString(strShipCnt, sLotID, 1, chSep); strShipCnt.Trim();
	AfxExtractSubString(strLotId, sLotID, 2, chSep);   strLotId.Trim();

	m_sOperPartNo = strPart; m_sOperLotID = strLotId; m_nOperCount = atoi(strShipCnt); m_sOperOpID = sOperID;

	if (!m_pThreadMESShip) {
		m_bThreadMESShip = TRUE;
		m_pThreadMESShip = AfxBeginThread(Thread_MESShip, NULL);
	}

	g_csMesLog.Lock();

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strPath, strFile, strSave, sState;
	strPath.Format("%s%04d%02d%02d", MAINLOG_FOLDER, time.wYear, time.wMonth, time.wDay);
	strFile.Format("%s\\%04d%02d%02d.txt", strPath, time.wYear, time.wMonth, time.wDay);
	Create_Folder(strPath);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			// MTRLTYPE=KSHTR01: Cap, ChangeCode=1: 재료소진
			sState = "MaterialExchangeConfirmRequest";
			strSave.Format("%04d-%02d-%02d %02d:%02d:%02d,LOTSTATUS=%s,MTRLTYPE=KSHTR01,PARTNO=%s,MLOTID=%s,SLOT=1,QUANTITY=%d,CHANGECODE=#0001,OPID=%s,APPLY_AMOUNT=0\r\n",
				time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond,
				sState, m_sOperPartNo, m_sOperLotID, m_nOperCount, m_sOperOpID);
			file.Write(strSave, strSave.GetLength());

			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	g_objMES.m_nMESShipSequence = 1;	// Start

	g_csMesLog.Unlock();
}

// Ship Thread Function
UINT CMESInterface::Thread_MESShip(LPVOID lpVoid)
{
	g_csMesThread.Lock();

	g_objLogFile.Save_HandlerLog("[MES] Thread_MESShip()");

	g_objMES.m_nReadCnt = 0;

	while (g_objMES.m_bThreadMESShip) {
		Sleep(100);
		g_objMES.Read_Result(2);
		g_objMES.m_nReadCnt++;
		if (g_objMES.m_nReadCnt >= MES_TIME_OUT) break;
	}

	if (g_objMES.m_nReadCnt >= MES_TIME_OUT) {	// Time Over
		g_objMES.Clear_Result();				// 수신 Folder All File 삭제
		g_objMES.m_nMESShipSequence = 0;
		g_objMES.m_pThreadMESShip = NULL;
		g_objCommon.Show_Error(9021);
		g_csMesThread.Unlock();
		return 0;
	}

	if (g_objMES.m_sMESResult == "1") {		// Lot 취소
		g_objMES.Clear_Result();			// 수신 Folder All File 삭제
		g_objMES.m_nMESShipSequence = 0;
		g_objMES.m_pThreadMESShip = NULL;
		g_objCommon.Show_Error(9022);
		g_csMesThread.Unlock();
		return 0;
	}

	g_objMES.m_nMESShipSequence = 1;			// OK
	g_objMES.m_bThreadMESShip = FALSE;
	g_objMES.m_pThreadMESShip = NULL;

	g_objMES.Clear_Result();				// 수신 Folder All File 삭제

	g_csMesThread.Unlock();

	return 0;
}
