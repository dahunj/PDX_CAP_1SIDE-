// LogFile.h : 헤더 파일
//
#pragma once

class CLogFile  
{
public:
	CLogFile();
	virtual ~CLogFile();

public:
	void Create_Folder(CString sPath);
	void MakeFolder(CString sPath);

	void Save_AlarmLog(CString sLog);
	void Save_AlarmResetLog(CString sLog);
	void Save_HandlerLog(CString sLog);
	void Save_SaveRunTimeLog(CString sLog);
	void Save_InspectorLog(CString sLog);
	void Save_JobListLog(CString sLog, BOOL bMode=FALSE);
	void Save_LotResult(int nPNo, CString sLog);
	void Save_LotLog(int nPNo);
	void Save_AviUDPLog(CString sLog);
	void Save_LotTime(int nPNo, CString sLog);
	void Save_LotError(CString sLog, int nPNo=0);
	void Save_ECMLog(int nType, CString strLog);	//nType:1[Alarm], 2[Joblist] 3[Inspect]
	void Save_TestLog(CString sLog);
	void Save_OperatingRatio(CString sLog);
	void Save_CapAssembly(int nPNo, CString sLog);
	void Save_CapLasLog(CString sShipLotId, CString sCapLotID, int nPort, int nTNo, int nCNo, int nPickNo);
	void Save_MachineStopLog(CString sLog);
	void Save_MesAgentLog(CString sLog);
	void Save_CmTrackingLog(CString strOut, int nTrayCount, int nPosX, int nPosY, int nPortNo, int nTrayNo, int nCmNo);
	void Save_ECMTracking(CString sLog, int nTrayCount, int nPosX, int nPosY, int nPortNo, int nTrayNo, int nCmNo);

	void Save_PCLog(int nPNo, CString sLog);

	void Save_BarcodeChkLog(CString sLog);
	BOOL Check_BarcodeLog(CString sBarcode);

	void Save_JobListExcel(CString sLog);
	void Save_AverageCycle(int nPNo);
	void Save_LoadCellLog(int nPNo, CString sLog);
	CString Job_Msg(int nFun, int nId);

	CString Get_CapYear(CString sYear);
	CString Get_CapMonth(CString sMonth);
	CString Get_CapDay(CString sDay);
	void Save_Interlock(int nType);					//nType:1[정시], 2[해제] 3[설정]
};

extern CLogFile g_objLogFile;

///////////////////////////////////////////////////////////////////////////////
