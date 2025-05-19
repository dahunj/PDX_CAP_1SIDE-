// SetupMoveTab2Dlg.h : 헤더 파일
//
#pragma once

// CSetupMoveTab2Dlg 대화 상자입니다.

class CSetupMoveTab2Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetupMoveTab2Dlg)

public:
	CSetupMoveTab2Dlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupMoveTab2Dlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SETUP_MOVE_TAB2_DLG };
	CGroupCS	m_Group[14];
	CLabelCS	m_Label[62];
	CStaticCS	m_stcIndexR[1];			
	CStaticCS	m_stcVisionCMAlignX[2];	
	CStaticCS	m_stcCapStage1X[3];		
	CStaticCS	m_stcCapStage1Z[4];
	CStaticCS	m_stcCapStage2X[3];		
	CStaticCS	m_stcCapStage2Z[4];		
	CStaticCS	m_stcCapPickerY[13];		
	CStaticCS	m_stcCapPickerZ[4];		
	CStaticCS	m_stcCapPickerP[2];		
	CStaticCS	m_stcCapBufferY[4];		
	CStaticCS	m_stcVisionCapAlignY[1];	
	CStaticCS	m_stcAssyPickerX[7];		
	CStaticCS	m_stcAssyPickerY[7];		
	CStaticCS	m_stcAssyPickerZ[7];		

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnStcIndexRlick(UINT nID);
	afx_msg void OnStcVisionCMAlignXClick(UINT nID);
	afx_msg void OnStcCapStage1XClick(UINT nID);
	afx_msg void OnStcCapStage1ZClick(UINT nID);
	afx_msg void OnStcCapStage2XClick(UINT nID);
	afx_msg void OnStcCapStage2ZClick(UINT nID);
	afx_msg void OnStcCapPickerYClick(UINT nID);
	afx_msg void OnStcCapPickerZClick(UINT nID);
	afx_msg void OnStcCapPickerPClick(UINT nID);
	afx_msg void OnStcCapBufferYClick(UINT nID);
	afx_msg void OnStcVisionCapAlignYClick(UINT nID);
	afx_msg void OnStcAssyPickerXClick(UINT nID);
	afx_msg void OnStcAssyPickerYClick(UINT nID);
	afx_msg void OnStcAssyPickerZClick(UINT nID);

private:
	void Initial_Controls();
	void Display_MoveData();

public:
	void Save_MoveData();
	void Cancel_MoveData();
};

///////////////////////////////////////////////////////////////////////////////
