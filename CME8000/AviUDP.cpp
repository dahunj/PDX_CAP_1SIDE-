// AviUDP.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CME8000.h"
#include "AviUDP.h"

#include "LogFile.h"
#include "Common.h"
#include "SequenceMain.h"
#include "WorkDlg.h"

#define UDP_AVI_LPORT	8050
#define UDP_AVI_HPORT	8050

IMPLEMENT_DYNAMIC(CAviUDP, CWnd)

CAviUDP g_objAviUDP;

CCriticalSection g_csAviUDP;	// Send_Command 문제 해결하기 위함

// CAviUDP

CAviUDP::CAviUDP()
{
	m_strRecvCmd = "";
	m_bOpened = FALSE;
	m_bConnected = FALSE;
	m_nStatusAvi = 0;	// 상태 (0:Not Ready, 1:Ready)
}

CAviUDP::~CAviUDP()
{
}

BEGIN_MESSAGE_MAP(CAviUDP, CWnd)
	ON_MESSAGE(UM_UDP_RECEIVE, OnUdpReceive)
END_MESSAGE_MAP()

// CAviUDPer 메시지 처리기입니다.

BOOL CAviUDP::Initialize()
{
	if(m_bOpened) return TRUE;

	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	m_bOpened = m_UdpAvi.Open_Socket(UDP_AVI_LPORT, UDP_AVI_HPORT, pEquipData->sAviIp, this);
	if (!m_bOpened) return FALSE;

	CString strLog;
	strLog.Format("Initialize : IP(%s)", pEquipData->sAviIp);
	g_objLogFile.Save_AviUDPLog(strLog);

	Set_ConnectRequest();

	return TRUE;
}

void CAviUDP::Terminate()
{
	Set_ConnectEnd();
	m_bOpened = FALSE;
	m_UdpAvi.Close_Socket();
	g_objLogFile.Save_AviUDPLog("Terminate");
}

///////////////////////////////////////////////////////////////////////////////
// UDP Socket Message

LRESULT CAviUDP::OnUdpReceive(WPARAM wLocalPort, LPARAM lParam)
{
	UINT nPort = (UINT)wLocalPort;
	BYTE byRecv[1024] = { 0 };
	CString strLog;

	int nLen = m_UdpAvi.Read_Socket(byRecv);

	if (nLen < 1) {
		strLog.Format("[C<-H] : Local Port (%d) Mismatch or Receive Data Zero (%d)", nPort, nLen);
		g_objLogFile.Save_AviUDPLog(strLog);
		return 0;
	}

	CString strRecvSocket;
	strRecvSocket.Format("%s", byRecv);
	m_strRecvCmd += strRecvSocket;

	while (!m_strRecvCmd.IsEmpty()) {
		int nStart = m_strRecvCmd.Find("@");
		int nEnd = m_strRecvCmd.Find("\n");

		if (nEnd < 0) break;	// 버퍼에 들어오는 중...

		if (nStart < 0 || nStart > nEnd) {
			strLog.Format("[C<-H] : <<Error>> %s : Start(%d), End(%d)", m_strRecvCmd, nStart, nEnd);
			g_objLogFile.Save_AviUDPLog(strLog);
			m_strRecvCmd.Delete(0, nEnd + 1);	// 쓰레기값이 채워져 있어서...
			continue;
		}

		CString strRecv = m_strRecvCmd.Mid(nStart + 1, nEnd - nStart - 1);
		m_strRecvCmd.Delete(0, nEnd + 1);

		EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
		if (!pEquipData->bUseInlineMode) return 0;

		char chSep = ',';
		CString strCmd, strOp;

		AfxExtractSubString(strCmd, strRecv, 0, chSep);
		AfxExtractSubString(strOp, strRecv, 1, chSep);

		// AviUDP Log /////////////////////////////////////////////////////
		strLog.Format("[C<-H] : %s", strRecv);
		g_objLogFile.Save_AviUDPLog(strLog);
		///////////////////////////////////////////////////////////////////////

		CString strArg[10];
		for (int i = 0; i < 10; i++) AfxExtractSubString(strArg[i], strRecv, i + 2, chSep);

		if (strCmd == "CONNECT") {
			if (strOp == "REQUEST")	Get_ConnectRequest();
			else if (strOp == "REPLY") Get_ConnectReply();
			else if (strOp == "END") Get_ConnectEnd();

		} else if (strCmd == "STATUS") {
			if (strOp == "REQUEST")	Get_StatusRequest();
			else if (strOp == "REPLY") Get_StatusReply(strArg[0]);
			else if (strOp == "UPDATE") Get_StatusUpdate(strArg[0]);

		} else if (strCmd == "LOT") {
			if (strOp == "START")	Get_LotStart(strArg[0],strArg[1],strArg[2],strArg[3],strArg[4],strArg[5]);
			else if (strOp == "END") Get_LotEnd(strArg[0],strArg[1]);
			else if (strOp == "INFO") Get_ContiuneLotInfo(strArg[0],strArg[1],strArg[2],strArg[3],strArg[4],strArg[5],strArg[6],strArg[7],strArg[8],strArg[9]);

		} else if (strCmd == "TRAY") {
			if (strOp == "LOAD") Get_TrayLoad(strArg[0],strArg[1],strArg[2],strArg[3],strArg[4]);
			else if (strOp == "END") Get_TrayEnd(strArg[0],strArg[1]);

		} else if (strCmd == "BARCODE") {
			if (strOp == "UPDATE") Get_BarcodeUpdate(strArg[0], strArg[1], strArg[2], strArg[3]);

		} else if (strCmd == "APD") {
			if (strOp == "REQUEST") Get_ApdRequest();

		} else if (strCmd == "TIME") {
			if (strOp == "UPDATE") Get_TimeUpdate(strArg[0]);
		} 
	}

	return 1;
}

///////////////////////////////////////////////////////////////////////////////
// Get Command

void CAviUDP::Get_ConnectRequest()
{
	m_bConnected = TRUE;
	Set_ConnectReply();
}

void CAviUDP::Get_ConnectReply()
{
	m_bConnected = TRUE;
}

void CAviUDP::Get_ConnectEnd()
{
	m_bConnected = FALSE;
}

void CAviUDP::Get_StatusRequest()
{
	BOOL bStatus = g_objSequenceMain.Is_MainThreadRun();
	if (bStatus) Set_StatusReply(1);
	else Set_StatusReply(0);
}

void CAviUDP::Get_StatusReply(CString sStatus)
{
	m_nStatusAvi = atoi(sStatus);
}

void CAviUDP::Get_StatusUpdate(CString sStatus)
{
	m_nStatusAvi = atoi(sStatus);
}

void CAviUDP::Get_LotStart(CString sLotID, CString sPortNo, CString sTrayCnt, CString sCmCnt, CString sModel, CString sContinueLot)
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	if (!pEquipData->bUseInlineMode) return;

	// Port No 확인. 1side Single은 -1로 온다.
	int nPortNo = atoi(sPortNo);

	//Lot 정보
	int nTrayCnt = atoi(sTrayCnt);
	int nCmCnt = atoi(sCmCnt);

	gData.bAviLotEnd[nPortNo-1] = FALSE;
	gData.sLotID[nPortNo-1] = sLotID;
	gData.nTrayUseCount[nPortNo-1] = nTrayCnt;
	gData.nCmUseCount[nPortNo-1] = nCmCnt;
	gData.sRecipeName = sModel;
	gData.bContinueLot = atoi(sContinueLot);

	g_dlgWork.Set_LotInfo(nPortNo);
}

void CAviUDP::Get_LotEnd(CString sLotID, CString sPortNo)
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	if (!pEquipData->bUseInlineMode) return;

	//LotID, Port No
	int nPortNo = atoi(sPortNo);
	gData.bAviLotEnd[nPortNo-1] = TRUE;
// 	gData.nLastTrayNo[nPortNo-1] = gData.nTNoAviPort;
}

void CAviUDP::Get_ContiuneLotInfo(CString sLotID1, CString sLotID2, CString sLotID3, CString sLotID4, CString sLotID5, CString sCmUseCnt1, CString sCmUseCnt2, CString sCmUseCnt3, CString sCmUseCnt4, CString sCmUseCnt5)
{
	gData.sLotsID[0] = sLotID1;
	gData.sLotsID[1] = sLotID2;
	gData.sLotsID[2] = sLotID3;
	gData.sLotsID[3] = sLotID4;
	gData.sLotsID[4] = sLotID5;

	gData.nCmsUseCnt[0] = atoi(sCmUseCnt1);
	gData.nCmsUseCnt[1] = atoi(sCmUseCnt2);
	gData.nCmsUseCnt[2] = atoi(sCmUseCnt3);
	gData.nCmsUseCnt[3] = atoi(sCmUseCnt4);
	gData.nCmsUseCnt[4] = atoi(sCmUseCnt5);
}

void CAviUDP::Get_TrayLoad(CString sLotID, CString sTrayNo, CString sCmCnt, CString sPortNo, CString sLotIndex)
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	if (!pEquipData->bUseInlineMode) return;

	// LotID, Load Tray Port No, Load Tray Tray No, Load Tray CM Count
	int nPortNo = atoi(sPortNo);
	int nTrayNo = atoi(sTrayNo);
	int nCmCnt = atoi(sCmCnt);

	gData.sLotID[nPortNo-1] = sLotID;
	gData.nPNoAviPort = nPortNo;
	gData.nTNoAviPort = nTrayNo;
	gData.nCmCntAviPort = nCmCnt;
	gData.nContinueLotIndex = atoi(sLotIndex);
// 	if (gData.bAviLotEnd[nPortNo-1]) { gData.nLastTrayNo[nPortNo-1] = nTrayNo; }

	if (gData.bContinueLot) {
		gData.sLotID[nPortNo-1] = sLotID;
		gData.nCmUseCount[nPortNo-1] = gData.nCmsUseCnt[gData.nContinueLotIndex];
		g_dlgWork.Set_LotInfo(nPortNo);
	}
	gData.bAviTrayLoad = TRUE;
}

void CAviUDP::Get_TrayEnd(CString sTrayNo, CString sPortNo)
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	if (!pEquipData->bUseInlineMode) return;

	// LotID, Load Tray Port No, Load Tray Tray No, Load Tray CM Count
	int nPortNo = atoi(sPortNo);
	int nTrayNo = atoi(sTrayNo);

	gData.nLastTrayNo[nPortNo-1] = nTrayNo;
}

void CAviUDP::Get_BarcodeUpdate(CString sPortNo, CString sTrayNo, CString sCmNo, CString sBarcode)
{
	int nPNo = atoi(sPortNo) - 1;
	int	nTNo = atoi(sTrayNo) - 1;
	int	nCNo = atoi(sCmNo) - 1;
	if (nTNo < 0 || nTNo > 99 || nCNo < 0 || nCNo > 200) { g_objCommon.Show_Error(6101); return; }

	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	gMes.sBarID[nPNo][nTNo][nCNo] = sBarcode;

	if (sBarcode == "NOREAD"/* && atoi(sMainNo) != 0 */) {
		CString strLog;
		strLog.Format("[Barcode NoRead] LotId(%s), PortNo(%d), TrayNo(%d), CmNo(%d)", gLot.sLotID[nPNo], nPNo, nTNo, nCNo);
		g_objLogFile.Save_HandlerLog(strLog);
	}
}

void CAviUDP::Get_ApdRequest()
{
	Set_ApdReply();
}

void CAviUDP::Get_TimeUpdate(CString sTime)
{
	// sTime Format : "2000-01-01 12:30:30 000"
	CString strTemp;
	SYSTEMTIME time;

	strTemp = sTime.Mid(0, 4);  time.wYear = atoi(strTemp);			// Year
	strTemp = sTime.Mid(5, 2);  time.wMonth = atoi(strTemp);		// Month
	strTemp = sTime.Mid(8, 2);  time.wDay = atoi(strTemp);			// Day
	strTemp = sTime.Mid(11, 2); time.wHour = atoi(strTemp);			// Hour
	strTemp = sTime.Mid(14, 2); time.wMinute = atoi(strTemp);		// Minite
	strTemp = sTime.Mid(17, 2); time.wSecond = atoi(strTemp);		// Second
	strTemp = sTime.Mid(20, 3); time.wMilliseconds = atoi(strTemp);	// MilliSecond

	SetLocalTime(&time);
}

/////////////////////////////////////////////////////////////////////////////
// Set Command

void CAviUDP::Set_ConnectRequest()
{
	CString	strSendCmd;
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	m_bConnected = FALSE;
	strSendCmd.Format("CONNECT,REQUEST,%s,%d", pEquipData->sAviIp, UDP_AVI_LPORT);
	Send_Command(strSendCmd);
}

void CAviUDP::Set_ConnectReply()
{
	CString	strSendCmd;
EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	strSendCmd.Format("CONNECT,REPLY,%s,%d", pEquipData->sAviIp, UDP_AVI_LPORT);
	Send_Command(strSendCmd);
}

void CAviUDP::Set_ConnectEnd()
{
	CString	strSendCmd;
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	strSendCmd.Format("CONNECT,END,%s,%d", pEquipData->sAviIp, UDP_AVI_LPORT);
	Send_Command(strSendCmd);
	m_bConnected = FALSE;
}

void CAviUDP::Set_StatusRequest()
{
	m_nStatusAvi = 0;
	CString	strSendCmd;
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	strSendCmd.Format("STATUS,REQUEST,%s,%d", pEquipData->sAviIp, UDP_AVI_LPORT);
	Send_Command(strSendCmd);
}

void CAviUDP::Set_StatusReply(int nStatus)
{
	CString	strSendCmd;
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	strSendCmd.Format("STATUS,REPLY,%d,%s,%d", nStatus, pEquipData->sAviIp, UDP_AVI_LPORT);
	Send_Command(strSendCmd);
}

void CAviUDP::Set_StatusUpdate(int nStatus)
{
	CString	strSendCmd;
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	strSendCmd.Format("STATUS,UPDATE,%d,%s,%d", nStatus, pEquipData->sAviIp, UDP_AVI_LPORT);
	Send_Command(strSendCmd);
}

void CAviUDP::Set_LotStart(int nPortNo)
{
	CString	strSendCmd;
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	CString strLotId = gData.sLotID[nPortNo-1];
	int nTrayUseCnt = gData.nTrayUseCount[nPortNo-1];
	int nCmUseCnt = gData.nCmUseCount[nPortNo-1];

	strSendCmd.Format("LOT,START,%s,%d,%d,%s,%d,%d,%s", strLotId, nTrayUseCnt, nCmUseCnt, pEquipData->sAviIp, UDP_AVI_LPORT, nPortNo, gData.sRecipeName);
	Send_Command(strSendCmd);
}

void CAviUDP::Set_LotEnd(int nPortNo)
{
	CString	strSendCmd;
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	CString strLotId = gLot.sLotID[nPortNo-1];

	strSendCmd.Format("LOT,END,%s,%d", strLotId, nPortNo);
	Send_Command(strSendCmd);
}

void CAviUDP::Set_LotReply(int nPortNo)
{
	CString	strSendCmd;
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	CString strLotId = gData.sLotID[nPortNo-1];

	strSendCmd.Format("LOT,REPLY,%d,%s", nPortNo, strLotId);
	Send_Command(strSendCmd);
}

void CAviUDP::Set_TrayUnload()
{
	CString	strSendCmd;
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	strSendCmd.Format("TRAY,UNLOAD,%s,%d", pEquipData->sAviIp, UDP_AVI_LPORT);
	Send_Command(strSendCmd);
}

void CAviUDP::Set_ApdReply()
{
	CString	strSendCmd;
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	double dCapForceTotal = 0.0, dCapForceAvg = 0.0;
	for (int i = 0; i < gData.nPickerUseCnt; i++) dCapForceTotal += gData.dAssyLoadCell[i];
	dCapForceAvg = dCapForceTotal / double(gData.nPickerUseCnt);

	strSendCmd.Format("APD,REPLY,%0.3lf,%s,%d", dCapForceAvg, pEquipData->sAviIp, UDP_AVI_LPORT);
	Send_Command(strSendCmd);
}

void CAviUDP::Set_TestTrayLoad()
{
	CString	strSendCmd;
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	strSendCmd.Format("TEST,LOAD,%s,%d", pEquipData->sAviIp, UDP_AVI_LPORT);
	Send_Command(strSendCmd);
}

void CAviUDP::Set_TimeUpdate()
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strTime;
	strTime.Format("%04d-%02d-%02d %02d:%02d:%02d %03d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);

	CString	strSendCmd;
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	strSendCmd.Format("TIME,UPDATE,%s,%s,%d", strTime, pEquipData->sAviIp, UDP_AVI_LPORT);
	Send_Command(strSendCmd);
}

/////////////////////////////////////////////////////////////////////////////
// UDP Socket Send Message

void CAviUDP::Send_Command(CString strSend)
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	if (!pEquipData->bUseInlineMode) return;

	CString strLog;
	strLog.Format("[C->H] : %s", strSend);
	g_objLogFile.Save_AviUDPLog(strLog);

	CString strSendSocket;
	strSendSocket.Format("@%s\n", strSend);

	char chSend[1024] = { 0 };
	int nLength = strSendSocket.GetLength();
	memcpy(chSend, (LPSTR)(LPCSTR)strSendSocket, nLength);

	g_csAviUDP.Lock();	// Critical Section

	m_UdpAvi.Write_Socket((BYTE*)chSend, nLength);

	g_csAviUDP.Unlock();	// Critical Section
}

/////////////////////////////////////////////////////////////////////////////

void CAviUDP::Test_Command(int nNo)
{
}
