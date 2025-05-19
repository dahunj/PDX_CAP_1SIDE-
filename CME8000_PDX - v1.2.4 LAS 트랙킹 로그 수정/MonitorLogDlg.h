// MonitorLogDlg.h : 헤더 파일
//
#pragma once

// CMonitorLogDlg 대화 상자입니다.

class CMonitorLogDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMonitorLogDlg)

public:
	CMonitorLogDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMonitorLogDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MONITOR_LOG_DLG };
	CGroupCS		m_Group[2];
	CLabelCS		m_Label;
	CRichEditCtrl	m_redtLogDisplay;
	CStaticCS		m_stcLogDate;
	CButtonCS		m_btnGoToday;
	CButtonCS		m_btnPrintList;
	CMonthCalCtrl	m_calMonth;
	CRadioCS		m_rdoLogAlarm;
	CRadioCS		m_rdoLogJobList;
	CRadioCS		m_rdoLogHandler;
	CRadioCS		m_rdoLogInspector;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedBtnGoToday();
	afx_msg void OnMcnSelectCalMonth(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedRdoLogAlarm();
	afx_msg void OnBnClickedRdoLogJobList();
	afx_msg void OnBnClickedRdoLogHandler();
	afx_msg void OnBnClickedRdoLogInspector();

private:
	CFont	m_fontCalendar;
	//CFont	m_fontREdit;

private:
	void Initial_Controls();		

protected:
	void Initial_RichEdit();
	void Read_LogFile();
};

///////////////////////////////////////////////////////////////////////////////
