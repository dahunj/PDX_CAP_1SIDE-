#pragma once

// CVersionDlg 대화 상자입니다.

class CVersionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVersionDlg)

public:
	CVersionDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CVersionDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_VERSION_DLG };
	CStaticCS	m_stcSummary;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

private:
	void Initial_Controls();
	char* UTF8ToANSI(const char *pszCode);
};

extern CVersionDlg g_dlgVersion;
