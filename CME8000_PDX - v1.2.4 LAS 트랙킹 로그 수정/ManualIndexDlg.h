// ManualIndexDlg.h : 헤더 파일
//
#pragma once

// CManualIndexDlg 대화 상자입니다.

class CManualIndexDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CManualIndexDlg)

public:
	CManualIndexDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CManualIndexDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MANUAL_INDEX_DLG };
	CGroupCS	m_Group[9];
	CLabelCS	m_Label[6];
	CStaticCS	m_stcAxisPos[6];
	CStaticCS	m_stcIndexPos;
	CButtonCS	m_btnIndexR[2];
	CLedCS		m_ledIndexPos[2];
	CButtonCS	m_btnIndexIo[38];
	CLedCS		m_ledIndexIo[22];
	CButtonCS	m_btnVisionCMAlignX[4];
	CButtonCS	m_btnVisionCapAlignY[3];
	CButtonCS	m_btnAssyPickerX[7];
	CButtonCS	m_btnAssyPickerY[7];
	CButtonCS	m_btnAssyPickerZ[7];
	CButtonCS	m_btnAssyPickerIo[31];
	CLedCS		m_ledAssyPickerIo[27];
	CStaticCS	m_stcAssyLoadCell[6];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBtnIndexRClick(UINT nID);
	afx_msg void OnBtnIndexIoClick(UINT nID);
	afx_msg void OnBtnVisionCMAlignXClick(UINT nID);
	afx_msg void OnBtnVisionCapAlignYClick(UINT nID);
	afx_msg void OnBtnAssyPickerXClick(UINT nID);
	afx_msg void OnBtnAssyPickerYClick(UINT nID);
	afx_msg void OnBtnAssyPickerZClick(UINT nID);
	afx_msg void OnBtnAssyPickerIoClick(UINT nID);

private:
	CString m_strLog;
	void Initial_Controls();

public:
	void Display_Status();
};
