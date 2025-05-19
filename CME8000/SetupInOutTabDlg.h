// SetupInOutTabDlg.h : 헤더 파일
//
#pragma once

// CSetupInOutTabDlg 대화 상자입니다.

class CSetupInOutTabDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetupInOutTabDlg)

public:
	CSetupInOutTabDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupInOutTabDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SETUP_IN_OUT_TAB_DLG };
	CGroupCS	m_Group[2];
	CStaticCS	m_stcIn[64];
	CCheckCS	m_chkOut[64];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnChkOutClick(UINT nID);	// IDC_CHK_OUT_XX Click

private:
	int		m_nInOutTab;
	CString m_strLog;

	void Initial_Controls();
	void Set_CheckBox(int nIdx, BOOL bChk);
	void Set_Output(int nIdx, BOOL bOut);

public:
	void Set_InOutTab(int nTab) { m_nInOutTab = nTab; }
	void Display_Status();
};

///////////////////////////////////////////////////////////////////////////////
