// ManualNgDlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CME8000.h"
#include "ManualUnloadDlg.h"
#include "afxdialogex.h"
#include "math.h"

#include "LogFile.h"
#include "AJinAXL.h"
#include "DataManager.h"
#include "Common.h"

// CManualNgDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CManualUnloadDlg, CDialogEx)

CManualUnloadDlg::CManualUnloadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CManualUnloadDlg::IDD, pParent)
{
}

CManualUnloadDlg::~CManualUnloadDlg()
{
}

void CManualUnloadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for (int i = 0; i < 16; i++) DDX_Control(pDX, IDC_GROUP_0 + i, m_Group[i]);
	for (int i = 0; i < 10; i++) DDX_Control(pDX, IDC_LABEL_0 + i, m_Label[i]);
	for (int i = 0; i < 10; i++) DDX_Control(pDX, IDC_STC_AXIS_POS_0 + i, m_stcAxisPos[i]);
	for (int i = 0; i <  2; i++) DDX_Control(pDX, IDC_BTN_TRANS_STAGE_X_0		+ i, m_btnTransStageX[i]);
	for (int i = 0; i <  3; i++) DDX_Control(pDX, IDC_BTN_TRANS_STAGE_Z_0		+ i, m_btnTransStageZ[i]);
	for (int i = 0; i <  2; i++) DDX_Control(pDX, IDC_BTN_TRANS_STAGE_T_0		+ i, m_btnTransStageT[i]);
	for (int i = 0; i < 20; i++) DDX_Control(pDX, IDC_BTN_TRANS_STAGE_IO_0		+ i, m_btnTransStageIo[i]);
	for (int i = 0; i < 10; i++) DDX_Control(pDX, IDC_LED_TRANS_STAGE_IO_0		+ i, m_ledTransStageIo[i]);
	for (int i = 0; i <  6; i++) DDX_Control(pDX, IDC_BTN_UNLOAD_PICKER_X_0		+ i, m_btnUnloadPickerX[i]);
	for (int i = 0; i <  4; i++) DDX_Control(pDX, IDC_BTN_UNLOAD_PICKER_Z_0		+ i, m_btnUnloadPickerZ[i]);
	for (int i = 0; i <  2; i++) DDX_Control(pDX, IDC_BTN_UNLOAD_PICKER_P_0		+ i, m_btnUnloadPickerP[i]);
	for (int i = 0; i < 28; i++) DDX_Control(pDX, IDC_BTN_UNLOAD_PICKER_IO_0	+ i, m_btnUnloadPickerIo[i]);
	for (int i = 0; i < 24; i++) DDX_Control(pDX, IDC_LED_UNLOAD_PICKER_IO_0	+ i, m_ledUnloadPickerIo[i]);
	for (int i = 0; i <  5; i++) DDX_Control(pDX, IDC_BTN_UNLOAD_STAGE1_Y_0		+ i, m_btnUnloadStage1Y[i]);
	for (int i = 0; i <  6; i++) DDX_Control(pDX, IDC_BTN_UNLOAD_STAGE1_Z_0		+ i, m_btnUnloadStage1Z[i]);
	for (int i = 0; i <  4; i++) DDX_Control(pDX, IDC_BTN_UNLOAD_STAGE1_IO_0	+ i, m_btnUnloadStage1Io[i]);
	for (int i = 0; i <  5; i++) DDX_Control(pDX, IDC_LED_UNLOAD_STAGE1_IO_0	+ i, m_ledUnloadStage1Io[i]);
	for (int i = 0; i <  5; i++) DDX_Control(pDX, IDC_BTN_UNLOAD_STAGE2_Y_0		+ i, m_btnUnloadStage2Y[i]);
	for (int i = 0; i <  6; i++) DDX_Control(pDX, IDC_BTN_UNLOAD_STAGE2_Z_0		+ i, m_btnUnloadStage2Z[i]);
	for (int i = 0; i <  4; i++) DDX_Control(pDX, IDC_BTN_UNLOAD_STAGE2_IO_0	+ i, m_btnUnloadStage2Io[i]);
	for (int i = 0; i <  5; i++) DDX_Control(pDX, IDC_LED_UNLOAD_STAGE2_IO_0	+ i, m_ledUnloadStage2Io[i]);
	for (int i = 0; i <  4; i++) DDX_Control(pDX, IDC_BTN_UNLOAD_PORT1_IO_0		+ i, m_btnUnloadPort1Io[i]);
	for (int i = 0; i < 10; i++) DDX_Control(pDX, IDC_LED_UNLOAD_PORT1_IO_0		+ i, m_ledUnloadPort1Io[i]);
	for (int i = 0; i <  4; i++) DDX_Control(pDX, IDC_BTN_UNLOAD_PORT2_IO_0		+ i, m_btnUnloadPort2Io[i]);
	for (int i = 0; i < 10; i++) DDX_Control(pDX, IDC_LED_UNLOAD_PORT2_IO_0		+ i, m_ledUnloadPort2Io[i]);
}

BEGIN_MESSAGE_MAP(CManualUnloadDlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_TRANS_STAGE_X_0, IDC_BTN_TRANS_STAGE_X_1		, OnBtnTransStageXClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_TRANS_STAGE_Z_0, IDC_BTN_TRANS_STAGE_Z_2		, OnBtnTransStageZClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_TRANS_STAGE_T_0, IDC_BTN_TRANS_STAGE_T_1		, OnBtnTransStageTClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_TRANS_STAGE_IO_0, IDC_BTN_TRANS_STAGE_IO_19	, OnBtnTransStageIoClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_UNLOAD_PICKER_X_0, IDC_BTN_UNLOAD_PICKER_X_5	, OnBtnUnloadPickerXClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_UNLOAD_PICKER_Z_0, IDC_BTN_UNLOAD_PICKER_Z_3	, OnBtnUnloadPickerZClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_UNLOAD_PICKER_P_0, IDC_BTN_UNLOAD_PICKER_P_1	, OnBtnUnloadPickerPClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_UNLOAD_PICKER_IO_0, IDC_BTN_UNLOAD_PICKER_IO_27, OnBtnUnloadPickerIoClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_UNLOAD_STAGE1_Y_0, IDC_BTN_UNLOAD_STAGE1_Y_4	, OnBtnUnloadStage1YClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_UNLOAD_STAGE1_Z_0, IDC_BTN_UNLOAD_STAGE1_Z_5	, OnBtnUnloadStage1ZClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_UNLOAD_STAGE1_IO_0, IDC_BTN_UNLOAD_STAGE1_IO_3	, OnBtnUnloadStage1IoClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_UNLOAD_STAGE2_Y_0, IDC_BTN_UNLOAD_STAGE2_Y_4	, OnBtnUnloadStage2YClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_UNLOAD_STAGE2_Z_0, IDC_BTN_UNLOAD_STAGE2_Z_5	, OnBtnUnloadStage2ZClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_UNLOAD_STAGE2_IO_0, IDC_BTN_UNLOAD_STAGE2_IO_3	, OnBtnUnloadStage2IoClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_UNLOAD_PORT1_IO_0, IDC_BTN_UNLOAD_PORT1_IO_3	, OnBtnUnloadPort1IoClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_UNLOAD_PORT2_IO_0, IDC_BTN_UNLOAD_PORT2_IO_3	, OnBtnUnloadPort2IoClick)
END_MESSAGE_MAP()

// CManualUnloadDlg 메시지 처리기입니다.

BOOL CManualUnloadDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(this, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	Initial_Controls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CManualUnloadDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CManualUnloadDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (!bShow) return;

	Display_Status();

	BOOL bOn = TRUE;
	if (gData.nPickerUseCnt < 6) { bOn = FALSE; }

	m_btnUnloadPickerIo[ 5].EnableWindow(bOn);	// Picker 6 Up
	m_btnUnloadPickerIo[11].EnableWindow(bOn);	// Picker 6 Down
	m_btnUnloadPickerIo[17].EnableWindow(bOn);	// Picker 6 Vac On
	m_btnUnloadPickerIo[23].EnableWindow(bOn);	// Picker 6 Vac Off
	m_btnTransStageIo[ 9].EnableWindow(bOn);	// Stage 6 Air On
	m_btnTransStageIo[15].EnableWindow(bOn);	// Stage 6 Air Off

	bOn = TRUE;
	if (gData.nPickerUseCnt < 5) { bOn = FALSE; }

	m_btnUnloadPickerIo[ 4].EnableWindow(bOn);	// Picker 5 Up
	m_btnUnloadPickerIo[10].EnableWindow(bOn);	// Picker 5 Down
	m_btnUnloadPickerIo[16].EnableWindow(bOn);	// Picker 5 Vac On
	m_btnUnloadPickerIo[22].EnableWindow(bOn);	// Picker 5 Vac Off
	m_btnTransStageIo[ 8].EnableWindow(bOn);	// Stage 5 Air On
	m_btnTransStageIo[14].EnableWindow(bOn);	// Stage 5 Air Off

	bOn = TRUE;
	if (gData.nPickerUseCnt < 4) { bOn = FALSE; }

	m_btnUnloadPickerIo[ 3].EnableWindow(bOn);	// Picker 4 Up
	m_btnUnloadPickerIo[ 9].EnableWindow(bOn);	// Picker 4 Down
	m_btnUnloadPickerIo[15].EnableWindow(bOn);	// Picker 4 Vac On
	m_btnUnloadPickerIo[21].EnableWindow(bOn);	// Picker 4 Vac Off
	m_btnTransStageIo[ 7].EnableWindow(bOn);	// Stage 4 Air On
	m_btnTransStageIo[13].EnableWindow(bOn);	// Stage 4 Air Off

	m_strLog.Format("[Manual Unload] Show Window");
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualUnloadDlg::Initial_Controls() 
{
	for (int i = 0; i < 4; i++) m_Group[i].Init_Ctrl("Arial", 11, TRUE, RGB(0x10, 0x10, 0xD0), COLOR_DEFAULT);
	for (int i = 4; i < 8; i++) m_Group[i].Init_Ctrl("Arial", 11, TRUE, RGB(0xD0, 0x10, 0x10), COLOR_DEFAULT);
	for (int i = 8; i < 11; i++) m_Group[i].Init_Ctrl("Arial", 11, TRUE, RGB(0x30, 0x80, 0x00), COLOR_DEFAULT);
	for (int i = 11; i < 14; i++) m_Group[i].Init_Ctrl("Arial", 11, TRUE, RGB(0xD0, 0x10, 0xD0), COLOR_DEFAULT);
	for (int i = 14; i < 16; i++) m_Group[i].Init_Ctrl("Arial", 11, TRUE, RGB(0xFF, 0x90, 0x00), COLOR_DEFAULT);
	for (int i = 0; i < 10; i++) m_Label[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, RGB(0xB0, 0xB0, 0xB0));
	for (int i = 0; i < 10; i++) m_stcAxisPos[i].Init_Ctrl("Arial", 10, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x10, 0x10, 0x60));
	for (int i = 0; i <  2; i++) m_btnTransStageX[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  3; i++) m_btnTransStageZ[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  2; i++) m_btnTransStageT[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 20; i++) m_btnTransStageIo[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 10; i++) m_ledTransStageIo[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i <  6; i++) m_btnUnloadPickerX[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  4; i++) m_btnUnloadPickerZ[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  2; i++) m_btnUnloadPickerP[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 28; i++) m_btnUnloadPickerIo[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 24; i++) m_ledUnloadPickerIo[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i <  5; i++) m_btnUnloadStage1Y[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  6; i++) m_btnUnloadStage1Z[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  4; i++) m_btnUnloadStage1Io[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  5; i++) m_ledUnloadStage1Io[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i <  5; i++) m_btnUnloadStage2Y[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  6; i++) m_btnUnloadStage2Z[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  4; i++) m_btnUnloadStage2Io[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  5; i++) m_ledUnloadStage2Io[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i <  4; i++) m_btnUnloadPort1Io[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 10; i++) m_ledUnloadPort1Io[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i <  4; i++) m_btnUnloadPort2Io[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 10; i++) m_ledUnloadPort2Io[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
}

void CManualUnloadDlg::Display_Status()
{
	CString strPos;
	for (int i = 0; i < 10; i++) {
		double dPos = g_objAJinAXL.Get_Position(AX_TRANS_STAGE_X + i);
		strPos.Format("%0.3lf", dPos);
		m_stcAxisPos[i].SetWindowText(strPos);
	}

	DX_DATA_03 *pDX03 = g_objAJinAXL.Get_pDX03();
	DX_DATA_05 *pDX05 = g_objAJinAXL.Get_pDX05();
	DX_DATA_10 *pDX10 = g_objAJinAXL.Get_pDX10();

	m_ledTransStageIo[0].Set_On(pDX05->iTransStageClamp1On);
	m_ledTransStageIo[1].Set_On(pDX05->iTransStageClamp2On);
	m_ledTransStageIo[2].Set_On(pDX05->iTransStageClamp1Off);
	m_ledTransStageIo[3].Set_On(pDX05->iTransStageClamp2Off);
	m_ledTransStageIo[4].Set_On(pDX05->iTransStageVac01);
	m_ledTransStageIo[5].Set_On(pDX05->iTransStageVac02);
	m_ledTransStageIo[6].Set_On(pDX05->iTransStageVac03);
	m_ledTransStageIo[7].Set_On(pDX05->iTransStageVac04);
	m_ledTransStageIo[8].Set_On(pDX05->iTransStageVac05);
	m_ledTransStageIo[9].Set_On(pDX05->iTransStageVac06);

	m_ledUnloadPickerIo[ 0].Set_On(pDX10->iUnloadPickerUp01);
	m_ledUnloadPickerIo[ 1].Set_On(pDX10->iUnloadPickerUp02);
	m_ledUnloadPickerIo[ 2].Set_On(pDX10->iUnloadPickerUp03);
	m_ledUnloadPickerIo[ 3].Set_On(pDX10->iUnloadPickerUp04);
	m_ledUnloadPickerIo[ 4].Set_On(pDX10->iUnloadPickerUp05);
	m_ledUnloadPickerIo[ 5].Set_On(pDX10->iUnloadPickerUp06);
	m_ledUnloadPickerIo[ 6].Set_On(pDX10->iUnloadPickerDown01);
	m_ledUnloadPickerIo[ 7].Set_On(pDX10->iUnloadPickerDown02);
	m_ledUnloadPickerIo[ 8].Set_On(pDX10->iUnloadPickerDown03);
	m_ledUnloadPickerIo[ 9].Set_On(pDX10->iUnloadPickerDown04);
	m_ledUnloadPickerIo[10].Set_On(pDX10->iUnloadPickerDown05);
	m_ledUnloadPickerIo[11].Set_On(pDX10->iUnloadPickerDown06);
	m_ledUnloadPickerIo[12].Set_On(pDX10->iUnloadPickerVac01);
	m_ledUnloadPickerIo[13].Set_On(pDX10->iUnloadPickerVac02);
	m_ledUnloadPickerIo[14].Set_On(pDX10->iUnloadPickerVac03);
	m_ledUnloadPickerIo[15].Set_On(pDX10->iUnloadPickerVac04);
	m_ledUnloadPickerIo[16].Set_On(pDX10->iUnloadPickerVac05);
	m_ledUnloadPickerIo[17].Set_On(pDX10->iUnloadPickerVac06);
	m_ledUnloadPickerIo[18].Set_On(pDX10->iUnloadPickerCapChk1);
	m_ledUnloadPickerIo[19].Set_On(pDX10->iUnloadPickerCapChk2);
	m_ledUnloadPickerIo[20].Set_On(pDX10->iUnloadPickerCapChk3);
	m_ledUnloadPickerIo[21].Set_On(pDX10->iUnloadPickerCapChk4);
	m_ledUnloadPickerIo[22].Set_On(pDX10->iUnloadPickerCapChk5);
	m_ledUnloadPickerIo[23].Set_On(pDX10->iUnloadPickerCapChk6);

	m_ledUnloadStage1Io[0].Set_On(pDX05->iUnloadStage1MasterIn);
	m_ledUnloadStage1Io[1].Set_On(pDX05->iUnloadStage1MasterOut);
	m_ledUnloadStage1Io[2].Set_On(pDX05->iUnloadStage1SlaveIn);
	m_ledUnloadStage1Io[3].Set_On(pDX05->iUnloadStage1SlaveOut);
	m_ledUnloadStage1Io[4].Set_On(pDX05->iUnloadStage1Exist);

	m_ledUnloadStage2Io[0].Set_On(pDX05->iUnloadStage2MasterIn);
	m_ledUnloadStage2Io[1].Set_On(pDX05->iUnloadStage2MasterOut);
	m_ledUnloadStage2Io[2].Set_On(pDX05->iUnloadStage2SlaveIn);
	m_ledUnloadStage2Io[3].Set_On(pDX05->iUnloadStage2SlaveOut);
	m_ledUnloadStage2Io[4].Set_On(pDX05->iUnloadStage2Exist);

	m_ledUnloadPort1Io[0].Set_On(pDX03->iUnlaodPort1LowCheck);
	m_ledUnloadPort1Io[1].Set_On(pDX03->iUnloadPort1SlideOpen);
	m_ledUnloadPort1Io[2].Set_On(pDX03->iUnloadPort1SlideClose);
	m_ledUnloadPort1Io[3].Set_On(pDX03->iUnloadPort1SlideLock);
	m_ledUnloadPort1Io[4].Set_On(pDX03->iUnloadPort1SlideUnlock);
	m_ledUnloadPort1Io[5].Set_On(pDX03->iUnloadPort1Support1In);
	m_ledUnloadPort1Io[6].Set_On(pDX03->iUnloadPort1Support2In);
	m_ledUnloadPort1Io[7].Set_On(pDX03->iUnloadPort1Support1Out);
	m_ledUnloadPort1Io[8].Set_On(pDX03->iUnloadPort1Support2Out);
	m_ledUnloadPort1Io[9].Set_On(!pDX03->iUnloadPort1AreaCheck);

	m_ledUnloadPort2Io[0].Set_On(pDX03->iUnlaodPort2LowCheck);
	m_ledUnloadPort2Io[1].Set_On(pDX03->iUnloadPort2SlideOpen);
	m_ledUnloadPort2Io[2].Set_On(pDX03->iUnloadPort2SlideClose);
	m_ledUnloadPort2Io[3].Set_On(pDX03->iUnloadPort2SlideLock);
	m_ledUnloadPort2Io[4].Set_On(pDX03->iUnloadPort2SlideUnlock);
	m_ledUnloadPort2Io[5].Set_On(pDX03->iUnloadPort2Support1In);
	m_ledUnloadPort2Io[6].Set_On(pDX03->iUnloadPort2Support2In);
	m_ledUnloadPort2Io[7].Set_On(pDX03->iUnloadPort2Support1Out);
	m_ledUnloadPort2Io[8].Set_On(pDX03->iUnloadPort2Support2Out);
	m_ledUnloadPort2Io[9].Set_On(!pDX03->iUnloadPort2AreaCheck);
}

///////////////////////////////////////////////////////////////////////////////

void CManualUnloadDlg::OnBtnTransStageXClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_TRANS_STAGE_X)) return;
	if (!g_objAJinAXL.Is_Home(AX_TRANS_STAGE_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_TRANS_STAGE_T)) return;

	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_TRANS_STAGE_X_0;

	if (!g_objCommon.Check_Position(AX_TRANS_STAGE_Z, 0)) {
		AfxMessageBox("Trans Stage Z축 Move Up 위치 인지 확인 후 진행하세요."); return;
	}
	if (!g_objCommon.Check_Position(AX_UNLOAD_PICKER_Z, 0)) {
		AfxMessageBox("Unload Picker Z축 Ready Up 위치 인지 확인 후 진행하세요."); return;
	}

	g_objCommon.Move_Position(AX_TRANS_STAGE_X, nIndex);

	m_strLog.Format("[Manual Unload] Trans Stage X (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualUnloadDlg::OnBtnTransStageZClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_TRANS_STAGE_X)) return;
	if (!g_objAJinAXL.Is_Home(AX_TRANS_STAGE_Z)) return;

	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_TRANS_STAGE_Z_0;

	if (nIndex == 1 && !g_objCommon.Check_Position(AX_TRANS_STAGE_X, 0)) {
		AfxMessageBox("Trans Stage X축 Load(Index) 위치 인지 확인 후 진행하세요."); return;
	}
	if (nIndex == 1 && !g_objCommon.Get_TransStageClampOff()) {
		AfxMessageBox("Trans Stage Clamp Off 상태 인지 확인 후 진행하세요."); return;
	}

	g_objCommon.Move_Position(AX_TRANS_STAGE_Z, nIndex);

	m_strLog.Format("[Manual Unload] Trans Stage Z (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualUnloadDlg::OnBtnTransStageTClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_TRANS_STAGE_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_TRANS_STAGE_T)) return;

	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_TRANS_STAGE_T_0;

	if (!g_objCommon.Check_Position(AX_TRANS_STAGE_Z, 0)) {
		AfxMessageBox("Trans Stage Z축 Move Up 위치 인지 확인 후 진행하세요."); return;
	}
	if (!g_objCommon.Check_Position(AX_TRANS_STAGE_X, 0) && !g_objCommon.Check_Position(AX_UNLOAD_PICKER_Z, 0)) {
		AfxMessageBox("Unload Picker Z축 Ready Up 위치 인지 확인 후 진행하세요."); return;
	}

	g_objCommon.Move_Position(AX_TRANS_STAGE_T, nIndex);

	m_strLog.Format("[Manual Unload] Trans Stage T (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualUnloadDlg::OnBtnTransStageIoClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_TRANS_STAGE_IO_0;

	DY_DATA_05 *pDY05 = g_objAJinAXL.Get_pDY05();

	if (nIndex == 0 || nIndex == 16) { pDY05->oTransStageClamp1On = TRUE; pDY05->oTransStageClamp1Off = FALSE; }
	if (nIndex == 1 || nIndex == 16) { pDY05->oTransStageClamp2On = TRUE; pDY05->oTransStageClamp2Off = FALSE; }

	if (nIndex == 2 || nIndex == 17) { pDY05->oTransStageClamp1On = FALSE; pDY05->oTransStageClamp1Off = TRUE; }
	if (nIndex == 3 || nIndex == 17) { pDY05->oTransStageClamp2On = FALSE; pDY05->oTransStageClamp2Off = TRUE; }

	if (nIndex ==  4 || nIndex == 18) pDY05->oTransStageVac01 = TRUE;
	if (nIndex ==  5 || nIndex == 18) pDY05->oTransStageVac02 = TRUE;
	if (nIndex ==  6 || nIndex == 18) pDY05->oTransStageVac03 = TRUE;
	if (nIndex ==  7 || nIndex == 18) pDY05->oTransStageVac04 = TRUE;
	if (nIndex ==  8 || nIndex == 18) pDY05->oTransStageVac05 = TRUE;
	if (nIndex ==  9 || nIndex == 18) pDY05->oTransStageVac06 = TRUE;

	if (nIndex == 10 || nIndex == 19) { pDY05->oTransStageVac01 = FALSE; pDY05->oTransStageAir01 = TRUE; }
	if (nIndex == 11 || nIndex == 19) { pDY05->oTransStageVac02 = FALSE; pDY05->oTransStageAir02 = TRUE; }
	if (nIndex == 12 || nIndex == 19) { pDY05->oTransStageVac03 = FALSE; pDY05->oTransStageAir03 = TRUE; }
	if (nIndex == 13 || nIndex == 19) { pDY05->oTransStageVac04 = FALSE; pDY05->oTransStageAir04 = TRUE; }
	if (nIndex == 14 || nIndex == 19) { pDY05->oTransStageVac05 = FALSE; pDY05->oTransStageAir05 = TRUE; }
	if (nIndex == 15 || nIndex == 19) { pDY05->oTransStageVac06 = FALSE; pDY05->oTransStageAir06 = TRUE; }

	if ((nIndex > 9 && nIndex < 16) || nIndex == 19) { g_objAJinAXL.Write_Output(5); Sleep(100); }

	if (nIndex == 10 || nIndex == 19) { pDY05->oTransStageAir01 = FALSE; }
	if (nIndex == 11 || nIndex == 19) { pDY05->oTransStageAir02 = FALSE; }
	if (nIndex == 12 || nIndex == 19) { pDY05->oTransStageAir03 = FALSE; }
	if (nIndex == 13 || nIndex == 19) { pDY05->oTransStageAir04 = FALSE; }
	if (nIndex == 14 || nIndex == 19) { pDY05->oTransStageAir05 = FALSE; }
	if (nIndex == 15 || nIndex == 19) { pDY05->oTransStageAir06 = FALSE; }

	g_objAJinAXL.Write_Output(5);

	m_strLog.Format("[Manual Unload] Trans Stage IO (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualUnloadDlg::OnBtnUnloadPickerXClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_UNLOAD_PICKER_X)) return;
	if (!g_objAJinAXL.Is_Home(AX_UNLOAD_PICKER_Z)) return;

	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_UNLOAD_PICKER_X_0;

	if (!g_objCommon.Check_Position(AX_UNLOAD_PICKER_Z, 0)) {
		AfxMessageBox("Unload Picker Z축 Ready Up 위치 인지 확인 후 진행하세요."); return;
	}

	if (nIndex == 4 || nIndex == 5) {
		EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
		int nDir = (nIndex == 5 ? 1 : -1);
		double dMove = pEquipData->dUnloadTrayPitchX * nDir;	// Temp
		g_objAJinAXL.Move_Relative(AX_UNLOAD_PICKER_X, dMove);

	} else {
		g_objCommon.Move_Position(AX_UNLOAD_PICKER_X, nIndex);
	}

 	m_strLog.Format("[Manual Unload] Unload Picker X (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualUnloadDlg::OnBtnUnloadPickerZClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_UNLOAD_PICKER_X)) return;
	if (!g_objAJinAXL.Is_Home(AX_UNLOAD_PICKER_Z)) return;

	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_UNLOAD_PICKER_Z_0;

	MOVE_DATA *pMoveData = g_objDataManager.Get_pMoveData();
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	if (nIndex == 1 && !g_objCommon.Check_Position(AX_UNLOAD_PICKER_X, 0)) {
		AfxMessageBox("Unload Picker X축 Load(Trans Stage) 위치 인지 확인 후 진행하세요."); return;
	}
	if (nIndex == 2 && !g_objCommon.Check_Position(AX_UNLOAD_PICKER_X, 1)) {
		double dCur = g_objAJinAXL.Get_Position(AX_UNLOAD_PICKER_X);
		double dPos1, dPos2;
		dPos1 = pMoveData->dUnloadPickerX[1] + (pEquipData->dUnloadTrayPitchX * (pEquipData->nUnloadTrayX-1));
		dPos2 = pMoveData->dUnloadPickerX[1] - (pEquipData->dUnloadTrayPitchX * (pEquipData->nUnloadTrayX-1));
		if (dPos1 < dCur || dPos2 > dCur) {
			AfxMessageBox("Unload Picker X축 Unload1 위치 인지 확인 후 진행하세요."); return;
		}
	}
	if (nIndex == 3 && !g_objCommon.Check_Position(AX_UNLOAD_PICKER_X, 2)) {
		double dCur = g_objAJinAXL.Get_Position(AX_UNLOAD_PICKER_X);
		double dPos1, dPos2;
		dPos1 = pMoveData->dUnloadPickerX[2] + (pEquipData->dUnloadTrayPitchX * (pEquipData->nUnloadTrayX-1));
		dPos2 = pMoveData->dUnloadPickerX[2] - (pEquipData->dUnloadTrayPitchX * (pEquipData->nUnloadTrayX-1));
		if (dPos1 < dCur || dPos2 > dCur) {
			AfxMessageBox("Unload Picker X축 Unload2 위치 인지 확인 후 진행하세요."); return;
		}
	}

	g_objCommon.Move_Position(AX_UNLOAD_PICKER_Z, nIndex);

	m_strLog.Format("[Manual Unload] Unload Picker Z (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualUnloadDlg::OnBtnUnloadPickerPClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_UNLOAD_PICKER_X)) return;
	if (!g_objAJinAXL.Is_Home(AX_UNLOAD_PICKER_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_UNLOAD_PICKER_P)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_UNLOAD_PICKER_P_0;

	if (!g_objCommon.Check_Position(AX_UNLOAD_PICKER_Z, 0)) {
		AfxMessageBox("Unload Picker Z축 Ready Up 위치 인지 확인 후 진행하세요."); return;
	}

	g_objCommon.Move_Position(AX_UNLOAD_PICKER_P, nIndex);

	m_strLog.Format("[Manual Unload] Unload Picker P (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualUnloadDlg::OnBtnUnloadPickerIoClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_UNLOAD_PICKER_IO_0;

	DY_DATA_10 *pDY10 = g_objAJinAXL.Get_pDY10();
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	if (nIndex == 0 || nIndex == 24) pDY10->oUnloadPickerDown01 = FALSE;
	if (nIndex == 1 || nIndex == 24) pDY10->oUnloadPickerDown02 = FALSE;
	if (nIndex == 2 || nIndex == 24) pDY10->oUnloadPickerDown03 = FALSE;
	if (nIndex == 3 || nIndex == 24) pDY10->oUnloadPickerDown04 = FALSE;
	if (nIndex == 4 || nIndex == 24) pDY10->oUnloadPickerDown05 = FALSE;
	if (nIndex == 5 || nIndex == 24) pDY10->oUnloadPickerDown06 = FALSE;

	if (nIndex ==  6 || nIndex == 25) pDY10->oUnloadPickerDown01 = TRUE;
	if (nIndex ==  7 || nIndex == 25) pDY10->oUnloadPickerDown02 = TRUE;
	if (nIndex ==  8 || nIndex == 25) pDY10->oUnloadPickerDown03 = TRUE;
	if (gData.nPickerUseCnt > 3) { if (nIndex ==  9 || nIndex == 25) pDY10->oUnloadPickerDown04 = TRUE; }
	if (gData.nPickerUseCnt > 4) { if (nIndex == 10 || nIndex == 25) pDY10->oUnloadPickerDown05 = TRUE; }
	if (gData.nPickerUseCnt > 5) { if (nIndex == 11 || nIndex == 25) pDY10->oUnloadPickerDown06 = TRUE; }

	if (nIndex == 12 || nIndex == 26) { pDY10->oUnloadPickerVac01 = TRUE; pDY10->oUnloadPickerAir01 = FALSE; }
	if (nIndex == 13 || nIndex == 26) { pDY10->oUnloadPickerVac02 = TRUE; pDY10->oUnloadPickerAir02 = FALSE; }
	if (nIndex == 14 || nIndex == 26) { pDY10->oUnloadPickerVac03 = TRUE; pDY10->oUnloadPickerAir03 = FALSE; }
	if (nIndex == 15 || nIndex == 26) { pDY10->oUnloadPickerVac04 = TRUE; pDY10->oUnloadPickerAir04 = FALSE; }
	if (nIndex == 16 || nIndex == 26) { pDY10->oUnloadPickerVac05 = TRUE; pDY10->oUnloadPickerAir05 = FALSE; }
	if (nIndex == 17 || nIndex == 26) { pDY10->oUnloadPickerVac06 = TRUE; pDY10->oUnloadPickerAir06 = FALSE; }

	if (nIndex == 18 || nIndex == 27) { pDY10->oUnloadPickerVac01 = FALSE; pDY10->oUnloadPickerAir01 = TRUE; }
	if (nIndex == 19 || nIndex == 27) { pDY10->oUnloadPickerVac02 = FALSE; pDY10->oUnloadPickerAir02 = TRUE; }
	if (nIndex == 20 || nIndex == 27) { pDY10->oUnloadPickerVac03 = FALSE; pDY10->oUnloadPickerAir03 = TRUE; }
	if (nIndex == 21 || nIndex == 27) { pDY10->oUnloadPickerVac04 = FALSE; pDY10->oUnloadPickerAir04 = TRUE; }
	if (nIndex == 22 || nIndex == 27) { pDY10->oUnloadPickerVac05 = FALSE; pDY10->oUnloadPickerAir05 = TRUE; }
	if (nIndex == 23 || nIndex == 27) { pDY10->oUnloadPickerVac06 = FALSE; pDY10->oUnloadPickerAir06 = TRUE; }

	if ((nIndex > 17 && nIndex < 24) || nIndex == 27) { g_objAJinAXL.Write_Output(10); Sleep(100); }

	if (nIndex == 18 || nIndex == 27) { pDY10->oUnloadPickerAir01 = FALSE; }
	if (nIndex == 19 || nIndex == 27) { pDY10->oUnloadPickerAir02 = FALSE; }
	if (nIndex == 20 || nIndex == 27) { pDY10->oUnloadPickerAir03 = FALSE; }
	if (nIndex == 21 || nIndex == 27) { pDY10->oUnloadPickerAir04 = FALSE; }
	if (nIndex == 22 || nIndex == 27) { pDY10->oUnloadPickerAir05 = FALSE; }
	if (nIndex == 23 || nIndex == 27) { pDY10->oUnloadPickerAir06 = FALSE; }

	g_objAJinAXL.Write_Output(10);

	m_strLog.Format("[Manual Unload] Unload Picker IO (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualUnloadDlg::OnBtnUnloadStage1YClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_UNLOAD_STAGE1_Y)) return;
	if (!g_objAJinAXL.Is_Home(AX_UNLOAD_STAGE1_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_UNLOAD_STAGE2_Z)) return;

	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_UNLOAD_STAGE1_Y_0;

	if (!g_objCommon.Check_Position(AX_UNLOAD_PICKER_X, 1) && !g_objCommon.Check_Position(AX_UNLOAD_PICKER_X, 3) && !g_objCommon.Check_Position(AX_UNLOAD_PICKER_Z, 0)) {
		AfxMessageBox("Unload Picker Ready Up 위치가 아닙니다. 확인 후 진행하세요."); return;
	}
	if ((g_objCommon.Check_Position(AX_UNLOAD_STAGE1_Z, 0) && g_objCommon.Check_Position(AX_UNLOAD_STAGE2_Z, 0)) ||
		(g_objCommon.Check_Position(AX_UNLOAD_STAGE1_Z, 1) && g_objCommon.Check_Position(AX_UNLOAD_STAGE2_Z, 1))) {
			AfxMessageBox("Unload Stage1, 2 Z축 높이가 같습니다. 확인 후 진행하세요."); return;
	}

	if ((g_objCommon.Check_Position(AX_UNLOAD_STAGE1_Z, 0) && g_objCommon.Check_Position(AX_UNLOAD_STAGE2_Z, 1)) ||
		(g_objCommon.Check_Position(AX_UNLOAD_STAGE1_Z, 1) && g_objCommon.Check_Position(AX_UNLOAD_STAGE2_Z, 0))) {
			if (nIndex == 3 || nIndex == 4) {
				EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
				int nDir = (nIndex == 4 ? 1 : -1);
				double dMove = pEquipData->dUnloadTrayPitchY * nDir;	// Temp
				g_objAJinAXL.Move_Relative(AX_UNLOAD_STAGE1_Y, dMove);

			} else {
				g_objCommon.Move_Position(AX_UNLOAD_STAGE1_Y, nIndex);
			}

	} else {
		AfxMessageBox("Unload Stage1, 2 Z축 높이 확인 후 진행하세요."); return;
	}

	m_strLog.Format("[Manual Unload] Unload Stage1 Y (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualUnloadDlg::OnBtnUnloadStage1ZClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_UNLOAD_STAGE1_Y)) return;
	if (!g_objAJinAXL.Is_Home(AX_UNLOAD_STAGE1_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_UNLOAD_STAGE2_Y)) return;

	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_UNLOAD_STAGE1_Z_0;

	double dStage1 = g_objAJinAXL.Get_Position(AX_UNLOAD_STAGE1_Y);
	double dStage2 = g_objAJinAXL.Get_Position(AX_UNLOAD_STAGE2_Y);
	double dDiff = fabs(dStage1 - dStage2);
	if (dDiff < 300.0) { AfxMessageBox("Unload Stage1, 2 충돌 위험!!! Y축 위치 확인 후 진행하세요."); return; }

	if (nIndex == 2 || nIndex == 3) {
		if (!g_objCommon.Check_Position(AX_UNLOAD_STAGE1_Y, 0)) {	// Tray Load Position
			AfxMessageBox("Tray Load 위치가 아닙니다. Y축 확인 후 진행하세요."); return;
		}
	}
	if (nIndex == 4 || nIndex == 5) {
		if (!g_objCommon.Check_Position(AX_UNLOAD_STAGE1_Y, 2)) {	// Tray Unload Position
			AfxMessageBox("Tray Unload 위치가 아닙니다. Y축 확인 후 진행하세요."); return;
		}
	}

	g_objCommon.Move_Position(AX_UNLOAD_STAGE1_Z, nIndex);

	m_strLog.Format("[Manual Unload] Unload Stage1 Z (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualUnloadDlg::OnBtnUnloadStage1IoClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_UNLOAD_STAGE1_IO_0;

	DY_DATA_05 *pDY05 = g_objAJinAXL.Get_pDY05();

	if (nIndex == 0) { pDY05->oUnloadStage1MasterIn = TRUE;  }
	if (nIndex == 1) { pDY05->oUnloadStage1MasterIn = FALSE; }
	if (nIndex == 2) { pDY05->oUnloadStage1SlaveIn  = TRUE;  }
	if (nIndex == 3) { pDY05->oUnloadStage1SlaveIn  = FALSE; }

	g_objAJinAXL.Write_Output(5);

	m_strLog.Format("[Manual Unload] Unload Stage1 IO (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualUnloadDlg::OnBtnUnloadStage2YClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_UNLOAD_STAGE2_Y)) return;
	if (!g_objAJinAXL.Is_Home(AX_UNLOAD_STAGE2_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_UNLOAD_STAGE1_Z)) return;

	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_UNLOAD_STAGE2_Y_0;

	if (!g_objCommon.Check_Position(AX_UNLOAD_PICKER_X, 1) && !g_objCommon.Check_Position(AX_UNLOAD_PICKER_X, 3) && !g_objCommon.Check_Position(AX_UNLOAD_PICKER_Z, 0)) {
		AfxMessageBox("Unload Picker Ready Up 위치가 아닙니다. 확인 후 진행하세요."); return;
	}
	if ((g_objCommon.Check_Position(AX_UNLOAD_STAGE1_Z, 0) && g_objCommon.Check_Position(AX_UNLOAD_STAGE2_Z, 0)) ||
		(g_objCommon.Check_Position(AX_UNLOAD_STAGE1_Z, 1) && g_objCommon.Check_Position(AX_UNLOAD_STAGE2_Z, 1))) {
			AfxMessageBox("Unload Stage1, 2 Z축 높이가 같습니다. 확인 후 진행하세요."); return;
	}

	if ((g_objCommon.Check_Position(AX_UNLOAD_STAGE1_Z, 0) && g_objCommon.Check_Position(AX_UNLOAD_STAGE2_Z, 1)) ||
		(g_objCommon.Check_Position(AX_UNLOAD_STAGE1_Z, 1) && g_objCommon.Check_Position(AX_UNLOAD_STAGE2_Z, 0))) {

			if (nIndex == 3 || nIndex == 4) {
				EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
				int nDir = (nIndex == 4 ? 1 : -1);
				double dMove = pEquipData->dUnloadTrayPitchY * nDir;	// Temp
				g_objAJinAXL.Move_Relative(AX_UNLOAD_STAGE2_Y, dMove);

			} else {
				g_objCommon.Move_Position(AX_UNLOAD_STAGE2_Y, nIndex);
			}

	} else {
		AfxMessageBox("Unload Stage1, 2 Z축 높이 확인 후 진행하세요."); return;
	}

	m_strLog.Format("[Manual Unload] Unload Stage2 Y (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualUnloadDlg::OnBtnUnloadStage2ZClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_UNLOAD_STAGE2_Y)) return;
	if (!g_objAJinAXL.Is_Home(AX_UNLOAD_STAGE2_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_UNLOAD_STAGE1_Y)) return;

	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_UNLOAD_STAGE2_Z_0;

	double dStage1 = g_objAJinAXL.Get_Position(AX_UNLOAD_STAGE1_Y);
	double dStage2 = g_objAJinAXL.Get_Position(AX_UNLOAD_STAGE2_Y);
	double dDiff = fabs(dStage1 - dStage2);
	if (dDiff < 300.0) { AfxMessageBox("Unload Stage1, 2 충돌 위험!!! Y축 위치 확인 후 진행하세요."); return; }

	if (nIndex == 2 || nIndex == 3) {
		if (!g_objCommon.Check_Position(AX_UNLOAD_STAGE2_Y, 0)) {	// Tray Load Position
			AfxMessageBox("Tray Load 위치가 아닙니다. Y축 확인 후 진행하세요."); return;
		}
	}
	if (nIndex == 4 || nIndex == 5) {
		if (!g_objCommon.Check_Position(AX_UNLOAD_STAGE2_Y, 2)) {	// Tray Unload Position
			AfxMessageBox("Tray Unload 위치가 아닙니다. Y축 확인 후 진행하세요."); return;
		}
	}

	g_objCommon.Move_Position(AX_UNLOAD_STAGE2_Z, nIndex);

	m_strLog.Format("[Manual Unload] Unload Stage2 Z (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualUnloadDlg::OnBtnUnloadStage2IoClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_UNLOAD_STAGE2_IO_0;

	DY_DATA_05 *pDY05 = g_objAJinAXL.Get_pDY05();

	if (nIndex == 0) { pDY05->oUnloadStage2MasterIn = TRUE;  }
	if (nIndex == 1) { pDY05->oUnloadStage2MasterIn = FALSE; }
	if (nIndex == 2) { pDY05->oUnloadStage2SlaveIn  = TRUE;  }
	if (nIndex == 3) { pDY05->oUnloadStage2SlaveIn  = FALSE; }

	g_objAJinAXL.Write_Output(5);

	m_strLog.Format("[Manual Unload] Unload Stage2 IO (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualUnloadDlg::OnBtnUnloadPort1IoClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_UNLOAD_PORT1_IO_0;

	DY_DATA_03 *pDY03 = g_objAJinAXL.Get_pDY03();

	if (nIndex == 0) { pDY03->oUnloadPort1SlideLock   = TRUE; pDY03->oUnloadPort1SlideUnlock = FALSE; }
	if (nIndex == 1) { pDY03->oUnloadPort1SlideUnlock = TRUE; pDY03->oUnloadPort1SlideLock   = FALSE; }
	if (nIndex == 2) { pDY03->oUnloadPort1SupportIn   = TRUE; pDY03->oUnloadPort1SupportOut  = FALSE; }
	if (nIndex == 3) { pDY03->oUnloadPort1SupportOut  = TRUE; pDY03->oUnloadPort1SupportIn   = FALSE; }

	g_objAJinAXL.Write_Output(3);

	m_strLog.Format("[Manual Unload] Unload Port1 IO (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualUnloadDlg::OnBtnUnloadPort2IoClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_UNLOAD_PORT2_IO_0;

	DY_DATA_03 *pDY03 = g_objAJinAXL.Get_pDY03();

	if (nIndex == 0) { pDY03->oUnloadPort2SlideLock   = TRUE; pDY03->oUnloadPort2SlideUnlock = FALSE; }
	if (nIndex == 1) { pDY03->oUnloadPort2SlideUnlock = TRUE; pDY03->oUnloadPort2SlideLock   = FALSE; }
	if (nIndex == 2) { pDY03->oUnloadPort2SupportIn   = TRUE; pDY03->oUnloadPort2SupportOut  = FALSE; }
	if (nIndex == 3) { pDY03->oUnloadPort2SupportOut  = TRUE; pDY03->oUnloadPort2SupportIn   = FALSE; }

	g_objAJinAXL.Write_Output(3);

	m_strLog.Format("[Manual Unload] Unload Port1 IO (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

///////////////////////////////////////////////////////////////////////////////
