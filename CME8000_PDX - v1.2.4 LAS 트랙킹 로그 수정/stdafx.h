// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.
#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 일부 CString 생성자는 명시적으로 선언됩니다.

// MFC의 공통 부분과 무시 가능한 경고 메시지에 대한 숨기기를 해제합니다.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소입니다.
#include <afxext.h>         // MFC 확장입니다.

#include <afxdisp.h>        // MFC 자동화 클래스입니다.

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 공용 컨트롤에 대한 MFC 지원입니다.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows 공용 컨트롤에 대한 MFC 지원입니다.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC의 리본 및 컨트롤 막대 지원


#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

///////////////////////////////////////////////////////////////////////////////

// Library Add
#include "CSControls.h"
#include "CSIniFile.h" 
#include "CSComPort.h"
#include "CSUdpSocket.h"
#include "CSClientSocket.h"
#include "CSGrid.h"
#ifdef _DEBUG
	#pragma comment(lib, "CSControlsD.lib")
	#pragma comment(lib, "CSIniFileD.lib")
	#pragma comment(lib, "CSComPortD.lib")
	#pragma comment(lib, "CSUdpSocketD.lib")
	#pragma comment(lib, "CSClientSocketD.lib")
	#pragma comment(lib, "CSGridD.lib")
#else
	#pragma comment(lib, "CSControlsR.lib")
	#pragma comment(lib, "CSIniFileR.lib")
	#pragma comment(lib, "CSComPortR.lib")
	#pragma comment(lib, "CSUdpSocketR.lib")
	#pragma comment(lib, "CSClientSocketR.lib")
	#pragma comment(lib, "CSGridR.lib")
#endif

#define MAIN_VERSION	_T("V 1.2.5p")
#define ECM_LOG			"D:\\EVMS\\TP\\log\\"
//
#define AJIN_BOARD_USE
#define LOT_BARCODE_USE
#define LOAD_CELL_USE

// 테스트 런 옵션
//#define DRY_RUN_TEST		// Dry Run Test 시 사용

///////////////////////////////////////////////////////////////////////////////

extern CString gsCurrentDir;	// 현재 프로젝트 폴더

typedef struct {
	CString	sLotID[2];
	CString	sOperID;				// Operator
	CString	sRecipeName;			// Recipe Item

	int		nTrayUseCount[2];		// 사용자 입력한 Tray 갯수
	int		nCmUseCount[2];			// 사용자 입력한 CM 갯수
	int		nCmMaxCount;			// 한 트레이의 최대 CM 갯수

	// Single ContinueLot 기능 사용할때
	CString	sLotsID[5];
	int		nTraysUseCnt[5];
	int		nCmsUseCnt[5];
	int		nContinueLotIndex;
	BOOL	bContinueLot;

	CString sCapLotID;
	int		nCapTrayUseCount;
	int		nCapUseCount;
	int		nCapMaxCount;
	int		nCapTrayCount;

	CString sShipLotID;
	int		nShipTrayUseCount;
	int		nShipUseCount;
	int		nShipMaxCount;
	int		nShipTrayCount;

	int		nLPNo;					// Load 쪽 Port No
	int		nLoadTrayCount[2];
	int		nUnloadTrayCount;
	int		nULPNo;					// Unload Port No
	int		nErrPNo;

	BOOL	IndexDone[3];			// 0:Load, 1:Assembly 2:Trans
	int		InfoLoadTray[20][20];	// Load Tray 정보 표시 (0:Not Use, 9:Use)
	int		InfoCapTray[20][20];		// Cap Tray 정보 표시 (0:Not Use, 9:Use)
	int		InfoUnloadTray[20][20];	// Unload Tray 정보 표시 (0:Not Use, 1:Exist)

	int		InfoIndex[3][6];		// Index 정보 표시 0: 좌측	(0:Not Use, 1:Exist)
	int		InfoCapBuffer[6];		// Cap Buffer 정보 표시 0:상단	(0:Not Use, 9:Use)
	int		InfoTransStage[6];		// Trans Stage 정보 표시 0:우측	(0:Not Use, 1:Exist)

	int		InfoLoadPick[6];		// 0:좌측
	int		InfoCapPick[6];			// 0:상단
	int		InfoAssyPick[6];		// 0:상단
	int		InfoUnloadPick[6];		// 0:우측부터 0 

	int		nCapTNoCapPicker[6];	// Cap Picker Cap Tray No.
	int		nCapTNoCapBuffer[6];	// Cap Buffer Cap Tray No.
	int		nCapTNoAssyPicker[6];	// Assy Picker Cap Tray No.

	int		nCapNoCapPicker[6];		// Cap Picker CapNo.
	int		nCapNoCapBuffer[6];		// Cap Buffer CapNo.
	int		nCapNoAssyPicker[6];	// Assy Picker CapNo.

	int		nPNoAviPort;			// Inline Mode 일때 Avi Port No.
	int		nTNoAviPort;			// Inline Mode 일때 Avi Port No.
	int		nPNoTrayPick;			//
	int		nPNoLoadPort;			//
	int		nPNoLoadTray[2];		// 0: stage1, 1: stage2, Load Tray Port No (1, 2)
	int		nPNoLoadPick;			// Load Picker Port No (1, 2)
	int		nPNoIndex[3];			// Index Port No (1, 2)
	int		nPNoTransStage;			// Trans Stage Port No (1, 2)
	int		nPNoUnloadPick;			// Unload Picker Port No (1, 2)
	int		nPNoUnloadTray;			// Unload Tray Port No (1, 2)
	int		nPNoUnloadPort;			// Unload Port Port No (1, 2)

	int		nTNoTrayPick;			// Tray Picker Tray No (1 ~ 25)
	int		nTNoLoadPort;			// Load Port Tray No (1 ~ 25)	//AVI 연동모드일때 쓴다.
	int		nTNoLoadTray[2];		// 0: stage1 , 1: stage2, Load Stage Tray No (1 ~ 25)
	int		nTNoLoadPick[6];		// Load Picker Tray No (1 ~ 25)
	int		nTNoIndex[3][6];		// Index Tray No (1 ~ 25)
	int		nTNoTransStage[6];		// Trans Stage Tray No (1 ~ 25)
	int		nTNoUnloadPick[6];		// Unload Picker Tray No (1 ~ 25)
	int		nTNoUnloadTray;			// Unload Stage Tray No(1 ~ 25)

	int		nCNoLoadPick[6];		// Load Picker CM No
	int		nCNoIndex[3][6];		// Index CM No
	int		nCNoTransStage[6];		// Trans Stage CM No
	int		nCNoUnloadPick[6];		// Load Index CM No R04A:(1 ~ 20), R04B:(1~15), R04C:(1~32)

	BOOL	bScanDone[2];			// 0:CM Align, 1:Cap Align
	int		nInspCmNo[2][2];		// 0:CM Align, 1:Cap Align, 0:CmNo1, 1:CmNo2	
	// Port 별로 저장해야함.
	int		nCmInspectInfo[2][50][40];		// CM Align 검사결과 (0:Empty, 1:Good, 2:Normal)
	int		nCapInspectInfo[2][100][40];	// Cap Align 검사결과 (0:Empty, 1:Good, 2:Normal)
	CString sNGData[2][100][40][2];		// NG 코드  (0:CM Align, 1:Cap Align)

	BOOL	bCycleStop;				// Run 중간에 멈출때 (초기화 필요)

	BOOL	bUnloadTrayLotEnd[2];
	BOOL	bLoadPort3Wait;			// Load Port3 배출 완료까지 대기
	BOOL	bCapPort1Wait;			// Cap Port1 투입 완료까지 대기
	BOOL	bCapPort2Wait;			// Cap Port2 배출 완료까지 대기
	BOOL	bUnloadPort1Wait;		// Unload Port1 투입 완료까지 대기
	BOOL	bUnloadPort2Wait;		// Unload Port2 배출 완료까지 대기

	BOOL	bLoadLampOn[2];			// Load Port Lamp 켜지는 시점을 Lot End되는 시점으로 바꿔준다.

	BOOL	bCmAlignErr[6];			// CM Align Inspection Error : TRUE
	BOOL	bCapAlignErr[6];		// CM Align Inspection Error : TRUE

	int		nTrayX;					// Tray X Count (R04A:4, R04B:5, R04C:8)
	int		nTrayY;					// Tray Y Count (R04A:5, R04B:3, R04C:4)
	int		nScanCnt;				// Scan Count (R04A:2, R04B:2, R04C:1)

	BOOL	bBuffTrayChange;		//
	BOOL	bContinueLotEnd;

	BOOL	bFirstLotStart;
	BOOL	bMesFirstLot;
	BOOL	bMesContinueLot;		// MES 사용중 연속랏사용할때.
	BOOL	bMesRegistered[2];		// Port 별로 MES 착공 했는지 안했는지 확인.	

	BOOL	bLotEndBeep;			// NG Full 알림음 때문에 LotEnd 알림음이 Skip 안되게 해준다.

	int		nCmInspPickNo1;
	int		nCmInspPickNo2;
	int		nCapInspPickNo1;
	int		nCapInspPickNo2;	

	BOOL	bCapCntClear;

	BOOL	bAviTrayLoad;
	BOOL	bAviLotEnd[2];
	BOOL	bCapLotEnd[2];
	int		nCmCntAviPort;
	int		nCmCntTrayPick;
	int		nCmCntLoadPort;
	int		nCmCntLoadStage[2];
	int		nLastTrayNo[2];

	// AVI에 보낸 Lot 정보의 Port번호가 아직 Cap 설비에서 작업 중일때 임시 정보 저장. 
	BOOL	bTempLot[2];	// 임시 저장한 Lot이 있으면 TRUE, LotEnd후 정보 옮겨준다.
	CString sTempLotID[2];
	int		nTempTrayUseCount[2];
	int		nTempCmUseCount[2];
	CString sTempRecipeName[2];

	BOOL	bLoadCellComplete;
	double	dLoadCell[2];
	double	dAssyLoadCell[6];
	double	dUnloadLoadCell[6];

	double	dAssyPickOffsetX[6];		// 0:상단
	double	dAssyPickOffsetY[6];		// 0:상단
	double	dAssyPickCapSizeX[6];		// 0:상단
	double	dAssyPickCapSizeY[6];		// 0:상단

	double	dIndexOffsetX[4][6];		// 0:상단
	double	dIndexOffsetY[4][6];		// 0:상단
	double	dIndexCmSizeX[4][6];		// 0:상단
	double	dIndexCmSizeY[4][6];		// 0:상단

	double	dAlignDiffX[6];				// 모듈옵셋 - 캡옵셋 = 이동량
	double	dAlignDiffY[6];				// 모듈옵셋 - 캡옵셋 = 이동량

	int		nPickerUseCnt;		// 픽커 사용 개수 (5ea or 6ea)

	BOOL	nCmAlignSkip;		// NG 조치가 안되면 Skip 해줄수 있게한다.
	BOOL	nCapAlignSkip;		// NG 조치가 안되면 Skip 해줄수 있게한다.

	BOOL	bCapAlignNg[6];		// 옵셋 보정한 Picker Check

	BOOL	bManualPassword;	// 매뉴얼 모드 들어갈때 비밀번호 확인하게 해준다.
	////////////////////////////////////////////////////////////////////////////////////////////
	// Cap LAS Log 관련
	CString sCIDCapStage[2];		// Cap Stage Cap ID
	CString sCIDCapPicker[6];
	CString sCIDCapBuffer[6];
	CString sCIDAssyPicker[6];
	CString sCIDIndex[3][6];		// 0: Load, 1: Assy, 2: Unload 사용하는건 1, 2 두개만 사용
	CString sCIDTransStage[6];
	CString sCIDUnloadPicker[6];

	CString sCapAttachStart[50][40];	// Cap Attach 작업 시작 : Tray, Cm 순으로 저장해준다.
	CString sCapAttachEnd[50][40];		// Cap Attach 작업 완료 : Tray, Cm 순으로 저장해준다.

	int		nINoTransStage;			// Index No
	int		nINoUnloadPicker;		// Index No

	int		nLNoUnloadPicker;
	DWORD	dwCmTactStart[50][10];	// Trans Stage PickUp
	////////////////////////////////////////////////////////////////////////////////////////////
	int		nCmJigNo[2][30][40][5];	// CM Jig 정보 저장 [Port][Tray][CM][Unit]
									// Unit 0:Load Stage, 1:Load Picker, 2:Index Load No, 3:Index Load Jig No, 4:Unload Picker

	DWORD	dwMStopStartTime;		// 작업자 기인 알람, Stop 버튼or스위치 눌러서 장비 세웠을때 
	DWORD	dwMStopEndTime;
	DWORD	dwMStopProcTime;

	CString sListName;
	CString sYYMMDD;
	CString sNGFileName;

	CString sAlarmTime[3];
	CString sAlarmList[3];

	CString	sComName;
	int		nDoorLockTime;
	DWORD	dwDoorStartTime;
} GLOVAL_DATA;

typedef struct {
	CString	sLotID[2];
	CString sStartTime[2];
	CString sEndTime[2];
	CString sMESStartTime[2];
	DWORD	dwLotStart[2];
	DWORD	dwLotEnd[2];
	int		nTrayCount[2];
	int		nCmCount[2];
	double  dTackTime;
	int		nGoodCount[2];
	int		nNgCount[2];
	int		nSNgCount[2][6];			// Special NG (MES, N1, N2)
	int		nRosRequest[2];
	int		nRosGood[2];
	int		nRosNg[2];
	int		nRosRepair[2];
	int		nRosTimeOut[2];
	int		nCapFailCount[2];			// Cap Tilt Error Count

	CString sCapLotID;
	CString sCapStartTime;
	CString sCapEndTime;
	DWORD	dwCapLotStart;
	DWORD	dwCapLotEnd;
	int		nCapCount[2];

	int		nErrorCount;
	DWORD	dwRunTime;
	DWORD	dwStopTime;
	DWORD	dwErrorTime;

	BOOL	bLotEndComplete[2];

	double	dAverageCycle[28][2][14];

	DWORD	dwTaktData[4];	// 0:Load, 1:Assembly, 2:Unload, 3:Index
	BOOL	bTaktDone[3];	// 0:Load, 1:Assembly, 2:Unload
} GLOVAL_LOT;

typedef struct {
	BOOL	bBegin;
	CString	sLotID;
	int		nAlmNo;
	CString sAlmMsg;
	CString sStartTime;
	CString sEndTime;
	DWORD	dwStartTime;
	DWORD	dwEndTime;
	DWORD	dwProcTime;

	double  dMotionChkPos;
	double  dMotionPos[35];

	int		nLDPickerNo;
	int		nCapPickeNo;
	int		nAssyPickNo;
	int		nUNPickerNo;
	int		nIndexVacNo;
} GLOVAL_ALM;

typedef struct {
	double  dTaktTime;
	double	dTakt[24][50];	// 시간당 각 Lot의 Takt (시간당 최대 50개)
	int		nLotCount[24];	// 시간당 Lot 수량 (0~23)
	int		nCmCount[2];	// 주간(0:7~19h), 야간(1:19~7h)
} GLOVAL_UPH;

typedef struct {
	int		nLotStatus;				// 0:Stop, 1:Start Send, 2:Start Receive
	DWORD	dwLotStartTime;			// Lot Start time
	int		nCmResult[50][40];		// 0:Not Receive, 1:Result Done, 2:Result Skip

	// Host 전공정 Data
	CString		sHostLotID;			//Host 수신 Lot ID
	CString		sHostRecipe;		//Host 수신 Recipe ID
	CString		sHostModel;			//Host 수신 Recipe ID
	int			nHostCount;			//Host 수신 CM 총갯수
	CString		sHostCancelCode;	//Host Cancel 수신 code
	CString		sHostCancelText;	//Host Cancel 수신 내용

	// 장비 실적 Data	// LJH Port별로 관리해야할지 정해야한다.
	CString		sBarID[2][50][40];		//Barcode ID(장비) [30]:Tray No, [42]:Pocket No
	CString		sJudge[2][50][40];		//N:NG, H:Special NG, M:성능불량(MES NG)
	CString		sNGCode[2][50][40];		//대표NG Code
} GLOVAL_MES;

typedef struct {
	int			nCapCount;
	CString		sCapPart[20];
	int			nShipCount;
	CString		sShipPart[20];
} GLOVAL_PART;

typedef struct {
	int		nType;			//nType:1[정시], 2[해제] 3[설정]
	int		nWriteHH;		//등록시간
	int		nOpenStart;		//1:Start
	DWORD	dwOpenStartTime;
	int		nOpenTime;
} GLOVAL_INTERLOCKDATA;

extern GLOVAL_DATA	gData;
extern GLOVAL_LOT	gLot;
extern GLOVAL_ALM	gAlm;
extern GLOVAL_UPH	gUph;
extern GLOVAL_MES	gMes;
extern GLOVAL_PART	gPart;
extern GLOVAL_INTERLOCKDATA	gIt;
