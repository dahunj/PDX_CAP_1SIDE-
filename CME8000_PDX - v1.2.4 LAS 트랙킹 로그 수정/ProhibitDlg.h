// ProhibitDlg.h : 헤더 파일
//
#pragma once

// CProhibitDlg 대화 상자입니다.

class CProhibitDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProhibitDlg)

public:
	CProhibitDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CProhibitDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PROHIBIT_DLG };
	CStaticCS	m_stcProhibitTitle;
	CStaticCS	m_stcProhibitMessage;
	CGroupCS	m_Group0;
	CLabelCS	m_Label0;
	CRadioCS	m_rdoMaintOpt[2];
	CEditCS		m_edtMaintLog;
	CButtonCS	m_btnMaintSave;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedBtnMaintSave();

private:
	CBrush	m_brushBack;
	void Initial_Controls();
};

extern CProhibitDlg g_dlgProhibit;

///////////////////////////////////////////////////////////////////////////////
