// SetupMoveTab1Dlg.h : 헤더 파일
//
#pragma once

// CSetupMoveTab1Dlg 대화 상자입니다.

class CSetupMoveTab1Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetupMoveTab1Dlg)

public:
	CSetupMoveTab1Dlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupMoveTab1Dlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SETUP_MOVE_TAB1_DLG };
	CGroupCS	m_Group[10];
	CLabelCS	m_Label[35];
	CStaticCS	m_stcTrayPickerX[3];
	CStaticCS	m_stcTrayPickerZ[3];
	CStaticCS	m_stcTrayPickerR[2];
	CStaticCS	m_stcLoadStage1X[4];
	CStaticCS	m_stcLoadStage1Z[5];
	CStaticCS	m_stcLoadStage2X[4];
	CStaticCS	m_stcLoadStage2Z[5];
	CStaticCS	m_stcLoadPickerY[3];
	CStaticCS	m_stcLoadPickerZ[4];
	CStaticCS	m_stcLoadPickerP[2];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnStcTrayPickerXClick(UINT nID);
	afx_msg void OnStcTrayPickerZClick(UINT nID);
	afx_msg void OnStcTrayPickerRClick(UINT nID);
	afx_msg void OnStcLoadStage1XClick(UINT nID);
	afx_msg void OnStcLoadStage1ZClick(UINT nID);
	afx_msg void OnStcLoadStage2XClick(UINT nID);
	afx_msg void OnStcLoadStage2ZClick(UINT nID);
	afx_msg void OnStcLoadPickerYClick(UINT nID);
	afx_msg void OnStcLoadPickerZClick(UINT nID);
	afx_msg void OnStcLoadPickerPClick(UINT nID);

private:
	void Initial_Controls();
	void Display_MoveData();

public:
	void Save_MoveData();
	void Cancel_MoveData();
};

///////////////////////////////////////////////////////////////////////////////
