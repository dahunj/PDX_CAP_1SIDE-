// SetupMoveTab3Dlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CME8000.h"
#include "SetupMoveTab3Dlg.h"
#include "afxdialogex.h"

#include "LogFile.h"
#include "DataManager.h"
#include "Common.h"
#include "MESInterface.h"

#include "AJinDefine.h"

// CSetupMoveTab3Dlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSetupMoveTab3Dlg, CDialogEx)

CSetupMoveTab3Dlg::CSetupMoveTab3Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetupMoveTab3Dlg::IDD, pParent)
{
}

CSetupMoveTab3Dlg::~CSetupMoveTab3Dlg()
{
}

void CSetupMoveTab3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for (int i = 0; i < 10; i++) DDX_Control(pDX, IDC_GROUP_0 + i, m_Group[i]);
	for (int i = 0; i < 35; i++) DDX_Control(pDX, IDC_LABEL_0 + i, m_Label[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_STC_TRANS_STAGE_X_0 + i, m_stcTransStageX[i]);
	for (int i = 0; i < 3; i++) DDX_Control(pDX, IDC_STC_TRANS_STAGE_Z_0 + i, m_stcTransStageZ[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_STC_TRANS_STAGE_T_0 + i, m_stcTransStageT[i]);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_STC_UNLOAD_PICKER_X_0 + i, m_stcUnloadPickerX[i]);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_STC_UNLOAD_PICKER_Z_0 + i, m_stcUnloadPickerZ[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_STC_UNLOAD_PICKER_P_0 + i, m_stcUnloadPickerP[i]);
	for (int i = 0; i < 3; i++) DDX_Control(pDX, IDC_STC_UNLOAD_STAGE1_Y_0 + i , m_stcUnloadStage1Y[i]);
	for (int i = 0; i < 6; i++) DDX_Control(pDX, IDC_STC_UNLOAD_STAGE1_Z_0 + i , m_stcUnloadStage1Z[i]);
	for (int i = 0; i < 3; i++) DDX_Control(pDX, IDC_STC_UNLOAD_STAGE2_Y_0 + i , m_stcUnloadStage2Y[i]);
	for (int i = 0; i < 6; i++) DDX_Control(pDX, IDC_STC_UNLOAD_STAGE2_Z_0 + i , m_stcUnloadStage2Z[i]);
}

BEGIN_MESSAGE_MAP(CSetupMoveTab3Dlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_TRANS_STAGE_X_0, IDC_STC_TRANS_STAGE_X_1, OnStcTransStageXClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_TRANS_STAGE_Z_0, IDC_STC_TRANS_STAGE_Z_2, OnStcTransStageZClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_TRANS_STAGE_T_0, IDC_STC_TRANS_STAGE_T_1, OnStcTransStageTClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_UNLOAD_PICKER_X_0, IDC_STC_UNLOAD_PICKER_X_3, OnStcUnloadPickerXClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_UNLOAD_PICKER_Z_0, IDC_STC_UNLOAD_PICKER_Z_3, OnStcUnloadPickerZClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_UNLOAD_PICKER_P_0, IDC_STC_UNLOAD_PICKER_P_1, OnStcUnloadPickerPClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_UNLOAD_STAGE1_Y_0, IDC_STC_UNLOAD_STAGE1_Y_2, OnStcUnloadStage1YClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_UNLOAD_STAGE1_Z_0, IDC_STC_UNLOAD_STAGE1_Z_5, OnStcUnloadStage1ZClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_UNLOAD_STAGE2_Y_0, IDC_STC_UNLOAD_STAGE2_Y_2, OnStcUnloadStage2YClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_UNLOAD_STAGE2_Z_0, IDC_STC_UNLOAD_STAGE2_Z_5, OnStcUnloadStage2ZClick)
END_MESSAGE_MAP()

// CSetupMoveTab3Dlg 메시지 처리기입니다.

// CSetupMoveTab3Dlg 메시지 처리기입니다.

BOOL CSetupMoveTab3Dlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(NULL, 5, 65, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	Initial_Controls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CSetupMoveTab3Dlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CSetupMoveTab3Dlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (!bShow) return;

	Display_MoveData();

	g_objLogFile.Save_HandlerLog("[Setup Move] Show Window - Tab3");
}

void CSetupMoveTab3Dlg::OnStcTransStageXClick(UINT nID)
{
	int ID = nID - IDC_STC_TRANS_STAGE_X_0;

	CString strOld, strNew;
	m_stcTransStageX[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_TRANS_STAGE_X) != IDOK) return;

	m_stcTransStageX[ID].SetWindowText(strNew);
}

void CSetupMoveTab3Dlg::OnStcTransStageZClick(UINT nID)
{
	int ID = nID - IDC_STC_TRANS_STAGE_Z_0;

	CString strOld, strNew;
	m_stcTransStageZ[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_TRANS_STAGE_Z) != IDOK) return;

	m_stcTransStageZ[ID].SetWindowText(strNew);
}

void CSetupMoveTab3Dlg::OnStcTransStageTClick(UINT nID)
{
	int ID = nID - IDC_STC_TRANS_STAGE_T_0;

	CString strOld, strNew;
	m_stcTransStageT[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_TRANS_STAGE_T) != IDOK) return;

	m_stcTransStageT[ID].SetWindowText(strNew);
}

void CSetupMoveTab3Dlg::OnStcUnloadPickerXClick(UINT nID)
{
	int ID = nID - IDC_STC_UNLOAD_PICKER_X_0;

	CString strOld, strNew;
	m_stcUnloadPickerX[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_UNLOAD_PICKER_X) != IDOK) return;

	m_stcUnloadPickerX[ID].SetWindowText(strNew);
}

void CSetupMoveTab3Dlg::OnStcUnloadPickerZClick(UINT nID)
{
	int ID = nID - IDC_STC_UNLOAD_PICKER_Z_0;

	CString strOld, strNew;
	m_stcUnloadPickerZ[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_UNLOAD_PICKER_Z) != IDOK) return;

	m_stcUnloadPickerZ[ID].SetWindowText(strNew);
}

void CSetupMoveTab3Dlg::OnStcUnloadPickerPClick(UINT nID)
{
	int ID = nID - IDC_STC_UNLOAD_PICKER_P_0;

	CString strOld, strNew;
	m_stcUnloadPickerP[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_UNLOAD_PICKER_P) != IDOK) return;

	m_stcUnloadPickerP[ID].SetWindowText(strNew);
}

void CSetupMoveTab3Dlg::OnStcUnloadStage1YClick(UINT nID)
{
	int ID = nID - IDC_STC_UNLOAD_STAGE1_Y_0;

	CString strOld, strNew;
	m_stcUnloadStage1Y[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_UNLOAD_STAGE1_Y) != IDOK) return;

	m_stcUnloadStage1Y[ID].SetWindowText(strNew);
}

void CSetupMoveTab3Dlg::OnStcUnloadStage1ZClick(UINT nID)			
{
	int ID = nID - IDC_STC_UNLOAD_STAGE1_Z_0;

	CString strOld, strNew;
	m_stcUnloadStage1Z[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_UNLOAD_STAGE1_Z) != IDOK) return;

	m_stcUnloadStage1Z[ID].SetWindowText(strNew);
}

void CSetupMoveTab3Dlg::OnStcUnloadStage2YClick(UINT nID)			
{
	int ID = nID - IDC_STC_UNLOAD_STAGE2_Y_0;

	CString strOld, strNew;
	m_stcUnloadStage2Y[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_UNLOAD_STAGE2_Y) != IDOK) return;

	m_stcUnloadStage2Y[ID].SetWindowText(strNew);
}

void CSetupMoveTab3Dlg::OnStcUnloadStage2ZClick(UINT nID)			
{
	int ID = nID - IDC_STC_UNLOAD_STAGE2_Z_0;

	CString strOld, strNew;
	m_stcUnloadStage2Z[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_UNLOAD_STAGE2_Z) != IDOK) return;

	m_stcUnloadStage2Z[ID].SetWindowText(strNew);
}

///////////////////////////////////////////////////////////////////////////////
// User Functions

void CSetupMoveTab3Dlg::Initial_Controls() 
{
	for (int i = 0; i < 10; i++) m_Group[i].Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT);
	for (int i = 0; i < 35; i++) m_Label[i].Init_Ctrl("바탕", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x60, 0x60, 0x60));
	for (int i = 0; i < 2; i++) m_stcTransStageX[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xC0, 0xFF, 0xF0));
	for (int i = 0; i < 3; i++) m_stcTransStageZ[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xC0, 0xFF, 0xF0));
	for (int i = 0; i < 2; i++) m_stcTransStageT[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xC0, 0xFF, 0xF0));
	for (int i = 0; i < 4; i++) m_stcUnloadPickerX[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xE0, 0xE0));
	for (int i = 0; i < 4; i++) m_stcUnloadPickerZ[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xE0, 0xE0));
	for (int i = 0; i < 2; i++) m_stcUnloadPickerP[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xE0, 0xE0));
	for (int i = 0; i < 3; i++) m_stcUnloadStage1Y[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xA0, 0xF0, 0xC0));
	for (int i = 0; i < 6; i++) m_stcUnloadStage1Z[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xA0, 0xF0, 0xC0));
	for (int i = 0; i < 3; i++) m_stcUnloadStage2Y[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xEF, 0xD5));
	for (int i = 0; i < 6; i++) m_stcUnloadStage2Z[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xEF, 0xD5));
}

void CSetupMoveTab3Dlg::Display_MoveData()
{
	MOVE_DATA *pMoveData = g_objDataManager.Get_pMoveData();
	CString strData;

	for (int i = 0; i < 2; i++) { strData.Format("%0.3lf", pMoveData->dTransStageX[i]);		m_stcTransStageX[i].SetWindowText(strData); }
	for (int i = 0; i < 3; i++) { strData.Format("%0.3lf", pMoveData->dTransStageZ[i]);		m_stcTransStageZ[i].SetWindowText(strData); }
	for (int i = 0; i < 2; i++) { strData.Format("%0.3lf", pMoveData->dTransStageT[i]);		m_stcTransStageT[i].SetWindowText(strData); }
	for (int i = 0; i < 4; i++) { strData.Format("%0.3lf", pMoveData->dUnloadPickerX[i]);	m_stcUnloadPickerX[i].SetWindowText(strData); }
	for (int i = 0; i < 4; i++) { strData.Format("%0.3lf", pMoveData->dUnloadPickerZ[i]);	m_stcUnloadPickerZ[i].SetWindowText(strData); }
	for (int i = 0; i < 2; i++) { strData.Format("%0.3lf", pMoveData->dUnloadPickerP[i]);	m_stcUnloadPickerP[i].SetWindowText(strData); }
	for (int i = 0; i < 3; i++) { strData.Format("%0.3lf", pMoveData->dUnloadStage1Y[i]);	m_stcUnloadStage1Y[i].SetWindowText(strData); }
	for (int i = 0; i < 6; i++) { strData.Format("%0.3lf", pMoveData->dUnloadStage1Z[i]);	m_stcUnloadStage1Z[i].SetWindowText(strData); }
	for (int i = 0; i < 3; i++) { strData.Format("%0.3lf", pMoveData->dUnloadStage2Y[i]);	m_stcUnloadStage2Y[i].SetWindowText(strData); }
	for (int i = 0; i < 6; i++) { strData.Format("%0.3lf", pMoveData->dUnloadStage2Z[i]);	m_stcUnloadStage2Z[i].SetWindowText(strData); }
}

void CSetupMoveTab3Dlg::Save_MoveData()
{
 	CIniFileCS INI(gsCurrentDir + "\\System\\MoveData.ini");
	if (!INI.Check_File()) { AfxMessageBox("MoveData.ini File Not Found!!!"); return; }

	g_objCommon.Backup_File(gsCurrentDir + "\\System", "MoveData");

	CString strKey, strData;
	double dData;

	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i);	m_stcTransStageX[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("25_TRANS_STAGE_X", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 3; i++) { strKey.Format("%02d", i);	m_stcTransStageZ[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("26_TRANS_STAGE_Z", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i);	m_stcTransStageT[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("27_TRANS_STAGE_T", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 4; i++) { strKey.Format("%02d", i);	m_stcUnloadPickerX[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("28_UNLOAD_PICKER_X", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i);	m_stcUnloadPickerP[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("30_UNLOAD_PICKER_P", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 3; i++) { strKey.Format("%02d", i);	m_stcUnloadStage1Y[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("31_UNLOAD_STAGE1_Y", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 6; i++) { strKey.Format("%02d", i);	m_stcUnloadStage1Z[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("32_UNLOAD_STAGE1_Z", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 3; i++) { strKey.Format("%02d", i);	m_stcUnloadStage2Y[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("33_UNLOAD_STAGE2_Y", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 6; i++) { strKey.Format("%02d", i);	m_stcUnloadStage2Z[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("34_UNLOAD_STAGE2_Z", strKey, dData, "%0.3lf"); }

	for (int i = 0; i < 4; i++) {
		strKey.Format("%02d", i);	m_stcUnloadPickerZ[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("29_UNLOAD_PICKER_Z", strKey, dData, "%0.3lf");
	
		// RMS 항목 저장
		if (i == 1) g_objMES.Save_AviRmsData("Cap-Unload Picker Z Transfer Down Position", strData);
		if (i == 2) g_objMES.Save_AviRmsData("Cap-Unload Picker Z Unload1 Down Position", strData);
		if (i == 3) g_objMES.Save_AviRmsData("Cap-Unload Picker Z Unload2 Down Position", strData);
	}

	g_objLogFile.Save_HandlerLog("[Setup - MoveTab Dialog 3] save");

	Cancel_MoveData();
}

void CSetupMoveTab3Dlg::Cancel_MoveData()
{
	g_objDataManager.Read_MoveData();
	Display_MoveData();
}

///////////////////////////////////////////////////////////////////////////////
