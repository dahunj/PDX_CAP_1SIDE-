// ManualUnloadDlg.h : 헤더 파일
//
#pragma once

// CManualNgDlg 대화 상자입니다.

class CManualUnloadDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CManualUnloadDlg)

public:
	CManualUnloadDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CManualUnloadDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MANUAL_UNLOAD_DLG };
	CGroupCS	m_Group[16];
	CLabelCS	m_Label[10];
	CStaticCS	m_stcAxisPos[10];
	CButtonCS	m_btnTransStageX[2];
	CButtonCS	m_btnTransStageZ[3];
	CButtonCS	m_btnTransStageT[2];
	CButtonCS	m_btnTransStageIo[20];
	CLedCS		m_ledTransStageIo[10];
	CButtonCS	m_btnUnloadPickerX[6];
	CButtonCS	m_btnUnloadPickerZ[4];
	CButtonCS	m_btnUnloadPickerP[2];
	CButtonCS	m_btnUnloadPickerIo[28];
	CLedCS		m_ledUnloadPickerIo[24];
	CButtonCS	m_btnUnloadStage1Y[5];
	CButtonCS	m_btnUnloadStage1Z[6];
	CButtonCS	m_btnUnloadStage1Io[4];
	CLedCS		m_ledUnloadStage1Io[5];
	CButtonCS	m_btnUnloadStage2Y[5];
	CButtonCS	m_btnUnloadStage2Z[6];
	CButtonCS	m_btnUnloadStage2Io[4];
	CLedCS		m_ledUnloadStage2Io[5];
	CButtonCS	m_btnUnloadPort1Io[4];
	CLedCS		m_ledUnloadPort1Io[10];
	CButtonCS	m_btnUnloadPort2Io[4];
	CLedCS		m_ledUnloadPort2Io[10];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBtnTransStageXClick(UINT nID);
	afx_msg void OnBtnTransStageZClick(UINT nID);
	afx_msg void OnBtnTransStageTClick(UINT nID);
	afx_msg void OnBtnTransStageIoClick(UINT nID);
	afx_msg void OnBtnUnloadPickerXClick(UINT nID);
	afx_msg void OnBtnUnloadPickerZClick(UINT nID);
	afx_msg void OnBtnUnloadPickerPClick(UINT nID);
	afx_msg void OnBtnUnloadPickerIoClick(UINT nID);
	afx_msg void OnBtnUnloadStage1YClick(UINT nID);
	afx_msg void OnBtnUnloadStage1ZClick(UINT nID);
	afx_msg void OnBtnUnloadStage1IoClick(UINT nID);
	afx_msg void OnBtnUnloadStage2YClick(UINT nID);
	afx_msg void OnBtnUnloadStage2ZClick(UINT nID);
	afx_msg void OnBtnUnloadStage2IoClick(UINT nID);
	afx_msg void OnBtnUnloadPort1IoClick(UINT nID);
	afx_msg void OnBtnUnloadPort2IoClick(UINT nID);

private:
	CString m_strLog;
	void Initial_Controls();

public:
	void Display_Status();
};
