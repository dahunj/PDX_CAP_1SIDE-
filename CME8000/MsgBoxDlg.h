// MsgBoxDlg.h : 헤더 파일
//
#pragma once

// CMsgBoxDlg 대화 상자입니다.

class CMsgBoxDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMsgBoxDlg)

public:
	CMsgBoxDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMsgBoxDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MSG_BOX_DLG };
	CStaticCS	m_stcOperId;
	CStaticCS	m_stcMsg;
	CButtonCS	m_btnOneOK;
	CButtonCS	m_btnTwoOK;
	CButtonCS	m_btnTwoCancel;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnOneOk();
	afx_msg void OnBnClickedBtnTwoOk();
	afx_msg void OnBnClickedBtnTwoCancel();
	afx_msg void OnStnClickedStcOperId();

private:
	CBrush		m_brushBack;
	int			m_nBtnCnt;
	CString		m_strMsg;

	void Initial_Controls();

public:
	void Set_Message(int nBtnCnt, CString strMsg);
};

///////////////////////////////////////////////////////////////////////////////
