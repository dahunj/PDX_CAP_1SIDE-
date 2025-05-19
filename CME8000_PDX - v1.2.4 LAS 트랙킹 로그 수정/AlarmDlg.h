// AlarmDlg.h : 헤더 파일
//
#pragma once

// CAlarmDlg 대화 상자입니다.

class CAlarmDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAlarmDlg)

public:
	CAlarmDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAlarmDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ALARM_DLG };
	CStaticCS	m_stcAlmMsg;
	CButtonCS	m_btnAlmBuzzOff;
	CButtonCS	m_btnAlmOK;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedBtnAlmBuzzOff();
	afx_msg void OnBnClickedBtnAlmOk();

private:
	CBrush		m_brushBack;
	CString		m_strMsg;
	int			m_nAlarmLoop;
	int			m_nState;

	void Initial_Controls();

public:
	void Set_AlarmMessage(CString sMsg) { m_strMsg = sMsg; }
	void Set_AlarmState(int nState) { m_nState = nState; }
};

extern CAlarmDlg g_dlgAlarm;

///////////////////////////////////////////////////////////////////////////////
