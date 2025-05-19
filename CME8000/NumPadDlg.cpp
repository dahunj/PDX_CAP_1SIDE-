// NumPadDlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CME8000.h"
#include "NumPadDlg.h"
#include "afxdialogex.h"

#include "AJinAXL.h"

// CNumPadDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CNumPadDlg, CDialogEx)

CString g_strReturnNumPad;

CNumPadDlg::CNumPadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNumPadDlg::IDD, pParent)
{
}

CNumPadDlg::~CNumPadDlg()
{
}

void CNumPadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_LABEL_0 + i, m_Label[i]);
	DDX_Control(pDX, IDC_STC_OLD_VALUE, m_stcOldValue);
	DDX_Control(pDX, IDC_STC_NEW_VALUE, m_stcNewValue);
	DDX_Control(pDX, IDC_BTN_CLEAR, m_btnClear);
	DDX_Control(pDX, IDC_STC_NUM_AXIS_NAME, m_stcNumAxisName);
	DDX_Control(pDX, IDC_STC_NUM_AXIS_POS, m_stcNumAxisPos);
	for (int i = 0; i < 10; i++) DDX_Control(pDX, IDC_BTN_NUM_0 + i, m_btnNum[i]);
	DDX_Control(pDX, IDC_BTN_DOT, m_btnDot);
	DDX_Control(pDX, IDC_BTN_MINUS, m_btnMinus);
	DDX_Control(pDX, IDC_BTN_OK, m_btnOK);
	DDX_Control(pDX, IDC_BTN_CANCEL, m_btnCancel);
}

BEGIN_MESSAGE_MAP(CNumPadDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CNumPadDlg::OnBnClickedBtnClear)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_NUM_0, IDC_BTN_NUM_9, OnBtnNumClick)
	ON_BN_CLICKED(IDC_BTN_DOT, &CNumPadDlg::OnBnClickedBtnDot)
	ON_BN_CLICKED(IDC_BTN_MINUS, &CNumPadDlg::OnBnClickedBtnMinus)
	ON_BN_CLICKED(IDC_BTN_OK, &CNumPadDlg::OnBnClickedBtnOk)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &CNumPadDlg::OnBnClickedBtnCancel)
END_MESSAGE_MAP()

// CNumPadDlg 메시지 처리기입니다.

BOOL CNumPadDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	Initial_Controls();

	g_strReturnNumPad = "";

	m_stcOldValue.SetWindowText(m_strOldValue);
	m_stcNewValue.SetWindowText("0");

	if (m_nNumAxis == -1) {
		for (int i = 0; i < 2; i++) m_Label[i + 2].ShowWindow(FALSE);
		m_stcNumAxisName.ShowWindow(FALSE);
		m_stcNumAxisPos.ShowWindow(FALSE);

	} else {
		CString strText = g_objAJinAXL.Get_AxisName(m_nNumAxis);
		m_stcNumAxisName.SetWindowText(strText);

		double dPos = g_objAJinAXL.Get_Position(m_nNumAxis);
		strText.Format("%0.3lf", dPos);
		m_stcNumAxisPos.SetWindowText(strText);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CNumPadDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

HBRUSH CNumPadDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	if (nCtlColor == CTLCOLOR_DLG)
		return (HBRUSH)m_brushBack;

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

void CNumPadDlg::OnBnClickedBtnClear()
{
	m_stcNewValue.SetWindowText("0");
}

void CNumPadDlg::OnBtnNumClick(UINT nID)
{
	int ID = nID - IDC_BTN_NUM_0;

	CString strNew, strNum;
	m_stcNewValue.GetWindowText(strNew);
	m_btnNum[ID].GetWindowText(strNum);
	if (strNew == "0") strNew = "";
	m_stcNewValue.SetWindowText(strNew + strNum);
}

void CNumPadDlg::OnBnClickedBtnDot()
{
	CString strNew;
	m_stcNewValue.GetWindowText(strNew);
	if (strNew.Find(".") > -1) return;
	m_stcNewValue.SetWindowText(strNew + ".");
}

void CNumPadDlg::OnBnClickedBtnMinus()
{
	CString strNew;
	m_stcNewValue.GetWindowText(strNew);
	if (strNew == "0") m_stcNewValue.SetWindowText("-");
}

void CNumPadDlg::OnBnClickedBtnOk()
{
	CString strNew;
	m_stcNewValue.GetWindowText(strNew);
	g_strReturnNumPad = strNew;
	EndDialog(IDOK);
}

void CNumPadDlg::OnBnClickedBtnCancel()
{
	EndDialog(IDCANCEL);
}

///////////////////////////////////////////////////////////////////////////////
// User Functions

void CNumPadDlg::Initial_Controls() 
{
	for (int i = 0; i < 2; i++) m_Label[i].Init_Ctrl("바탕", 14, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x60, 0x60, 0x60));
	for (int i = 2; i < 4; i++) m_Label[i].Init_Ctrl("바탕", 10, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x60, 0x60, 0x60));

	m_brushBack.CreateSolidBrush(RGB(0x90, 0x80, 0xC0));

	m_stcOldValue.Init_Ctrl("바탕", 18, TRUE, RGB(0x00, 0x00, 0xFF), RGB(0xFF, 0xFF, 0xB0));
	m_stcNewValue.Init_Ctrl("바탕", 18, TRUE, RGB(0xFF, 0x00, 0x00), RGB(0xFF, 0xFF, 0xB0));
	m_btnClear.Init_Ctrl("바탕", 15, TRUE, RGB(0x00, 0x80, 0x00), COLOR_DEFAULT, 0, 0);

	m_stcNumAxisName.Init_Ctrl("Arial", 9, FALSE, COLOR_DEFAULT, RGB(0xE0, 0xE0, 0xE0));
	m_stcNumAxisPos.Init_Ctrl("Arial", 11, TRUE, COLOR_DEFAULT, RGB(0xE0, 0xE0, 0xE0));

	for (int i = 0; i < 10; i++) m_btnNum[i].Init_Ctrl("바탕", 20, TRUE, RGB(0xB0, 0x50, 0x00), COLOR_DEFAULT, 0, 0);
	m_btnDot.Init_Ctrl("바탕", 20, TRUE, RGB(0xB0, 0x50, 0x00), COLOR_DEFAULT, 0, 0);
	m_btnMinus.Init_Ctrl("바탕", 20, TRUE, RGB(0xB0, 0x50, 0x00), COLOR_DEFAULT, 0, 0);

	m_btnOK.Init_Ctrl("바탕", 20, TRUE, RGB(0xFF, 0x00, 0xFF), COLOR_DEFAULT, IDI_OK, CButtonCS::emLeft);
	m_btnCancel.Init_Ctrl("바탕", 20, TRUE, RGB(0x00, 0x00, 0xFF), COLOR_DEFAULT, IDI_CANCEL, CButtonCS::emLeft);
}

void CNumPadDlg::Set_OldValue(CString sOld, int nAxis) 
{
	m_strOldValue = sOld;
	m_nNumAxis = nAxis;
}

///////////////////////////////////////////////////////////////////////////////
