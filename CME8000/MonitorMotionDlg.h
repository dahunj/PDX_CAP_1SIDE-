// MonitorMotionDlg.h : 헤더 파일
//
#pragma once

#include "MonitorMotionTabDlg.h"

// CMonitorMotionDlg 대화 상자입니다.

class CMonitorMotionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMonitorMotionDlg)

public:
	CMonitorMotionDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMonitorMotionDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MONITOR_MOTION_DLG };
	CTabCS		m_tabMonitorMotion;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTcnSelchangingTabMonitorMotion(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnSelchangeTabMonitorMotion(NMHDR *pNMHDR, LRESULT *pResult);

public:
	CMonitorMotionTabDlg *m_pMonitorMotionTabDlg;

private:
	void Initial_Controls();

public:
	void Display_Status();
};

///////////////////////////////////////////////////////////////////////////////
