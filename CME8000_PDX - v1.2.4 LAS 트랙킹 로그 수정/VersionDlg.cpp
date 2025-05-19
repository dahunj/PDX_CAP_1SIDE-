// VersionDlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CME8000.h"
#include "VersionDlg.h"
#include "afxdialogex.h"

// CVersionDlg 대화 상자입니다.
CVersionDlg g_dlgVersion;

IMPLEMENT_DYNAMIC(CVersionDlg, CDialogEx)

CVersionDlg::CVersionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVersionDlg::IDD, pParent)
{
}

CVersionDlg::~CVersionDlg()
{
}

void CVersionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STC_SUMMARY, m_stcSummary);
}

BEGIN_MESSAGE_MAP(CVersionDlg, CDialogEx)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

// CVersionDlg 메시지 처리기입니다.

BOOL CVersionDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(this, 1344, 80, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	Initial_Controls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CVersionDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CVersionDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (!bShow) return;

	CIniFileCS INI(gsCurrentDir + "\\System\\Version.ini");
	if (!INI.Check_File()) { AfxMessageBox("Version.ini File Not Found!!!"); return; }

	CString strKey, strRead, strData, strText = "\n ";
	for (int i = 0; i < 9; i++) {
		strKey.Format("%d", i + 1);
		strRead = INI.Get_String(MAIN_VERSION, strKey, "");
		strData.Empty();
		strData.Insert(0, UTF8ToANSI(strRead));
		if (strData == "") break;
		
		if (i != 0) strText += "\n\n ";
		strText += strKey + ". ";
		strText += strData;
	}

	m_stcSummary.SetWindowText(strText);
}

///////////////////////////////////////////////////////////////////////////////
// User Functions

void CVersionDlg::Initial_Controls()
{
	m_stcSummary.Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xFF, 0xFF));
}

char* CVersionDlg::UTF8ToANSI(const char *pszCode)
{
	BSTR    bstrWide;
	char*   pszAnsi;
	int     nLength;

	nLength = MultiByteToWideChar(CP_UTF8, 0, pszCode, lstrlen(pszCode) + 1, NULL, NULL);
	bstrWide = SysAllocStringLen(NULL, nLength);

	MultiByteToWideChar(CP_UTF8, 0, pszCode, lstrlen(pszCode) + 1, bstrWide, nLength);

	nLength = WideCharToMultiByte(CP_ACP, 0, bstrWide, -1, NULL, 0, NULL, NULL);
	pszAnsi = new char[nLength];

	WideCharToMultiByte(CP_ACP, 0, bstrWide, -1, pszAnsi, nLength, NULL, NULL);
	SysFreeString(bstrWide);

	return pszAnsi;
}

///////////////////////////////////////////////////////////////////////////////
