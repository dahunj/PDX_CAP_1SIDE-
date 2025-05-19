// Inspector.h : 헤더 파일
//
#pragma once

// CInspector

class CInspector : public CWnd
{
	DECLARE_DYNAMIC(CInspector)

public:
	CInspector();
	virtual ~CInspector();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnUdpReceive(WPARAM wLocalPort, LPARAM lParam);

private:
	CUdpSocketCS	m_UdpVisionPC;

	CString		m_strRecvCmd;

	int		m_nStatusVPC;		// Vision PC 상태 (0:Not Ready, 1:Ready)
	BOOL	m_bLotReady;

	int		m_nB1ScanReq, m_nT1ScanReq;
	int		m_nB1ScanCnt, m_nT1ScanCnt;

	void Get_ConnectRequest();
	void Get_ConnectReply();
	void Get_ConnectEnd();
	void Get_StatusRequest();
	void Get_StatusReply(CString sStatus);
	void Get_StatusUpdate(CString sStatus);

	void Get_LotReady(CString sLotId, CString sPortNo, CString sProgVer, CString sParaVer);
	void Get_InspectComplete(CString sGbn, CString sLotId, CString sPortNo, CString sTrayNo, CString sCmNo, CString sJudge, CString sNGCode, CString sOffsetX, CString sOffsetY, CString sSizeX, CString sSizeY);
	void Get_BarcodeResult(CString sLotId, CString sPortNo, CString sTrayNo, CString sCmNo, CString sBarcode);
	void Get_ErrorRequest(CString sGbn, CString sLotId, CString sPortNo, CString sTrayNo, CString sCmNo, CString sErrNo);

	void Get_TimeUpdate(CString sTime);
	void Get_HeartBeat();

	void Send_Command(CString strSend);
	void Exception_Log(CString sFunc, CString sGbn, int nCase);	// Recevie Exception Log
	
public:
	void Initialize();
	void Terminate();

	void Set_ConnectRequest();
	void Set_ConnectReply();
	void Set_ConnectEnd();
	void Set_StatusRequest();
	void Set_StatusReply(int nStatus);
	void Set_StatusUpdate(int nStatus);

	void Set_LotStart(CString sLotId, int nPortNo, CString sProgVer, CString sParaVer);
	void Set_LotEnd(CString sLotId, int nPortNo);
	void Set_LoadComplete(CString sGbn, CString sLotId, int nPortNo, int nTNo1, int nTNo2, int nCNo1, int nCNo2, int nPickNo1, int nPickNo2);
	void Set_InitialRequest();
	void Set_TimeUpdate();
	void Set_LightOff();

	int  Get_VisionStatus();
	BOOL Check_LotReady();

	void Test_Command();

	int Get_Btm1ScanRequest() {return m_nB1ScanReq;}
	int Get_Top1ScanRequest() {return m_nT1ScanReq;}
};

extern CInspector g_objInspector;

///////////////////////////////////////////////////////////////////////////////
