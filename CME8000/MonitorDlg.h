// MonitorDlg.h : 헤더 파일
//
#pragma once

#include "MonitorLogDlg.h"
#include "MonitorMotionDlg.h"
#include "MonitorInOutDlg.h"

// CMonitorDlg 대화 상자입니다.

class CMonitorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMonitorDlg)

public:
	CMonitorDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMonitorDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MONITOR_DLG };
	CPictureCS	m_picMonitorBack;
	CRadioCS	m_rdoMonitorLog;
	CRadioCS	m_rdoMonitorMotion;
	CRadioCS	m_rdoMonitorInOut;
	CButtonCS	m_btnMonitorClose;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedRdoMonitorLog();
	afx_msg void OnBnClickedRdoMonitorMotion();
	afx_msg void OnBnClickedRdoMonitorInOut();
	afx_msg void OnBnClickedBtnMonitorClose();

public:
	CMonitorLogDlg		*m_pMonitorLogDlg;
	CMonitorMotionDlg	*m_pMonitorMotionDlg;
	CMonitorInOutDlg	*m_pMonitorInOutDlg;

private:
	void Initial_Controls();
	void Hide_Windows();
};

extern CMonitorDlg g_dlgMonitor;

///////////////////////////////////////////////////////////////////////////////
