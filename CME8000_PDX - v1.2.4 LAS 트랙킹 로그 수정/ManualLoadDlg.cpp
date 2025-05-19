// ManualLoadDlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CME8000.h"
#include "ManualLoadDlg.h"
#include "afxdialogex.h"
#include "math.h"

#include "LogFile.h"
#include "AJinAXL.h"
#include "DataManager.h"
#include "Common.h"
#include "SequenceMain.h"

// CManualLoadDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CManualLoadDlg, CDialogEx)

CManualLoadDlg::CManualLoadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CManualLoadDlg::IDD, pParent)
{
}

CManualLoadDlg::~CManualLoadDlg()
{
}

void CManualLoadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for (int i = 0; i < 17; i++) DDX_Control(pDX, IDC_GROUP_0 + i, m_Group[i]);
	for (int i = 0; i < 11; i++) DDX_Control(pDX, IDC_LABEL_0 + i, m_Label[i]);
	for (int i = 0; i < 11; i++) DDX_Control(pDX, IDC_STC_AXIS_POS_0 + i, m_stcAxisPos[i]);
	for (int i = 0; i <  3; i++) DDX_Control(pDX, IDC_BTN_TRAY_PICKER_X_0	+ i	, m_btnTrayPickerX[i]);
	for (int i = 0; i <  3; i++) DDX_Control(pDX, IDC_BTN_TRAY_PICKER_Z_0	+ i	, m_btnTrayPickerZ[i]);
	for (int i = 0; i <  2; i++) DDX_Control(pDX, IDC_BTN_TRAY_PICKER_R_0	+ i	, m_btnTrayPickerR[i]);
	for (int i = 0; i <  4; i++) DDX_Control(pDX, IDC_BTN_TRAY_PICKER_IO_0	+ i	, m_btnTrayPickerIo[i]);
	for (int i = 0; i <  9; i++) DDX_Control(pDX, IDC_LED_TRAY_PICKER_IO_0	+ i	, m_ledTrayPickerIo[i]);
	for (int i = 0; i <  4; i++) DDX_Control(pDX, IDC_BTN_LOAD_PORT1_IO_0	+ i	, m_btnLoadPort1Io[i]);
	for (int i = 0; i <  9; i++) DDX_Control(pDX, IDC_LED_LOAD_PORT1_IO_0	+ i	, m_ledLoadPort1Io[i]);
	for (int i = 0; i <  4; i++) DDX_Control(pDX, IDC_BTN_LOAD_PORT2_IO_0	+ i	, m_btnLoadPort2Io[i]);
	for (int i = 0; i < 10; i++) DDX_Control(pDX, IDC_LED_LOAD_PORT2_IO_0	+ i	, m_ledLoadPort2Io[i]);
	for (int i = 0; i <  2; i++) DDX_Control(pDX, IDC_BTN_LOAD_PORT3_IO_0	+ i	, m_btnLoadPort3Io[i]);
	for (int i = 0; i <  7; i++) DDX_Control(pDX, IDC_LED_LOAD_PORT3_IO_0	+ i	, m_ledLoadPort3Io[i]);
	for (int i = 0; i <  4; i++) DDX_Control(pDX, IDC_BTN_LOAD_STAGE1_X_0	+ i	, m_btnLoadStage1X[i]);
	for (int i = 0; i <  5; i++) DDX_Control(pDX, IDC_BTN_LOAD_STAGE1_Z_0	+ i	, m_btnLoadStage1Z[i]);
	for (int i = 0; i <  4; i++) DDX_Control(pDX, IDC_BTN_LOAD_STAGE1_IO_0	+ i	, m_btnLoadStage1Io[i]);
	for (int i = 0; i <  5; i++) DDX_Control(pDX, IDC_LED_LOAD_STAGE1_IO_0	+ i	, m_ledLoadStage1Io[i]);
	for (int i = 0; i <  4; i++) DDX_Control(pDX, IDC_BTN_LOAD_STAGE2_X_0	+ i	, m_btnLoadStage2X[i]);
	for (int i = 0; i <  5; i++) DDX_Control(pDX, IDC_BTN_LOAD_STAGE2_Z_0	+ i	, m_btnLoadStage2Z[i]);
	for (int i = 0; i <  4; i++) DDX_Control(pDX, IDC_BTN_LOAD_STAGE2_IO_0	+ i	, m_btnLoadStage2Io[i]);
	for (int i = 0; i <  5; i++) DDX_Control(pDX, IDC_LED_LOAD_STAGE2_IO_0	+ i	, m_ledLoadStage2Io[i]);
	for (int i = 0; i <  5; i++) DDX_Control(pDX, IDC_BTN_LOAD_PICKER_Y_0	+ i	, m_btnLoadPickerX[i]);
	for (int i = 0; i <  4; i++) DDX_Control(pDX, IDC_BTN_LOAD_PICKER_Z_0	+ i	, m_btnLoadPickerZ[i]);
	for (int i = 0; i <  2; i++) DDX_Control(pDX, IDC_BTN_LOAD_PICKER_P_0	+ i	, m_btnLoadPickerP[i]);
	for (int i = 0; i < 28; i++) DDX_Control(pDX, IDC_BTN_LOAD_PICKER_IO_0	+ i	, m_btnLoadPickerIo[i]);
	for (int i = 0; i < 24; i++) DDX_Control(pDX, IDC_LED_LOAD_PICKER_IO_0	+ i	, m_ledLoadPickerIo[i]);
}

BEGIN_MESSAGE_MAP(CManualLoadDlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_TRAY_PICKER_X_0	, IDC_BTN_TRAY_PICKER_X_2	, OnbtnTrayPickerXClick		)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_TRAY_PICKER_Z_0	, IDC_BTN_TRAY_PICKER_Z_2	, OnbtnTrayPickerZClick		)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_TRAY_PICKER_R_0	, IDC_BTN_TRAY_PICKER_R_1	, OnbtnTrayPickerRClick		)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_TRAY_PICKER_IO_0	, IDC_BTN_TRAY_PICKER_IO_3	, OnbtnTrayPickerIoClick	)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_LOAD_PORT1_IO_0	, IDC_BTN_LOAD_PORT1_IO_3	, OnbtnLoadPort1IoClick		)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_LOAD_PORT2_IO_0	, IDC_BTN_LOAD_PORT2_IO_3	, OnbtnLoadPort2IoClick		)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_LOAD_PORT3_IO_0	, IDC_BTN_LOAD_PORT3_IO_1	, OnbtnLoadPort3IoClick		)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_LOAD_STAGE1_X_0	, IDC_BTN_LOAD_STAGE1_X_3	, OnbtnLoadStage1XClick		)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_LOAD_STAGE1_Z_0	, IDC_BTN_LOAD_STAGE1_Z_4	, OnbtnLoadStage1ZClick		)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_LOAD_STAGE1_IO_0	, IDC_BTN_LOAD_STAGE1_IO_3	, OnbtnLoadStage1IoClick	)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_LOAD_STAGE2_X_0	, IDC_BTN_LOAD_STAGE2_X_3	, OnbtnLoadStage2XClick		)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_LOAD_STAGE2_Z_0	, IDC_BTN_LOAD_STAGE2_Z_4	, OnbtnLoadStage2ZClick		)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_LOAD_STAGE2_IO_0	, IDC_BTN_LOAD_STAGE2_IO_3	, OnbtnLoadStage2IoClick	)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_LOAD_PICKER_Y_0	, IDC_BTN_LOAD_PICKER_Y_4	, OnbtnLoadPickerYClick		)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_LOAD_PICKER_Z_0	, IDC_BTN_LOAD_PICKER_Z_3	, OnbtnLoadPickerZClick		)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_LOAD_PICKER_P_0	, IDC_BTN_LOAD_PICKER_P_1	, OnbtnLoadPickerPClick		)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_LOAD_PICKER_IO_0	, IDC_BTN_LOAD_PICKER_IO_27	, OnbtnLoadPickerIoClick	)
END_MESSAGE_MAP()

// CManualLoadDlg 메시지 처리기입니다.

BOOL CManualLoadDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(this, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	Initial_Controls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CManualLoadDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CManualLoadDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (!bShow) return;

	Display_Status();

	BOOL bOn = TRUE;
	if (gData.nPickerUseCnt < 6) { bOn = FALSE; }

	m_btnLoadPickerIo[ 5].EnableWindow(bOn);	// Picker 6 Up
	m_btnLoadPickerIo[11].EnableWindow(bOn);	// Picker 6 Down
	m_btnLoadPickerIo[17].EnableWindow(bOn);	// Picker 6 Open
	m_btnLoadPickerIo[23].EnableWindow(bOn);	// Picker 6 Close

	bOn = TRUE;
	if (gData.nPickerUseCnt < 5) { bOn = FALSE; }

	m_btnLoadPickerIo[ 4].EnableWindow(bOn);	// Picker 5 Up
	m_btnLoadPickerIo[10].EnableWindow(bOn);	// Picker 5 Down
	m_btnLoadPickerIo[16].EnableWindow(bOn);	// Picker 5 Open
	m_btnLoadPickerIo[22].EnableWindow(bOn);	// Picker 5 Close

	bOn = TRUE;
	if (gData.nPickerUseCnt < 4) { bOn = FALSE; }

	m_btnLoadPickerIo[ 3].EnableWindow(bOn);	// Picker 4 Up
	m_btnLoadPickerIo[ 9].EnableWindow(bOn);	// Picker 4 Down
	m_btnLoadPickerIo[15].EnableWindow(bOn);	// Picker 4 Open
	m_btnLoadPickerIo[21].EnableWindow(bOn);	// Picker 4 Close

	m_strLog.Format("[Manual Load] Show Window");
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualLoadDlg::Initial_Controls() 
{
	for (int i =  0; i < 4; i++) m_Group[i].Init_Ctrl("Arial", 11, TRUE, RGB(0x30, 0x80, 0x00), COLOR_DEFAULT);
	for (int i =  4; i < 7; i++) m_Group[i].Init_Ctrl("Arial", 11, TRUE, RGB(0xD0, 0x10, 0x10), COLOR_DEFAULT);
	for (int i =  7; i < 10; i++) m_Group[i].Init_Ctrl("Arial", 11, TRUE, RGB(0xD0, 0x10, 0xD0), COLOR_DEFAULT);
	for (int i = 10; i < 13; i++) m_Group[i].Init_Ctrl("Arial", 11, TRUE, RGB(0x00, 0x00, 0xFF), COLOR_DEFAULT);
	for (int i = 13; i < 17; i++) m_Group[i].Init_Ctrl("Arial", 11, TRUE, RGB(0x30, 0x80, 0x00), COLOR_DEFAULT);
	for (int i = 0; i < 11; i++) m_Label[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, RGB(0xB0, 0xB0, 0xB0));
	for (int i = 0; i < 11; i++) m_stcAxisPos[i].Init_Ctrl("Arial", 10, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x10, 0x10, 0x60));
	for (int i = 0; i <  3; i++) m_btnTrayPickerX[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  3; i++) m_btnTrayPickerZ[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  2; i++) m_btnTrayPickerR[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  4; i++) m_btnTrayPickerIo[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  9; i++) m_ledTrayPickerIo[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i <  4; i++) m_btnLoadPort1Io[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  9; i++) m_ledLoadPort1Io[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i <  4; i++) m_btnLoadPort2Io[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 10; i++) m_ledLoadPort2Io[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i <  2; i++) m_btnLoadPort3Io[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  7; i++) m_ledLoadPort3Io[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i <  4; i++) m_btnLoadStage1X[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  5; i++) m_btnLoadStage1Z[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  4; i++) m_btnLoadStage1Io[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  5; i++) m_ledLoadStage1Io[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i <  4; i++) m_btnLoadStage2X[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  5; i++) m_btnLoadStage2Z[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  4; i++) m_btnLoadStage2Io[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  5; i++) m_ledLoadStage2Io[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i <  5; i++) m_btnLoadPickerX[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  4; i++) m_btnLoadPickerZ[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  2; i++) m_btnLoadPickerP[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 28; i++) m_btnLoadPickerIo[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 24; i++) m_ledLoadPickerIo[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
}

void CManualLoadDlg::Display_Status()
{
	CString strPos;
	for (int i = 0; i < 11; i++) {
		double dPos = g_objAJinAXL.Get_Position(AX_TRAY_PICKER_X + i);
		strPos.Format("%0.3lf", dPos);
		m_stcAxisPos[i].SetWindowText(strPos);
	}

	DX_DATA_00 *pDX00 = g_objAJinAXL.Get_pDX00();
	DX_DATA_01 *pDX01 = g_objAJinAXL.Get_pDX01();
	DX_DATA_04 *pDX04 = g_objAJinAXL.Get_pDX04();
	DX_DATA_06 *pDX06 = g_objAJinAXL.Get_pDX06();

	m_ledTrayPickerIo[0].Set_On(pDX00->iTrayPickerExist);
	m_ledTrayPickerIo[1].Set_On(pDX00->iTrayPickerGrip1Open);
	m_ledTrayPickerIo[2].Set_On(pDX00->iTrayPickerGrip2Open);
	m_ledTrayPickerIo[3].Set_On(pDX00->iTrayPickerGrip3Open);
	m_ledTrayPickerIo[4].Set_On(pDX00->iTrayPickerGrip4Open);
	m_ledTrayPickerIo[5].Set_On(pDX00->iTrayPickerGrip1Close);
	m_ledTrayPickerIo[6].Set_On(pDX00->iTrayPickerGrip2Close);
	m_ledTrayPickerIo[7].Set_On(pDX00->iTrayPickerGrip3Close);
	m_ledTrayPickerIo[8].Set_On(pDX00->iTrayPickerGrip4Close);

	m_ledLoadPort1Io[0].Set_On(pDX00->iLoadPort1LowCheck);
	m_ledLoadPort1Io[1].Set_On(pDX00->iLoadPort1SlideOpen);
	m_ledLoadPort1Io[2].Set_On(pDX00->iLoadPort1SlideClose);
	m_ledLoadPort1Io[3].Set_On(pDX00->iLoadPort1SlideLock);
	m_ledLoadPort1Io[4].Set_On(pDX00->iLoadPort1SlideUnlock);
	m_ledLoadPort1Io[5].Set_On(pDX00->iLoadPort1Support1In);
	m_ledLoadPort1Io[6].Set_On(pDX00->iLoadPort1Support2In);
	m_ledLoadPort1Io[7].Set_On(pDX00->iLoadPort1Support1Out);
	m_ledLoadPort1Io[8].Set_On(pDX00->iLoadPort1Support2Out);

	m_ledLoadPort2Io[0].Set_On(pDX01->iLoadPort2LowCheck);
	m_ledLoadPort2Io[1].Set_On(pDX01->iLoadPort2SlideOpen);
	m_ledLoadPort2Io[2].Set_On(pDX01->iLoadPort2SlideClose);
	m_ledLoadPort2Io[3].Set_On(pDX01->iLoadPort2SlideLock);
	m_ledLoadPort2Io[4].Set_On(pDX01->iLoadPort2SlideUnlock);
	m_ledLoadPort2Io[5].Set_On(pDX01->iLoadPort2Support1In);
	m_ledLoadPort2Io[6].Set_On(pDX01->iLoadPort2Support2In);
	m_ledLoadPort2Io[7].Set_On(pDX01->iLoadPort2Support1Out);
	m_ledLoadPort2Io[8].Set_On(pDX01->iLoadPort2Support2Out);
	m_ledLoadPort2Io[9].Set_On(!pDX01->iLoadPortAreaCheck);

	m_ledLoadPort3Io[0].Set_On(pDX01->iLoadPort3LowCheck);
	m_ledLoadPort3Io[1].Set_On(pDX01->iLoadPort3HighCheck);
	m_ledLoadPort3Io[2].Set_On(pDX01->iLoadPort3SlideOpen);
	m_ledLoadPort3Io[3].Set_On(pDX01->iLoadPort3SlideClose);
	m_ledLoadPort3Io[4].Set_On(pDX01->iLoadPort3SlideLock);
	m_ledLoadPort3Io[5].Set_On(pDX01->iLoadPort3SlideUnlock);
	m_ledLoadPort3Io[6].Set_On(!pDX01->iLoadPort3AreaCheck);

	m_ledLoadStage1Io[0].Set_On(pDX04->iLoadStage1MasterIn);
	m_ledLoadStage1Io[1].Set_On(pDX04->iLoadStage1MasterOut);
	m_ledLoadStage1Io[2].Set_On(pDX04->iLoadStage1SlaveIn);
	m_ledLoadStage1Io[3].Set_On(pDX04->iLoadStage1SlaveOut);
	m_ledLoadStage1Io[4].Set_On(pDX04->iLoadStage1Exist);

	m_ledLoadStage2Io[0].Set_On(pDX04->iLoadStage2MasterIn);
	m_ledLoadStage2Io[1].Set_On(pDX04->iLoadStage2MasterOut);
	m_ledLoadStage2Io[2].Set_On(pDX04->iLoadStage2SlaveIn);
	m_ledLoadStage2Io[3].Set_On(pDX04->iLoadStage2SlaveOut);
	m_ledLoadStage2Io[4].Set_On(pDX04->iLoadStage2Exist);

	m_ledLoadPickerIo[ 0].Set_On(pDX06->iLoadPickerUp01);
	m_ledLoadPickerIo[ 1].Set_On(pDX06->iLoadPickerUp02);
	m_ledLoadPickerIo[ 2].Set_On(pDX06->iLoadPickerUp03);
	m_ledLoadPickerIo[ 3].Set_On(pDX06->iLoadPickerUp04);
	m_ledLoadPickerIo[ 4].Set_On(pDX06->iLoadPickerUp05);
	m_ledLoadPickerIo[ 5].Set_On(pDX06->iLoadPickerUp06);
	m_ledLoadPickerIo[ 6].Set_On(pDX06->iLoadPickerDown01);
	m_ledLoadPickerIo[ 7].Set_On(pDX06->iLoadPickerDown02);
	m_ledLoadPickerIo[ 8].Set_On(pDX06->iLoadPickerDown03);
	m_ledLoadPickerIo[ 9].Set_On(pDX06->iLoadPickerDown04);
	m_ledLoadPickerIo[10].Set_On(pDX06->iLoadPickerDown05);
	m_ledLoadPickerIo[11].Set_On(pDX06->iLoadPickerDown06);
	m_ledLoadPickerIo[12].Set_On(pDX06->iLoadPickerOpen01);
	m_ledLoadPickerIo[13].Set_On(pDX06->iLoadPickerOpen02);
	m_ledLoadPickerIo[14].Set_On(pDX06->iLoadPickerOpen03);
	m_ledLoadPickerIo[15].Set_On(pDX06->iLoadPickerOpen04);
	m_ledLoadPickerIo[16].Set_On(pDX06->iLoadPickerOpen05);
	m_ledLoadPickerIo[17].Set_On(pDX06->iLoadPickerOpen06);
	m_ledLoadPickerIo[18].Set_On(pDX06->iLoadPickerCMCheck01);
	m_ledLoadPickerIo[19].Set_On(pDX06->iLoadPickerCMCheck02);
	m_ledLoadPickerIo[20].Set_On(pDX06->iLoadPickerCMCheck03);
	m_ledLoadPickerIo[21].Set_On(pDX06->iLoadPickerCMCheck04);
	m_ledLoadPickerIo[22].Set_On(pDX06->iLoadPickerCMCheck05);
	m_ledLoadPickerIo[23].Set_On(pDX06->iLoadPickerCMCheck06);
}

///////////////////////////////////////////////////////////////////////////////

void CManualLoadDlg::OnbtnTrayPickerXClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_TRAY_PICKER_X)) return;
	if (!g_objAJinAXL.Is_Home(AX_TRAY_PICKER_Z)) return;

	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_TRAY_PICKER_X_0;

	if (!g_objCommon.Check_Position(AX_TRAY_PICKER_Z, 0)) {
		AfxMessageBox("Tray Picker Z축이 Ready Up 위치인지 확인 후 진행하세요."); return;
	}

	g_objCommon.Move_Position(AX_TRAY_PICKER_X, nIndex);

	m_strLog.Format("[Manual Load] Tray Picker X (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualLoadDlg::OnbtnTrayPickerZClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_TRAY_PICKER_Z)) return;

	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_TRAY_PICKER_Z_0;

	if (nIndex == 1 && !g_objCommon.Check_Position(AX_TRAY_PICKER_X, 0)) {
		AfxMessageBox("Tray Picker X축이 AVI 위치인지 확인 후 진행하세요."); return;
	}
	if (nIndex == 2 && !g_objCommon.Check_Position(AX_TRAY_PICKER_X, 1)) {
		AfxMessageBox("Tray Picker X축이 Load1 위치인지 확인 후 진행하세요."); return;
	}
	g_objCommon.Move_Position(AX_TRAY_PICKER_Z, nIndex);

	m_strLog.Format("[Manual Load] Tray Picker Z (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualLoadDlg::OnbtnTrayPickerRClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_TRAY_PICKER_Z)) return;

	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_TRAY_PICKER_R_0;

	if (!g_objCommon.Check_Position(AX_TRAY_PICKER_Z, 0)) {
		AfxMessageBox("Tray Picker Z축이 Ready Up 위치인지 확인 후 진행하세요."); return;
	}

	g_objCommon.Move_Position(AX_TRAY_PICKER_R, nIndex);

	m_strLog.Format("[Manual Load] Tray Picker R (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualLoadDlg::OnbtnTrayPickerIoClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_TRAY_PICKER_IO_0;

	DY_DATA_00 *pDY00 = g_objAJinAXL.Get_pDY00();

	if (nIndex == 0) { pDY00->oTrayPickerGrip12Open  = TRUE; pDY00->oTrayPickerGrip12Close = FALSE; }
	if (nIndex == 1) { pDY00->oTrayPickerGrip34Open  = TRUE; pDY00->oTrayPickerGrip34Close = FALSE; }
	if (nIndex == 2) { pDY00->oTrayPickerGrip12Close = TRUE; pDY00->oTrayPickerGrip12Open  = FALSE; }
	if (nIndex == 3) { pDY00->oTrayPickerGrip34Close = TRUE; pDY00->oTrayPickerGrip34Open  = FALSE; }

	g_objAJinAXL.Write_Output(0);

	m_strLog.Format("[Manual Load] Tray Picker IO (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualLoadDlg::OnbtnLoadPort1IoClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_LOAD_PORT1_IO_0;

	DY_DATA_00 *pDY00 = g_objAJinAXL.Get_pDY00();

	if (nIndex == 0) { pDY00->oLoadPort1SlideLock   = TRUE; pDY00->oLoadPort1SlideUnlock = FALSE; }
	if (nIndex == 1) { pDY00->oLoadPort1SlideUnlock = TRUE; pDY00->oLoadPort1SlideLock   = FALSE; }
	if (nIndex == 2) { pDY00->oLoadPort1SupportIn   = TRUE; pDY00->oLoadPort1SupportOut  = FALSE; }
	if (nIndex == 3) { pDY00->oLoadPort1SupportOut  = TRUE; pDY00->oLoadPort1SupportIn   = FALSE; }

	g_objAJinAXL.Write_Output(0);

	m_strLog.Format("[Manual Load] Load Port1 IO (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualLoadDlg::OnbtnLoadPort2IoClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_LOAD_PORT2_IO_0;

	DY_DATA_01 *pDY01 = g_objAJinAXL.Get_pDY01();

	if (nIndex == 0) { pDY01->oLoadPort2SlideLock   = TRUE; pDY01->oLoadPort2SlideUnlock = FALSE; }
	if (nIndex == 1) { pDY01->oLoadPort2SlideUnlock = TRUE; pDY01->oLoadPort2SlideLock   = FALSE; }
	if (nIndex == 2) { pDY01->oLoadPort2SupportIn   = TRUE; pDY01->oLoadPort2SupportOut  = FALSE; }
	if (nIndex == 3) { pDY01->oLoadPort2SupportOut  = TRUE; pDY01->oLoadPort2SupportIn   = FALSE; }

	g_objAJinAXL.Write_Output(1);

	m_strLog.Format("[Manual Load] Load Port2 IO (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualLoadDlg::OnbtnLoadPort3IoClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_LOAD_PORT3_IO_0;

	DY_DATA_01 *pDY01 = g_objAJinAXL.Get_pDY01();

	if (nIndex == 0) { pDY01->oLoadPort3SlideLock   = TRUE; pDY01->oLoadPort3SlideUnlock = FALSE; }
	if (nIndex == 1) { pDY01->oLoadPort3SlideUnlock = TRUE; pDY01->oLoadPort3SlideLock   = FALSE; }

	g_objAJinAXL.Write_Output(1);

	m_strLog.Format("[Manual Load] Load Port3 (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualLoadDlg::OnbtnLoadStage1XClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_LOAD_STAGE1_X)) return;
	if (!g_objAJinAXL.Is_Home(AX_LOAD_STAGE1_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_LOAD_STAGE2_Z)) return;

	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_LOAD_STAGE1_X_0;

	if (!g_objCommon.Check_Position(AX_LOAD_PICKER_Y, 2) && !g_objCommon.Check_Position(AX_LOAD_PICKER_Z, 0)) {
		AfxMessageBox("Load Picker Ready Up 위치가 아닙니다. 확인 후 진행하세요."); return;
	}
	if ((g_objCommon.Check_Position(AX_LOAD_STAGE1_Z, 0) && g_objCommon.Check_Position(AX_LOAD_STAGE2_Z, 0)) ||
		(g_objCommon.Check_Position(AX_LOAD_STAGE1_Z, 1) && g_objCommon.Check_Position(AX_LOAD_STAGE2_Z, 1))) {
			AfxMessageBox("Load Stage1, 2 Z축 높이가 같습니다. 확인 후 진행하세요."); return;
	}

	if ((g_objCommon.Check_Position(AX_LOAD_STAGE1_Z, 0) && g_objCommon.Check_Position(AX_LOAD_STAGE2_Z, 1)) ||
		(g_objCommon.Check_Position(AX_LOAD_STAGE1_Z, 1) && g_objCommon.Check_Position(AX_LOAD_STAGE2_Z, 0))) {

			g_objCommon.Move_Position(AX_LOAD_STAGE1_X, nIndex);

	} else {
		AfxMessageBox("Load Stage1, 2 Z축 높이 확인 후 진행하세요."); return;
	}

	m_strLog.Format("[Manual Load] Load Stage1 X (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualLoadDlg::OnbtnLoadStage1ZClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_LOAD_STAGE1_X)) return;
	if (!g_objAJinAXL.Is_Home(AX_LOAD_STAGE1_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_LOAD_STAGE2_X)) return;

	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_LOAD_STAGE1_Z_0;

	double dStage1 = g_objAJinAXL.Get_Position(AX_LOAD_STAGE1_X);
	double dStage2 = g_objAJinAXL.Get_Position(AX_LOAD_STAGE2_X);
	double dDiff = fabs(dStage1 - dStage2);
	if (dDiff < 300.0) { AfxMessageBox("Load Stage1, 2 충돌 위험!!! X축 위치 확인 후 진행하세요."); return; }

	if (nIndex == 2 || nIndex == 3) {
		if (!g_objCommon.Check_Position(AX_LOAD_STAGE1_X, 0) && !g_objCommon.Check_Position(AX_LOAD_STAGE1_X, 1)) {
			AfxMessageBox("Load Port1또는 Port2 위치가 아닙니다. X축 확인 후 진행하세요."); return;
		}
	}
	if (nIndex == 4) {
		if (!g_objCommon.Check_Position(AX_LOAD_STAGE1_X, 3)) {
			AfxMessageBox("Unload 위치가 아닙니다. X축 확인 후 진행하세요."); return;
		}
	}

	g_objCommon.Move_Position(AX_LOAD_STAGE1_Z, nIndex);

	m_strLog.Format("[Manual Load] Load Stage1 Z (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualLoadDlg::OnbtnLoadStage1IoClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_LOAD_STAGE1_IO_0;

	DY_DATA_04 *pDY04 = g_objAJinAXL.Get_pDY04();

	if (nIndex == 0) { pDY04->oLoadStage1MasterIn = TRUE; }
	if (nIndex == 1) { pDY04->oLoadStage1SlaveIn  = TRUE; }
	if (nIndex == 2) { pDY04->oLoadStage1MasterIn = FALSE; }
	if (nIndex == 3) { pDY04->oLoadStage1SlaveIn  = FALSE; }

	g_objAJinAXL.Write_Output(4);

	m_strLog.Format("[Manual Load] Load Stage1 IO (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualLoadDlg::OnbtnLoadStage2XClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_LOAD_STAGE2_X)) return;
	if (!g_objAJinAXL.Is_Home(AX_LOAD_STAGE2_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_LOAD_STAGE1_Z)) return;

	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_LOAD_STAGE2_X_0;

	if (!g_objCommon.Check_Position(AX_LOAD_PICKER_Y, 2) && !g_objCommon.Check_Position(AX_LOAD_PICKER_Z, 0)) {
		AfxMessageBox("Load Picker Ready Up 위치가 아닙니다. 확인 후 진행하세요."); return;
	}
	if ((g_objCommon.Check_Position(AX_LOAD_STAGE1_Z, 0) && g_objCommon.Check_Position(AX_LOAD_STAGE2_Z, 0)) ||
		(g_objCommon.Check_Position(AX_LOAD_STAGE1_Z, 1) && g_objCommon.Check_Position(AX_LOAD_STAGE2_Z, 1))) {
			AfxMessageBox("Load Stage1, 2 Z축 높이가 같습니다. 확인 후 진행하세요."); return;
	}

	if ((g_objCommon.Check_Position(AX_LOAD_STAGE1_Z, 0) && g_objCommon.Check_Position(AX_LOAD_STAGE2_Z, 1)) ||
		(g_objCommon.Check_Position(AX_LOAD_STAGE1_Z, 1) && g_objCommon.Check_Position(AX_LOAD_STAGE2_Z, 0))) {

			g_objCommon.Move_Position(AX_LOAD_STAGE2_X, nIndex);

	} else {
		AfxMessageBox("Load Stage1, 2 Z축 높이 확인 후 진행하세요."); return;
	}

	m_strLog.Format("[Manual Load] Load Stage2 X (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualLoadDlg::OnbtnLoadStage2ZClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_LOAD_STAGE2_X)) return;
	if (!g_objAJinAXL.Is_Home(AX_LOAD_STAGE2_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_LOAD_STAGE1_X)) return;

	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_LOAD_STAGE2_Z_0;

	double dStage1 = g_objAJinAXL.Get_Position(AX_LOAD_STAGE1_X);
	double dStage2 = g_objAJinAXL.Get_Position(AX_LOAD_STAGE2_X);
	double dDiff = fabs(dStage1 - dStage2);
	if (dDiff < 300.0) { AfxMessageBox("Load Stage1, 2 충돌 위험!!! X축 위치 확인 후 진행하세요."); return; }

	if (nIndex == 2 || nIndex == 3) {
		if (!g_objCommon.Check_Position(AX_LOAD_STAGE2_X, 0) && !g_objCommon.Check_Position(AX_LOAD_STAGE2_X, 1)) {
			AfxMessageBox("Load Port1또는 Port2 위치가 아닙니다. X축 확인 후 진행하세요."); return;
		}
	}
	if (nIndex == 4) {
		if (!g_objCommon.Check_Position(AX_LOAD_STAGE2_X, 3)) {
			AfxMessageBox("Unload 위치가 아닙니다. X축 확인 후 진행하세요."); return;
		}
	}

	g_objCommon.Move_Position(AX_LOAD_STAGE2_Z, nIndex);

	m_strLog.Format("[Manual Load] Load Stage2 Z (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualLoadDlg::OnbtnLoadStage2IoClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_LOAD_STAGE2_IO_0;

	DY_DATA_04 *pDY04 = g_objAJinAXL.Get_pDY04();

	if (nIndex == 0) { pDY04->oLoadStage2MasterIn = TRUE; }
	if (nIndex == 1) { pDY04->oLoadStage2SlaveIn  = TRUE; }
	if (nIndex == 2) { pDY04->oLoadStage2MasterIn = FALSE; }
	if (nIndex == 3) { pDY04->oLoadStage2SlaveIn  = FALSE; }

	g_objAJinAXL.Write_Output(4);

	m_strLog.Format("[Manual Load] Load Stage2 IO (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualLoadDlg::OnbtnLoadPickerYClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_LOAD_PICKER_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_LOAD_PICKER_Y)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_LOAD_PICKER_Y_0;

	DX_DATA_06 *pDX06 = g_objAJinAXL.Get_pDX06();

	if (!g_objCommon.Check_Position(AX_LOAD_PICKER_Z, 0)) {
		AfxMessageBox("Z축이 Ready Up 위치가 아닙니다. Z축 확인 후 진행하세요."); return;
	}
	if (!pDX06->iLoadPickerUp01 || !pDX06->iLoadPickerUp02 || !pDX06->iLoadPickerUp03 ||
		!pDX06->iLoadPickerUp04 || !pDX06->iLoadPickerUp05 || !pDX06->iLoadPickerUp06) {

		AfxMessageBox("Load Picker Up 상태가 아닙니다. 확인 후 진행하세요."); return;
	}
	if (!g_objCommon.Check_Position(AX_VISION_CM_ALIGN_X, 0)) {
		AfxMessageBox("Vision CM Align X축이 Ready 위치가 아닙니다. 확인 후 진행하세요."); return;
	}

	if (nIndex == 3 || nIndex == 4) {
		EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
		int nDir = (nIndex == 3 ? 1 : -1);
		double dMove = pEquipData->dLoadTrayPitchY * nDir;
		g_objAJinAXL.Move_Relative(AX_LOAD_PICKER_Y, dMove);

	} else {
		g_objCommon.Move_Position(AX_LOAD_PICKER_Y, nIndex);
	}	

	m_strLog.Format("[Manual Load] Load Picker Y (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualLoadDlg::OnbtnLoadPickerZClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_LOAD_PICKER_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_LOAD_PICKER_Y)) return;

 	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_LOAD_PICKER_Z_0;
	
	MOVE_DATA *pMoveData = g_objDataManager.Get_pMoveData();
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	if (nIndex == 1 && !g_objCommon.Check_Position(AX_LOAD_PICKER_Y, 0)) {
		double dCur = g_objAJinAXL.Get_Position(AX_LOAD_PICKER_Y);
		double dPos1, dPos2;

		dPos1 = pMoveData->dLoadPickerY[0];
		dPos2 = pMoveData->dLoadPickerY[0] - ( pEquipData->dLoadTrayPitchY * (pEquipData->nLoadTrayY-1));
		if ((dPos1 + 1.0) < dCur || (dPos2 - 1.0) > dCur) {
			AfxMessageBox("Y축이 Load Stage1 위치가 아닙니다. 확인 후 진행하세요."); return;
		}
	}
	if (nIndex == 2 && !g_objCommon.Check_Position(AX_LOAD_PICKER_Y, 1)) {
		double dCur = g_objAJinAXL.Get_Position(AX_LOAD_PICKER_Y);
		double dPos1, dPos2;
		
		dPos1 = pMoveData->dLoadPickerY[1];
		dPos2 = pMoveData->dLoadPickerY[1] - ( pEquipData->dLoadTrayPitchY * (pEquipData->nLoadTrayY-1));
		if ((dPos1 + 1.0) < dCur || (dPos2 - 1.0) > dCur) {
			AfxMessageBox("Y축이 Load Stage2 위치가 아닙니다. 확인 후 진행하세요."); return;
		}
	}
	if (nIndex == 3 && !g_objCommon.Check_Position(AX_LOAD_PICKER_Y, 2)) {
		AfxMessageBox("Y축이 Index 위치가 아닙니다. 확인 후 진행하세요."); return;
	}

	g_objCommon.Move_Position(AX_LOAD_PICKER_Z, nIndex);

	m_strLog.Format("[Manual Load] Load Picker Z (%d)", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualLoadDlg::OnbtnLoadPickerPClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_LOAD_PICKER_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_LOAD_PICKER_P)) return;

	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_LOAD_PICKER_P_0;

	if (!g_objCommon.Check_Position(AX_LOAD_PICKER_Z, 0)) {
		AfxMessageBox("Z축이 Ready Up 위치가 아닙니다. Z축 확인 후 진행하세요."); return;
	}

	g_objCommon.Move_Position(AX_LOAD_PICKER_P, nIndex);

	m_strLog.Format("[Manual Load] Load Picker P (%d)", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualLoadDlg::OnbtnLoadPickerIoClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_LOAD_PICKER_IO_0;

	DX_DATA_06 *pDX06 = g_objAJinAXL.Get_pDX06();
	DY_DATA_06 *pDY06 = g_objAJinAXL.Get_pDY06();
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	// Up
	if (nIndex == 0 || nIndex == 24) pDY06->oLoadPickerDown01 = FALSE;
	if (nIndex == 1 || nIndex == 24) pDY06->oLoadPickerDown02 = FALSE;
	if (nIndex == 2 || nIndex == 24) pDY06->oLoadPickerDown03 = FALSE;
	if (nIndex == 3 || nIndex == 24) pDY06->oLoadPickerDown04 = FALSE;
	if (nIndex == 4 || nIndex == 24) pDY06->oLoadPickerDown05 = FALSE;
	if (nIndex == 5 || nIndex == 24) pDY06->oLoadPickerDown06 = FALSE;

	//Down
	if (nIndex ==  6 || nIndex == 25) pDY06->oLoadPickerDown01 = TRUE;
	if (nIndex ==  7 || nIndex == 25) pDY06->oLoadPickerDown02 = TRUE;
	if (nIndex ==  8 || nIndex == 25) pDY06->oLoadPickerDown03 = TRUE;
	if (gData.nPickerUseCnt > 3) { if (nIndex ==  9 || nIndex == 25) pDY06->oLoadPickerDown04 = TRUE; }
	if (gData.nPickerUseCnt > 4) { if (nIndex == 10 || nIndex == 25) pDY06->oLoadPickerDown05 = TRUE; }
	if (gData.nPickerUseCnt > 5) { if (nIndex == 11 || nIndex == 25) pDY06->oLoadPickerDown06 = TRUE; }
	
	//Open
	if (nIndex == 12 || nIndex == 26) pDY06->oLoadPickerGrip01 = FALSE;
	if (nIndex == 13 || nIndex == 26) pDY06->oLoadPickerGrip02 = FALSE;
	if (nIndex == 14 || nIndex == 26) pDY06->oLoadPickerGrip03 = FALSE;
	if (nIndex == 15 || nIndex == 26) pDY06->oLoadPickerGrip04 = FALSE;
	if (nIndex == 16 || nIndex == 26) pDY06->oLoadPickerGrip05 = FALSE;
	if (nIndex == 17 || nIndex == 26) pDY06->oLoadPickerGrip06 = FALSE;

	//Close
	if (nIndex == 18 || nIndex == 27) pDY06->oLoadPickerGrip01 = TRUE;
	if (nIndex == 19 || nIndex == 27) pDY06->oLoadPickerGrip02 = TRUE;
	if (nIndex == 20 || nIndex == 27) pDY06->oLoadPickerGrip03 = TRUE;
	if (gData.nPickerUseCnt > 3) { if (nIndex == 21 || nIndex == 27) pDY06->oLoadPickerGrip04 = TRUE; }
	if (gData.nPickerUseCnt > 4) { if (nIndex == 22 || nIndex == 27) pDY06->oLoadPickerGrip05 = TRUE; }
	if (gData.nPickerUseCnt > 5) { if (nIndex == 23 || nIndex == 27) pDY06->oLoadPickerGrip06 = TRUE; }

	g_objAJinAXL.Write_Output(6);

	m_strLog.Format("[Manual Load] Load Picker IO (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

///////////////////////////////////////////////////////////////////////////////
