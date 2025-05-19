// SetupDlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CME8000.h"
#include "SetupDlg.h"
#include "afxdialogex.h"

#include "LogFile.h"
#include "AJinAXL.h"
#include "Common.h"

// CSetupDlg 대화 상자입니다.

CSetupDlg g_dlgSetup;

IMPLEMENT_DYNAMIC(CSetupDlg, CDialogEx)

CSetupDlg::CSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetupDlg::IDD, pParent)
{
	m_nLoginUser = 0;
}

CSetupDlg::~CSetupDlg()
{
}

void CSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PIC_SETUP_BACK, m_picSetupBack);
	DDX_Control(pDX, IDC_RDO_SETUP_EQUIP, m_rdoSetupEquip);
	DDX_Control(pDX, IDC_RDO_SETUP_MOVE, m_rdoSetupMove);
	DDX_Control(pDX, IDC_RDO_SETUP_MOTION, m_rdoSetupMotion);
	DDX_Control(pDX, IDC_RDO_SETUP_IN_OUT, m_rdoSetupInOut);
	DDX_Control(pDX, IDC_BTN_SETUP_SAVE, m_btnSetupSave);
	DDX_Control(pDX, IDC_BTN_SETUP_CANCEL, m_btnSetupCancel);
}

BEGIN_MESSAGE_MAP(CSetupDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RDO_SETUP_EQUIP, &CSetupDlg::OnBnClickedRdoSetupEquip)
	ON_BN_CLICKED(IDC_RDO_SETUP_MOVE, &CSetupDlg::OnBnClickedRdoSetupMove)
	ON_BN_CLICKED(IDC_RDO_SETUP_MOTION, &CSetupDlg::OnBnClickedRdoSetupMotion)
	ON_BN_CLICKED(IDC_RDO_SETUP_IN_OUT, &CSetupDlg::OnBnClickedRdoSetupInOut)
	ON_BN_CLICKED(IDC_BTN_SETUP_SAVE, &CSetupDlg::OnBnClickedBtnSetupSave)
	ON_BN_CLICKED(IDC_BTN_SETUP_CANCEL, &CSetupDlg::OnBnClickedBtnSetupCancel)
END_MESSAGE_MAP()

// CSetupDlg 메시지 처리기입니다.

BOOL CSetupDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(this, 0, 75, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	Initial_Controls();

	m_pSetupEquipDlg = new CSetupEquipDlg(this);
	m_pSetupEquipDlg->Create(IDD_SETUP_EQUIP_DLG, this);

	m_pSetupMoveDlg = new CSetupMoveDlg(this);
	m_pSetupMoveDlg->Create(IDD_SETUP_MOVE_DLG, this);

	m_pSetupMotionDlg = new CSetupMotionDlg(this);
	m_pSetupMotionDlg->Create(IDD_SETUP_MOTION_DLG, this);

	m_pSetupInOutDlg = new CSetupInOutDlg(this);
	m_pSetupInOutDlg->Create(IDD_SETUP_IN_OUT_DLG, this);

	m_rdoSetupEquip.SetCheck(TRUE);
	m_rdoSetupEquip.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CSetupDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CSetupDlg::OnDestroy() 
{
	CDialogEx::OnDestroy();

	m_pSetupInOutDlg->DestroyWindow();
	m_pSetupMotionDlg->DestroyWindow();
	m_pSetupMoveDlg->DestroyWindow();
	m_pSetupEquipDlg->DestroyWindow();

	if (m_pSetupInOutDlg) delete m_pSetupInOutDlg;
	if (m_pSetupMotionDlg) delete m_pSetupMotionDlg;
	if (m_pSetupMoveDlg) delete m_pSetupMoveDlg;
	if (m_pSetupEquipDlg) delete m_pSetupEquipDlg;

	m_pSetupInOutDlg = NULL;
	m_pSetupMotionDlg = NULL;
	m_pSetupMoveDlg = NULL;
	m_pSetupEquipDlg = NULL;
}

void CSetupDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow) {
		if (theApp.bParamMode) {
			m_rdoSetupMove.SetCheck(TRUE);
			OnBnClickedRdoSetupMove();

		} else {
			if (m_rdoSetupEquip.GetCheck()) m_pSetupEquipDlg->ShowWindow(SW_SHOW);
			if (m_rdoSetupMove.GetCheck()) m_pSetupMoveDlg->ShowWindow(SW_SHOW);
			if (m_rdoSetupMotion.GetCheck()) m_pSetupMotionDlg->ShowWindow(SW_SHOW);
			if (m_rdoSetupInOut.GetCheck()) m_pSetupInOutDlg->ShowWindow(SW_SHOW);
		}

		g_objAJinAXL.Set_ReadVelocity(TRUE);

		SetTimer(0, 100, NULL);

	} else {
		KillTimer(0);

		g_objAJinAXL.Set_ReadVelocity(FALSE);

		if (m_rdoSetupEquip.GetCheck()) m_pSetupEquipDlg->ShowWindow(SW_HIDE);
		if (m_rdoSetupMove.GetCheck()) m_pSetupMoveDlg->ShowWindow(SW_HIDE);
		if (m_rdoSetupMotion.GetCheck()) m_pSetupMotionDlg->ShowWindow(SW_HIDE);
		if (m_rdoSetupInOut.GetCheck()) m_pSetupInOutDlg->ShowWindow(SW_HIDE);
	}
}

void CSetupDlg::OnTimer(UINT_PTR nIDEvent) 
{
	KillTimer(0);

	int nState = theApp.Get_MainState();
	if (nState != STATE_ERROR) g_objCommon.Check_MainEmgAir();	// Emg & Main Air

	if (m_pSetupMotionDlg->IsWindowVisible()) m_pSetupMotionDlg->Display_Status();
	if (m_pSetupInOutDlg->IsWindowVisible()) m_pSetupInOutDlg->Display_Status();

	SetTimer(0, 100, NULL);
	CDialogEx::OnTimer(nIDEvent);
}

void CSetupDlg::OnBnClickedRdoSetupEquip()
{
	if (m_pSetupEquipDlg->IsWindowVisible()) return;
	Hide_Windows();
	m_btnSetupSave.EnableWindow(TRUE);
	m_btnSetupCancel.EnableWindow(TRUE);
	g_objLogFile.Save_HandlerLog("[Setup - Equip] Start");
	m_rdoSetupEquip.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);
	m_pSetupEquipDlg->ShowWindow(SW_SHOW);
}

void CSetupDlg::OnBnClickedRdoSetupMove()
{
	if (m_pSetupMoveDlg->IsWindowVisible()) return;
	Hide_Windows();
	m_btnSetupSave.EnableWindow(TRUE);
	m_btnSetupCancel.EnableWindow(TRUE);
	g_objLogFile.Save_HandlerLog("[Setup - Move] Start");
	m_rdoSetupMove.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);
	m_pSetupMoveDlg->ShowWindow(SW_SHOW);
}

void CSetupDlg::OnBnClickedRdoSetupMotion()
{
	if (m_pSetupMotionDlg->IsWindowVisible()) return;
	Hide_Windows();
	m_btnSetupSave.EnableWindow(TRUE);
	m_btnSetupCancel.EnableWindow(TRUE);
	g_objLogFile.Save_HandlerLog("[Setup - Motion] Start");
	m_rdoSetupMotion.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);
	m_pSetupMotionDlg->ShowWindow(SW_SHOW);
}

void CSetupDlg::OnBnClickedRdoSetupInOut()
{
	if (m_pSetupInOutDlg->IsWindowVisible()) return;
	Hide_Windows();
	m_btnSetupSave.EnableWindow(FALSE);
	m_btnSetupCancel.EnableWindow(FALSE);
	g_objLogFile.Save_HandlerLog("[Setup - InOut] Start");
	m_rdoSetupInOut.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);
	m_pSetupInOutDlg->ShowWindow(SW_SHOW);
}

void CSetupDlg::OnBnClickedBtnSetupSave()
{
	if (m_pSetupEquipDlg->IsWindowVisible()) m_pSetupEquipDlg->Save_EquipData();
	if (m_pSetupMoveDlg->IsWindowVisible()) m_pSetupMoveDlg->Save_MoveData();
	if (m_pSetupMotionDlg->IsWindowVisible()) m_pSetupMotionDlg->Save_MotionData();
}

void CSetupDlg::OnBnClickedBtnSetupCancel()
{
	if (m_pSetupEquipDlg->IsWindowVisible()) m_pSetupEquipDlg->Cancel_EquipData();
	if (m_pSetupMoveDlg->IsWindowVisible()) m_pSetupMoveDlg->Cancel_MoveData();
	if (m_pSetupMotionDlg->IsWindowVisible()) m_pSetupMotionDlg->Cancel_MotionData();
}

///////////////////////////////////////////////////////////////////////////////
// User Functions

void CSetupDlg::Initial_Controls() 
{
	m_picSetupBack.Init_Ctrl(RGB(0x00, 0x00, 0x00), RGB(0xF0, 0xE0, 0xF0));
	m_rdoSetupEquip.Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0x00, 0x00), COLOR_DEFAULT, 0, 0);
	m_rdoSetupMove.Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0x00, 0x00), COLOR_DEFAULT, 0, 0);
	m_rdoSetupMotion.Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0x00, 0x00), COLOR_DEFAULT, IDI_MOTION, CRadioCS::emLeft);
	m_rdoSetupInOut.Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0x00, 0x00), COLOR_DEFAULT, IDI_IN_OUT, CRadioCS::emLeft);
	m_btnSetupSave.Init_Ctrl("바탕", 16, TRUE, RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT, 0, 0);
	m_btnSetupCancel.Init_Ctrl("바탕", 16, TRUE, RGB(0x00, 0x00, 0xFF), COLOR_DEFAULT, 0, 0);
}

void CSetupDlg::Hide_Windows()
{
	m_pSetupEquipDlg->ShowWindow(SW_HIDE);
	m_pSetupMoveDlg->ShowWindow(SW_HIDE);
	m_pSetupMotionDlg->ShowWindow(SW_HIDE);
	m_pSetupInOutDlg->ShowWindow(SW_HIDE);

	m_rdoSetupEquip.Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
	m_rdoSetupMove.Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
	m_rdoSetupMotion.Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
	m_rdoSetupInOut.Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
}

///////////////////////////////////////////////////////////////////////////////
