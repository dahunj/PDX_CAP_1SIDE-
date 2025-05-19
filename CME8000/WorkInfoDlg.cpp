// WorkInfoDlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CME8000.h"
#include "WorkInfoDlg.h"
#include "afxdialogex.h"

#include "AJinAXL.h"

// CWorkInfoDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CWorkInfoDlg, CDialogEx)

CWorkInfoDlg::CWorkInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWorkInfoDlg::IDD, pParent)
{
}

CWorkInfoDlg::~CWorkInfoDlg()
{
}

void CWorkInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for (int i = 0; i < 3; i++) DDX_Control(pDX, IDC_GROUP_0 + i, m_Group[i]);
	for (int i = 0; i < 6; i++) DDX_Control(pDX, IDC_LABEL_0 + i, m_Label[i]);
	for (int i = 0; i < 6; i++) DDX_Control(pDX, IDC_LED_LOAD_PICK_INFO_0 + i,	m_ledLoadPickInfo[i]);
	for (int i = 0; i < 6; i++) DDX_Control(pDX, IDC_LED_ASSY_PICK_INFO_0 + i, m_ledAssyPickInfo[i]);
	for (int i = 0; i < 6; i++) DDX_Control(pDX, IDC_LED_CAP_PICK_INFO_0 + i, m_ledCapPickInfo[i]);
	for (int i = 0; i < 6; i++) DDX_Control(pDX, IDC_LED_UNLOAD_PICK_INFO_0 + i, m_ledUnloadPickInfo[i]);
	for (int i = 0; i < 6; i++) DDX_Control(pDX, IDC_LED_CAP_BUFFER_INFO_0 + i, m_ledCapBufferInfo[i]);
	for (int i = 0; i < 6; i++) DDX_Control(pDX, IDC_LED_TRANS_STAGE_INFO_0 + i, m_ledTransStageInfo[i]);
	for (int i = 0; i < 5; i++) DDX_Control(pDX, IDC_LED_VIS_SCAN_DONE_0 + i, m_ledVisScanDone[i]);
}

BEGIN_MESSAGE_MAP(CWorkInfoDlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CWorkInfoDlg 메시지 처리기입니다.

BOOL CWorkInfoDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(this, 30, 180, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	Initial_Controls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CWorkInfoDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CWorkInfoDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow) SetTimer(0, 100, NULL);
	else KillTimer(0);
}

void CWorkInfoDlg::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(0);

	Display_Status();

	SetTimer(0, 100, NULL);
	CDialogEx::OnTimer(nIDEvent);
}

///////////////////////////////////////////////////////////////////////////////
// User Functions

void CWorkInfoDlg::Initial_Controls() 
{
	for (int i = 0; i < 3; i++) m_Group[i].Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT);
	for (int i = 0; i < 6; i++) m_Label[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT);
	for (int i = 0; i < 6; i++) m_ledLoadPickInfo[i].Init_Ctrl(RGB(0x00, 0xFF, 0x00), RGB(0x40, 0x40, 0x40));
	for (int i = 0; i < 6; i++) m_ledCapPickInfo[i].Init_Ctrl(RGB(0x00, 0xFF, 0x00), RGB(0x40, 0x40, 0x40));
	for (int i = 0; i < 6; i++) m_ledAssyPickInfo[i].Init_Ctrl(RGB(0x00, 0xFF, 0x00), RGB(0x40, 0x40, 0x40));
	for (int i = 0; i < 6; i++) m_ledUnloadPickInfo[i].Init_Ctrl(RGB(0x00, 0xFF, 0x00), RGB(0x40, 0x40, 0x40));
	for (int i = 0; i < 6; i++) m_ledCapBufferInfo[i].Init_Ctrl(RGB(0x00, 0xFF, 0x00), RGB(0x40, 0x40, 0x40));
	for (int i = 0; i < 6; i++) m_ledTransStageInfo[i].Init_Ctrl(RGB(0x00, 0xFF, 0x00), RGB(0x40, 0x40, 0x40));
	for (int i = 0; i < 2; i++) m_ledVisScanDone[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
}

void CWorkInfoDlg::Display_Status()
{
	for (int i = 0; i < 6; i++) gData.InfoLoadPick[i]	> 0 ? m_ledLoadPickInfo[i].On()		: m_ledLoadPickInfo[i].Off();
	for (int i = 0; i < 6; i++) gData.InfoCapPick[i]	> 0 ? m_ledCapPickInfo[i].On()		: m_ledCapPickInfo[i].Off();
	for (int i = 0; i < 6; i++) gData.InfoAssyPick[i]	> 0 ? m_ledAssyPickInfo[i].On()		: m_ledAssyPickInfo[i].Off();
	for (int i = 0; i < 6; i++) gData.InfoUnloadPick[i] > 0 ? m_ledUnloadPickInfo[i].On()	: m_ledUnloadPickInfo[i].Off();
	for (int i = 0; i < 6; i++) gData.InfoCapBuffer[i]  > 0 ? m_ledCapBufferInfo[i].On()	: m_ledCapBufferInfo[i].Off();
	for (int i = 0; i < 6; i++) gData.InfoTransStage[i] > 0 ? m_ledTransStageInfo[i].On()	: m_ledTransStageInfo[i].Off();
	for (int i = 0; i < 2; i++) m_ledVisScanDone[i].Set_On(gData.bScanDone[i]);
}

///////////////////////////////////////////////////////////////////////////////
