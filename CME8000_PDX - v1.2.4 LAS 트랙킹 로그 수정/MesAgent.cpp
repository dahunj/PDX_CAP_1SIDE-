// MesAgent.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "MesAgent.h"

#include "LogFile.h"
#include "Common.h"

#include "SequenceMain.h"

#include "Inspector.h"

#define	MES_AGENT_IP	"127.0.0.1"
#define MES_AGENT_PORT	10000		// MesAgent Handler Port

IMPLEMENT_DYNAMIC(CMesAgent, CWnd)

CMesAgent g_objMesAgent;

// CMesAgent

CMesAgent::CMesAgent()
{
	m_bConnected = FALSE;
	m_bHostOnline = FALSE;
	m_strRecvCmd = "";
}

CMesAgent::~CMesAgent()
{
}

BEGIN_MESSAGE_MAP(CMesAgent, CWnd)
	ON_MESSAGE(UM_CLIENT_CONNECT, OnClientConnect)
	ON_MESSAGE(UM_CLIENT_RECEIVE, OnClientReceive)
	ON_MESSAGE(UM_CLIENT_CLOSE, OnClientClose)
END_MESSAGE_MAP()

// CMesAgent 메시지 처리기입니다.

void CMesAgent::Initialize()
{
	if (m_bConnected) return;
	
	m_Client.Open_Socket(MES_AGENT_IP, MES_AGENT_PORT, this);
	if (!g_objSequenceMain.Get_IsAutoRun()) gMes.nLotStatus = 0;

	g_objLogFile.Save_MesAgentLog("MesAgent Initialize.");
}

void CMesAgent::Terminate()
{
	m_bConnected = FALSE;
	m_bHostOnline = FALSE;
	m_Client.Close_Socket();

	g_objLogFile.Save_MesAgentLog("MesAgent Terminate.");
}

/////////////////////////////////////////////////////////////////////////////

LRESULT CMesAgent::OnClientConnect(WPARAM wConnect, LPARAM lParam)
{
	m_bConnected = (BOOL)wConnect;
	if (!m_bConnected) return 0;

	Set_OperUpdate(gData.sOperID);
	g_objLogFile.Save_MesAgentLog("MesAgent Connected");
	return 0;
}

LRESULT CMesAgent::OnClientClose(WPARAM wParam, LPARAM lParam)
{
	m_bConnected = FALSE;
	m_bHostOnline = FALSE;
	m_Client.Close_Socket();
	g_objLogFile.Save_MesAgentLog("MesAgent Disconnected");
	return 0;
}

LRESULT CMesAgent::OnClientReceive(WPARAM wParam, LPARAM lParam)
{
	BYTE byRecv[1025] = { 0 };	// Buffer 1024, Last 0x00
	int nLen = m_Client.Read_Socket(byRecv);

	CString strRecvSocket, strLog;
	strRecvSocket.Format("%s", byRecv);
	m_strRecvCmd += strRecvSocket;

	while (!m_strRecvCmd.IsEmpty()) {
		int nStart = m_strRecvCmd.Find("@");
		int nEnd = m_strRecvCmd.Find("\n");

		if (nEnd < 0) break;	// 버퍼에 들어오는 중...

		if (nStart < 0 || nStart > nEnd) {
			strLog.Format("[<-] : <<Error>> %s : Start(%d), End(%d)", m_strRecvCmd, nStart, nEnd);
			g_objLogFile.Save_MesAgentLog(strLog);
			m_strRecvCmd.Delete(0, nEnd + 1);	// 쓰레기값이 채워져 있어서...
			continue;
		}

		CString strRecv = m_strRecvCmd.Mid(nStart + 1, nEnd - nStart - 1);
		m_strRecvCmd.Delete(0, nEnd + 1);

		// Inspector Log ////////////////////////////////////////////////////////////
		strLog.Format("[<-] : %s", strRecv);
		g_objLogFile.Save_MesAgentLog(strLog);
		/////////////////////////////////////////////////////////////////////////////

		char chSep = ',';
		CString strCmd, strOp;

		AfxExtractSubString(strCmd, strRecv, 0, chSep);
		AfxExtractSubString(strOp, strRecv, 1, chSep);

		CString strArg[4];
		for (int i = 0; i < 4; i++) AfxExtractSubString(strArg[i], strRecv, i + 2, chSep);

		if (strCmd == "CONTROL") {
			if (strOp == "STATE") Get_ControlState(strArg[0]);

		} else if (strCmd == "LOT") {
			if (strOp == "START")  Get_LotStart(strArg[0], strArg[1], strArg[2], strArg[3]);
			if (strOp == "CANCEL") Get_LotCancel(strArg[0], strArg[1]);

		} else if (strCmd == "CM") {
			if (strOp == "RESULT") Get_CmResult(strArg[0], strArg[1], strArg[2], strArg[3]);

		} else if (strCmd == "TIME") {
			if (strOp == "UPDATE") Get_TimeSync();
		}
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// Get Command

void CMesAgent::Get_ControlState(CString sFlag)
{
	int nOnline = atoi(sFlag);	// 1:Online, 2:Offline
	if (m_bHostOnline && nOnline == 2) g_objCommon.Show_Error(9006);	// Agent 에서 Offline 변경
	m_bHostOnline = (nOnline == 1 ? TRUE : FALSE);
}

void CMesAgent::Get_CmResult(CString sCmId, CString sJudge, CString sNgCode, CString sNgText)
{
	CString	strLog;

	if (sCmId.GetLength() < 10) {
		strLog.Format("MESAgent ReciveData ModuleID error => (%s) (%s) (%s)", sCmId, sJudge, sNgCode);
		g_objLogFile.Save_MesAgentLog(strLog);
		return;
	}

	int nPort = -1, nTray = -1, nCm = -1;
	for (int p = 0; p < 2; p++) {
		for (int i = 0; i < 40; i++) {
			for (int j = 0; j < 45; j++) {
				if (sCmId != gMes.sBarID[p][i][j]) continue;
				if (gMes.nCmResult[i][j] != 0) continue;
				nPort = p; nTray = i, nCm = j;
				break;
			}
			if (nPort != -1 && nTray != -1 && nCm != -1) break;
		}
	}

	if (nPort == -1 || nTray == -1 || nCm == -1) {
		strLog.Format("MESAgent ReciveData ModuleID Not Found => (%s) (%s) (%s)", sCmId, sJudge, sNgCode);
		g_objLogFile.Save_MesAgentLog(strLog);
		return;
	}

	if (sJudge == "NG") {
		gMes.sJudge[nPort][nTray][nCm] = "M";
		gMes.sNGCode[nPort][nTray][nCm] = sNgCode;
		gData.sNGData[nPort][nTray][nCm][0] = "MES_NG";
		strLog.Format("%s,%s,%s,%s,%s", gLot.sLotID[gData.nPNoNgTray-1], sCmId, sJudge, sNgCode, sNgText);
		g_objLogFile.Save_MesNgLog(strLog);

		if (sNgCode == "02") {	// Lot 혼입 불량 ==> sNgCode ("01":성능검사, "02":혼입검사)
			strLog.Format("Lot 혼입불량 발생\nLotId(%s)\nCmId(%s)", gLot.sLotID[gData.nPNoNgTray-1], sCmId);
			g_objCommon.Show_Alarm(strLog);
		}
	}

	gMes.nCmResult[nTray][nCm] = 1;	// Result Done
}

void CMesAgent::Get_LotStart(CString sLotId, CString sCount, CString sModel, CString sRecipe)
{
	gMes.sHostLotID = sLotId;
	gMes.nHostCount = atoi(sCount);
	gMes.sHostModel = sModel;
	gMes.sHostRecipe = sRecipe;
	int nNo = gData.nLPNo-1;
	if (nNo < 0) nNo = 0;

	//Lot ID 및 수량 확인 후 Alarm 또는 Job start 처리
	if (gMes.sHostLotID != gData.sLotID[nNo] || gMes.nHostCount != gData.nCmUseCount[nNo]) {
		gMes.nLotStatus = 0;
		g_objCommon.Show_Error(9001);
	} else {
		gMes.nLotStatus = 2;
		Set_LotStart(gData.sLotID[nNo], gData.nCmUseCount[nNo]);
		g_objSequenceMain.Begin_MainRunThread();
	}
}

void CMesAgent::Get_LotCancel(CString sCode, CString sText)
{
	gMes.sHostCancelCode = sCode;
	gMes.sHostCancelText = sText;
	gMes.nLotStatus = 0;
	g_objCommon.Show_Error(9002);
}

void CMesAgent::Get_TimeSync()
{
	g_objInspector.Set_TimeUpdate(INSPECTOR_ALL);
// 	g_objAligner.Set_TimeUpdate(ALIGNER_MAIN);
// 	g_objDispatcher.Set_TimeUpdate();
}

///////////////////////////////////////////////////////////////////////////////
// Set Command

void CMesAgent::Set_EquipState(int nFlag)
{
	CString strSend;
	strSend.Format("EQUIP,STATE,%d", nFlag);	// 1:Idle, 2:Run, 3:Down
	Send_Command(strSend);
}

void CMesAgent::Set_ErrorUpdate(int nFlag, CString sErrNo)
{
	CString strSend;
	strSend.Format("ERROR,UPDATE,%d,%s", nFlag, sErrNo);
	Send_Command(strSend);
}

void CMesAgent::Set_ControlState(int nFlag, CString sOperId)
{
	CString strSend;
	strSend.Format("CONTROL,STATE,%d,%s", nFlag, sOperId);
	Send_Command(strSend);
	if (nFlag == 2) m_bHostOnline = FALSE;	// 사용자 Offline
}

void CMesAgent::Set_LotReady(CString sLotId)
{
	CString strSend;
	strSend.Format("LOT,READY,%s", sLotId);
	Send_Command(strSend);
	gMes.dwLotStartTime = GetTickCount();
}

void CMesAgent::Set_LotStart(CString sLotId, int nCount)
{
	CString strSend;
	strSend.Format("LOT,START,%s,%d", sLotId, nCount);
	Send_Command(strSend);
}

void CMesAgent::Set_LotCancel(CString sLotId, CString sCancelMode)
{
	CString strSend;
	strSend.Format("LOT,CANCEL,%s,%s", sLotId, sCancelMode);
	Send_Command(strSend);
}

void CMesAgent::Set_LotEnd(CString sLotId, int nOk, int nNg)
{
	CString strSend;
	strSend.Format("LOT,END,%s,%d,%d", sLotId, nOk, nNg);
	Send_Command(strSend);
}

void CMesAgent::Set_LotAbort(CString sLotId)
{
	CString strSend;
	strSend.Format("LOT,ABORT,%s", sLotId);
	Send_Command(strSend);
}

void CMesAgent::Set_OperUpdate(CString sOperId)
{
	CString strSend;
	strSend.Format("OPER,UPDATE,%s", sOperId);
	Send_Command(strSend);
}

void CMesAgent::Set_CmRequest(int nTrayNo, int nCmNo, CString sLotId, CString sCmId)
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	if (!m_bHostOnline || !pEquipData->bUseMesNg || sCmId.GetLength() < 12) {
		gMes.nCmResult[nTrayNo-1][nCmNo-1] = 2;	// Result Skip
		return;
	}
	CString strSend;
	strSend.Format("CM,REQUEST,%s,%s", sLotId, sCmId);
	Send_Command(strSend);
}

void CMesAgent::Set_IdleRequest()
{
	CString strSend;
	strSend.Format("IDLE,REQUEST");
	Send_Command(strSend);
}

void CMesAgent::Set_IdleReport(CString sOperId, int nCount, CString sSTime, CString sETime, CString sCode, CString sText)
{
	CString strSend;
	strSend.Format("IDLE,REPORT,%s,%d,%s,%s,%s,%s", sOperId, nCount, sSTime, sETime, sCode, sText);
	Send_Command(strSend);
}

void CMesAgent::Set_IdleReportSet(CString sSet, CString sOperId, CString sCode)
{
	CString strSend;
	strSend.Format("IDLE,REPORT,%s,%s,%s", sSet, sOperId, sCode);
	Send_Command(strSend);
}

///////////////////////////////////////////////////////////////////////////////

void CMesAgent::Set_CmEnd(int nPosX, int nPosY, int nPortNo, int nTrayNo, int nCmNo, int nOut)
{
	if (nTrayNo < 1 || nTrayNo > 40 || nCmNo < 1 || nCmNo > 45) return;

	int nPocket;
	if		(nOut == 3) nPocket = 200 + nPosY * 3 + nPosX + 1;	// DropTable
	else if (nOut == 2) nPocket = 100 + nPosY * 6 + nPosX + 1;	// RpTray
	else if (nOut == 1) nPocket = 0   + nPosY * 6 + nPosX + 1;	// NgTray
	else				nPocket = 0;							// Ship Tray

	CString	strCmId = gMes.sBarID[nPortNo][nTrayNo-1][nCmNo-1];
	CString strResult = "";
	CString strNgCode = "";
	CString strShipId = "0";
	double  dCapX = 0.0, dCapY = 0.0, dCapR = 0.0;
	int nNo = gData.nLPNo-1;
	if (nNo < 0) nNo = 0;

	if (gMes.sJudge[nPortNo-1][nTrayNo-1][nCmNo-1] == "H" || gMes.sJudge[nPortNo-1][nTrayNo-1][nCmNo-1] == "M" || gMes.sJudge[nPortNo-1][nTrayNo-1][nCmNo-1] == "N") {
		strResult = "NG";
		strNgCode = gMes.sNGCode[nPortNo-1][nTrayNo-1][nCmNo-1];
//		g_objLogFile.Save_Print(1, nTrayNo, nCmNo, gData.nNgTrayCount, nPosX+1, nPosY+1);
	} else {
		strResult = "OK";
		strNgCode = "00";
	}

	CString strSend;
	strSend.Format("CM,END,%s,%s,%s,%s,,%s,%0.3lf,%0.3lf,%0.3lf", gData.sLotID[nNo], strCmId, strResult, strNgCode, /*nPocket,*/ strShipId, dCapX, dCapY, dCapR);
	Send_Command(strSend);
}

///////////////////////////////////////////////////////////////////////////////

void CMesAgent::Send_Command(CString sSend)
{
	CString strSendSocket, strLog;

	if (sSend.Left(7) != "CONTROL" && sSend.Left(9) != "LOT,ABORT") {
		if (!m_bHostOnline) return;
	}

	strSendSocket.Format("@%s\n", sSend);

	char chSend[1001] = { 0 };	// Buffer 1000, Last 0x00
	int nLength = strSendSocket.GetLength();
	memcpy(chSend, (LPSTR)(LPCSTR)strSendSocket, nLength);

	if (!m_Client.Write_Socket((BYTE*)chSend, nLength)) return;

	// Host Log ////////////////////////////////////////////
	strLog.Format("[->] : %s", sSend);
	g_objLogFile.Save_MesAgentLog(strLog);
	///////////////////////////////////////////////////////
}

///////////////////////////////////////////////////////////////////////////////

void CMesAgent::Test_Set()
{
}