#pragma once


// COperatorDlg 대화 상자입니다.

// OperatorDlg.h : 헤더 파일
//
class COperatorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COperatorDlg)

public:
	COperatorDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~COperatorDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_OPERATOR_DLG };
	CLabelCS	m_Label[16];
	CPictureCS	m_picOperator[4];
	CStaticCS	m_stcOperAlmTime[3];
	CStaticCS	m_stcOperAlmList[3];
	CStaticCS	m_stcOperLotId;
	CStaticCS	m_stcOperOperId;
	CStaticCS	m_stcOperModel;
	CStaticCS	m_stcOperResult[3];
	CStaticCS	m_stcOperDoor[2];
	CStaticCS	m_stcOperCmCount;
	CStaticCS	m_stcOperTrayCount;
	CStaticCS	m_stcOperLoadTray;
	CStaticCS	m_stcOperGoodTray;
	CStaticCS	m_stcOperNNgTray;
	CStaticCS	m_stcOperSNgTray;
	CGridCS		m_grdOperLoadTray;
	CGridCS		m_grdOperCapTray;
	CGridCS		m_grdOperUnloadTray;
	CButtonCS	m_btnOperStart;
	CButtonCS	m_btnOperStop;
	CButtonCS	m_btnOperCycleStop;
	CButtonCS	m_btnOperTest;
	CButtonCS	m_btnOperInitial;
	CButtonCS	m_btnOperWork;
	CButtonCS	m_btnOperManual;
	CButtonCS	m_btnOperSetup;
	CButtonCS	m_btnOperMonitor;
	CButtonCS	m_btnOperExit;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnStnClickedStcOperLotId();
	afx_msg void OnStnClickedStcOperOperId();
	afx_msg void OnStnClickedStcOperCmCount();
	afx_msg void OnBnClickedBtnOperInitial();
	afx_msg void OnBnClickedBtnOperWork();
	afx_msg void OnBnClickedBtnOperManual();
	afx_msg void OnBnClickedBtnOperSetup();
	afx_msg void OnBnClickedBtnOperMonitor();
	afx_msg void OnBnClickedBtnOperExit();
	afx_msg void OnBnClickedBtnOperStart();
	afx_msg void OnBnClickedBtnOperStop();
	afx_msg void OnBnClickedBtnOperCycleStop();
	afx_msg void OnBnClickedBtnOperEngineer();
	afx_msg void OnBnClickedBtnOperTest();

private:
	void Initial_Controls();
	void Initial_Grid(CGridCS *pGrid, int nRows, int nCols, int nNoDir=0);
	void Enable_OpModeButton(BOOL bEnable);
	void Display_Status();

public:
	void Update_TrayInfo(int nTray, int nNo=0);
	void Set_GridModelChange();
};

extern COperatorDlg g_dlgOperator;

///////////////////////////////////////////////////////////////////////////////
