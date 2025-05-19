// DataManager.h : 헤더 파일
//
#pragma once

///////////////////////////////////////////////////////////////////////////////
// Equip Data
typedef struct tag_EQUIP_DATA {
	CString sEquipName;
	int		nLotBarcodePort;
	int		nAssyLoadCellPort;
	int		nUnloadLoadCellPort;
	BOOL	bJobListTaktLog;
	int		nScreenOff;
	int		nNoWorkTime;
	BOOL	bManualTaktTest;
	BOOL	bUseDoorLock;
	CString sRmsNetDrive;
	BOOL	bUseVisionCmAlign;
	BOOL	bUseVisionCapAlign;
	BOOL	bUseTrayPickerTurn;
	BOOL	bUseCapPickerTurn;
	BOOL	bUseAssyPickerVac;
	BOOL	bUseIndexAssyVac;
	BOOL	bUseAssyPickerTiltChk;
	BOOL	bUseVisionAlignAlarm;
	BOOL	bUseAlignOffset;
	BOOL	bUseInlineMode;
	BOOL	bUseTrayTurn;
	BOOL	bUseCapTurn;
	BOOL	bUseMes;
	BOOL	bSortDirRight;	// Load Tray 잔량 정리 방향. TRUE = 오른쪽으로 정렬, FALSE = 아래쪽으로 정렬
	BOOL	bCapPickUpMulti;	// Cap Pick Up Type 설정 : TRUE = 멀티 픽업, FALSE = 개별 픽업
	BOOL	bShipTrayOneEmpty;	// Ship Tray 마지막 한 포켓을 비우고 언로딩 한다. 25ea->24ea
	BOOL	bUseMesCapReg;
	BOOL	bUseMesShipReg;

	int		nLoadTrayX;
	int		nLoadTrayY;
	double	dLoadTrayPitchX;
	double	dLoadTrayPitchY;
	int		nCapTrayX;
	int		nCapTrayY;
	double	dCapTrayPitchX;
	double	dCapTrayPitchY;
	int		nUnloadTrayX;
	int		nUnloadTrayY;
	double	dUnloadTrayPitchX;
	double	dUnloadTrayPitchY;
	double	dIndexPitch;
	int		nVacOffDelay[4];	// 0:Btm1Picker,1:Btm2Picker,2:SortPicker1,3:SortPicker2
	int		nVacOffRepeat[4];	// 0:Btm1Picker,1:Btm2Picker,2:SortPicker1,3:SortPicker2
	int		nDelayAdd[6];		// 0:LoadPicker Grip, 1:CapPicker VacOn, 2:AssyPicker VacOn, 3:UnloadPicker VacOn, 4:LoadCell, 5:AssyPicker CapPress
	double	dAlignOffset;
	BOOL	bTower[6][4];		// [0:None,1:Init,2:Ready,3:Run,4:Alarm,5:Error][0:Green,1:Yellow,2:Red,3:Flicker]
	BOOL	bBuzzer[5][6];		// [0::Alarm,1:Error,2:LotEnd,3:CapTray,4:ShipTray][0:Bit0,1:Bit1,2:Bit2,3:Bit3,4:Bit4,5:Flicker]
	CString	sPasswordMt;
	CString	sPasswordSi;
	BOOL	bResultTestUse;		// Result Test Use
	int		nResultTestNNg;		// Result Test N-NG Ratio (0~100)
	int		nResultTestSNg;		// Result Test S-NG Ratio (0~100)
	int		nResultTestS2Ng;	// Result Test S-NG Ratio (0~100)
	CString	sAviIp;
	int		nCappingCnt;		// Load Cell 측정을 위해 Capping한 Cap 수량 저장
	int		nLoadCellChkCnt;	// Assy Picker Load Cell 측정 주기
	double	dAssyPickForce[2];	// 0: Assy Picker Force Min, 1: Assy Picker Force Max 
} EQUIP_DATA;

///////////////////////////////////////////////////////////////////////////////
// Move Data
typedef struct tag_MOVE_DATA {
	double  dTrayPickerX[3];		// AXIS 00	[2]
	double  dTrayPickerZ[3];		// AXIS 01	[3]
	double  dTrayPickerR[2];		// AXIS 02	[2]
	double  dLoadStage1X[4];		// AXIS 04	[4]
	double  dLoadStage1Z[5];		// AXIS 05	[5]
	double  dLoadStage2X[4];		// AXIS 06	[4]
	double  dLoadStage2Z[5];		// AXIS 07	[5]
	double  dLoadPickerY[3];		// AXIS 08	[3]
	double  dLoadPickerZ[4];		// AXIS 09	[4]
	double  dLoadPickerP[2];		// AXIS 10	[2]
	double  dIndexR[1];				// AXIS 11	[1]
	double  dVisionCMAlignX[2];		// AXIS 12	[2]
	double  dCapStage1X[3];			// AXIS 13	[3]
	double  dCapStage1Z[4];			// AXIS 14	[4]
	double  dCapStage2X[3];			// AXIS 15	[3]
	double  dCapStage2Z[4];			// AXIS 16	[4]
	double  dCapPickerY[13];		// AXIS 17	[3]
	double  dCapPickerZ[4];			// AXIS 18	[4]
	double  dCapPickerP[2];			// AXIS 19	[2]
	double  dCapBufferStageY[4];		// AXIS 20	[4]
	double  dVisionCapAlignY[1];	// AXIS 21	[1]
	double  dAssyPickerX[7];		// AXIS 22	[3]
	double  dAssyPickerY[7];		// AXIS 23	[3]
	double  dAssyPickerZ[7];		// AXIS 24	[5]
	double  dTransStageX[2];		// AXIS 25	[2]
	double  dTransStageZ[3];		// AXIS 26	[3]
	double  dTransStageT[2];		// AXIS 27	[2]
	double  dUnloadPickerX[4];		// AXIS 28	[4]
	double  dUnloadPickerZ[4];		// AXIS 29	[4]
	double  dUnloadPickerP[2];		// AXIS 30	[2]
	double  dUnloadStage1Y[3];		// AXIS 31	[3]
	double  dUnloadStage1Z[6];		// AXIS 32	[6]
	double  dUnloadStage2Y[3];		// AXIS 33	[3]
	double  dUnloadStage2Z[6];		// AXIS 34	[6]
} MOVE_DATA;

///////////////////////////////////////////////////////////////////////////////

class CDataManager
{
public:
	CDataManager();
	virtual ~CDataManager();

private:
	EQUIP_DATA	m_EquipData;
	MOVE_DATA	m_MoveData;

public:	
	void Reset_EquipData();
	void Reset_MoveData();

	BOOL Read_EquipData();
	BOOL Read_MoveData();
	BOOL Read_ModelData(int nType=0);

	EQUIP_DATA *Get_pEquipData() { return &m_EquipData; }
	MOVE_DATA *Get_pMoveData() { return &m_MoveData; }
};

extern CDataManager g_objDataManager;

///////////////////////////////////////////////////////////////////////////////
