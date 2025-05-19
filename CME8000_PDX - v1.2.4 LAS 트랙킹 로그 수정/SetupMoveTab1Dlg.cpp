// SetupMoveTab1Dlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CME8000.h"
#include "SetupMoveTab1Dlg.h"
#include "afxdialogex.h"

#include "LogFile.h"
#include "DataManager.h"
#include "Common.h"
#include "MESInterface.h"

#include "AJinDefine.h"

// CSetupMoveTab1Dlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSetupMoveTab1Dlg, CDialogEx)

CSetupMoveTab1Dlg::CSetupMoveTab1Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetupMoveTab1Dlg::IDD, pParent)
{
}

CSetupMoveTab1Dlg::~CSetupMoveTab1Dlg()
{
}

void CSetupMoveTab1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for (int i = 0; i < 10; i++) DDX_Control(pDX, IDC_GROUP_0 + i, m_Group[i]);
	for (int i = 0; i < 35; i++) DDX_Control(pDX, IDC_LABEL_0 + i, m_Label[i]);
	for (int i = 0; i < 3; i++) DDX_Control(pDX, IDC_STC_TRAY_PICKER_X_0 + i, m_stcTrayPickerX[i]);
	for (int i = 0; i < 3; i++) DDX_Control(pDX, IDC_STC_TRAY_PICKER_Z_0 + i, m_stcTrayPickerZ[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_STC_TRAY_PICKER_R_0 + i, m_stcTrayPickerR[i]);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_STC_LOAD_STAGE1_X_0 + i, m_stcLoadStage1X[i]);
	for (int i = 0; i < 5; i++) DDX_Control(pDX, IDC_STC_LOAD_STAGE1_Z_0 + i, m_stcLoadStage1Z[i]);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_STC_LOAD_STAGE2_X_0 + i, m_stcLoadStage2X[i]);
	for (int i = 0; i < 5; i++) DDX_Control(pDX, IDC_STC_LOAD_STAGE2_Z_0 + i, m_stcLoadStage2Z[i]);
	for (int i = 0; i < 3; i++) DDX_Control(pDX, IDC_STC_LOAD_PICKER_Y_0 + i, m_stcLoadPickerY[i]);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_STC_LOAD_PICKER_Z_0 + i, m_stcLoadPickerZ[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_STC_LOAD_PICKER_P_0 + i, m_stcLoadPickerP[i]);
}

BEGIN_MESSAGE_MAP(CSetupMoveTab1Dlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_TRAY_PICKER_X_0, IDC_STC_TRAY_PICKER_X_2, OnStcTrayPickerXClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_TRAY_PICKER_Z_0, IDC_STC_TRAY_PICKER_Z_2, OnStcTrayPickerZClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_TRAY_PICKER_R_0, IDC_STC_TRAY_PICKER_R_1, OnStcTrayPickerRClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_LOAD_STAGE1_X_0, IDC_STC_LOAD_STAGE1_X_3, OnStcLoadStage1XClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_LOAD_STAGE1_Z_0, IDC_STC_LOAD_STAGE1_Z_4, OnStcLoadStage1ZClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_LOAD_STAGE2_X_0, IDC_STC_LOAD_STAGE2_X_3, OnStcLoadStage2XClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_LOAD_STAGE2_Z_0, IDC_STC_LOAD_STAGE2_Z_4, OnStcLoadStage2ZClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_LOAD_PICKER_Y_0, IDC_STC_LOAD_PICKER_Y_2, OnStcLoadPickerYClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_LOAD_PICKER_Z_0, IDC_STC_LOAD_PICKER_Z_3, OnStcLoadPickerZClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_LOAD_PICKER_P_0, IDC_STC_LOAD_PICKER_P_1, OnStcLoadPickerPClick)
END_MESSAGE_MAP()

// CSetupMoveTab1Dlg 메시지 처리기입니다.

BOOL CSetupMoveTab1Dlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(NULL, 5, 65, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	Initial_Controls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CSetupMoveTab1Dlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CSetupMoveTab1Dlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (!bShow) return;

	Display_MoveData();

	g_objLogFile.Save_HandlerLog("[Setup Move] Show Window - Tab1");
}

void CSetupMoveTab1Dlg::OnStcTrayPickerXClick(UINT nID)
{
	int ID = nID - IDC_STC_TRAY_PICKER_X_0;

	CString strOld, strNew;
	m_stcTrayPickerX[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_TRAY_PICKER_X) != IDOK) return;

	m_stcTrayPickerX[ID].SetWindowText(strNew);
}

void CSetupMoveTab1Dlg::OnStcTrayPickerZClick(UINT nID)
{
	int ID = nID - IDC_STC_TRAY_PICKER_Z_0;

	CString strOld, strNew;
	m_stcTrayPickerZ[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_TRAY_PICKER_Z) != IDOK) return;

	m_stcTrayPickerZ[ID].SetWindowText(strNew);
}

void CSetupMoveTab1Dlg::OnStcTrayPickerRClick(UINT nID)
{
	int ID = nID - IDC_STC_TRAY_PICKER_R_0;

	CString strOld, strNew;
	m_stcTrayPickerR[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_TRAY_PICKER_R) != IDOK) return;

	m_stcTrayPickerR[ID].SetWindowText(strNew);
}

void CSetupMoveTab1Dlg::OnStcLoadStage1XClick(UINT nID)
{
	int ID = nID - IDC_STC_LOAD_STAGE1_X_0;

	CString strOld, strNew;
	m_stcLoadStage1X[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_LOAD_STAGE1_X) != IDOK) return;

	m_stcLoadStage1X[ID].SetWindowText(strNew);
}

void CSetupMoveTab1Dlg::OnStcLoadStage1ZClick(UINT nID)
{
	int ID = nID - IDC_STC_LOAD_STAGE1_Z_0;

	CString strOld, strNew;
	m_stcLoadStage1Z[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_LOAD_STAGE1_Z) != IDOK) return;

	m_stcLoadStage1Z[ID].SetWindowText(strNew);
}

void CSetupMoveTab1Dlg::OnStcLoadStage2XClick(UINT nID)
{
	int ID = nID - IDC_STC_LOAD_STAGE2_X_0;

	CString strOld, strNew;
	m_stcLoadStage2X[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_LOAD_STAGE2_X) != IDOK) return;

	m_stcLoadStage2X[ID].SetWindowText(strNew);
}

void CSetupMoveTab1Dlg::OnStcLoadStage2ZClick(UINT nID)
{
	int ID = nID - IDC_STC_LOAD_STAGE2_Z_0;

	CString strOld, strNew;
	m_stcLoadStage2Z[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_LOAD_STAGE2_Z) != IDOK) return;

	m_stcLoadStage2Z[ID].SetWindowText(strNew);
}

void CSetupMoveTab1Dlg::OnStcLoadPickerYClick(UINT nID)
{
	int ID = nID - IDC_STC_LOAD_PICKER_Y_0;

	CString strOld, strNew;
	m_stcLoadPickerY[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_LOAD_PICKER_Y) != IDOK) return;

	m_stcLoadPickerY[ID].SetWindowText(strNew);
}

void CSetupMoveTab1Dlg::OnStcLoadPickerZClick(UINT nID)
{
	int ID = nID - IDC_STC_LOAD_PICKER_Z_0;

	CString strOld, strNew;
	m_stcLoadPickerZ[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_LOAD_PICKER_Z) != IDOK) return;

	m_stcLoadPickerZ[ID].SetWindowText(strNew);
}

void CSetupMoveTab1Dlg::OnStcLoadPickerPClick(UINT nID)
{
	int ID = nID - IDC_STC_LOAD_PICKER_P_0;

	CString strOld, strNew;
	m_stcLoadPickerP[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_LOAD_PICKER_P) != IDOK) return;

	m_stcLoadPickerP[ID].SetWindowText(strNew);
}

///////////////////////////////////////////////////////////////////////////////
// User Functions

void CSetupMoveTab1Dlg::Initial_Controls() 
{
	for (int i = 0; i < 10; i++) m_Group[i].Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT);
	for (int i = 0; i < 35; i++) m_Label[i].Init_Ctrl("바탕", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x60, 0x60, 0x60));
	for (int i = 0; i < 3; i++)  m_stcTrayPickerX[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xA0, 0xF0, 0xC0));
	for (int i = 0; i < 3; i++)  m_stcTrayPickerZ[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xA0, 0xF0, 0xC0));
	for (int i = 0; i < 2; i++)  m_stcTrayPickerR[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xA0, 0xF0, 0xC0));
	for (int i = 0; i < 4; i++)  m_stcLoadStage1X[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xEF, 0xD5));
	for (int i = 0; i < 5; i++)  m_stcLoadStage1Z[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xEF, 0xD5));
	for (int i = 0; i < 4; i++)  m_stcLoadStage2X[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xE0, 0xE0));
	for (int i = 0; i < 5; i++)  m_stcLoadStage2Z[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xE0, 0xE0));
	for (int i = 0; i < 3; i++)  m_stcLoadPickerY[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xC0, 0xFF, 0xF0));
	for (int i = 0; i < 4; i++)  m_stcLoadPickerZ[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xC0, 0xFF, 0xF0));
	for (int i = 0; i < 2; i++)  m_stcLoadPickerP[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xC0, 0xFF, 0xF0));
}

void CSetupMoveTab1Dlg::Display_MoveData()
{
	MOVE_DATA *pMoveData = g_objDataManager.Get_pMoveData();
	CString strData;

	for (int i = 0; i < 3; i++) { strData.Format("%0.3lf", pMoveData->dTrayPickerX[i]);		m_stcTrayPickerX[i].SetWindowText(strData); }
	for (int i = 0; i < 3; i++) { strData.Format("%0.3lf", pMoveData->dTrayPickerZ[i]);		m_stcTrayPickerZ[i].SetWindowText(strData); }
	for (int i = 0; i < 2; i++) { strData.Format("%0.3lf", pMoveData->dTrayPickerR[i]);		m_stcTrayPickerR[i].SetWindowText(strData); }
	for (int i = 0; i < 4; i++) { strData.Format("%0.3lf", pMoveData->dLoadStage1X[i]);		m_stcLoadStage1X[i].SetWindowText(strData); }
	for (int i = 0; i < 5; i++) { strData.Format("%0.3lf", pMoveData->dLoadStage1Z[i]);		m_stcLoadStage1Z[i].SetWindowText(strData); }
	for (int i = 0; i < 4; i++) { strData.Format("%0.3lf", pMoveData->dLoadStage2X[i]);		m_stcLoadStage2X[i].SetWindowText(strData); }
	for (int i = 0; i < 5; i++) { strData.Format("%0.3lf", pMoveData->dLoadStage2Z[i]);		m_stcLoadStage2Z[i].SetWindowText(strData); }
	for (int i = 0; i < 3; i++) { strData.Format("%0.3lf", pMoveData->dLoadPickerY[i]);		m_stcLoadPickerY[i].SetWindowText(strData); }
	for (int i = 0; i < 4; i++) { strData.Format("%0.3lf", pMoveData->dLoadPickerZ[i]);		m_stcLoadPickerZ[i].SetWindowText(strData); }
	for (int i = 0; i < 2; i++) { strData.Format("%0.3lf", pMoveData->dLoadPickerP[i]);		m_stcLoadPickerP[i].SetWindowText(strData); }
}

void CSetupMoveTab1Dlg::Save_MoveData()
{
 	CIniFileCS INI(gsCurrentDir + "\\System\\MoveData.ini");
	if (!INI.Check_File()) { AfxMessageBox("MoveData.ini File Not Found!!!"); return; }

	g_objCommon.Backup_File(gsCurrentDir + "\\System", "MoveData");

	CString strKey, strData;
	double dData;

	for (int i = 0; i < 3; i++) { strKey.Format("%02d", i);	m_stcTrayPickerX[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("00_TRAY_PICKER_X", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 3; i++) { strKey.Format("%02d", i);	m_stcTrayPickerZ[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("01_TRAY_PICKER_Z", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i);	m_stcTrayPickerR[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("02_TRAY_PICKER_R", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 4; i++) { strKey.Format("%02d", i);	m_stcLoadStage1X[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("04_LOAD_STAGE1_X", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 5; i++) { strKey.Format("%02d", i);	m_stcLoadStage1Z[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("05_LOAD_STAGE1_Z", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 4; i++) { strKey.Format("%02d", i);	m_stcLoadStage2X[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("06_LOAD_STAGE2_X", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 5; i++) { strKey.Format("%02d", i);	m_stcLoadStage2Z[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("07_LOAD_STAGE2_Z", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 3; i++) { strKey.Format("%02d", i);	m_stcLoadPickerY[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("08_LOAD_PICKER_Y", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 2; i++) { strKey.Format("%02d", i);	m_stcLoadPickerP[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("10_LOAD_PICKER_P", strKey, dData, "%0.3lf"); }
	
	for (int i = 0; i < 4; i++) {
		strKey.Format("%02d", i);	m_stcLoadPickerZ[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("09_LOAD_PICKER_Z", strKey, dData, "%0.3lf");

		// RMS 항목 저장
		if (i == 1) g_objMES.Save_AviRmsData("Cap-Load Picker Z Load1 Down Position", strData);
		if (i == 2) g_objMES.Save_AviRmsData("Cap-Load Picker Z Load2 Down Position", strData);
		if (i == 3) g_objMES.Save_AviRmsData("Cap-Load Picker Z Index Down Position", strData);
	}

	g_objLogFile.Save_HandlerLog("[Setup - MoveTab Dialog 1] save");

	Cancel_MoveData();
}

void CSetupMoveTab1Dlg::Cancel_MoveData()
{
	g_objDataManager.Read_MoveData();
	Display_MoveData();
}

///////////////////////////////////////////////////////////////////////////////
