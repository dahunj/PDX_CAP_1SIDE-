// AviUDP.h : 헤더 파일
//
#pragma once

// CAviUDP

class CAviUDP : public CWnd
{
	DECLARE_DYNAMIC(CAviUDP)

public:
	CAviUDP();
	virtual ~CAviUDP();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnUdpReceive(WPARAM wLocalPort, LPARAM lParam);

private:
	CUdpSocketCS	m_UdpAvi;

	CString		m_strRecvCmd;

	BOOL	m_bOpened;
	BOOL	m_bConnected;
	int		m_nStatusAvi;

	void Get_ConnectRequest();
	void Get_ConnectReply();
	void Get_ConnectEnd();
	void Get_StatusRequest();
	void Get_StatusReply(CString sStatus);
	void Get_StatusUpdate(CString sStatus);
	void Get_TimeUpdate(CString sTime);
	void Get_LotStart(CString sLotID, CString sPortNo, CString sTrayCnt, CString sCmCnt, CString sModel, CString sContinueLot);
	void Get_LotEnd(CString sLotID, CString sPortNo);
	void Get_ContiuneLotInfo(CString sLotID1, CString sLotID2, CString sLotID3, CString sLotID4, CString sLotID5, CString sCmUseCnt1, CString sCmUseCnt2, CString sCmUseCnt3, CString sCmUseCnt4, CString sCmUseCnt5);
	void Get_TrayLoad(CString sLotID, CString sTrayNo, CString sCmCnt, CString sPortNo, CString sLotIndex);
	void Get_TrayEnd(CString sTrayNo, CString sPortNo);
	void Get_BarcodeUpdate(CString sPortNo, CString sTrayNo, CString sCmNo, CString sBarcode);
	void Get_ApdRequest();

	void Send_Command(CString strSend);

public:
	BOOL Initialize();
	void Terminate();

	void Set_ConnectRequest();
	void Set_ConnectReply();
	void Set_ConnectEnd();
	void Set_StatusRequest();
	void Set_StatusReply(int nStatus);
	void Set_StatusUpdate(int nStatus);
	void Set_LotStart(int nPortNo);
	void Set_LotEnd(int nPortNo);
	void Set_LotReply(int nPortNo);
	void Set_TrayUnload();
	void Set_ApdReply();
	void Set_TestTrayLoad();
	void Set_TimeUpdate();

	void Set_ConnectStatus(BOOL isConnected) { m_bConnected = isConnected; }

	int	 Get_AviStatus() { return m_nStatusAvi; }

	BOOL Is_Opened() { return m_bOpened; }
	BOOL Is_Connected() { return m_bConnected; } 

	void Test_Command(int nNo);
};

extern CAviUDP g_objAviUDP;

///////////////////////////////////////////////////////////////////////////////
