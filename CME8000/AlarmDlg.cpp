// AlarmDlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CME8000.h"
#include "AlarmDlg.h"
#include "afxdialogex.h"

#include "LogFile.h"
#include "SequenceInit.h"
#include "CME8000Dlg.h"

// CAlarmDlg 대화 상자입니다.
CAlarmDlg g_dlgAlarm;

IMPLEMENT_DYNAMIC(CAlarmDlg, CDialogEx)

CAlarmDlg::CAlarmDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAlarmDlg::IDD, pParent)
{
}

CAlarmDlg::~CAlarmDlg()
{
}

void CAlarmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STC_ALM_MSG, m_stcAlmMsg);
	DDX_Control(pDX, IDC_BTN_ALM_BUZZ_OFF, m_btnAlmBuzzOff);
	DDX_Control(pDX, IDC_BTN_ALM_OK, m_btnAlmOK);
}

BEGIN_MESSAGE_MAP(CAlarmDlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_ALM_BUZZ_OFF, &CAlarmDlg::OnBnClickedBtnAlmBuzzOff)
	ON_BN_CLICKED(IDC_BTN_ALM_OK, &CAlarmDlg::OnBnClickedBtnAlmOk)
END_MESSAGE_MAP()

// CAlarmDlg 메시지 처리기입니다.

BOOL CAlarmDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	Initial_Controls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CAlarmDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CAlarmDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	CCME8000Dlg *pMainDlg = (CCME8000Dlg*)AfxGetApp()->GetMainWnd();
	if (bShow) {
		CString strLog;
		strLog.Format("[Alarm Mode] %s", m_strMsg);
		strLog.Replace("\n", " ");
		g_objLogFile.Save_HandlerLog(strLog);

		m_stcAlmMsg.SetWindowText(m_strMsg);
		//pMainDlg->Set_CurrentState(STATE_ALARM);
		pMainDlg->Set_CurrentState(m_nState);
		
	} else {
		m_strMsg = "";
		pMainDlg->Set_BuzzerFlicker(FALSE);
		if (g_objSequenceInit.Get_InitComplete()) pMainDlg->Set_CurrentState(STATE_STOP);
		else pMainDlg->Set_CurrentState(STATE_NONE);

		g_objLogFile.Save_HandlerLog("[Alarm Mode] Close Alarm");
	}
}

HBRUSH CAlarmDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	if (nCtlColor == CTLCOLOR_DLG)
		return (HBRUSH)m_brushBack;

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

void CAlarmDlg::OnBnClickedBtnAlmBuzzOff()
{
	CCME8000Dlg *pMainDlg = (CCME8000Dlg*)AfxGetApp()->GetMainWnd();
	pMainDlg->Set_BuzzerFlicker(FALSE);
}

void CAlarmDlg::OnBnClickedBtnAlmOk()
{
	ShowWindow(SW_HIDE);
}

///////////////////////////////////////////////////////////////////////////////
// User Functions

void CAlarmDlg::Initial_Controls() 
{
	m_brushBack.CreateSolidBrush(RGB(0xF0, 0x80, 0x00));
	
	m_stcAlmMsg.Init_Ctrl("바탕", 20, TRUE, RGB(0xFF, 0xFF, 0x00), RGB(0x80, 0x80, 0x80));

	m_btnAlmBuzzOff.Init_Ctrl("바탕", 20, TRUE, RGB(0x00, 0x00, 0xFF), COLOR_DEFAULT,0, 0);
	m_btnAlmOK.Init_Ctrl("바탕", 20, TRUE, RGB(0xFF, 0x00, 0xFF), COLOR_DEFAULT, IDI_OK, CButtonCS::emLeft);
}

///////////////////////////////////////////////////////////////////////////////
