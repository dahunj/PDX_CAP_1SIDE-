// MESInterface.h : 헤더 파일
//
#pragma once

class CMESInterface
{
public:
	CMESInterface();
	virtual ~CMESInterface();

private:
	int		m_nReadCnt;
	CString	m_sOperID;
	CString	m_sResultFileName;

	// APD Data
	int		m_nTCount;
	int		m_nSNo;
	CString	m_sLotID;
	CString	m_sDate[3];		//[0]:년, [1]:월, [2]:일
	CString	m_sTime[500];
	CString	m_sBarID[500];
	CString	m_sJudge[500];
	CString	m_sNGCode[500];
	CString	m_sNGText[500];
	int		m_nLoadTray[500];
	int		m_nLoadPocket[500];
	int		m_nOKTray[500];
	int		m_nOKPocket[500];
	int		m_nNGTray[500];
	int		m_nNGPocket[500];

	void Read_Result(int nType);
	void Clear_Result();
	void Write_APD();

	void Begin_MESThread();
	void End_MESThread();
	BOOL Get_ThreadMES() { return m_bThreadMES; }

	void Create_Folder(CString sPath);
	void MakeFolder(CString sPath);
	void FileAllDelete(CString sPath);
	void ResultFileSearch();
	char* UTF8ToANSI(const char *pszCode);

protected:
	BOOL	m_bThreadMES;
	CWinThread *m_pThreadMES;
	static UINT	Thread_MES(LPVOID lpVoid);

	BOOL	m_bThreadMESShip;
	CWinThread *m_pThreadMESShip;
	static UINT	Thread_MESShip(LPVOID lpVoid);

public:
	BOOL	m_bMESUse, m_bStart;
	CString	m_sMESResult, m_sMESLotID, m_sReasonCode, m_sReasonText;	//m_MESResult:0[OK], 1[NG]
	CString	m_sOperPartNo, m_sOperLotID, m_sOperOpID;
	int		m_nOperCount;
	int		m_nMESCount, m_nMESSequence, m_nMESCapSequence, m_nMESShipSequence;
	BOOL	m_bMesErr;

	void Initialize(BOOL bMESUse);		//pEquipData->bUseMES
	void Set_MESUse(BOOL bMESUse);		//pEquipData->bUseMES
	void Set_Status(int	nStste);		//1:Run, 2:Stop, 3:Idle
	void Save_AviRmsData(CString sKey, CString sValue);

	void Set_CapLotChangeRequest(CString sLotID, CString sOperID);
	void Set_ShipLotChangeRequest(CString sLotID, CString sOperID);
};

extern CMESInterface g_objMES;

///////////////////////////////////////////////////////////////////////////////
