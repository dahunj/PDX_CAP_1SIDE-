// CME8000.h : PROJECT_NAME 응용 프로그램에 대한 주 헤더 파일입니다.
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"		// 주 기호입니다.

#define	MODE_OPERATOR		0
#define MODE_INITIAL		1
#define MODE_WORK			2
#define MODE_MANUAL			3
#define MODE_SETUP			4
#define MODE_PROHIBIT		5
#define MODE_PARAM			6
#define MODE_ALARM			7

#define STATE_NONE			0
#define STATE_INIT			1
#define STATE_STOP			2
#define STATE_RUN			3
#define STATE_ALARM			4
#define STATE_ERROR			5
#define STATE_LOTEND		6
#define STATE_CAPTRAY		7
#define STATE_SHIPTRAY		8

// CCME8000App:
// 이 클래스의 구현에 대해서는 CME8000.cpp을 참조하십시오.
//
class CCME8000App : public CWinApp
{
public:
	CCME8000App();

// 재정의입니다.
public:
	virtual BOOL InitInstance();

// 구현입니다.
	DECLARE_MESSAGE_MAP()

private:
	int m_nMainMode;
	int m_nMainState;

public:
	void Set_MainMode(int nMode) { m_nMainMode = nMode; }
	int  Get_MainMode() { return m_nMainMode; }

	void Set_MainState(int nState) { m_nMainState = nState; }
	int  Get_MainState() { return m_nMainState; }

	BOOL bParamMode;
	BOOL bIoMode;
	BOOL bAlarmMode;

	void DoEvents();
	void uSleep(int msec);
};

extern CCME8000App theApp;
