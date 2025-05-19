// ManualDlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CME8000.h"
#include "ManualDlg.h"
#include "afxdialogex.h"

#include "LogFile.h"
#include "Common.h"
#include "DataManager.h"

// CManualDlg 대화 상자입니다.

CManualDlg g_dlgManual;

IMPLEMENT_DYNAMIC(CManualDlg, CDialogEx)

CManualDlg::CManualDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CManualDlg::IDD, pParent)
{
}

CManualDlg::~CManualDlg()
{
}

void CManualDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PIC_MANUAL_BACK, m_picManualBack);
	DDX_Control(pDX, IDC_RDO_MANUAL_LOAD, m_rdoManualLoad);
	DDX_Control(pDX, IDC_RDO_MANUAL_INDEX, m_rdoManualBtm1);
	DDX_Control(pDX, IDC_RDO_MANUAL_CAP, m_rdoManualCap);
	DDX_Control(pDX, IDC_RDO_MANUAL_UNLOAD, m_rdoManualUnload);
	DDX_Control(pDX, IDC_RDO_MANUAL_DOOR_LOCK, m_rdoManualDoorLock);
	DDX_Control(pDX, IDC_RDO_MANUAL_DOOR_UNLOCK, m_rdoManualDoorUnlock);
}

BEGIN_MESSAGE_MAP(CManualDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RDO_MANUAL_LOAD, &CManualDlg::OnBnClickedRdoManualLoad)
	ON_BN_CLICKED(IDC_RDO_MANUAL_INDEX, &CManualDlg::OnBnClickedRdoManualIndex)
	ON_BN_CLICKED(IDC_RDO_MANUAL_CAP, &CManualDlg::OnBnClickedRdoManualCap)
	ON_BN_CLICKED(IDC_RDO_MANUAL_UNLOAD, &CManualDlg::OnBnClickedRdoManualUnload)
	ON_BN_CLICKED(IDC_RDO_MANUAL_DOOR_LOCK, &CManualDlg::OnBnClickedRdoManualDoorLock)
	ON_BN_CLICKED(IDC_RDO_MANUAL_DOOR_UNLOCK, &CManualDlg::OnBnClickedRdoManualDoorUnlock)
END_MESSAGE_MAP()

// CManualDlg 메시지 처리기입니다.

BOOL CManualDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(this, 0, 75, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	Initial_Controls();

	m_pManualLoadDlg = new CManualLoadDlg(this);
	m_pManualLoadDlg->Create(IDD_MANUAL_LOAD_DLG, this);

	m_pManualIndexDlg = new CManualIndexDlg(this);
	m_pManualIndexDlg->Create(IDD_MANUAL_INDEX_DLG, this);

	m_pManualCapDlg = new CManualCapDlg(this);
	m_pManualCapDlg->Create(IDD_MANUAL_CAP_DLG, this);

	m_pManualUnloadDlg = new CManualUnloadDlg(this);
	m_pManualUnloadDlg->Create(IDD_MANUAL_UNLOAD_DLG, this);

	// Load Dlg Visible
	m_rdoManualLoad.SetCheck(TRUE);
	m_rdoManualLoad.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CManualDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CManualDlg::OnDestroy() 
{
	CDialogEx::OnDestroy();

	m_pManualUnloadDlg->DestroyWindow();
	m_pManualCapDlg->DestroyWindow();
	m_pManualIndexDlg->DestroyWindow();
	m_pManualLoadDlg->DestroyWindow();

	if (m_pManualUnloadDlg) delete m_pManualUnloadDlg;
	if (m_pManualCapDlg) delete m_pManualCapDlg;
	if (m_pManualIndexDlg) delete m_pManualIndexDlg;
	if (m_pManualLoadDlg) delete m_pManualLoadDlg;
	
	m_pManualUnloadDlg = NULL;
	m_pManualCapDlg = NULL;
	m_pManualIndexDlg = NULL;
	m_pManualLoadDlg = NULL;
}

void CManualDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow) {
		if (m_rdoManualLoad.GetCheck()) m_pManualLoadDlg->ShowWindow(SW_SHOW);
		if (m_rdoManualBtm1.GetCheck()) m_pManualIndexDlg->ShowWindow(SW_SHOW);
		if (m_rdoManualCap.GetCheck()) m_pManualCapDlg->ShowWindow(SW_SHOW);
		if (m_rdoManualUnload.GetCheck()) m_pManualUnloadDlg->ShowWindow(SW_SHOW);

		EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
		if (pEquipData->bUseDoorLock) {
			g_objCommon.Locking_MainDoor(TRUE);
			m_rdoManualDoorLock.SetCheck(TRUE);
			m_rdoManualDoorLock.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);
			m_rdoManualDoorUnlock.Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
		} else {
			g_objCommon.Locking_MainDoor(FALSE);
			m_rdoManualDoorUnlock.SetCheck(TRUE);
			m_rdoManualDoorUnlock.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);
			m_rdoManualDoorLock.Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
		}

		SetTimer(0, 100, NULL);

	} else {
		KillTimer(0);

		g_objCommon.Locking_MainDoor(FALSE);

		if (m_rdoManualLoad.GetCheck()) m_pManualLoadDlg->ShowWindow(SW_HIDE);
		if (m_rdoManualBtm1.GetCheck()) m_pManualIndexDlg->ShowWindow(SW_HIDE);
		if (m_rdoManualCap.GetCheck()) m_pManualCapDlg->ShowWindow(SW_HIDE);
		if (m_rdoManualUnload.GetCheck()) m_pManualUnloadDlg->ShowWindow(SW_HIDE);
	}
}

void CManualDlg::OnTimer(UINT_PTR nIDEvent) 
{
	KillTimer(0);

	int nState = theApp.Get_MainState();
	if (nState != STATE_ERROR) g_objCommon.Check_MainEmgAir();	// Emg & Main Air

	if (m_pManualLoadDlg->IsWindowVisible()) m_pManualLoadDlg->Display_Status();
	else if (m_pManualIndexDlg->IsWindowVisible()) m_pManualIndexDlg->Display_Status();
	else if (m_pManualCapDlg->IsWindowVisible()) m_pManualCapDlg->Display_Status();
	else if (m_pManualUnloadDlg->IsWindowVisible()) m_pManualUnloadDlg->Display_Status();

	SetTimer(0, 100, NULL);
	CDialogEx::OnTimer(nIDEvent);
}

void CManualDlg::OnBnClickedRdoManualLoad()
{
	if (m_pManualLoadDlg->IsWindowVisible()) return;
	Hide_Windows();
	g_objLogFile.Save_HandlerLog("[Manual - Load] Start");
	m_rdoManualLoad.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);
	m_pManualLoadDlg->ShowWindow(SW_SHOW);
}

void CManualDlg::OnBnClickedRdoManualIndex()
{
	if (m_pManualIndexDlg->IsWindowVisible()) return;
	Hide_Windows();
	g_objLogFile.Save_HandlerLog("[Manual - Index] Start");
	m_rdoManualBtm1.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);
	m_pManualIndexDlg->ShowWindow(SW_SHOW);
}

void CManualDlg::OnBnClickedRdoManualCap()
{
	if (m_pManualCapDlg->IsWindowVisible()) return;
	Hide_Windows();
	g_objLogFile.Save_HandlerLog("[Manual - Trans] Start");
	m_rdoManualCap.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);
	m_pManualCapDlg->ShowWindow(SW_SHOW);
}

void CManualDlg::OnBnClickedRdoManualUnload()
{
	if (m_pManualUnloadDlg->IsWindowVisible()) return;
	Hide_Windows();
	g_objLogFile.Save_HandlerLog("[Manual - Unload] Start");
	m_rdoManualUnload.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);
	m_pManualUnloadDlg->ShowWindow(SW_SHOW);
}

void CManualDlg::OnBnClickedRdoManualDoorLock()
{
	if (!m_rdoManualDoorLock.GetCheck()) return;
	m_rdoManualDoorLock.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);
	m_rdoManualDoorUnlock.Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
	g_objCommon.Locking_MainDoor(TRUE);
}

void CManualDlg::OnBnClickedRdoManualDoorUnlock()
{
	if (!m_rdoManualDoorUnlock.GetCheck()) return;
	m_rdoManualDoorUnlock.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);
	m_rdoManualDoorLock.Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
	g_objCommon.Locking_MainDoor(FALSE);
}

///////////////////////////////////////////////////////////////////////////////
// User Functions

void CManualDlg::Initial_Controls() 
{
	m_picManualBack.Init_Ctrl(COLOR_DEFAULT, RGB(0xA0, 0xF0, 0xF0));
	
	m_rdoManualLoad.Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	m_rdoManualBtm1.Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	m_rdoManualCap.Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	m_rdoManualUnload.Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	m_rdoManualDoorLock.Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	m_rdoManualDoorUnlock.Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
}

void CManualDlg::Hide_Windows()
{
	m_pManualLoadDlg->ShowWindow(SW_HIDE);
	m_pManualIndexDlg->ShowWindow(SW_HIDE);
	m_pManualCapDlg->ShowWindow(SW_HIDE);
	m_pManualUnloadDlg->ShowWindow(SW_HIDE);

	m_rdoManualLoad.Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
	m_rdoManualBtm1.Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
	m_rdoManualCap.Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
	m_rdoManualUnload.Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
}

void CManualDlg::Set_ManualPos(int nPos)
{
	m_rdoManualLoad.Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
	m_rdoManualBtm1.Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
	m_rdoManualCap.Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
	m_rdoManualUnload.Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);

	if (nPos == 1) { m_rdoManualLoad.SetCheck(TRUE);    m_rdoManualLoad.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT); }
	if (nPos == 2) { m_rdoManualBtm1.SetCheck(TRUE);   m_rdoManualBtm1.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT); }
	if (nPos == 3) { m_rdoManualCap.SetCheck(TRUE);   m_rdoManualCap.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT); }
	if (nPos == 4) { m_rdoManualUnload.SetCheck(TRUE);  m_rdoManualUnload.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT); }
}

///////////////////////////////////////////////////////////////////////////////
