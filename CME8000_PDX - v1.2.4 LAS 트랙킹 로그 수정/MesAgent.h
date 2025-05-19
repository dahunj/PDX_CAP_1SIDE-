// MesAgent.h : 헤더 파일
//
#pragma once

// CMesAgent

class CMesAgent : public CWnd
{
	DECLARE_DYNAMIC(CMesAgent)

public:
	CMesAgent();
	virtual ~CMesAgent();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnClientConnect(WPARAM wConnect, LPARAM lParam);
	afx_msg LRESULT OnClientReceive(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClientClose(WPARAM wParam, LPARAM lParam);

private:
	CClientSocketCS m_Client;
	
	BOOL	m_bConnected;
	BOOL	m_bHostOnline;
	CString m_strRecvCmd;

private:
	void Get_ReciveData();
	void Get_ControlState(CString sFlag);	// 1:Online, 2:Offline
	void Get_LotStart(CString sLotId, CString sCount, CString sModel, CString sRecipe);
	void Get_LotCancel(CString sCode, CString sText);
	void Get_CmResult(CString sCmId, CString sJudge, CString sNgCode, CString sNgText);		// M30에서는 MES 정보 받지 않음.

	void Send_Command(CString sSend);

public:
	void Initialize();
	void Terminate();

	BOOL Is_Connected() { return m_bConnected; }
	BOOL Is_HostOnline() { return m_bHostOnline; }

	void Set_OperUpdate(CString sOperId);
	void Set_EquipState(int nFlag);		// 1:Idle, 2:Run, 3:Down
	void Set_ErrorUpdate(int nFlag, CString sErrNo);	// 0:해제, 1:발생
	void Set_ControlState(int nFlag, CString sOperId);	// 1:Onine, 2:Offline
	void Set_LotReady(CString sLotId);
	void Set_LotStart(CString sLotId, int nCount);
	void Set_LotEnd(CString sLotId, int nOk, int nNg);
	void Set_LotCancel(CString sLotId, CString sCancelMode); // Cancel Mode : Host가 취소했으면 H, 작업자라면 O
	void Set_LotAbort(CString sLotId);
	void Set_CmRequest(int nTrayNo, int nCmNo, CString sLotId, CString sCmId);
	void Set_IdleRequest();
	
	void Set_IdleReport(CString sOperId, int nCount, CString sSTime, CString sETime, CString sCode, CString sText);	// 1개만 보냄
	void Set_IdleReportSet(CString sSet, CString sOperId, CString sCode);	// 비가동 집계 Set / Reset 에 쓰임.

	void Set_CmEnd(int nPosX, int nPosY, int nPortNo, int nTrayNo, int nCmNo, int nOut);
	void Get_TimeSync();

	void Test_Set();
};

extern CMesAgent g_objMesAgent;

///////////////////////////////////////////////////////////////////////////////
