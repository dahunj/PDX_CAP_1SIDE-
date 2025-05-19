// Common.h : 헤더 파일
//
#pragma once

#define AUTO_COUNT			16

#define AUTO_TRAY_PICKER	0
#define AUTO_LOAD_STAGE1	1
#define AUTO_LOAD_STAGE2	2
#define AUTO_LOAD_PICKER	3
#define AUTO_MAIN_INDEX		4
#define AUTO_VISION_CM		5
#define AUTO_VISION_CAP		6
#define AUTO_CAP_STAGE1		7
#define AUTO_CAP_STAGE2		8
#define AUTO_CAP_PICKER		9
#define AUTO_CAP_BUFFER		10
#define AUTO_ASSY_PICKER	11
#define AUTO_TRANS_STAGE	12
#define AUTO_UNLOAD_PICKER	13
#define AUTO_UNLOAD_STAGE1	14
#define AUTO_UNLOAD_STAGE2	15

#define INFO_STAGE		0
#define INFO_PICK		1

#define R04A	0
#define R04B	1
#define R04C	2

#define ASSY_LOAD_CELL		1
#define UNLOAD_LOAD_CELL	2

#define ERROR_CODE_BASE		10000		// Cap Attach Error Code 변경 10000 ~ 19999

// Module Tracking
#define LOAD_STAGE		0	// Stage No 저장. 1 or 2
#define LOAD_PICK		1	// Picker Jig 번호 저장. 1~6
#define INDEX_LOAD_NO	2	// Index Loading 할때 번호 저장. 1~8
#define INDEX_LOAD_JIG	3	// Index Load Jig 번호 저장. 1~5
#define UNLOAD_PICK		4	// Picker Jig 번호 저장. 1~6

const double GOOD_TRAY_WIDTH = 360.0;	// Good Tray 1/2 간섭없는 간격

// CCommon

class CCommon : public CWnd
{
	DECLARE_DYNAMIC(CCommon)

public:
	CCommon();
	virtual ~CCommon();

protected:
	DECLARE_MESSAGE_MAP()

private:
	LONGLONG	m_nFreq;		// Frequence

public:
	void DoEvents();
	void uSleep(int msec);
	void MakeFolder(CString sPath);
	void Save_MotionPos();
	int	 Check_MotionPos();

	BOOL Check_Position(int nAxis, int nMoveIdx, double dRange = 0.1);
	void Move_Position(int nAxis, int nMoveIdx);

	void Show_Error(int nErrNo);
	void Set_ErrorSubMessage(CString sErrSubMsg);
	void Show_Alarm(CString sMsg, int nState=4, BOOL bOn=TRUE);
	int Show_MsgBox(int nBtnCnt, CString sMsg);
	int Show_NumPad(CString sOld, CString &sNew, int nAxis = -1);
	int Show_KeyPad(CString &sKey);
	int Show_Password(int &nUser);

	BOOL Check_MainEmgAir();
	BOOL Check_MainDoor(BOOL bAuto=FALSE);
	BOOL Check_PortArea(BOOL bAuto=FALSE);
	BOOL Check_TrayFull();

	BOOL Check_ServoOn();
	BOOL Check_DirveAlarm();
	BOOL Check_EndLimit();
	BOOL Check_HomeDone();

 	void Locking_MainDoor(BOOL bLock, BOOL bAuto=FALSE);
 	void Locking_PortSlide(BOOL bLock, int nPart=0);

	///////////////////////////////////////////////////////////////////////////
	void Get_RAMSize(CString &strRAM);
	void Get_CPUInfo(CString &strCPU);
	void Get_HardInfo(CString &strHardDisk);


	int Get_Random(int nStart, int nEnd);
	void Backup_File(CString strPath, CString strName);	// System Folder

	///////////////////////////////////////////////////////////////////////////
//	1. Tray Picker I/O 함수
	void Set_TrayPickerGripClose();
	BOOL Get_TrayPickerGripClose();
	void Set_TrayPickerGripOpen();
	BOOL Get_TrayPickerGripOpen();

// 	2. Load Port I/O 함수
	void Set_LoadPortSupportIn(int nNo);
	BOOL Get_LoadPortSupportIn(int nNo);
	void Set_LoadPortSupportOut(int nNo);
	BOOL Get_LoadPortSupportOut(int nNo);

// 	3. Load Picker I/O 함수
	void Set_LoadPickerUp();
	BOOL Get_LoadPickerUp();
	void Set_LoadPickerDown(int nNo);
	BOOL Get_LoadPickerDown(int nNo);
	void Set_LoadPickerOpen();
	BOOL Get_LoadPickerOpen();
	BOOL Get_LoadPickerCmCheckOff();
	void Set_LoadPickerDownMulti(int nNo, int nCnt);
	BOOL Get_LoadPickerDownMulti(int nNo, int nCnt);

// 4. Index Table I/O 함수
	BOOL Get_IndexLoadVacuumOff(int nNo);
	void Set_IndexLoadVacuumOff(int nNo);
	BOOL Get_IndexAssyVacuumOff(int nNo);
	void Set_IndexAssyVacuumOff(int nNo);
	int	 Get_IndexPos(int nPos);

// 	5. Cap Port I/O 함수
	void Set_CapPortSupportIn(int nNo);
	BOOL Get_CapPortSupportIn(int nNo);
	void Set_CapPortSupportOut(int nNo);
	BOOL Get_CapPortSupportOut(int nNo);

// 	6. Cap Picker I/O 함수
	void Set_CapPickerUp(int nNo);
	BOOL Get_CapPickerUp(int nNo);
	void Set_CapPickerNormal(int nNo);
	BOOL Get_CapPickerNormal(int nNo);
	void Set_CapPickerTurn(int nNo);
	BOOL Get_CapPickerTurn(int nNo);
	void Set_CapPickerVacOff(int nNo);
	BOOL Get_CapPickerVacOff(int nNo);
	void Set_CapPickerAirOff(int nNo);

	void Set_CapPickerDownMulti(int nNo, int nCnt);
	BOOL Get_CapPickerDownMulti(int nNo, int nCnt);
	void Set_CapPickerVacOnMulti(int nNo, int nCnt);
	BOOL Get_CapPickerVacOnMulti(int nNo, int nCnt);
	void Set_CapPickerVacOffMulti(int nNo, int nCnt);
	BOOL Get_CapPickerVacOffMulti(int nNo, int nCnt);
	void Set_CapPickerAirOffMulti(int nNo, int nCnt);

//	7. Cap Buffer I/O 함수
	void Set_CapBufferAlign(BOOL bOn);
	BOOL Get_CapBufferAlign(BOOL bOn);
	void Set_CapCleanAirOn();
	void Set_CapCleanAirOff();

// 	8. Assembly Picker I/O 함수
	void Set_AssyPickerUp(int nNo);
	BOOL Get_AssyPickerUp(int nNo);
	void Set_AssyPickerDown(int nNo);
	BOOL Get_AssyPickerDown(int nNo);
	void Set_AssyPickerVacOff(int nNo);
	BOOL Get_AssyPickerVacOff(int nNo);
	void Set_AssyPickerAirOff(int nNo);

// 	9. Trans Stage I/O 함수
	void Set_TransStageVacOff(int nNo);
	BOOL Get_TransStageVacOff(int nNo);
	void Set_TransStageAirOff(int nNo);
	void Set_TransStageClampOn();
	BOOL Get_TransStageClampOn();
	void Set_TransStageClampOff();
	BOOL Get_TransStageClampOff();

//	10. Unload Picker I/O 함수
	void Set_UnloadPickerUp(int nNo);
	BOOL Get_UnloadPickerUp(int nNo);
	void Set_UnloadPickerVacOff(int nNo);
	BOOL Get_UnloadPickerVacOff(int nNo);
	void Set_UnloadPickerAirOff(int nNo);

	void Set_UnloadPickerDownMulti(int nNo, int nCnt);
	BOOL Get_UnloadPickerDownMulti(int nNo, int nCnt);
	void Set_UnloadPickerVacOffMulti(int nNo, int nCnt);
	BOOL Get_UnloadPickerVacOffMulti(int nNo, int nCnt);
	void Set_UnloadPickerAirOffMulti(int nNo, int nCnt);

// 	11. Unload Port I/O 함수
	void Set_UnloadPortSupportIn(int nNo);
	BOOL Get_UnloadPortSupportIn(int nNo);
	void Set_UnloadPortSupportOut(int nNo);
	BOOL Get_UnloadPortSupportOut(int nNo);

//  12. Tray I/O 함수
	BOOL Get_LoadTrayMasterSlaveIn(int nNo);
	BOOL Get_LoadTrayMasterSlaveOut(int nNo);
	BOOL Get_CapTrayMasterSlaveIn(int nNo);
	BOOL Get_CapTrayMasterSlaveOut(int nNo);
	BOOL Get_UnloadTrayMasterSlaveIn(int nNo);
	BOOL Get_UnloadTrayMasterSlaveOut(int nNo);

///////////////////////////////////////////////////////////////////////////
	// 1. Info Btm1 Picker I/O 함수
	void Set_InfoLoadPickerDown(int nPos, int nLine=0);
	BOOL Get_InfoLoadPickerDown(int nPos, int nLine=0);
	void Set_InfoLoadPickerGrip(int nPos, int nLine=0);
	BOOL Get_InfoLoadPickerGrip(int nPos, int nLine=0);
	BOOL Get_InfoLoadPickerGripOpen(int nPos, int nLine=0);
	BOOL Get_InfoLoadPickerCmCheck();
	int  Get_InfoLoadPickerDownCnt(int nTrayY, int nTrayX, int nDownSu);

	// 2. Info Main Index I/O 함수
	BOOL Get_InfoIndexLoadVacuumOn(int nPos=0);
	void Set_InfoIndexLoadVacuumOn(int nPos=0);
	BOOL Get_InfoIndexAssyVacuumOn();
	void Set_InfoIndexAssyVacuumOn();

	// 3. Info Cap Picker I/O 함수
	void Set_InfoCapPickerDown(int nPos);
	BOOL Get_InfoCapPickerDown(int nPos);
	BOOL Get_InfoCapPickerVacOn(int nPos);
	void Set_InfoCapPickerVacOff(int nPos);
	BOOL Get_InfoCapPickerVacOff(int nPos);
	void Set_InfoCapPickerAirOff(int nPos);
	void Set_InfoCapPickerNormal();
	BOOL Get_InfoCapPickerNormal();
// 
	// 4. Info Assy Picker I/O 함수
	void Set_InfoAssyPickerDown(int nPos);
	BOOL Get_InfoAssyPickerDown(int nPos);
	void Set_InfoAssyPickerGoodDown();
	BOOL Get_InfoAssyPickerGoodDown();
	void Set_InfoAssyPickerVacOn(int nPos);
	BOOL Get_InfoAssyPickerVacOn(int nPos);
	void Set_InfoAssyPickerVacOff(int nPos);
	BOOL Get_InfoAssyPickerVacOff(int nPos);
	void Set_InfoAssyPickerAirOff(int nPos);
	void Set_InfoAssyPickerGoodVacOff();
	BOOL Get_InfoAssyPickerGoodVacOff();
	void Set_InfoAssyPickerGoodAirOff();

	// 5. Info Trans Stage I/O 함수
	void Set_InfoTransStageVacOn(int nPos=0);
	BOOL Get_InfoTransStageVacOn(int nPos=0);

	// 6. Info Unload Picker I/O 함수
	void Set_InfoUnloadPickerDown(int nPos);
	BOOL Get_InfoUnloadPickerDown(int nPos);
	void Set_InfoUnloadPickerVacOn(int nPos);
	BOOL Get_InfoUnloadPickerVacOn(int nPos);
	BOOL Get_InfoUnloadPickerVacOff(int nPos);
	BOOL Get_InfoUnloadPickerCapCheck();

	// 7. Info Cap Clean I/O 함수
	void Set_InfoCapCleanAirOn();

///////////////////////////////////////////////////////////////////////////
// Load Tray Info
	int *Get_LoadTrayInfo(int nLine);

///////////////////////////////////////////////////////////////////////////
	CString Get_AssyPickerTiltErr();
	CString Get_AssyPickerForceErr();
	CString Get_LoadPickerCheckErr();

	void Save_CapShiipData(int nType);	//1:Cap, 2:Ship
///////////////////////////////////////////////////////////////////////////
	void Read_CapShipPoatNo();
	BOOL Check_CapShipPoatNo(int nType, CString sPartNo);	//1:Cap, 2:Ship
};

extern CCommon g_objCommon;

///////////////////////////////////////////////////////////////////////////////
