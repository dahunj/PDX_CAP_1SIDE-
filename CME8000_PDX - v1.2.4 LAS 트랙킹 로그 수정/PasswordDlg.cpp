// PasswordDlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CME8000.h"
#include "PasswordDlg.h"
#include "afxdialogex.h"

#include "DataManager.h"

// CPasswordDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPasswordDlg, CDialogEx)

int g_nLoginUser;	// 0:None, 1:MT, 2:SI

CPasswordDlg::CPasswordDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPasswordDlg::IDD, pParent)
{
}

CPasswordDlg::~CPasswordDlg()
{
}

void CPasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDT_PASSWORD, m_edtPassword);
	for (int i = 0; i < 10; i++) DDX_Control(pDX, IDC_BTN_PASS_NUM_0 + i, m_btnPassNum[i]);
	DDX_Control(pDX, IDC_BTN_OK, m_btnOK);
	DDX_Control(pDX, IDC_BTN_CANCEL, m_btnCancel);
}

BEGIN_MESSAGE_MAP(CPasswordDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_PASS_NUM_0, IDC_BTN_PASS_NUM_9, OnBtnPassNumClick)
	ON_BN_CLICKED(IDC_BTN_OK, &CPasswordDlg::OnBnClickedBtnOk)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &CPasswordDlg::OnBnClickedBtnCancel)
END_MESSAGE_MAP()

// CPasswordDlg 메시지 처리기입니다.

BOOL CPasswordDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	Initial_Controls();

	m_edtPassword.SetWindowText("");

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CPasswordDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_ESCAPE) return TRUE;
		if (pMsg->wParam == VK_RETURN) { OnBnClickedBtnOk(); return TRUE; }
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

HBRUSH CPasswordDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	if (nCtlColor == CTLCOLOR_DLG)
		return (HBRUSH)m_brushBack;

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

void CPasswordDlg::OnBtnPassNumClick(UINT nID)
{
	int ID = nID - IDC_BTN_PASS_NUM_0;

	CString strPassword, strNum;
	m_edtPassword.GetWindowText(strPassword);
	m_btnPassNum[ID].GetWindowText(strNum);
	m_edtPassword.SetWindowText(strPassword + strNum);
}

void CPasswordDlg::OnBnClickedBtnOk()
{
// 분/시/일
// 	SYSTEMTIME time;
// 	GetLocalTime(&time);
// 	strPassSi.Format("%02d%02d%02d", time.wMinute, time.wHour, time.wDay);

	CString strInput;
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	m_edtPassword.GetWindowText(strInput);
	if (strInput == pEquipData->sPasswordMt) {
		g_nLoginUser = 1;	// MT
		EndDialog(IDOK);
	} else if (strInput == pEquipData->sPasswordSi) {
		g_nLoginUser = 2;	// SI
		EndDialog(IDOK);
	} else {
		g_nLoginUser = 0;	// None
		m_edtPassword.SetWindowText("");
	}
}

void CPasswordDlg::OnBnClickedBtnCancel()
{
	g_nLoginUser = 0;
	EndDialog(IDCANCEL);
}

///////////////////////////////////////////////////////////////////////////////
// User Functions

void CPasswordDlg::Initial_Controls() 
{
	m_brushBack.CreateSolidBrush(RGB(0x70, 0xD0, 0xF0));

	m_edtPassword.Init_Ctrl("바탕", 14, TRUE, RGB(0x00, 0x00, 0xFF), RGB(0xFF, 0xFF, 0xB0));

	for (int i = 0; i < 10; i++) m_btnPassNum[i].Init_Ctrl("바탕", 20, TRUE, RGB(0xB0, 0x50, 0x00), COLOR_DEFAULT, 0, 0);

	m_btnOK.Init_Ctrl("바탕", 20, TRUE, RGB(0xFF, 0x00, 0xFF), COLOR_DEFAULT, IDI_OK, CButtonCS::emLeft);
	m_btnCancel.Init_Ctrl("바탕", 20, TRUE, RGB(0x00, 0x00, 0xFF), COLOR_DEFAULT, IDI_CANCEL, CButtonCS::emLeft);
}

///////////////////////////////////////////////////////////////////////////////
