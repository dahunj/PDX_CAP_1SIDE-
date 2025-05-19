// InitialDlg.h : 헤더 파일
//
#pragma once

// CInitialDlg 대화 상자입니다.

class CInitialDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInitialDlg)

public:
	CInitialDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CInitialDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_INITIAL_DLG };
	CGroupCS	m_Group[6];
	CLabelCS	m_Label[6];
	CStaticCS	m_stcAxisName[46];
	CLedRectCS	m_ledAxisRun[46];
	CLedRectCS	m_ledAxisHom[46];
	CRadioCS	m_rdoInitStart;
	CRadioCS	m_rdoInitStop;
	CLedCS		m_ledInitialOK;
	CPictureCS	m_Image[3];
	CPictureCS	m_picInitFlow[8];
	CStaticCS	m_stcInitFlow[8];
	CLedCS		m_ledMainAir[2];
	CLedCS		m_ledEmgSw[5];
	CLedCS		m_ledDoorOpen[13];
	CStaticCS	m_stcInitCase[14];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedRdoInitStart();
	afx_msg void OnBnClickedRdoInitStop();
	afx_msg void OnBnClickedChkMotionTest();
	afx_msg void OnBnClickedButton1();

private:
	CBitmap	m_bmpImage[3];
	int		m_nBackColorLoop;
	BOOL	m_bInitialRunning;

	void Initial_Controls();
	void Display_Status();
	void Display_Initial();
	void Set_StatusColor(int nIdx, int nColor);	// nColor : 0:White,1:Red,2:Green,3:Blue
};

extern CInitialDlg g_dlgInitial;

///////////////////////////////////////////////////////////////////////////////
