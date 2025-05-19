// SetupMoveDlg.h : 헤더 파일
//
#pragma once

#include "SetupMoveTab1Dlg.h"
#include "SetupMoveTab2Dlg.h"
#include "SetupMoveTab3Dlg.h"

// CSetupMoveDlg 대화 상자입니다.

class CSetupMoveDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetupMoveDlg)

public:
	CSetupMoveDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupMoveDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SETUP_MOVE_DLG };
	CTabCS		m_tabSetupMove;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTcnSelchangingTabSetupMove(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnSelchangeTabSetupMove(NMHDR *pNMHDR, LRESULT *pResult);

public:
	CSetupMoveTab1Dlg *m_pSetupMoveTab1Dlg;
	CSetupMoveTab2Dlg *m_pSetupMoveTab2Dlg;
	CSetupMoveTab3Dlg *m_pSetupMoveTab3Dlg;

private:
	void Initial_Controls();

public:
	void Save_MoveData();
	void Cancel_MoveData();
};

///////////////////////////////////////////////////////////////////////////////
