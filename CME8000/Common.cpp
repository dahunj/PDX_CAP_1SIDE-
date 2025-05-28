// Common.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CME8000.h"
#include "Common.h"

#include "LogFile.h"
#include "AJinAXL.h"
#include "DataManager.h"

#include "ErrorDlg.h"
#include "AlarmDlg.h"
#include "MsgBoxDlg.h"
#include "NumPadDlg.h"
#include "KeyPadDlg.h"
#include "PasswordDlg.h"
#include "WorkDlg.h"

#include "SequenceMain.h"

#include <iostream>
#include <string>
#include <fstream>
#include "Wininet.h"

#include <wbemidl.h>
#include <comdef.h>

#pragma comment(lib, "wbemuuid.lib")

// CCommon
CCommon g_objCommon;

IMPLEMENT_DYNAMIC(CCommon, CWnd)

CCriticalSection g_csShowError;
CCriticalSection g_csShowAlarm;
CCriticalSection g_csIndexVacuum;

CCommon::CCommon()
{
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	m_nFreq = freq.QuadPart;
}

CCommon::~CCommon()
{
}

BEGIN_MESSAGE_MAP(CCommon, CWnd)
END_MESSAGE_MAP()

// CCommon 메시지 처리기입니다.

void CCommon::DoEvents()
{
	MSG msg;
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void CCommon::uSleep(int msec)
{
	LARGE_INTEGER tStart, tNow;
	LONGLONG lTerm = 0;
	QueryPerformanceCounter(&tStart);

	while (TRUE) {
		QueryPerformanceCounter(&tNow);
		lTerm = (tNow.QuadPart - tStart.QuadPart) * 1000 / m_nFreq;
		if (lTerm > msec) break;
		DoEvents();
	}
}

void CCommon::MakeFolder(CString sPath)	//"D:\\Run\\Log\\LotData\\YYYY\\MM\\DD\\";
{
	CString sTemp;
	int nIdx = 0;

	while(true) {
		nIdx = sPath.Find("\\");
		sTemp += sPath.Mid(0, nIdx) + "\\";
		CreateDirectory(sTemp,FALSE);
		sPath = sPath.Mid(nIdx+1, sPath.GetLength());
		if(nIdx<0) break;
	}
}

void CCommon::Save_MotionPos()
{
	int nCount = 0;
	for(int i=0; i<AXIS_COUNT; i++) {
		if (i == 3 || i == 11) continue;	// No Axis, Index R Axis
		if(g_objAJinAXL.Is_Done(i)) { gAlm.dMotionPos[i] = g_objAJinAXL.Get_Position(i); }
		else						{ gAlm.dMotionPos[i] = -100.0; nCount++; }
	}
	if (nCount == 0) return;

	uSleep(1000);
	nCount = 0;
	for(int i=0; i<AXIS_COUNT; i++) {
		if (i == 3 || i == 11) continue;	// No Axis, Index R Axis
		if (gAlm.dMotionPos[i] < -10.0) {
			if(g_objAJinAXL.Is_Done(i)) gAlm.dMotionPos[i] = g_objAJinAXL.Get_Position(i);
			else nCount++;
		}
	}
	if (nCount == 0) return;

	uSleep(3000);
	for(int i=0; i<AXIS_COUNT; i++) {
		if (i == 3 || i == 11) continue;	// No Axis, Index R Axis
		if (gAlm.dMotionPos[i] < -10.0) {
			if(g_objAJinAXL.Is_Done(i)) gAlm.dMotionPos[i] = g_objAJinAXL.Get_Position(i);
		}
	}
}

int CCommon::Check_MotionPos()
{
	int		nMotionNo = 99;
	double	dCurrentPos, dCheckPos;
	double	dRange = gAlm.dMotionChkPos;
	if (dRange < 0.05) return nMotionNo;

	for(int i=0; i<AXIS_COUNT; i++) {
		if (i == 3 || i == 11) continue;	// No Axis, Index R Axis
		if (gAlm.dMotionPos[i] < -10.0) continue;
		if (!g_objAJinAXL.Is_Done(i)) continue;

		dCheckPos   = gAlm.dMotionPos[i];
		dCurrentPos = g_objAJinAXL.Get_Position(i);
		if (fabs(dCurrentPos - dCheckPos) > dRange) return i;
	}
	return nMotionNo;
}

BOOL CCommon::Check_Position(int nAxis, int nMoveIdx, double dRange)
{
	double dCurrentPos = g_objAJinAXL.Get_Position(nAxis);
	MOVE_DATA *pMoveData = g_objDataManager.Get_pMoveData();

	double dCheckPos = -1.0;

	if (g_objAJinAXL.Is_Done(nAxis)) {
		if (nAxis == AX_TRAY_PICKER_X)		dCheckPos = pMoveData->dTrayPickerX[nMoveIdx];		// AXIS 00
		if (nAxis == AX_TRAY_PICKER_Z)		dCheckPos = pMoveData->dTrayPickerZ[nMoveIdx];		// AXIS 01
		if (nAxis == AX_TRAY_PICKER_R)		dCheckPos = pMoveData->dTrayPickerR[nMoveIdx];		// AXIS 02
		if (nAxis == AX_LOAD_STAGE1_X)		dCheckPos = pMoveData->dLoadStage1X[nMoveIdx];		// AXIS 04
		if (nAxis == AX_LOAD_STAGE1_Z)		dCheckPos = pMoveData->dLoadStage1Z[nMoveIdx];		// AXIS 05
		if (nAxis == AX_LOAD_STAGE2_X)		dCheckPos = pMoveData->dLoadStage2X[nMoveIdx];		// AXIS 06
		if (nAxis == AX_LOAD_STAGE2_Z)		dCheckPos = pMoveData->dLoadStage2Z[nMoveIdx];		// AXIS 07
		if (nAxis == AX_LOAD_PICKER_Y)		dCheckPos = pMoveData->dLoadPickerY[nMoveIdx];		// AXIS 08
		if (nAxis == AX_LOAD_PICKER_Z)		dCheckPos = pMoveData->dLoadPickerZ[nMoveIdx];		// AXIS 09
		if (nAxis == AX_LOAD_PICKER_P)		dCheckPos = pMoveData->dLoadPickerP[nMoveIdx];		// AXIS 00
		if (nAxis == AX_INDEX_R)			dCheckPos = pMoveData->dIndexR[nMoveIdx];			// AXIS 11
		if (nAxis == AX_VISION_CM_ALIGN_X)	dCheckPos = pMoveData->dVisionCMAlignX[nMoveIdx];	// AXIS 12
		if (nAxis == AX_CAP_STAGE1_X)		dCheckPos = pMoveData->dCapStage1X[nMoveIdx];		// AXIS 13
		if (nAxis == AX_CAP_STAGE1_Z)		dCheckPos = pMoveData->dCapStage1Z[nMoveIdx];		// AXIS 14
		if (nAxis == AX_CAP_STAGE2_X)		dCheckPos = pMoveData->dCapStage2X[nMoveIdx];		// AXIS 15
		if (nAxis == AX_CAP_STAGE2_Z)		dCheckPos = pMoveData->dCapStage2Z[nMoveIdx];		// AXIS 16
		if (nAxis == AX_CAP_PICKER_Y)		dCheckPos = pMoveData->dCapPickerY[nMoveIdx];		// AXIS 17
		if (nAxis == AX_CAP_PICKER_Z)		dCheckPos = pMoveData->dCapPickerZ[nMoveIdx];		// AXIS 18
		if (nAxis == AX_CAP_PICKER_P)		dCheckPos = pMoveData->dCapPickerP[nMoveIdx];		// AXIS 19
		if (nAxis == AX_CAP_BUFFER_STAGE_Y)	dCheckPos = pMoveData->dCapBufferStageY[nMoveIdx];	// AXIS 10
		if (nAxis == AX_VISION_CAP_ALIGN_Y)	dCheckPos = pMoveData->dVisionCapAlignY[nMoveIdx];	// AXIS 21
		if (nAxis == AX_ASSY_PICKER_X)		dCheckPos = pMoveData->dAssyPickerX[nMoveIdx];		// AXIS 22
		if (nAxis == AX_ASSY_PICKER_Y)		dCheckPos = pMoveData->dAssyPickerY[nMoveIdx];		// AXIS 23
		if (nAxis == AX_ASSY_PICKER_Z)		dCheckPos = pMoveData->dAssyPickerZ[nMoveIdx];		// AXIS 24
		if (nAxis == AX_TRANS_STAGE_X)		dCheckPos = pMoveData->dTransStageX[nMoveIdx];		// AXIS 25
		if (nAxis == AX_TRANS_STAGE_Z)		dCheckPos = pMoveData->dTransStageZ[nMoveIdx];		// AXIS 26
		if (nAxis == AX_TRANS_STAGE_T)		dCheckPos = pMoveData->dTransStageT[nMoveIdx];		// AXIS 27
		if (nAxis == AX_UNLOAD_PICKER_X)	dCheckPos = pMoveData->dUnloadPickerX[nMoveIdx];	// AXIS 28
		if (nAxis == AX_UNLOAD_PICKER_Z)	dCheckPos = pMoveData->dUnloadPickerZ[nMoveIdx];	// AXIS 29
		if (nAxis == AX_UNLOAD_PICKER_P)	dCheckPos = pMoveData->dUnloadPickerP[nMoveIdx];	// AXIS 20
		if (nAxis == AX_UNLOAD_STAGE1_Y)	dCheckPos = pMoveData->dUnloadStage1Y[nMoveIdx];	// AXIS 31
		if (nAxis == AX_UNLOAD_STAGE1_Z)	dCheckPos = pMoveData->dUnloadStage1Z[nMoveIdx];	// AXIS 32
		if (nAxis == AX_UNLOAD_STAGE2_Y)	dCheckPos = pMoveData->dUnloadStage2Y[nMoveIdx];	// AXIS 33
		if (nAxis == AX_UNLOAD_STAGE2_Z)	dCheckPos = pMoveData->dUnloadStage2Z[nMoveIdx];	// AXIS 34

		if (fabs(dCurrentPos - dCheckPos) < dRange) return TRUE;

	} else return FALSE;

	return FALSE;
}

void CCommon::Move_Position(int nAxis, int nMoveIdx)
{
	MOVE_DATA *pMoveData = g_objDataManager.Get_pMoveData();

	double dMovePos = -999.0;

	if (nAxis == AX_TRAY_PICKER_X)			dMovePos = pMoveData->dTrayPickerX[nMoveIdx];		// AXIS 00
	if (nAxis == AX_TRAY_PICKER_Z)			dMovePos = pMoveData->dTrayPickerZ[nMoveIdx];		// AXIS 01
	if (nAxis == AX_TRAY_PICKER_R)			dMovePos = pMoveData->dTrayPickerR[nMoveIdx];		// AXIS 02
	if (nAxis == AX_LOAD_STAGE1_X)			dMovePos = pMoveData->dLoadStage1X[nMoveIdx];		// AXIS 04
	if (nAxis == AX_LOAD_STAGE1_Z)			dMovePos = pMoveData->dLoadStage1Z[nMoveIdx];		// AXIS 05
	if (nAxis == AX_LOAD_STAGE2_X)			dMovePos = pMoveData->dLoadStage2X[nMoveIdx];		// AXIS 06
	if (nAxis == AX_LOAD_STAGE2_Z)			dMovePos = pMoveData->dLoadStage2Z[nMoveIdx];		// AXIS 07
	if (nAxis == AX_LOAD_PICKER_Y)			dMovePos = pMoveData->dLoadPickerY[nMoveIdx];		// AXIS 08
	if (nAxis == AX_LOAD_PICKER_Z)			dMovePos = pMoveData->dLoadPickerZ[nMoveIdx];		// AXIS 09
	if (nAxis == AX_LOAD_PICKER_P)			dMovePos = pMoveData->dLoadPickerP[nMoveIdx];		// AXIS 00
	if (nAxis == AX_INDEX_R)				dMovePos = pMoveData->dIndexR[nMoveIdx];			// AXIS 11
	if (nAxis == AX_VISION_CM_ALIGN_X)		dMovePos = pMoveData->dVisionCMAlignX[nMoveIdx];	// AXIS 12
	if (nAxis == AX_CAP_STAGE1_X)			dMovePos = pMoveData->dCapStage1X[nMoveIdx];		// AXIS 13
	if (nAxis == AX_CAP_STAGE1_Z)			dMovePos = pMoveData->dCapStage1Z[nMoveIdx];		// AXIS 14
	if (nAxis == AX_CAP_STAGE2_X)			dMovePos = pMoveData->dCapStage2X[nMoveIdx];		// AXIS 15
	if (nAxis == AX_CAP_STAGE2_Z)			dMovePos = pMoveData->dCapStage2Z[nMoveIdx];		// AXIS 16
	if (nAxis == AX_CAP_PICKER_Y)			dMovePos = pMoveData->dCapPickerY[nMoveIdx];		// AXIS 17
	if (nAxis == AX_CAP_PICKER_Z)			dMovePos = pMoveData->dCapPickerZ[nMoveIdx];		// AXIS 18
	if (nAxis == AX_CAP_PICKER_P)			dMovePos = pMoveData->dCapPickerP[nMoveIdx];		// AXIS 19
	if (nAxis == AX_CAP_BUFFER_STAGE_Y)		dMovePos = pMoveData->dCapBufferStageY[nMoveIdx];	// AXIS 10
	if (nAxis == AX_VISION_CAP_ALIGN_Y)		dMovePos = pMoveData->dVisionCapAlignY[nMoveIdx];	// AXIS 21
	if (nAxis == AX_ASSY_PICKER_X)			dMovePos = pMoveData->dAssyPickerX[nMoveIdx];		// AXIS 22
	if (nAxis == AX_ASSY_PICKER_Y)			dMovePos = pMoveData->dAssyPickerY[nMoveIdx];		// AXIS 23
	if (nAxis == AX_ASSY_PICKER_Z)			dMovePos = pMoveData->dAssyPickerZ[nMoveIdx];		// AXIS 24
	if (nAxis == AX_TRANS_STAGE_X)			dMovePos = pMoveData->dTransStageX[nMoveIdx];		// AXIS 25
	if (nAxis == AX_TRANS_STAGE_Z)			dMovePos = pMoveData->dTransStageZ[nMoveIdx];		// AXIS 26
	if (nAxis == AX_TRANS_STAGE_T)			dMovePos = pMoveData->dTransStageT[nMoveIdx];		// AXIS 27
	if (nAxis == AX_UNLOAD_PICKER_X)		dMovePos = pMoveData->dUnloadPickerX[nMoveIdx];		// AXIS 28
	if (nAxis == AX_UNLOAD_PICKER_Z)		dMovePos = pMoveData->dUnloadPickerZ[nMoveIdx];		// AXIS 29
	if (nAxis == AX_UNLOAD_PICKER_P)		dMovePos = pMoveData->dUnloadPickerP[nMoveIdx];		// AXIS 20
	if (nAxis == AX_UNLOAD_STAGE1_Y)		dMovePos = pMoveData->dUnloadStage1Y[nMoveIdx];		// AXIS 31
	if (nAxis == AX_UNLOAD_STAGE1_Z)		dMovePos = pMoveData->dUnloadStage1Z[nMoveIdx];		// AXIS 32
	if (nAxis == AX_UNLOAD_STAGE2_Y)		dMovePos = pMoveData->dUnloadStage2Y[nMoveIdx];		// AXIS 33
	if (nAxis == AX_UNLOAD_STAGE2_Z)		dMovePos = pMoveData->dUnloadStage2Z[nMoveIdx];		// AXIS 34

	if (dMovePos == -999) return;
	g_objAJinAXL.Move_Absolute(nAxis, dMovePos);
}

void CCommon::Show_Error(int nErrNo)
{
	g_csShowError.Lock();
	if (!g_dlgError.IsWindowVisible() && !g_dlgAlarm.IsWindowVisible()) {
		g_dlgError.Set_ErrorNumber(nErrNo);
		g_dlgError.ShowWindow(SW_SHOW);
	}
	g_csShowError.Unlock();
}

void CCommon::Set_ErrorSubMessage(CString sErrSubMsg)
{
	g_dlgError.Set_ErrorSubMessage(sErrSubMsg);
}

void CCommon::Show_Alarm(CString sMsg, int nState, BOOL bOn)
{
	g_csShowAlarm.Lock();
	if (bOn) {
		if (!g_dlgError.IsWindowVisible() && !g_dlgAlarm.IsWindowVisible()) {
			g_dlgWork.MachineStopLog("ALARM", sMsg);
			g_dlgAlarm.Set_AlarmMessage(sMsg);
			g_dlgAlarm.Set_AlarmState(nState);
			g_dlgAlarm.ShowWindow(SW_SHOW);
		}
	} else {
		if (!g_dlgError.IsWindowVisible() && g_dlgAlarm.IsWindowVisible()) {
			g_dlgAlarm.ShowWindow(SW_HIDE);
		}
	}
	g_csShowAlarm.Unlock();
}

int CCommon::Show_MsgBox(int nBtnCnt, CString sMsg)
{
	CMsgBoxDlg dlgMsgBox;
	dlgMsgBox.Set_Message(nBtnCnt, sMsg);
	return dlgMsgBox.DoModal();
}

int CCommon::Show_NumPad(CString sOld, CString &sNew, int nAxis)
{
	CNumPadDlg dlgNumPad;
	dlgNumPad.Set_OldValue(sOld, nAxis);
	if (dlgNumPad.DoModal() != IDOK) return IDCANCEL;

	sNew = g_strReturnNumPad;
	return IDOK;
}

int CCommon::Show_KeyPad(CString &sKey)
{
	CKeyPadDlg dlgKeyPad;
	if (dlgKeyPad.DoModal() != IDOK) return IDCANCEL;

	sKey = g_strReturnKeyPad;
	return IDOK;
}

int CCommon::Show_Password(int &nUser)
{
	CPasswordDlg dlgPassword;
	if (dlgPassword.DoModal() != IDOK) return IDCANCEL;

	nUser = g_nLoginUser;
	return IDOK;
}

BOOL CCommon::Check_MainEmgAir()
{
#ifdef AJIN_BOARD_USE
	DX_DATA_12 *pDX12 = g_objAJinAXL.Get_pDX12();
	if (pDX12->iEmgSw1)		{ Show_Error(1); return FALSE; }
	if (pDX12->iEmgSw2)		{ Show_Error(2); return FALSE; }
	if (pDX12->iEmgSw3)		{ Show_Error(3); return FALSE; }
	if (!pDX12->iMainAir1)	{ Show_Error(7); return FALSE; }
	if (!pDX12->iMainAir2)	{ Show_Error(8); return FALSE; }
#endif
	return TRUE;
}

BOOL CCommon::Check_MainDoor(BOOL bAuto)
{
#ifdef DRY_RUN_TEST
	return TRUE;
#endif
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

#ifdef AJIN_BOARD_USE
	DX_DATA_13 *pDX13 = g_objAJinAXL.Get_pDX13();
	BOOL bLoad1 = g_objSequenceMain.Get_LotLoadEnable(0);
	BOOL bLoad2 = g_objSequenceMain.Get_LotLoadEnable(1);


// 	if ((!bAuto || !bLoad) && pDX13->iDoor04Unlock) { Show_Alarm("Main 04번 Door(X1019) Unlocked."); return FALSE; }

	if (pEquipData->bUseInlineMode && !pEquipData->bUseDoorLock) {
		// Inline 모드는 도어락 사용 안해도 무조건 Load Port쪽 확인한다.
		if (pDX13->iDoor01Unlock) { Show_Alarm("Main 01번 Door(X1300) Unlocked."); return FALSE; }
		if (pDX13->iDoor02Unlock) { Show_Alarm("Main 02번 Door(X1301) Unlocked."); return FALSE; }

	} else {
		if (!pEquipData->bUseDoorLock) return TRUE;

		// temp
		if (pDX13->iDoor01Unlock && !bLoad1)					{ Show_Alarm("Main 01번 Door(X1300) Unlocked."); return FALSE; }
		if (pDX13->iDoor02Unlock && !bLoad2)					{ Show_Alarm("Main 02번 Door(X1301) Unlocked."); return FALSE; }
		if (pDX13->iDoor03Unlock)								{ Show_Alarm("Main 03번 Door(X1302) Unlocked."); return FALSE; }
		if (pDX13->iDoor04Unlock && !gData.bLoadPort3Wait)		{ Show_Alarm("Main 04번 Door(X1303) Unlocked."); return FALSE; }
		if (pDX13->iDoor05Unlock && !gData.bUnloadPort2Wait)	{ Show_Alarm("Main 05번 Door(X1304) Unlocked."); return FALSE; }
		if (pDX13->iDoor06Unlock && !gData.bUnloadPort2Wait)	{ Show_Alarm("Main 06번 Door(X1305) Unlocked."); return FALSE; }
		if (pDX13->iDoor07Unlock)								{ Show_Alarm("Main 07번 Door(X1306) Unlocked."); return FALSE; }
		if (pDX13->iDoor08Unlock)								{ Show_Alarm("Main 08번 Door(X1307) Unlocked."); return FALSE; }
		if (pDX13->iDoor09Unlock)								{ Show_Alarm("Main 09번 Door(X1308) Unlocked."); return FALSE; }
		if (pDX13->iDoor10Unlock && !gData.bUnloadPort1Wait)	{ Show_Alarm("Main 10번 Door(X1309) Unlocked."); return FALSE; }
		if (pDX13->iDoor11Unlock && !gData.bCapPort1Wait)		{ Show_Alarm("Main 11번 Door(X1310) Unlocked."); return FALSE; }
		if (pDX13->iDoor12Unlock && !gData.bCapPort2Wait)		{ Show_Alarm("Main 12번 Door(X1311) Unlocked."); return FALSE; }
		if (pDX13->iDoor13Unlock)								{ Show_Alarm("Main 13번 Door(X1312) Unlocked."); return FALSE; }
	}
#endif
	return TRUE;
}

BOOL CCommon::Check_PortArea(BOOL bAuto)
{
	// 슬라이드를 사용하지 않고 손을 장비안으로 넣어 트레이 적재 및 배출을 하기 때문에 사용하지 않는다.
	return TRUE;
#ifdef DRY_RUN_TEST
	return TRUE;
#endif
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	if (!bAuto)	return TRUE;

#ifdef AJIN_BOARD_USE
	DX_DATA_00 *pDX00 = g_objAJinAXL.Get_pDX00();
	DX_DATA_01 *pDX01 = g_objAJinAXL.Get_pDX01();
	DX_DATA_02 *pDX02 = g_objAJinAXL.Get_pDX02();
	DX_DATA_03 *pDX03 = g_objAJinAXL.Get_pDX03();
	BOOL bLoad1 = g_objSequenceMain.Get_LotLoadEnable(0);
	BOOL bLoad2 = g_objSequenceMain.Get_LotLoadEnable(1);

	if (pEquipData->bUseInlineMode && !pEquipData->bUseDoorLock) {
		// Inline 모드는 도어락 사용 안해도 무조건 Load Port쪽 확인한다.
		if (!pDX01->iLoadPortAreaCheck) { Show_Alarm("Load Port Area(X0115) 감지 되었습니다."); return FALSE; }

	} else {
		if (!pEquipData->bUseDoorLock) return TRUE;

		// Load Port1
		if (!pDX01->iLoadPortAreaCheck && (pDX00->iLoadPort1SlideOpen || pDX00->iLoadPort1SlideClose)) {
			Show_Alarm("Load Port Area(X0115) 감지 되었습니다."); return FALSE;

		} else { /*!bLoad1;*/ }

		// Load Port2
		if (!pDX01->iLoadPortAreaCheck && (pDX01->iLoadPort2SlideOpen || pDX01->iLoadPort2SlideClose)) {
			Show_Alarm("Load Port Area(X0115) 감지 되었습니다."); return FALSE;

		} else { /*!bLoad2;*/ }

		// Load Port3
		if (!pDX01->iLoadPort3AreaCheck && !gData.bLoadPort3Wait) { Show_Alarm("Load Port Area(X0131) 감지 되었습니다."); return FALSE; }

		// Cap Port1
		if (!pDX02->iCapPortAreaCheck	&& (pDX02->iCapPort1SlideOpen || pDX02->iCapPort1SlideClose)) {
			Show_Alarm("Cap Port Area(X0231) 감지 되었습니다."); return FALSE;

		} else { /*!gData.bCapPort1Wait;*/ }

		// Cap Port2
		if (!pDX02->iCapPortAreaCheck	&& (pDX02->iCapPort2SlideOpen || pDX02->iCapPort2SlideClose)) {
			Show_Alarm("Cap Port Area(X0231) 감지 되었습니다."); return FALSE;

		} else { /*!gData.bCapPort2Wait;*/ }

		// Unload Port 1
		if (!pDX03->iUnloadPort1AreaCheck && (pDX03->iUnloadPort1SlideOpen || pDX03->iUnloadPort1SlideClose)) {
			Show_Alarm("Unload Port1 Area(X0315) 감지 되었습니다."); return FALSE;

		} else { /*!gData.bUnloadPort1Wait;*/ }

		// Unload Port 2
		if (!pDX03->iUnloadPort2AreaCheck && (pDX03->iUnloadPort2SlideOpen || pDX03->iUnloadPort2SlideClose)) {
			Show_Alarm("Unload Port2 Area(X0331) 감지 되었습니다."); return FALSE;

		} else { /*!gData.bUnloadPort2Wait;*/ }
	}
#endif
	return TRUE;
}

BOOL CCommon::Check_TrayFull()
{
#ifdef AJIN_BOARD_USE
	DX_DATA_01 *pDX01 = g_objAJinAXL.Get_pDX01();
	DX_DATA_02 *pDX02 = g_objAJinAXL.Get_pDX02();
	DX_DATA_03 *pDX03 = g_objAJinAXL.Get_pDX03();

	if (pDX01->iLoadPort3HighCheck)		{ Show_Alarm("Load Port3 Full Sensor Checked."); return FALSE; }
	if (pDX02->iCapPort1HighCheck)		{ Show_Alarm("Cap Port1 Full Sensor Checked.", STATE_CAPTRAY); return FALSE; }
	if (pDX03->iUnloadPort2HighCheck)	{ Show_Alarm("Unload Port2 Full Sensor Checked.", STATE_SHIPTRAY); return FALSE; }
#endif
	return TRUE;
}

// Servo On Error : Error (100 - 145)
BOOL CCommon::Check_ServoOn()
{
#ifdef AJIN_BOARD_USE
	for (int i = 0; i < AXIS_COUNT; i++) {
		if (i == 3) continue;
		if (!g_objAJinAXL.Get_ServoOn(i)) { Show_Error(i + 100); return FALSE; }
	}
#endif
	return TRUE;
}

// Drive Alarm : Error(200 - 245)
BOOL CCommon::Check_DirveAlarm()
{
#ifdef AJIN_BOARD_USE
	for (int i = 0; i < AXIS_COUNT; i++) {
		if (i == 3) continue;
		if (g_objAJinAXL.Get_AmpAlarm(i)) { Show_Error(i + 200); return FALSE; }
	}
#endif
	return TRUE;
}

// Limit Check : Error ELN(300 - 345), ELP(400 - 445)
BOOL CCommon::Check_EndLimit()
{
#ifdef AJIN_BOARD_USE
	for (int i = 0; i < AXIS_COUNT; i++) {
		if (g_objAJinAXL.Use_OrgAxis(i)) continue;
		if (g_objAJinAXL.Get_EndLimitPos(i)) { Show_Error(i + 300); return FALSE; }
		if (g_objAJinAXL.Get_EndLimitNeg(i)) { Show_Error(i + 400); return FALSE; }
	}
#endif
	return TRUE;
}

// Home Check : Error (500 - 545)
BOOL CCommon::Check_HomeDone()
{
#ifdef AJIN_BOARD_USE
	for (int i = 0; i < AXIS_COUNT; i++) {
		if (i == AX_NO_AXIS) continue;
 		if (i == AX_INDEX_R && g_objSequenceMain.Get_MainRunCase(AUTO_MAIN_INDEX) > 20) continue;
		if (!g_objAJinAXL.Get_HomeDone(i)) { Show_Error(i + 500); return FALSE; }
	}
#endif
	return TRUE;
}

void CCommon::Locking_MainDoor(BOOL bLock, BOOL bAuto)
{
	DY_DATA_13 *pDY13 = g_objAJinAXL.Get_pDY13();

	// 0000 0000 0000 0000 0001 1111 1111 1111
	// 0    0    0    0    1    F    F    F
	if (bLock) {
		BOOL bLoad1 = g_objSequenceMain.Get_LotLoadEnable(0);
		BOOL bLoad2 = g_objSequenceMain.Get_LotLoadEnable(1);
		pDY13->nValue &= ~0x1FFF;	// All Lock
		if (bAuto && bLoad1)				pDY13->nValue |= 0x1;	// Except 1
		if (bAuto && bLoad2)				pDY13->nValue |= 0x2;	// Except 2
		if (bAuto && gData.bLoadPort3Wait)	pDY13->nValue |= 0x8;	// Except 4
		if (bAuto && gData.bCapPort1Wait)	pDY13->nValue |= 0x400;	// Except 11
		if (bAuto && gData.bCapPort2Wait)	pDY13->nValue |= 0x800;	// Except 12
		if (bAuto && gData.bUnloadPort1Wait)	pDY13->nValue |= 0x200;	// Except 10
		if (bAuto && gData.bUnloadPort2Wait)	pDY13->nValue |= 0x30;	// Except 5, 6
		// temp
		
	} else {
		DWORD dwStart = GetTickCount();
		while (TRUE) {
			BOOL bMove = FALSE;
			for (int i = 0; i < AXIS_COUNT; i++) {
				if (i == 2 || i == 3) continue;
				if (!g_objAJinAXL.Is_Done(i)) { bMove = TRUE; break; }
			}
			if (!bMove) break;
			if (GetTickCount() - dwStart > 5000) break;	// 5초
			theApp.DoEvents();
		}
		pDY13->nValue |= 0x1FFF;
	}
	g_objAJinAXL.Write_Output(13);
}

void CCommon::Locking_PortSlide(BOOL bLock, int nPart)
{
	if (nPart == 0 || nPart == 1) {
		DY_DATA_00 *pDY00 = g_objAJinAXL.Get_pDY00();
		pDY00->oLoadPort1SlideLock = bLock; pDY00->oLoadPort1SlideUnlock = !bLock;
		g_objAJinAXL.Write_Output(0);
	}
	if (nPart == 0 || nPart == 2) {
		DY_DATA_01 *pDY01 = g_objAJinAXL.Get_pDY01();
		pDY01->oLoadPort2SlideLock = bLock; pDY01->oLoadPort2SlideUnlock = !bLock;
		g_objAJinAXL.Write_Output(1);
	}
	if (nPart == 0 || nPart == 3) {
		DY_DATA_01 *pDY01 = g_objAJinAXL.Get_pDY01();
		pDY01->oLoadPort3SlideLock = bLock; pDY01->oLoadPort3SlideUnlock = !bLock;
		g_objAJinAXL.Write_Output(1);
	}
	if (nPart == 0 || nPart == 4) {
		DY_DATA_02 *pDY02 = g_objAJinAXL.Get_pDY02();
		pDY02->oCapPort1SlideLock = bLock; pDY02->oCapPort1SlideUnlock = !bLock;
		g_objAJinAXL.Write_Output(2);
	}
	if (nPart == 0 || nPart == 5) {
		DY_DATA_02 *pDY02 = g_objAJinAXL.Get_pDY02();
		pDY02->oCapPort2SlideLock = bLock; pDY02->oCapPort2SlideUnlock = !bLock;
		g_objAJinAXL.Write_Output(2);
	}
	if (nPart == 0 || nPart == 6) {
		DY_DATA_03 *pDY03 = g_objAJinAXL.Get_pDY03();
		pDY03->oUnloadPort1SlideLock = bLock; pDY03->oUnloadPort1SlideUnlock = !bLock;
		g_objAJinAXL.Write_Output(3);
	}
	if (nPart == 0 || nPart == 7) {
		DY_DATA_03 *pDY03 = g_objAJinAXL.Get_pDY03();
		pDY03->oUnloadPort2SlideLock = bLock; pDY03->oUnloadPort2SlideUnlock = !bLock;
		g_objAJinAXL.Write_Output(3);
	}
}

/////////////////////////////////////////////////////////////////////////////

int CCommon::Get_Random(int nStart, int nEnd)
{
	static BOOL bSeed = FALSE;
	if (nStart >= nEnd) return 0;
	if (!bSeed) { srand((unsigned)time(NULL)); bSeed = TRUE; }
	return (rand() % (nEnd - nStart + 1) + nStart);
}

void CCommon::Backup_File(CString strPath, CString strName)
{
	g_objLogFile.Create_Folder(strPath + "\\Backup");

	CString strOriginFile, strTargetFile, strDateTime;

	SYSTEMTIME time;
	GetLocalTime(&time);
	strDateTime.Format("%04d%02d%02d_%02d%02d%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);

	strOriginFile.Format("%s\\%s.ini", strPath, strName);
	strTargetFile.Format("%s\\Backup\\%s_%s.ini", strPath, strName, strDateTime);

	if (!CopyFile(strOriginFile, strTargetFile, FALSE)) return;
}


///////////////////////////////////////////////////////////////////////////////
//	1. Tray Picker I/O 함수
void CCommon::Set_TrayPickerGripClose()
{
	DY_DATA_00 *pDY00 = g_objAJinAXL.Get_pDY00();
	pDY00->oTrayPickerGrip12Open = FALSE; pDY00->oTrayPickerGrip12Close = TRUE;
	pDY00->oTrayPickerGrip34Open = FALSE; pDY00->oTrayPickerGrip34Close = TRUE;
	g_objAJinAXL.Write_Output(0);
}

BOOL CCommon::Get_TrayPickerGripClose()
{
	DX_DATA_00 *pDX00 = g_objAJinAXL.Get_pDX00();
	if (!pDX00->iTrayPickerGrip1Open && pDX00->iTrayPickerGrip1Close &&
		!pDX00->iTrayPickerGrip2Open && pDX00->iTrayPickerGrip2Close &&
		!pDX00->iTrayPickerGrip3Open && pDX00->iTrayPickerGrip3Close &&
		!pDX00->iTrayPickerGrip4Open && pDX00->iTrayPickerGrip4Close) return TRUE;

	return FALSE;
}

void CCommon::Set_TrayPickerGripOpen()
{
	DY_DATA_00 *pDY00 = g_objAJinAXL.Get_pDY00();
	pDY00->oTrayPickerGrip12Open = TRUE; pDY00->oTrayPickerGrip12Close = FALSE;
	pDY00->oTrayPickerGrip34Open = TRUE; pDY00->oTrayPickerGrip34Close = FALSE;
	g_objAJinAXL.Write_Output(0);
}

BOOL CCommon::Get_TrayPickerGripOpen()
{
	DX_DATA_00 *pDX00 = g_objAJinAXL.Get_pDX00();
	if (pDX00->iTrayPickerGrip1Open && !pDX00->iTrayPickerGrip1Close &&
		pDX00->iTrayPickerGrip2Open && !pDX00->iTrayPickerGrip2Close &&
		pDX00->iTrayPickerGrip3Open && !pDX00->iTrayPickerGrip3Close &&
		pDX00->iTrayPickerGrip4Open && !pDX00->iTrayPickerGrip4Close) return TRUE;

	return FALSE;
}

// 	2. Load Port I/O 함수
void CCommon::Set_LoadPortSupportIn(int nNo)
{
	DY_DATA_00 *pDY00 = g_objAJinAXL.Get_pDY00();
	DY_DATA_01 *pDY01 = g_objAJinAXL.Get_pDY01();
	if (nNo == 1) { pDY00->oLoadPort1SupportIn = TRUE; pDY00->oLoadPort1SupportOut = FALSE; }
	if (nNo == 2) { pDY01->oLoadPort2SupportIn = TRUE; pDY01->oLoadPort2SupportOut = FALSE; }
	g_objAJinAXL.Write_Output(0);
	g_objAJinAXL.Write_Output(1);
}

BOOL CCommon::Get_LoadPortSupportIn(int nNo)
{
	DX_DATA_00 *pDX00 = g_objAJinAXL.Get_pDX00();
	DX_DATA_01 *pDX01 = g_objAJinAXL.Get_pDX01();
	if (nNo == 1) {
		if (!pDX00->iLoadPort1Support1In || pDX00->iLoadPort1Support1Out) return FALSE;
		if (!pDX00->iLoadPort1Support2In || pDX00->iLoadPort1Support2Out) return FALSE;
	} else if (nNo == 2) {
		if (!pDX01->iLoadPort2Support1In || pDX01->iLoadPort2Support1Out) return FALSE;
		if (!pDX01->iLoadPort2Support2In || pDX01->iLoadPort2Support2Out) return FALSE;
	} 
	return TRUE;
}

void CCommon::Set_LoadPortSupportOut(int nNo)
{
	DY_DATA_00 *pDY00 = g_objAJinAXL.Get_pDY00();
	DY_DATA_01 *pDY01 = g_objAJinAXL.Get_pDY01();
	if (nNo == 1) pDY00->oLoadPort1SupportIn = FALSE; pDY00->oLoadPort1SupportOut = TRUE;
	if (nNo == 2) pDY01->oLoadPort2SupportIn = FALSE; pDY01->oLoadPort2SupportOut = TRUE;
	g_objAJinAXL.Write_Output(0);
	g_objAJinAXL.Write_Output(1);
}

BOOL CCommon::Get_LoadPortSupportOut(int nNo)
{
	DX_DATA_00 *pDX00 = g_objAJinAXL.Get_pDX00();
	DX_DATA_01 *pDX01 = g_objAJinAXL.Get_pDX01();
	if (nNo == 1) {
		if (pDX00->iLoadPort1Support1In || !pDX00->iLoadPort1Support1Out) return FALSE;
		if (pDX00->iLoadPort1Support2In || !pDX00->iLoadPort1Support2Out) return FALSE;
	} else if (nNo == 2) {
		if (pDX01->iLoadPort2Support1In || !pDX01->iLoadPort2Support1Out) return FALSE;
		if (pDX01->iLoadPort2Support2In || !pDX01->iLoadPort2Support2Out) return FALSE;
	} 
	return TRUE;
}

// 	3. Load Picker I/O 함수
void CCommon::Set_LoadPickerUp()
{
	DY_DATA_06 *pDY06 = g_objAJinAXL.Get_pDY06();
	pDY06->oLoadPickerDown01 = FALSE;
	pDY06->oLoadPickerDown02 = FALSE;
	pDY06->oLoadPickerDown03 = FALSE;
	if (gData.nPickerUseCnt > 3 ) pDY06->oLoadPickerDown04 = FALSE;
	if (gData.nPickerUseCnt > 4 ) pDY06->oLoadPickerDown05 = FALSE;
	if (gData.nPickerUseCnt > 5 ) pDY06->oLoadPickerDown06 = FALSE;
	g_objAJinAXL.Write_Output(6);
}

BOOL CCommon::Get_LoadPickerUp()
{
	DX_DATA_06 *pDX06 = g_objAJinAXL.Get_pDX06();

	if (!pDX06->iLoadPickerUp01 || pDX06->iLoadPickerDown01) { gAlm.nLDPickerNo = 1; return FALSE; }
	if (!pDX06->iLoadPickerUp02 || pDX06->iLoadPickerDown02) { gAlm.nLDPickerNo = 2; return FALSE; }
	if (!pDX06->iLoadPickerUp03 || pDX06->iLoadPickerDown03) { gAlm.nLDPickerNo = 3; return FALSE; }
	if (gData.nPickerUseCnt > 3 && (!pDX06->iLoadPickerUp04 || pDX06->iLoadPickerDown04)) { gAlm.nLDPickerNo = 4; return FALSE; }
	if (gData.nPickerUseCnt > 4 && (!pDX06->iLoadPickerUp05 || pDX06->iLoadPickerDown05)) { gAlm.nLDPickerNo = 5; return FALSE; }
	if (gData.nPickerUseCnt > 5 && (!pDX06->iLoadPickerUp06 || pDX06->iLoadPickerDown06)) { gAlm.nLDPickerNo = 6; return FALSE; }
	return TRUE;
}

void CCommon::Set_LoadPickerDown(int nNo)
{
	DY_DATA_06 *pDY06 = g_objAJinAXL.Get_pDY06();
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	if (nNo == 0 || nNo == 1) pDY06->oLoadPickerDown01 = TRUE;
	if (nNo == 0 || nNo == 2) pDY06->oLoadPickerDown02 = TRUE;
	if (nNo == 0 || nNo == 3) pDY06->oLoadPickerDown03 = TRUE;
	if (gData.nPickerUseCnt > 3) { if (nNo == 0 || nNo == 4) pDY06->oLoadPickerDown04 = TRUE; }
	if (gData.nPickerUseCnt > 4) { if (nNo == 0 || nNo == 5) pDY06->oLoadPickerDown05 = TRUE; }
	if (gData.nPickerUseCnt > 5) { if (nNo == 0 || nNo == 6) pDY06->oLoadPickerDown06 = TRUE; }
	g_objAJinAXL.Write_Output(6);
}

BOOL CCommon::Get_LoadPickerDown(int nNo)
{
	DX_DATA_06 *pDX06 = g_objAJinAXL.Get_pDX06();
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	if (nNo == 0) {
		if (pDX06->iLoadPickerUp01 || !pDX06->iLoadPickerDown01) { gAlm.nLDPickerNo = 1; return FALSE; }
		if (pDX06->iLoadPickerUp02 || !pDX06->iLoadPickerDown02) { gAlm.nLDPickerNo = 2; return FALSE; }
		if (pDX06->iLoadPickerUp03 || !pDX06->iLoadPickerDown03) { gAlm.nLDPickerNo = 3; return FALSE; }
		if (gData.nPickerUseCnt > 3) { if (pDX06->iLoadPickerUp04 || !pDX06->iLoadPickerDown04) { gAlm.nLDPickerNo = 4; return FALSE; } }
		if (gData.nPickerUseCnt > 4) { if (pDX06->iLoadPickerUp05 || !pDX06->iLoadPickerDown05) { gAlm.nLDPickerNo = 5; return FALSE; } }
		if (gData.nPickerUseCnt > 5) { if (pDX06->iLoadPickerUp06 || !pDX06->iLoadPickerDown06) { gAlm.nLDPickerNo = 6; return FALSE; } }
		return TRUE;
	}
	else if (nNo ==  1 && (pDX06->iLoadPickerUp01 || !pDX06->iLoadPickerDown01)) { gAlm.nLDPickerNo = 1; return FALSE; }
	else if (nNo ==  2 && (pDX06->iLoadPickerUp02 || !pDX06->iLoadPickerDown02)) { gAlm.nLDPickerNo = 2; return FALSE; }
	else if (nNo ==  3 && (pDX06->iLoadPickerUp03 || !pDX06->iLoadPickerDown03)) { gAlm.nLDPickerNo = 3; return FALSE; }
	else if (nNo ==  4 && (pDX06->iLoadPickerUp04 || !pDX06->iLoadPickerDown04)) { gAlm.nLDPickerNo = 4; return FALSE; }
	else if (nNo ==  5 && (pDX06->iLoadPickerUp05 || !pDX06->iLoadPickerDown05)) { gAlm.nLDPickerNo = 5; return FALSE; }
	else if (nNo ==  6 && (pDX06->iLoadPickerUp06 || !pDX06->iLoadPickerDown06)) { gAlm.nLDPickerNo = 6; return FALSE; }
	return TRUE;
}

void CCommon::Set_LoadPickerOpen()
{
	DY_DATA_06 *pDY06 = g_objAJinAXL.Get_pDY06();
	pDY06->oLoadPickerGrip01 = FALSE;
	pDY06->oLoadPickerGrip02 = FALSE;
	pDY06->oLoadPickerGrip03 = FALSE;
	if (gData.nPickerUseCnt > 3 ) pDY06->oLoadPickerGrip04 = FALSE;
	if (gData.nPickerUseCnt > 4 ) pDY06->oLoadPickerGrip05 = FALSE;
	if (gData.nPickerUseCnt > 5 ) pDY06->oLoadPickerGrip06 = FALSE;
	g_objAJinAXL.Write_Output(6);
}

BOOL CCommon::Get_LoadPickerOpen()
{
	DX_DATA_06 *pDX06 = g_objAJinAXL.Get_pDX06();

	if (!pDX06->iLoadPickerOpen01) { gAlm.nLDPickerNo = 1; return FALSE; }
	if (!pDX06->iLoadPickerOpen02) { gAlm.nLDPickerNo = 2; return FALSE; }
	if (!pDX06->iLoadPickerOpen03) { gAlm.nLDPickerNo = 3; return FALSE; }
	if (gData.nPickerUseCnt > 3 && !pDX06->iLoadPickerOpen04) { gAlm.nLDPickerNo = 4; return FALSE; }
	if (gData.nPickerUseCnt > 4 && !pDX06->iLoadPickerOpen05) { gAlm.nLDPickerNo = 5; return FALSE; }
	if (gData.nPickerUseCnt > 5 && !pDX06->iLoadPickerOpen06) { gAlm.nLDPickerNo = 6; return FALSE; }
	return TRUE;
}

BOOL CCommon::Get_LoadPickerCmCheckOff()
{
	DX_DATA_06 *pDX06 = g_objAJinAXL.Get_pDX06();

	if (pDX06->iLoadPickerCMCheck01) { gAlm.nLDPickerNo = 1; return FALSE; }
	if (pDX06->iLoadPickerCMCheck02) { gAlm.nLDPickerNo = 2; return FALSE; }
	if (pDX06->iLoadPickerCMCheck03) { gAlm.nLDPickerNo = 3; return FALSE; }
	if (gData.nPickerUseCnt > 3 && pDX06->iLoadPickerCMCheck04) { gAlm.nLDPickerNo = 4; return FALSE; }
	if (gData.nPickerUseCnt > 4 && pDX06->iLoadPickerCMCheck05) { gAlm.nLDPickerNo = 5; return FALSE; }
	if (gData.nPickerUseCnt > 5 && pDX06->iLoadPickerCMCheck06) { gAlm.nLDPickerNo = 6; return FALSE; }
	return TRUE;
}

void CCommon::Set_LoadPickerDownMulti(int nNo, int nCnt)
{
	DY_DATA_06 *pDY06 = g_objAJinAXL.Get_pDY06();

	if (nNo == 1) {
		if (nCnt > 0) pDY06->oLoadPickerDown01 = TRUE;
		if (nCnt > 1) pDY06->oLoadPickerDown02 = TRUE;
		if (nCnt > 2) pDY06->oLoadPickerDown03 = TRUE;
		if (gData.nPickerUseCnt > 3) { if (nCnt > 3) pDY06->oLoadPickerDown04 = TRUE; }
		if (gData.nPickerUseCnt > 4) { if (nCnt > 4) pDY06->oLoadPickerDown05 = TRUE; }
		if (gData.nPickerUseCnt > 5) { if (nCnt > 5) pDY06->oLoadPickerDown06 = TRUE; }
	}
	else if (nNo == 2) {
		if (nCnt > 0) pDY06->oLoadPickerDown02 = TRUE;
		if (nCnt > 1) pDY06->oLoadPickerDown03 = TRUE;
		if (gData.nPickerUseCnt > 3) { if (nCnt > 2) pDY06->oLoadPickerDown04 = TRUE; }
		if (gData.nPickerUseCnt > 4) { if (nCnt > 3) pDY06->oLoadPickerDown05 = TRUE; }
		if (gData.nPickerUseCnt > 5) { if (nCnt > 4) pDY06->oLoadPickerDown06 = TRUE; }
	}
	else if (nNo == 3) {
		if (nCnt > 0) pDY06->oLoadPickerDown03 = TRUE;
		if (gData.nPickerUseCnt > 3) { if (nCnt > 1) pDY06->oLoadPickerDown04 = TRUE; }
		if (gData.nPickerUseCnt > 4) { if (nCnt > 2) pDY06->oLoadPickerDown05 = TRUE; }
		if (gData.nPickerUseCnt > 5) {if (nCnt > 3) pDY06->oLoadPickerDown06 = TRUE; }
	}
	else if (nNo == 4) {
		if (gData.nPickerUseCnt > 3) {if (nCnt > 0) pDY06->oLoadPickerDown04 = TRUE;}
		if (gData.nPickerUseCnt > 4) {if (nCnt > 1) pDY06->oLoadPickerDown05 = TRUE;}
		if (gData.nPickerUseCnt > 5) {if (nCnt > 2) pDY06->oLoadPickerDown06 = TRUE;}
	}
	else if (nNo == 5) {
		if (gData.nPickerUseCnt > 4) {if (nCnt > 0) pDY06->oLoadPickerDown05 = TRUE;}
		if (gData.nPickerUseCnt > 5) {if (nCnt > 1) pDY06->oLoadPickerDown06 = TRUE;}
	}
	else if (nNo == 6) {
		if (gData.nPickerUseCnt > 5) {if (nCnt > 0) pDY06->oLoadPickerDown06 = TRUE;}
	}

	g_objAJinAXL.Write_Output(6);
}

BOOL CCommon::Get_LoadPickerDownMulti(int nNo, int nCnt)
{
	DX_DATA_06 *pDX06 = g_objAJinAXL.Get_pDX06();

	if (nNo == 1) {
		if (nCnt > 0 && (pDX06->iLoadPickerUp01 || !pDX06->iLoadPickerDown01)) { gAlm.nLDPickerNo = 1; return FALSE; }
		if (nCnt > 1 && (pDX06->iLoadPickerUp02 || !pDX06->iLoadPickerDown02)) { gAlm.nLDPickerNo = 2; return FALSE; }
		if (nCnt > 2 && (pDX06->iLoadPickerUp03 || !pDX06->iLoadPickerDown03)) { gAlm.nLDPickerNo = 3; return FALSE; }
		if (gData.nPickerUseCnt > 3) {if (nCnt > 3 && (pDX06->iLoadPickerUp04 || !pDX06->iLoadPickerDown04)) { gAlm.nLDPickerNo = 4; return FALSE; }}
		if (gData.nPickerUseCnt > 4) {if (nCnt > 4 && (pDX06->iLoadPickerUp05 || !pDX06->iLoadPickerDown05)) { gAlm.nLDPickerNo = 5; return FALSE; }}
		if (gData.nPickerUseCnt > 5) {if (nCnt > 5 && (pDX06->iLoadPickerUp06 || !pDX06->iLoadPickerDown06)) { gAlm.nLDPickerNo = 6; return FALSE; }}
	}
	else if (nNo == 2) {
		if (nCnt > 0 && (pDX06->iLoadPickerUp02 || !pDX06->iLoadPickerDown02)) { gAlm.nLDPickerNo = 2; return FALSE; }
		if (nCnt > 1 && (pDX06->iLoadPickerUp03 || !pDX06->iLoadPickerDown03)) { gAlm.nLDPickerNo = 3; return FALSE; }
		if (gData.nPickerUseCnt > 3) {if (nCnt > 2 && (pDX06->iLoadPickerUp04 || !pDX06->iLoadPickerDown04)) { gAlm.nLDPickerNo = 4; return FALSE; }}
		if (gData.nPickerUseCnt > 4) {if (nCnt > 3 && (pDX06->iLoadPickerUp05 || !pDX06->iLoadPickerDown05)) { gAlm.nLDPickerNo = 5; return FALSE; }}
		if (gData.nPickerUseCnt > 5) {if (nCnt > 4 && (pDX06->iLoadPickerUp06 || !pDX06->iLoadPickerDown06)) { gAlm.nLDPickerNo = 6; return FALSE; }}
	}
	else if (nNo == 3) {
		if (nCnt > 0 && (pDX06->iLoadPickerUp03 || !pDX06->iLoadPickerDown03)) { gAlm.nLDPickerNo = 3; return FALSE; }
		if (gData.nPickerUseCnt > 3) {if (nCnt > 1 && (pDX06->iLoadPickerUp04 || !pDX06->iLoadPickerDown04)) { gAlm.nLDPickerNo = 4; return FALSE; }}
		if (gData.nPickerUseCnt > 4) {if (nCnt > 2 && (pDX06->iLoadPickerUp05 || !pDX06->iLoadPickerDown05)) { gAlm.nLDPickerNo = 5; return FALSE; }}
		if (gData.nPickerUseCnt > 5) {if (nCnt > 3 && (pDX06->iLoadPickerUp06 || !pDX06->iLoadPickerDown06)) { gAlm.nLDPickerNo = 6; return FALSE; }}
	}
	else if (nNo == 4) {
		if (gData.nPickerUseCnt > 3) { if (nCnt > 0 && (pDX06->iLoadPickerUp04 || !pDX06->iLoadPickerDown04)) { gAlm.nLDPickerNo = 4; return FALSE; }}
		if (gData.nPickerUseCnt > 4) {if (nCnt > 1 && (pDX06->iLoadPickerUp05 || !pDX06->iLoadPickerDown05)) { gAlm.nLDPickerNo = 5; return FALSE; }}
		if (gData.nPickerUseCnt > 5) {if (nCnt > 2 && (pDX06->iLoadPickerUp06 || !pDX06->iLoadPickerDown06)) { gAlm.nLDPickerNo = 6; return FALSE; }}
	}
	else if (nNo == 5) {
		if (gData.nPickerUseCnt > 4) {if (nCnt > 0 && (pDX06->iLoadPickerUp05 || !pDX06->iLoadPickerDown05)) { gAlm.nLDPickerNo = 5; return FALSE; }}
		if (gData.nPickerUseCnt > 5) {if (nCnt > 1 && (pDX06->iLoadPickerUp06 || !pDX06->iLoadPickerDown06)) { gAlm.nLDPickerNo = 6; return FALSE; }}
	}
	else if (nNo == 6) {
		if (gData.nPickerUseCnt > 5) {if (nCnt > 0 && (pDX06->iLoadPickerUp06 || !pDX06->iLoadPickerDown06)) { gAlm.nLDPickerNo = 6; return FALSE; }}
	}

	return TRUE;
}

// 4. Index Table I/O 함수
BOOL CCommon::Get_IndexLoadVacuumOff(int nNo)
{
	DX_DATA_11 *pDX11 = g_objAJinAXL.Get_pDX11();

	if(nNo == 0){
		if (pDX11->iIndexLoadVac01) { gAlm.nIndexVacNo = 1; return FALSE; }
		if (pDX11->iIndexLoadVac02) { gAlm.nIndexVacNo = 2; return FALSE; }
		if (pDX11->iIndexLoadVac03) { gAlm.nIndexVacNo = 3; return FALSE; }
		if (pDX11->iIndexLoadVac04) { gAlm.nIndexVacNo = 4; return FALSE; }
		if (pDX11->iIndexLoadVac05) { gAlm.nIndexVacNo = 5; return FALSE; }
		if (pDX11->iIndexLoadVac06) { gAlm.nIndexVacNo = 6; return FALSE; }
	}
	else if (nNo == 1 && pDX11->iIndexLoadVac01) { gAlm.nIndexVacNo = 1; return FALSE; }
	else if (nNo == 2 && pDX11->iIndexLoadVac02) { gAlm.nIndexVacNo = 2; return FALSE; }
	else if (nNo == 3 && pDX11->iIndexLoadVac03) { gAlm.nIndexVacNo = 3; return FALSE; }
	else if (nNo == 4 && pDX11->iIndexLoadVac04) { gAlm.nIndexVacNo = 4; return FALSE; }
	else if (nNo == 5 && pDX11->iIndexLoadVac05) { gAlm.nIndexVacNo = 5; return FALSE; }
	else if (nNo == 6 && pDX11->iIndexLoadVac06) { gAlm.nIndexVacNo = 6; return FALSE; }
	return TRUE;
}

void CCommon::Set_IndexLoadVacuumOff(int nNo)
{
	DY_DATA_11 *pDY11 = g_objAJinAXL.Get_pDY11();

	if (nNo == 0 || nNo == 1) pDY11->oIndexLoadVac01 = FALSE;
	if (nNo == 0 || nNo == 2) pDY11->oIndexLoadVac02 = FALSE;
	if (nNo == 0 || nNo == 3) pDY11->oIndexLoadVac03 = FALSE;
	if (nNo == 0 || nNo == 4) pDY11->oIndexLoadVac04 = FALSE;
	if (nNo == 0 || nNo == 5) pDY11->oIndexLoadVac05 = FALSE;
	if (nNo == 0 || nNo == 6) pDY11->oIndexLoadVac06 = FALSE;

	g_objAJinAXL.Write_Output(11);
}

BOOL CCommon::Get_IndexAssyVacuumOff(int nNo)
{
	DX_DATA_11 *pDX11 = g_objAJinAXL.Get_pDX11();

	if(nNo == 0){
		if (pDX11->iIndexAssyVac01) { gAlm.nIndexVacNo = 1; return FALSE; }
		if (pDX11->iIndexAssyVac02) { gAlm.nIndexVacNo = 2; return FALSE; }
		if (pDX11->iIndexAssyVac03) { gAlm.nIndexVacNo = 3; return FALSE; }
		if (pDX11->iIndexAssyVac04) { gAlm.nIndexVacNo = 4; return FALSE; }
		if (pDX11->iIndexAssyVac05) { gAlm.nIndexVacNo = 5; return FALSE; }
		if (pDX11->iIndexAssyVac06) { gAlm.nIndexVacNo = 6; return FALSE; }
	}
	else if (nNo == 1 && pDX11->iIndexAssyVac01) { gAlm.nIndexVacNo = 1; return FALSE; }
	else if (nNo == 2 && pDX11->iIndexAssyVac02) { gAlm.nIndexVacNo = 2; return FALSE; }
	else if (nNo == 3 && pDX11->iIndexAssyVac03) { gAlm.nIndexVacNo = 3; return FALSE; }
	else if (nNo == 4 && pDX11->iIndexAssyVac04) { gAlm.nIndexVacNo = 4; return FALSE; }
	else if (nNo == 5 && pDX11->iIndexAssyVac05) { gAlm.nIndexVacNo = 5; return FALSE; }
	else if (nNo == 6 && pDX11->iIndexAssyVac06) { gAlm.nIndexVacNo = 6; return FALSE; }
	return TRUE;
}

void CCommon::Set_IndexAssyVacuumOff(int nNo)
{
	DY_DATA_11 *pDY11 = g_objAJinAXL.Get_pDY11();

	if (nNo == 0 || nNo == 1) pDY11->oIndexAssyVac01 = FALSE;
	if (nNo == 0 || nNo == 2) pDY11->oIndexAssyVac02 = FALSE;
	if (nNo == 0 || nNo == 3) pDY11->oIndexAssyVac03 = FALSE;
	if (nNo == 0 || nNo == 4) pDY11->oIndexAssyVac04 = FALSE;
	if (nNo == 0 || nNo == 5) pDY11->oIndexAssyVac05 = FALSE;
	if (nNo == 0 || nNo == 6) pDY11->oIndexAssyVac06 = FALSE;

	g_objAJinAXL.Write_Output(11);
}

// 	5. Cap Port I/O 함수
void CCommon::Set_CapPortSupportIn(int nNo)
{
	DY_DATA_02 *pDY02 = g_objAJinAXL.Get_pDY02();
	if (nNo == 1) { pDY02->oCapPort1SupportIn = TRUE; pDY02->oCapPort1SupportOut = FALSE; }
	if (nNo == 2) { pDY02->oCapPort2SupportIn = TRUE; pDY02->oCapPort2SupportOut = FALSE; }
	g_objAJinAXL.Write_Output(2);
}

BOOL CCommon::Get_CapPortSupportIn(int nNo)
{
	DX_DATA_02 *pDX02 = g_objAJinAXL.Get_pDX02();
	if (nNo == 1) {
		if (!pDX02->iCapPort1Support1In || pDX02->iCapPort1Support1Out) return FALSE;
		if (!pDX02->iCapPort1Support2In || pDX02->iCapPort1Support2Out) return FALSE;
	} else if (nNo == 2) {
		if (!pDX02->iCapPort2Support1In || pDX02->iCapPort2Support1Out) return FALSE;
		if (!pDX02->iCapPort2Support2In || pDX02->iCapPort2Support2Out) return FALSE;
	} 
	return TRUE;
}

void CCommon::Set_CapPortSupportOut(int nNo)
{
	DY_DATA_02 *pDY02 = g_objAJinAXL.Get_pDY02();
	if (nNo == 1) pDY02->oCapPort1SupportIn = FALSE; pDY02->oCapPort1SupportOut = TRUE;
	if (nNo == 2) pDY02->oCapPort2SupportIn = FALSE; pDY02->oCapPort2SupportOut = TRUE;
	g_objAJinAXL.Write_Output(2);
}

BOOL CCommon::Get_CapPortSupportOut(int nNo)
{
	DX_DATA_02 *pDX02 = g_objAJinAXL.Get_pDX02();
	if (nNo == 1) {
		if (pDX02->iCapPort1Support1In || !pDX02->iCapPort1Support1Out) return FALSE;
		if (pDX02->iCapPort1Support2In || !pDX02->iCapPort1Support2Out) return FALSE;
	} else if (nNo == 2) {
		if (pDX02->iCapPort2Support1In || !pDX02->iCapPort2Support1Out) return FALSE;
		if (pDX02->iCapPort2Support2In || !pDX02->iCapPort2Support2Out) return FALSE;
	} 
	return TRUE;
}

// 	6. Cap Picker I/O 함수
void CCommon::Set_CapPickerUp(int nNo)
{
	DY_DATA_07 *pDY07 = g_objAJinAXL.Get_pDY07();
	if (nNo == 0 || nNo == 1) pDY07->oCapPickerDown01 = FALSE;
	if (nNo == 0 || nNo == 2) pDY07->oCapPickerDown02 = FALSE;
	if (nNo == 0 || nNo == 3) pDY07->oCapPickerDown03 = FALSE;
	if (nNo == 0 || nNo == 4) pDY07->oCapPickerDown04 = FALSE;
	if (nNo == 0 || nNo == 5) pDY07->oCapPickerDown05 = FALSE;
	if (nNo == 0 || nNo == 6) pDY07->oCapPickerDown06 = FALSE;
	g_objAJinAXL.Write_Output(7);
}

BOOL CCommon::Get_CapPickerUp(int nNo)
{
	DX_DATA_07 *pDX07 = g_objAJinAXL.Get_pDX07();
	if (nNo == 0) {
		if (!pDX07->iCapPickerUp01 || pDX07->iCapPickerDown01) { gAlm.nCapPickeNo = 1; return FALSE; }
		if (!pDX07->iCapPickerUp02 || pDX07->iCapPickerDown02) { gAlm.nCapPickeNo = 2; return FALSE; }
		if (!pDX07->iCapPickerUp03 || pDX07->iCapPickerDown03) { gAlm.nCapPickeNo = 3; return FALSE; }
		if (gData.nPickerUseCnt > 3 && (!pDX07->iCapPickerUp04 || pDX07->iCapPickerDown04)) { gAlm.nCapPickeNo = 4; return FALSE; }
		if (gData.nPickerUseCnt > 4 && (!pDX07->iCapPickerUp05 || pDX07->iCapPickerDown05)) { gAlm.nCapPickeNo = 5; return FALSE; }
		if (gData.nPickerUseCnt > 5 && (!pDX07->iCapPickerUp06 || pDX07->iCapPickerDown06)) { gAlm.nCapPickeNo = 6; return FALSE; }
	}
	else if (nNo ==  1 && (!pDX07->iCapPickerUp01 || pDX07->iCapPickerDown01)) { gAlm.nCapPickeNo = 1; return FALSE; }
	else if (nNo ==  2 && (!pDX07->iCapPickerUp02 || pDX07->iCapPickerDown02)) { gAlm.nCapPickeNo = 2; return FALSE; }
	else if (nNo ==  3 && (!pDX07->iCapPickerUp03 || pDX07->iCapPickerDown03)) { gAlm.nCapPickeNo = 3; return FALSE; }
	else if (nNo ==  4 && (!pDX07->iCapPickerUp04 || pDX07->iCapPickerDown04)) { gAlm.nCapPickeNo = 4; return FALSE; }
	else if (nNo ==  5 && (!pDX07->iCapPickerUp05 || pDX07->iCapPickerDown05)) { gAlm.nCapPickeNo = 5; return FALSE; }
	else if (nNo ==  6 && (!pDX07->iCapPickerUp06 || pDX07->iCapPickerDown06)) { gAlm.nCapPickeNo = 6; return FALSE; }
	return TRUE;
}

void CCommon::Set_CapPickerNormal(int nNo)
{
	DY_DATA_07 *pDY07 = g_objAJinAXL.Get_pDY07();
	if (nNo == 0 || nNo == 1) pDY07->oCapPickerTurn01 = FALSE;
	if (nNo == 0 || nNo == 2) pDY07->oCapPickerTurn02 = FALSE;
	if (nNo == 0 || nNo == 3) pDY07->oCapPickerTurn03 = FALSE;
	if (nNo == 0 || nNo == 4) pDY07->oCapPickerTurn04 = FALSE;
	if (nNo == 0 || nNo == 5) pDY07->oCapPickerTurn05 = FALSE;
	if (nNo == 0 || nNo == 6) pDY07->oCapPickerTurn06 = FALSE;
	g_objAJinAXL.Write_Output(7);
}

BOOL CCommon::Get_CapPickerNormal(int nNo)
{
	DX_DATA_07 *pDX07 = g_objAJinAXL.Get_pDX07();
	if (nNo == 0) {
		if (pDX07->iCapPickerTurn01) return FALSE;
		if (pDX07->iCapPickerTurn02) return FALSE;
		if (pDX07->iCapPickerTurn03) return FALSE;
		if (gData.nPickerUseCnt > 3 && pDX07->iCapPickerTurn04) return FALSE;
		if (gData.nPickerUseCnt > 4 && pDX07->iCapPickerTurn05) return FALSE;
		if (gData.nPickerUseCnt > 5 && pDX07->iCapPickerTurn06) return FALSE;
		return TRUE;
	}
	else if (nNo ==  1 && !pDX07->iCapPickerTurn01) return TRUE;
	else if (nNo ==  2 && !pDX07->iCapPickerTurn02) return TRUE;
	else if (nNo ==  3 && !pDX07->iCapPickerTurn03) return TRUE;
	else if (nNo ==  4 && !pDX07->iCapPickerTurn04) return TRUE;
	else if (nNo ==  5 && !pDX07->iCapPickerTurn05) return TRUE;
	else if (nNo ==  6 && !pDX07->iCapPickerTurn06) return TRUE;
	return FALSE;
}

void CCommon::Set_CapPickerTurn(int nNo)
{
	DY_DATA_07 *pDY07 = g_objAJinAXL.Get_pDY07();
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	if (nNo == 0 || nNo == 1) pDY07->oCapPickerTurn01 = TRUE;
	if (nNo == 0 || nNo == 2) pDY07->oCapPickerTurn02 = TRUE;
	if (nNo == 0 || nNo == 3) pDY07->oCapPickerTurn03 = TRUE;
	if (gData.nPickerUseCnt > 3) { if (nNo == 0 || nNo == 4) pDY07->oCapPickerTurn04 = TRUE; }
	if (gData.nPickerUseCnt > 4) { if (nNo == 0 || nNo == 5) pDY07->oCapPickerTurn05 = TRUE; }
	if (gData.nPickerUseCnt > 5) { if (nNo == 0 || nNo == 6) pDY07->oCapPickerTurn06 = TRUE; }
	g_objAJinAXL.Write_Output(7);
}

BOOL CCommon::Get_CapPickerTurn(int nNo)
{
	DX_DATA_07 *pDX07 = g_objAJinAXL.Get_pDX07();
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	if (nNo == 0) {
		if (!pDX07->iCapPickerTurn01) { gAlm.nCapPickeNo = 1; return FALSE; }
		if (!pDX07->iCapPickerTurn02) { gAlm.nCapPickeNo = 2; return FALSE; }
		if (!pDX07->iCapPickerTurn03) { gAlm.nCapPickeNo = 3; return FALSE; }
		if (gData.nPickerUseCnt > 3 && !pDX07->iCapPickerTurn04) { gAlm.nCapPickeNo = 4; return FALSE; }
		if (gData.nPickerUseCnt > 4 && !pDX07->iCapPickerTurn05) { gAlm.nCapPickeNo = 5; return FALSE; }
		if (gData.nPickerUseCnt > 5 && !pDX07->iCapPickerTurn06) { gAlm.nCapPickeNo = 6; return FALSE; }
	}
	else if (nNo ==  1 && !pDX07->iCapPickerTurn01) { gAlm.nCapPickeNo = 1; return FALSE; }
	else if (nNo ==  2 && !pDX07->iCapPickerTurn02) { gAlm.nCapPickeNo = 2; return FALSE; }
	else if (nNo ==  3 && !pDX07->iCapPickerTurn03) { gAlm.nCapPickeNo = 3; return FALSE; }
	else if (nNo ==  4 && !pDX07->iCapPickerTurn04) { gAlm.nCapPickeNo = 4; return FALSE; }
	else if (nNo ==  5 && !pDX07->iCapPickerTurn05) { gAlm.nCapPickeNo = 5; return FALSE; }
	else if (nNo ==  6 && !pDX07->iCapPickerTurn06) { gAlm.nCapPickeNo = 6; return FALSE; }
	return TRUE;
}

void CCommon::Set_CapPickerVacOff(int nNo)
{
	DY_DATA_07 *pDY07 = g_objAJinAXL.Get_pDY07();
	if (nNo == 0 || nNo == 1) { pDY07->oCapPickerVac01 = FALSE; pDY07->oCapPickerAir01 = TRUE; }
	if (nNo == 0 || nNo == 2) { pDY07->oCapPickerVac02 = FALSE; pDY07->oCapPickerAir02 = TRUE; }
	if (nNo == 0 || nNo == 3) { pDY07->oCapPickerVac03 = FALSE; pDY07->oCapPickerAir03 = TRUE; }
	if (nNo == 0 || nNo == 4) { pDY07->oCapPickerVac04 = FALSE; pDY07->oCapPickerAir04 = TRUE; }
	if (nNo == 0 || nNo == 5) { pDY07->oCapPickerVac05 = FALSE; pDY07->oCapPickerAir05 = TRUE; }
	if (nNo == 0 || nNo == 6) { pDY07->oCapPickerVac06 = FALSE; pDY07->oCapPickerAir06 = TRUE; }
	g_objAJinAXL.Write_Output(7);
}

BOOL CCommon::Get_CapPickerVacOff(int nNo)
{
	DX_DATA_07 *pDX07 = g_objAJinAXL.Get_pDX07();
	if (nNo == 0) {
		if (!pDX07->iCapPickerVac01 &&
			!pDX07->iCapPickerVac02 &&
			!pDX07->iCapPickerVac03 &&
			!pDX07->iCapPickerVac04 &&
			!pDX07->iCapPickerVac05 &&
			!pDX07->iCapPickerVac06) return TRUE;
	}
	else if (nNo ==  1 && !pDX07->iCapPickerVac01) return TRUE;
	else if (nNo ==  2 && !pDX07->iCapPickerVac02) return TRUE;
	else if (nNo ==  3 && !pDX07->iCapPickerVac03) return TRUE;
	else if (nNo ==  4 && !pDX07->iCapPickerVac04) return TRUE;
	else if (nNo ==  5 && !pDX07->iCapPickerVac05) return TRUE;
	else if (nNo ==  6 && !pDX07->iCapPickerVac06) return TRUE;
	return FALSE;
}

void CCommon::Set_CapPickerAirOff(int nNo)
{
	DY_DATA_07 *pDY07 = g_objAJinAXL.Get_pDY07();
	if (nNo == 0 || nNo == 1) pDY07->oCapPickerAir01 = FALSE;
	if (nNo == 0 || nNo == 2) pDY07->oCapPickerAir02 = FALSE;
	if (nNo == 0 || nNo == 3) pDY07->oCapPickerAir03 = FALSE;
	if (nNo == 0 || nNo == 4) pDY07->oCapPickerAir04 = FALSE;
	if (nNo == 0 || nNo == 5) pDY07->oCapPickerAir05 = FALSE;
	if (nNo == 0 || nNo == 6) pDY07->oCapPickerAir06 = FALSE;
	g_objAJinAXL.Write_Output(7);
}

void CCommon::Set_CapPickerDownMulti(int nNo, int nCnt)
{
	DY_DATA_07 *pDY07 = g_objAJinAXL.Get_pDY07();

	if (nNo == 1) {
		if (nCnt > 0) pDY07->oCapPickerDown01 = TRUE;
		if (nCnt > 1) pDY07->oCapPickerDown02 = TRUE;
		if (nCnt > 2) pDY07->oCapPickerDown03 = TRUE;
		if (nCnt > 3) pDY07->oCapPickerDown04 = TRUE;
		if (nCnt > 4) pDY07->oCapPickerDown05 = TRUE;
		if (nCnt > 5) pDY07->oCapPickerDown06 = TRUE;
	}
	else if (nNo == 2) {
		if (nCnt > 0) pDY07->oCapPickerDown02 = TRUE;
		if (nCnt > 1) pDY07->oCapPickerDown03 = TRUE;
		if (nCnt > 2) pDY07->oCapPickerDown04 = TRUE;
		if (nCnt > 3) pDY07->oCapPickerDown05 = TRUE;
		if (nCnt > 4) pDY07->oCapPickerDown06 = TRUE;
	}
	else if (nNo == 3) {
		if (nCnt > 0) pDY07->oCapPickerDown03 = TRUE;
		if (nCnt > 1) pDY07->oCapPickerDown04 = TRUE;
		if (nCnt > 2) pDY07->oCapPickerDown05 = TRUE;
		if (nCnt > 3) pDY07->oCapPickerDown06 = TRUE;
	}
	else if (nNo == 4) {
		if (nCnt > 0) pDY07->oCapPickerDown04 = TRUE;
		if (nCnt > 1) pDY07->oCapPickerDown05 = TRUE;
		if (nCnt > 2) pDY07->oCapPickerDown06 = TRUE;
	}
	else if (nNo == 5) {
		if (nCnt > 0) pDY07->oCapPickerDown05 = TRUE;
		if (nCnt > 1) pDY07->oCapPickerDown06 = TRUE;
	}
	else if (nNo == 6) {
		if (nCnt > 0) pDY07->oCapPickerDown06 = TRUE;
	}

	g_objAJinAXL.Write_Output(7);
}

BOOL CCommon::Get_CapPickerDownMulti(int nNo, int nCnt)
{
	DX_DATA_07 *pDX07 = g_objAJinAXL.Get_pDX07();

	if (nNo == 1) {
		if (nCnt > 0 && (pDX07->iCapPickerUp01 || !pDX07->iCapPickerDown01)) { gAlm.nCapPickeNo = 1; return FALSE; }
		if (nCnt > 1 && (pDX07->iCapPickerUp02 || !pDX07->iCapPickerDown02)) { gAlm.nCapPickeNo = 2; return FALSE; }
		if (nCnt > 2 && (pDX07->iCapPickerUp03 || !pDX07->iCapPickerDown03)) { gAlm.nCapPickeNo = 3; return FALSE; }
		if (gData.nPickerUseCnt > 3) {if (nCnt > 3 && (pDX07->iCapPickerUp04 || !pDX07->iCapPickerDown04)) { gAlm.nCapPickeNo = 4; return FALSE; }}
		if (gData.nPickerUseCnt > 4) {if (nCnt > 4 && (pDX07->iCapPickerUp05 || !pDX07->iCapPickerDown05)) { gAlm.nCapPickeNo = 5; return FALSE; }}
		if (gData.nPickerUseCnt > 5) {if (nCnt > 5 && (pDX07->iCapPickerUp06 || !pDX07->iCapPickerDown06)) { gAlm.nCapPickeNo = 6; return FALSE; }}
	}
	else if (nNo == 2) {
		if (nCnt > 0 && (pDX07->iCapPickerUp02 || !pDX07->iCapPickerDown02)) { gAlm.nCapPickeNo = 2; return FALSE; }
		if (nCnt > 1 && (pDX07->iCapPickerUp03 || !pDX07->iCapPickerDown03)) { gAlm.nCapPickeNo = 3; return FALSE; }
		if (gData.nPickerUseCnt > 3){if (nCnt > 2 && (pDX07->iCapPickerUp04 || !pDX07->iCapPickerDown04)) { gAlm.nCapPickeNo = 4; return FALSE; }}
		if (gData.nPickerUseCnt > 4){if (nCnt > 3 && (pDX07->iCapPickerUp05 || !pDX07->iCapPickerDown05)) { gAlm.nCapPickeNo = 5; return FALSE; }}
		if (gData.nPickerUseCnt > 5){if (nCnt > 4 && (pDX07->iCapPickerUp06 || !pDX07->iCapPickerDown06)) { gAlm.nCapPickeNo = 6; return FALSE; }}
	}
	else if (nNo == 3) {
		if (nCnt > 0 && (pDX07->iCapPickerUp03 || !pDX07->iCapPickerDown03)) { gAlm.nCapPickeNo = 3; return FALSE; }
		if (gData.nPickerUseCnt > 3){if (nCnt > 1 && (pDX07->iCapPickerUp04 || !pDX07->iCapPickerDown04)) { gAlm.nCapPickeNo = 4; return FALSE; }}
		if (gData.nPickerUseCnt > 4){if (nCnt > 2 && (pDX07->iCapPickerUp05 || !pDX07->iCapPickerDown05)) { gAlm.nCapPickeNo = 5; return FALSE; }}
		if (gData.nPickerUseCnt > 5){if (nCnt > 3 && (pDX07->iCapPickerUp06 || !pDX07->iCapPickerDown06)) { gAlm.nCapPickeNo = 6; return FALSE; }}
	}
	else if (nNo == 4) {
		if (gData.nPickerUseCnt > 3){if (nCnt > 0 && (pDX07->iCapPickerUp04 || !pDX07->iCapPickerDown04)) { gAlm.nCapPickeNo = 4; return FALSE; }}
		if (gData.nPickerUseCnt > 4){if (nCnt > 1 && (pDX07->iCapPickerUp05 || !pDX07->iCapPickerDown05)) { gAlm.nCapPickeNo = 5; return FALSE; }}
		if (gData.nPickerUseCnt > 5){if (nCnt > 2 && (pDX07->iCapPickerUp06 || !pDX07->iCapPickerDown06)) { gAlm.nCapPickeNo = 6; return FALSE; }}
	}
	else if (nNo == 5) {
		if (gData.nPickerUseCnt > 4){if (nCnt > 0 && (pDX07->iCapPickerUp05 || !pDX07->iCapPickerDown05)) { gAlm.nCapPickeNo = 5; return FALSE; }}
		if (gData.nPickerUseCnt > 5){if (nCnt > 1 && (pDX07->iCapPickerUp06 || !pDX07->iCapPickerDown06)) { gAlm.nCapPickeNo = 6; return FALSE; }}
	}
	else if (nNo == 6) {
		if (gData.nPickerUseCnt > 5){if (nCnt > 0 && (pDX07->iCapPickerUp06 || !pDX07->iCapPickerDown06)) { gAlm.nCapPickeNo = 6; return FALSE; }}
	}

	return TRUE;
}

void CCommon::Set_CapPickerVacOnMulti(int nNo, int nCnt)
{
#ifdef DRY_RUN_TEST
	return;
#endif
	DY_DATA_07 *pDY07 = g_objAJinAXL.Get_pDY07();

	if (nNo == 1) {
		if (nCnt > 0) { pDY07->oCapPickerVac01 = TRUE; }
		if (nCnt > 1) { pDY07->oCapPickerVac02 = TRUE; }
		if (nCnt > 2) { pDY07->oCapPickerVac03 = TRUE; }
		if (nCnt > 3) { pDY07->oCapPickerVac04 = TRUE; }
		if (nCnt > 4) { pDY07->oCapPickerVac05 = TRUE; }
		if (nCnt > 5) { pDY07->oCapPickerVac06 = TRUE; }
	}
	else if (nNo == 2) {
		if (nCnt > 0) { pDY07->oCapPickerVac02 = TRUE; }
		if (nCnt > 1) { pDY07->oCapPickerVac03 = TRUE; }
		if (nCnt > 2) { pDY07->oCapPickerVac04 = TRUE; }
		if (nCnt > 3) { pDY07->oCapPickerVac05 = TRUE; }
		if (nCnt > 4) { pDY07->oCapPickerVac06 = TRUE; }
	}
	else if (nNo == 3) {
		if (nCnt > 0) { pDY07->oCapPickerVac03 = TRUE; }
		if (nCnt > 1) { pDY07->oCapPickerVac04 = TRUE; }
		if (nCnt > 2) { pDY07->oCapPickerVac05 = TRUE; }
		if (nCnt > 3) { pDY07->oCapPickerVac06 = TRUE; }
	}
	else if (nNo == 4) {
		if (nCnt > 0) { pDY07->oCapPickerVac04 = TRUE; }
		if (nCnt > 1) { pDY07->oCapPickerVac05 = TRUE; }
		if (nCnt > 2) { pDY07->oCapPickerVac06 = TRUE; }
	}
	else if (nNo == 5) {
		if (nCnt > 0) { pDY07->oCapPickerVac05 = TRUE; }
		if (nCnt > 1) { pDY07->oCapPickerVac06 = TRUE; }
	}
	else if (nNo == 6) {
		if (nCnt > 0) { pDY07->oCapPickerVac06 = TRUE; }
	}

	g_objAJinAXL.Write_Output(7);
}

BOOL CCommon::Get_CapPickerVacOnMulti(int nNo, int nCnt)
{
#ifdef DRY_RUN_TEST
	return TRUE;
#endif
	DX_DATA_07 *pDX07 = g_objAJinAXL.Get_pDX07();

	if (nNo == 1) {
		if (nCnt > 0 && !pDX07->iCapPickerVac01) return FALSE;
		if (nCnt > 1 && !pDX07->iCapPickerVac02) return FALSE;
		if (nCnt > 2 && !pDX07->iCapPickerVac03) return FALSE;
		if (gData.nPickerUseCnt > 3){if (nCnt > 3 && !pDX07->iCapPickerVac04) return FALSE;}
		if (gData.nPickerUseCnt > 4){if (nCnt > 4 && !pDX07->iCapPickerVac05) return FALSE;}
		if (gData.nPickerUseCnt > 5){if (nCnt > 5 && !pDX07->iCapPickerVac06) return FALSE;}
	}
	else if (nNo == 2) {
		if (nCnt > 0 && !pDX07->iCapPickerVac02) return FALSE;
		if (nCnt > 1 && !pDX07->iCapPickerVac03) return FALSE;
		if (gData.nPickerUseCnt > 3){if (nCnt > 2 && !pDX07->iCapPickerVac04) return FALSE;}
		if (gData.nPickerUseCnt > 4){if (nCnt > 3 && !pDX07->iCapPickerVac05) return FALSE;}
		if (gData.nPickerUseCnt > 5){if (nCnt > 4 && !pDX07->iCapPickerVac06) return FALSE;}
	}
	else if (nNo == 3) {
		if (nCnt > 0 && !pDX07->iCapPickerVac03) return FALSE;
		if (gData.nPickerUseCnt > 3){if (nCnt > 1 && !pDX07->iCapPickerVac04) return FALSE;}
		if (gData.nPickerUseCnt > 4){if (nCnt > 2 && !pDX07->iCapPickerVac05) return FALSE;}
		if (gData.nPickerUseCnt > 5){if (nCnt > 3 && !pDX07->iCapPickerVac06) return FALSE;}
	}
	else if (nNo == 4) {
		if (gData.nPickerUseCnt > 3){if (nCnt > 0 && !pDX07->iCapPickerVac04) return FALSE;}
		if (gData.nPickerUseCnt > 4){if (nCnt > 1 && !pDX07->iCapPickerVac05) return FALSE;}
		if (gData.nPickerUseCnt > 5){if (nCnt > 2 && !pDX07->iCapPickerVac06) return FALSE;}
	}
	else if (nNo == 5) {
		if (gData.nPickerUseCnt > 4){if (nCnt > 0 && !pDX07->iCapPickerVac05) return FALSE;}
		if (gData.nPickerUseCnt > 5){if (nCnt > 1 && !pDX07->iCapPickerVac06) return FALSE;}
	}
	else if (nNo == 6) {
		if (gData.nPickerUseCnt > 5){if (nCnt > 0 && !pDX07->iCapPickerVac06) return FALSE;}
	}

	return TRUE;
}

void CCommon::Set_CapPickerVacOffMulti(int nNo, int nCnt)
{
	DY_DATA_07 *pDY07 = g_objAJinAXL.Get_pDY07();

	if (nNo == 1) {
		if (nCnt > 0) { pDY07->oCapPickerVac01 = FALSE; pDY07->oCapPickerAir01 = TRUE; }
		if (nCnt > 1) { pDY07->oCapPickerVac02 = FALSE; pDY07->oCapPickerAir02 = TRUE; }
		if (nCnt > 2) { pDY07->oCapPickerVac03 = FALSE; pDY07->oCapPickerAir03 = TRUE; }
		if (nCnt > 3) { pDY07->oCapPickerVac04 = FALSE; pDY07->oCapPickerAir04 = TRUE; }
		if (nCnt > 4) { pDY07->oCapPickerVac05 = FALSE; pDY07->oCapPickerAir05 = TRUE; }
		if (nCnt > 5) { pDY07->oCapPickerVac06 = FALSE; pDY07->oCapPickerAir06 = TRUE; }
	}
	else if (nNo == 2) {
		if (nCnt > 0) { pDY07->oCapPickerVac02 = FALSE; pDY07->oCapPickerAir02 = TRUE; }
		if (nCnt > 1) { pDY07->oCapPickerVac03 = FALSE; pDY07->oCapPickerAir03 = TRUE; }
		if (nCnt > 2) { pDY07->oCapPickerVac04 = FALSE; pDY07->oCapPickerAir04 = TRUE; }
		if (nCnt > 3) { pDY07->oCapPickerVac05 = FALSE; pDY07->oCapPickerAir05 = TRUE; }
		if (nCnt > 4) { pDY07->oCapPickerVac06 = FALSE; pDY07->oCapPickerAir06 = TRUE; }
	}
	else if (nNo == 3) {
		if (nCnt > 0) { pDY07->oCapPickerVac03 = FALSE; pDY07->oCapPickerAir03 = TRUE; }
		if (nCnt > 1) { pDY07->oCapPickerVac04 = FALSE; pDY07->oCapPickerAir04 = TRUE; }
		if (nCnt > 2) { pDY07->oCapPickerVac05 = FALSE; pDY07->oCapPickerAir05 = TRUE; }
		if (nCnt > 3) { pDY07->oCapPickerVac06 = FALSE; pDY07->oCapPickerAir06 = TRUE; }
	}
	else if (nNo == 4) {
		if (nCnt > 0) { pDY07->oCapPickerVac04 = FALSE; pDY07->oCapPickerAir04 = TRUE; }
		if (nCnt > 1) { pDY07->oCapPickerVac05 = FALSE; pDY07->oCapPickerAir05 = TRUE; }
		if (nCnt > 2) { pDY07->oCapPickerVac06 = FALSE; pDY07->oCapPickerAir06 = TRUE; }
	}
	else if (nNo == 5) {
		if (nCnt > 0) { pDY07->oCapPickerVac05 = FALSE; pDY07->oCapPickerAir05 = TRUE; }
		if (nCnt > 1) { pDY07->oCapPickerVac06 = FALSE; pDY07->oCapPickerAir06 = TRUE; }
	}
	else if (nNo == 6) {
		if (nCnt > 0) { pDY07->oCapPickerVac06 = FALSE; pDY07->oCapPickerAir06 = TRUE; }
	}

	g_objAJinAXL.Write_Output(8);
}

BOOL CCommon::Get_CapPickerVacOffMulti(int nNo, int nCnt)
{
	DX_DATA_07 *pDX07 = g_objAJinAXL.Get_pDX07();

	if (nNo == 1) {
		if (nCnt > 0 && pDX07->iCapPickerVac01) return FALSE;
		if (nCnt > 1 && pDX07->iCapPickerVac02) return FALSE;
		if (nCnt > 2 && pDX07->iCapPickerVac03) return FALSE;
		if (gData.nPickerUseCnt > 3) {if (nCnt > 3 && pDX07->iCapPickerVac04) return FALSE;}
		if (gData.nPickerUseCnt > 4) {if (nCnt > 4 && pDX07->iCapPickerVac05) return FALSE;}
		if (gData.nPickerUseCnt > 5) {if (nCnt > 5 && pDX07->iCapPickerVac06) return FALSE;}
	}
	else if (nNo == 2) {
		if (nCnt > 0 && pDX07->iCapPickerVac02) return FALSE;
		if (nCnt > 1 && pDX07->iCapPickerVac03) return FALSE;
		if (gData.nPickerUseCnt > 3) {if (nCnt > 2 && pDX07->iCapPickerVac04) return FALSE;}
		if (gData.nPickerUseCnt > 4) {if (nCnt > 3 && pDX07->iCapPickerVac05) return FALSE;}
		if (gData.nPickerUseCnt > 5) {if (nCnt > 4 && pDX07->iCapPickerVac06) return FALSE;}
	}
	else if (nNo == 3) {
		if (nCnt > 0 && pDX07->iCapPickerVac03) return FALSE;
		if (gData.nPickerUseCnt > 3){if (nCnt > 1 && pDX07->iCapPickerVac04) return FALSE;}
		if (gData.nPickerUseCnt > 4){if (nCnt > 2 && pDX07->iCapPickerVac05) return FALSE;}
		if (gData.nPickerUseCnt > 5){if (nCnt > 3 && pDX07->iCapPickerVac06) return FALSE;}
	}
	else if (nNo == 4) {
		if (gData.nPickerUseCnt > 3){if (nCnt > 0 && pDX07->iCapPickerVac04) return FALSE;}
		if (gData.nPickerUseCnt > 4){if (nCnt > 1 && pDX07->iCapPickerVac05) return FALSE;}
		if (gData.nPickerUseCnt > 5){if (nCnt > 2 && pDX07->iCapPickerVac06) return FALSE;}
	}
	else if (nNo == 5) {
		if (gData.nPickerUseCnt > 4){if (nCnt > 0 && pDX07->iCapPickerVac05) return FALSE;}
		if (gData.nPickerUseCnt > 5){if (nCnt > 1 && pDX07->iCapPickerVac06) return FALSE;}
	}
	else if (nNo == 6) {
		if (gData.nPickerUseCnt > 5){if (nCnt > 0 && pDX07->iCapPickerVac06) return FALSE;}
	}

	return TRUE;
}

void CCommon::Set_CapPickerAirOffMulti(int nNo, int nCnt)
{
	DY_DATA_07 *pDY07 = g_objAJinAXL.Get_pDY07();

	if (nNo == 1) {
		if (nCnt > 0) pDY07->oCapPickerAir01 = FALSE;
		if (nCnt > 1) pDY07->oCapPickerAir02 = FALSE;
		if (nCnt > 2) pDY07->oCapPickerAir03 = FALSE;
		if (nCnt > 3) pDY07->oCapPickerAir04 = FALSE;
		if (nCnt > 4) pDY07->oCapPickerAir05 = FALSE;
		if (nCnt > 5) pDY07->oCapPickerAir06 = FALSE;
	}
	else if (nNo == 2) {
		if (nCnt > 0) pDY07->oCapPickerAir02 = FALSE;
		if (nCnt > 1) pDY07->oCapPickerAir03 = FALSE;
		if (nCnt > 2) pDY07->oCapPickerAir04 = FALSE;
		if (nCnt > 3) pDY07->oCapPickerAir05 = FALSE;
		if (nCnt > 4) pDY07->oCapPickerAir06 = FALSE;
	}
	else if (nNo == 3) {
		if (nCnt > 0) pDY07->oCapPickerAir03 = FALSE;
		if (nCnt > 1) pDY07->oCapPickerAir04 = FALSE;
		if (nCnt > 2) pDY07->oCapPickerAir05 = FALSE;
		if (nCnt > 3) pDY07->oCapPickerAir06 = FALSE;
	}
	else if (nNo == 4) {
		if (nCnt > 0) pDY07->oCapPickerAir04 = FALSE;
		if (nCnt > 1) pDY07->oCapPickerAir05 = FALSE;
		if (nCnt > 2) pDY07->oCapPickerAir06 = FALSE;
	}
	else if (nNo == 5) {
		if (nCnt > 0) pDY07->oCapPickerAir05 = FALSE;
		if (nCnt > 1) pDY07->oCapPickerAir06 = FALSE;
	}
	else if (nNo == 6) {
		if (nCnt > 0) pDY07->oCapPickerAir06 = FALSE;
	}

	g_objAJinAXL.Write_Output(7);
}

//	7. Cap Buffer I/O 함수
void CCommon::Set_CapBufferAlign(BOOL bOn)
{
	DY_DATA_09 *pDY09 = g_objAJinAXL.Get_pDY09();
	pDY09->oCapBufferAlignIn = bOn;

	g_objAJinAXL.Write_Output(9);
}

BOOL CCommon::Get_CapBufferAlign(BOOL bOn)
{
	DX_DATA_09 *pDX09 = g_objAJinAXL.Get_pDX09();
	
	if (bOn && pDX09->iCapBufferStageAlignIn && !pDX09->iCapBufferStageAlignOut) return TRUE;
	if (!bOn && !pDX09->iCapBufferStageAlignIn && pDX09->iCapBufferStageAlignOut) return TRUE;

	return FALSE;	
}

void CCommon::Set_CapCleanAirOn()
{
	DY_DATA_09 *pDY09 = g_objAJinAXL.Get_pDY09();
	pDY09->oCapCleanAir01 = TRUE;
	pDY09->oCapCleanAir02 = TRUE;
	pDY09->oCapCleanAir03 = TRUE;
	pDY09->oCapCleanAir04 = TRUE;
	pDY09->oCapCleanAir05 = TRUE;
	pDY09->oCapCleanAir06 = TRUE;
	pDY09->oCapCleanSuction = TRUE;

	g_objAJinAXL.Write_Output(9);
}

void CCommon::Set_CapCleanAirOff()
{
	DY_DATA_09 *pDY09 = g_objAJinAXL.Get_pDY09();
	pDY09->oCapCleanAir01 = FALSE;
	pDY09->oCapCleanAir02 = FALSE;
	pDY09->oCapCleanAir03 = FALSE;
	pDY09->oCapCleanAir04 = FALSE;
	pDY09->oCapCleanAir05 = FALSE;
	pDY09->oCapCleanAir06 = FALSE;
	pDY09->oCapCleanSuction = FALSE;

	g_objAJinAXL.Write_Output(9);
}

// 	8. Assembly Picker I/O 함수
void CCommon::Set_AssyPickerUp(int nNo)
{
	DY_DATA_08 *pDY08 = g_objAJinAXL.Get_pDY08();
	if (nNo == 0 || nNo == 1) pDY08->oAssyPickerDown01 = FALSE;
	if (nNo == 0 || nNo == 2) pDY08->oAssyPickerDown02 = FALSE;
	if (nNo == 0 || nNo == 3) pDY08->oAssyPickerDown03 = FALSE;
	if (nNo == 0 || nNo == 4) pDY08->oAssyPickerDown04 = FALSE;
	if (nNo == 0 || nNo == 5) pDY08->oAssyPickerDown05 = FALSE;
	if (nNo == 0 || nNo == 6) pDY08->oAssyPickerDown06 = FALSE;
	g_objAJinAXL.Write_Output(8);
}

BOOL CCommon::Get_AssyPickerUp(int nNo)
{
	DX_DATA_08 *pDX08 = g_objAJinAXL.Get_pDX08();
	if (nNo == 0) {
		if (gData.nPickerUseCnt == 6) {
			if (!pDX08->iAssyPickerUp01 || pDX08->iAssyPickerDown01) { gAlm.nAssyPickNo = 1; return FALSE; }
			if (!pDX08->iAssyPickerUp02 || pDX08->iAssyPickerDown02) { gAlm.nAssyPickNo = 2; return FALSE; }
			if (!pDX08->iAssyPickerUp03 || pDX08->iAssyPickerDown03) { gAlm.nAssyPickNo = 3; return FALSE; }
			if (!pDX08->iAssyPickerUp04 || pDX08->iAssyPickerDown04) { gAlm.nAssyPickNo = 4; return FALSE; }
			if (!pDX08->iAssyPickerUp05 || pDX08->iAssyPickerDown05) { gAlm.nAssyPickNo = 5; return FALSE; }
			if (!pDX08->iAssyPickerUp06 || pDX08->iAssyPickerDown06) { gAlm.nAssyPickNo = 6; return FALSE; }
		}
		else if (gData.nPickerUseCnt == 5) {
			if (!pDX08->iAssyPickerUp01 || pDX08->iAssyPickerDown01) { gAlm.nAssyPickNo = 1; return FALSE; }
			if (!pDX08->iAssyPickerUp02 || pDX08->iAssyPickerDown02) { gAlm.nAssyPickNo = 2; return FALSE; }
			if (!pDX08->iAssyPickerUp03 || pDX08->iAssyPickerDown03) { gAlm.nAssyPickNo = 3; return FALSE; }
			if (!pDX08->iAssyPickerUp04 || pDX08->iAssyPickerDown04) { gAlm.nAssyPickNo = 4; return FALSE; }
			if (!pDX08->iAssyPickerUp05 || pDX08->iAssyPickerDown05) { gAlm.nAssyPickNo = 5; return FALSE; }
		}
		else if (gData.nPickerUseCnt == 4) {
			if (!pDX08->iAssyPickerUp01 || pDX08->iAssyPickerDown01) { gAlm.nAssyPickNo = 1; return FALSE; }
			if (!pDX08->iAssyPickerUp02 || pDX08->iAssyPickerDown02) { gAlm.nAssyPickNo = 2; return FALSE; }
			if (!pDX08->iAssyPickerUp03 || pDX08->iAssyPickerDown03) { gAlm.nAssyPickNo = 3; return FALSE; }
			if (!pDX08->iAssyPickerUp04 || pDX08->iAssyPickerDown04) { gAlm.nAssyPickNo = 4; return FALSE; }
		}
		else if (gData.nPickerUseCnt == 3) {
			if (!pDX08->iAssyPickerUp01 || pDX08->iAssyPickerDown01) { gAlm.nAssyPickNo = 1; return FALSE; }
			if (!pDX08->iAssyPickerUp02 || pDX08->iAssyPickerDown02) { gAlm.nAssyPickNo = 2; return FALSE; }
			if (!pDX08->iAssyPickerUp03 || pDX08->iAssyPickerDown03) { gAlm.nAssyPickNo = 3; return FALSE; }
		}
	}
	else if (nNo ==  1 && (!pDX08->iAssyPickerUp01 || pDX08->iAssyPickerDown01)) { gAlm.nAssyPickNo = 1; return FALSE; }
	else if (nNo ==  2 && (!pDX08->iAssyPickerUp02 || pDX08->iAssyPickerDown02)) { gAlm.nAssyPickNo = 2; return FALSE; }
	else if (nNo ==  3 && (!pDX08->iAssyPickerUp03 || pDX08->iAssyPickerDown03)) { gAlm.nAssyPickNo = 3; return FALSE; }
	else if (nNo ==  4 && (!pDX08->iAssyPickerUp04 || pDX08->iAssyPickerDown04)) { gAlm.nAssyPickNo = 4; return FALSE; }
	else if (nNo ==  5 && (!pDX08->iAssyPickerUp05 || pDX08->iAssyPickerDown05)) { gAlm.nAssyPickNo = 5; return FALSE; }
	else if (nNo ==  6 && (!pDX08->iAssyPickerUp06 || pDX08->iAssyPickerDown06)) { gAlm.nAssyPickNo = 6; return FALSE; }
	return TRUE;
}

void CCommon::Set_AssyPickerDown(int nNo)
{
	DY_DATA_08 *pDY08 = g_objAJinAXL.Get_pDY08();
	if (nNo == 0 || nNo == 1) pDY08->oAssyPickerDown01 = TRUE;
	if (nNo == 0 || nNo == 2) pDY08->oAssyPickerDown02 = TRUE;
	if (nNo == 0 || nNo == 3) pDY08->oAssyPickerDown03 = TRUE;
	if (nNo == 0 || nNo == 4) pDY08->oAssyPickerDown04 = TRUE;
	if (nNo == 0 || nNo == 5) pDY08->oAssyPickerDown05 = TRUE;
	if (nNo == 0 || nNo == 6) pDY08->oAssyPickerDown06 = TRUE;
	g_objAJinAXL.Write_Output(8);
}

BOOL CCommon::Get_AssyPickerDown(int nNo)
{
	DX_DATA_08 *pDX08 = g_objAJinAXL.Get_pDX08();
	if (nNo == 0) {
		if (gData.nPickerUseCnt == 6) {
			if (pDX08->iAssyPickerUp01 || !pDX08->iAssyPickerDown01) { gAlm.nAssyPickNo = 1; return FALSE; }
			if (pDX08->iAssyPickerUp02 || !pDX08->iAssyPickerDown02) { gAlm.nAssyPickNo = 2; return FALSE; }
			if (pDX08->iAssyPickerUp03 || !pDX08->iAssyPickerDown03) { gAlm.nAssyPickNo = 3; return FALSE; }
			if (pDX08->iAssyPickerUp04 || !pDX08->iAssyPickerDown04) { gAlm.nAssyPickNo = 4; return FALSE; }
			if (pDX08->iAssyPickerUp05 || !pDX08->iAssyPickerDown05) { gAlm.nAssyPickNo = 5; return FALSE; }
			if (pDX08->iAssyPickerUp06 || !pDX08->iAssyPickerDown06) { gAlm.nAssyPickNo = 6; return FALSE; }
		}
		else if (gData.nPickerUseCnt == 5) {
			if (pDX08->iAssyPickerUp01 || !pDX08->iAssyPickerDown01) { gAlm.nAssyPickNo = 1; return FALSE; }
			if (pDX08->iAssyPickerUp02 || !pDX08->iAssyPickerDown02) { gAlm.nAssyPickNo = 2; return FALSE; }
			if (pDX08->iAssyPickerUp03 || !pDX08->iAssyPickerDown03) { gAlm.nAssyPickNo = 3; return FALSE; }
			if (pDX08->iAssyPickerUp04 || !pDX08->iAssyPickerDown04) { gAlm.nAssyPickNo = 4; return FALSE; }
			if (pDX08->iAssyPickerUp05 || !pDX08->iAssyPickerDown05) { gAlm.nAssyPickNo = 5; return FALSE; }
		}
		else if (gData.nPickerUseCnt == 4) {
			if (pDX08->iAssyPickerUp01 || !pDX08->iAssyPickerDown01) { gAlm.nAssyPickNo = 1; return FALSE; }
			if (pDX08->iAssyPickerUp02 || !pDX08->iAssyPickerDown02) { gAlm.nAssyPickNo = 2; return FALSE; }
			if (pDX08->iAssyPickerUp03 || !pDX08->iAssyPickerDown03) { gAlm.nAssyPickNo = 3; return FALSE; }
			if (pDX08->iAssyPickerUp04 || !pDX08->iAssyPickerDown04) { gAlm.nAssyPickNo = 4; return FALSE; }
		}
		else if (gData.nPickerUseCnt == 3) {
			if (pDX08->iAssyPickerUp01 || !pDX08->iAssyPickerDown01) { gAlm.nAssyPickNo = 1; return FALSE; }
			if (pDX08->iAssyPickerUp02 || !pDX08->iAssyPickerDown02) { gAlm.nAssyPickNo = 2; return FALSE; }
			if (pDX08->iAssyPickerUp03 || !pDX08->iAssyPickerDown03) { gAlm.nAssyPickNo = 3; return FALSE; }
		}
	}
	else if (nNo ==  1 && (pDX08->iAssyPickerUp01 || !pDX08->iAssyPickerDown01)) { gAlm.nAssyPickNo = 1; return FALSE; }
	else if (nNo ==  2 && (pDX08->iAssyPickerUp02 || !pDX08->iAssyPickerDown02)) { gAlm.nAssyPickNo = 2; return FALSE; }
	else if (nNo ==  3 && (pDX08->iAssyPickerUp03 || !pDX08->iAssyPickerDown03)) { gAlm.nAssyPickNo = 3; return FALSE; }
	else if (nNo ==  4 && (pDX08->iAssyPickerUp04 || !pDX08->iAssyPickerDown04)) { gAlm.nAssyPickNo = 4; return FALSE; }
	else if (nNo ==  5 && (pDX08->iAssyPickerUp05 || !pDX08->iAssyPickerDown05)) { gAlm.nAssyPickNo = 5; return FALSE; }
	else if (nNo ==  6 && (pDX08->iAssyPickerUp06 || !pDX08->iAssyPickerDown06)) { gAlm.nAssyPickNo = 6; return FALSE; }
	return TRUE;
}

void CCommon::Set_AssyPickerVacOff(int nNo)
{
	DY_DATA_08 *pDY08 = g_objAJinAXL.Get_pDY08();
	if (nNo == 0 || nNo == 1) { pDY08->oAssyPickerVac01 = FALSE; pDY08->oAssyPickerAir01 = TRUE; }
	if (nNo == 0 || nNo == 2) { pDY08->oAssyPickerVac02 = FALSE; pDY08->oAssyPickerAir02 = TRUE; }
	if (nNo == 0 || nNo == 3) { pDY08->oAssyPickerVac03 = FALSE; pDY08->oAssyPickerAir03 = TRUE; }
	if (nNo == 0 || nNo == 4) { pDY08->oAssyPickerVac04 = FALSE; pDY08->oAssyPickerAir04 = TRUE; }
	if (nNo == 0 || nNo == 5) { pDY08->oAssyPickerVac05 = FALSE; pDY08->oAssyPickerAir05 = TRUE; }
	if (nNo == 0 || nNo == 6) { pDY08->oAssyPickerVac06 = FALSE; pDY08->oAssyPickerAir06 = TRUE; }
	g_objAJinAXL.Write_Output(8);
}

BOOL CCommon::Get_AssyPickerVacOff(int nNo)
{
	DX_DATA_08 *pDX08 = g_objAJinAXL.Get_pDX08();
	if (nNo == 0) {
		if (!pDX08->iAssyPickerVac01 &&
			!pDX08->iAssyPickerVac02 &&
			!pDX08->iAssyPickerVac03 &&
			!pDX08->iAssyPickerVac04 &&
			!pDX08->iAssyPickerVac05 &&
			!pDX08->iAssyPickerVac06) return TRUE;
	}
	else if (nNo ==  1 && !pDX08->iAssyPickerVac01) return TRUE;
	else if (nNo ==  2 && !pDX08->iAssyPickerVac02) return TRUE;
	else if (nNo ==  3 && !pDX08->iAssyPickerVac03) return TRUE;
	else if (nNo ==  4 && !pDX08->iAssyPickerVac04) return TRUE;
	else if (nNo ==  5 && !pDX08->iAssyPickerVac05) return TRUE;
	else if (nNo ==  6 && !pDX08->iAssyPickerVac06) return TRUE;
	return FALSE;
}

void CCommon::Set_AssyPickerAirOff(int nNo)
{
	DY_DATA_08 *pDY08 = g_objAJinAXL.Get_pDY08();
	if (nNo == 0 || nNo == 1) pDY08->oAssyPickerAir01 = FALSE;
	if (nNo == 0 || nNo == 2) pDY08->oAssyPickerAir02 = FALSE;
	if (nNo == 0 || nNo == 3) pDY08->oAssyPickerAir03 = FALSE;
	if (nNo == 0 || nNo == 4) pDY08->oAssyPickerAir04 = FALSE;
	if (nNo == 0 || nNo == 5) pDY08->oAssyPickerAir05 = FALSE;
	if (nNo == 0 || nNo == 6) pDY08->oAssyPickerAir06 = FALSE;
	g_objAJinAXL.Write_Output(8);
}

// 	9. Trans Stage I/O 함수
void CCommon::Set_TransStageVacOff(int nNo)
{
	DY_DATA_05 *pDY05 = g_objAJinAXL.Get_pDY05();
	if (nNo == 0 || nNo == 1) { pDY05->oTransStageVac01 = FALSE; pDY05->oTransStageAir01 = TRUE; }
	if (nNo == 0 || nNo == 2) { pDY05->oTransStageVac02 = FALSE; pDY05->oTransStageAir02 = TRUE; }
	if (nNo == 0 || nNo == 3) { pDY05->oTransStageVac03 = FALSE; pDY05->oTransStageAir03 = TRUE; }
	if (nNo == 0 || nNo == 4) { pDY05->oTransStageVac04 = FALSE; pDY05->oTransStageAir04 = TRUE; }
	if (nNo == 0 || nNo == 5) { pDY05->oTransStageVac05 = FALSE; pDY05->oTransStageAir05 = TRUE; }
	if (nNo == 0 || nNo == 6) { pDY05->oTransStageVac06 = FALSE; pDY05->oTransStageAir06 = TRUE; }
	g_objAJinAXL.Write_Output(5);
}

BOOL CCommon::Get_TransStageVacOff(int nNo)
{
	DX_DATA_05 *pDX05 = g_objAJinAXL.Get_pDX05();
	if (nNo == 0) {
		if (!pDX05->iTransStageVac01 &&
			!pDX05->iTransStageVac02 &&
			!pDX05->iTransStageVac03 &&
			!pDX05->iTransStageVac04 &&
			!pDX05->iTransStageVac05 &&
			!pDX05->iTransStageVac06) return TRUE;
	}
	else if (nNo ==  1 && !pDX05->iTransStageVac01) return TRUE;
	else if (nNo ==  2 && !pDX05->iTransStageVac02) return TRUE;
	else if (nNo ==  3 && !pDX05->iTransStageVac03) return TRUE;
	else if (nNo ==  4 && !pDX05->iTransStageVac04) return TRUE;
	else if (nNo ==  5 && !pDX05->iTransStageVac05) return TRUE;
	else if (nNo ==  6 && !pDX05->iTransStageVac06) return TRUE;
	return FALSE;
}

void CCommon::Set_TransStageAirOff(int nNo)
{
	DY_DATA_05 *pDY05 = g_objAJinAXL.Get_pDY05();
	if (nNo == 0 || nNo == 1) pDY05->oTransStageAir01 = FALSE;
	if (nNo == 0 || nNo == 2) pDY05->oTransStageAir02 = FALSE;
	if (nNo == 0 || nNo == 3) pDY05->oTransStageAir03 = FALSE;
	if (nNo == 0 || nNo == 4) pDY05->oTransStageAir04 = FALSE;
	if (nNo == 0 || nNo == 5) pDY05->oTransStageAir05 = FALSE;
	if (nNo == 0 || nNo == 6) pDY05->oTransStageAir06 = FALSE;
	g_objAJinAXL.Write_Output(5);
}

void CCommon::Set_TransStageClampOn()
{
	DY_DATA_05 *pDY05 = g_objAJinAXL.Get_pDY05();
	pDY05->oTransStageClamp1On = TRUE; pDY05->oTransStageClamp1Off = FALSE;
	pDY05->oTransStageClamp2On = TRUE; pDY05->oTransStageClamp2Off = FALSE;	
	g_objAJinAXL.Write_Output(5);
}

BOOL CCommon::Get_TransStageClampOn()
{
	DX_DATA_05 *pDX05 = g_objAJinAXL.Get_pDX05();
	if (pDX05->iTransStageClamp1On && !pDX05->iTransStageClamp1Off &&
		pDX05->iTransStageClamp2On && !pDX05->iTransStageClamp2Off) return TRUE;
	return FALSE;
}

void CCommon::Set_TransStageClampOff()
{
	DY_DATA_05 *pDY05 = g_objAJinAXL.Get_pDY05();
	pDY05->oTransStageClamp1On = FALSE; pDY05->oTransStageClamp1Off = TRUE;
	pDY05->oTransStageClamp2On = FALSE; pDY05->oTransStageClamp2Off = TRUE;	
	g_objAJinAXL.Write_Output(5);
}


BOOL CCommon::Get_TransStageClampOff()
{
	DX_DATA_05 *pDX05 = g_objAJinAXL.Get_pDX05();
	if (!pDX05->iTransStageClamp1On && pDX05->iTransStageClamp1Off &&
		!pDX05->iTransStageClamp2On && pDX05->iTransStageClamp2Off) return TRUE;
	return FALSE;
}

//	10. Unload Picker I/O 함수
void CCommon::Set_UnloadPickerUp(int nNo)
{
	DY_DATA_10 *pDY10 = g_objAJinAXL.Get_pDY10();
	if (nNo == 0 || nNo == 1) pDY10->oUnloadPickerDown01 = FALSE;
	if (nNo == 0 || nNo == 2) pDY10->oUnloadPickerDown02 = FALSE;
	if (nNo == 0 || nNo == 3) pDY10->oUnloadPickerDown03 = FALSE;
	if (nNo == 0 || nNo == 4) pDY10->oUnloadPickerDown04 = FALSE;
	if (nNo == 0 || nNo == 5) pDY10->oUnloadPickerDown05 = FALSE;
	if (nNo == 0 || nNo == 6) pDY10->oUnloadPickerDown06 = FALSE;
	g_objAJinAXL.Write_Output(10);
}

BOOL CCommon::Get_UnloadPickerUp(int nNo)
{
	DX_DATA_10 *pDX10 = g_objAJinAXL.Get_pDX10();
	if (nNo == 0) {
		if (gData.nPickerUseCnt == 6) {
			if (!pDX10->iUnloadPickerUp01 || pDX10->iUnloadPickerDown01) { gAlm.nUNPickerNo = 1; return FALSE; }
			if (!pDX10->iUnloadPickerUp02 || pDX10->iUnloadPickerDown02) { gAlm.nUNPickerNo = 2; return FALSE; }
			if (!pDX10->iUnloadPickerUp03 || pDX10->iUnloadPickerDown03) { gAlm.nUNPickerNo = 3; return FALSE; }
			if (!pDX10->iUnloadPickerUp04 || pDX10->iUnloadPickerDown04) { gAlm.nUNPickerNo = 4; return FALSE; }
			if (!pDX10->iUnloadPickerUp05 || pDX10->iUnloadPickerDown05) { gAlm.nUNPickerNo = 5; return FALSE; }
			if (!pDX10->iUnloadPickerUp06 || pDX10->iUnloadPickerDown06) { gAlm.nUNPickerNo = 6; return FALSE; }
		} else if (gData.nPickerUseCnt == 5) {
			if (!pDX10->iUnloadPickerUp01 || pDX10->iUnloadPickerDown01) { gAlm.nUNPickerNo = 1; return FALSE; }
			if (!pDX10->iUnloadPickerUp02 || pDX10->iUnloadPickerDown02) { gAlm.nUNPickerNo = 2; return FALSE; }
			if (!pDX10->iUnloadPickerUp03 || pDX10->iUnloadPickerDown03) { gAlm.nUNPickerNo = 3; return FALSE; }
			if (!pDX10->iUnloadPickerUp04 || pDX10->iUnloadPickerDown04) { gAlm.nUNPickerNo = 4; return FALSE; }
			if (!pDX10->iUnloadPickerUp05 || pDX10->iUnloadPickerDown05) { gAlm.nUNPickerNo = 5; return FALSE; }
		} else if (gData.nPickerUseCnt == 4) {
			if (!pDX10->iUnloadPickerUp01 || pDX10->iUnloadPickerDown01) { gAlm.nUNPickerNo = 1; return FALSE; }
			if (!pDX10->iUnloadPickerUp02 || pDX10->iUnloadPickerDown02) { gAlm.nUNPickerNo = 2; return FALSE; }
			if (!pDX10->iUnloadPickerUp03 || pDX10->iUnloadPickerDown03) { gAlm.nUNPickerNo = 3; return FALSE; }
			if (!pDX10->iUnloadPickerUp04 || pDX10->iUnloadPickerDown04) { gAlm.nUNPickerNo = 4; return FALSE; }
		} else if (gData.nPickerUseCnt == 3) {
			if (!pDX10->iUnloadPickerUp01 || pDX10->iUnloadPickerDown01) { gAlm.nUNPickerNo = 1; return FALSE; }
			if (!pDX10->iUnloadPickerUp02 || pDX10->iUnloadPickerDown02) { gAlm.nUNPickerNo = 2; return FALSE; }
			if (!pDX10->iUnloadPickerUp03 || pDX10->iUnloadPickerDown03) { gAlm.nUNPickerNo = 3; return FALSE; }
		}		

	}
	else if (nNo ==  1 && (!pDX10->iUnloadPickerUp01 || pDX10->iUnloadPickerDown01)) { gAlm.nUNPickerNo = 1; return FALSE; }
	else if (nNo ==  2 && (!pDX10->iUnloadPickerUp02 || pDX10->iUnloadPickerDown02)) { gAlm.nUNPickerNo = 2; return FALSE; }
	else if (nNo ==  3 && (!pDX10->iUnloadPickerUp03 || pDX10->iUnloadPickerDown03)) { gAlm.nUNPickerNo = 3; return FALSE; }
	else if (nNo ==  4 && (!pDX10->iUnloadPickerUp04 || pDX10->iUnloadPickerDown04)) { gAlm.nUNPickerNo = 4; return FALSE; }
	else if (nNo ==  5 && (!pDX10->iUnloadPickerUp05 || pDX10->iUnloadPickerDown05)) { gAlm.nUNPickerNo = 5; return FALSE; }
	else if (nNo ==  6 && (!pDX10->iUnloadPickerUp06 || pDX10->iUnloadPickerDown06)) { gAlm.nUNPickerNo = 6; return FALSE; }
	return TRUE;
}

void CCommon::Set_UnloadPickerVacOff(int nNo)
{
#ifdef DRY_RUN_TEST
	return;
#endif
	DY_DATA_10 *pDY10 = g_objAJinAXL.Get_pDY10();
	if (nNo == 0 || nNo == 1) { pDY10->oUnloadPickerVac01 = FALSE; pDY10->oUnloadPickerAir01 = TRUE; }
	if (nNo == 0 || nNo == 2) { pDY10->oUnloadPickerVac02 = FALSE; pDY10->oUnloadPickerAir02 = TRUE; }
	if (nNo == 0 || nNo == 3) { pDY10->oUnloadPickerVac03 = FALSE; pDY10->oUnloadPickerAir03 = TRUE; }
	if (nNo == 0 || nNo == 4) { pDY10->oUnloadPickerVac04 = FALSE; pDY10->oUnloadPickerAir04 = TRUE; }
	if (nNo == 0 || nNo == 5) { pDY10->oUnloadPickerVac05 = FALSE; pDY10->oUnloadPickerAir05 = TRUE; }
	if (nNo == 0 || nNo == 6) { pDY10->oUnloadPickerVac06 = FALSE; pDY10->oUnloadPickerAir06 = TRUE; }
	g_objAJinAXL.Write_Output(10);
}

BOOL CCommon::Get_UnloadPickerVacOff(int nNo)
{
	DX_DATA_10 *pDX10 = g_objAJinAXL.Get_pDX10();
	if (nNo == 0) {
		if (!pDX10->iUnloadPickerVac01 &&
			!pDX10->iUnloadPickerVac02 &&
			!pDX10->iUnloadPickerVac03 &&
			!pDX10->iUnloadPickerVac04 &&
			!pDX10->iUnloadPickerVac05 &&
			!pDX10->iUnloadPickerVac06) return TRUE;
	}
	else if (nNo ==  1 && !pDX10->iUnloadPickerVac01) return TRUE;
	else if (nNo ==  2 && !pDX10->iUnloadPickerVac02) return TRUE;
	else if (nNo ==  3 && !pDX10->iUnloadPickerVac03) return TRUE;
	else if (nNo ==  4 && !pDX10->iUnloadPickerVac04) return TRUE;
	else if (nNo ==  5 && !pDX10->iUnloadPickerVac05) return TRUE;
	else if (nNo ==  6 && !pDX10->iUnloadPickerVac06) return TRUE;
	return FALSE;
}

void CCommon::Set_UnloadPickerAirOff(int nNo)
{
	DY_DATA_10 *pDY10 = g_objAJinAXL.Get_pDY10();
	if (nNo == 0 || nNo == 1) pDY10->oUnloadPickerAir01 = FALSE;
	if (nNo == 0 || nNo == 2) pDY10->oUnloadPickerAir02 = FALSE;
	if (nNo == 0 || nNo == 3) pDY10->oUnloadPickerAir03 = FALSE;
	if (nNo == 0 || nNo == 4) pDY10->oUnloadPickerAir04 = FALSE;
	if (nNo == 0 || nNo == 5) pDY10->oUnloadPickerAir05 = FALSE;
	if (nNo == 0 || nNo == 6) pDY10->oUnloadPickerAir06 = FALSE;
	g_objAJinAXL.Write_Output(10);
}

void CCommon::Set_UnloadPickerDownMulti(int nNo, int nCnt)
{
	DY_DATA_10 *pDY10 = g_objAJinAXL.Get_pDY10();

	if (nNo == 1) {
		if (nCnt > 0) pDY10->oUnloadPickerDown01 = TRUE;
		if (nCnt > 1) pDY10->oUnloadPickerDown02 = TRUE;
		if (nCnt > 2) pDY10->oUnloadPickerDown03 = TRUE;
		if (nCnt > 3) pDY10->oUnloadPickerDown04 = TRUE;
		if (nCnt > 4) pDY10->oUnloadPickerDown05 = TRUE;
		if (nCnt > 5) pDY10->oUnloadPickerDown06 = TRUE;
	}
	else if (nNo == 2) {
		if (nCnt > 0) pDY10->oUnloadPickerDown02 = TRUE;
		if (nCnt > 1) pDY10->oUnloadPickerDown03 = TRUE;
		if (nCnt > 2) pDY10->oUnloadPickerDown04 = TRUE;
		if (nCnt > 3) pDY10->oUnloadPickerDown05 = TRUE;
		if (nCnt > 4) pDY10->oUnloadPickerDown06 = TRUE;
	}
	else if (nNo == 3) {
		if (nCnt > 0) pDY10->oUnloadPickerDown03 = TRUE;
		if (nCnt > 1) pDY10->oUnloadPickerDown04 = TRUE;
		if (nCnt > 2) pDY10->oUnloadPickerDown05 = TRUE;
		if (nCnt > 3) pDY10->oUnloadPickerDown06 = TRUE;
	}
	else if (nNo == 4) {
		if (nCnt > 0) pDY10->oUnloadPickerDown04 = TRUE;
		if (nCnt > 1) pDY10->oUnloadPickerDown05 = TRUE;
		if (nCnt > 2) pDY10->oUnloadPickerDown06 = TRUE;
	}
	else if (nNo == 5) {
		if (nCnt > 0) pDY10->oUnloadPickerDown05 = TRUE;
		if (nCnt > 1) pDY10->oUnloadPickerDown06 = TRUE;
	}
	else if (nNo == 6) {
		if (nCnt > 0) pDY10->oUnloadPickerDown06 = TRUE;
	}

	g_objAJinAXL.Write_Output(10);
}

BOOL CCommon::Get_UnloadPickerDownMulti(int nNo, int nCnt)
{
	DX_DATA_10 *pDX10 = g_objAJinAXL.Get_pDX10();

	if (nNo == 1) {
		if (nCnt > 0 && (pDX10->iUnloadPickerUp01 || !pDX10->iUnloadPickerDown01)) { gAlm.nUNPickerNo = 1; return FALSE; }
		if (nCnt > 1 && (pDX10->iUnloadPickerUp02 || !pDX10->iUnloadPickerDown02)) { gAlm.nUNPickerNo = 2; return FALSE; }
		if (nCnt > 2 && (pDX10->iUnloadPickerUp03 || !pDX10->iUnloadPickerDown03)) { gAlm.nUNPickerNo = 3; return FALSE; }
		if (gData.nPickerUseCnt > 3){if (nCnt > 3 && (pDX10->iUnloadPickerUp04 || !pDX10->iUnloadPickerDown04)) { gAlm.nUNPickerNo = 4; return FALSE; }}
		if (gData.nPickerUseCnt > 4){if (nCnt > 4 && (pDX10->iUnloadPickerUp05 || !pDX10->iUnloadPickerDown05)) { gAlm.nUNPickerNo = 5; return FALSE; }}
		if (gData.nPickerUseCnt > 5){if (nCnt > 5 && (pDX10->iUnloadPickerUp06 || !pDX10->iUnloadPickerDown06)) { gAlm.nUNPickerNo = 6; return FALSE; }}
	}
	else if (nNo == 2) {
		if (nCnt > 0 && (pDX10->iUnloadPickerUp02 || !pDX10->iUnloadPickerDown02)) { gAlm.nUNPickerNo = 2; return FALSE; }
		if (nCnt > 1 && (pDX10->iUnloadPickerUp03 || !pDX10->iUnloadPickerDown03)) { gAlm.nUNPickerNo = 3; return FALSE; }
		if (gData.nPickerUseCnt > 3){if (nCnt > 2 && (pDX10->iUnloadPickerUp04 || !pDX10->iUnloadPickerDown04)) { gAlm.nUNPickerNo = 4; return FALSE; }}
		if (gData.nPickerUseCnt > 4){if (nCnt > 3 && (pDX10->iUnloadPickerUp05 || !pDX10->iUnloadPickerDown05)) { gAlm.nUNPickerNo = 5; return FALSE; }}
		if (gData.nPickerUseCnt > 5){if (nCnt > 4 && (pDX10->iUnloadPickerUp06 || !pDX10->iUnloadPickerDown06)) { gAlm.nUNPickerNo = 6; return FALSE; }}
	}
	else if (nNo == 3) {
		if (nCnt > 0 && (pDX10->iUnloadPickerUp03 || !pDX10->iUnloadPickerDown03)) { gAlm.nUNPickerNo = 3; return FALSE; }
		if (gData.nPickerUseCnt > 3){if (nCnt > 1 && (pDX10->iUnloadPickerUp04 || !pDX10->iUnloadPickerDown04)) { gAlm.nUNPickerNo = 4; return FALSE; }}
		if (gData.nPickerUseCnt > 4){if (nCnt > 2 && (pDX10->iUnloadPickerUp05 || !pDX10->iUnloadPickerDown05)) { gAlm.nUNPickerNo = 5; return FALSE; }}
		if (gData.nPickerUseCnt > 5){if (nCnt > 3 && (pDX10->iUnloadPickerUp06 || !pDX10->iUnloadPickerDown06)) { gAlm.nUNPickerNo = 6; return FALSE; }}
	}
	else if (nNo == 4) {
		if (gData.nPickerUseCnt > 3){if (nCnt > 0 && (pDX10->iUnloadPickerUp04 || !pDX10->iUnloadPickerDown04)) { gAlm.nUNPickerNo = 4; return FALSE; }}
		if (gData.nPickerUseCnt > 4){if (nCnt > 1 && (pDX10->iUnloadPickerUp05 || !pDX10->iUnloadPickerDown05)) { gAlm.nUNPickerNo = 5; return FALSE; }}
		if (gData.nPickerUseCnt > 5){if (nCnt > 2 && (pDX10->iUnloadPickerUp06 || !pDX10->iUnloadPickerDown06)) { gAlm.nUNPickerNo = 6; return FALSE; }}
	}
	else if (nNo == 5) {
		if (gData.nPickerUseCnt > 4){if (nCnt > 0 && (pDX10->iUnloadPickerUp05 || !pDX10->iUnloadPickerDown05)) { gAlm.nUNPickerNo = 5; return FALSE; }}
		if (gData.nPickerUseCnt > 5){if (nCnt > 1 && (pDX10->iUnloadPickerUp06 || !pDX10->iUnloadPickerDown06)) { gAlm.nUNPickerNo = 6; return FALSE; }}
	}
	else if (nNo == 6) {
		if (gData.nPickerUseCnt > 5) {if (nCnt > 0 && (pDX10->iUnloadPickerUp06 || !pDX10->iUnloadPickerDown06)) { gAlm.nUNPickerNo = 6; return FALSE; }}
	}

	return TRUE;
}

void CCommon::Set_UnloadPickerVacOffMulti(int nNo, int nCnt)
{
#ifdef DRY_RUN_TEST
	return;
#endif
	DY_DATA_10 *pDY10 = g_objAJinAXL.Get_pDY10();

	if (nNo == 1) {
		if (nCnt > 0) { pDY10->oUnloadPickerVac01 = FALSE; pDY10->oUnloadPickerAir01 = TRUE; }
		if (nCnt > 1) { pDY10->oUnloadPickerVac02 = FALSE; pDY10->oUnloadPickerAir02 = TRUE; }
		if (nCnt > 2) { pDY10->oUnloadPickerVac03 = FALSE; pDY10->oUnloadPickerAir03 = TRUE; }
		if (nCnt > 3) { pDY10->oUnloadPickerVac04 = FALSE; pDY10->oUnloadPickerAir04 = TRUE; }
		if (nCnt > 4) { pDY10->oUnloadPickerVac05 = FALSE; pDY10->oUnloadPickerAir05 = TRUE; }
		if (nCnt > 5) { pDY10->oUnloadPickerVac06 = FALSE; pDY10->oUnloadPickerAir06 = TRUE; }
	}
	else if (nNo == 2) {
		if (nCnt > 0) { pDY10->oUnloadPickerVac02 = FALSE; pDY10->oUnloadPickerAir02 = TRUE; }
		if (nCnt > 1) { pDY10->oUnloadPickerVac03 = FALSE; pDY10->oUnloadPickerAir03 = TRUE; }
		if (nCnt > 2) { pDY10->oUnloadPickerVac04 = FALSE; pDY10->oUnloadPickerAir04 = TRUE; }
		if (nCnt > 3) { pDY10->oUnloadPickerVac05 = FALSE; pDY10->oUnloadPickerAir05 = TRUE; }
		if (nCnt > 4) { pDY10->oUnloadPickerVac06 = FALSE; pDY10->oUnloadPickerAir06 = TRUE; }
	}
	else if (nNo == 3) {
		if (nCnt > 0) { pDY10->oUnloadPickerVac03 = FALSE; pDY10->oUnloadPickerAir03 = TRUE; }
		if (nCnt > 1) { pDY10->oUnloadPickerVac04 = FALSE; pDY10->oUnloadPickerAir04 = TRUE; }
		if (nCnt > 2) { pDY10->oUnloadPickerVac05 = FALSE; pDY10->oUnloadPickerAir05 = TRUE; }
		if (nCnt > 3) { pDY10->oUnloadPickerVac06 = FALSE; pDY10->oUnloadPickerAir06 = TRUE; }
	}
	else if (nNo == 4) {
		if (nCnt > 0) { pDY10->oUnloadPickerVac04 = FALSE; pDY10->oUnloadPickerAir04 = TRUE; }
		if (nCnt > 1) { pDY10->oUnloadPickerVac05 = FALSE; pDY10->oUnloadPickerAir05 = TRUE; }
		if (nCnt > 2) { pDY10->oUnloadPickerVac06 = FALSE; pDY10->oUnloadPickerAir06 = TRUE; }
	}
	else if (nNo == 5) {
		if (nCnt > 0) { pDY10->oUnloadPickerVac05 = FALSE; pDY10->oUnloadPickerAir05 = TRUE; }
		if (nCnt > 1) { pDY10->oUnloadPickerVac06 = FALSE; pDY10->oUnloadPickerAir06 = TRUE; }
	}
	else if (nNo == 6) {
		if (nCnt > 0) { pDY10->oUnloadPickerVac06 = FALSE; pDY10->oUnloadPickerAir06 = TRUE; }
	}

	g_objAJinAXL.Write_Output(10);
}

BOOL CCommon::Get_UnloadPickerVacOffMulti(int nNo, int nCnt)
{
	DX_DATA_10 *pDX10 = g_objAJinAXL.Get_pDX10();

	if (nNo == 1) {
		if (nCnt > 0 && pDX10->iUnloadPickerVac01) return FALSE;
		if (nCnt > 1 && pDX10->iUnloadPickerVac02) return FALSE;
		if (nCnt > 2 && pDX10->iUnloadPickerVac03) return FALSE;
		if (gData.nPickerUseCnt > 3){if (nCnt > 3 && pDX10->iUnloadPickerVac04) return FALSE;}
		if (gData.nPickerUseCnt > 4){if (nCnt > 4 && pDX10->iUnloadPickerVac05) return FALSE;}
		if (gData.nPickerUseCnt > 5){if (nCnt > 5 && pDX10->iUnloadPickerVac06) return FALSE;}
	}
	else if (nNo == 2) {
		if (nCnt > 0 && pDX10->iUnloadPickerVac02) return FALSE;
		if (nCnt > 1 && pDX10->iUnloadPickerVac03) return FALSE;
		if (gData.nPickerUseCnt > 3){if (nCnt > 2 && pDX10->iUnloadPickerVac04) return FALSE;}
		if (gData.nPickerUseCnt > 4){if (nCnt > 3 && pDX10->iUnloadPickerVac05) return FALSE;}
		if (gData.nPickerUseCnt > 5){if (nCnt > 4 && pDX10->iUnloadPickerVac06) return FALSE;}
	}
	else if (nNo == 3) {
		if (nCnt > 0 && pDX10->iUnloadPickerVac03) return FALSE;
		if (gData.nPickerUseCnt > 3){if (nCnt > 1 && pDX10->iUnloadPickerVac04) return FALSE;}
		if (gData.nPickerUseCnt > 4){if (nCnt > 2 && pDX10->iUnloadPickerVac05) return FALSE;}
		if (gData.nPickerUseCnt > 5){if (nCnt > 3 && pDX10->iUnloadPickerVac06) return FALSE;}
	}
	else if (nNo == 4) {
		if (gData.nPickerUseCnt > 3){if (nCnt > 0 && pDX10->iUnloadPickerVac04) return FALSE;}
		if (gData.nPickerUseCnt > 4){if (nCnt > 1 && pDX10->iUnloadPickerVac05) return FALSE;}
		if (gData.nPickerUseCnt > 5){if (nCnt > 2 && pDX10->iUnloadPickerVac06) return FALSE;}
	}
	else if (nNo == 5) {
		if (gData.nPickerUseCnt > 4){if (nCnt > 0 && pDX10->iUnloadPickerVac05) return FALSE;}
		if (gData.nPickerUseCnt > 5){if (nCnt > 1 && pDX10->iUnloadPickerVac06) return FALSE;}
	}
	else if (nNo == 6) {
		if (gData.nPickerUseCnt > 5){if (nCnt > 0 && pDX10->iUnloadPickerVac06) return FALSE;}
	}

	return TRUE;
}

void CCommon::Set_UnloadPickerAirOffMulti(int nNo, int nCnt)
{
	DY_DATA_10 *pDY10 = g_objAJinAXL.Get_pDY10();

	if (nNo == 1) {
		if (nCnt > 0) pDY10->oUnloadPickerAir01 = FALSE;
		if (nCnt > 1) pDY10->oUnloadPickerAir02 = FALSE;
		if (nCnt > 2) pDY10->oUnloadPickerAir03 = FALSE;
		if (nCnt > 3) pDY10->oUnloadPickerAir04 = FALSE;
		if (nCnt > 4) pDY10->oUnloadPickerAir05 = FALSE;
		if (nCnt > 5) pDY10->oUnloadPickerAir06 = FALSE;
	}
	else if (nNo == 2) {
		if (nCnt > 0) pDY10->oUnloadPickerAir02 = FALSE;
		if (nCnt > 1) pDY10->oUnloadPickerAir03 = FALSE;
		if (nCnt > 2) pDY10->oUnloadPickerAir04 = FALSE;
		if (nCnt > 3) pDY10->oUnloadPickerAir05 = FALSE;
		if (nCnt > 4) pDY10->oUnloadPickerAir06 = FALSE;
	}
	else if (nNo == 3) {
		if (nCnt > 0) pDY10->oUnloadPickerAir03 = FALSE;
		if (nCnt > 1) pDY10->oUnloadPickerAir04 = FALSE;
		if (nCnt > 2) pDY10->oUnloadPickerAir05 = FALSE;
		if (nCnt > 3) pDY10->oUnloadPickerAir06 = FALSE;
	}
	else if (nNo == 4) {
		if (nCnt > 0) pDY10->oUnloadPickerAir04 = FALSE;
		if (nCnt > 1) pDY10->oUnloadPickerAir05 = FALSE;
		if (nCnt > 2) pDY10->oUnloadPickerAir06 = FALSE;
	}
	else if (nNo == 5) {
		if (nCnt > 0) pDY10->oUnloadPickerAir05 = FALSE;
		if (nCnt > 1) pDY10->oUnloadPickerAir06 = FALSE;
	}
	else if (nNo == 6) {
		if (nCnt > 0) pDY10->oUnloadPickerAir06 = FALSE;
	}

	g_objAJinAXL.Write_Output(10);
}

// 	11. Unload Port I/O 함수
void CCommon::Set_UnloadPortSupportIn(int nNo)
{
	DY_DATA_03 *pDY03 = g_objAJinAXL.Get_pDY03();
	if (nNo == 1) { pDY03->oUnloadPort1SupportIn = TRUE; pDY03->oUnloadPort1SupportOut = FALSE; }
	if (nNo == 2) { pDY03->oUnloadPort2SupportIn = TRUE; pDY03->oUnloadPort2SupportOut = FALSE; }
	g_objAJinAXL.Write_Output(3);
}

BOOL CCommon::Get_UnloadPortSupportIn(int nNo)
{
	DX_DATA_03 *pDX03 = g_objAJinAXL.Get_pDX03();
	if (nNo == 1) {
		if (!pDX03->iUnloadPort1Support1In || pDX03->iUnloadPort1Support1Out) return FALSE;
		if (!pDX03->iUnloadPort1Support2In || pDX03->iUnloadPort1Support2Out) return FALSE;
	} else if (nNo == 2) {
		if (!pDX03->iUnloadPort2Support1In || pDX03->iUnloadPort2Support1Out) return FALSE;
		if (!pDX03->iUnloadPort2Support2In || pDX03->iUnloadPort2Support2Out) return FALSE;
	} 
	return TRUE;
}

void CCommon::Set_UnloadPortSupportOut(int nNo)
{
	DY_DATA_03 *pDY03 = g_objAJinAXL.Get_pDY03();
	if (nNo == 1) { pDY03->oUnloadPort1SupportIn = FALSE; pDY03->oUnloadPort1SupportOut = TRUE; }
	if (nNo == 2) { pDY03->oUnloadPort2SupportIn = FALSE; pDY03->oUnloadPort2SupportOut = TRUE; }
	g_objAJinAXL.Write_Output(3);
}

BOOL CCommon::Get_UnloadPortSupportOut(int nNo)
{
	DX_DATA_03 *pDX03 = g_objAJinAXL.Get_pDX03();
	if (nNo == 1) {
		if (pDX03->iUnloadPort1Support1In || !pDX03->iUnloadPort1Support1Out) return FALSE;
		if (pDX03->iUnloadPort1Support2In || !pDX03->iUnloadPort1Support2Out) return FALSE;
	} else if (nNo == 2) {
		if (pDX03->iUnloadPort2Support1In || !pDX03->iUnloadPort2Support1Out) return FALSE;
		if (pDX03->iUnloadPort2Support2In || !pDX03->iUnloadPort2Support2Out) return FALSE;
	} 
	return TRUE;
}

//  12. Tray I/O 함수
BOOL CCommon::Get_LoadTrayMasterSlaveIn(int nNo)
{
	DX_DATA_04 *pDX04 = g_objAJinAXL.Get_pDX04();

	if (nNo == 1) {
		if (!pDX04->iLoadStage1MasterIn || pDX04->iLoadStage1MasterOut) return FALSE;
		if (!pDX04->iLoadStage1SlaveIn  || pDX04->iLoadStage1SlaveOut) return FALSE;
	} else if (nNo == 2) {
		if (!pDX04->iLoadStage2MasterIn || pDX04->iLoadStage2MasterOut) return FALSE;
		if (!pDX04->iLoadStage2SlaveIn  || pDX04->iLoadStage2SlaveOut) return FALSE;
	}
	return TRUE;
}

BOOL CCommon::Get_LoadTrayMasterSlaveOut(int nNo)
{
	DX_DATA_04 *pDX04 = g_objAJinAXL.Get_pDX04();

	if (nNo == 1) {
		if (pDX04->iLoadStage1MasterIn || !pDX04->iLoadStage1MasterOut) return FALSE;
		if (pDX04->iLoadStage1SlaveIn  || !pDX04->iLoadStage1SlaveOut) return FALSE;
	} else if (nNo == 2) {
		if (pDX04->iLoadStage2MasterIn || !pDX04->iLoadStage2MasterOut) return FALSE;
		if (pDX04->iLoadStage2SlaveIn  || !pDX04->iLoadStage2SlaveOut) return FALSE;
	}
	return TRUE;
}

BOOL CCommon::Get_CapTrayMasterSlaveIn(int nNo)
{
	DX_DATA_04 *pDX04 = g_objAJinAXL.Get_pDX04();

	if (nNo == 1) {
		if (!pDX04->iCapStage1MasterIn || pDX04->iCapStage1MasterOut) return FALSE;
		if (!pDX04->iCapStage1SlaveIn  || pDX04->iCapStage1SlaveOut) return FALSE;
	} else if (nNo == 2) {
		if (!pDX04->iCapStage2MasterIn || pDX04->iCapStage2MasterOut) return FALSE;
		if (!pDX04->iCapStage2SlaveIn  || pDX04->iCapStage2SlaveOut) return FALSE;
	}
	return TRUE;
}

BOOL CCommon::Get_CapTrayMasterSlaveOut(int nNo)
{
	DX_DATA_04 *pDX04 = g_objAJinAXL.Get_pDX04();

	if (nNo == 1) {
		if (pDX04->iCapStage1MasterIn || !pDX04->iCapStage1MasterOut) return FALSE;
		if (pDX04->iCapStage1SlaveIn  || !pDX04->iCapStage1SlaveOut) return FALSE;
	} else if (nNo == 2) {
		if (pDX04->iCapStage2MasterIn || !pDX04->iCapStage2MasterOut) return FALSE;
		if (pDX04->iCapStage2SlaveIn  || !pDX04->iCapStage2SlaveOut) return FALSE;
	}
	return TRUE;
}

BOOL CCommon::Get_UnloadTrayMasterSlaveIn(int nNo)
{
	DX_DATA_05 *pDX05 = g_objAJinAXL.Get_pDX05();

	if (nNo == 1) {
		if (!pDX05->iUnloadStage1MasterIn || pDX05->iUnloadStage1MasterOut) return FALSE;
		if (!pDX05->iUnloadStage1SlaveIn  || pDX05->iUnloadStage1SlaveOut) return FALSE;
	} else if (nNo == 2) {
		if (!pDX05->iUnloadStage2MasterIn || pDX05->iUnloadStage2MasterOut) return FALSE;
		if (!pDX05->iUnloadStage2SlaveIn  || pDX05->iUnloadStage2SlaveOut) return FALSE;
	}
	return TRUE;
}

BOOL CCommon::Get_UnloadTrayMasterSlaveOut(int nNo)
{
	DX_DATA_05 *pDX05 = g_objAJinAXL.Get_pDX05();

	if (nNo == 1) {
		if (pDX05->iUnloadStage1MasterIn || !pDX05->iUnloadStage1MasterOut) return FALSE;
		if (pDX05->iUnloadStage1SlaveIn  || !pDX05->iUnloadStage1SlaveOut) return FALSE;
	} else if (nNo == 2) {
		if (pDX05->iUnloadStage2MasterIn || !pDX05->iUnloadStage2MasterOut) return FALSE;
		if (pDX05->iUnloadStage2SlaveIn  || !pDX05->iUnloadStage2SlaveOut) return FALSE;
	}
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////
// 1. Info Load Picker I/O 함수
void CCommon::Set_InfoLoadPickerDown(int nPos, int nLine)
{
	DY_DATA_06 *pDY06 = g_objAJinAXL.Get_pDY06();
	int *pInfo = NULL;

	if (nPos == 0) pInfo = Get_LoadTrayInfo(nLine);
	if (nPos == 1) pInfo = gData.InfoLoadPick;

	if (pInfo[0] > 0) pDY06->oLoadPickerDown01 = TRUE;
	if (pInfo[1] > 0) pDY06->oLoadPickerDown02 = TRUE;
	if (pInfo[2] > 0) pDY06->oLoadPickerDown03 = TRUE;
	if (gData.nPickerUseCnt > 3) {if (pInfo[3] > 0) pDY06->oLoadPickerDown04 = TRUE;}
	if (gData.nPickerUseCnt > 4) {if (pInfo[4] > 0) pDY06->oLoadPickerDown05 = TRUE;}
	if (gData.nPickerUseCnt > 5) {if (pInfo[5] > 0) pDY06->oLoadPickerDown06 = TRUE;}

	g_objAJinAXL.Write_Output(6);
}

BOOL CCommon::Get_InfoLoadPickerDown(int nPos, int nLine)
{
	DX_DATA_06 *pDX06 = g_objAJinAXL.Get_pDX06();
	int *pInfo = NULL;

	if (nPos == 0) pInfo = Get_LoadTrayInfo(nLine);
	if (nPos == 1) pInfo = gData.InfoLoadPick;

	if (pInfo[0] > 0 && (pDX06->iLoadPickerUp01 || !pDX06->iLoadPickerDown01)) { gAlm.nLDPickerNo = 1; return FALSE; }
	if (pInfo[1] > 0 && (pDX06->iLoadPickerUp02 || !pDX06->iLoadPickerDown02)) { gAlm.nLDPickerNo = 2; return FALSE; }
	if (pInfo[2] > 0 && (pDX06->iLoadPickerUp03 || !pDX06->iLoadPickerDown03)) { gAlm.nLDPickerNo = 3; return FALSE; }
	if (gData.nPickerUseCnt > 3){if (pInfo[3] > 0 && (pDX06->iLoadPickerUp04 || !pDX06->iLoadPickerDown04)) { gAlm.nLDPickerNo = 4; return FALSE; }}
	if (gData.nPickerUseCnt > 4){if (pInfo[4] > 0 && (pDX06->iLoadPickerUp05 || !pDX06->iLoadPickerDown05)) { gAlm.nLDPickerNo = 5; return FALSE; }}
	if (gData.nPickerUseCnt > 5){if (pInfo[5] > 0 && (pDX06->iLoadPickerUp06 || !pDX06->iLoadPickerDown06)) { gAlm.nLDPickerNo = 6; return FALSE; }}
	return TRUE;
}

void CCommon::Set_InfoLoadPickerGrip(int nPos, int nLine)
{
	DY_DATA_06 *pDY06 = g_objAJinAXL.Get_pDY06();
	int *pInfo = NULL;

	if (nPos == 0) pInfo = Get_LoadTrayInfo(nLine);
	if (nPos == 1) pInfo = gData.InfoLoadPick;

	if (pInfo[0] > 0) pDY06->oLoadPickerGrip01 = TRUE;
	if (pInfo[1] > 0) pDY06->oLoadPickerGrip02 = TRUE;
	if (pInfo[2] > 0) pDY06->oLoadPickerGrip03 = TRUE;
	if (pInfo[3] > 0) pDY06->oLoadPickerGrip04 = TRUE;
	if (pInfo[4] > 0) pDY06->oLoadPickerGrip05 = TRUE;
	if (pInfo[5] > 0) pDY06->oLoadPickerGrip06 = TRUE;
	
	g_objAJinAXL.Write_Output(6);
}

BOOL CCommon::Get_InfoLoadPickerGrip(int nPos, int nLine)
{
	DX_DATA_06 *pDX06 = g_objAJinAXL.Get_pDX06();
	int *pInfo = NULL;

	if (nPos == 0) pInfo = Get_LoadTrayInfo(nLine);
	if (nPos == 1) pInfo = gData.InfoLoadPick;

	if (pInfo[0] > 0 && pDX06->iLoadPickerOpen01) { gAlm.nLDPickerNo = 1; return FALSE; }
	if (pInfo[1] > 0 && pDX06->iLoadPickerOpen02) { gAlm.nLDPickerNo = 2; return FALSE; }
	if (pInfo[2] > 0 && pDX06->iLoadPickerOpen03) { gAlm.nLDPickerNo = 3; return FALSE; }
	if (gData.nPickerUseCnt > 3){if (pInfo[3] > 0 && pDX06->iLoadPickerOpen04) { gAlm.nLDPickerNo = 4; return FALSE; }}
	if (gData.nPickerUseCnt > 4){if (pInfo[4] > 0 && pDX06->iLoadPickerOpen05) { gAlm.nLDPickerNo = 5; return FALSE; }}
	if (gData.nPickerUseCnt > 5){if (pInfo[5] > 0 && pDX06->iLoadPickerOpen06) { gAlm.nLDPickerNo = 6; return FALSE; }}
	return TRUE;
}

BOOL CCommon::Get_InfoLoadPickerGripOpen(int nPos, int nLine)
{
	DX_DATA_06 *pDX06 = g_objAJinAXL.Get_pDX06();
	int *pInfo = NULL;

	if (nPos == 0) pInfo = Get_LoadTrayInfo(nLine);
	if (nPos == 1) pInfo = gData.InfoLoadPick;

	if (pInfo[0] == 0 && !pDX06->iLoadPickerOpen01) { gAlm.nLDPickerNo = 1; return FALSE; }
	if (pInfo[1] == 0 && !pDX06->iLoadPickerOpen02) { gAlm.nLDPickerNo = 2; return FALSE; }
	if (pInfo[2] == 0 && !pDX06->iLoadPickerOpen03) { gAlm.nLDPickerNo = 3; return FALSE; }
	if (gData.nPickerUseCnt > 3){if (pInfo[3] == 0 && !pDX06->iLoadPickerOpen04) { gAlm.nLDPickerNo = 4; return FALSE; }}
	if (gData.nPickerUseCnt > 4){if (pInfo[4] == 0 && !pDX06->iLoadPickerOpen05) { gAlm.nLDPickerNo = 5; return FALSE; }}
	if (gData.nPickerUseCnt > 5){if (pInfo[5] == 0 && !pDX06->iLoadPickerOpen06) { gAlm.nLDPickerNo = 6; return FALSE; }}
	return TRUE;
}

BOOL CCommon::Get_InfoLoadPickerCmCheck()
{
#ifdef DRY_RUN_TEST
	return TRUE;
#endif

	DX_DATA_06 *pDX06 = g_objAJinAXL.Get_pDX06();
	int *pInfo = NULL;

	pInfo = gData.InfoLoadPick;

	if (pInfo[0] > 0 && !pDX06->iLoadPickerCMCheck01) { gAlm.nLDPickerNo = 1; return FALSE; }
	if (pInfo[1] > 0 && !pDX06->iLoadPickerCMCheck02) { gAlm.nLDPickerNo = 2; return FALSE; }
	if (pInfo[2] > 0 && !pDX06->iLoadPickerCMCheck03) { gAlm.nLDPickerNo = 3; return FALSE; }
	if (gData.nPickerUseCnt > 3)if (pInfo[3] > 0 && !pDX06->iLoadPickerCMCheck04) { gAlm.nLDPickerNo = 4; return FALSE; }
	if (gData.nPickerUseCnt > 4)if (pInfo[4] > 0 && !pDX06->iLoadPickerCMCheck05) { gAlm.nLDPickerNo = 5; return FALSE; }
	if (gData.nPickerUseCnt > 5)if (pInfo[5] > 0 && !pDX06->iLoadPickerCMCheck06) { gAlm.nLDPickerNo = 6; return FALSE; }
	return TRUE;
}

int CCommon::Get_InfoLoadPickerDownCnt(int nTrayY, int nTrayX, int nDownSu)
{
	DX_DATA_06 *pDX06 = g_objAJinAXL.Get_pDX06();
	int *pInfo = NULL;
	int nCnt = 0;
	int nENo = nTrayX + nDownSu;

	pInfo = Get_LoadTrayInfo(nTrayY);

	for (int i=nTrayX; i<nENo; i++) {
		if (pInfo[i] > 0) nCnt++;
	}

	return nCnt;
}

// void CCommon::Set_InfoLoadPickerDownMulti(int nNo, int nCnt, int nLine, int nTNo)
// {
// 	DY_DATA_06 *pDY06 = g_objAJinAXL.Get_pDY06();
// 	int *pInfo = NULL;
// 
// 	pInfo = Get_LoadTrayInfo(nLine);
// 
// 	if (nNo == 1) {
// 		if (nCnt > 0 && pInfo[nTNo]   > 0) pDY06->oLoadPickerDown01 = TRUE;
// 		if (nCnt > 1 && pInfo[nTNo+1] > 0) pDY06->oLoadPickerDown02 = TRUE;
// 		if (nCnt > 2 && pInfo[nTNo+2] > 0) pDY06->oLoadPickerDown03 = TRUE;
// 		if (nCnt > 3 && pInfo[nTNo+3] > 0) pDY06->oLoadPickerDown04 = TRUE;
// 		if (nCnt > 4 && pInfo[nTNo+4] > 0) pDY06->oLoadPickerDown05 = TRUE;
// 		if (nCnt > 5 && pInfo[nTNo+5] > 0) pDY06->oLoadPickerDown06 = TRUE;
// 	}
// 	else if (nNo == 2) {
// 		if (nCnt > 0 && pInfo[nTNo]   > 0) pDY06->oLoadPickerDown02 = TRUE;
// 		if (nCnt > 1 && pInfo[nTNo+1] > 0) pDY06->oLoadPickerDown03 = TRUE;
// 		if (nCnt > 2 && pInfo[nTNo+2] > 0) pDY06->oLoadPickerDown04 = TRUE;
// 		if (nCnt > 3 && pInfo[nTNo+3] > 0) pDY06->oLoadPickerDown05 = TRUE;
// 		if (nCnt > 4 && pInfo[nTNo+4] > 0) pDY06->oLoadPickerDown06 = TRUE;
// 	}
// 	else if (nNo == 3) {
// 		if (nCnt > 0 && pInfo[nTNo]   > 0) pDY06->oLoadPickerDown03 = TRUE;
// 		if (nCnt > 1 && pInfo[nTNo+1] > 0) pDY06->oLoadPickerDown04 = TRUE;
// 		if (nCnt > 2 && pInfo[nTNo+2] > 0) pDY06->oLoadPickerDown05 = TRUE;
// 		if (nCnt > 3 && pInfo[nTNo+3] > 0) pDY06->oLoadPickerDown06 = TRUE;
// 	}
// 	else if (nNo == 4) {
// 		if (nCnt > 0 && pInfo[nTNo]   > 0) pDY06->oLoadPickerDown04 = TRUE;
// 		if (nCnt > 1 && pInfo[nTNo+1] > 0) pDY06->oLoadPickerDown05 = TRUE;
// 		if (nCnt > 2 && pInfo[nTNo+2] > 0) pDY06->oLoadPickerDown06 = TRUE;
// 	}
// 	else if (nNo == 5) {
// 		if (nCnt > 0 && pInfo[nTNo]   > 0) pDY06->oLoadPickerDown05 = TRUE;
// 		if (nCnt > 1 && pInfo[nTNo+1] > 0) pDY06->oLoadPickerDown06 = TRUE;
// 	}
// 	else if (nNo == 6) {
// 		if (nCnt > 0 && pInfo[nTNo]   > 0) pDY06->oLoadPickerDown06 = TRUE;
// 	}
// 
// 	g_objAJinAXL.Write_Output(6);
// }

// int CCommon::Get_InfoLoadPickerDownMulti(int nLine, int nTNo)
// {
// 	DX_DATA_06 *pDX06 = g_objAJinAXL.Get_pDX06();
// 
// 	int *pInfo = NULL;
// 
// 	pInfo = Get_LoadTrayInfo(nLine);
// 	
// 	if (nNo == 1) {
// 		if (nCnt > 0 && pInfo[nTNo]   > 0 && (pDX06->iLoadPickerUp01 || !pDX06->iLoadPickerDown01)) return FALSE;
// 		if (nCnt > 1 && pInfo[nTNo+1] > 0 && (pDX06->iLoadPickerUp02 || !pDX06->iLoadPickerDown02)) return FALSE;
// 		if (nCnt > 2 && pInfo[nTNo+2] > 0 && (pDX06->iLoadPickerUp03 || !pDX06->iLoadPickerDown03)) return FALSE;
// 		if (nCnt > 3 && pInfo[nTNo+3] > 0 && (pDX06->iLoadPickerUp04 || !pDX06->iLoadPickerDown04)) return FALSE;
// 		if (nCnt > 4 && pInfo[nTNo+4] > 0 && (pDX06->iLoadPickerUp05 || !pDX06->iLoadPickerDown05)) return FALSE;
// 		if (nCnt > 5 && pInfo[nTNo+5] > 0 && (pDX06->iLoadPickerUp06 || !pDX06->iLoadPickerDown06)) return FALSE;
// 	}
// 	else if (nNo == 2) {
// 		if (nCnt > 0 && pInfo[nTNo]   > 0 && (pDX06->iLoadPickerUp02 || !pDX06->iLoadPickerDown02)) return FALSE;
// 		if (nCnt > 1 && pInfo[nTNo+1] > 0 && (pDX06->iLoadPickerUp03 || !pDX06->iLoadPickerDown03)) return FALSE;
// 		if (nCnt > 2 && pInfo[nTNo+2] > 0 && (pDX06->iLoadPickerUp04 || !pDX06->iLoadPickerDown04)) return FALSE;
// 		if (nCnt > 3 && pInfo[nTNo+3] > 0 && (pDX06->iLoadPickerUp05 || !pDX06->iLoadPickerDown05)) return FALSE;
// 		if (nCnt > 4 && pInfo[nTNo+4] > 0 && (pDX06->iLoadPickerUp06 || !pDX06->iLoadPickerDown06)) return FALSE;
// 	}
// 	else if (nNo == 3) {
// 		if (nCnt > 0 && pInfo[nTNo]   > 0 && (pDX06->iLoadPickerUp03 || !pDX06->iLoadPickerDown03)) return FALSE;
// 		if (nCnt > 1 && pInfo[nTNo+1] > 0 && (pDX06->iLoadPickerUp04 || !pDX06->iLoadPickerDown04)) return FALSE;
// 		if (nCnt > 2 && pInfo[nTNo+2] > 0 && (pDX06->iLoadPickerUp05 || !pDX06->iLoadPickerDown05)) return FALSE;
// 		if (nCnt > 3 && pInfo[nTNo+3] > 0 && (pDX06->iLoadPickerUp06 || !pDX06->iLoadPickerDown06)) return FALSE;
// 	}
// 	else if (nNo == 4) {
// 		if (nCnt > 0 && pInfo[nTNo]   > 0 && (pDX06->iLoadPickerUp04 || !pDX06->iLoadPickerDown04)) return FALSE;
// 		if (nCnt > 1 && pInfo[nTNo+1] > 0 && (pDX06->iLoadPickerUp05 || !pDX06->iLoadPickerDown05)) return FALSE;
// 		if (nCnt > 2 && pInfo[nTNo+2] > 0 && (pDX06->iLoadPickerUp06 || !pDX06->iLoadPickerDown06)) return FALSE;
// 	}
// 	else if (nNo == 5) {
// 		if (nCnt > 0 && pInfo[nTNo]   > 0 && (pDX06->iLoadPickerUp05 || !pDX06->iLoadPickerDown05)) return FALSE;
// 		if (nCnt > 1 && pInfo[nTNo+1] > 0 && (pDX06->iLoadPickerUp06 || !pDX06->iLoadPickerDown06)) return FALSE;
// 	}
// 	else if (nNo == 6) {
// 		if (nCnt > 0 && pInfo[nTNo]   > 0 && (pDX06->iLoadPickerUp06 || !pDX06->iLoadPickerDown06)) return FALSE;
// 	}
// 
// 	return TRUE;
// }
// 2. Info Main Index I/O 함수
BOOL CCommon::Get_InfoIndexLoadVacuumOn(int nPos)
{
#ifdef DRY_RUN_TEST
	return TRUE;	// 테스트
#endif
	DX_DATA_11 *pDX11 = g_objAJinAXL.Get_pDX11();

	int *pInfo = NULL;
	if (nPos == 0) pInfo = gData.InfoIndex[0];
	if (nPos == 1) pInfo = gData.InfoLoadPick;

	if (!pDX11->iIndexLoadVac01 && pInfo[0] > 0) { gAlm.nIndexVacNo = 1; return FALSE; }
	if (!pDX11->iIndexLoadVac02 && pInfo[1] > 0) { gAlm.nIndexVacNo = 2; return FALSE; }
	if (!pDX11->iIndexLoadVac03 && pInfo[2] > 0) { gAlm.nIndexVacNo = 3; return FALSE; }
	if (gData.nPickerUseCnt > 3)if (!pDX11->iIndexLoadVac04 && pInfo[3] > 0) { gAlm.nIndexVacNo = 4; return FALSE; }
	if (gData.nPickerUseCnt > 4)if (!pDX11->iIndexLoadVac05 && pInfo[4] > 0) { gAlm.nIndexVacNo = 5; return FALSE; }
	if (gData.nPickerUseCnt > 5)if (!pDX11->iIndexLoadVac06 && pInfo[5] > 0) { gAlm.nIndexVacNo = 6; return FALSE; }
	return TRUE;
}

void CCommon::Set_InfoIndexLoadVacuumOn(int nPos)
{
#ifdef DRY_RUN_TEST
	return;		// 테스트
#endif
	DY_DATA_11 *pDY11 = g_objAJinAXL.Get_pDY11();

	int *pInfo = NULL;
	if (nPos == 0) pInfo = gData.InfoIndex[0];
	if (nPos == 1) pInfo = gData.InfoLoadPick;

	if (pInfo[0] > 0) pDY11->oIndexLoadVac01 = TRUE;
	if (pInfo[1] > 0) pDY11->oIndexLoadVac02 = TRUE;
	if (pInfo[2] > 0) pDY11->oIndexLoadVac03 = TRUE;
	if (pInfo[3] > 0) pDY11->oIndexLoadVac04 = TRUE;
	if (pInfo[4] > 0) pDY11->oIndexLoadVac05 = TRUE;
	if (pInfo[5] > 0) pDY11->oIndexLoadVac06 = TRUE;

	g_objAJinAXL.Write_Output(11);
}

BOOL CCommon::Get_InfoIndexAssyVacuumOn()
{
#ifdef DRY_RUN_TEST
	return TRUE;	// 테스트
#endif
	DX_DATA_11 *pDX11 = g_objAJinAXL.Get_pDX11();

	int *pInfo = NULL;
	pInfo = gData.InfoIndex[1];

	if (((pDX11->iIndexAssyVac01 && pInfo[0] > 0) || pInfo[0] == 0) &&
		((pDX11->iIndexAssyVac02 && pInfo[1] > 0) || pInfo[1] == 0) &&
		((pDX11->iIndexAssyVac03 && pInfo[2] > 0) || pInfo[2] == 0) &&
		((pDX11->iIndexAssyVac04 && pInfo[3] > 0) || pInfo[3] == 0) &&
		((pDX11->iIndexAssyVac05 && pInfo[4] > 0) || pInfo[4] == 0) &&
		((pDX11->iIndexAssyVac06 && pInfo[5] > 0) || pInfo[5] == 0)) return TRUE;

	return FALSE;
}

void CCommon::Set_InfoIndexAssyVacuumOn()
{
#ifdef DRY_RUN_TEST
	return;		// 테스트
#endif
	DY_DATA_11 *pDY11 = g_objAJinAXL.Get_pDY11();

	int *pInfo = NULL;
	pInfo = gData.InfoIndex[1];

	if (pInfo[0] > 0) pDY11->oIndexAssyVac01 = TRUE;
	if (pInfo[1] > 0) pDY11->oIndexAssyVac02 = TRUE;
	if (pInfo[2] > 0) pDY11->oIndexAssyVac03 = TRUE;
	if (pInfo[3] > 0) pDY11->oIndexAssyVac04 = TRUE;
	if (pInfo[4] > 0) pDY11->oIndexAssyVac05 = TRUE;
	if (pInfo[5] > 0) pDY11->oIndexAssyVac06 = TRUE;

	g_objAJinAXL.Write_Output(11);
}

int CCommon::Get_IndexPos(int nPos)
{
	DX_DATA_11 *pDX11 = g_objAJinAXL.Get_pDX11();
	int nIndexPos = pDX11->iIndexPosition0 + (pDX11->iIndexPosition1 << 1);
	int nNo = 0;

	if (nPos == 1) {	// Load
		if (nIndexPos == 0) nNo = 0;
		if (nIndexPos == 1) nNo = 1;
		if (nIndexPos == 2) nNo = 2;
		if (nIndexPos == 3) nNo = 3;

	} else if (nPos == 2) {	// Assy
		if (nIndexPos == 0) nNo = 3;
		if (nIndexPos == 1) nNo = 0;
		if (nIndexPos == 2) nNo = 1;
		if (nIndexPos == 3) nNo = 2;

	} else if (nPos == 3) {	// Trans
		if (nIndexPos == 0) nNo = 2;
		if (nIndexPos == 1) nNo = 3;
		if (nIndexPos == 2) nNo = 0;
		if (nIndexPos == 3) nNo = 1;
	}

	return nNo;
}

// 3. Info Cap Picker I/O 함수
void CCommon::Set_InfoCapPickerDown(int nPos)
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	DY_DATA_07 *pDY07 = g_objAJinAXL.Get_pDY07();
	int *pInfo = NULL;
	
	if (nPos == 0) pInfo = gData.InfoCapBuffer;

	if (pInfo[0] == 0) pDY07->oCapPickerDown01 = TRUE;
	if (pInfo[1] == 0) pDY07->oCapPickerDown02 = TRUE;
	if (pInfo[2] == 0) pDY07->oCapPickerDown03 = TRUE;
	if (gData.nPickerUseCnt > 3) { if (pInfo[3] == 0) pDY07->oCapPickerDown04 = TRUE; }
	if (gData.nPickerUseCnt > 4) { if (pInfo[4] == 0) pDY07->oCapPickerDown05 = TRUE; }
	if (gData.nPickerUseCnt > 5) { if (pInfo[5] == 0) pDY07->oCapPickerDown06 = TRUE; }

	g_objAJinAXL.Write_Output(7);
}

BOOL CCommon::Get_InfoCapPickerDown(int nPos)
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	DX_DATA_07 *pDX07 = g_objAJinAXL.Get_pDX07();
	int *pInfo = NULL;
	
	if (nPos == 0) pInfo = gData.InfoCapBuffer;

	if (((pDX07->iCapPickerUp01 || !pDX07->iCapPickerDown01) && pInfo[0] == 0 ) || ((!pDX07->iCapPickerUp01 || pDX07->iCapPickerDown01) && pInfo[0] > 0)) { gAlm.nCapPickeNo = 1; return FALSE; }
	if (((pDX07->iCapPickerUp02 || !pDX07->iCapPickerDown02) && pInfo[1] == 0 ) || ((!pDX07->iCapPickerUp02 || pDX07->iCapPickerDown02) && pInfo[1] > 0)) { gAlm.nCapPickeNo = 2; return FALSE; }
	if (((pDX07->iCapPickerUp03 || !pDX07->iCapPickerDown03) && pInfo[2] == 0 ) || ((!pDX07->iCapPickerUp03 || pDX07->iCapPickerDown03) && pInfo[2] > 0)) { gAlm.nCapPickeNo = 3; return FALSE; }
	if (gData.nPickerUseCnt > 3) { if (((pDX07->iCapPickerUp04 || !pDX07->iCapPickerDown04) && pInfo[3] == 0 ) || ((!pDX07->iCapPickerUp04 || pDX07->iCapPickerDown04) && pInfo[3] > 0)) { gAlm.nCapPickeNo = 4; return FALSE; }}
	if (gData.nPickerUseCnt > 4) { if (((pDX07->iCapPickerUp05 || !pDX07->iCapPickerDown05) && pInfo[4] == 0 ) || ((!pDX07->iCapPickerUp05 || pDX07->iCapPickerDown05) && pInfo[4] > 0)) { gAlm.nCapPickeNo = 5; return FALSE; }}
	if (gData.nPickerUseCnt > 5) { if (((pDX07->iCapPickerUp06 || !pDX07->iCapPickerDown06) && pInfo[5] == 0 ) || ((!pDX07->iCapPickerUp06 || pDX07->iCapPickerDown06) && pInfo[5] > 0)) { gAlm.nCapPickeNo = 6; return FALSE; }}
	return TRUE;
}

BOOL CCommon::Get_InfoCapPickerVacOn(int nPos)
{
#ifdef DRY_RUN_TEST
	return TRUE;
#endif

	DX_DATA_07 *pDX07 = g_objAJinAXL.Get_pDX07();
	int *pInfo = NULL;

	if (nPos == 0) pInfo = gData.InfoCapPick;

	if (!pDX07->iCapPickerVac01 && pInfo[0] > 0) { gAlm.nCapPickeNo = 1; return FALSE; }
	if (!pDX07->iCapPickerVac02 && pInfo[1] > 0) { gAlm.nCapPickeNo = 2; return FALSE; }
	if (!pDX07->iCapPickerVac03 && pInfo[2] > 0) { gAlm.nCapPickeNo = 3; return FALSE; }
	if (gData.nPickerUseCnt > 3)if (!pDX07->iCapPickerVac04 && pInfo[3] > 0) { gAlm.nCapPickeNo = 4; return FALSE; }
	if (gData.nPickerUseCnt > 4)if (!pDX07->iCapPickerVac05 && pInfo[4] > 0) { gAlm.nCapPickeNo = 5; return FALSE; }
	if (gData.nPickerUseCnt > 5)if (!pDX07->iCapPickerVac06 && pInfo[5] > 0) { gAlm.nCapPickeNo = 6; return FALSE; }
	return TRUE;
}

void CCommon::Set_InfoCapPickerVacOff(int nPos)
{
#ifdef DRY_RUN_TEST
	return;		// 테스트
#endif

	DY_DATA_07 *pDY07 = g_objAJinAXL.Get_pDY07();
	int *pInfo = NULL;

	if (nPos == 0) pInfo = gData.InfoCapBuffer;

	if (pInfo[0] == 0) { pDY07->oCapPickerVac01 = FALSE; pDY07->oCapPickerAir01 = TRUE; }
	if (pInfo[1] == 0) { pDY07->oCapPickerVac02 = FALSE; pDY07->oCapPickerAir02 = TRUE; }
	if (pInfo[2] == 0) { pDY07->oCapPickerVac03 = FALSE; pDY07->oCapPickerAir03 = TRUE; }
	if (pInfo[3] == 0) { pDY07->oCapPickerVac04 = FALSE; pDY07->oCapPickerAir04 = TRUE; }
	if (pInfo[4] == 0) { pDY07->oCapPickerVac05 = FALSE; pDY07->oCapPickerAir05 = TRUE; }
	if (pInfo[5] == 0) { pDY07->oCapPickerVac06 = FALSE; pDY07->oCapPickerAir06 = TRUE; }

	g_objAJinAXL.Write_Output(7);
}

BOOL CCommon::Get_InfoCapPickerVacOff(int nPos)
{
#ifdef DRY_RUN_TEST
	return TRUE;
#endif

	DX_DATA_07 *pDX07 = g_objAJinAXL.Get_pDX07();
	int *pInfo = NULL;

	if (nPos == 0) pInfo = gData.InfoCapPick;

	if (pDX07->iCapPickerVac01 && pInfo[0] == 0) { gAlm.nCapPickeNo = 1; return FALSE; }
	if (pDX07->iCapPickerVac02 && pInfo[1] == 0) { gAlm.nCapPickeNo = 2; return FALSE; }
	if (pDX07->iCapPickerVac03 && pInfo[2] == 0) { gAlm.nCapPickeNo = 3; return FALSE; }
	if (gData.nPickerUseCnt > 3)if (pDX07->iCapPickerVac04 && pInfo[3] == 0) { gAlm.nCapPickeNo = 4; return FALSE; }
	if (gData.nPickerUseCnt > 4)if (pDX07->iCapPickerVac05 && pInfo[4] == 0) { gAlm.nCapPickeNo = 5; return FALSE; }
	if (gData.nPickerUseCnt > 5)if (pDX07->iCapPickerVac06 && pInfo[5] == 0) { gAlm.nCapPickeNo = 6; return FALSE; }
	return TRUE;
}

void CCommon::Set_InfoCapPickerAirOff(int nPos)
{
	DY_DATA_07 *pDY07 = g_objAJinAXL.Get_pDY07();
	int *pInfo = NULL;

	if (nPos == 0) pInfo = gData.InfoCapPick;

	if (pInfo[0] == 0) { pDY07->oCapPickerAir01 = FALSE; }
	if (pInfo[1] == 0) { pDY07->oCapPickerAir02 = FALSE; }
	if (pInfo[2] == 0) { pDY07->oCapPickerAir03 = FALSE; }
	if (pInfo[3] == 0) { pDY07->oCapPickerAir04 = FALSE; }
	if (pInfo[4] == 0) { pDY07->oCapPickerAir05 = FALSE; }
	if (pInfo[5] == 0) { pDY07->oCapPickerAir06 = FALSE; }

	g_objAJinAXL.Write_Output(7);
}

void CCommon::Set_InfoCapPickerNormal()
{
	DY_DATA_07 *pDY07 = g_objAJinAXL.Get_pDY07();
	int *pInfo = NULL;

	pInfo = gData.InfoCapPick;

	if (pInfo[0] == 0) pDY07->oCapPickerTurn01 = FALSE;
	if (pInfo[1] == 0) pDY07->oCapPickerTurn02 = FALSE;
	if (pInfo[2] == 0) pDY07->oCapPickerTurn03 = FALSE;
	if (pInfo[3] == 0) pDY07->oCapPickerTurn04 = FALSE;
	if (pInfo[4] == 0) pDY07->oCapPickerTurn05 = FALSE;
	if (pInfo[5] == 0) pDY07->oCapPickerTurn06 = FALSE;
	g_objAJinAXL.Write_Output(7);
}

BOOL CCommon::Get_InfoCapPickerNormal()
{
	DX_DATA_07 *pDX07 = g_objAJinAXL.Get_pDX07();
	int *pInfo = NULL;

	pInfo = gData.InfoCapPick;

	if (pDX07->iCapPickerTurn01 && pInfo[0] == 0) { gAlm.nCapPickeNo = 1; return FALSE; }
	if (pDX07->iCapPickerTurn02 && pInfo[1] == 0) { gAlm.nCapPickeNo = 2; return FALSE; }
	if (pDX07->iCapPickerTurn03 && pInfo[2] == 0) { gAlm.nCapPickeNo = 3; return FALSE; }
	if (gData.nPickerUseCnt > 3)if (pDX07->iCapPickerTurn04 && pInfo[3] == 0) { gAlm.nCapPickeNo = 4; return FALSE; }
	if (gData.nPickerUseCnt > 4)if (pDX07->iCapPickerTurn05 && pInfo[4] == 0) { gAlm.nCapPickeNo = 5; return FALSE; }
	if (gData.nPickerUseCnt > 5)if (pDX07->iCapPickerTurn06 && pInfo[5] == 0) { gAlm.nCapPickeNo = 6; return FALSE; }
	return TRUE;
}

// 4. Info Assy Picker I/O 함수
void CCommon::Set_InfoAssyPickerDown(int nPos)
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	DY_DATA_08 *pDY08 = g_objAJinAXL.Get_pDY08();
	int *pInfo = NULL;

	if (nPos == 0) {
		pInfo = gData.InfoAssyPick;

		if (pInfo[0] == 0) pDY08->oAssyPickerDown01 = TRUE;
		if (pInfo[1] == 0) pDY08->oAssyPickerDown02 = TRUE;
		if (pInfo[2] == 0) pDY08->oAssyPickerDown03 = TRUE;
		if (gData.nPickerUseCnt > 3) { if (pInfo[3] == 0) pDY08->oAssyPickerDown04 = TRUE; }
		if (gData.nPickerUseCnt > 4) { if (pInfo[4] == 0) pDY08->oAssyPickerDown05 = TRUE; }
		if (gData.nPickerUseCnt > 5) { if (pInfo[5] == 0) pDY08->oAssyPickerDown06 = TRUE; }
	}
	if (nPos == 1) {
		pInfo = gData.InfoIndex[1];

		if (pInfo[0] > 0) pDY08->oAssyPickerDown01 = TRUE;
		if (pInfo[1] > 0) pDY08->oAssyPickerDown02 = TRUE;
		if (pInfo[2] > 0) pDY08->oAssyPickerDown03 = TRUE;
		if (pInfo[3] > 0) pDY08->oAssyPickerDown04 = TRUE;
		if (pInfo[4] > 0) pDY08->oAssyPickerDown05 = TRUE;
		if (pInfo[5] > 0) pDY08->oAssyPickerDown06 = TRUE;
	}
	g_objAJinAXL.Write_Output(8);
}

BOOL CCommon::Get_InfoAssyPickerDown(int nPos)
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	DX_DATA_08 *pDX08 = g_objAJinAXL.Get_pDX08();
	int *pInfo = NULL;

	if (nPos == 0) {
		pInfo = gData.InfoAssyPick;
		
		if (((pDX08->iAssyPickerUp01 || !pDX08->iAssyPickerDown01) && pInfo[0] == 0 ) || ((!pDX08->iAssyPickerUp01 || pDX08->iAssyPickerDown01) && pInfo[0] > 0)) { gAlm.nAssyPickNo = 1; return FALSE; }
		if (((pDX08->iAssyPickerUp02 || !pDX08->iAssyPickerDown02) && pInfo[1] == 0 ) || ((!pDX08->iAssyPickerUp02 || pDX08->iAssyPickerDown02) && pInfo[1] > 0)) { gAlm.nAssyPickNo = 2; return FALSE; }
		if (((pDX08->iAssyPickerUp03 || !pDX08->iAssyPickerDown03) && pInfo[2] == 0 ) || ((!pDX08->iAssyPickerUp03 || pDX08->iAssyPickerDown03) && pInfo[2] > 0)) { gAlm.nAssyPickNo = 3; return FALSE; }
		if (gData.nPickerUseCnt > 3) { if (((pDX08->iAssyPickerUp04 || !pDX08->iAssyPickerDown04) && pInfo[3] == 0 ) || ((!pDX08->iAssyPickerUp04 || pDX08->iAssyPickerDown04) && pInfo[3] > 0)) { gAlm.nAssyPickNo = 4; return FALSE; } }
		if (gData.nPickerUseCnt > 4) { if (((pDX08->iAssyPickerUp05 || !pDX08->iAssyPickerDown05) && pInfo[4] == 0 ) || ((!pDX08->iAssyPickerUp05 || pDX08->iAssyPickerDown05) && pInfo[4] > 0)) { gAlm.nAssyPickNo = 5; return FALSE; } }
		if (gData.nPickerUseCnt > 5) { if (((pDX08->iAssyPickerUp06 || !pDX08->iAssyPickerDown06) && pInfo[5] == 0 ) || ((!pDX08->iAssyPickerUp06 || pDX08->iAssyPickerDown06) && pInfo[5] > 0)) { gAlm.nAssyPickNo = 6; return FALSE; } }
	}
	if (nPos == 1) {
		pInfo = gData.InfoIndex[1];

		if ((pDX08->iAssyPickerUp01 || !pDX08->iAssyPickerDown01) && pInfo[0] > 0) { gAlm.nAssyPickNo = 1; return FALSE; }
		if ((pDX08->iAssyPickerUp02 || !pDX08->iAssyPickerDown02) && pInfo[1] > 0) { gAlm.nAssyPickNo = 2; return FALSE; }
		if ((pDX08->iAssyPickerUp03 || !pDX08->iAssyPickerDown03) && pInfo[2] > 0) { gAlm.nAssyPickNo = 3; return FALSE; }
		if (gData.nPickerUseCnt > 3)if ((pDX08->iAssyPickerUp04 || !pDX08->iAssyPickerDown04) && pInfo[3] > 0) { gAlm.nAssyPickNo = 4; return FALSE; }
		if (gData.nPickerUseCnt > 4)if ((pDX08->iAssyPickerUp05 || !pDX08->iAssyPickerDown05) && pInfo[4] > 0) { gAlm.nAssyPickNo = 5; return FALSE; }
		if (gData.nPickerUseCnt > 5)if ((pDX08->iAssyPickerUp06 || !pDX08->iAssyPickerDown06) && pInfo[5] > 0) { gAlm.nAssyPickNo = 6; return FALSE; }
	}
	return TRUE;
}

void CCommon::Set_InfoAssyPickerGoodDown()
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	DY_DATA_08 *pDY08 = g_objAJinAXL.Get_pDY08();

	if (gData.InfoIndex[1][0] > 0 && gData.InfoAssyPick[0] == 9) pDY08->oAssyPickerDown01 = TRUE;
	if (gData.InfoIndex[1][1] > 0 && gData.InfoAssyPick[1] == 9) pDY08->oAssyPickerDown02 = TRUE;
	if (gData.InfoIndex[1][2] > 0 && gData.InfoAssyPick[2] == 9) pDY08->oAssyPickerDown03 = TRUE;
	if (gData.InfoIndex[1][3] > 0 && gData.InfoAssyPick[3] == 9) pDY08->oAssyPickerDown04 = TRUE;
	if (gData.InfoIndex[1][4] > 0 && gData.InfoAssyPick[4] == 9) pDY08->oAssyPickerDown05 = TRUE;
	if (gData.InfoIndex[1][5] > 0 && gData.InfoAssyPick[5] == 9) pDY08->oAssyPickerDown06 = TRUE;

	g_objAJinAXL.Write_Output(8);
}

BOOL CCommon::Get_InfoAssyPickerGoodDown()
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	DX_DATA_08 *pDX08 = g_objAJinAXL.Get_pDX08();

	if (gData.InfoIndex[1][0] > 0 && gData.InfoAssyPick[0] == 9 && (pDX08->iAssyPickerUp01 || !pDX08->iAssyPickerDown01)) { gAlm.nAssyPickNo = 1; return FALSE; }
	if (gData.InfoIndex[1][1] > 0 && gData.InfoAssyPick[1] == 9 && (pDX08->iAssyPickerUp02 || !pDX08->iAssyPickerDown02)) { gAlm.nAssyPickNo = 2; return FALSE; }
	if (gData.InfoIndex[1][2] > 0 && gData.InfoAssyPick[2] == 9 && (pDX08->iAssyPickerUp03 || !pDX08->iAssyPickerDown03)) { gAlm.nAssyPickNo = 3; return FALSE; }
	if (gData.nPickerUseCnt > 3)if (gData.InfoIndex[1][3] > 0 && gData.InfoAssyPick[3] == 9 && (pDX08->iAssyPickerUp04 || !pDX08->iAssyPickerDown04)) { gAlm.nAssyPickNo = 4; return FALSE; }
	if (gData.nPickerUseCnt > 4)if (gData.InfoIndex[1][4] > 0 && gData.InfoAssyPick[4] == 9 && (pDX08->iAssyPickerUp05 || !pDX08->iAssyPickerDown05)) { gAlm.nAssyPickNo = 5; return FALSE; }
	if (gData.nPickerUseCnt > 5)if (gData.InfoIndex[1][5] > 0 && gData.InfoAssyPick[5] == 9 && (pDX08->iAssyPickerUp06 || !pDX08->iAssyPickerDown06)) { gAlm.nAssyPickNo = 6; return FALSE; }
	return TRUE;
}

void CCommon::Set_InfoAssyPickerVacOn(int nPos)
{
#ifdef DRY_RUN_TEST
	return;		// 테스트
#endif

	DY_DATA_08 *pDY08 = g_objAJinAXL.Get_pDY08();
	int *pInfo = NULL;

	if (nPos == 0) pInfo = gData.InfoAssyPick;

	if (pInfo[0] == 0) { pDY08->oAssyPickerVac01 = TRUE; }
	if (pInfo[1] == 0) { pDY08->oAssyPickerVac02 = TRUE; }
	if (pInfo[2] == 0) { pDY08->oAssyPickerVac03 = TRUE; }
	if (pInfo[3] == 0) { pDY08->oAssyPickerVac04 = TRUE; }
	if (pInfo[4] == 0) { pDY08->oAssyPickerVac05 = TRUE; }
	if (pInfo[5] == 0) { pDY08->oAssyPickerVac06 = TRUE; }

	g_objAJinAXL.Write_Output(8);
}

BOOL CCommon::Get_InfoAssyPickerVacOn(int nPos)
{
#ifdef DRY_RUN_TEST
	return TRUE;
#endif

	DX_DATA_08 *pDX08 = g_objAJinAXL.Get_pDX08();
	int *pInfo = NULL;

	if (nPos == 0) pInfo = gData.InfoAssyPick;

	if (!pDX08->iAssyPickerVac01 && pInfo[0] > 0) { gAlm.nAssyPickNo = 1; return FALSE; }
	if (!pDX08->iAssyPickerVac02 && pInfo[1] > 0) { gAlm.nAssyPickNo = 2; return FALSE; }
	if (!pDX08->iAssyPickerVac03 && pInfo[2] > 0) { gAlm.nAssyPickNo = 3; return FALSE; }
	if (gData.nPickerUseCnt > 3)if (!pDX08->iAssyPickerVac04 && pInfo[3] > 0) { gAlm.nAssyPickNo = 4; return FALSE; }
	if (gData.nPickerUseCnt > 4)if (!pDX08->iAssyPickerVac05 && pInfo[4] > 0) { gAlm.nAssyPickNo = 5; return FALSE; }
	if (gData.nPickerUseCnt > 5)if (!pDX08->iAssyPickerVac06 && pInfo[5] > 0) { gAlm.nAssyPickNo = 6; return FALSE; }
	return TRUE;
}

void CCommon::Set_InfoAssyPickerVacOff(int nPos)
{
	DY_DATA_08 *pDY08 = g_objAJinAXL.Get_pDY08();
	int *pInfo = NULL;

	if (nPos == 0) pInfo = gData.InfoIndex[1];

	if (pInfo[0] > 0) { pDY08->oAssyPickerVac01 = FALSE; pDY08->oAssyPickerAir01 = TRUE; }
	if (pInfo[1] > 0) { pDY08->oAssyPickerVac02 = FALSE; pDY08->oAssyPickerAir02 = TRUE; }
	if (pInfo[2] > 0) { pDY08->oAssyPickerVac03 = FALSE; pDY08->oAssyPickerAir03 = TRUE; }
	if (pInfo[3] > 0) { pDY08->oAssyPickerVac04 = FALSE; pDY08->oAssyPickerAir04 = TRUE; }
	if (pInfo[4] > 0) { pDY08->oAssyPickerVac05 = FALSE; pDY08->oAssyPickerAir05 = TRUE; }
	if (pInfo[5] > 0) { pDY08->oAssyPickerVac06 = FALSE; pDY08->oAssyPickerAir06 = TRUE; }

	g_objAJinAXL.Write_Output(8);
}

BOOL CCommon::Get_InfoAssyPickerVacOff(int nPos)
{
	DX_DATA_08 *pDX08 = g_objAJinAXL.Get_pDX08();
	int *pInfo = NULL;

	if (nPos == 0) pInfo = gData.InfoIndex[1];

	if (pDX08->iAssyPickerVac01 && pInfo[0] > 0) { gAlm.nAssyPickNo = 1; return FALSE; }
	if (pDX08->iAssyPickerVac02 && pInfo[1] > 0) { gAlm.nAssyPickNo = 2; return FALSE; }
	if (pDX08->iAssyPickerVac03 && pInfo[2] > 0) { gAlm.nAssyPickNo = 3; return FALSE; }
	if (gData.nPickerUseCnt > 3)if (pDX08->iAssyPickerVac04 && pInfo[3] > 0) { gAlm.nAssyPickNo = 4; return FALSE; }
	if (gData.nPickerUseCnt > 4)if (pDX08->iAssyPickerVac05 && pInfo[4] > 0) { gAlm.nAssyPickNo = 5; return FALSE; }
	if (gData.nPickerUseCnt > 5)if (pDX08->iAssyPickerVac06 && pInfo[5] > 0) { gAlm.nAssyPickNo = 6; return FALSE; }
	return TRUE;
}

void CCommon::Set_InfoAssyPickerAirOff(int nPos)
{
	DY_DATA_08 *pDY08 = g_objAJinAXL.Get_pDY08();
	int *pInfo = NULL;

	if (nPos == 0) pInfo = gData.InfoIndex[1];

	if (pInfo[0] > 0) { pDY08->oAssyPickerAir01 = FALSE; }
	if (pInfo[1] > 0) { pDY08->oAssyPickerAir02 = FALSE; }
	if (pInfo[2] > 0) { pDY08->oAssyPickerAir03 = FALSE; }
	if (pInfo[3] > 0) { pDY08->oAssyPickerAir04 = FALSE; }
	if (pInfo[4] > 0) { pDY08->oAssyPickerAir05 = FALSE; }
	if (pInfo[5] > 0) { pDY08->oAssyPickerAir06 = FALSE; }

	g_objAJinAXL.Write_Output(8);
}

void CCommon::Set_InfoAssyPickerGoodVacOff()
{
	DY_DATA_08 *pDY08 = g_objAJinAXL.Get_pDY08();

	if (gData.InfoIndex[1][0] > 0 && gData.InfoAssyPick[0] == 9) { pDY08->oAssyPickerVac01 = FALSE; pDY08->oAssyPickerAir01 = TRUE; }
	if (gData.InfoIndex[1][1] > 0 && gData.InfoAssyPick[1] == 9) { pDY08->oAssyPickerVac02 = FALSE; pDY08->oAssyPickerAir02 = TRUE; }
	if (gData.InfoIndex[1][2] > 0 && gData.InfoAssyPick[2] == 9) { pDY08->oAssyPickerVac03 = FALSE; pDY08->oAssyPickerAir03 = TRUE; }
	if (gData.InfoIndex[1][3] > 0 && gData.InfoAssyPick[3] == 9) { pDY08->oAssyPickerVac04 = FALSE; pDY08->oAssyPickerAir04 = TRUE; }
	if (gData.InfoIndex[1][4] > 0 && gData.InfoAssyPick[4] == 9) { pDY08->oAssyPickerVac05 = FALSE; pDY08->oAssyPickerAir05 = TRUE; }
	if (gData.InfoIndex[1][5] > 0 && gData.InfoAssyPick[5] == 9) { pDY08->oAssyPickerVac06 = FALSE; pDY08->oAssyPickerAir06 = TRUE; }

	g_objAJinAXL.Write_Output(8);
}

BOOL CCommon::Get_InfoAssyPickerGoodVacOff()
{
	DX_DATA_08 *pDX08 = g_objAJinAXL.Get_pDX08();

	if (((!pDX08->iAssyPickerVac01 && gData.InfoIndex[1][0] > 0 && gData.InfoAssyPick[0] == 9 ) || (gData.InfoIndex[1][0] == 0 && gData.InfoAssyPick[0] != 9)) &&
		((!pDX08->iAssyPickerVac02 && gData.InfoIndex[1][1] > 0 && gData.InfoAssyPick[1] == 9 ) || (gData.InfoIndex[1][1] == 0 && gData.InfoAssyPick[1] != 9)) &&
		((!pDX08->iAssyPickerVac03 && gData.InfoIndex[1][2] > 0 && gData.InfoAssyPick[2] == 9 ) || (gData.InfoIndex[1][2] == 0 && gData.InfoAssyPick[2] != 9)) &&
		((!pDX08->iAssyPickerVac04 && gData.InfoIndex[1][3] > 0 && gData.InfoAssyPick[3] == 9 ) || (gData.InfoIndex[1][3] == 0 && gData.InfoAssyPick[3] != 9)) &&
		((!pDX08->iAssyPickerVac05 && gData.InfoIndex[1][4] > 0 && gData.InfoAssyPick[4] == 9 ) || (gData.InfoIndex[1][4] == 0 && gData.InfoAssyPick[4] != 9)) &&
		((!pDX08->iAssyPickerVac06 && gData.InfoIndex[1][5] > 0 && gData.InfoAssyPick[5] == 9 ) || (gData.InfoIndex[1][5] == 0 && gData.InfoAssyPick[5] != 9))) return TRUE;

	return FALSE;
}

void CCommon::Set_InfoAssyPickerGoodAirOff()
{
	DY_DATA_08 *pDY08 = g_objAJinAXL.Get_pDY08();

	if (gData.InfoIndex[1][0] > 0 && gData.InfoAssyPick[0] == 9) { pDY08->oAssyPickerAir01 = FALSE; }
	if (gData.InfoIndex[1][1] > 0 && gData.InfoAssyPick[1] == 9) { pDY08->oAssyPickerAir02 = FALSE; }
	if (gData.InfoIndex[1][2] > 0 && gData.InfoAssyPick[2] == 9) { pDY08->oAssyPickerAir03 = FALSE; }
	if (gData.InfoIndex[1][3] > 0 && gData.InfoAssyPick[3] == 9) { pDY08->oAssyPickerAir04 = FALSE; }
	if (gData.InfoIndex[1][4] > 0 && gData.InfoAssyPick[4] == 9) { pDY08->oAssyPickerAir05 = FALSE; }
	if (gData.InfoIndex[1][5] > 0 && gData.InfoAssyPick[5] == 9) { pDY08->oAssyPickerAir06 = FALSE; }

	g_objAJinAXL.Write_Output(8);
}

// 5. Info Trans Stage I/O 함수
void CCommon::Set_InfoTransStageVacOn(int nPos)
{
#ifdef DRY_RUN_TEST
	return;
#endif
	DY_DATA_05 *pDY05 = g_objAJinAXL.Get_pDY05();
	int *pInfo = gData.InfoIndex[2];
	if (nPos == 1) pInfo = gData.InfoTransStage;

	if (pInfo[0] > 0) { pDY05->oTransStageVac01 = TRUE; }
	if (pInfo[1] > 0) { pDY05->oTransStageVac02 = TRUE; }
	if (pInfo[2] > 0) { pDY05->oTransStageVac03 = TRUE; }
	if (pInfo[3] > 0) { pDY05->oTransStageVac04 = TRUE; }
	if (pInfo[4] > 0) { pDY05->oTransStageVac05 = TRUE; }
	if (pInfo[5] > 0) { pDY05->oTransStageVac06 = TRUE; }

	g_objAJinAXL.Write_Output(5);
}

BOOL CCommon::Get_InfoTransStageVacOn(int nPos)
{
#ifdef DRY_RUN_TEST
	return TRUE;
#endif
	DX_DATA_05 *pDX05 = g_objAJinAXL.Get_pDX05();
	int *pInfo = gData.InfoIndex[2];
	if (nPos == 1) pInfo = gData.InfoTransStage;

	if (((pDX05->iTransStageVac01 && pInfo[0] > 0) || pInfo[0] == 0) &&
		((pDX05->iTransStageVac02 && pInfo[1] > 0) || pInfo[1] == 0) &&
		((pDX05->iTransStageVac03 && pInfo[2] > 0) || pInfo[2] == 0) &&
		((pDX05->iTransStageVac04 && pInfo[3] > 0) || pInfo[3] == 0) &&
		((pDX05->iTransStageVac05 && pInfo[4] > 0) || pInfo[4] == 0) &&
		((pDX05->iTransStageVac06 && pInfo[5] > 0) || pInfo[5] == 0)) return TRUE;

	return FALSE;
}

// 6. Info Unload Picker I/O 함수
void CCommon::Set_InfoUnloadPickerDown(int nPos)
{
	DY_DATA_10  *pDY10 = g_objAJinAXL.Get_pDY10();
	int *pInfo = NULL;

	if (nPos == 0) pInfo = gData.InfoTransStage;

	if (pInfo[0] > 0) pDY10->oUnloadPickerDown01 = TRUE;
	if (pInfo[1] > 0) pDY10->oUnloadPickerDown02 = TRUE;
	if (pInfo[2] > 0) pDY10->oUnloadPickerDown03 = TRUE;
	if (pInfo[3] > 0) pDY10->oUnloadPickerDown04 = TRUE;
	if (pInfo[4] > 0) pDY10->oUnloadPickerDown05 = TRUE;
	if (pInfo[5] > 0) pDY10->oUnloadPickerDown06 = TRUE;

	g_objAJinAXL.Write_Output(10);
}

BOOL CCommon::Get_InfoUnloadPickerDown(int nPos)
{
	DX_DATA_10 *pDX10 = g_objAJinAXL.Get_pDX10();
	int *pInfo = NULL;

	if (nPos == 0) pInfo = gData.InfoTransStage;

	if ((pDX10->iUnloadPickerUp01 || !pDX10->iUnloadPickerDown01) && pInfo[0] > 0 ) { gAlm.nUNPickerNo = 1; return FALSE; }
	if ((pDX10->iUnloadPickerUp02 || !pDX10->iUnloadPickerDown02) && pInfo[1] > 0 ) { gAlm.nUNPickerNo = 2; return FALSE; }
	if ((pDX10->iUnloadPickerUp03 || !pDX10->iUnloadPickerDown03) && pInfo[2] > 0 ) { gAlm.nUNPickerNo = 3; return FALSE; }
	if (gData.nPickerUseCnt > 3)if ((pDX10->iUnloadPickerUp04 || !pDX10->iUnloadPickerDown04) && pInfo[3] > 0 ) { gAlm.nUNPickerNo = 4; return FALSE; }
	if (gData.nPickerUseCnt > 4)if ((pDX10->iUnloadPickerUp05 || !pDX10->iUnloadPickerDown05) && pInfo[4] > 0 ) { gAlm.nUNPickerNo = 5; return FALSE; }
	if (gData.nPickerUseCnt > 5)if ((pDX10->iUnloadPickerUp06 || !pDX10->iUnloadPickerDown06) && pInfo[5] > 0 ) { gAlm.nUNPickerNo = 6; return FALSE; }
	return TRUE;
}

void CCommon::Set_InfoUnloadPickerVacOn(int nPos)
{
#ifdef DRY_RUN_TEST
	return;		// 테스트
#endif

	DY_DATA_10 *pDY10 = g_objAJinAXL.Get_pDY10();
	int *pInfo = NULL;

	if (nPos == 0) pInfo = gData.InfoTransStage;
	if (nPos == 1) pInfo = gData.InfoUnloadPick;

	if (pInfo[0] > 0) { pDY10->oUnloadPickerVac01 = TRUE; }
	if (pInfo[1] > 0) { pDY10->oUnloadPickerVac02 = TRUE; }
	if (pInfo[2] > 0) { pDY10->oUnloadPickerVac03 = TRUE; }
	if (pInfo[3] > 0) { pDY10->oUnloadPickerVac04 = TRUE; }
	if (pInfo[4] > 0) { pDY10->oUnloadPickerVac05 = TRUE; }
	if (pInfo[5] > 0) { pDY10->oUnloadPickerVac06 = TRUE; }

	g_objAJinAXL.Write_Output(10);
}

BOOL CCommon::Get_InfoUnloadPickerVacOn(int nPos)
{
#ifdef DRY_RUN_TEST
	return TRUE;
#endif

	DX_DATA_10 *pDX10 = g_objAJinAXL.Get_pDX10();
	int *pInfo = NULL;

	if (nPos == 0) pInfo = gData.InfoTransStage;
	if (nPos == 1) pInfo = gData.InfoUnloadPick;

	if (!pDX10->iUnloadPickerVac01 && pInfo[0] > 0 ) { gAlm.nUNPickerNo = 1; return FALSE; }
	if (!pDX10->iUnloadPickerVac02 && pInfo[1] > 0 ) { gAlm.nUNPickerNo = 2; return FALSE; }
	if (!pDX10->iUnloadPickerVac03 && pInfo[2] > 0 ) { gAlm.nUNPickerNo = 3; return FALSE; }
	if (gData.nPickerUseCnt > 3)if (!pDX10->iUnloadPickerVac04 && pInfo[3] > 0 ) { gAlm.nUNPickerNo = 4; return FALSE; }
	if (gData.nPickerUseCnt > 4)if (!pDX10->iUnloadPickerVac05 && pInfo[4] > 0 ) { gAlm.nUNPickerNo = 5; return FALSE; }
	if (gData.nPickerUseCnt > 5)if (!pDX10->iUnloadPickerVac06 && pInfo[5] > 0 ) { gAlm.nUNPickerNo = 6; return FALSE; }
	return TRUE;
}

BOOL CCommon::Get_InfoUnloadPickerVacOff(int nPos)
{
#ifdef DRY_RUN_TEST
	return TRUE;
#endif

	DX_DATA_10 *pDX10 = g_objAJinAXL.Get_pDX10();
	int *pInfo = NULL;

	if (nPos == 0) pInfo = gData.InfoUnloadPick;

	if (pDX10->iUnloadPickerVac01 && pInfo[0] == 0 ) { gAlm.nUNPickerNo = 1; return FALSE; }
	if (pDX10->iUnloadPickerVac02 && pInfo[1] == 0 ) { gAlm.nUNPickerNo = 2; return FALSE; }
	if (pDX10->iUnloadPickerVac03 && pInfo[2] == 0 ) { gAlm.nUNPickerNo = 3; return FALSE; }
	if (gData.nPickerUseCnt > 3)if (pDX10->iUnloadPickerVac04 && pInfo[3] == 0 ) { gAlm.nUNPickerNo = 4; return FALSE; }
	if (gData.nPickerUseCnt > 4)if (pDX10->iUnloadPickerVac05 && pInfo[4] == 0 ) { gAlm.nUNPickerNo = 5; return FALSE; }
	if (gData.nPickerUseCnt > 5)if (pDX10->iUnloadPickerVac06 && pInfo[5] == 0 ) { gAlm.nUNPickerNo = 6; return FALSE; }
	return TRUE;
}

BOOL CCommon::Get_InfoUnloadPickerCapCheck()
{
#ifdef DRY_RUN_TEST
	return TRUE;
#endif

	DX_DATA_10 *pDX10 = g_objAJinAXL.Get_pDX10();
	int *pInfo = gData.InfoUnloadPick;

	if (!pDX10->iUnloadPickerCapChk1 && pInfo[0] > 0 ) { gAlm.nUNPickerNo = 1; return FALSE; }
	if (!pDX10->iUnloadPickerCapChk2 && pInfo[1] > 0 ) { gAlm.nUNPickerNo = 2; return FALSE; }
	if (!pDX10->iUnloadPickerCapChk3 && pInfo[2] > 0 ) { gAlm.nUNPickerNo = 3; return FALSE; }
	if (gData.nPickerUseCnt > 3)if (!pDX10->iUnloadPickerCapChk4 && pInfo[3] > 0 ) { gAlm.nUNPickerNo = 4; return FALSE; }
	if (gData.nPickerUseCnt > 4)if (!pDX10->iUnloadPickerCapChk5 && pInfo[4] > 0 ) { gAlm.nUNPickerNo = 5; return FALSE; }
	if (gData.nPickerUseCnt > 5)if (!pDX10->iUnloadPickerCapChk6 && pInfo[5] > 0 ) { gAlm.nUNPickerNo = 6; return FALSE; }
	return TRUE;
}

//7. Info Cap Clean I/O
void CCommon::Set_InfoCapCleanAirOn()
{
#ifdef DRY_RUN_TEST
	return;		// 테스트
#endif
	// Cap Picker Down인 곳만 Air 불어준다.
	DY_DATA_09 *pDY09 = g_objAJinAXL.Get_pDY09();
	DX_DATA_07 *pDX07 = g_objAJinAXL.Get_pDX07();

	if (pDX07->iCapPickerDown01) pDY09->oCapCleanAir01 = TRUE;
	if (pDX07->iCapPickerDown02) pDY09->oCapCleanAir02 = TRUE;
	if (pDX07->iCapPickerDown03) pDY09->oCapCleanAir03 = TRUE;
	if (pDX07->iCapPickerDown04) pDY09->oCapCleanAir04 = TRUE;
	if (pDX07->iCapPickerDown05) pDY09->oCapCleanAir05 = TRUE;
	if (pDX07->iCapPickerDown06) pDY09->oCapCleanAir06 = TRUE;
	pDY09->oCapCleanSuction = TRUE;

	g_objAJinAXL.Write_Output(9);

	//int *pInfo = gData.InfoCapBuffer;
	//int *pInfo = gData.InfoCapPick;

// 	if (pInfo[0] > 0) pDY09->oCapCleanAir01 = TRUE;
// 	if (pInfo[1] > 0) pDY09->oCapCleanAir02 = TRUE;
// 	if (pInfo[2] > 0) pDY09->oCapCleanAir03 = TRUE;
// 	if (pInfo[3] > 0) pDY09->oCapCleanAir04 = TRUE;
// 	if (pInfo[4] > 0) pDY09->oCapCleanAir05 = TRUE;
// 	if (pInfo[5] > 0) pDY09->oCapCleanAir06 = TRUE;

//	g_objAJinAXL.Write_Output(9);
}

///////////////////////////////////////////////////////////////////////////////
// Load Tray Info
int *CCommon::Get_LoadTrayInfo(int nLine)
{
	static int nInfo[6] = { 0 };

	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	int nENo = pEquipData->nLoadTrayX;
	for (int i = 0; i < nENo; i++) {
		nInfo[i] = gData.InfoLoadTray[nLine][i];
	}
	return nInfo;
}
///////////////////////////////////////////////////////////////////////////////
CString CCommon::Get_AssyPickerTiltErr()
{
	DX_DATA_09 *pDX09 = g_objAJinAXL.Get_pDX09();
	int *pInfo = gData.InfoIndex[1];
	int nPNo = gData.nPNoIndex[1] - 1;

	CString strErr, strSNo;
	strErr = "";
	if (pDX09->iAssyPickerTilt01 && pInfo[0] > 0) { 
		if (strErr == "") strErr = " #==> ";

		if (gData.bCapAlignNg[0]) strErr += "1(Align NG). ";
		else					  strErr += "1(Align Good). ";

		gLot.nCapFailCount[nPNo]++;
	}
	if (pDX09->iAssyPickerTilt02 && pInfo[1] > 0) {
		if (strErr == "") strErr = " #==> ";

		if (gData.bCapAlignNg[1]) strErr += "2(Align NG). ";
		else					  strErr += "2(Align Good). ";

		gLot.nCapFailCount[nPNo]++;
	}
	if (pDX09->iAssyPickerTilt03 && pInfo[2] > 0) {
		if (strErr == "") strErr = " #==> ";
		
		if (gData.bCapAlignNg[2]) strErr += "3(Align NG). ";
		else					  strErr += "3(Align Good). ";

		gLot.nCapFailCount[nPNo]++;
	}
	if (pDX09->iAssyPickerTilt04 && pInfo[3] > 0) {
		if (strErr == "") strErr = " #==> ";

		if (gData.bCapAlignNg[3]) strErr += "4(Align NG). ";
		else					  strErr += "4(Align Good). ";

		gLot.nCapFailCount[nPNo]++;
	}
	if (pDX09->iAssyPickerTilt05 && pInfo[4] > 0) {
		if (strErr == "") strErr = " #==> ";

		if (gData.bCapAlignNg[4]) strErr += "5(Align NG). ";
		else					  strErr += "5(Align Good). ";

		gLot.nCapFailCount[nPNo]++;
	}
	if (pDX09->iAssyPickerTilt06 && pInfo[5] > 0) {
		if (strErr == "") strErr = " #==> ";

		if (gData.bCapAlignNg[5]) strErr += "6(Align NG). ";
		else					  strErr += "6(Align Good). ";

		gLot.nCapFailCount[nPNo]++;
	}

	if (strErr != "") {
		strErr += "번 Picker Tilt를 확인 하여 주십시오.";
	}
	return strErr;
}

CString CCommon::Get_AssyPickerForceErr()
{
	DX_DATA_08 *pDX08 = g_objAJinAXL.Get_pDX08();
	int *pInfo = gData.InfoIndex[1];
	int nPNo = gData.nPNoIndex[1] - 1;

	CString strErr, strData;
	strErr = "";
	if (pDX08->iAssyPickerDown01) { 
		if (strErr == "") strErr = " #==> ";
		strData.Format("1번 Picker : %0.3lf kg", gData.dLoadCell[0]);
		strErr += strData;
	
	} else if (pDX08->iAssyPickerDown02) { 
		if (strErr == "") strErr = " #==> ";
		strData.Format("2번 Picker : %0.3lf kg", gData.dLoadCell[0]);
		strErr += strData;

	} else if (pDX08->iAssyPickerDown03) { 
		if (strErr == "") strErr = " #==> ";
		strData.Format("3번 Picker : %0.3lf kg", gData.dLoadCell[0]);
		strErr += strData;

	} else if (pDX08->iAssyPickerDown04) { 
		if (strErr == "") strErr = " #==> ";
		strData.Format("4번 Picker : %0.3lf kg", gData.dLoadCell[0]);
		strErr += strData;

	} else if (pDX08->iAssyPickerDown05) { 
		if (strErr == "") strErr = " #==> ";
		strData.Format("5번 Picker : %0.3lf kg", gData.dLoadCell[0]);
		strErr += strData;

	} else if (pDX08->iAssyPickerDown06) { 
		if (strErr == "") strErr = " #==> ";
		strData.Format("6번 Picker : %0.3lf kg", gData.dLoadCell[0]);
		strErr += strData;
	}	

	if (strErr != "") {
		strErr += "Load Cell 측정값 확인 하여 주십시오.";
	}
	return strErr;
}

CString CCommon::Get_LoadPickerCheckErr()
{
	DX_DATA_06 *pDX06 = g_objAJinAXL.Get_pDX06();
	int *pInfo = gData.InfoLoadPick;

	CString strErr, strSNo;
	strErr = "";
	if (!pDX06->iLoadPickerCMCheck01 && pInfo[0] > 0) { if (strErr == "") { strErr = " ==> "; } strErr += "1. "; }
	if (!pDX06->iLoadPickerCMCheck02 && pInfo[1] > 0) { if (strErr == "") { strErr = " ==> "; } strErr += "2. "; }
	if (!pDX06->iLoadPickerCMCheck03 && pInfo[2] > 0) { if (strErr == "") { strErr = " ==> "; } strErr += "3. "; }
	if (!pDX06->iLoadPickerCMCheck04 && pInfo[3] > 0) { if (strErr == "") { strErr = " ==> "; } strErr += "4. "; }
	if (!pDX06->iLoadPickerCMCheck05 && pInfo[4] > 0) { if (strErr == "") { strErr = " ==> "; } strErr += "5. "; }
	if (!pDX06->iLoadPickerCMCheck06 && pInfo[5] > 0) { if (strErr == "") { strErr = " ==> "; } strErr += "6. "; }
	
	if (strErr != "") {
		strErr += "번 Picker CM Check 센서를 확인 하여 주십시오.";
	}
	return strErr;
}

void CCommon::Save_CapShiipData(int nType)	//1:Cap, 2:Ship
{
	CIniFileCS INI(gsCurrentDir + "\\System\\CapShipData.ini");

	if (nType == 1) {
		INI.Set_String("CAP_DATA", "LOTID", gData.sCapLotID);
		INI.Set_Integer("CAP_DATA", "TRAYCOUNT", gData.nCapTrayUseCount);
		INI.Set_Integer("CAP_DATA", "CMCOUNT", gData.nCapUseCount);
		INI.Set_Integer("CAP_DATA", "USETYCOUNT", gData.nCapTrayCount);
	}
	if (nType == 2) {
		INI.Set_String("SHIP_DATA", "LOTID", gData.sShipLotID);
		INI.Set_Integer("SHIP_DATA", "TRAYCOUNT", gData.nShipTrayUseCount);
		INI.Set_Integer("SHIP_DATA", "CMCOUNT", gData.nShipUseCount);
		INI.Set_Integer("SHIP_DATA", "USETYCOUNT", gData.nShipTrayCount);
	}
}

void CCommon::Read_CapShipPoatNo()
{
 	CIniFileCS INI(gsCurrentDir + "\\System\\CapShipPartNo.ini");
	if (!INI.Check_File()) {
		AfxMessageBox("CapShipPartNo.ini File Not Found!!!");
		gPart.nCapCount = gPart.nShipCount = 0;
		return;
	}
	
	CString sName;
	gPart.nCapCount = INI.Get_Integer("CAP_PARTNO", "COUNT", 0);
	for(int i=0; i<gPart.nCapCount; i++) {
		sName.Format("%02d", i+1);
		gPart.sCapPart[i] = INI.Get_String("CAP_PARTNO", sName, "");
		if (gPart.sCapPart[i].GetLength() < 1) gPart.sCapPart[i] = "????";
	}
	gPart.nShipCount = INI.Get_Integer("SHIP_PARTNO", "COUNT", 0);
	for(int i=0; i<gPart.nShipCount; i++) {
		sName.Format("%02d", i+1);
		gPart.sShipPart[i] = INI.Get_String("SHIP_PARTNO", sName, "");
		if (gPart.sShipPart[i].GetLength() < 1) gPart.sShipPart[i] = "????";
	}
}

BOOL CCommon::Check_CapShipPoatNo(int nType, CString sPartNo)	//1:Cap, 2:Ship
{
	if (nType != 1 && nType != 2) return FALSE;
	if (sPartNo.GetLength() < 4) return FALSE;

	int nSize;
	if (nType == 1) {
		for(int i=0; i<gPart.nCapCount; i++) {
			nSize = gPart.sCapPart[i].GetLength();
			if (sPartNo.Left(nSize) == gPart.sCapPart[i]) return TRUE;
		}
	}

	if (nType == 2) {
		for(int i=0; i<gPart.nShipCount; i++) {
			nSize = gPart.sShipPart[i].GetLength();
			if (sPartNo.Left(nSize) == gPart.sShipPart[i]) return TRUE;
		}
	}
	return FALSE;
}

/*
CString CCommon::Get_Btm1PickerVacErr()
{
	DX_DATA_04 *pDX04 = g_objAJinAXL.Get_pDX04();
	int *pInfo = gData.InfoBtm1Pick;
	
	CString strErr, strSNo;
	strErr = "";
	if (!pDX04->iBtm1PickerVac01 && pInfo[0] > 0) { if (strErr == "") { strErr = " ==> "; } strErr += "1. "; }
	if (!pDX04->iBtm1PickerVac02 && pInfo[1] > 0) { if (strErr == "") { strErr = " ==> "; } strErr += "2. "; }
	if (!pDX04->iBtm1PickerVac03 && pInfo[2] > 0) { if (strErr == "") { strErr = " ==> "; } strErr += "3. "; }
	if (!pDX04->iBtm1PickerVac04 && pInfo[3] > 0) { if (strErr == "") { strErr = " ==> "; } strErr += "4. "; }
	if (!pDX04->iBtm1PickerVac05 && pInfo[4] > 0) { if (strErr == "") { strErr = " ==> "; } strErr += "5. "; }
	if (!pDX04->iBtm1PickerVac06 && pInfo[5] > 0) { if (strErr == "") { strErr = " ==> "; } strErr += "6. "; }
	if (!pDX04->iBtm1PickerVac07 && pInfo[6] > 0) { if (strErr == "") { strErr = " ==> "; } strErr += "7. "; }
	if (!pDX04->iBtm1PickerVac08 && pInfo[7] > 0) { if (strErr == "") { strErr = " ==> "; } strErr += "8. "; }
	if (!pDX04->iBtm1PickerVac09 && pInfo[8] > 0) { if (strErr == "") { strErr = " ==> "; } strErr += "9. "; }
	if (!pDX04->iBtm1PickerVac10 && pInfo[9] > 0) { if (strErr == "") { strErr = " ==> "; } strErr += "10."; }

	if (strErr != "") {
		strErr += "번 Picker Vacuum을 확인 하여 주십시오.";

		if (gData.nErrAngleStageNo != 0) {
			strSNo.Format("%d번 Stage.", gData.nErrAngleStageNo);
			strErr += strSNo;
			gData.nErrAngleStageNo = 0;
		}
	}
	return strErr;
}

CString CCommon::Get_Btm2PickerVacErr()
{
	DX_DATA_08 *pDX08 = g_objAJinAXL.Get_pDX08();
	int *pInfo = gData.InfoBtm2Pick;

	CString strErr;
	strErr = "";
	if (!pDX08->iBtm2PickerVac01 && pInfo[0] > 0) { if (strErr == "") { strErr = " ==> "; } strErr += "1. "; }
	if (!pDX08->iBtm2PickerVac02 && pInfo[1] > 0) { if (strErr == "") { strErr = " ==> "; } strErr += "2. "; }
	if (!pDX08->iBtm2PickerVac03 && pInfo[2] > 0) { if (strErr == "") { strErr = " ==> "; } strErr += "3. "; }
	if (!pDX08->iBtm2PickerVac04 && pInfo[3] > 0) { if (strErr == "") { strErr = " ==> "; } strErr += "4. "; }
	if (!pDX08->iBtm2PickerVac05 && pInfo[4] > 0) { if (strErr == "") { strErr = " ==> "; } strErr += "5. "; }
	if (!pDX08->iBtm2PickerVac06 && pInfo[5] > 0) { if (strErr == "") { strErr = " ==> "; } strErr += "6. "; }
	if (!pDX08->iBtm2PickerVac07 && pInfo[6] > 0) { if (strErr == "") { strErr = " ==> "; } strErr += "7. "; }
	if (!pDX08->iBtm2PickerVac08 && pInfo[7] > 0) { if (strErr == "") { strErr = " ==> "; } strErr += "8. "; }
	if (!pDX08->iBtm2PickerVac09 && pInfo[8] > 0) { if (strErr == "") { strErr = " ==> "; } strErr += "9. "; }
	if (!pDX08->iBtm2PickerVac10 && pInfo[9] > 0) { if (strErr == "") { strErr = " ==> "; } strErr += "10."; }

	if (strErr != "") { 
		strErr += "번 Picker Vacuum을 확인 하여 주십시오.";
	}
	return strErr;
}

CString CCommon::Get_Sort1PickerVacErr()
{
	DX_DATA_10 *pDX10 = g_objAJinAXL.Get_pDX10();
	int *pInfo = gData.InfoBtm2Pick;

	CString strErr, strSNo;
	strErr = "";
	if (!pDX10->iSortPicker1Vac1 && pInfo[0] > 0) { if (strErr == "") { strErr = " ==> "; } strErr += "1. "; }
	if (!pDX10->iSortPicker1Vac2 && pInfo[1] > 0) { if (strErr == "") { strErr = " ==> "; } strErr += "2. "; }
	if (!pDX10->iSortPicker1Vac3 && pInfo[2] > 0) { if (strErr == "") { strErr = " ==> "; } strErr += "3. "; }
	if (!pDX10->iSortPicker1Vac4 && pInfo[3] > 0) { if (strErr == "") { strErr = " ==> "; } strErr += "4. "; }
	if (gData.bR04B) {
		if (!pDX10->iSortPicker1Vac5 && pInfo[4] > 0) { if (strErr == "") { strErr = " ==> "; } strErr += "5. "; }
	}

	if (strErr != "") {
		strErr += "번 Picker Vacuum을 확인 하여 주십시오.";

		if (gData.nErrBufferStageNo != 0) {
			strSNo.Format("%d번 Stage.", gData.nErrBufferStageNo);
			strErr += strSNo;
			gData.nErrBufferStageNo = 0;
		}
	}
	return strErr;
}

CString CCommon::Get_Sort2PickerVacErr()
{
	DX_DATA_11 *pDX11 = g_objAJinAXL.Get_pDX11();
	int *pInfo = gData.InfoBtm2Pick;

	CString strErr, strSNo;
	strErr = "";
	if (!pDX11->iSortPicker2Vac1 && pInfo[0] > 0) { if (strErr == "") { strErr = " ==> "; } strErr += "1. "; }
	if (!pDX11->iSortPicker2Vac2 && pInfo[1] > 0) { if (strErr == "") { strErr = " ==> "; } strErr += "2. "; }
	if (!pDX11->iSortPicker2Vac3 && pInfo[2] > 0) { if (strErr == "") { strErr = " ==> "; } strErr += "3. "; }
	if (!pDX11->iSortPicker2Vac4 && pInfo[3] > 0) { if (strErr == "") { strErr = " ==> "; } strErr += "4. "; }
	if (!pDX11->iSortPicker2Vac5 && pInfo[4] > 0) { if (strErr == "") { strErr = " ==> "; } strErr += "5. "; }

	if (strErr != "") {
		strErr += "번 Picker Vacuum을 확인 하여 주십시오.";

		if (gData.nErrBufferStageNo != 0) {
			strSNo.Format("%d번 Stage.", gData.nErrBufferStageNo);
			strErr += strSNo;
			gData.nErrBufferStageNo = 0;
		}
	}
	return strErr;
}
*/


void CCommon::Get_HardInfo(CString &strHardDisk)
{
	ULARGE_INTEGER freeBytesAvailable, totalBytes, totalFreeBytes;

	// GetDiskFreeSpaceEx 호출
	if (GetDiskFreeSpaceEx("C:\\", &freeBytesAvailable, &totalBytes, &totalFreeBytes)) {
		// GB로 변환하여 출력
		double totalGB = static_cast<double>(totalBytes.QuadPart) / (1024 * 1024 * 1024);
		double freeGB = static_cast<double>(totalFreeBytes.QuadPart) / (1024 * 1024 * 1024);
		double usedGB = totalGB - freeGB;

		strHardDisk.Format("%lf", freeGB);
	}
}

void CCommon::Get_RAMSize(CString &strRAM)
{
	ULONGLONG Size;
	GetPhysicallyInstalledSystemMemory(&Size);
	strRAM.Format(_T("%llu"), ((Size/1024)/1024));
}

void CCommon::Get_CPUInfo(CString& strCPU)
{
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);

	HKEY hKey;	
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0"), 0, KEY_READ, &hKey) == ERROR_SUCCESS)
	{
		char buffer[256];
		DWORD bufferSize = sizeof(buffer);
		if (RegQueryValueEx(hKey, _T("ProcessorNameString"), NULL, NULL, (LPBYTE)buffer, &bufferSize) == ERROR_SUCCESS)
		{
			strCPU = (CString)buffer;
		}
		RegCloseKey(hKey);
	}

	switch (sysInfo.wProcessorArchitecture) {
	case PROCESSOR_ARCHITECTURE_AMD64: strCPU += " x64";  break;
	case PROCESSOR_ARCHITECTURE_INTEL: strCPU += " x86"; break;
	default:  strCPU += " Unknown"; break;
	}   


}