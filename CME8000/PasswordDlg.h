// PasswordDlg.h : 헤더 파일
//
#pragma once

// CPasswordDlg 대화 상자입니다.

extern int g_nLoginUser;	// 0:None, 1:MT, 2:SI

class CPasswordDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPasswordDlg)

public:
	CPasswordDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPasswordDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PASSWORD_DLG };
	CEditCS		m_edtPassword;
	CButtonCS	m_btnPassNum[10];
	CButtonCS	m_btnOK;
	CButtonCS	m_btnCancel;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBtnPassNumClick(UINT nID);	// IDC_BTN_NUM_X Click
	afx_msg void OnBnClickedBtnOk();
	afx_msg void OnBnClickedBtnCancel();

private:
	CBrush	m_brushBack;

private:
	void Initial_Controls();
};

///////////////////////////////////////////////////////////////////////////////
