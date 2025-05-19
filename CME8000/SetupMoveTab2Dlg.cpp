// SetupMoveTab2Dlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CME8000.h"
#include "SetupMoveTab2Dlg.h"
#include "afxdialogex.h"

#include "LogFile.h"
#include "DataManager.h"
#include "Common.h"
#include "MESInterface.h"

#include "AJinDefine.h"

// CSetupMoveTab2Dlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSetupMoveTab2Dlg, CDialogEx)

CSetupMoveTab2Dlg::CSetupMoveTab2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetupMoveTab2Dlg::IDD, pParent)
{
}

CSetupMoveTab2Dlg::~CSetupMoveTab2Dlg()
{
}

void CSetupMoveTab2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for (int i = 0; i < 14; i++) DDX_Control(pDX, IDC_GROUP_0 + i, m_Group[i]);
	for (int i = 0; i < 62; i++) DDX_Control(pDX, IDC_LABEL_0 + i, m_Label[i]);
	for (int i = 0; i <  1; i++) DDX_Control(pDX, IDC_STC_INDEX_R_0 + i				, m_stcIndexR[i]);
	for (int i = 0; i <  2; i++) DDX_Control(pDX, IDC_STC_VISION_CM_ALIGN_X_0 + i	, m_stcVisionCMAlignX[i]);
	for (int i = 0; i <  3; i++) DDX_Control(pDX, IDC_STC_CAP_STAGE1_X_0 + i		, m_stcCapStage1X[i]);
	for (int i = 0; i <  4; i++) DDX_Control(pDX, IDC_STC_CAP_STAGE1_Z_0 + i		, m_stcCapStage1Z[i]);
	for (int i = 0; i <  3; i++) DDX_Control(pDX, IDC_STC_CAP_STAGE2_X_0 + i		, m_stcCapStage2X[i]);
	for (int i = 0; i <  4; i++) DDX_Control(pDX, IDC_STC_CAP_STAGE2_Z_0 + i		, m_stcCapStage2Z[i]);
	for (int i = 0; i < 13; i++) DDX_Control(pDX, IDC_STC_CAP_PICKER_Y_0 + i		, m_stcCapPickerY[i]);
	for (int i = 0; i <  4; i++) DDX_Control(pDX, IDC_STC_CAP_PICKER_Z_0 + i		, m_stcCapPickerZ[i]);
	for (int i = 0; i <  2; i++) DDX_Control(pDX, IDC_STC_CAP_PICKER_P_0 + i		, m_stcCapPickerP[i]);
	for (int i = 0; i <  4; i++) DDX_Control(pDX, IDC_STC_CAP_BUFFER_Y_0  + i		, m_stcCapBufferY[i]);
	for (int i = 0; i <  1; i++) DDX_Control(pDX, IDC_STC_VISION_CAP_ALIGN_Y_0 + i	, m_stcVisionCapAlignY[i]);
	for (int i = 0; i <  7; i++) DDX_Control(pDX, IDC_STC_ASSY_PICKER_X_0 + i		, m_stcAssyPickerX[i]);
	for (int i = 0; i <  7; i++) DDX_Control(pDX, IDC_STC_ASSY_PICKER_Y_0 + i		, m_stcAssyPickerY[i]);
	for (int i = 0; i <  7; i++) DDX_Control(pDX, IDC_STC_ASSY_PICKER_Z_0 + i		, m_stcAssyPickerZ[i]);
}

BEGIN_MESSAGE_MAP(CSetupMoveTab2Dlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_INDEX_R_0				, IDC_STC_INDEX_R_0				, OnStcIndexRlick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_VISION_CM_ALIGN_X_0	, IDC_STC_VISION_CM_ALIGN_X_1	, OnStcVisionCMAlignXClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_CAP_STAGE1_X_0		, IDC_STC_CAP_STAGE1_X_2		, OnStcCapStage1XClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_CAP_STAGE1_Z_0		, IDC_STC_CAP_STAGE1_Z_3		, OnStcCapStage1ZClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_CAP_STAGE2_X_0		, IDC_STC_CAP_STAGE2_X_2		, OnStcCapStage2XClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_CAP_STAGE2_Z_0		, IDC_STC_CAP_STAGE2_Z_3		, OnStcCapStage2ZClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_CAP_PICKER_Y_0		, IDC_STC_CAP_PICKER_Y_12		, OnStcCapPickerYClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_CAP_PICKER_Z_0		, IDC_STC_CAP_PICKER_Z_3		, OnStcCapPickerZClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_CAP_PICKER_P_0		, IDC_STC_CAP_PICKER_P_1		, OnStcCapPickerPClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_CAP_BUFFER_Y_0		, IDC_STC_CAP_BUFFER_Y_3		, OnStcCapBufferYClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_VISION_CAP_ALIGN_Y_0	, IDC_STC_VISION_CAP_ALIGN_Y_0	, OnStcVisionCapAlignYClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_ASSY_PICKER_X_0		, IDC_STC_ASSY_PICKER_X_6		, OnStcAssyPickerXClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_ASSY_PICKER_Y_0		, IDC_STC_ASSY_PICKER_Y_6		, OnStcAssyPickerYClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_ASSY_PICKER_Z_0		, IDC_STC_ASSY_PICKER_Z_6		, OnStcAssyPickerZClick)
END_MESSAGE_MAP()

// CSetupMoveTab2Dlg 메시지 처리기입니다.

BOOL CSetupMoveTab2Dlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(NULL, 5, 65, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	Initial_Controls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CSetupMoveTab2Dlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CSetupMoveTab2Dlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (!bShow) return;

	Display_MoveData();

	g_objLogFile.Save_HandlerLog("[Setup Move] Show Window - Tab2");
}

void CSetupMoveTab2Dlg::OnStcIndexRlick(UINT nID)
{
	int ID = nID - IDC_STC_INDEX_R_0;

	CString strOld, strNew;
	m_stcIndexR[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_INDEX_R) != IDOK) return;

	m_stcIndexR[ID].SetWindowText(strNew);
}

void CSetupMoveTab2Dlg::OnStcVisionCMAlignXClick(UINT nID)
{
	int ID = nID - IDC_STC_VISION_CM_ALIGN_X_0;

	CString strOld, strNew;
	m_stcVisionCMAlignX[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_VISION_CM_ALIGN_X) != IDOK) return;

	m_stcVisionCMAlignX[ID].SetWindowText(strNew);
}

void CSetupMoveTab2Dlg::OnStcCapStage1XClick(UINT nID)
{
	int ID = nID - IDC_STC_CAP_STAGE1_X_0;

	CString strOld, strNew;
	m_stcCapStage1X[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_CAP_STAGE1_X) != IDOK) return;

	m_stcCapStage1X[ID].SetWindowText(strNew);
}

void CSetupMoveTab2Dlg::OnStcCapStage1ZClick(UINT nID)			
{
	int ID = nID - IDC_STC_CAP_STAGE1_Z_0;

	CString strOld, strNew;
	m_stcCapStage1Z[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_CAP_STAGE1_Z) != IDOK) return;

	m_stcCapStage1Z[ID].SetWindowText(strNew);
}

void CSetupMoveTab2Dlg::OnStcCapStage2XClick(UINT nID)			
{
	int ID = nID - IDC_STC_CAP_STAGE2_X_0;

	CString strOld, strNew;
	m_stcCapStage2X[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_CAP_STAGE2_X) != IDOK) return;

	m_stcCapStage2X[ID].SetWindowText(strNew);
}

void CSetupMoveTab2Dlg::OnStcCapStage2ZClick(UINT nID)
{
	int ID = nID - IDC_STC_CAP_STAGE2_Z_0;

	CString strOld, strNew;
	m_stcCapStage2Z[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_CAP_STAGE2_Z) != IDOK) return;

	m_stcCapStage2Z[ID].SetWindowText(strNew);
}

void CSetupMoveTab2Dlg::OnStcCapPickerYClick(UINT nID)
{
	int ID = nID - IDC_STC_CAP_PICKER_Y_0;

	CString strOld, strNew;
	m_stcCapPickerY[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_CAP_PICKER_Y) != IDOK) return;

	m_stcCapPickerY[ID].SetWindowText(strNew);
}

void CSetupMoveTab2Dlg::OnStcCapPickerZClick(UINT nID)
{
	int ID = nID - IDC_STC_CAP_PICKER_Z_0;

	CString strOld, strNew;
	m_stcCapPickerZ[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_CAP_PICKER_Z) != IDOK) return;

	m_stcCapPickerZ[ID].SetWindowText(strNew);
}

void CSetupMoveTab2Dlg::OnStcCapPickerPClick(UINT nID)
{
	int ID = nID - IDC_STC_CAP_PICKER_P_0;

	CString strOld, strNew;
	m_stcCapPickerP[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_CAP_PICKER_P) != IDOK) return;

	m_stcCapPickerP[ID].SetWindowText(strNew);
}

void CSetupMoveTab2Dlg::OnStcCapBufferYClick(UINT nID)			
{
	int ID = nID - IDC_STC_CAP_BUFFER_Y_0;

	CString strOld, strNew;
	m_stcCapBufferY[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_CAP_BUFFER_STAGE_Y) != IDOK) return;

	m_stcCapBufferY[ID].SetWindowText(strNew);
}

void CSetupMoveTab2Dlg::OnStcVisionCapAlignYClick(UINT nID)			
{
	int ID = nID - IDC_STC_VISION_CAP_ALIGN_Y_0;

	CString strOld, strNew;
	m_stcVisionCapAlignY[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_VISION_CAP_ALIGN_Y) != IDOK) return;

	m_stcVisionCapAlignY[ID].SetWindowText(strNew);
}

void CSetupMoveTab2Dlg::OnStcAssyPickerXClick(UINT nID)			
{
	int ID = nID - IDC_STC_ASSY_PICKER_X_0;

	CString strOld, strNew;
	m_stcAssyPickerX[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_ASSY_PICKER_X) != IDOK) return;

	m_stcAssyPickerX[ID].SetWindowText(strNew);
}

void CSetupMoveTab2Dlg::OnStcAssyPickerYClick(UINT nID)			
{
	int ID = nID - IDC_STC_ASSY_PICKER_Y_0;

	CString strOld, strNew;
	m_stcAssyPickerY[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_ASSY_PICKER_Y) != IDOK) return;

	m_stcAssyPickerY[ID].SetWindowText(strNew);
}

void CSetupMoveTab2Dlg::OnStcAssyPickerZClick(UINT nID)			
{
	int ID = nID - IDC_STC_ASSY_PICKER_Z_0;

	CString strOld, strNew;
	m_stcAssyPickerZ[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_ASSY_PICKER_Z) != IDOK) return;

	m_stcAssyPickerZ[ID].SetWindowText(strNew);
}

///////////////////////////////////////////////////////////////////////////////
// User Functions

void CSetupMoveTab2Dlg::Initial_Controls() 
{
	for (int i = 0; i < 14; i++) m_Group[i].Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT);
	for (int i = 0; i < 62; i++) m_Label[i].Init_Ctrl("바탕", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x60, 0x60, 0x60));
	for (int i = 0; i < 1; i++) m_stcIndexR[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xA0, 0xF0, 0xC0));
	for (int i = 0; i < 2; i++) m_stcVisionCMAlignX[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xEF, 0xD5));
	for (int i = 0; i < 3; i++) m_stcCapStage1X[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xE0, 0xE0));
	for (int i = 0; i < 4; i++) m_stcCapStage1Z[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xE0, 0xE0));
	for (int i = 0; i < 3; i++) m_stcCapStage2X[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xC0, 0xFF, 0xF0));
	for (int i = 0; i < 4; i++) m_stcCapStage2Z[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xC0, 0xFF, 0xF0));
	for (int i = 0; i < 13; i++) m_stcCapPickerY[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xFF, 0xA0));
	for (int i = 0; i < 4; i++) m_stcCapPickerZ[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xFF, 0xA0));
	for (int i = 0; i < 2; i++) m_stcCapPickerP[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xFF, 0xA0));
	for (int i = 0; i < 4; i++) m_stcCapBufferY[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0x40, 0xC0, 0xF0));
	for (int i = 0; i < 1; i++) m_stcVisionCapAlignY[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xEF, 0xD5));
	for (int i = 0; i < 7; i++) m_stcAssyPickerX[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xB0, 0xC0, 0xFF));
	for (int i = 0; i < 7; i++) m_stcAssyPickerY[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xB0, 0xC0, 0xFF));
	for (int i = 0; i < 7; i++) m_stcAssyPickerZ[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xB0, 0xC0, 0xFF));
}

void CSetupMoveTab2Dlg::Display_MoveData()
{
	MOVE_DATA *pMoveData = g_objDataManager.Get_pMoveData();
	CString strData;

	for (int i = 0; i < 1; i++) { strData.Format("%0.3lf", pMoveData->dIndexR[i]);			m_stcIndexR[i].SetWindowText(strData); }
	for (int i = 0; i < 2; i++) { strData.Format("%0.3lf", pMoveData->dVisionCMAlignX[i]);	m_stcVisionCMAlignX[i].SetWindowText(strData); }
	for (int i = 0; i < 3; i++) { strData.Format("%0.3lf", pMoveData->dCapStage1X[i]);		m_stcCapStage1X[i].SetWindowText(strData); }
	for (int i = 0; i < 4; i++) { strData.Format("%0.3lf", pMoveData->dCapStage1Z[i]);		m_stcCapStage1Z[i].SetWindowText(strData); }
	for (int i = 0; i < 3; i++) { strData.Format("%0.3lf", pMoveData->dCapStage2X[i]);		m_stcCapStage2X[i].SetWindowText(strData); }
	for (int i = 0; i < 4; i++) { strData.Format("%0.3lf", pMoveData->dCapStage2Z[i]);		m_stcCapStage2Z[i].SetWindowText(strData); }
	for (int i = 0; i < 13; i++) { strData.Format("%0.3lf", pMoveData->dCapPickerY[i]);		m_stcCapPickerY[i].SetWindowText(strData); }
	for (int i = 0; i < 4; i++) { strData.Format("%0.3lf", pMoveData->dCapPickerZ[i]);		m_stcCapPickerZ[i].SetWindowText(strData); }
	for (int i = 0; i < 2; i++) { strData.Format("%0.3lf", pMoveData->dCapPickerP[i]);		m_stcCapPickerP[i].SetWindowText(strData); }
	for (int i = 0; i < 4; i++) { strData.Format("%0.3lf", pMoveData->dCapBufferStageY[i]);	m_stcCapBufferY[i].SetWindowText(strData); }
	for (int i = 0; i < 1; i++) { strData.Format("%0.3lf", pMoveData->dVisionCapAlignY[i]);	m_stcVisionCapAlignY[i].SetWindowText(strData); }
	for (int i = 0; i < 7; i++) { strData.Format("%0.3lf", pMoveData->dAssyPickerX[i]);		m_stcAssyPickerX[i].SetWindowText(strData); }
	for (int i = 0; i < 7; i++) { strData.Format("%0.3lf", pMoveData->dAssyPickerY[i]);		m_stcAssyPickerY[i].SetWindowText(strData); }
	for (int i = 0; i < 7; i++) { strData.Format("%0.3lf", pMoveData->dAssyPickerZ[i]);		m_stcAssyPickerZ[i].SetWindowText(strData); }
}

void CSetupMoveTab2Dlg::Save_MoveData()
{
 	CIniFileCS INI(gsCurrentDir + "\\System\\MoveData.ini");
	if (!INI.Check_File()) { AfxMessageBox("MoveData.ini File Not Found!!!"); return; }

	g_objCommon.Backup_File(gsCurrentDir + "\\System", "MoveData");

	CString strKey, strData;
	double dData;

	for (int i = 0; i <  1; i++) { strKey.Format("%02d", i);	m_stcIndexR[i].GetWindowText(strData);			dData = atof(strData);	INI.Set_Double("11_INDEX_R", strKey, dData, "%0.3lf"); }
	for (int i = 0; i <  2; i++) { strKey.Format("%02d", i);	m_stcVisionCMAlignX[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("12_VISION_CM_ALIGN_X", strKey, dData, "%0.3lf"); }
	for (int i = 0; i <  3; i++) { strKey.Format("%02d", i);	m_stcCapStage1X[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("13_CAP_STAGE1_X", strKey, dData, "%0.3lf"); }
	for (int i = 0; i <  4; i++) { strKey.Format("%02d", i);	m_stcCapStage1Z[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("14_CAP_STAGE1_Z", strKey, dData, "%0.3lf"); }
	for (int i = 0; i <  3; i++) { strKey.Format("%02d", i);	m_stcCapStage2X[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("15_CAP_STAGE2_X", strKey, dData, "%0.3lf"); }
	for (int i = 0; i <  4; i++) { strKey.Format("%02d", i);	m_stcCapStage2Z[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("16_CAP_STAGE2_Z", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 13; i++) { strKey.Format("%02d", i);	m_stcCapPickerY[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("17_CAP_PICKER_Y", strKey, dData, "%0.3lf"); }
	for (int i = 0; i <  4; i++) { strKey.Format("%02d", i);	m_stcCapPickerZ[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("18_CAP_PICKER_Z", strKey, dData, "%0.3lf"); }
	for (int i = 0; i <  2; i++) { strKey.Format("%02d", i);	m_stcCapPickerP[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("19_CAP_PICKER_P", strKey, dData, "%0.3lf"); }
	for (int i = 0; i <  4; i++) { strKey.Format("%02d", i);	m_stcCapBufferY[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("20_CAP_BUFFER_STAGE_Y", strKey, dData, "%0.3lf"); }
	for (int i = 0; i <  1; i++) { strKey.Format("%02d", i);	m_stcVisionCapAlignY[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("21_VISION_CAP_ALIGN_Y", strKey, dData, "%0.3lf"); }
	for (int i = 0; i <  7; i++) { strKey.Format("%02d", i);	m_stcAssyPickerX[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("22_ASSEMBLY_PICKER_X", strKey, dData, "%0.3lf"); }
	for (int i = 0; i <  7; i++) { strKey.Format("%02d", i);	m_stcAssyPickerY[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("23_ASSEMBLY_PICKER_Y", strKey, dData, "%0.3lf"); }
	
	for (int i = 0; i <  7; i++) {
		strKey.Format("%02d", i);	m_stcAssyPickerZ[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("24_ASSEMBLY_PICKER_Z", strKey, dData, "%0.3lf");

		// RMS 항목 저장
		if (i == 3) g_objMES.Save_AviRmsData("Cap-Assembly Picker Z Assembly Down Position", strData);
		if (i == 4) g_objMES.Save_AviRmsData("Cap-Assembly Picker Z Cap Press Position", strData);
		if (i == 4) g_objMES.Save_AviRmsData("Cap Attach Z-Pos", strData);
	}

	g_objLogFile.Save_HandlerLog("[Setup - MoveTab Dialog 2] save");

	Cancel_MoveData();
}

void CSetupMoveTab2Dlg::Cancel_MoveData()
{
	g_objDataManager.Read_MoveData();
	Display_MoveData();
}

///////////////////////////////////////////////////////////////////////////////
