// ManualLoadDlg.h : 헤더 파일
//
#pragma once

// CManualLoadDlg 대화 상자입니다.

class CManualLoadDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CManualLoadDlg)

public:
	CManualLoadDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CManualLoadDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MANUAL_LOAD_DLG };
	CGroupCS	m_Group[17];
	CLabelCS	m_Label[11];
	CStaticCS	m_stcAxisPos[11];
	CButtonCS	m_btnTrayPickerX[3];
	CButtonCS	m_btnTrayPickerZ[3];
	CButtonCS	m_btnTrayPickerR[2];
	CButtonCS	m_btnTrayPickerIo[4];
	CLedCS		m_ledTrayPickerIo[9];
	CButtonCS	m_btnLoadPort1Io[4];
	CLedCS		m_ledLoadPort1Io[9];
	CButtonCS	m_btnLoadPort2Io[4];
	CLedCS		m_ledLoadPort2Io[10];
	CButtonCS	m_btnLoadPort3Io[2];
	CLedCS		m_ledLoadPort3Io[7];
	CButtonCS	m_btnLoadStage1X[4];
	CButtonCS	m_btnLoadStage1Z[5];
	CButtonCS	m_btnLoadStage1Io[4];
	CLedCS		m_ledLoadStage1Io[5];
	CButtonCS	m_btnLoadStage2X[4];
	CButtonCS	m_btnLoadStage2Z[5];
	CButtonCS	m_btnLoadStage2Io[4];
	CLedCS		m_ledLoadStage2Io[5];
	CButtonCS	m_btnLoadPickerX[5];
	CButtonCS	m_btnLoadPickerZ[4];
	CButtonCS	m_btnLoadPickerP[2];
	CButtonCS	m_btnLoadPickerIo[28];
	CLedCS		m_ledLoadPickerIo[24];	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnbtnTrayPickerXClick(UINT nID);
	afx_msg void OnbtnTrayPickerZClick(UINT nID);
	afx_msg void OnbtnTrayPickerRClick(UINT nID);
	afx_msg void OnbtnTrayPickerIoClick(UINT nID);
	afx_msg void OnbtnLoadPort1IoClick(UINT nID);
	afx_msg void OnbtnLoadPort2IoClick(UINT nID);
	afx_msg void OnbtnLoadPort3IoClick(UINT nID);
	afx_msg void OnbtnLoadStage1XClick(UINT nID);
	afx_msg void OnbtnLoadStage1ZClick(UINT nID);
	afx_msg void OnbtnLoadStage1IoClick(UINT nID);
	afx_msg void OnbtnLoadStage2XClick(UINT nID);
	afx_msg void OnbtnLoadStage2ZClick(UINT nID);
	afx_msg void OnbtnLoadStage2IoClick(UINT nID);
	afx_msg void OnbtnLoadPickerYClick(UINT nID);
	afx_msg void OnbtnLoadPickerZClick(UINT nID);
	afx_msg void OnbtnLoadPickerPClick(UINT nID);
	afx_msg void OnbtnLoadPickerIoClick(UINT nID);

private:
	CString m_strLog;
	void Initial_Controls();

public:
	void Display_Status();
};
