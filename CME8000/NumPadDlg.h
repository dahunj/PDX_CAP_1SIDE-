// NumPadDlg.h : 헤더 파일
//
#pragma once

// CNumPadDlg 대화 상자입니다.

extern CString g_strReturnNumPad;

class CNumPadDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNumPadDlg)

public:
	CNumPadDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CNumPadDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_NUM_PAD_DLG };
	CLabelCS	m_Label[4];
	CStaticCS	m_stcOldValue;
	CStaticCS	m_stcNewValue;
	CButtonCS	m_btnClear;
	CStaticCS	m_stcNumAxisName;
	CStaticCS	m_stcNumAxisPos;
	CButtonCS	m_btnNum[10];
	CButtonCS	m_btnDot;
	CButtonCS	m_btnMinus;
	CButtonCS	m_btnOK;
	CButtonCS	m_btnCancel;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedBtnClear();
	afx_msg void OnBtnNumClick(UINT nID);	// IDC_BTN_NUM_X Click
	afx_msg void OnBnClickedBtnDot();
	afx_msg void OnBnClickedBtnMinus();
	afx_msg void OnBnClickedBtnOk();
	afx_msg void OnBnClickedBtnCancel();

private:
	CString		m_strOldValue;
	CString		m_strNewValue;
	CBrush		m_brushBack;
	int			m_nNumAxis;

private:
	void Initial_Controls();

public:
	void Set_OldValue(CString sOld, int nAxis = -1);
};

///////////////////////////////////////////////////////////////////////////////
