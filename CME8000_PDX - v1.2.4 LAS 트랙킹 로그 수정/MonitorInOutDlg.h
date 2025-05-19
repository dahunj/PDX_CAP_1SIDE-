// MonitorInOutDlg.h : 헤더 파일
//
#pragma once

#include "MonitorInOutTabDlg.h"

// CMonitorInOutDlg 대화 상자입니다.

class CMonitorInOutDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMonitorInOutDlg)

public:
	CMonitorInOutDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMonitorInOutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MONITOR_IN_OUT_DLG };
	CTabCS		m_tabMonitorInOut;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTcnSelchangingTabMonitorInOut(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnSelchangeTabMonitorInOut(NMHDR *pNMHDR, LRESULT *pResult);

public:
	CMonitorInOutTabDlg *m_pMonitorInOutTabDlg;

private:
	void Initial_Controls();

public:
	void Display_Status();
};

///////////////////////////////////////////////////////////////////////////////
