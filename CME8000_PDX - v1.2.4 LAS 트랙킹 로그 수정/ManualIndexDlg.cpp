// ManualIndexDlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CME8000.h"
#include "ManualIndexDlg.h"
#include "afxdialogex.h"
#include "math.h"

#include "LogFile.h"
#include "AJinAXL.h"
#include "DataManager.h"
#include "Common.h"
#include "SequenceMain.h"
#include "LoadCell.h"

// CManualIndexDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CManualIndexDlg, CDialogEx)

CManualIndexDlg::CManualIndexDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CManualIndexDlg::IDD, pParent)
{
}

CManualIndexDlg::~CManualIndexDlg()
{
}

void CManualIndexDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for (int i = 0; i <  9; i++) DDX_Control(pDX, IDC_GROUP_0 + i, m_Group[i]);
	for (int i = 0; i <  6; i++) DDX_Control(pDX, IDC_LABEL_0 + i, m_Label[i]);
	for (int i = 0; i <  6; i++) DDX_Control(pDX, IDC_STC_AXIS_POS_0 + i, m_stcAxisPos[i]);
	DDX_Control(pDX, IDC_STC_INDEX_POS, m_stcIndexPos);
	for (int i = 0; i <  2; i++) DDX_Control(pDX, IDC_BTN_INDEX_R_0 + i, m_btnIndexR[i]);
	for (int i = 0; i <  2; i++) DDX_Control(pDX, IDC_LED_INDEX_POS_0 + i, m_ledIndexPos[i]);
	for (int i = 0; i < 38; i++) DDX_Control(pDX, IDC_BTN_INDEX_IO_0 + i, m_btnIndexIo[i]);
	for (int i = 0; i < 22; i++) DDX_Control(pDX, IDC_LED_INDEX_IO_0 + i, m_ledIndexIo[i]);
	for (int i = 0; i <  4; i++) DDX_Control(pDX, IDC_BTN_VISION_CM_X_0 + i, m_btnVisionCMAlignX[i]);
	for (int i = 0; i <  3; i++) DDX_Control(pDX, IDC_BTN_VISION_CAP_Y_0 + i, m_btnVisionCapAlignY[i]);
	for (int i = 0; i <  7; i++) DDX_Control(pDX, IDC_BTN_ASSY_PICKER_X_0 + i, m_btnAssyPickerX[i]);
	for (int i = 0; i <  7; i++) DDX_Control(pDX, IDC_BTN_ASSY_PICKER_Y_0 + i, m_btnAssyPickerY[i]);
	for (int i = 0; i <  7; i++) DDX_Control(pDX, IDC_BTN_ASSY_PICKER_Z_0 + i, m_btnAssyPickerZ[i]);
	for (int i = 0; i < 31; i++) DDX_Control(pDX, IDC_BTN_ASSY_PICKER_IO_0 + i, m_btnAssyPickerIo[i]);
	for (int i = 0; i < 27; i++) DDX_Control(pDX, IDC_LED_ASSY_PICKER_IO_0 + i, m_ledAssyPickerIo[i]);
	for (int i = 0; i <  6; i++) DDX_Control(pDX, IDC_STC_ASSY_LOAD_CELL_0 + i, m_stcAssyLoadCell[i]);
}

BEGIN_MESSAGE_MAP(CManualIndexDlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_INDEX_R_0, IDC_BTN_INDEX_R_1, OnBtnIndexRClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_INDEX_IO_0, IDC_BTN_INDEX_IO_37, OnBtnIndexIoClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_VISION_CM_X_0, IDC_BTN_VISION_CM_X_3, OnBtnVisionCMAlignXClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_VISION_CAP_Y_0, IDC_BTN_VISION_CAP_Y_2, OnBtnVisionCapAlignYClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_ASSY_PICKER_X_0, IDC_BTN_ASSY_PICKER_X_6, OnBtnAssyPickerXClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_ASSY_PICKER_Y_0, IDC_BTN_ASSY_PICKER_Y_6, OnBtnAssyPickerYClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_ASSY_PICKER_Z_0, IDC_BTN_ASSY_PICKER_Z_6, OnBtnAssyPickerZClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_ASSY_PICKER_IO_0, IDC_BTN_ASSY_PICKER_IO_30, OnBtnAssyPickerIoClick)
END_MESSAGE_MAP()

// CManualIndexDlg 메시지 처리기입니다.
BOOL CManualIndexDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(this, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	Initial_Controls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CManualIndexDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CManualIndexDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (!bShow) return;

	Display_Status();
	CString sLoadCell;
	for (int i=0; i<6; i++) {
		sLoadCell.Format("%0.3lf", gData.dAssyLoadCell[i]);
		m_stcAssyLoadCell[i].SetWindowText(sLoadCell);
	}

	BOOL bOn = TRUE;
	if (gData.nPickerUseCnt < 6) { bOn = FALSE; }

	m_btnIndexIo[15].EnableWindow(bOn);	// Picker 6 Up
	m_btnIndexIo[21].EnableWindow(bOn);	// Picker 6 Down
	m_btnIndexIo[27].EnableWindow(bOn);	// Picker 6 Vac On
	m_btnIndexIo[33].EnableWindow(bOn);	// Picker 6 Vac Off
	m_btnAssyPickerIo[ 5].EnableWindow(bOn);	// Picker 6 Up
	m_btnAssyPickerIo[11].EnableWindow(bOn);	// Picker 6 Down
	m_btnAssyPickerIo[17].EnableWindow(bOn);	// Picker 6 Vac On
	m_btnAssyPickerIo[23].EnableWindow(bOn);	// Picker 6 Vac Off

	bOn = TRUE;
	if (gData.nPickerUseCnt < 5) { bOn = FALSE; }

	m_btnIndexIo[14].EnableWindow(bOn);	// Picker 5 Up
	m_btnIndexIo[20].EnableWindow(bOn);	// Picker 5 Down
	m_btnIndexIo[26].EnableWindow(bOn);	// Picker 5 Vac On
	m_btnIndexIo[32].EnableWindow(bOn);	// Picker 5 Vac Off
	m_btnAssyPickerIo[ 4].EnableWindow(bOn);	// Picker 5 Up
	m_btnAssyPickerIo[10].EnableWindow(bOn);	// Picker 5 Down
	m_btnAssyPickerIo[16].EnableWindow(bOn);	// Picker 5 Vac On
	m_btnAssyPickerIo[22].EnableWindow(bOn);	// Picker 5 Vac Off

	bOn = TRUE;
	if (gData.nPickerUseCnt < 4) { bOn = FALSE; }

	m_btnIndexIo[13].EnableWindow(bOn);	// Picker 4 Up
	m_btnIndexIo[19].EnableWindow(bOn);	// Picker 4 Down
	m_btnIndexIo[25].EnableWindow(bOn);	// Picker 4 Vac On
	m_btnIndexIo[31].EnableWindow(bOn);	// Picker 4 Vac Off
	m_btnAssyPickerIo[ 3].EnableWindow(bOn);	// Picker 4 Up
	m_btnAssyPickerIo[ 9].EnableWindow(bOn);	// Picker 4 Down
	m_btnAssyPickerIo[15].EnableWindow(bOn);	// Picker 4 Vac On
	m_btnAssyPickerIo[21].EnableWindow(bOn);	// Picker 4 Vac Off

	m_strLog.Format("[Manual Index] Show Window");
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualIndexDlg::Initial_Controls() 
{
	for (int i = 0; i <  3; i++) m_Group[i].Init_Ctrl("Arial", 11, TRUE, RGB(0xD0, 0x10, 0x10), COLOR_DEFAULT);
	for (int i = 3; i <  4; i++) m_Group[i].Init_Ctrl("Arial", 11, TRUE, RGB(0xD0, 0x10, 0xD0), COLOR_DEFAULT);
	for (int i = 4; i <  5; i++) m_Group[i].Init_Ctrl("Arial", 11, TRUE, RGB(0x00, 0x00, 0xFF), COLOR_DEFAULT);
	for (int i = 5; i <  9; i++) m_Group[i].Init_Ctrl("Arial", 11, TRUE, RGB(0x00, 0x80, 0x40), COLOR_DEFAULT);
	for (int i = 0; i <  6; i++) m_Label[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, RGB(0xB0, 0xB0, 0xB0));
	for (int i = 0; i <  6; i++) m_stcAxisPos[i].Init_Ctrl("Arial", 10, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x10, 0x10, 0x60));
	m_stcIndexPos.Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT);
	for (int i = 0; i <  2; i++) m_btnIndexR[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  2; i++) m_ledIndexPos[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i < 38; i++) m_btnIndexIo[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 22; i++) m_ledIndexIo[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i <  4; i++) m_btnVisionCMAlignX[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  3; i++) m_btnVisionCapAlignY[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  7; i++) m_btnAssyPickerX[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  7; i++) m_btnAssyPickerY[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  7; i++) m_btnAssyPickerZ[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 31; i++) m_btnAssyPickerIo[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 27; i++) m_ledAssyPickerIo[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i <  6; i++) m_stcAssyLoadCell[i].Init_Ctrl("Arial", 10, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x10, 0x10, 0x60));
}

void CManualIndexDlg::Display_Status()
{
	CString strPos;
	for (int i = 0; i < 2; i++) {
		double dPos = g_objAJinAXL.Get_Position(AX_INDEX_R + i);
		strPos.Format("%0.3lf", dPos);
		m_stcAxisPos[i].SetWindowText(strPos);
	}
	for (int i = 0; i < 4; i++) {
		double dPos = g_objAJinAXL.Get_Position(AX_VISION_CAP_ALIGN_Y + i);
		strPos.Format("%0.3lf", dPos);
		m_stcAxisPos[i+2].SetWindowText(strPos);
	}

	DX_DATA_08 *pDX08 = g_objAJinAXL.Get_pDX08();
	DX_DATA_09 *pDX09 = g_objAJinAXL.Get_pDX09();
	DX_DATA_11 *pDX11 = g_objAJinAXL.Get_pDX11();
	DY_DATA_11 *pDY11 = g_objAJinAXL.Get_pDY11();

	int nPos = g_objCommon.Get_IndexPos(2);//pDX11->iIndexPosition0 + (pDX11->iIndexPosition1 << 1);
	strPos.Format("%d", nPos + 1);
	m_stcIndexPos.SetWindowText(strPos);
	for (int i = 0; i < 2; i++) m_ledIndexPos[i].Set_On((pDX11->nValue >> i) & 1);					// Index Position

	m_ledIndexIo[0].Set_On(pDX11->iIndexLoadAlignIn);
	m_ledIndexIo[1].Set_On(pDX11->iIndexAssyAlignIn);
	m_ledIndexIo[2].Set_On(pDX11->iIndexTransAlignIn);
	m_ledIndexIo[3].Set_On(pDX11->iIndexLoadAlignOut);
	m_ledIndexIo[4].Set_On(pDX11->iIndexAssyAlignOut);
	m_ledIndexIo[5].Set_On(pDX11->iIndexTransAlignOut);
	m_ledIndexIo[6].Set_On(pDX11->iIndexLoadVacUp);
	m_ledIndexIo[7].Set_On(pDX11->iIndexAssyVacUp);
	m_ledIndexIo[8].Set_On(pDX11->iIndexLoadVacDown);
	m_ledIndexIo[9].Set_On(pDX11->iIndexAssyVacDown);

	m_ledIndexIo[10].Set_On(pDX11->iIndexLoadVac01);
	m_ledIndexIo[11].Set_On(pDX11->iIndexLoadVac02);
	m_ledIndexIo[12].Set_On(pDX11->iIndexLoadVac03);
	m_ledIndexIo[13].Set_On(pDX11->iIndexLoadVac04);
	m_ledIndexIo[14].Set_On(pDX11->iIndexLoadVac05);
	m_ledIndexIo[15].Set_On(pDX11->iIndexLoadVac06);
	m_ledIndexIo[16].Set_On(pDX11->iIndexAssyVac01);
	m_ledIndexIo[17].Set_On(pDX11->iIndexAssyVac02);
	m_ledIndexIo[18].Set_On(pDX11->iIndexAssyVac03);
	m_ledIndexIo[19].Set_On(pDX11->iIndexAssyVac04);
	m_ledIndexIo[20].Set_On(pDX11->iIndexAssyVac05);
	m_ledIndexIo[21].Set_On(pDX11->iIndexAssyVac06);	

	m_ledAssyPickerIo[ 0].Set_On(pDX08->iAssyPickerUp01);
	m_ledAssyPickerIo[ 1].Set_On(pDX08->iAssyPickerUp02);
	m_ledAssyPickerIo[ 2].Set_On(pDX08->iAssyPickerUp03);
	m_ledAssyPickerIo[ 3].Set_On(pDX08->iAssyPickerUp04);
	m_ledAssyPickerIo[ 4].Set_On(pDX08->iAssyPickerUp05);
	m_ledAssyPickerIo[ 5].Set_On(pDX08->iAssyPickerUp06);
	m_ledAssyPickerIo[ 6].Set_On(pDX08->iAssyPickerDown01);
	m_ledAssyPickerIo[ 7].Set_On(pDX08->iAssyPickerDown02);
	m_ledAssyPickerIo[ 8].Set_On(pDX08->iAssyPickerDown03);
	m_ledAssyPickerIo[ 9].Set_On(pDX08->iAssyPickerDown04);
	m_ledAssyPickerIo[10].Set_On(pDX08->iAssyPickerDown05);
	m_ledAssyPickerIo[11].Set_On(pDX08->iAssyPickerDown06);
	m_ledAssyPickerIo[12].Set_On(pDX08->iAssyPickerVac01);
	m_ledAssyPickerIo[13].Set_On(pDX08->iAssyPickerVac02);
	m_ledAssyPickerIo[14].Set_On(pDX08->iAssyPickerVac03);
	m_ledAssyPickerIo[15].Set_On(pDX08->iAssyPickerVac04);
	m_ledAssyPickerIo[16].Set_On(pDX08->iAssyPickerVac05);
	m_ledAssyPickerIo[17].Set_On(pDX08->iAssyPickerVac06);

	m_ledAssyPickerIo[18].Set_On(pDX09->iAssyPickerTilt01);
	m_ledAssyPickerIo[19].Set_On(pDX09->iAssyPickerTilt02);
	m_ledAssyPickerIo[20].Set_On(pDX09->iAssyPickerTilt03);
	m_ledAssyPickerIo[21].Set_On(pDX09->iAssyPickerTilt04);
	m_ledAssyPickerIo[22].Set_On(pDX09->iAssyPickerTilt05);
	m_ledAssyPickerIo[23].Set_On(pDX09->iAssyPickerTilt06);
	m_ledAssyPickerIo[24].Set_On(pDX09->iAssyPickerCrossTilt);

	m_ledAssyPickerIo[25].Set_On(pDX08->iAssyPickerTiltUp);
	m_ledAssyPickerIo[26].Set_On(pDX08->iAssyPickerTiltDown);
}

///////////////////////////////////////////////////////////////////////////////

void CManualIndexDlg::OnBtnIndexRClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_INDEX_R)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_INDEX_R_0;

	if (!g_objSequenceMain.Check_MainIndexCylinder()) {
		AfxMessageBox("Jig Align Out 및 Vacuum Pad Down 후 진행하세요."); return;
	}

	if (!g_objCommon.Check_Position(AX_LOAD_PICKER_Y, 0) && !g_objCommon.Check_Position(AX_LOAD_PICKER_Y, 1) && !g_objCommon.Check_Position(AX_LOAD_PICKER_Z, 0)) {
		AfxMessageBox("Load Picker Z축 Ready Up 위치 인지 확인 후 진행하세요."); return;
	}
	if (!g_objCommon.Check_Position(AX_ASSY_PICKER_X, 0) && !g_objCommon.Check_Position(AX_ASSY_PICKER_X, 1) && !g_objCommon.Check_Position(AX_ASSY_PICKER_Z, 0)) {
		AfxMessageBox("Assembly Picker Z축 Ready Up 위치 인지 확인 후 진행하세요."); return;
	}
	if (!g_objCommon.Check_Position(AX_TRANS_STAGE_X, 1) && !g_objCommon.Check_Position(AX_TRANS_STAGE_Z, 0)) {
		AfxMessageBox("Trans Stage Z축 Ready Up 위치 인지 확인 후 진행하세요."); return;
	}

	MOVE_DATA *pMoveData = g_objDataManager.Get_pMoveData();
	if (nIndex == 0) g_objAJinAXL.Move_Relative(AX_INDEX_R, pMoveData->dIndexR[0]);		// 정방향
	if (nIndex == 1) g_objAJinAXL.Move_Relative(AX_INDEX_R, -pMoveData->dIndexR[0]);	// 역방향

	m_strLog.Format("[Manual Index] Index R Click (%d)", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualIndexDlg::OnBtnIndexIoClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_INDEX_IO_0;

	if (g_objAJinAXL.Is_Done(AX_INDEX_R) == FALSE) {
		AfxMessageBox("Index R축 구동 중 입니다."); return;
	}
	
	DY_DATA_11 *pDY11 = g_objAJinAXL.Get_pDY11();

	if (nIndex == 0) pDY11->oIndexLoadAlignOut = FALSE;
	if (nIndex == 1) pDY11->oIndexAssyAlignOut = FALSE;
	if (nIndex == 2) pDY11->oIndexTransAlignOut = FALSE;
	if (nIndex == 3) pDY11->oIndexLoadAlignOut = TRUE;
	if (nIndex == 4) pDY11->oIndexAssyAlignOut = TRUE;
	if (nIndex == 5) pDY11->oIndexTransAlignOut = TRUE;

	if (nIndex == 6) pDY11->oIndexLoadVacUp = TRUE;
	if (nIndex == 7) pDY11->oIndexAssyVacUp = TRUE;
	if (nIndex == 8) pDY11->oIndexLoadVacUp = FALSE;
	if (nIndex == 9) pDY11->oIndexAssyVacUp = FALSE;

	if (nIndex == 10 || nIndex == 34) pDY11->oIndexLoadVac01 = TRUE;
	if (nIndex == 11 || nIndex == 34) pDY11->oIndexLoadVac02 = TRUE;
	if (nIndex == 12 || nIndex == 34) pDY11->oIndexLoadVac03 = TRUE;
	if (nIndex == 13 || nIndex == 34) pDY11->oIndexLoadVac04 = TRUE;
	if (nIndex == 14 || nIndex == 34) pDY11->oIndexLoadVac05 = TRUE;
	if (nIndex == 15 || nIndex == 34) pDY11->oIndexLoadVac06 = TRUE;
	if (nIndex == 16 || nIndex == 35) pDY11->oIndexLoadVac01 = FALSE;
	if (nIndex == 17 || nIndex == 35) pDY11->oIndexLoadVac02 = FALSE;
	if (nIndex == 18 || nIndex == 35) pDY11->oIndexLoadVac03 = FALSE;
	if (nIndex == 19 || nIndex == 35) pDY11->oIndexLoadVac04 = FALSE;
	if (nIndex == 20 || nIndex == 35) pDY11->oIndexLoadVac05 = FALSE;
	if (nIndex == 21 || nIndex == 35) pDY11->oIndexLoadVac06 = FALSE;

	if (nIndex == 22 || nIndex == 36) pDY11->oIndexAssyVac01 = TRUE;
	if (nIndex == 23 || nIndex == 36) pDY11->oIndexAssyVac02 = TRUE;
	if (nIndex == 24 || nIndex == 36) pDY11->oIndexAssyVac03 = TRUE;
	if (nIndex == 25 || nIndex == 36) pDY11->oIndexAssyVac04 = TRUE;
	if (nIndex == 26 || nIndex == 36) pDY11->oIndexAssyVac05 = TRUE;
	if (nIndex == 27 || nIndex == 36) pDY11->oIndexAssyVac06 = TRUE;
	if (nIndex == 28 || nIndex == 37) pDY11->oIndexAssyVac01 = FALSE;
	if (nIndex == 29 || nIndex == 37) pDY11->oIndexAssyVac02 = FALSE;
	if (nIndex == 30 || nIndex == 37) pDY11->oIndexAssyVac03 = FALSE;
	if (nIndex == 31 || nIndex == 37) pDY11->oIndexAssyVac04 = FALSE;
	if (nIndex == 32 || nIndex == 37) pDY11->oIndexAssyVac05 = FALSE;
	if (nIndex == 33 || nIndex == 37) pDY11->oIndexAssyVac06 = FALSE;

	g_objAJinAXL.Write_Output(11);

	m_strLog.Format("[Manual Index] Index IO Click (%d)", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualIndexDlg::OnBtnVisionCMAlignXClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_VISION_CM_ALIGN_X)) return;
	if (!g_objAJinAXL.Is_Home(AX_LOAD_PICKER_Y)) return;

	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_VISION_CM_X_0;

	if (!g_objCommon.Check_Position(AX_LOAD_PICKER_Y, 0) && !g_objCommon.Check_Position(AX_LOAD_PICKER_Y, 1)) {
		AfxMessageBox("Load Picker Y축 Load Stage 위치인지 확인 후 진행하세요."); return;
	}

	if (nIndex == 2 || nIndex == 3) {
		EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
		int nDir = (nIndex == 3 ? 1 : -1);
		double dMove = pEquipData->dIndexPitch * nDir;
		g_objAJinAXL.Move_Relative(AX_VISION_CM_ALIGN_X, dMove);

	} else {
		g_objCommon.Move_Position(AX_VISION_CM_ALIGN_X, nIndex);
	}

	m_strLog.Format("[Manual Index] Vision CM Align X Click (%d)", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualIndexDlg::OnBtnVisionCapAlignYClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_VISION_CAP_ALIGN_Y)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_VISION_CAP_Y_0;

	if (nIndex == 1 || nIndex == 2) {
		EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
		int nDir = (nIndex == 2 ? 1 : -1);
		double dMove = pEquipData->dIndexPitch * nDir;	// temp
		g_objAJinAXL.Move_Relative(AX_VISION_CAP_ALIGN_Y, dMove);

	} else {
		g_objCommon.Move_Position(AX_VISION_CAP_ALIGN_Y, nIndex);
	}

	m_strLog.Format("[Manual Index] Vision Cap Align Y Click (%d)", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualIndexDlg::OnBtnAssyPickerXClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_ASSY_PICKER_X)) return;
	if (!g_objAJinAXL.Is_Home(AX_ASSY_PICKER_Z)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_ASSY_PICKER_X_0;

	DX_DATA_08 *pDX08 = g_objAJinAXL.Get_pDX08();

	if (!g_objCommon.Check_Position(AX_ASSY_PICKER_Z, 0)) {
		AfxMessageBox("Assembly Picker Z축 Ready Up 위치 인지 확인 후 진행하세요."); return;
	}
	if (!g_objCommon.Get_AssyPickerUp(0)) {
		AfxMessageBox("Assembly Picker Cylinder Up 인지 확인 후 진행하세요."); return;
	}
	if (pDX08->iAssyPickerTiltDown || !pDX08->iAssyPickerTiltUp) {
		AfxMessageBox("Assembly Picker Tilt Up 인지 확인 후 진행하세요."); return;
	}

	g_objCommon.Move_Position(AX_ASSY_PICKER_X, nIndex);

	m_strLog.Format("[Manual Index] Assembly Picker X Click (%d)", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualIndexDlg::OnBtnAssyPickerYClick(UINT nID)
{
 	if (!g_objAJinAXL.Is_Home(AX_ASSY_PICKER_Y)) return;
	if (!g_objAJinAXL.Is_Home(AX_ASSY_PICKER_Z)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_ASSY_PICKER_Y_0;

	DX_DATA_08 *pDX08 = g_objAJinAXL.Get_pDX08();

	if (!g_objCommon.Check_Position(AX_ASSY_PICKER_Z, 0)) {
		AfxMessageBox("Assembly Picker Z축 Ready Up 위치 인지 확인 후 진행하세요."); return;
	}
	if (!g_objCommon.Get_AssyPickerUp(0)) {
		AfxMessageBox("Assembly Picker Cylinder Up 인지 확인 후 진행하세요."); return;
	}
	if (pDX08->iAssyPickerTiltDown || !pDX08->iAssyPickerTiltUp) {
		AfxMessageBox("Assembly Picker Tilt Up 인지 확인 후 진행하세요."); return;
	}
	g_objCommon.Move_Position(AX_ASSY_PICKER_Y, nIndex);

	m_strLog.Format("[Manual Index] Assembly Picker Y Click (%d)", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualIndexDlg::OnBtnAssyPickerZClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_ASSY_PICKER_Z)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_ASSY_PICKER_Z_0;

	MOVE_DATA *pMoveData = g_objDataManager.Get_pMoveData();
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	if (nIndex == 1 && !g_objCommon.Check_Position(AX_ASSY_PICKER_X, 0) && !g_objCommon.Check_Position(AX_ASSY_PICKER_Y, 0)) {
		AfxMessageBox("Assembly Picker X축, Y축 Cap Buffer 위치 인지 확인 후 진행하세요."); return;
	}
	if (nIndex == 2 && !g_objCommon.Check_Position(AX_ASSY_PICKER_X, 1) && !g_objCommon.Check_Position(AX_ASSY_PICKER_Y, 1)) {
		AfxMessageBox("Assembly Picker X축, Y축 Inspect 위치 인지 확인 후 진행하세요."); return;
	}
	if ((nIndex == 3 || nIndex == 4 || nIndex == 5)) {
		int nPos = g_objCommon.Get_IndexPos(2);
		if (!g_objCommon.Check_Position(AX_ASSY_PICKER_X, 2+nPos) && !g_objCommon.Check_Position(AX_ASSY_PICKER_Y, 2+nPos)) {
			AfxMessageBox("Assembly Picker X축, Y축 Index 위치 인지 확인 후 진행하세요."); return;
		}
	}
	if (nIndex == 6) {
		if (!g_objCommon.Check_Position(AX_ASSY_PICKER_X, 0) && !g_objCommon.Check_Position(AX_ASSY_PICKER_Y, 0)) {
			AfxMessageBox("Assembly Picker X축, Y축 Cap Buffer 위치 인지 확인 후 진행하세요."); return;
		}

		double dCur = g_objAJinAXL.Get_Position(AX_CAP_BUFFER_STAGE_Y);
		double dPos1, dPos2;

		dPos1 = pMoveData->dCapBufferStageY[3];
		dPos2 = pMoveData->dCapBufferStageY[3] + ( pEquipData->dIndexPitch * 5);
		if (dPos1 > dCur || dPos2 < dCur) {
			AfxMessageBox("Cap Buffer Y축이 Load Cell 위치가 아닙니다. 확인 후 진행하세요."); return;
		}
	}

	if (nIndex == 4 || nIndex == 6) g_objAJinAXL.Move_AbsSlow(AX_ASSY_PICKER_Z, pMoveData->dAssyPickerZ[nIndex], 0.5);	// Slow
	else g_objCommon.Move_Position(AX_ASSY_PICKER_Z, nIndex);

	m_strLog.Format("[Manual Index] Assembly Picker Z Click (%d)", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualIndexDlg::OnBtnAssyPickerIoClick(UINT nID)
{
 	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_ASSY_PICKER_IO_0;

//	if (nIndex == 25) nIndex = 30;
	DY_DATA_08 *pDY08 = g_objAJinAXL.Get_pDY08();
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	// Up
	if (nIndex == 0 || nIndex == 26) pDY08->oAssyPickerDown01 = FALSE;
	if (nIndex == 1 || nIndex == 26) pDY08->oAssyPickerDown02 = FALSE;
	if (nIndex == 2 || nIndex == 26) pDY08->oAssyPickerDown03 = FALSE;
	if (nIndex == 3 || nIndex == 26) pDY08->oAssyPickerDown04 = FALSE;
	if (nIndex == 4 || nIndex == 26) pDY08->oAssyPickerDown05 = FALSE;
	if (nIndex == 5 || nIndex == 26) pDY08->oAssyPickerDown06 = FALSE;

	// Down
	if (nIndex ==  6 || nIndex == 27) pDY08->oAssyPickerDown01 = TRUE;
	if (nIndex ==  7 || nIndex == 27) pDY08->oAssyPickerDown02 = TRUE;
	if (nIndex ==  8 || nIndex == 27) pDY08->oAssyPickerDown03 = TRUE;
	if (gData.nPickerUseCnt > 3) { if (nIndex ==  9 || nIndex == 27) pDY08->oAssyPickerDown04 = TRUE; }
	if (gData.nPickerUseCnt > 4) { if (nIndex == 10 || nIndex == 27) pDY08->oAssyPickerDown05 = TRUE; }
	if (gData.nPickerUseCnt > 5) { if (nIndex == 11 || nIndex == 27) pDY08->oAssyPickerDown06 = TRUE; }

	if (nIndex == 12 || nIndex == 28) { pDY08->oAssyPickerVac01 = TRUE; pDY08->oAssyPickerAir01 = FALSE; }
	if (nIndex == 13 || nIndex == 28) { pDY08->oAssyPickerVac02 = TRUE; pDY08->oAssyPickerAir02 = FALSE; }
	if (nIndex == 14 || nIndex == 28) { pDY08->oAssyPickerVac03 = TRUE; pDY08->oAssyPickerAir03 = FALSE; }
	if (nIndex == 15 || nIndex == 28) { pDY08->oAssyPickerVac04 = TRUE; pDY08->oAssyPickerAir04 = FALSE; }
	if (nIndex == 16 || nIndex == 28) { pDY08->oAssyPickerVac05 = TRUE; pDY08->oAssyPickerAir05 = FALSE; }
	if (nIndex == 17 || nIndex == 28) { pDY08->oAssyPickerVac06 = TRUE; pDY08->oAssyPickerAir06 = FALSE; }

	if (nIndex == 18 || nIndex == 29) { pDY08->oAssyPickerVac01 = FALSE; pDY08->oAssyPickerAir01 = TRUE; }
	if (nIndex == 19 || nIndex == 29) { pDY08->oAssyPickerVac02 = FALSE; pDY08->oAssyPickerAir02 = TRUE; }
	if (nIndex == 20 || nIndex == 29) { pDY08->oAssyPickerVac03 = FALSE; pDY08->oAssyPickerAir03 = TRUE; }
	if (nIndex == 21 || nIndex == 29) { pDY08->oAssyPickerVac04 = FALSE; pDY08->oAssyPickerAir04 = TRUE; }
	if (nIndex == 22 || nIndex == 29) { pDY08->oAssyPickerVac05 = FALSE; pDY08->oAssyPickerAir05 = TRUE; }
	if (nIndex == 23 || nIndex == 29) { pDY08->oAssyPickerVac06 = FALSE; pDY08->oAssyPickerAir06 = TRUE; }

	if ((nIndex > 17 && nIndex < 24) || nIndex == 29) { g_objAJinAXL.Write_Output(8); Sleep(100); }

	if (nIndex == 18 || nIndex == 29) { pDY08->oAssyPickerAir01 = FALSE; }
	if (nIndex == 19 || nIndex == 29) { pDY08->oAssyPickerAir02 = FALSE; }
	if (nIndex == 20 || nIndex == 29) { pDY08->oAssyPickerAir03 = FALSE; }
	if (nIndex == 21 || nIndex == 29) { pDY08->oAssyPickerAir04 = FALSE; }
	if (nIndex == 22 || nIndex == 29) { pDY08->oAssyPickerAir05 = FALSE; }
	if (nIndex == 23 || nIndex == 29) { pDY08->oAssyPickerAir06 = FALSE; }

	if (nIndex == 24) { pDY08->oAssyPickerTiltDown = FALSE; }
	if (nIndex == 25) { pDY08->oAssyPickerTiltDown = TRUE; }

	g_objAJinAXL.Write_Output(8);

	if (nIndex == 30) {
		if (!pDY08->oAssyPickerDown01 && !pDY08->oAssyPickerDown02 && !pDY08->oAssyPickerDown03 &&
			!pDY08->oAssyPickerDown04 && !pDY08->oAssyPickerDown05 && !pDY08->oAssyPickerDown06)
		{
			AfxMessageBox("Assembly Picker가 모두 Up 상태 입니다. Picker 확인하여 주십시오."); return;
		}
		g_objLoadCell.Get_Weight(ASSY_LOAD_CELL);

		int nStart = GetTickCount();
		while(!gData.bLoadCellComplete) {
			if (GetTickCount() - nStart > 5000) {
				AfxMessageBox("Load Cell에서 값을 얻어오지 못했습니다. Load Cell 확인하여 주십시오."); 
				break;
			}
			theApp.uSleep(10);
		}

		CString strMsg;
		int	nNo = 0;

		if (pDY08->oAssyPickerDown01) { nNo = 1; }
		if (pDY08->oAssyPickerDown02) { nNo = 2; }
		if (pDY08->oAssyPickerDown03) { nNo = 3; }
		if (pDY08->oAssyPickerDown04) { nNo = 4; }
		if (pDY08->oAssyPickerDown05) { nNo = 5; }
		if (pDY08->oAssyPickerDown06) { nNo = 6; }

		if (nNo == 0) { AfxMessageBox("Down된 Assembly Picker가 없습니다. Picker 확인하여 주십시오."); return; }

// 		gData.dAssyLoadCell[nNo-1] = gData.dLoadCell[0];
		strMsg.Format("Assy Picker %d번 Load Cell 측정값 : %0.3lf", nNo, gData.dLoadCell[0]);
		AfxMessageBox(strMsg);
// 
// 		strMsg.Format("%0.3lf", gData.dAssyLoadCell[nNo-1]);
// 		m_stcAssyLoadCell[nNo-1].SetWindowText(strMsg);
// 
// 		CIniFileCS INI(gsCurrentDir + "\\System\\EquipData.ini");
// 		if (!INI.Check_File()) { AfxMessageBox("EquipData.ini File Not Found!!!"); return; }
// 
// 		CString strKey;
// 		double	dData;
// 		strKey.Format("%d", nNo-1); dData = gData.dAssyLoadCell[nNo-1]; INI.Set_Double("ASSY_LOAD_CELL", strKey, dData);
	}

	m_strLog.Format("[Manual Index] Assembly Picker IO Click (%d)", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

///////////////////////////////////////////////////////////////////////////////
