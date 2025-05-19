// SetupInOutDlg.h : 헤더 파일
//
#pragma once

#include "SetupInOutTabDlg.h"

// CSetupInOutDlg 대화 상자입니다.

class CSetupInOutDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetupInOutDlg)

public:
	CSetupInOutDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupInOutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SETUP_IN_OUT_DLG };
	CTabCS		m_tabSetupInOut;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTcnSelchangingTabSetupInOut(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnSelchangeTabSetupInOut(NMHDR *pNMHDR, LRESULT *pResult);

public:
	CSetupInOutTabDlg *m_pSetupInOutTabDlg;

private:
	void Initial_Controls();

public:
	void Display_Status();
};

///////////////////////////////////////////////////////////////////////////////
