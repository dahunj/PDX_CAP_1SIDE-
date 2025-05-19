// ProhibitDlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CME8000.h"
#include "ProhibitDlg.h"
#include "afxdialogex.h"

#include "CME8000Dlg.h"
#include "LogFile.h"

// CProhibitDlg 대화 상자입니다.

CProhibitDlg g_dlgProhibit;

IMPLEMENT_DYNAMIC(CProhibitDlg, CDialogEx)

CProhibitDlg::CProhibitDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProhibitDlg::IDD, pParent)
{
}

CProhibitDlg::~CProhibitDlg()
{
}

void CProhibitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STC_PROHIBIT_TITLE, m_stcProhibitTitle);
	DDX_Control(pDX, IDC_STC_PROHIBIT_MESSAGE, m_stcProhibitMessage);
	DDX_Control(pDX, IDC_GROUP_0, m_Group0);
	DDX_Control(pDX, IDC_LABEL_0, m_Label0);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_RDO_MAINT_OPT_0 + i, m_rdoMaintOpt[i]);
	DDX_Control(pDX, IDC_EDT_MAINT_LOG, m_edtMaintLog);
	DDX_Control(pDX, IDC_BTN_MAINT_SAVE, m_btnMaintSave);
}

BEGIN_MESSAGE_MAP(CProhibitDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_MAINT_SAVE, &CProhibitDlg::OnBnClickedBtnMaintSave)
END_MESSAGE_MAP()

// CProhibitDlg 메시지 처리기입니다.

BOOL CProhibitDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(this, 0, 75, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	Initial_Controls();

	m_rdoMaintOpt[0].SetCheck(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CProhibitDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	if (pMsg->message == WM_MOUSEMOVE) {
		CCME8000Dlg *pMainDlg = (CCME8000Dlg*)AfxGetApp()->GetMainWnd();
		pMainDlg->Set_ScreenOn();
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

HBRUSH CProhibitDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	if (nCtlColor == CTLCOLOR_DLG)
		return (HBRUSH)m_brushBack;

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

///////////////////////////////////////////////////////////////////////////////
// User Functions

void CProhibitDlg::Initial_Controls() 
{
	m_brushBack.CreateSolidBrush(RGB(0x00, 0x00, 0x00));

	m_stcProhibitTitle.Init_Ctrl("바탕", 40, TRUE, RGB(0xFF, 0xFF, 0x00), RGB(0x00, 0x00, 0x00));
	m_stcProhibitMessage.Init_Ctrl("바탕", 30, FALSE, RGB(0xFF, 0x00, 0x00), RGB(0x00, 0x00, 0x00));
	m_Group0.Init_Ctrl("바탕", 15, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x00, 0x00));
	m_Label0.Init_Ctrl("바탕", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x00, 0x00));
	for (int i = 0; i < 2; i++) m_rdoMaintOpt[i].Init_Ctrl("바탕", 11, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x00, 0x00), CCheckCS::emRed, 0);
	m_edtMaintLog.Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, COLOR_DEFAULT);
	m_btnMaintSave.Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
}

void CProhibitDlg::OnBnClickedBtnMaintSave()
{
	CString strText, strLog;
	m_edtMaintLog.GetWindowText(strText);
	CString strOpt = (m_rdoMaintOpt[1].GetCheck() ? "설비점검 완료" : "설비점검 시작");
	strLog.Format("[%s] %s", strOpt, strText);
	g_objLogFile.Save_HandlerLog(strLog);
}

///////////////////////////////////////////////////////////////////////////////
