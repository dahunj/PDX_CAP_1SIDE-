// SequenceMain.h : 헤더 파일
//
#pragma once

#include "AJinAXL.h"
#include "DataManager.h"
#include "LoopTimer.h"

const double TRAY_WIDTH = 260.0;	// Good Tray 1/2 간섭없는 간격

class CSequenceMain  
{
public:
	CSequenceMain();
	virtual ~CSequenceMain();
	
private:
	AXIS_STATUS *m_pStatus[AXIS_COUNT];
	AXIS_PARAM	*m_pParam[AXIS_COUNT];
	
	DX_DATA_00	*m_pDX00;	// Input (X0000 - X0031)
	DX_DATA_01	*m_pDX01;	// Input (X0100 - X0131)
	DX_DATA_02	*m_pDX02;	// Input (X0200 - X0231)
	DX_DATA_03	*m_pDX03;	// Input (X0300 - X0331)
	DX_DATA_04	*m_pDX04;	// Input (X0400 - X0431)
	DX_DATA_05	*m_pDX05;	// Input (X0500 - X0531)
	DX_DATA_06	*m_pDX06;	// Input (X0600 - X0631)
	DX_DATA_07	*m_pDX07;	// Input (X0700 - X0731)
	DX_DATA_08	*m_pDX08;	// Input (X0800 - X0831)
	DX_DATA_09	*m_pDX09;	// Input (X0900 - X0931)
	DX_DATA_10	*m_pDX10;	// Input (X1000 - X1031)
	DX_DATA_11	*m_pDX11;	// Input (X1100 - X1131)
	DX_DATA_12	*m_pDX12;	// Input (X1200 - X1231)
	DX_DATA_13	*m_pDX13;	// Input (X1300 - X1331)

	DY_DATA_00	*m_pDY00;	// Output (Y0000 - Y0031)
	DY_DATA_01	*m_pDY01;	// Output (Y0100 - Y0131)
	DY_DATA_02	*m_pDY02;	// Output (Y0200 - Y0231)
	DY_DATA_03	*m_pDY03;	// Output (Y0300 - Y0331)
	DY_DATA_04	*m_pDY04;	// Output (Y0400 - Y0431)
	DY_DATA_05	*m_pDY05;	// Output (Y0500 - Y0531)
	DY_DATA_06	*m_pDY06;	// Output (Y0600 - Y0631)
	DY_DATA_07	*m_pDY07;	// Output (Y0700 - Y0731)
	DY_DATA_08	*m_pDY08;	// Output (Y0800 - Y0831)
	DY_DATA_09	*m_pDY09;	// Output (Y0900 - Y0931)
	DY_DATA_10	*m_pDY10;	// Output (Y1000 - Y1031)
	DY_DATA_11	*m_pDY11;	// Output (Y1100 - Y1131)
	DY_DATA_12	*m_pDY12;	// Output (Y1200 - Y1231)
	DY_DATA_13	*m_pDY13;	// Output (Y1300 - Y1331)
	
	EQUIP_DATA	*m_pEquipData;
	MOVE_DATA	*m_pMoveData;

	CString		m_strLog;	// Log String

protected:
	CWinThread	*m_pThreadVacuumCap;
	CWinThread	*m_pThreadVacuumAssy;
	CWinThread	*m_pThreadVacuumUnload;
	CWinThread	*m_pThreadVacuumCapMulti;
	CWinThread	*m_pThreadVacuumUnloadMulti;
	CWinThread	*m_pThreadBeep;
	static UINT	Thread_Vacuum_Cap(LPVOID lpVoid);
	static UINT	Thread_Vacuum_Assy(LPVOID lpVoid);
	static UINT	Thread_Vacuum_Unload(LPVOID lpVoid);
	static UINT	Thread_Vacuum_Cap_Multi(LPVOID lpVoid);
	static UINT	Thread_Vacuum_Unload_Multi(LPVOID lpVoid);
	static UINT	Thread_Beep(LPVOID lpVoid);

	BOOL		m_bThreadMainRun;
	CWinThread	*m_pThreadMainRun;
	static UINT	Thread_MainRun(LPVOID lpVoid);

private:
	void Vacuum_CapPicker(int nSub);	// Thread
	void Vacuum_AssyPicker(int nSub);	// Thread
	void Vacuum_UnloadPicker(int nSub);	// Thread
	void Vacuum_CapPickerMulti(int nSub);	// Thread
	void Vacuum_UnloadPickerMulti(int nSub);	// Thread

private:
	int			m_nTrayPickCase;		//  1. (Error : 3100)
	int			m_nLoadStage1Case;		//  2. (Error : 3200)
	int			m_nLoadStage2Case;		//  3. (Error : 3300)
	int			m_nLoadPickCase;		//  4. (Error : 3400)
	int			m_nMainIndexCase;		//  5. (Error : 3500)
	int			m_nVisCmAlignCase;		//  6. (Error : 3600)
	int			m_nVisCapAlignCase;		//  7. (Error : 3700)
	int			m_nCapStage1Case;		//  8. (Error : 3800)  
	int			m_nCapStage2Case;		//  9. (Error : 3900)  
	int			m_nCapPickCase;			// 10. (Error : 4000)
	int			m_nCapBufferCase;		// 11. (Error : 4100)
	int			m_nAssyPickCase;		// 12. (Error : 4200)
	int			m_nTransStageCase;		// 13. (Error : 4300)
	int			m_nUnloadPickCase;		// 14. (Error : 4400)
	int			m_nUnloadStage1Case;	// 15. (Error : 4500)
	int			m_nUnloadStage2Case;	// 16. (Error : 4600)

	DWORD		m_dwFirstLoad;			//  Lot Start 후 첫 모듈이 Main Index에 안착되기까지의 시간
	DWORD		m_dwLastUnLoad;			//  마지막 모듈이 Main Index 안착 후 Lot End 되는 시점까지

	DWORD		m_dwTrayPick;		//  1. (Error : 3100)
	DWORD		m_dwLoadStage1;		//  2. (Error : 3200)
	DWORD		m_dwLoadStage2;		//  3. (Error : 3300)
	DWORD		m_dwLoadPick;		//  4. (Error : 3400)
	DWORD		m_dwMainIndex;		//  5. (Error : 3500)
	DWORD		m_dwVisCmAlign;		//  6. (Error : 3600)
	DWORD		m_dwVisCapAlign;	//  7. (Error : 3700)
	DWORD		m_dwCapStage1;		//  8. (Error : 3800)
	DWORD		m_dwCapStage2;		//  9. (Error : 3900)
	DWORD		m_dwCapPick;		// 10. (Error : 4000)
	DWORD		m_dwCapBuffer;		// 11. (Error : 4100)
	DWORD		m_dwAssyPick;		// 12. (Error : 4200)
	DWORD		m_dwTransStage;		// 13. (Error : 4300)
	DWORD		m_dwUnloadPick;		// 14. (Error : 4400)
	DWORD		m_dwUnloadStage1;	// 15. (Error : 4500)
	DWORD		m_dwUnloadStage2;	// 16. (Error : 4600)
	
	CLoopTimer	m_tTrayPickLoop;		//  1. (Error : 3100)
	CLoopTimer	m_tLoadStage1Loop;		//  2. (Error : 3200)
	CLoopTimer	m_tLoadStage2Loop;		//  3. (Error : 3300)
	CLoopTimer	m_tLoadPickLoop;		//  4. (Error : 3400)
	CLoopTimer	m_tMainIndexLoop;		//  5. (Error : 3500)
	CLoopTimer	m_tVisCmAlignLoop;		//  6. (Error : 3600)
	CLoopTimer	m_tVisCapAlignLoop;		//  7. (Error : 3700)
	CLoopTimer	m_tCapStage1Loop;		//  8. (Error : 3800)
	CLoopTimer	m_tCapStage2Loop;		//  9. (Error : 3900)
	CLoopTimer	m_tCapPickLoop;			// 10. (Error : 4000)
	CLoopTimer	m_tCapBufferLoop;		// 11. (Error : 4100)
	CLoopTimer	m_tAssyPickLoop;		// 12. (Error : 4200)
	CLoopTimer	m_tTransStageLoop;		// 13. (Error : 4300)
	CLoopTimer	m_tUnloadPickLoop;		// 14. (Error : 4400)
	CLoopTimer	m_tUnloadStage1Loop;	// 15. (Error : 4500)
	CLoopTimer	m_tUnloadStage2Loop;	// 16. (Error : 4600)

	BOOL	m_bLoadLotEnd;			// Load Stage 에서 판단
	BOOL	m_bUnloadLotEnd;		// Sort Picker 에서 판단

	BOOL	m_bLotLoadEnable[2];		// 2nd Lot 투입하기 위해...// Load Port 트레이 로드 완료하고 대기 중일때 TRUE;
	BOOL	m_bBtm1ModulePick;			// R04B 전용.

#ifdef VISION_REPEAT
	int		m_nStartIndexJig;		// 비전 반복성 검증시 첫번째 Index
#endif

	int		m_nLoadPickMultiCnt;		// Load Picker Multi Pick Count
	int		m_nCapPickMultiCnt;			// Cap Picker Multi Pick Count
	int		m_nUnloadPickMultiCnt;		// Unload Picker Multi Pick Count

	int		m_nUnloadLotCmCnt;			// ContinueLot 사용시 확인용.
	int		m_nUnloadLotIndex;

	CString m_strLoadCellStart;
	CString m_strLoadCellEnd;

	BOOL	m_bLoadPortTrayExist;		// Load Port에 Tray가 있는지 없는지 변수로도 확인할수 있게 해준다.

private:
	BOOL TrayPicker_Run();			//  1. (Error : 3100)
	BOOL LoadStage1_Run();			//  2. (Error : 3200)
	BOOL LoadStage2_Run();			//  3. (Error : 3300)
	BOOL LoadPicker_Run();			//  4. (Error : 3400)
	BOOL MainIndex_Run();			//  5. (Error : 3500)
	BOOL VisionCM_Run();			//  6. (Error : 3600)
	BOOL VisionCap_Run();			//  7. (Error : 3700)
	BOOL CapStage1_Run();			//  8. (Error : 3800)
	BOOL CapStage2_Run();			//  9. (Error : 3900)
	BOOL CapPicker_Run();			// 10. (Error : 4000)
	BOOL CapBuffer_Run();			// 11. (Error : 4100)
	BOOL AssyPicker_Run();			// 12. (Error : 4200)
	BOOL TransStage_Run();			// 13. (Error : 4300)
	BOOL UnloadPicker_Run();		// 14. (Error : 4400)
	BOOL UnloadStage1_Run();		// 15. (Error : 4500)
	BOOL UnloadStage2_Run();		// 16. (Error : 4600)
		
	BOOL LotEnd_Run();

	void Job_LotStart(int nPort);
	void Job_LotEnd(int nPort);
	void Beep_Post(int nState, int nTime);

	void Init_LoadTray(int nPNo, int nSNo);
	void Init_CapTray();
	void Init_UnloadTray();

	BOOL Check_AviTrayPickup();
	BOOL Check_LoadTrayEmpty(int nNo=0);
	BOOL Check_LoadTrayLineEmpty(int nLine);
	BOOL Check_CapTrayEmpty();
	BOOL Check_CapTrayLineEmpty(int nLine);
	BOOL Check_UnloadTrayEmpty();
	BOOL Check_LoadPickerEmpty();
	BOOL Check_CapPickerEmpty();
	BOOL Check_AssyPickerEmpty();
	BOOL Check_UnloadPickerEmpty();
	BOOL Check_CapBufferEmpty();
	BOOL Check_TransStageEmpty();	
	
	BOOL Check_LoadPickerFull();
	BOOL Check_CapPickerFull();
	BOOL Check_CapBufferFull();
	BOOL Check_UnloadTrayFull();

	BOOL Check_IndexModule();
	BOOL Check_IndexEmpty(int nPos);
	BOOL Check_PickerEmpty(int nPos);

	BOOL Check_CapInspAllGood();

 	int  Get_CmScanCnt();
	int	 Get_CapScanCnt();
// 	BOOL Select_BtmScanPos(int nPos, int nScanCnt);
// 	BOOL Select_Top1ScanPos(int nPos, int nScanCnt);
// 	BOOL Select_Top2ScanPos(int nPos, int nScanCnt);

	BOOL Select_LoadTrayPos(int &nTrayPosX, int &nTrayPosY);
	BOOL Select_CapTrayPos(int &nTrayPosX, int &nTrayPosY, int &nCnt);
	BOOL Select_UnloadTrayPos(int &nTrayPosX, int &nTrayPosY, int &nCnt);
	BOOL Select_LoadPickerPos(int &nPos, int &nCnt);
	BOOL Select_CapPickerPos(int &nPos, int &nCnt);
	BOOL Select_AssyPickerPos(int &nPos, int &nCnt);
	BOOL Select_AssyPickerNgPos(int &nPos);
	BOOL Select_UnloadPickerPos(int &nPos, int &nCnt);
	
	void Set_InspectJigNo(int nType, int nScanCnt, int &nINo1, int &nINo2);

	BOOL Check_CmAlignDone();
	BOOL Check_CapAlignDone();

	BOOL Check_LoadLotEnd(int nPNo, int nMode=0);
	BOOL Check_UnloadLotEnd(int nMode=0);

	BOOL Get_VisionInspectUse();
	void Set_TaktLog(int nNo);
	void Set_IndexEnd();

public:
	double	m_dAlignZ, m_dAngleZ, m_dAngleX, m_dAngleY, m_dAngleR, m_dBtm1Z, m_dTop1Z, m_dTop1MirrorZ, m_dTop2Z, m_dBtm2Z;
	BOOL	m_bReqShiftB1Y, m_bReqMirrorT1Z, m_bReqShiftT2Y;
	DWORD	m_dwULPickUpTime;		// Tray 1번 모듈 집은 시간.
	DWORD	m_dwULCycleTime;		//  Unload 시작한 Tray와 다음 Tray Unload 시작되는 Cycle Time 저장해준다.

	void Begin_MainRunThread();
	void End_MainRunThread();
	void Run_Replay();
	
	BOOL Is_MainThreadRun() { return m_bThreadMainRun; }
	BOOL Check_Mode();
	
	void Reset_MainRunCase();
	int *Get_pMainRunCase();
	int  Get_MainRunCase(int nRun);
	void Set_MainRunCase(int nRun, int nCase);
	void Set_MainRunLoop(int nRun, int nLoop);

	BOOL Get_IsAutoRun();
	void Set_ClearRunData(int nType);

	BOOL Check_MainIndexCylinder();
	BOOL Check_LoadTrayLoading(int nPNo);

	BOOL Get_LotLoadEnable(int nNo) { return m_bLotLoadEnable[nNo]; }
	void Set_LotLoadEnable(BOOL bOn, int nNo) { m_bLotLoadEnable[nNo] = bOn; }

	void Init_NgTray(int nNo);

#ifdef VISION_REPEAT
	void Set_StartIndexJig(int nJig) { m_nStartIndexJig = nJig; }
#endif

	void Test_CSKIM();	// Run Test

};

extern CSequenceMain g_objSequenceMain;

///////////////////////////////////////////////////////////////////////////////
