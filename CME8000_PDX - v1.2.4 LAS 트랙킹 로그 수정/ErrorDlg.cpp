// ErrorDlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CME8000.h"
#include "ErrorDlg.h"
#include "afxdialogex.h"

#include "LogFile.h"
#include "Common.h"

#include "MESInterface.h"
#include "SequenceInit.h"
#include "SequenceMain.h"

#include "WorkDlg.h"
#include "ManualDlg.h"
#include "CME8000Dlg.h"

// CErrorDlg 대화 상자입니다.
CErrorDlg g_dlgError;

IMPLEMENT_DYNAMIC(CErrorDlg, CDialogEx)

CErrorDlg::CErrorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CErrorDlg::IDD, pParent)
{
}

CErrorDlg::~CErrorDlg()
{
}

void CErrorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_GROUP_0 + i, m_Group[i]);
	for (int i = 0; i < 8; i++) DDX_Control(pDX, IDC_LABEL_0 + i, m_Label[i]);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_STC_ERR_BACK_0 + i, m_stcErrBack[i]);
	DDX_Control(pDX, IDC_STC_ERR_TITLE, m_stcErrTitle);
	DDX_Control(pDX, IDC_IMAGE_0, m_Image);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_LED_MAIN_AIR_0 + i, m_ledMainAir[i]);
	for (int i = 0; i < 3; i++) DDX_Control(pDX, IDC_LED_EMG_SW_0 + i, m_ledEmgSw[i]);
	for (int i = 0; i < 13; i++) DDX_Control(pDX, IDC_LED_DOOR_OPEN_0 + i, m_ledDoorOpen[i]);
	for (int i = 0; i < 14; i++) DDX_Control(pDX, IDC_STC_ERR_POS_0 + i, m_stcErrPos[i]);
	DDX_Control(pDX, IDC_STC_ERR_NO, m_stcErrNo);
	DDX_Control(pDX, IDC_STC_MSG_BACK, m_stcMsgBack);
	DDX_Control(pDX, IDC_STC_ERR_MSG, m_stcErrMsg);
	DDX_Control(pDX, IDC_BTN_ERR_BUZZ_OFF, m_btnErrBuzzOff);
	DDX_Control(pDX, IDC_BTN_ERR_SKIP, m_btnErrSkip);
	DDX_Control(pDX, IDC_BTN_ERR_RETRY, m_btnErrRetry);
	DDX_Control(pDX, IDC_BTN_ERR_OK, m_btnErrOK);
	DDX_Control(pDX, IDC_BTN_ERR_SYSTEM_EXIT, m_btnErrSystemExit);
	DDX_Control(pDX, IDC_BTN_ERR_TO_MANUAL, m_btnErrToManual);
}

BEGIN_MESSAGE_MAP(CErrorDlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_ERR_BUZZ_OFF, &CErrorDlg::OnBnClickedBtnErrBuzzOff)
	ON_BN_CLICKED(IDC_BTN_ERR_SKIP, &CErrorDlg::OnBnClickedBtnErrSkip)
	ON_BN_CLICKED(IDC_BTN_ERR_RETRY, &CErrorDlg::OnBnClickedBtnErrRetry)
	ON_BN_CLICKED(IDC_BTN_ERR_OK, &CErrorDlg::OnBnClickedBtnErrOk)
	ON_BN_CLICKED(IDC_BTN_ERR_SYSTEM_EXIT, &CErrorDlg::OnBnClickedBtnErrSystemExit)
	ON_BN_CLICKED(IDC_BTN_ERR_TO_MANUAL, &CErrorDlg::OnBnClickedBtnErrToManual)
END_MESSAGE_MAP()

// CErrorDlg 메시지 처리기입니다.

BOOL CErrorDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	Initial_Controls();

	m_nErrNo = 0;
	m_nErrCode = BASE_ERR_CODE;	// Cap Attach Type Error Code 변경 10000 ~ 19999
	m_nBackColorLoop = 0;
	m_nErrorPos = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CErrorDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CErrorDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	CCME8000Dlg *pMainDlg = (CCME8000Dlg*)AfxGetApp()->GetMainWnd();
	if (bShow) {
		BringWindowToTop();	// 화면 위로...

		CString strErrNo, strErrCode, strErrMsg, strShow, strLog;
		CString strMes = "";
		int nNo = gData.nULPNo-1;
		if (nNo < 0) nNo = gData.nLPNo-1;
		if (nNo < 0) nNo = 0;

		pMainDlg->Set_CurrentState(STATE_ERROR);

		strErrNo.Format("%04d", m_nErrNo);
		strErrCode.Format("%05d", m_nErrCode);
		m_stcErrNo.SetWindowText(strErrCode);
		
		CIniFileCS INI(gsCurrentDir + "\\System\\ErrorList.ini");
		if (!INI.Check_File()) { AfxMessageBox("ErrorList.ini File Not Found!!!"); return; }
		strErrMsg = INI.Get_String("ERROR", strErrNo, "");

		CString strErrPick = "";
		if (m_nErrNo == 3401 || m_nErrNo == 3403 || m_nErrNo == 3404 || m_nErrNo == 3405 || m_nErrNo == 3417 || m_nErrNo == 3418 || m_nErrNo == 3419) {
			strErrPick.Format("#==> %d번 Picker를 확인 하여 주십시오.", gAlm.nLDPickerNo);
		}
		if (m_nErrNo == 3406 || m_nErrNo == 3407 || m_nErrNo == 3411 || m_nErrNo == 3412 || m_nErrNo == 3416) {
			strErrPick = g_objCommon.Get_LoadPickerCheckErr();
		}
		if (m_nErrNo == 3508) {
			strErrPick.Format("#==> %d번 Index를 확인 하여 주십시오.", gAlm.nIndexVacNo);
		}
		if (m_nErrNo == 4001 || m_nErrNo == 4004 || m_nErrNo == 4006 || m_nErrNo == 4007 || m_nErrNo == 4009 || m_nErrNo == 4010 || m_nErrNo == 4013 || m_nErrNo == 4014 || m_nErrNo == 4015 || m_nErrNo == 4016 || m_nErrNo == 4017 || m_nErrNo == 4018) {
			strErrPick.Format("#==> %d번 Picker를 확인 하여 주십시오.", gAlm.nCapPickeNo);
		}
		if (m_nErrNo == 4201 || m_nErrNo == 4202 || m_nErrNo == 4203 || m_nErrNo == 4205 || m_nErrNo == 4225 || m_nErrNo == 4226 || m_nErrNo == 4228 || m_nErrNo == 4229 || m_nErrNo == 4235 || m_nErrNo == 4236 || m_nErrNo == 4237 ||
			m_nErrNo == 4251 || m_nErrNo == 4255 || m_nErrNo == 4256 || m_nErrNo == 4262 || m_nErrNo == 4265 || m_nErrNo == 4266 || m_nErrNo == 4270 || m_nErrNo == 4271 || m_nErrNo == 4272 || m_nErrNo == 4274 || m_nErrNo == 4275) {
			strErrPick.Format("#==> %d번 Picker를 확인 하여 주십시오.", gAlm.nAssyPickNo);
		}
		if (m_nErrNo == 4229) {
			strErrPick = g_objCommon.Get_AssyPickerTiltErr();
		}
		if (m_nErrNo == 4273) {
			strErrPick = g_objCommon.Get_AssyPickerForceErr();
			g_objCommon.Move_Position(AX_ASSY_PICKER_Z, 0);
			g_objCommon.Set_AssyPickerUp(0);
			g_objSequenceMain.Set_MainRunCase(AUTO_ASSY_PICKER, 75);
		}
		if (m_nErrNo == 4401 || m_nErrNo == 4402 || m_nErrNo == 4405 || m_nErrNo == 4406 || m_nErrNo == 4410 || m_nErrNo == 4412 || m_nErrNo == 4413 || m_nErrNo == 4414 || m_nErrNo == 4415 || m_nErrNo == 4416) {
			strErrPick.Format("#==> %d번 Picker를 확인 하여 주십시오.", gAlm.nUNPickerNo);
		}

		if (m_nErrNo > 990 && m_nErrNo < 995) {
			if(m_nErrNo == 992) strMes.Format("==> Reason[%s] Text[%s]..", g_objMES.m_sReasonCode, g_objMES.m_sReasonText);
			if(m_nErrNo == 993) strMes.Format("==> User_CapLotID[%s] != MES_CapLotID[%s]..", g_objMES.m_sOperLotID, g_objMES.m_sMESLotID);
			if(m_nErrNo == 994) strMes.Format("==> User_CapCount[%d] != MES_CapCount[%d]..", g_objMES.m_nOperCount, g_objMES.m_nMESCount);
		}

		if (m_nErrNo == 3804 || m_nErrNo == 3904) {	// Cap 자재등록 실패
			strErrPick.Format("#==> Reason[%s] Text[%s]..", g_objMES.m_sReasonCode, g_objMES.m_sReasonText);
			if (m_nErrNo == 3804) g_objSequenceMain.Set_MainRunCase(AUTO_CAP_STAGE1, 3);	// Cap 자재등록하는 Case로 다시 보내준다.
			if (m_nErrNo == 3904) g_objSequenceMain.Set_MainRunCase(AUTO_CAP_STAGE2, 3);	// Cap 자재등록하는 Case로 다시 보내준다.
		}
		if (m_nErrNo == 4516 || m_nErrNo == 4616) {	// Ship 자재등록 실패
			strErrPick.Format("#==> Reason[%s] Text[%s]..", g_objMES.m_sReasonCode, g_objMES.m_sReasonText);
			if (m_nErrNo == 4516) g_objSequenceMain.Set_MainRunCase(AUTO_UNLOAD_STAGE1, 15);	// Ship 자재등록하는 Case로 다시 보내준다.
			if (m_nErrNo == 4616) g_objSequenceMain.Set_MainRunCase(AUTO_UNLOAD_STAGE2, 15);	// Ship 자재등록하는 Case로 다시 보내준다.
		}

		if (m_nErrNo == 3606 || m_nErrNo == 3706) m_btnErrToManual.SetWindowText("Skip");
		else									  m_btnErrToManual.SetWindowText("To Manual");

		if (m_nErrNo > 2 && m_nErrNo < 7) g_objSequenceInit.Set_InitComplete(FALSE);	// 3,4,5,6

		m_strErrMsg = strErrMsg + strErrPick + strMes;

		strShow = m_strErrMsg;
		if (strShow.Left(1) == "#") strShow.Delete(0);
		strShow.Replace("#", "\n\n");
		m_stcErrMsg.SetWindowText(strShow);

		SYSTEMTIME time;
		GetLocalTime(&time);
		for (int i = 2; i > 0; i--) gData.sAlarmTime[i] = gData.sAlarmTime[i - 1];
		for (int i = 2; i > 0; i--) gData.sAlarmList[i] = gData.sAlarmList[i - 1];
		gData.sAlarmTime[0].Format("%02d:%02d:%02d", time.wHour, time.wMinute, time.wSecond);
		gData.sAlarmList[0].Format("[%s] %s", strErrNo, m_strErrMsg);

		strLog.Format("%s,%s,%s", gData.sLotID[nNo], strErrNo, m_strErrMsg);
		g_objLogFile.Save_AlarmLog(strLog);
		
		Set_AlarmLog(m_nErrCode, m_strErrMsg);

		int *pCase = g_objSequenceMain.Get_pMainRunCase();
		strLog.Format("[Error Mode] Error No ==> %s Seq(%d-%d-%d-%d-%d  %d-%d-%d-%d-%d  %d-%d-%d-%d-%d  %d) Ver(%s)", strErrNo,
						*(pCase+0), *(pCase+1), *(pCase+2), *(pCase+3), *(pCase+4),
						*(pCase+5), *(pCase+6), *(pCase+7), *(pCase+8), *(pCase+9), 
						*(pCase+10), *(pCase+11), *(pCase+12), *(pCase+13), *(pCase+14), *(pCase+15), MAIN_VERSION);
		g_objLogFile.Save_HandlerLog(strLog);

		pMainDlg->Set_LotErrorLog("ERROR", m_nErrCode, m_strErrMsg);

		m_nBackColorLoop = 0;
		if		(m_nErrNo > 3100 && m_nErrNo < 3200) m_nErrorPos =  1;	//  1.Tray Picker
		else if (m_nErrNo > 3200 && m_nErrNo < 3400) m_nErrorPos =  2;	//  2.Load Stage
		else if (m_nErrNo > 3400 && m_nErrNo < 3500) m_nErrorPos =  3;	//  3.Load Picker

		else if (m_nErrNo > 3500 && m_nErrNo < 3800) m_nErrorPos =  4;	//  4.Main Index & Vision
		else if (m_nErrNo > 4200 && m_nErrNo < 4300) m_nErrorPos =  8;	//  8.Assy Picker

		else if (m_nErrNo > 3800 && m_nErrNo < 4000) m_nErrorPos =  5;	//  5.Cap Stage		
		else if (m_nErrNo > 4000 && m_nErrNo < 4100) m_nErrorPos =  6;	//  6.Cap Picker
		else if (m_nErrNo > 4100 && m_nErrNo < 4200) m_nErrorPos =  7;	//  7.Cap Buffer		
		
		else if (m_nErrNo > 4300 && m_nErrNo < 4400) m_nErrorPos =  9;	//  9.Trans Stage
		else if (m_nErrNo > 4400 && m_nErrNo < 4500) m_nErrorPos = 10;	// 10.Unload Picker
		else if (m_nErrNo > 4500 && m_nErrNo < 4700) m_nErrorPos = 11;	// 11.Unload Stage

		else										 m_nErrorPos =  0;
		m_stcErrPos[m_nErrorPos].Set_Color(RGB(0xFF, 0xFF, 0xFF), RGB(0xFF, 0x00, 0x00));

		SetTimer(0, 100, NULL);

	} else {
		KillTimer(0);

		for (int i = 0; i < 12; i++) m_stcErrPos[i].Set_Color(RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x00, 0x00));

		pMainDlg->Set_BuzzerFlicker(FALSE);
		if (g_objSequenceInit.Get_InitComplete()) pMainDlg->Set_CurrentState(STATE_STOP);
		else pMainDlg->Set_CurrentState(STATE_NONE);

		g_objLogFile.Save_HandlerLog("[Error Mode] Close Error");

		pMainDlg->Set_LotErrorLog("RESET", 905, "Error Reset");
	}
}

void CErrorDlg::OnTimer(UINT_PTR nIDEvent) 
{
	KillTimer(0);

	DX_DATA_12 *pDX12 = g_objAJinAXL.Get_pDX12();
	DX_DATA_13 *pDX13 = g_objAJinAXL.Get_pDX13();

	if (pDX12->iResetSw) {
		OnBnClickedBtnErrOk();
		return;
	}

	if (m_nBackColorLoop == 1) {
		for (int i = 0; i < 4; i++) m_stcErrBack[i].Set_Color(COLOR_DEFAULT, RGB(0x00, 0x00, 0xFF));
	} else if (m_nBackColorLoop == 6) {
		for (int i = 0; i < 4; i++) m_stcErrBack[i].Set_Color(COLOR_DEFAULT, RGB(0xFF, 0xFF, 0x00));
	} else if (m_nBackColorLoop == 10) m_nBackColorLoop = 0;
	m_nBackColorLoop++;

	m_ledMainAir[0].Set_On(pDX12->iMainAir1);
	m_ledMainAir[1].Set_On(pDX12->iMainAir2);

	m_ledEmgSw[0].Set_On(pDX12->iEmgSw1);
	m_ledEmgSw[1].Set_On(pDX12->iEmgSw2);
	m_ledEmgSw[2].Set_On(pDX12->iEmgSw3);

	for (int i = 0; i < 13; i++) m_ledDoorOpen[i].Set_On((pDX13->nValue >> (i + 18)) & 1);

	SetTimer(0, 100, NULL);
	CDialogEx::OnTimer(nIDEvent);
}

void CErrorDlg::OnBnClickedBtnErrBuzzOff()
{
	CCME8000Dlg *pMainDlg = (CCME8000Dlg*)AfxGetApp()->GetMainWnd();
	pMainDlg->Set_BuzzerFlicker(FALSE);
}

void CErrorDlg::OnBnClickedBtnErrSkip()
{
// 	g_objLogFile.Save_HandlerLog("[Error Mode] SKIP button push");
// 
// 	if (m_nErrNo == 3325) {		// Inspection Not Complete
// 	}
// 
// 	g_dlgWork.Set_AutoRun(TRUE);
// 
// 	ShowWindow(SW_HIDE);
}

void CErrorDlg::OnBnClickedBtnErrRetry()
{
// 	g_objLogFile.Save_HandlerLog("[Error Mode] RETRY button push");
// 
// 	switch (m_nErrNo) {
// 	}
// 
// 	ShowWindow(SW_HIDE);
// 	g_dlgWork.Set_AutoRun(TRUE);
}

void CErrorDlg::OnBnClickedBtnErrOk()
{
	g_objLogFile.Save_HandlerLog("[Error Mode] OK button push");
	ShowWindow(SW_HIDE);
}

void CErrorDlg::OnBnClickedBtnErrSystemExit()
{
	if (g_objCommon.Show_MsgBox(2, "Do you want to exit the program?") != IDOK) return;
	CCME8000Dlg *pMainDlg = (CCME8000Dlg*)AfxGetApp()->GetMainWnd();
	pMainDlg->Exit_System(EXIT_SYSTEM_AJIN);
}

void CErrorDlg::OnBnClickedBtnErrToManual()
{
	int nManualPos = 0;
	if (m_nErrorPos >   0 && m_nErrorPos <=   3) nManualPos = 1;	// Load 1, 2, 3
	if (m_nErrorPos ==  4 || m_nErrorPos ==   8) nManualPos = 2;	// Index 4, 8
	if (m_nErrorPos >=  5 && m_nErrorPos <    8) nManualPos = 3;	// Cap 5, 6, 7
	if (m_nErrorPos >   8 && m_nErrorPos <=  13) nManualPos = 4;	// Unload 9, 10, 11

	if (m_nErrNo == 3606){
		gData.nCmAlignSkip = TRUE;
		ShowWindow(SW_HIDE);
		CString strLog;
		strLog.Format("[Error Mode] CM Skip button push");
		g_objLogFile.Save_HandlerLog(strLog);
		return;
	}
	if (m_nErrNo == 3706) {
		gData.nCapAlignSkip = TRUE;
		ShowWindow(SW_HIDE);
		CString strLog;
		strLog.Format("[Error Mode] Cap Skip button push");
		g_objLogFile.Save_HandlerLog(strLog);
		return;
	}
	if (nManualPos == 0) return;

	ShowWindow(SW_HIDE);

	g_dlgManual.Set_ManualPos(nManualPos);
	CCME8000Dlg *pMainDlg = (CCME8000Dlg*)AfxGetApp()->GetMainWnd();
	pMainDlg->Set_CurrentMode(MODE_MANUAL);

	CString strLog;
	strLog.Format("[Error Mode] Manual button push - Pos[%d]", nManualPos);
	g_objLogFile.Save_HandlerLog(strLog);
}

///////////////////////////////////////////////////////////////////////////////
// User Functions

void CErrorDlg::Set_AlarmLog(int nErrNo, CString sErrMsg)
{
	SYSTEMTIME time;
	int nNo = gData.nLPNo-1;
	if (nNo < 0) nNo = 0;
	gAlm.bBegin = TRUE;

	gAlm.sLotID = gData.sLotID[nNo];
	gAlm.nAlmNo = nErrNo;
	gAlm.sAlmMsg = sErrMsg;
	GetLocalTime(&time);
	gAlm.dwStartTime = GetTickCount();
	gAlm.sStartTime.Format("%04d%02d%02d_%02d%02d%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);

	// 작업자 기인 알람으로 설비 멈췄을때.
	// MES 알람. (Door, Tray Empty/Full Alarm은 Alarm Dlg)
	if (m_nErrNo > 990 && m_nErrNo < 995) {
		CString sErr;
		sErr.Format("%04d(%s)", nErrNo, sErrMsg);
		g_dlgWork.MachineStopLog("ALARM", sErr);
	}
}

void CErrorDlg::Initial_Controls() 
{
	for (int i = 0; i < 4; i++) m_Group[i].Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0x00, 0xFF), COLOR_DEFAULT);
	for (int i = 0; i < 8; i++) m_Label[i].Init_Ctrl("바탕", 20, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x60, 0x60, 0x60));
	for (int i = 0; i < 4; i++) m_stcErrBack[i].Set_Color(COLOR_DEFAULT, RGB(0x00, 0x00, 0xFF));

	m_stcErrTitle.Init_Ctrl("바탕", 24, TRUE, RGB(0xFF, 0xFF, 0xFF),RGB(0xFF, 0x00, 0x00));

	m_bmpImage.LoadBitmap(IDB_EQUIP_ERROR);
	m_Image.SetBitmap(m_bmpImage);
// 	m_Image.SetWindowPos(NULL, 0, 0, 1060, 600, SWP_NOZORDER | SWP_NOMOVE);	// 원본 960x540 (1680x945)
	m_Image.SetWindowPos(NULL, 0, 0, 933, 677, SWP_NOZORDER | SWP_NOMOVE);	// 533x387 : 원본 800x580 (933x677)

	for (int i = 0; i < 2; i++) m_ledMainAir[i].Init_Ctrl("바탕", 11, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em24);
	for (int i = 0; i < 3; i++) m_ledEmgSw[i].Init_Ctrl("바탕", 11, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emRed, CLedCS::em24);
	for (int i = 0; i < 13; i++) m_ledDoorOpen[i].Init_Ctrl("바탕", 11, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em24);

	for (int i = 0; i < 14; i++) m_stcErrPos[i].Init_Ctrl("바탕", 12, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x00, 0x00));

	m_stcErrNo.Init_Ctrl("바탕", 30, TRUE, RGB(0x00, 0x00, 0xFF), RGB(0xFF, 0xF0, 0xE0));
	m_stcMsgBack.Init_Ctrl("바탕", 16, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xE0, 0xF0, 0xF0));
	m_stcErrMsg.Init_Ctrl("바탕", 16, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xE0, 0xF0, 0xF0));

	m_btnErrBuzzOff.Init_Ctrl("바탕", 16, TRUE, RGB(0x00, 0x00, 0x00), COLOR_DEFAULT, 0, 0);
	m_btnErrSkip.Init_Ctrl("바탕", 16, TRUE, RGB(0x00, 0x00, 0x00), COLOR_DEFAULT, 0, 0);
	m_btnErrRetry.Init_Ctrl("바탕", 16, TRUE, RGB(0x00, 0x00, 0x00), COLOR_DEFAULT, 0, 0);
	m_btnErrOK.Init_Ctrl("바탕", 16, TRUE, RGB(0x00, 0x00, 0x00), COLOR_DEFAULT, IDI_OK, CButtonCS::emLeft);
	m_btnErrSystemExit.Init_Ctrl("바탕", 16, TRUE, RGB(0x00, 0x00, 0x00), COLOR_DEFAULT, 0, 0);
	m_btnErrToManual.Init_Ctrl("바탕", 18, TRUE, RGB(0xFF, 0x00, 0xFF), COLOR_DEFAULT, 0, 0);
}

///////////////////////////////////////////////////////////////////////////////
