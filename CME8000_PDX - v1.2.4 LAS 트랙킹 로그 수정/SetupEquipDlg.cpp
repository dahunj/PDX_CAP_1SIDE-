// SetupEquipDlg.cpp : ±∏«ˆ ∆ƒ¿œ¿‘¥œ¥Ÿ.
//
#include "stdafx.h"
#include "CME8000.h"
#include "SetupEquipDlg.h"
#include "afxdialogex.h"

#include "LogFile.h"
#include "DataManager.h"
#include "Common.h"

#include "SetupDlg.h"
#include "Inspector.h"
#include "SequenceMain.h"
#include "SequenceInit.h"
#include "WorkDlg.h"
#include "MESInterface.h"

#include "CME8000Dlg.h"
#include "AviUDP.h"

// CSetupEquipDlg ¥Î»≠ ªÛ¿⁄¿‘¥œ¥Ÿ.

IMPLEMENT_DYNAMIC(CSetupEquipDlg, CDialogEx)

CSetupEquipDlg::CSetupEquipDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetupEquipDlg::IDD, pParent)
{
}

CSetupEquipDlg::~CSetupEquipDlg()
{
}

void CSetupEquipDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for (int i = 0; i < 11; i++) DDX_Control(pDX, IDC_GROUP_0 + i, m_Group[i]);
	for (int i = 0; i < 36; i++) DDX_Control(pDX, IDC_LABEL_0 + i,  m_Label[i]);
	DDX_Control(pDX, IDC_STC_EQUIP_NAME, m_stcEquipName);
	DDX_Control(pDX, IDC_CBO_LOT_BARCODE_PORT, m_cboLotBarcodePort);
	DDX_Control(pDX, IDC_CBO_LOAD_CELL_PORT_0, m_cboAssyLoadCellPort);
	DDX_Control(pDX, IDC_CBO_LOAD_CELL_PORT_1, m_cboUnloadLoadCellPort);
	DDX_Control(pDX, IDC_CHK_JOB_LIST_TAKT_LOG, m_chkJobListTaktLog);
	DDX_Control(pDX, IDC_STC_SCREEN_OFF, m_stcScreenOff);
	DDX_Control(pDX, IDC_STC_NOWORK_TIME, m_stcNoWorkTime);
	DDX_Control(pDX, IDC_STC_MOTION_CHECK, m_stcMotionCheck);
	DDX_Control(pDX, IDC_CHK_MANUAL_TAKT_TEST, m_chkManualTaktTest);
	DDX_Control(pDX, IDC_LBL_DOOR_LOCK, m_lblDoorLock);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_RDO_DOOR_LOCK_0 + i, m_rdoDoorLock[i]);
	for (int i = 0; i < 3; i++) DDX_Control(pDX, IDC_RDO_PICK_COUNT_0 + i, m_rdoPickCnt[i]);
	DDX_Control(pDX, IDC_LBL_DOOR_LOCK2, m_lblDoorLock2);
	DDX_Control(pDX, IDC_STC_DOORLOCK_TIME, m_stcDoorLockTime);

	DDX_Control(pDX, IDC_CHK_USE_INLINE_MODE, m_chkUseInlineMode);
	DDX_Control(pDX, IDC_CHK_USE_VISION_CM_ALIGN, m_chkUseVisionCmAlign);
	DDX_Control(pDX, IDC_CHK_USE_VISION_CAP_ALIGN, m_chkUseVisionCapAlign);
	DDX_Control(pDX, IDC_CHK_USE_TRAY_TURN, m_chkUseTrayPickerTurn);
	DDX_Control(pDX, IDC_CHK_USE_CAP_TURN, m_chkUseCapPickerTurn);
	DDX_Control(pDX, IDC_CHK_USE_ASSY_VAC, m_chkUseAssyPickerVac);
	DDX_Control(pDX, IDC_CHK_USE_INDEX_ASSY_VAC, m_chkUseIndexAssyVac);
	DDX_Control(pDX, IDC_CHK_USE_TILT, m_chkUseAssyPickerTiltChk);
	DDX_Control(pDX, IDC_CHK_USE_ALIGN_ALARM, m_chkUseVisionAlignAlarm);
	DDX_Control(pDX, IDC_CHK_USE_ALIGN_OFFSET, m_chkUseVisionAlignOffset);
	DDX_Control(pDX, IDC_CHK_LOAD_SORT_DIR, m_chkUseLoadTraySortDir);
	DDX_Control(pDX, IDC_CHK_CAP_PICKER_MULTI, m_chkCapPickUpMulti);
	DDX_Control(pDX, IDC_CHK_SHIP_ONE_EMPTY, m_chkShipTrayOneEmpty);
	DDX_Control(pDX, IDC_CHK_USE_MES_CAP_REG, m_chkUseMesCapReg);
	DDX_Control(pDX, IDC_CHK_USE_MES_SHIP_REG, m_chkUseMesShipReg);

	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_STC_LOAD_TRAY_DATA_0 + i, m_stcLoadTrayData[i]);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_STC_CAP_TRAY_DATA_0 + i, m_stcCapTrayData[i]);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_STC_UNLOAD_TRAY_DATA_0 + i, m_stcUnloadTrayData[i]);
	for (int i = 0; i < 1; i++) DDX_Control(pDX, IDC_STC_INDEX_DATA_0 + i, m_stcIndexData[i]);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_STC_VAC_OFF_DELAY_0 + i, m_stcVacOffDelay[i]);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_STC_VAC_OFF_REPEAT_0 + i, m_stcVacOffRepeat[i]);
	for (int i = 0; i < 6; i++) DDX_Control(pDX, IDC_STC_DELAY_ADD_0 + i, m_stcDelayAdd[i]);
	DDX_Control(pDX, IDC_STC_ALIGN_OFFSET, m_stcAlignOffset);

	DDX_Control(pDX, IDC_GRP_AVI, m_grpAvi);
	DDX_Control(pDX, IDC_LBL_AVI, m_lblAvi);
	DDX_Control(pDX, IDC_IPA_AVI_IP, m_ipaAviIp);
	DDX_Control(pDX, IDC_GRP_TOWER, m_grpTower);
	for (int i = 0; i < 10; i++) DDX_Control(pDX, IDC_LBL_TOWER_0 + i,  m_lblTower[i]);
	for (int i = 0; i < 6; i++) for (int j = 0; j < 4; j++) DDX_Control(pDX, IDC_CHK_TOWER_0_0 + i * 4 + j,  m_chkTower[i][j]);
	DDX_Control(pDX, IDC_GRP_BUZZER, m_grpBuzzer);
	for (int i = 0; i < 11; i++) DDX_Control(pDX, IDC_LBL_BUZZER_0 + i,  m_lblBuzzer[i]);
	for (int i = 0; i < 5; i++) for (int j = 0; j < 6; j++) DDX_Control(pDX, IDC_CHK_BUZZER_0_0 + i * 6 + j,  m_chkBuzzer[i][j]);
	DDX_Control(pDX, IDC_GRP_HIDDEN, m_grpHidden);
	DDX_Control(pDX, IDC_LBL_PASSWORD_MT, m_lblPasswordMt);
	DDX_Control(pDX, IDC_STC_PASSWORD_MT, m_stcPasswordMt);
	DDX_Control(pDX, IDC_LBL_PASSWORD_SI, m_lblPasswordSi);
	DDX_Control(pDX, IDC_EDT_PASSWORD_SI, m_edtPasswordSi);
	DDX_Control(pDX, IDC_GRP_RESULT_TEST, m_grpResultTest);
	DDX_Control(pDX, IDC_CHK_RESULT_TEST_USE, m_chkResultTestUse);
	for (int i = 0; i < 3; i++) DDX_Control(pDX, IDC_LBL_RESULT_TEST_0 + i, m_lblResultTest[i]);
	for (int i = 0; i < 3; i++) DDX_Control(pDX, IDC_EDT_RESULT_TEST_0 + i, m_edtResultTest[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_STC_LOAD_CELL_DATA_0 + i, m_stcLoadCellData[i]);
}

BEGIN_MESSAGE_MAP(CSetupEquipDlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_STN_CLICKED(IDC_STC_EQUIP_NAME, &CSetupEquipDlg::OnStnClickedStcEquipName)
	ON_STN_CLICKED(IDC_STC_SCREEN_OFF, &CSetupEquipDlg::OnStnClickedStcScreenOff)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_LOAD_TRAY_DATA_0, IDC_STC_LOAD_TRAY_DATA_3, OnStcLoadTrayDataClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_CAP_TRAY_DATA_0, IDC_STC_CAP_TRAY_DATA_3, OnStcCapTrayDataClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_UNLOAD_TRAY_DATA_0, IDC_STC_UNLOAD_TRAY_DATA_3, OnStcUnloadTrayDataClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_INDEX_DATA_0, IDC_STC_INDEX_DATA_0, OnStcIndexDataClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_VAC_OFF_DELAY_0, IDC_STC_VAC_OFF_DELAY_3, OnStcVacOffDelayClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_VAC_OFF_REPEAT_0, IDC_STC_VAC_OFF_REPEAT_3, OnStcVacOffRepeatClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_DELAY_ADD_0, IDC_STC_DELAY_ADD_5, OnStcDelayAddClick)
	ON_STN_CLICKED(IDC_STC_ALIGN_OFFSET, &CSetupEquipDlg::OnStcAlignOffsetClick)
	ON_STN_CLICKED(IDC_STC_SHOW_HIDDEN, &CSetupEquipDlg::OnStnClickedStcShowHidden)
	ON_STN_CLICKED(IDC_STC_PASSWORD_MT, &CSetupEquipDlg::OnStnClickedStcPasswordMt)
	ON_STN_CLICKED(IDC_STC_NOWORK_TIME, &CSetupEquipDlg::OnStnClickedStcNoworkTime)
	ON_STN_CLICKED(IDC_STC_MOTION_CHECK, &CSetupEquipDlg::OnStnClickedStcMotionCheck)
	ON_STN_CLICKED(IDC_STC_DOORLOCK_TIME, &CSetupEquipDlg::OnStnClickedstcRockTime)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_LOAD_CELL_DATA_0, IDC_STC_LOAD_CELL_DATA_1, OnStcLoadCellDataClick)
	ON_BN_CLICKED(IDC_BTN_AVI_CONNECT, &CSetupEquipDlg::OnBnClickedBtnAviConnect)
	ON_BN_CLICKED(IDC_BTN_AVI_DISCONNECT, &CSetupEquipDlg::OnBnClickedBtnAviDisconnect)
END_MESSAGE_MAP()

// CSetupEquipDlg ∏ﬁΩ√¡ˆ √≥∏Æ±‚¿‘¥œ¥Ÿ.

BOOL CSetupEquipDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	// TODO:  ø©±‚ø° √ﬂ∞° √ ±‚»≠ ¿€æ˜¿ª √ﬂ∞°«’¥œ¥Ÿ.
	SetWindowPos(this, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	m_nPreModel = -1;
	Initial_Controls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// øπø‹: OCX º”º∫ ∆‰¿Ã¡ˆ¥¬ FALSE∏¶ π›»Ø«ÿæﬂ «’¥œ¥Ÿ.
}

BOOL CSetupEquipDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CSetupEquipDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (!bShow) {
		if (m_nPreModel != -1) {
			CIniFileCS INI(gsCurrentDir + "\\System\\ModelData.ini");
			if (!INI.Check_File()) { AfxMessageBox("ModelData.ini File Not Found!!!"); return; }

			INI.Set_Integer("MODEL", "MODEL", m_nPreModel);

			g_objDataManager.Read_ModelData();
			g_objDataManager.Read_EquipData();
			m_nPreModel = -1;
		}
		return;
	}

	Display_EquipData();

	m_grpHidden.ShowWindow(SW_HIDE);
	m_lblPasswordMt.ShowWindow(SW_HIDE);
	m_stcPasswordMt.ShowWindow(SW_HIDE);
	m_lblPasswordSi.ShowWindow(SW_HIDE);
	m_edtPasswordSi.ShowWindow(SW_HIDE);

#ifdef DRY_RUN_TEST
	m_grpResultTest.ShowWindow(SW_SHOW);
	m_chkResultTestUse.ShowWindow(SW_HIDE);
	for (int i = 0; i < 3; i++) m_lblResultTest[i].ShowWindow(SW_SHOW);
	for (int i = 0; i < 3; i++) m_edtResultTest[i].ShowWindow(SW_SHOW);
#else
	m_grpResultTest.ShowWindow(SW_HIDE);
	m_chkResultTestUse.ShowWindow(SW_HIDE);
	for (int i = 0; i < 3; i++) m_lblResultTest[i].ShowWindow(SW_HIDE);
	for (int i = 0; i < 3; i++) m_edtResultTest[i].ShowWindow(SW_HIDE);
#endif

}

void CSetupEquipDlg::OnStnClickedStcEquipName()
{
	CString strKey;
	if (g_objCommon.Show_KeyPad(strKey) != IDOK) return;

	m_stcEquipName.SetWindowText(strKey);
}

void CSetupEquipDlg::OnStnClickedStcScreenOff()
{
	CString strOld, strNew;
	m_stcScreenOff.GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	m_stcScreenOff.SetWindowText(strNew);
}

void CSetupEquipDlg::OnStnClickedStcNoworkTime()
{
	CString strOld, strNew;
	m_stcNoWorkTime.GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	m_stcNoWorkTime.SetWindowText(strNew);
}

void CSetupEquipDlg::OnStnClickedStcMotionCheck()
{
	CString strOld, strNew;
	m_stcMotionCheck.GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	m_stcMotionCheck.SetWindowText(strNew);
}

void CSetupEquipDlg::OnStcLoadTrayDataClick(UINT nID)
{
	int ID = nID - IDC_STC_LOAD_TRAY_DATA_0;

	CString strOld, strNew;
	m_stcLoadTrayData[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	m_stcLoadTrayData[ID].SetWindowText(strNew);
}

void CSetupEquipDlg::OnStcCapTrayDataClick(UINT nID)
{
	int ID = nID - IDC_STC_CAP_TRAY_DATA_0;

	CString strOld, strNew;
	m_stcCapTrayData[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	m_stcCapTrayData[ID].SetWindowText(strNew);
}

void CSetupEquipDlg::OnStcUnloadTrayDataClick(UINT nID)
{
	int ID = nID - IDC_STC_UNLOAD_TRAY_DATA_0;

	CString strOld, strNew;
	m_stcUnloadTrayData[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	m_stcUnloadTrayData[ID].SetWindowText(strNew);
}

void CSetupEquipDlg::OnStcIndexDataClick(UINT nID)
{
	int ID = nID - IDC_STC_INDEX_DATA_0;

	CString strOld, strNew;
	m_stcIndexData[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	m_stcIndexData[ID].SetWindowText(strNew);
}

void CSetupEquipDlg::OnStcVacOffDelayClick(UINT nID)
{
	int ID = nID - IDC_STC_VAC_OFF_DELAY_0;

	CString strOld, strNew;
	m_stcVacOffDelay[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	m_stcVacOffDelay[ID].SetWindowText(strNew);
}

void CSetupEquipDlg::OnStcVacOffRepeatClick(UINT nID)
{
	int ID = nID - IDC_STC_VAC_OFF_REPEAT_0;

	CString strOld, strNew;
	m_stcVacOffRepeat[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	m_stcVacOffRepeat[ID].SetWindowText(strNew);
}

void CSetupEquipDlg::OnStcDelayAddClick(UINT nID)
{
	int ID = nID - IDC_STC_DELAY_ADD_0;

	CString strOld, strNew;
	m_stcDelayAdd[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	m_stcDelayAdd[ID].SetWindowText(strNew);
}

void CSetupEquipDlg::OnStcAlignOffsetClick()
{
	CString strOld, strNew;
	m_stcAlignOffset.GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	m_stcAlignOffset.SetWindowText(strNew);
}

void CSetupEquipDlg::OnStnClickedstcRockTime()
{
	CString strOld, strNew;

	m_stcDoorLockTime.GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	m_stcDoorLockTime.SetWindowText(strNew);
}

void CSetupEquipDlg::OnStnClickedStcPasswordMt()
{
	CString strTemp, strData;

	if (g_objCommon.Show_NumPad("", strData) != IDOK) return;

	int nLen = strData.GetLength();
	if (nLen < 1) return;

	for (int i = 0; i < nLen; i++ ) {
		strTemp = strData.Mid(i, 1);
		if (strTemp < "0" || strTemp > "9") { AfxMessageBox("Password Input Error......"); return; }
	}
	m_stcPasswordMt.SetWindowText(strData);
}

void CSetupEquipDlg::OnStnClickedStcShowHidden()
{
	if (m_grpHidden.IsWindowVisible()) {
		m_grpHidden.ShowWindow(SW_HIDE);
		m_lblPasswordMt.ShowWindow(SW_HIDE);
		m_stcPasswordMt.ShowWindow(SW_HIDE);
		if (g_dlgSetup.Get_LoginUser() != 2) return;
		m_lblPasswordSi.ShowWindow(SW_HIDE);
		m_edtPasswordSi.ShowWindow(SW_HIDE);
	} else {
		m_grpHidden.ShowWindow(SW_SHOW);
		m_lblPasswordMt.ShowWindow(SW_SHOW);
		m_stcPasswordMt.ShowWindow(SW_SHOW);
		if (g_dlgSetup.Get_LoginUser() != 2) return;
		m_lblPasswordSi.ShowWindow(SW_SHOW);
		m_edtPasswordSi.ShowWindow(SW_SHOW);
	}

#ifndef DRY_RUN_TEST
	if (m_grpResultTest.IsWindowVisible()) {
		m_grpResultTest.ShowWindow(SW_HIDE);
		m_chkResultTestUse.ShowWindow(SW_HIDE);
		for (int i = 0; i < 3; i++) m_lblResultTest[i].ShowWindow(SW_HIDE);
		for (int i = 0; i < 3; i++) m_edtResultTest[i].ShowWindow(SW_HIDE);
	} else {
		m_grpResultTest.ShowWindow(SW_SHOW);
		m_chkResultTestUse.ShowWindow(SW_SHOW);
		for (int i = 0; i < 3; i++) m_lblResultTest[i].ShowWindow(SW_SHOW);
		for (int i = 0; i < 3; i++) m_edtResultTest[i].ShowWindow(SW_SHOW);
	}
#endif
}

void CSetupEquipDlg::OnStcLoadCellDataClick(UINT nID)
{
	int ID = nID - IDC_STC_LOAD_CELL_DATA_0;

	CString strOld, strNew;
	m_stcLoadCellData[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	m_stcLoadCellData[ID].SetWindowText(strNew);
}

///////////////////////////////////////////////////////////////////////////////
// User Functions

void CSetupEquipDlg::Initial_Controls() 
{
	CString strText;
	for (int i = 0; i < 11; i++) m_Group[i].Init_Ctrl("πŸ≈¡", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT);
	for (int i = 0; i < 3; i++) m_Label[i].Init_Ctrl("πŸ≈¡", 11, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x10, 0xC0));	// Equip Data
	for (int i = 3; i < 7; i++) m_Label[i].Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x20, 0x50, 0x00));	// Tray Data
	for (int i = 7; i < 11; i++) m_Label[i].Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x40, 0x20, 0x20));	// Picker Data
	for (int i = 11; i < 15; i++) m_Label[i].Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x40, 0x80));	// Inspect Data
	for (int i = 15; i < 17; i++) m_Label[i].Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x60, 0x60, 0x60));	// Vac Label
	for (int i = 17; i < 21; i++) m_Label[i].Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x80, 0x00, 0x00));	// Vacuum Off
	for (int i = 21; i < 24; i++) m_Label[i].Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x40, 0x60, 0x40));	// Delay Add
	m_Label[24].Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x40, 0x60, 0x40));	// Delay Add
	m_Label[25].Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x80, 0xC0));	// Inspect Offset
	m_Label[26].Init_Ctrl("πŸ≈¡", 11, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x10, 0xC0));	// Equip Data
	m_Label[27].Init_Ctrl("πŸ≈¡", 10, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x10, 0xC0));	// Equip Data
	m_Label[28].Init_Ctrl("πŸ≈¡", 11, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x10, 0xC0));	// Equip Data
	m_Label[29].Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x80, 0xC0));	// Vision Align Offset
	m_Label[32].Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x40, 0x60, 0x40));	// Delay Add
	m_Label[33].Init_Ctrl("πŸ≈¡", 11, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xFF, 0xA0, 0x00));	// Picker Count
	m_Label[34].Init_Ctrl("πŸ≈¡", 11, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x10, 0xC0));	// Equip Data Motion check
	m_Label[35].Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x40, 0x60, 0x40));	// Delay Add
	m_stcEquipName.Init_Ctrl("πŸ≈¡", 15, TRUE, RGB(0x00, 0x00, 0x80), RGB(0xE0, 0xFF, 0xE0));
	for (int i = 0; i < 4; i++) { strText.Format("COM%d", i + 1); m_cboLotBarcodePort.AddString(strText); }
	m_cboLotBarcodePort.Init_Ctrl("πŸ≈¡", 12, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xF0, 0xE0, 0x00));

	for (int i = 7; i < 9; i++) { strText.Format("COM%d", i + 1); m_cboAssyLoadCellPort.AddString(strText); }
	m_cboAssyLoadCellPort.Init_Ctrl("πŸ≈¡", 12, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xF0, 0xE0, 0x00));

	for (int i = 7; i < 9; i++) { strText.Format("COM%d", i + 1); m_cboUnloadLoadCellPort.AddString(strText); }
	m_cboUnloadLoadCellPort.Init_Ctrl("πŸ≈¡", 12, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xF0, 0xE0, 0x00));

	m_chkJobListTaktLog.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xC0, 0xC0, 0xC0), CCheckCS::emRed, 0);
	m_stcScreenOff.Init_Ctrl("πŸ≈¡", 11, TRUE, RGB(0x00, 0x00, 0x80), RGB(0xF0, 0xE0, 0x00));
	m_stcNoWorkTime.Init_Ctrl("πŸ≈¡", 11, TRUE, RGB(0x00, 0x00, 0x80), RGB(0xF0, 0xE0, 0x00));
	m_stcMotionCheck.Init_Ctrl("πŸ≈¡", 11, TRUE, RGB(0x00, 0x00, 0x80), RGB(0xF0, 0xE0, 0x00));
	m_chkManualTaktTest.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xC0, 0xC0, 0xC0), CCheckCS::emBlue, 0);
	m_lblDoorLock.Init_Ctrl("πŸ≈¡", 11, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x60, 0x60, 0x60));
	for (int i = 0; i < 2; i++) m_rdoDoorLock[i].Init_Ctrl("πŸ≈¡", 11, FALSE, COLOR_DEFAULT, RGB(0xC0, 0xC0, 0xC0), CRadioCS::emRed, 0);
	for (int i = 0; i < 3; i++) m_rdoPickCnt[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xA0, 0x00), CRadioCS::emRed, 0);
	m_lblDoorLock2.Init_Ctrl("πŸ≈¡", 11, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x60, 0x60, 0x60));
	m_stcDoorLockTime.Init_Ctrl("πŸ≈¡", 11, TRUE, RGB(0x00, 0x00, 0x80), RGB(0xF0, 0xE0, 0x00));

	m_chkUseInlineMode.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xD0, 0x80, 0x20), CCheckCS::emRed, 0);
	m_chkUseVisionCmAlign.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0x60, 0xF0, 0x80), CCheckCS::emRed, 0);
	m_chkUseVisionCapAlign.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0x60, 0xF0, 0x80), CCheckCS::emRed, 0);
	m_chkUseTrayPickerTurn.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xD0, 0xA0, 0xF0), CCheckCS::emRed, 0);
	m_chkUseCapPickerTurn.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xD0, 0xA0, 0xF0), CCheckCS::emRed, 0);
	m_chkUseAssyPickerVac.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xD0, 0xA0, 0xF0), CCheckCS::emRed, 0);
	m_chkUseIndexAssyVac.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xD0, 0xA0, 0xF0), CCheckCS::emRed, 0);
	m_chkUseAssyPickerTiltChk.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xD0, 0xA0, 0xF0), CCheckCS::emRed, 0);
	m_chkUseVisionAlignAlarm.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xD0, 0xA0, 0xF0), CCheckCS::emRed, 0);
	m_chkUseVisionAlignOffset.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xD0, 0xA0, 0xF0), CCheckCS::emRed, 0);
	m_chkUseLoadTraySortDir.Init_Ctrl("πŸ≈¡", 10, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xFF, 0x00), CCheckCS::emRed, 0);
	m_chkCapPickUpMulti.Init_Ctrl("πŸ≈¡", 10, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xA0, 0x00), CCheckCS::emRed, 0);
	m_chkShipTrayOneEmpty.Init_Ctrl("πŸ≈¡", 10, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xFF, 0x00), CCheckCS::emRed, 0);
	m_chkUseMesCapReg.Init_Ctrl("πŸ≈¡", 10, TRUE, COLOR_DEFAULT, RGB(0xD0, 0x80, 0x20), CCheckCS::emRed, 0);
	m_chkUseMesShipReg.Init_Ctrl("πŸ≈¡", 10, TRUE, COLOR_DEFAULT, RGB(0xD0, 0x80, 0x20), CCheckCS::emRed, 0);

	for (int i = 0; i < 2; i++) m_stcLoadTrayData[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xD0, 0xD0, 0xD0));
	for (int i = 2; i < 4; i++) m_stcLoadTrayData[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xFF, 0xE0));
	for (int i = 0; i < 2; i++) m_stcCapTrayData[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xD0, 0xD0, 0xD0));
	for (int i = 2; i < 4; i++) m_stcCapTrayData[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xFF, 0xE0));
	for (int i = 0; i < 2; i++) m_stcUnloadTrayData[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xD0, 0xD0, 0xD0));
	for (int i = 2; i < 4; i++) m_stcUnloadTrayData[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xFF, 0xE0));
	for (int i = 0; i < 1; i++) m_stcIndexData[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xFF, 0xE0));
	for (int i = 0; i < 4; i++) m_stcVacOffDelay[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xE0, 0xE0));
	for (int i = 0; i < 4; i++) m_stcVacOffRepeat[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xE0, 0xE0));
	for (int i = 0; i < 6; i++) m_stcDelayAdd[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xFF, 0xE0));
	m_stcAlignOffset.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xFF, 0xE0));
	m_grpAvi.Init_Ctrl("πŸ≈¡", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT);
	m_lblAvi.Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x60, 0x60, 0x60)); // ROS Data
	m_grpTower.Init_Ctrl("πŸ≈¡", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT);
	for (int i = 0; i < 10; i++) m_lblTower[i].Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x40, 0x40, 0x40));
	for (int i = 0; i < 6; i++) for (int j = 0; j < 4; j++) m_chkTower[i][j].Init_Ctrl("πŸ≈¡", 11, FALSE, COLOR_DEFAULT, RGB(0x00, 0xF0, 0x00), CCheckCS::emRed, 0);
	m_grpBuzzer.Init_Ctrl("πŸ≈¡", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT);
	for (int i = 0; i < 11; i++) m_lblBuzzer[i].Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x40, 0x40, 0x40));
	for (int i = 0; i < 5; i++) for (int j = 0; j < 6; j++) m_chkBuzzer[i][j].Init_Ctrl("πŸ≈¡", 11, FALSE, COLOR_DEFAULT, RGB(0x00, 0xE0, 0xF0), CCheckCS::emRed, 0);
	m_grpHidden.Init_Ctrl("πŸ≈¡", 10, TRUE, COLOR_DEFAULT, COLOR_DEFAULT);
	m_lblPasswordMt.Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x60, 0x60, 0x60));
	m_stcPasswordMt.Init_Ctrl("πŸ≈¡", 11, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0xC0, 0xC0, 0xC0));
	m_lblPasswordSi.Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x60, 0x60, 0x60));
	m_edtPasswordSi.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, COLOR_DEFAULT);
	m_grpResultTest.Init_Ctrl("πŸ≈¡", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT);
	m_chkResultTestUse.Init_Ctrl("πŸ≈¡", 10, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x60, 0x60, 0x60), CCheckCS::emRed, 0);
	for (int i = 0; i < 3; i++) m_lblResultTest[i].Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x60, 0x60, 0x60));
	for (int i = 0; i < 3; i++) m_edtResultTest[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, COLOR_DEFAULT);
	for (int i = 0; i < 2; i++) m_stcLoadCellData[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xE0, 0xE0));
}

void CSetupEquipDlg::Display_EquipData()
{
	CString strData;
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	m_stcEquipName.SetWindowText(pEquipData->sEquipName);
	m_cboLotBarcodePort.SetCurSel(pEquipData->nLotBarcodePort - 1);
	m_cboAssyLoadCellPort.SetCurSel(pEquipData->nAssyLoadCellPort - 8);
	m_cboUnloadLoadCellPort.SetCurSel(pEquipData->nUnloadLoadCellPort - 8);
	m_chkJobListTaktLog.SetCheck(pEquipData->bJobListTaktLog);
	strData.Format("%d", pEquipData->nScreenOff); m_stcScreenOff.SetWindowText(strData);
	strData.Format("%d", pEquipData->nNoWorkTime); m_stcNoWorkTime.SetWindowText(strData);
	strData.Format("%0.3lf", gAlm.dMotionChkPos);	 m_stcMotionCheck.SetWindowText(strData);
	m_chkManualTaktTest.SetCheck(pEquipData->bManualTaktTest);
	pEquipData->bUseDoorLock ? m_rdoDoorLock[1].SetCheck(TRUE) : m_rdoDoorLock[0].SetCheck(TRUE);
	strData.Format("%d", gData.nDoorLockTime);	m_stcDoorLockTime.SetWindowText(strData);

	m_chkUseInlineMode.SetCheck(pEquipData->bUseInlineMode);
	m_chkUseVisionCmAlign.SetCheck(pEquipData->bUseVisionCmAlign);
	m_chkUseVisionCapAlign.SetCheck(pEquipData->bUseVisionCapAlign);
	m_chkUseTrayPickerTurn.SetCheck(pEquipData->bUseTrayPickerTurn);
	m_chkUseCapPickerTurn.SetCheck(pEquipData->bUseCapPickerTurn);
	m_chkUseAssyPickerVac.SetCheck(pEquipData->bUseAssyPickerVac);
	m_chkUseIndexAssyVac.SetCheck(pEquipData->bUseIndexAssyVac);
	m_chkUseAssyPickerTiltChk.SetCheck(pEquipData->bUseAssyPickerTiltChk);
	m_chkUseVisionAlignAlarm.SetCheck(pEquipData->bUseVisionAlignAlarm);
	m_chkUseVisionAlignOffset.SetCheck(pEquipData->bUseAlignOffset);
	m_chkUseLoadTraySortDir.SetCheck(pEquipData->bSortDirRight);
	m_chkCapPickUpMulti.SetCheck(pEquipData->bCapPickUpMulti);
	m_chkShipTrayOneEmpty.SetCheck(pEquipData->bShipTrayOneEmpty);
	m_chkUseMesCapReg.SetCheck(pEquipData->bUseMesCapReg);
	m_chkUseMesShipReg.SetCheck(pEquipData->bUseMesShipReg);

	strData.Format("%d", pEquipData->nLoadTrayX); m_stcLoadTrayData[0].SetWindowText(strData);
	strData.Format("%d", pEquipData->nLoadTrayY); m_stcLoadTrayData[1].SetWindowText(strData);
	strData.Format("%0.2lf", pEquipData->dLoadTrayPitchX); m_stcLoadTrayData[2].SetWindowText(strData);
	strData.Format("%0.2lf", pEquipData->dLoadTrayPitchY); m_stcLoadTrayData[3].SetWindowText(strData);
	strData.Format("%d", pEquipData->nCapTrayX); m_stcCapTrayData[0].SetWindowText(strData);
	strData.Format("%d", pEquipData->nCapTrayY); m_stcCapTrayData[1].SetWindowText(strData);
	strData.Format("%0.2lf", pEquipData->dCapTrayPitchX); m_stcCapTrayData[2].SetWindowText(strData);
	strData.Format("%0.2lf", pEquipData->dCapTrayPitchY); m_stcCapTrayData[3].SetWindowText(strData);
	strData.Format("%d", pEquipData->nUnloadTrayX); m_stcUnloadTrayData[0].SetWindowText(strData);
	strData.Format("%d", pEquipData->nUnloadTrayY); m_stcUnloadTrayData[1].SetWindowText(strData);
	strData.Format("%0.2lf", pEquipData->dUnloadTrayPitchX); m_stcUnloadTrayData[2].SetWindowText(strData);
	strData.Format("%0.2lf", pEquipData->dUnloadTrayPitchY); m_stcUnloadTrayData[3].SetWindowText(strData);
	strData.Format("%0.2lf", pEquipData->dIndexPitch); m_stcIndexData[0].SetWindowText(strData);

	for (int i = 0; i < 4; i++) { strData.Format("%d", pEquipData->nVacOffDelay[i]); m_stcVacOffDelay[i].SetWindowText(strData); }
	for (int i = 0; i < 4; i++) { strData.Format("%d", pEquipData->nVacOffRepeat[i]); m_stcVacOffRepeat[i].SetWindowText(strData); }
	for (int i = 0; i < 6; i++) { strData.Format("%d", pEquipData->nDelayAdd[i]); m_stcDelayAdd[i].SetWindowText(strData); }
	strData.Format("%0.3lf", pEquipData->dAlignOffset); m_stcAlignOffset.SetWindowText(strData);

	m_ipaAviIp.SetWindowText(pEquipData->sAviIp);
	for (int i = 0; i < 6; i++) for (int j = 0; j < 4; j++) m_chkTower[i][j].SetCheck(pEquipData->bTower[i][j]);
	for (int i = 0; i < 5; i++) for (int j = 0; j < 6; j++) m_chkBuzzer[i][j].SetCheck(pEquipData->bBuzzer[i][j]);
	m_stcPasswordMt.SetWindowText(pEquipData->sPasswordMt);
	m_edtPasswordSi.SetWindowText(pEquipData->sPasswordSi);
	m_chkResultTestUse.SetCheck(pEquipData->bResultTestUse);
	strData.Format("%d", pEquipData->nResultTestNNg); m_edtResultTest[0].SetWindowText(strData);
	strData.Format("%d", pEquipData->nResultTestSNg); m_edtResultTest[1].SetWindowText(strData);
	strData.Format("%d", pEquipData->nResultTestS2Ng); m_edtResultTest[2].SetWindowText(strData);

	strData.Format("%0.3lf", pEquipData->dAssyPickForce[0]); m_stcLoadCellData[0].SetWindowText(strData);
	strData.Format("%0.3lf", pEquipData->dAssyPickForce[1]); m_stcLoadCellData[1].SetWindowText(strData);

	gData.nPickerUseCnt == 3 ? m_rdoPickCnt[0].SetCheck(TRUE) : (gData.nPickerUseCnt == 4 ? m_rdoPickCnt[1].SetCheck(TRUE) : m_rdoPickCnt[2].SetCheck(TRUE));
}

void CSetupEquipDlg::Save_EquipData()
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	CString strKey, strData;
	int nData;
	double dData;

 	CIniFileCS INI(gsCurrentDir + "\\System\\EquipData.ini");
	if (!INI.Check_File()) { AfxMessageBox("EquipData.ini File Not Found!!!"); return; }

	g_objCommon.Backup_File(gsCurrentDir + "\\System", "EquipData");

	m_stcEquipName.GetWindowText(strData); INI.Set_String("EQUIPMENT", "NAME", strData);
	nData = m_cboLotBarcodePort.GetCurSel(); INI.Set_Integer("EQUIPMENT", "LOT_BARCODE", nData + 1);
	nData = m_cboAssyLoadCellPort.GetCurSel(); INI.Set_Integer("EQUIPMENT", "ASSY_LOAD_CELL", nData + 8);
	nData = m_cboUnloadLoadCellPort.GetCurSel(); INI.Set_Integer("EQUIPMENT", "UNLOAD_LOAD_CELL", nData + 8);
	INI.Set_Bool("EQUIPMENT", "JOB_LIST_TAKT_LOG", m_chkJobListTaktLog.GetCheck());
	m_stcScreenOff.GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("EQUIPMENT", "SCREEN_OFF", nData);
	m_stcNoWorkTime.GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("EQUIPMENT", "NOWORK_TIME", nData);
	m_stcMotionCheck.GetWindowText(strData); dData = atof(strData); INI.Set_Double("EQUIPMENT", "MOTION_CHECK", dData, "%0.3lf");
	INI.Set_Bool("EQUIPMENT", "MANUAL_TAKT_TEST", m_chkManualTaktTest.GetCheck());
	INI.Set_Bool("EQUIPMENT", "DOOR_LOCK", m_rdoDoorLock[1].GetCheck());
	m_stcDoorLockTime.GetWindowText(strData);
	gData.nDoorLockTime = atoi(strData);
	INI.Set_Integer("EQUIPMENT", "DOOR_LOCK_TIME", gData.nDoorLockTime);

#ifndef DRY_RUN_TEST
	INI.Set_Bool("OPTION", "VISION_CM_ALIGN", m_chkUseVisionCmAlign.GetCheck());
	INI.Set_Bool("OPTION", "VISION_CAP_ALIGN", m_chkUseVisionCapAlign.GetCheck());
#endif
	INI.Set_Bool("OPTION", "INLINE_MODE", m_chkUseInlineMode.GetCheck());
	INI.Set_Bool("OPTION", "TRAY_PICKER_TURN", m_chkUseTrayPickerTurn.GetCheck());
	INI.Set_Bool("OPTION", "CAP_PICKER_TURN", m_chkUseCapPickerTurn.GetCheck());
	INI.Set_Bool("OPTION", "ASSY_VAC", m_chkUseAssyPickerVac.GetCheck());
	INI.Set_Bool("OPTION", "INDEX_ASSY_VAC", m_chkUseIndexAssyVac.GetCheck());
	INI.Set_Bool("OPTION", "ASSY_PICKER_TILT", m_chkUseAssyPickerTiltChk.GetCheck());
	INI.Set_Bool("OPTION", "VISION_ALIGN_ALARM", m_chkUseVisionAlignAlarm.GetCheck());
	INI.Set_Bool("OPTION", "VISION_ALIGN_OFFSET", m_chkUseVisionAlignOffset.GetCheck());
	INI.Set_Bool("OPTION", "LOAD_SORT_DIR_RIGHT", m_chkUseLoadTraySortDir.GetCheck());
	INI.Set_Bool("OPTION", "CAP_PICKUP_MULTI", m_chkCapPickUpMulti.GetCheck());
	INI.Set_Bool("OPTION", "SHIP_TRAY_ONE_EMPTY", m_chkShipTrayOneEmpty.GetCheck());
	INI.Set_Bool("OPTION", "MES_CAP_REG", m_chkUseMesCapReg.GetCheck());
	INI.Set_Bool("OPTION", "MES_SHIP_REG", m_chkUseMesShipReg.GetCheck());

	m_stcLoadTrayData[0].GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("LOAD_TRAY", "COUNT_X", nData);
	m_stcLoadTrayData[1].GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("LOAD_TRAY", "COUNT_Y", nData);
	m_stcLoadTrayData[2].GetWindowText(strData); dData = atof(strData); INI.Set_Double ("LOAD_TRAY", "PITCH_X", dData, "%0.2lf");
	m_stcLoadTrayData[3].GetWindowText(strData); dData = atof(strData); INI.Set_Double ("LOAD_TRAY", "PITCH_Y", dData, "%0.2lf");

	m_stcCapTrayData[0].GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("CAP_TRAY", "COUNT_X", nData);
	m_stcCapTrayData[1].GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("CAP_TRAY", "COUNT_Y", nData);
	m_stcCapTrayData[2].GetWindowText(strData); dData = atof(strData); INI.Set_Double ("CAP_TRAY", "PITCH_X", dData, "%0.2lf");
	m_stcCapTrayData[3].GetWindowText(strData); dData = atof(strData); INI.Set_Double ("CAP_TRAY", "PITCH_Y", dData, "%0.2lf");

	m_stcUnloadTrayData[0].GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("UNLOAD_TRAY", "COUNT_X", nData);
	m_stcUnloadTrayData[1].GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("UNLOAD_TRAY", "COUNT_Y", nData);
	m_stcUnloadTrayData[2].GetWindowText(strData); dData = atof(strData); INI.Set_Double ("UNLOAD_TRAY", "PITCH_X", dData, "%0.2lf");
	m_stcUnloadTrayData[3].GetWindowText(strData); dData = atof(strData); INI.Set_Double ("UNLOAD_TRAY", "PITCH_Y", dData, "%0.2lf");

	m_stcIndexData[0].GetWindowText(strData); dData = atof(strData); INI.Set_Double ("INDEX", "PITCH", dData, "%0.2lf");
	m_stcAlignOffset.GetWindowText(strData); dData = atof(strData); INI.Set_Double("VISION_ALIGN", "OFFSET", dData, "%0.3lf");

	for (int i = 0; i < 4; i++) {
		strKey.Format("%d", i); m_stcVacOffDelay[i].GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("VAC_OFF_DELAY", strKey, nData);
	
		if (i == 0) g_objMES.Save_AviRmsData("Cap Clean Blowing Time", strData);	
	}
	for (int i = 0; i < 4; i++) { strKey.Format("%d", i); m_stcVacOffRepeat[i].GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("VAC_OFF_REPEAT", strKey, nData); }
	for (int i = 0; i < 6; i++) { strKey.Format("%d", i); m_stcDelayAdd[i].GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("DELAY_ADD", strKey, nData); }
	for (int i = 0; i < 6; i++) for (int j = 0; j < 4; j++) { strKey.Format("%d%d", i, j); INI.Set_Bool("TOWER", strKey, m_chkTower[i][j].GetCheck()); }
	for (int i = 0; i < 5; i++) for (int j = 0; j < 6; j++) { strKey.Format("%d%d", i, j); INI.Set_Bool("BUZZER", strKey, m_chkBuzzer[i][j].GetCheck()); }

	m_stcPasswordMt.GetWindowText(strData); INI.Set_String("HIDDEN", "PASSWORD_MT", strData);
	m_edtPasswordSi.GetWindowText(strData); INI.Set_String("HIDDEN", "PASSWORD_SI", strData);

	pEquipData->bResultTestUse = m_chkResultTestUse.GetCheck();
	m_edtResultTest[0].GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("RESULT_TEST", "N-NG", nData);
	m_edtResultTest[1].GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("RESULT_TEST", "S-NG", nData);
	m_edtResultTest[2].GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("RESULT_TEST", "S2-NG", nData);

	m_stcLoadCellData[0].GetWindowText(strData); dData = atof(strData); INI.Set_Double("LOAD_CELL", "MIN", dData);
	m_stcLoadCellData[1].GetWindowText(strData); dData = atof(strData); INI.Set_Double("LOAD_CELL", "MAX", dData);

	m_ipaAviIp.GetWindowText(strData); INI.Set_String("AVI", "AVI_IP", strData);

	if (m_rdoPickCnt[0].GetCheck()) INI.Set_Integer("EQUIPMENT", "PICK_CNT", 3);
	if (m_rdoPickCnt[1].GetCheck()) INI.Set_Integer("EQUIPMENT", "PICK_CNT", 4);
	if (m_rdoPickCnt[2].GetCheck()) INI.Set_Integer("EQUIPMENT", "PICK_CNT", 5);

	g_objLogFile.Save_HandlerLog("[Setup Equip] Save Click");

	Cancel_EquipData();
}

void CSetupEquipDlg::Cancel_EquipData()
{
	if (m_nPreModel != -1) {
		CIniFileCS INI(gsCurrentDir + "\\System\\ModelData.ini");
		if (!INI.Check_File()) { AfxMessageBox("ModelData.ini File Not Found!!!"); return; }

		INI.Set_Integer("MODEL", "MODEL", m_nPreModel);
		m_nPreModel = -1;
	}
	g_objDataManager.Read_ModelData();
	g_objDataManager.Read_EquipData();
	g_objCommon.Read_CapShipPoatNo();

	Display_EquipData();

	g_objLogFile.Save_HandlerLog("[Setup Equip] Cancel Click");
}

///////////////////////////////////////////////////////////////////////////////


void CSetupEquipDlg::OnBnClickedBtnAviConnect()
{
	g_objAviUDP.Initialize();
}


void CSetupEquipDlg::OnBnClickedBtnAviDisconnect()
{
	g_objAviUDP.Terminate();
}
