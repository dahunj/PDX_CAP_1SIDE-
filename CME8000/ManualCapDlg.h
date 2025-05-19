// ManualCapDlg.h : 헤더 파일
//
#pragma once

// CManualCapDlg 대화 상자입니다.

class CManualCapDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CManualCapDlg)

public:
	CManualCapDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CManualCapDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MANUAL_CAP_DLG };
	CGroupCS	m_Group[14];
	CLabelCS	m_Label[8];
	CStaticCS	m_stcAxisPos[8];
	CButtonCS	m_btnCapPort1Io[4];
	CLedCS		m_ledCapPort1Io[10];
	CButtonCS	m_btnCapPort2Io[4];
	CLedCS		m_ledCapPort2Io[10];
	CButtonCS	m_btnCapStage1X[5];
	CButtonCS	m_btnCapStage1Z[4];
	CButtonCS	m_btnCapStage1Io[4];
	CLedCS		m_ledCapStage1Io[5];
	CButtonCS	m_btnCapStage2X[5];
	CButtonCS	m_btnCapStage2Z[4];
	CButtonCS	m_btnCapStage2Io[4];
	CLedCS		m_ledCapStage2Io[5];
	CButtonCS	m_btnCapPickerY[15];
	CButtonCS	m_btnCapPickerZ[4];
	CButtonCS	m_btnCapPickerP[2];
	CButtonCS	m_btnCapPickerIo[42];
	CLedCS		m_ledCapPickerIo[24];
	CButtonCS	m_btnCapBufferY[6];
	CButtonCS	m_btnCapBufferIo[22];
	CLedCS		m_ledCapBufferIo[8];

	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBtnCapPort1IoClick(UINT nID);
	afx_msg void OnBtnCapPort2IoClick(UINT nID);
	afx_msg void OnBtnCapStage1XClick(UINT nID);
	afx_msg void OnBtnCapStage1ZClick(UINT nID);
	afx_msg void OnBtnCapStage1IoClick(UINT nID);
	afx_msg void OnBtnCapStage2XClick(UINT nID);
	afx_msg void OnBtnCapStage2ZClick(UINT nID);
	afx_msg void OnBtnCapStage2IoClick(UINT nID);
	afx_msg void OnBtnCapPickerYClick(UINT nID);
	afx_msg void OnBtnCapPickerZClick(UINT nID);
	afx_msg void OnBtnCapPickerPClick(UINT nID);
	afx_msg void OnBtnCapPickerIoClick(UINT nID);
	afx_msg void OnBtnCapBufferYClick(UINT nID);
	afx_msg void OnBtnCapBufferIoClick(UINT nID);

private:
	CString m_strLog;
	void Initial_Controls();

public:
	void Display_Status();
};
