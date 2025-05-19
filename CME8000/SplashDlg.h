// SplashDlg.h : 헤더 파일
//
#pragma once

// CSplashDlg 대화 상자입니다.

class CSplashDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSplashDlg)

public:
	CSplashDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSplashDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SPLASH_DLG };
	CLabelCS	m_Label[4];
	CPictureCS	m_Image[2];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

private:
	CBitmap	m_bmpImage[2];

private:
	void Initial_Controls();
};

///////////////////////////////////////////////////////////////////////////////
