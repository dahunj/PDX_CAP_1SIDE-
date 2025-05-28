// SetupMotionTabDlg.h : 헤더 파일
//
#pragma once

// CSetupMotionTabDlg 대화 상자입니다.

class CSetupMotionTabDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetupMotionTabDlg)

public:
	CSetupMotionTabDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupMotionTabDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SETUP_MOTION_TAB_DLG };
	CGroupCS	m_Group[10];
	CLabelCS	m_Label[62];
	CStaticCS	m_stcAxisName[8];
	CStaticCS	m_stcAxisPos[8];
	CStaticCS	m_stcAxisVel[8];
	CLedRectCS	m_ledAxisSOn[8];
	CLedRectCS	m_ledAxisOrg[8];
	CLedRectCS	m_ledAxisElp[8];
	CLedRectCS	m_ledAxisEln[8];
	CLedRectCS	m_ledAxisAlm[8];
	CLedRectCS	m_ledAxisInP[8];
	CLedRectCS	m_ledAxisRun[8];
	CLedRectCS	m_ledAxisHom[8];
	CCheckCS	m_chkSOn[8];
	CButtonCS	m_btnReset[8];
	CButtonCS	m_btnHome[8];
	CButtonCS	m_btnStop[8];
	CButtonCS	m_btnJogP[8];
	CButtonCS	m_btnJogN[8];
	CStaticCS	m_stcAbsDist[8];
	CButtonCS	m_btnAbsMove[8];
	CStaticCS	m_stcRelDist[8];
	CButtonCS	m_btnRelMoveP[8];
	CButtonCS	m_btnRelMoveN[8];
	CStaticCS	m_stcSpeedM[8];
	CStaticCS	m_stcSpeedJ[8];
	CStaticCS	m_stcAccel[8];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnChkSOnClick(UINT nID);				// IDC_CHK_S_ON_X Click
	afx_msg void OnBtnResetClick(UINT nID);				// IDC_BTN_RESET_X Click
	afx_msg void OnBtnHomeClick(UINT nID);				// IDC_BTN_HOME_X Click
	afx_msg void OnBtnStopClick(UINT nID);				// IDC_BTN_STOP_X Click
	afx_msg LRESULT OnCsLButtonDown(WPARAM wParam, LPARAM lParam);	// CS Button Down
	afx_msg LRESULT OnCsLButtonUp(WPARAM wParam, LPARAM lParam);	// CS Button Up
	afx_msg void OnStcAbsDistClick(UINT nID);			// IDC_STC_ABS_DIST_X Click
	afx_msg void OnBtnAbsMoveClick(UINT nID);			// IDC_BTN_ABS_MOVE_X Click
	afx_msg void OnStcRelDistClick(UINT nID);			// IDC_STC_REL_DIST_X Click
	afx_msg void OnBtnRelMovePClick(UINT nID);			// IDC_BTN_REL_MOVE_P_X Click
	afx_msg void OnBtnRelMoveNClick(UINT nID);			// IDC_BTN_REL_MOVE_N_X Click
	afx_msg void OnStcSpeedMClick(UINT nID);			// IDC_STC_SPEED_M_X Click
	afx_msg void OnStcSpeedJClick(UINT nID);			// IDC_STC_SPEED_J_X Click
	afx_msg void OnStcAccelClick(UINT nID);				// IDC_STC_ACCEL_X Click

private:
	int		m_nMotionTab;
	CString	m_strLog;

	void Initial_Controls();

public:
	void Set_MotionTab(int nTab) { m_nMotionTab = nTab; }
	void Display_Status();
	void Save_MotionData(int nTab);
	void Cancel_MotionData(int nTab);

	BOOL Check_Interlock(int AxisNo);
};

///////////////////////////////////////////////////////////////////////////////
