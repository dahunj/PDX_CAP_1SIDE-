// AJinAXL.cpp : 구현 파일
//
#include "stdafx.h"
#include "AJinAXL.h"
#include "CME8000.h"
#include <math.h>
#include "MESInterface.h"

// AJin Board Library
#include "AXL.h"
#include "AXM.h"
#include "AXD.h"
#include "AXDev.h"
#pragma comment (lib, "AXL.lib")

CAJinAXL g_objAJinAXL;

CAJinAXL::CAJinAXL(void)
{
	for (int i = 0; i < DIO_MODULE_COUNT; i++) { DXY_DATA *pDX = Get_pDX(i); pDX->nValue = 0; }
	for (int i = 0; i < DIO_MODULE_COUNT; i++) { DXY_DATA *pDY = Get_pDY(i); pDY->nValue = 0; }

	for (int i = 0; i < AXIS_COUNT; i++) m_strAxisName[i] = _T("");

	m_pThreadAJin = NULL;
	m_bThreadAJin = FALSE;

	m_bReadVelocity = FALSE;
}

CAJinAXL::~CAJinAXL(void)
{
}

BOOL CAJinAXL::Initialize()
{
#ifdef AJIN_BOARD_USE
	DWORD dwReturn;

	dwReturn = AxlOpenNoReset(7);
	if (dwReturn != AXT_RT_SUCCESS) return FALSE;

	// DIO
	long lDIOCount;
	dwReturn = AxdInfoGetModuleCount(&lDIOCount);
	if (dwReturn != AXT_RT_SUCCESS) return FALSE;
	if (lDIOCount < DIO_MODULE_COUNT) return FALSE;

	Read_Input();
	Read_Output();

	// Door Interlock
	m_DY13.oModeSelect = TRUE; Write_Output(13); theApp.uSleep(500);
	m_DY13.oSafetyReset = TRUE;  Write_Output(13); theApp.uSleep(500);
	m_DY13.oSafetyReset = FALSE; Write_Output(13); theApp.uSleep(500);
	m_DY13.oSafetyReset = TRUE;  Write_Output(13); theApp.uSleep(500);
	m_DY13.oSafetyReset = FALSE; Write_Output(13); theApp.uSleep(500);

	Alarm_Reset(-1);	// All Reset

	// Motion
	long lAxisCount;
	dwReturn = AxmInfoGetAxisCount(&lAxisCount);
	if (dwReturn != AXT_RT_SUCCESS) return FALSE;
	if (lAxisCount < AXIS_COUNT) return FALSE;

	CString strMotFile = gsCurrentDir + "\\System\\AJinSetting.mot";
	dwReturn = AxmMotLoadParaAll((LPSTR)(LPCSTR)strMotFile);
	if (dwReturn != AXT_RT_SUCCESS) return FALSE;
#endif

	Read_AxisList();	// Axis Name & Param

	for (long i = 0; i < AXIS_COUNT; i++) Set_ServoOn(i);

	m_bThreadAJin = TRUE;
	m_pThreadAJin = AfxBeginThread(Thread_AJin, NULL);

	return TRUE;
}

void CAJinAXL::Terminate()
{
	if (m_pThreadAJin) {
		m_bThreadAJin = FALSE;
		WaitForSingleObject(m_pThreadAJin->m_hThread, INFINITE);
	}

	for (long i = 0; i < AXIS_COUNT; i++) Set_ServoOff(i);

#ifdef AJIN_BOARD_USE
	if (AxlIsOpened()) AxlClose();
#endif
}

void CAJinAXL::Read_Input()
{
#ifdef AJIN_BOARD_USE
	AxdiReadInportDword( 0, 0, &m_DX00.nValue);
	AxdiReadInportDword( 1, 0, &m_DX01.nValue);
	AxdiReadInportDword( 2, 0, &m_DX02.nValue);
	AxdiReadInportDword( 3, 0, &m_DX03.nValue);
	AxdiReadInportDword( 4, 0, &m_DX04.nValue);
	AxdiReadInportDword( 5, 0, &m_DX05.nValue);
	AxdiReadInportDword( 6, 0, &m_DX06.nValue);
	AxdiReadInportDword( 7, 0, &m_DX07.nValue);
	AxdiReadInportDword( 8, 0, &m_DX08.nValue);
	AxdiReadInportDword( 9, 0, &m_DX09.nValue);
	AxdiReadInportDword(10, 0, &m_DX10.nValue);
	AxdiReadInportDword(11, 0, &m_DX11.nValue);
	AxdiReadInportDword(12, 0, &m_DX12.nValue);
	AxdiReadInportDword(13, 0, &m_DX13.nValue);
#endif
}

void CAJinAXL::Read_Output()
{
#ifdef AJIN_BOARD_USE
	AxdoReadOutportDword(14, 0, &m_DY00.nValue);
	AxdoReadOutportDword(15, 0, &m_DY01.nValue);
	AxdoReadOutportDword(16, 0, &m_DY02.nValue);
	AxdoReadOutportDword(17, 0, &m_DY03.nValue);
	AxdoReadOutportDword(18, 0, &m_DY04.nValue);
	AxdoReadOutportDword(19, 0, &m_DY05.nValue);
	AxdoReadOutportDword(20, 0, &m_DY06.nValue);
	AxdoReadOutportDword(21, 0, &m_DY07.nValue);
	AxdoReadOutportDword(22, 0, &m_DY08.nValue);
	AxdoReadOutportDword(23, 0, &m_DY09.nValue);
	AxdoReadOutportDword(24, 0, &m_DY10.nValue);
	AxdoReadOutportDword(25, 0, &m_DY11.nValue);
	AxdoReadOutportDword(26, 0, &m_DY12.nValue);
	AxdoReadOutportDword(27, 0, &m_DY13.nValue);
#endif
}

void CAJinAXL::Write_Output(int nModule)
{
#ifdef AJIN_BOARD_USE
	if (nModule ==  0) AxdoWriteOutportDword(14, 0, m_DY00.nValue);
	if (nModule ==  1) AxdoWriteOutportDword(15, 0, m_DY01.nValue);
	if (nModule ==  2) AxdoWriteOutportDword(16, 0, m_DY02.nValue);
	if (nModule ==  3) AxdoWriteOutportDword(17, 0, m_DY03.nValue);
	if (nModule ==  4) AxdoWriteOutportDword(18, 0, m_DY04.nValue);
	if (nModule ==  5) AxdoWriteOutportDword(19, 0, m_DY05.nValue);
	if (nModule ==  6) AxdoWriteOutportDword(20, 0, m_DY06.nValue);
	if (nModule ==  7) AxdoWriteOutportDword(21, 0, m_DY07.nValue);
	if (nModule ==  8) AxdoWriteOutportDword(22, 0, m_DY08.nValue);
	if (nModule ==  9) AxdoWriteOutportDword(23, 0, m_DY09.nValue);
	if (nModule == 10) AxdoWriteOutportDword(24, 0, m_DY10.nValue);
	if (nModule == 11) AxdoWriteOutportDword(25, 0, m_DY11.nValue);
	if (nModule == 12) AxdoWriteOutportDword(26, 0, m_DY12.nValue);
	if (nModule == 13) AxdoWriteOutportDword(27, 0, m_DY13.nValue);
#endif
}

void CAJinAXL::Read_MotionStatus()
{
#ifdef AJIN_BOARD_USE
	DWORD dwStatus;
	for (int i = 0; i < AXIS_COUNT; i++) {
		//AxmStatusGetCmdPos(i, &m_Status[i].dPos);		// Position Reading
		AxmStatusGetActPos(i, &m_Status[i].dPos);		// Position Reading

		if (m_bReadVelocity) AxmStatusReadVel(i, &m_Status[i].dVel);	// Velocity Reading

		AxmSignalIsServoOn(i, &dwStatus);								// Servo-On Reading
		m_Status[i].bSOn = (BOOL)dwStatus;

		AxmSignalReadInputBit(i, UIO_INP0, &dwStatus);	// Origin Limit (In0 : Home)
		m_Status[i].bOrg = (BOOL)dwStatus;

		AxmStatusReadMechanical(i, &dwStatus);	// Mechanical Input
		m_Status[i].bELP = (BOOL)((dwStatus >> 0) & 1);	// Positive Limit (bit0)
		m_Status[i].bELN = (BOOL)((dwStatus >> 1) & 1);	// Negative Limit (bit1)
		m_Status[i].bALM = (BOOL)((dwStatus >> 4) & 1);	// Alarm (bit4)
		m_Status[i].bInP = (BOOL)((dwStatus >> 5) & 1);	// In-Position (bit5)

		AxmStatusReadInMotion(i, &dwStatus);			// Motion Running
		m_Status[i].bRun = (BOOL)dwStatus;

		AxmHomeGetResult(i, &dwStatus);					// Home Done
		m_Status[i].bHom = (dwStatus == HOME_SUCCESS ? TRUE : FALSE);
	}
#endif
}

void CAJinAXL::Set_ServoOn(int nAxis)
{
#ifdef AJIN_BOARD_USE
	AxmMoveEStop(nAxis);
	AxmSignalServoOn(nAxis, TRUE);
#endif
}

void CAJinAXL::Set_ServoOff(int nAxis)
{
#ifdef AJIN_BOARD_USE
	AxmMoveEStop(nAxis);
	AxmSignalServoOn(nAxis, FALSE);
#endif
}

void CAJinAXL::Set_EncoderType(int nAxis, int nType)
{
#ifdef AJIN_BOARD_USE
	if (!Is_AbsoluteType(nAxis)) return;
	if (nType == 0) return;		// RTEX일 경우 Incremental 모드 설정을 하지 않음
	AxmSignalSetEncoderType(nAxis, nType);	// ENCODER_TYPE_INCREMENTAL(0), ENCODER_TYPE_ABSOLUTE(1)
	Sleep(10);		// 추가 2017.07.28
#endif
}

void CAJinAXL::Home_Search(int nAxis)
{
#ifdef AJIN_BOARD_USE
	AxmHomeSetStart(nAxis);
	m_Status[nAxis].bHom = FALSE; m_Status[nAxis].bRun = TRUE;
#endif
}

void CAJinAXL::Set_Home(int nAxis)
{
#ifdef AJIN_BOARD_USE
	AxmStatusSetActPos(nAxis, 0.0);
	AxmStatusSetCmdPos(nAxis, 0.0);
	AxmHomeSetResult(nAxis, HOME_SUCCESS);
#endif
}

void CAJinAXL::Move_Absolute(int nAxis, double dPos)
{
#ifdef AJIN_BOARD_USE
	AxmMotSetAbsRelMode(nAxis, POS_ABS_MODE);
	double	dVel = m_Param[nAxis].dSpeedM;
	double	dAcc = m_Param[nAxis].dAccel;
	AxmMoveStartPos(nAxis, dPos, dVel, dAcc, dAcc);
	m_Status[nAxis].bInP = FALSE; m_Status[nAxis].bRun = TRUE;
#endif
}

void CAJinAXL::Move_AbsSlow(int nAxis, double dPos, double dRatio)
{
#ifdef AJIN_BOARD_USE
	AxmMotSetAbsRelMode(nAxis, POS_ABS_MODE);
	double	dVel = m_Param[nAxis].dSpeedM * dRatio;
	double	dAcc = m_Param[nAxis].dAccel * dRatio;
	AxmMoveStartPos(nAxis, dPos, dVel, dAcc, dAcc);
	m_Status[nAxis].bInP = FALSE; m_Status[nAxis].bRun = TRUE;
#endif
}

void CAJinAXL::Move_Relative(int nAxis, double dPos)
{
#ifdef AJIN_BOARD_USE
	if (nAxis == AX_INDEX_R) {
		AxmStatusSetActPos(nAxis, 0.0);
		AxmStatusSetCmdPos(nAxis, 0.0);
	}
	AxmMotSetAbsRelMode(nAxis, POS_REL_MODE);
	double	dVel = m_Param[nAxis].dSpeedM;
	double	dAcc = m_Param[nAxis].dAccel;
	AxmMoveStartPos(nAxis, dPos, dVel, dAcc, dAcc);
	m_Status[nAxis].bInP = FALSE; m_Status[nAxis].bRun = TRUE;
#endif
}

void CAJinAXL::Jog_Positive(int nAxis)
{
#ifdef AJIN_BOARD_USE
	double	dVel = m_Param[nAxis].dSpeedJ;
	double	dAcc = m_Param[nAxis].dSpeedJ * 4.0;
	AxmMoveVel(nAxis, dVel, dAcc, dAcc);
#endif
}

void CAJinAXL::Jog_Negative(int nAxis)
{
#ifdef AJIN_BOARD_USE
	double	dVel = m_Param[nAxis].dSpeedJ * -1.0;
	double	dAcc = m_Param[nAxis].dSpeedJ * 4.0;
	AxmMoveVel(nAxis, dVel, dAcc, dAcc);
#endif
}

void CAJinAXL::Stop_Motion(int nAxis)
{
#ifdef AJIN_BOARD_USE
	AxmMoveSStop(nAxis);
#endif
}

void CAJinAXL::EStop_Motion(int nAxis)
{
#ifdef AJIN_BOARD_USE
	AxmMoveEStop(nAxis);
#endif
}

void CAJinAXL::Alarm_Reset(int nAxis)
{
#ifdef AJIN_BOARD_USE
	int nAxStart = (nAxis == -1 ? 0 : nAxis);
	int nAxEnd = (nAxis == -1 ? AXIS_COUNT : nAxis + 1);
	for (int i = nAxStart; i < nAxEnd; i++) AxmMoveEStop(i);
	Sleep(200);
	for (int i = nAxStart; i < nAxEnd; i++) AxmSignalServoAlarmReset(i, TRUE);
	Sleep(200);
	for (int i = nAxStart; i < nAxEnd; i++) AxmSignalServoAlarmReset(i, FALSE);
#endif
}

void CAJinAXL::Move_Abs_Override(int nAxis, double dPos, double dAt)
{
#ifdef AJIN_BOARD_USE
	AxmMotSetAbsRelMode(nAxis, POS_ABS_MODE);
	double	dVel = m_Param[nAxis].dSpeedM;
	double	dAcc = m_Param[nAxis].dSpeedM * 4.0;
	double dMaxVel;
	AxmMotGetMaxVel(nAxis, &dMaxVel);

	AxmOverrideSetMaxVel(nAxis, dMaxVel);	// 오버라이드 최대값 설정
	AxmOverrideVelAtPos (nAxis, dPos, dVel, dAcc, dAcc, dAt, dVel/4.0, COMMAND);
	m_Status[nAxis].bInP = FALSE; m_Status[nAxis].bRun = TRUE;
#endif
}

void CAJinAXL::Move_Rel_Override(int nAxis, double dPos, double dAt)
{
#ifdef AJIN_BOARD_USE
	AxmMotSetAbsRelMode(nAxis, POS_REL_MODE);
	double	dVel = m_Param[nAxis].dSpeedM;
	double	dAcc = m_Param[nAxis].dSpeedM * 4.0;
	double dMaxVel;
	AxmMotGetMaxVel(nAxis, &dMaxVel);
	
	AxmOverrideSetMaxVel(nAxis, dMaxVel);	// 오버라이드 최대값 설정
	AxmOverrideVelAtPos (nAxis, dPos, dVel, dAcc, dAcc, dAt, dVel/4.0, COMMAND);
	m_Status[nAxis].bInP = FALSE; m_Status[nAxis].bRun = TRUE;
#endif
}

BOOL CAJinAXL::Is_Done(int nAxis)
{
#ifdef AJIN_BOARD_USE
	if (m_Status[nAxis].bRun) return FALSE;
	if (!m_Status[nAxis].bInP) return FALSE;
#endif
	return TRUE;
}

BOOL CAJinAXL::Is_Home(int nAxis)
{
#ifdef AJIN_BOARD_USE
	if (!m_Status[nAxis].bHom) return FALSE;
	if (m_Status[nAxis].bRun) return FALSE;
#endif
	return TRUE;
}

BOOL CAJinAXL::Is_MoveDone(int nAxis, double dPos, double dRange)
{
#ifdef AJIN_BOARD_USE
	if (!Is_Done(nAxis)) return FALSE;
	if (fabs(m_Status[nAxis].dPos - dPos) > dRange) return FALSE;
#endif
	return TRUE;
}

void CAJinAXL::Start_Trigger(int nAxis, double dStartPos, double dEndPos, double dPeriod, double dWidth)
{
#ifdef AJIN_BOARD_USE
	AxmTriggerSetReset(nAxis);

	double dTrigTime = dWidth / m_Param[nAxis].dSpeedM * 1000000.0;	// mm->usec

	// 1. Command Position ****************************************************
// 	AxmTriggerSetTimeLevel(nAxis, dTrigTime, HIGH, COMMAND, DISABLE);
	// 2. Actual Position *****************************************************
	AxmTriggerSetTimeLevel(nAxis, dTrigTime, HIGH, ACTUAL, DISABLE);
	//*************************************************************************
	
	DWORD dwCode = AxmTriggerSetBlock(nAxis, dStartPos, dEndPos, dPeriod);
	if (dwCode != AXT_RT_SUCCESS) AfxMessageBox("Trigger Setting Error");
#endif
}

void CAJinAXL::Stop_Trigger(int nAxis)
{
#ifdef AJIN_BOARD_USE
	AxmTriggerSetReset(nAxis);
#endif
}

void CAJinAXL::Start_Scan(int nAxis, double dPos, double dVel, double dTrigStart, double dTrigEnd, double dPeriod, double dWidth)
{
#ifdef AJIN_BOARD_USE
	// Trigger Setting
	AxmTriggerSetReset(nAxis);
	
	dPeriod /= 1000;
	dWidth /= 1000;

	double dTrigTime = dWidth / dVel * 1000000.0;	// mm->usec

	//dTrigTime = 20.0;	// mm->usec

	// 1. Command Position ****************************************************
// 	AxmTriggerSetTimeLevel(nAxis, dTrigTime, HIGH, COMMAND, DISABLE);
	// 2. Actual Position *****************************************************
	AxmTriggerSetTimeLevel(nAxis, dTrigTime, HIGH, ACTUAL, DISABLE);
	//*************************************************************************
	
	DWORD dwCode = AxmTriggerSetBlock(nAxis, dTrigStart, dTrigEnd, dPeriod);
	if (dwCode != AXT_RT_SUCCESS) AfxMessageBox("Trigger Setting Error");

	// Scan Move
	AxmMotSetAbsRelMode(nAxis, POS_ABS_MODE);
	double	dAcc = dVel * 10.0;
	AxmMoveStartPos(nAxis, dPos, dVel, dAcc, dAcc);
	m_Status[nAxis].bInP = FALSE; m_Status[nAxis].bRun = TRUE;
#endif
}

void CAJinAXL::Stop_Scan(int nAxis)
{
#ifdef AJIN_BOARD_USE
	AxmTriggerSetReset(nAxis);
#endif
}

/////////////////////////////////////////////////////////////////////////////
// Motion Param Read / Write Function

void CAJinAXL::Read_AxisList()
{
	CIniFileCS INI(gsCurrentDir + "\\System\\AxisList.ini");
	if (!INI.Check_File()) {
		AfxMessageBox("AxisList.ini File Not Found!!!");
		return;
	}

	CString strSection, strName;
	for (int i = 0; i < AXIS_COUNT; i++) {
		strSection.Format("AXIS_%02d", i);

		strName = INI.Get_String(strSection, "NAME", "");
		m_strAxisName[i].Format("[%02d] %s", i, strName);
		m_Param[i].dSpeedM = INI.Get_Double(strSection, "MOVE", 0.0);
		m_Param[i].dSpeedJ = INI.Get_Double(strSection, "JOG", 0.0);
		m_Param[i].dAccel = INI.Get_Double(strSection, "ACC", 0.0);
	}
}

void CAJinAXL::Read_MotionParam(int nAxis)
{
	CIniFileCS INI(gsCurrentDir + "\\System\\AxisList.ini");
	if (!INI.Check_File()) {
		AfxMessageBox("AxisList.ini File Not Found!!!");
		return;
	}
	
	CString strSection;
	strSection.Format("AXIS_%02d", nAxis);
	m_Param[nAxis].dSpeedM = INI.Get_Double(strSection, "MOVE", 0.0);
	m_Param[nAxis].dSpeedJ = INI.Get_Double(strSection, "JOG", 0.0);
	m_Param[nAxis].dAccel = INI.Get_Double(strSection, "ACC", 0.0);
}

void CAJinAXL::Save_MotionParam(int nAxis, double dSpeedM, double dSpeedJ, double dAccel)
{
	CIniFileCS INI(gsCurrentDir + "\\System\\AxisList.ini");
	if (!INI.Check_File()) {
		AfxMessageBox("AxisList.ini File Not Found!!!");
		return;
	}
	
	CString strSection;
	strSection.Format("AXIS_%02d", nAxis);
	INI.Set_Double(strSection, "MOVE", dSpeedM, "%0.3lf");
	INI.Set_Double(strSection, "JOG", dSpeedJ, "%0.3lf");
	INI.Set_Double(strSection, "ACC", dAccel, "%0.3lf");

	// RMS 항목
	Save_RmsMotionSpeed(nAxis, dSpeedM, dAccel);
}

void CAJinAXL::Save_RmsMotionSpeed(int nAxis, double dSpeedM, double dAccel)
{	
	CString strAxis, strSpeed, strAccel;
	strSpeed.Format("%0.3lf", dSpeedM);
	strAccel.Format("%0.3lf", dAccel);
	
	if (nAxis == AX_TRAY_PICKER_X) {
		strAxis.Format("Cap-Tray Picker X Speed"); g_objMES.Save_AviRmsData(strAxis, strSpeed);
		strAxis.Format("Cap-Tray Picker X Accel"); g_objMES.Save_AviRmsData(strAxis, strAccel);

	} else if (nAxis == AX_TRAY_PICKER_Z) {
		strAxis.Format("Cap-Tray Picker Z Speed"); g_objMES.Save_AviRmsData(strAxis, strSpeed);
		strAxis.Format("Cap-Tray Picker Z Accel"); g_objMES.Save_AviRmsData(strAxis, strAccel);

	} else if (nAxis == AX_TRAY_PICKER_R) {
		strAxis.Format("Cap-Tray Picker R Speed"); g_objMES.Save_AviRmsData(strAxis, strSpeed);
		strAxis.Format("Cap-Tray Picker R Accel"); g_objMES.Save_AviRmsData(strAxis, strAccel);

	} else if (nAxis == AX_LOAD_STAGE1_X) {
		strAxis.Format("Cap-Load Stage1 X Speed"); g_objMES.Save_AviRmsData(strAxis, strSpeed);
		strAxis.Format("Cap-Load Stage1 X Accel"); g_objMES.Save_AviRmsData(strAxis, strAccel);

	} else if (nAxis == AX_LOAD_STAGE1_Z)	{
		strAxis.Format("Cap-Load Stage1 Z Speed"); g_objMES.Save_AviRmsData(strAxis, strSpeed);
		strAxis.Format("Cap-Load Stage1 Z Accel"); g_objMES.Save_AviRmsData(strAxis, strAccel);

	} else if (nAxis == AX_LOAD_STAGE2_X)	{
		strAxis.Format("Cap-Load Stage2 X Speed"); g_objMES.Save_AviRmsData(strAxis, strSpeed);
		strAxis.Format("Cap-Load Stage2 X Accel"); g_objMES.Save_AviRmsData(strAxis, strAccel);

	} else if (nAxis == AX_LOAD_STAGE2_Z)	{
		strAxis.Format("Cap-Load Stage2 Z Speed"); g_objMES.Save_AviRmsData(strAxis, strSpeed);
		strAxis.Format("Cap-Load Stage2 Z Accel"); g_objMES.Save_AviRmsData(strAxis, strAccel);

	} else if (nAxis == AX_LOAD_PICKER_Y) {
		strAxis.Format("Cap-Load Picker Y Speed"); g_objMES.Save_AviRmsData(strAxis, strSpeed);
		strAxis.Format("Cap-Load Picker Y Accel"); g_objMES.Save_AviRmsData(strAxis, strAccel);

	} else if (nAxis == AX_LOAD_PICKER_Z)	{
		strAxis.Format("Cap-Load Picker Z Speed"); g_objMES.Save_AviRmsData(strAxis, strSpeed);
		strAxis.Format("Cap-Load Picker Z Accel"); g_objMES.Save_AviRmsData(strAxis, strAccel);

	} else if (nAxis == AX_LOAD_PICKER_P) {
		strAxis.Format("Cap-Load Picker P Speed"); g_objMES.Save_AviRmsData(strAxis, strSpeed);
		strAxis.Format("Cap-Load Picker P Accel"); g_objMES.Save_AviRmsData(strAxis, strAccel);

	} else if (nAxis == AX_INDEX_R) {
		strAxis.Format("Cap-Index R Speed"); g_objMES.Save_AviRmsData(strAxis, strSpeed);
		strAxis.Format("Cap-Index R Accel"); g_objMES.Save_AviRmsData(strAxis, strAccel);

	} else if (nAxis == AX_VISION_CM_ALIGN_X) {
		strAxis.Format("Cap-CM Vision X Speed"); g_objMES.Save_AviRmsData(strAxis, strSpeed);
		strAxis.Format("Cap-CM Vision X Accel"); g_objMES.Save_AviRmsData(strAxis, strAccel);

	} else if (nAxis == AX_CAP_STAGE1_X) {
		strAxis.Format("Cap-Cap Stage1 X Speed"); g_objMES.Save_AviRmsData(strAxis, strSpeed);
		strAxis.Format("Cap-Cap Stage1 X Accel"); g_objMES.Save_AviRmsData(strAxis, strAccel);

	} else if (nAxis == AX_CAP_STAGE1_Z) {
		strAxis.Format("Cap-Cap Stage1 Z Speed"); g_objMES.Save_AviRmsData(strAxis, strSpeed);
		strAxis.Format("Cap-Cap Stage1 Z Accel"); g_objMES.Save_AviRmsData(strAxis, strAccel);

	} else if (nAxis == AX_CAP_STAGE2_X) {
		strAxis.Format("Cap-Cap Stage2 X Speed"); g_objMES.Save_AviRmsData(strAxis, strSpeed);
		strAxis.Format("Cap-Cap Stage2 X Accel"); g_objMES.Save_AviRmsData(strAxis, strAccel);

	} else if (nAxis == AX_CAP_STAGE2_Z) {
		strAxis.Format("Cap-Cap Stage2 Z Speed"); g_objMES.Save_AviRmsData(strAxis, strSpeed);
		strAxis.Format("Cap-Cap Stage2 Z Accel"); g_objMES.Save_AviRmsData(strAxis, strAccel);

	} else if (nAxis == AX_CAP_PICKER_Y) {
		strAxis.Format("Cap-Cap Picker Y Speed"); g_objMES.Save_AviRmsData(strAxis, strSpeed);
		strAxis.Format("Cap-Cap Picker Y Accel"); g_objMES.Save_AviRmsData(strAxis, strAccel);

	} else if (nAxis == AX_CAP_PICKER_Z)	{
		strAxis.Format("Cap-Cap Picker Z Speed"); g_objMES.Save_AviRmsData(strAxis, strSpeed);
		strAxis.Format("Cap-Cap Picker Z Accel"); g_objMES.Save_AviRmsData(strAxis, strAccel);

	} else if (nAxis == AX_CAP_PICKER_P)	{
		strAxis.Format("Cap-Cap Picker P Speed"); g_objMES.Save_AviRmsData(strAxis, strSpeed);
		strAxis.Format("Cap-Cap Picker P Accel"); g_objMES.Save_AviRmsData(strAxis, strAccel);

	} else if (nAxis == AX_CAP_BUFFER_STAGE_Y) {
		strAxis.Format("Cap-Cap Buffer Y Speed"); g_objMES.Save_AviRmsData(strAxis, strSpeed);
		strAxis.Format("Cap-Cap Buffer Y Accel"); g_objMES.Save_AviRmsData(strAxis, strAccel);

	} else if (nAxis == AX_VISION_CAP_ALIGN_Y) {
		strAxis.Format("Cap-Cap Vision Y Speed"); g_objMES.Save_AviRmsData(strAxis, strSpeed);
		strAxis.Format("Cap-Cap Vision Y Accel"); g_objMES.Save_AviRmsData(strAxis, strAccel);

	} else if (nAxis == AX_ASSY_PICKER_X) {
		strAxis.Format("Cap-Assembly Picker X Speed"); g_objMES.Save_AviRmsData(strAxis, strSpeed);
		strAxis.Format("Cap-Assembly Picker X Accel"); g_objMES.Save_AviRmsData(strAxis, strAccel);

	} else if (nAxis == AX_ASSY_PICKER_Y) {
		strAxis.Format("Cap-Assembly Picker Y Speed"); g_objMES.Save_AviRmsData(strAxis, strSpeed);
		strAxis.Format("Cap-Assembly Picker Y Accel"); g_objMES.Save_AviRmsData(strAxis, strAccel);

	} else if (nAxis == AX_ASSY_PICKER_Z) {
		strAxis.Format("Cap-Assembly Picker Z Speed"); g_objMES.Save_AviRmsData(strAxis, strSpeed);
		strAxis.Format("Cap-Assembly Picker Z Accel"); g_objMES.Save_AviRmsData(strAxis, strAccel);
		g_objMES.Save_AviRmsData("Cap Attach Speed", strSpeed);

	} else if (nAxis == AX_TRANS_STAGE_X) {
		strAxis.Format("Cap-Transfer Stage X Speed"); g_objMES.Save_AviRmsData(strAxis, strSpeed);
		strAxis.Format("Cap-Transfer Stage X Accel"); g_objMES.Save_AviRmsData(strAxis, strAccel);

	} else if (nAxis == AX_TRANS_STAGE_Z) {
		strAxis.Format("Cap-Transfer Stage Z Speed"); g_objMES.Save_AviRmsData(strAxis, strSpeed);
		strAxis.Format("Cap-Transfer Stage Z Accel"); g_objMES.Save_AviRmsData(strAxis, strAccel);

	} else if (nAxis == AX_TRANS_STAGE_T) {
		strAxis.Format("Cap-Transfer Stage T Speed"); g_objMES.Save_AviRmsData(strAxis, strSpeed);
		strAxis.Format("Cap-Transfer Stage T Accel"); g_objMES.Save_AviRmsData(strAxis, strAccel);

	} else if (nAxis == AX_UNLOAD_PICKER_X) {
		strAxis.Format("Cap-Unload Picker X Speed"); g_objMES.Save_AviRmsData(strAxis, strSpeed);
		strAxis.Format("Cap-Unload Picker X Accel"); g_objMES.Save_AviRmsData(strAxis, strAccel);

	} else if (nAxis == AX_UNLOAD_PICKER_Z)	{
		strAxis.Format("Cap-Unload Picker Z Speed"); g_objMES.Save_AviRmsData(strAxis, strSpeed);
		strAxis.Format("Cap-Unload Picker Z Accel"); g_objMES.Save_AviRmsData(strAxis, strAccel);

	} else if (nAxis == AX_UNLOAD_PICKER_P) {
		strAxis.Format("Cap-Unload Picker P Speed"); g_objMES.Save_AviRmsData(strAxis, strSpeed);
		strAxis.Format("Cap-Unload Picker P Accel"); g_objMES.Save_AviRmsData(strAxis, strAccel);

	} else if (nAxis == AX_UNLOAD_STAGE1_Y) {
		strAxis.Format("Cap-Unload Stage1 Y Speed"); g_objMES.Save_AviRmsData(strAxis, strSpeed);
		strAxis.Format("Cap-Unload Stage1 Y Accel"); g_objMES.Save_AviRmsData(strAxis, strAccel);

	} else if (nAxis == AX_UNLOAD_STAGE1_Z) {
		strAxis.Format("Cap-Unload Stage1 Z Speed"); g_objMES.Save_AviRmsData(strAxis, strSpeed);
		strAxis.Format("Cap-Unload Stage1 Z Accel"); g_objMES.Save_AviRmsData(strAxis, strAccel);

	} else if (nAxis == AX_UNLOAD_STAGE2_Y)	{
		strAxis.Format("Cap-Unload Stage2 Y Speed"); g_objMES.Save_AviRmsData(strAxis, strSpeed);
		strAxis.Format("Cap-Unload Stage2 Y Accel"); g_objMES.Save_AviRmsData(strAxis, strAccel);

	} else if (nAxis == AX_UNLOAD_STAGE2_Z)	{
		strAxis.Format("Cap-Unload Stage2 Z Speed"); g_objMES.Save_AviRmsData(strAxis, strSpeed);
		strAxis.Format("Cap-Unload Stage2 Z Accel"); g_objMES.Save_AviRmsData(strAxis, strAccel);
	}
}
/////////////////////////////////////////////////////////////////////////////
// Thread Function 
UINT CAJinAXL::Thread_AJin(LPVOID lpVoid)
{
	while (g_objAJinAXL.m_bThreadAJin) {
		g_objAJinAXL.Read_Input();
		g_objAJinAXL.Read_MotionStatus();
		Sleep(5);
	}
	g_objAJinAXL.m_bThreadAJin = FALSE;
	g_objAJinAXL.m_pThreadAJin = NULL;
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
BOOL CAJinAXL::Is_AbsoluteType(int nAxis)
{
	switch (nAxis) {
	case AX_LOAD_STAGE1_Z:
	case AX_LOAD_STAGE2_Z:
	case AX_CAP_STAGE1_Z:
	case AX_CAP_STAGE2_Z:
	case AX_UNLOAD_STAGE1_Z:
	case AX_UNLOAD_STAGE2_Z:
		return TRUE;
	default:
		return FALSE;
	}
}

BOOL CAJinAXL::Use_OrgAxis(int nAxis)
{
	switch (nAxis) {
	case AX_INDEX_R:
	case AX_TRANS_STAGE_T:
		return TRUE;
	default:
		return FALSE;
	}
}

BOOL CAJinAXL::Use_ElpAxis(int nAxis)
{
	if (Use_OrgAxis(nAxis)) return FALSE;
	else return TRUE;
}

BOOL CAJinAXL::Use_ElnAxis(int nAxis)
{
	if (Use_OrgAxis(nAxis)) return FALSE;
	else return TRUE;
}

///////////////////////////////////////////////////////////////////////////////

DXY_DATA *CAJinAXL::Get_pDX(int nIndex)
{
	if (nIndex ==  0) return (DXY_DATA*)&m_DX00;
	if (nIndex ==  1) return (DXY_DATA*)&m_DX01;
	if (nIndex ==  2) return (DXY_DATA*)&m_DX02;
	if (nIndex ==  3) return (DXY_DATA*)&m_DX03;
	if (nIndex ==  4) return (DXY_DATA*)&m_DX04;
	if (nIndex ==  5) return (DXY_DATA*)&m_DX05;
	if (nIndex ==  6) return (DXY_DATA*)&m_DX06;
	if (nIndex ==  7) return (DXY_DATA*)&m_DX07;
	if (nIndex ==  8) return (DXY_DATA*)&m_DX08;
	if (nIndex ==  9) return (DXY_DATA*)&m_DX09;
	if (nIndex == 10) return (DXY_DATA*)&m_DX10;
	if (nIndex == 11) return (DXY_DATA*)&m_DX11;
	if (nIndex == 12) return (DXY_DATA*)&m_DX12;
	if (nIndex == 13) return (DXY_DATA*)&m_DX13;
	return NULL;
}

DXY_DATA *CAJinAXL::Get_pDY(int nIndex)
{
	if (nIndex ==  0) return (DXY_DATA*)&m_DY00;
	if (nIndex ==  1) return (DXY_DATA*)&m_DY01;
	if (nIndex ==  2) return (DXY_DATA*)&m_DY02;
	if (nIndex ==  3) return (DXY_DATA*)&m_DY03;
	if (nIndex ==  4) return (DXY_DATA*)&m_DY04;
	if (nIndex ==  5) return (DXY_DATA*)&m_DY05;
	if (nIndex ==  6) return (DXY_DATA*)&m_DY06;
	if (nIndex ==  7) return (DXY_DATA*)&m_DY07;
	if (nIndex ==  8) return (DXY_DATA*)&m_DY08;
	if (nIndex ==  9) return (DXY_DATA*)&m_DY09;
	if (nIndex == 10) return (DXY_DATA*)&m_DY10;
	if (nIndex == 11) return (DXY_DATA*)&m_DY11;
	if (nIndex == 12) return (DXY_DATA*)&m_DY12;
	if (nIndex == 13) return (DXY_DATA*)&m_DY13;
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////

void CAJinAXL::Save_AxisList()
{
	CIniFileCS INI(gsCurrentDir + "\\System\\AxisList.ini");
	if (!INI.Check_File()) {
		AfxMessageBox("AxisList.ini File Not Found!!!");
		return;
	}

	CString strAxis[46];

	strAxis[0 ] = "LOAD STAGE X";
	strAxis[1 ] = "LOAD STAGE Z";
	strAxis[2 ] = "LOAD PICKER Y";
	strAxis[3 ] = "ANGLE UNIT Y";
	strAxis[4 ] = "ANGLE UNIT X";
	strAxis[5 ] = "ANGLE UNIT R";
	strAxis[6 ] = "VISION ANGLE Z";
	strAxis[7 ] = "VISION ALIGN Z";
	strAxis[8 ] = "ANGLE STAGE1 Y";
	strAxis[9 ] = "ANGLE STAGE1 Z";
	strAxis[10] = "ANGLE STAGE2 Y";
	strAxis[11] = "ANGLE STAGE2 Z";
	strAxis[12] = "BTM1 PICKER X";
	strAxis[13] = "BTM1 PICKER Z";
	strAxis[14] = "BTM1 PICKER P1";
	strAxis[15] = "BTM1 PICKER P2";
	strAxis[16] = "BTM1 SHIFT Y";
	strAxis[17] = "INSPECT STAGE1 X";
	strAxis[18] = "INSPECT STAGE2 X";
	strAxis[19] = "INSPECT STAGE3 X";
	strAxis[20] = "MODULE ALIGN Y";
	strAxis[21] = "MODULE ALIGN Z";
	strAxis[22] = "VISION TOP1 Z";
	strAxis[23] = "TOP1 MIRROR Z";
	strAxis[24] = "TOP2 SHIFT Y";
	strAxis[25] = "VISION TOP2 Z";
	strAxis[26] = "BTM2 PICKER X";
	strAxis[27] = "BTM2 PICKER Z";
	strAxis[28] = "BTM2 PICKER P1";
	strAxis[29] = "BTM2 PICKER P2";
	strAxis[30] = "BUFFER STAGE1 Y";
	strAxis[31] = "BUFFER STAGE2 Y";
	strAxis[32] = "SORT PICKER1 X";
	strAxis[33] = "SORT PICKER1 Z";
	strAxis[34] = "SORT PICKER1 P";
	strAxis[35] = "SORT PICKER2 X";
	strAxis[36] = "SORT PICKER2 Z";
	strAxis[37] = "SORT PICKER2 P";
	strAxis[38] = "GOOD STAGE1 Y";
	strAxis[39] = "GOOD STAGE1 Z";
	strAxis[40] = "GOOD STAGE2 Y";
	strAxis[41] = "GOOD STAGE2 Z";
	strAxis[42] = "NG STAGE Y";
	strAxis[43] = "EMPTY TRANS1 X";
	strAxis[44] = "EMPTY PORT Z";
	strAxis[45] = "EMPTY TRANS2 Y";

	CString strSection, strName;
	for (int i = 0; i < AXIS_COUNT; i++) {
		strSection.Format("AXIS_%02d", i);
		INI.Set_String(strSection, "NAME", strAxis[i]);
	}
}

///////////////////////////////////////////////////////////////////////////////
