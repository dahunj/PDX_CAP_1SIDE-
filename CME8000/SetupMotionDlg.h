// SetupMotionDlg.h : 헤더 파일
//
#pragma once

#include "SetupMotionTabDlg.h"

// CSetupMotionDlg 대화 상자입니다.

class CSetupMotionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetupMotionDlg)

public:
	CSetupMotionDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupMotionDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SETUP_MOTION_DLG };
	CTabCS		m_tabSetupMotion;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTcnSelchangingTabSetupMotion(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnSelchangeTabSetupMotion(NMHDR *pNMHDR, LRESULT *pResult);

public:
	CSetupMotionTabDlg *m_pSetupMotionTabDlg;

private:
	void Initial_Controls();

public:
	void Display_Status();
	void Save_MotionData();
	void Cancel_MotionData();
};

///////////////////////////////////////////////////////////////////////////////
