// MsgBoxDlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CME8000.h"
#include "MsgBoxDlg.h"
#include "afxdialogex.h"

#include "Common.h"
#include "NoWorkDlg.h"
#include "SequenceMain.h"

// CMsgBoxDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMsgBoxDlg, CDialogEx)

CMsgBoxDlg::CMsgBoxDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMsgBoxDlg::IDD, pParent)
{
}

CMsgBoxDlg::~CMsgBoxDlg()
{
}

void CMsgBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STC_OPER_ID, m_stcOperId);
	DDX_Control(pDX, IDC_STC_MSG, m_stcMsg);
	DDX_Control(pDX, IDC_BTN_ONE_OK, m_btnOneOK);
	DDX_Control(pDX, IDC_BTN_TWO_OK, m_btnTwoOK);
	DDX_Control(pDX, IDC_BTN_TWO_CANCEL, m_btnTwoCancel);
}

BEGIN_MESSAGE_MAP(CMsgBoxDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_ONE_OK, &CMsgBoxDlg::OnBnClickedBtnOneOk)
	ON_BN_CLICKED(IDC_BTN_TWO_OK, &CMsgBoxDlg::OnBnClickedBtnTwoOk)
	ON_BN_CLICKED(IDC_BTN_TWO_CANCEL, &CMsgBoxDlg::OnBnClickedBtnTwoCancel)
	ON_STN_CLICKED(IDC_STC_OPER_ID, &CMsgBoxDlg::OnStnClickedStcOperId)
END_MESSAGE_MAP()

// CMsgBoxDlg 메시지 처리기입니다.

BOOL CMsgBoxDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	Initial_Controls();

	if (m_nBtnCnt == 2) {
		m_btnOneOK.ShowWindow(SW_HIDE);
		m_btnTwoOK.ShowWindow(SW_SHOW);
		m_btnTwoCancel.Set_Text("Cancel"); m_btnTwoCancel.ShowWindow(SW_SHOW);
	} else if (m_nBtnCnt == 3) {				// 비가동 집계
		m_btnOneOK.ShowWindow(SW_HIDE);
		m_btnTwoOK.ShowWindow(SW_HIDE);
		m_stcOperId.ShowWindow(SW_SHOW); m_stcOperId.SetWindowText("Oper ID");
		m_btnTwoCancel.Set_Text("IDLE End"); m_btnTwoCancel.ShowWindow(SW_SHOW); 
	} else {
		m_btnOneOK.ShowWindow(SW_SHOW);
		m_btnTwoOK.ShowWindow(SW_HIDE);
		m_btnTwoCancel.ShowWindow(SW_HIDE);
	}
	m_stcMsg.SetWindowText(m_strMsg);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CMsgBoxDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

HBRUSH CMsgBoxDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	if (nCtlColor == CTLCOLOR_DLG)
		return (HBRUSH)m_brushBack;

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

void CMsgBoxDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow) 
	{
		SetTimer(0, 100, NULL);
	} 
	else 
	{
		KillTimer(0);
	}
}

void CMsgBoxDlg::OnTimer(UINT_PTR nIDEvent) 
{
	KillTimer(0);

	DX_DATA_12 *pDX12 = g_objAJinAXL.Get_pDX12();

	if (pDX12->iUnload2Sw) 
	{
		EndDialog(IDOK);
		return;
	}

	SetTimer(0, 100, NULL);
	CDialogEx::OnTimer(nIDEvent);
}

void CMsgBoxDlg::OnBnClickedBtnOneOk()
{
	EndDialog(IDOK);
}

void CMsgBoxDlg::OnBnClickedBtnTwoOk()
{
	EndDialog(IDOK);
}

void CMsgBoxDlg::OnBnClickedBtnTwoCancel()
{
	if (m_nBtnCnt == 3) {
		CString strTemp;
		m_stcOperId.GetWindowText(strTemp) ;
		if (strTemp == "Oper ID") { AfxMessageBox("Input the Operator ID."); return; }

		g_dlgNoWork.m_strOperId = strTemp;
		g_dlgNoWork.Set_NoWorkReport(FALSE);
		g_dlgNoWork.ShowWindow(SW_HIDE);
	}
	EndDialog(IDCANCEL);
	
}

///////////////////////////////////////////////////////////////////////////////
// User Functions

void CMsgBoxDlg::Initial_Controls() 
{
	m_brushBack.CreateSolidBrush(RGB(0x20, 0x00, 0x60));

	m_stcMsg.Init_Ctrl("바탕", 24, TRUE, RGB(0xFF, 0xFF, 0x00), RGB(0x80, 0x80, 0x80));
	m_stcOperId.Init_Ctrl("Segoe UI", 20, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xFF, 0xFF, 0xFF));

	m_btnOneOK.Init_Ctrl("바탕", 20, TRUE, RGB(0xFF, 0x00, 0xFF), COLOR_DEFAULT, IDI_OK, CButtonCS::emLeft);
	m_btnTwoOK.Init_Ctrl("바탕", 20, TRUE, RGB(0xFF, 0x00, 0xFF), COLOR_DEFAULT, IDI_OK, CButtonCS::emLeft);
	m_btnTwoCancel.Init_Ctrl("바탕", 20, TRUE, RGB(0x00, 0x00, 0xFF), COLOR_DEFAULT, IDI_CANCEL, CButtonCS::emLeft);
}

void CMsgBoxDlg::Set_Message(int nBtnCnt, CString strMsg) 
{
	m_nBtnCnt = nBtnCnt;
	m_strMsg = strMsg;
}

///////////////////////////////////////////////////////////////////////////////


void CMsgBoxDlg::OnStnClickedStcOperId()
{
	CString strKey;
	if (g_objCommon.Show_KeyPad(strKey) != IDOK) return;

	m_stcOperId.SetWindowText(strKey);
}
