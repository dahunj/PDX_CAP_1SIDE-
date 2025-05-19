// SequenceInit.h : 헤더 파일
//
#pragma once

#include "AJinAXL.h"
#include "DataManager.h"
#include "LoopTimer.h"

class CSequenceInit
{
public:
	CSequenceInit();
	virtual ~CSequenceInit();
	
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

	int			m_niMainInitCase;		//  0. (Error : 1000)
	int			m_niLoadPickCase;		//  1. (Error : 1100)
	int			m_niLoadStageCase;		//  2. (Error : 1200)
	int			m_niMainIndexCase;		//  3. (Error : 1300)
	int			m_niAssyPickCase;		//  4. (Error : 1400)
	int			m_niCapPickCase;		//  5. (Error : 1500)
	int			m_niCapBufferCase; 		//  6. (Error : 1600)
	int			m_niCapStageCase;		//  7. (Error : 1700)
	int			m_niTransStageCase;		//  8. (Error : 1800)
	int			m_niUnloadPickCase;		//  9. (Error : 1900)
	int			m_niUnloadStageCase;	// 10. (Error : 2000)

	CLoopTimer	m_tiMainInitLoop;		//  0. (Error : 1000)
	CLoopTimer	m_tiLoadPickLoop;		//  1. (Error : 1100)
	CLoopTimer	m_tiLoadStageLoop;		//  2. (Error : 1200)
	CLoopTimer	m_tiMainIndexLoop;		//  3. (Error : 1300)
	CLoopTimer	m_tiAssyPickLoop;		//  4. (Error : 1400)
	CLoopTimer	m_tiCapPickLoop;		//  5. (Error : 1500)
	CLoopTimer	m_tiCapBufferLoop;		//  6. (Error : 1600)
	CLoopTimer	m_tiCapStageLoop; 		//  7. (Error : 1700)
	CLoopTimer	m_tiTransStageLoop;		//  8. (Error : 1800)
	CLoopTimer	m_tiUnloadPickLoop;		//  9. (Error : 1900)
	CLoopTimer	m_tiUnloadStageLoop;	// 10. (Error : 2000)

	BOOL		m_bInitComplete;
	
protected:
	BOOL		m_bThreadInitial;
	CWinThread	*m_pThreadInitial;
	static UINT	Thread_Initial(LPVOID lpVoid);

private:
	BOOL Initial_MainInit();			//  0. (Error : 1000)
	BOOL Initial_LoadPicker();			//  1. (Error : 1100)
	BOOL Initial_LoadStage();			//  2. (Error : 1200)
	BOOL Initial_MainIndex();			//  3. (Error : 1300)
	BOOL Initial_AssyPicker();			//  4. (Error : 1400)
	BOOL Initial_CapPicker();			//  5. (Error : 1500)
	BOOL Initial_CapBuffer();			//  6. (Error : 1600)
	BOOL Initial_CapStage();			//  7. (Error : 1700)
	BOOL Initial_TransStage();			//  8. (Error : 1800)
	BOOL Initial_UnloadPicker();		//  9. (Error : 1900)
	BOOL Initial_UnloadStage();			// 10. (Error : 2000)

public:
	BOOL Get_ThreadInitial() { return m_bThreadInitial; }
	BOOL Get_InitComplete() { return m_bInitComplete; }
	void Set_InitComplete(BOOL bInitComp) { m_bInitComplete = bInitComp; }	
	
	int *Get_InitialCase();

	void Begin_InitialThread();
	void End_InitialThread();
	void Init_Replay();
	void Reset_InitialCase();

	BOOL Is_InitThreadRun() { return m_bThreadInitial; }
};

extern CSequenceInit g_objSequenceInit;

///////////////////////////////////////////////////////////////////////////////
