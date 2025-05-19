// WorkInfoDlg.h : 헤더 파일
//
#pragma once

// CWorkInfoDlg 대화 상자입니다.

class CWorkInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWorkInfoDlg)

public:
	CWorkInfoDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CWorkInfoDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_WORK_INFO_DLG };
	CGroupCS	m_Group[3];
	CLabelCS	m_Label[6];
	CLedRectCS		m_ledLoadPickInfo[6];
	CLedRectCS		m_ledCapPickInfo[6];
	CLedRectCS		m_ledAssyPickInfo[6];
	CLedRectCS		m_ledUnloadPickInfo[6];
	CLedRectCS		m_ledCapBufferInfo[6];
	CLedRectCS		m_ledTransStageInfo[6];
	CLedCS			m_ledVisScanDone[5];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	void Initial_Controls();
	void Display_Status();
};

///////////////////////////////////////////////////////////////////////////////
