// SetupMoveTab3Dlg.h : 헤더 파일
//
#pragma once

// CSetupMoveTab3Dlg 대화 상자입니다.

class CSetupMoveTab3Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetupMoveTab3Dlg)

public:
	CSetupMoveTab3Dlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupMoveTab3Dlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SETUP_MOVE_TAB3_DLG };
	CGroupCS	m_Group[10];
	CLabelCS	m_Label[35];
	CStaticCS	m_stcTransStageX[2];
	CStaticCS	m_stcTransStageZ[3];
	CStaticCS	m_stcTransStageT[2];
	CStaticCS	m_stcUnloadPickerX[4];
	CStaticCS	m_stcUnloadPickerZ[4];
	CStaticCS	m_stcUnloadPickerP[2];
	CStaticCS	m_stcUnloadStage1Y[3];
	CStaticCS	m_stcUnloadStage1Z[6];
	CStaticCS	m_stcUnloadStage2Y[3];
	CStaticCS	m_stcUnloadStage2Z[6];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnStcTransStageXClick(UINT nID);
	afx_msg void OnStcTransStageZClick(UINT nID);
	afx_msg void OnStcTransStageTClick(UINT nID);
	afx_msg void OnStcUnloadPickerXClick(UINT nID);
	afx_msg void OnStcUnloadPickerZClick(UINT nID);
	afx_msg void OnStcUnloadPickerPClick(UINT nID);
	afx_msg void OnStcUnloadStage1YClick(UINT nID);
	afx_msg void OnStcUnloadStage1ZClick(UINT nID);
	afx_msg void OnStcUnloadStage2YClick(UINT nID);
	afx_msg void OnStcUnloadStage2ZClick(UINT nID);

private:
	void Initial_Controls();
	void Display_MoveData();

public:
	void Save_MoveData();
	void Cancel_MoveData();
};

///////////////////////////////////////////////////////////////////////////////
