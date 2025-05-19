// NoWorkDlg.h : 헤더 파일
//
#pragma once

#include "afxcmn.h"

// CNoWorkDlg 대화 상자입니다.

// 비가동 집계를 위한 클래스
class CNoWorkDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNoWorkDlg)

public:
	CNoWorkDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CNoWorkDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_NOWORK_DLG };
	CLabelCS	m_LabelTitle;
	CGroupCS	m_group[2];
	CRadioCS	m_rdoStopReason[16];
	CLabelCS	m_lblOperId;
	CStaticCS	m_stcOperId;
	CLabelCS	m_lblTime[3];
	CStaticCS	m_stcTime[3];
	CStaticCS	m_stcExit;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnStnClickedStcOperId();
	afx_msg void OnBtnClickedStopLoss(UINT nID);
	afx_msg void OnStnClickedStcExit();

private:
	CString		m_strCode;
	CString		m_strTxt;
	CString		m_strStopArray[4][3];
	CString		m_strLossArray[12][3];
	
public:
	CString		m_strOperId;
	BOOL		m_bStart;
	DWORD		m_dwStartTime;
	CString		m_strStartTime;

private:
	BOOL Initial_Controls();
	BOOL Read_StopLossList();

public:
	void Set_NoWorkReport(BOOL bSet);
};

extern CNoWorkDlg g_dlgNoWork;
