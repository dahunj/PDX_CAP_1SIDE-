// MonitorInOutTabDlg.h : 헤더 파일
//
#pragma once

// CMonitorInOutTabDlg 대화 상자입니다.

class CMonitorInOutTabDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMonitorInOutTabDlg)

public:
	CMonitorInOutTabDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMonitorInOutTabDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MONITOR_IN_OUT_TAB_DLG };
	CGroupCS	m_Group[2];
	CLedCS		m_ledIn[32];
	CLedCS		m_ledOut[32];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

private:
	int		m_nInOutTab;
	CString m_strLog;

	void Initial_Controls();

public:
	void Set_InOutTab(int nTab) { m_nInOutTab = nTab; }
	void Display_Status();
};

///////////////////////////////////////////////////////////////////////////////
