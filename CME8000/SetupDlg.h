// SetupDlg.h : 헤더 파일
//
#pragma once

#include "SetupEquipDlg.h"
#include "SetupMoveDlg.h"
#include "SetupMotionDlg.h"
#include "SetupInOutDlg.h"

// CSetupDlg 대화 상자입니다.

class CSetupDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetupDlg)

public:
	CSetupDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SETUP_DLG };
	CPictureCS	m_picSetupBack;
	CRadioCS	m_rdoSetupEquip;
	CRadioCS	m_rdoSetupMove;
	CRadioCS	m_rdoSetupMotion;
	CRadioCS	m_rdoSetupInOut;
	CButtonCS	m_btnSetupSave;
	CButtonCS	m_btnSetupCancel;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedRdoSetupEquip();
	afx_msg void OnBnClickedRdoSetupMove();
	afx_msg void OnBnClickedRdoSetupMotion();
	afx_msg void OnBnClickedRdoSetupInOut();
	afx_msg void OnBnClickedBtnSetupSave();
	afx_msg void OnBnClickedBtnSetupCancel();

public:
	CSetupEquipDlg	*m_pSetupEquipDlg;
	CSetupMoveDlg	*m_pSetupMoveDlg;
	CSetupMotionDlg	*m_pSetupMotionDlg;
	CSetupInOutDlg	*m_pSetupInOutDlg;

private:
	void Initial_Controls();
	void Hide_Windows();

private:
	int m_nLoginUser;

public:
	void Set_LoginUser(int nUser) { m_nLoginUser = nUser; }
	int Get_LoginUser() { return m_nLoginUser; }
};

extern CSetupDlg g_dlgSetup;

///////////////////////////////////////////////////////////////////////////////
