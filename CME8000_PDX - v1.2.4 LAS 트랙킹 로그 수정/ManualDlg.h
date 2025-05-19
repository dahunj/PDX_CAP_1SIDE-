// ManualDlg.h : 헤더 파일
//
#pragma once

#include "ManualLoadDlg.h"
#include "ManualIndexDlg.h"
#include "ManualCapDlg.h"
#include "ManualUnloadDlg.h"

// CManualDlg 대화 상자입니다.

class CManualDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CManualDlg)

public:
	CManualDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CManualDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MANUAL_DLG };
	CPictureCS	m_picManualBack;
	CRadioCS	m_rdoManualLoad;
	CRadioCS	m_rdoManualBtm1;
	CRadioCS	m_rdoManualCap;
	CRadioCS	m_rdoManualUnload;
	CRadioCS	m_rdoManualDoorLock;
	CRadioCS	m_rdoManualDoorUnlock;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedRdoManualLoad();
	afx_msg void OnBnClickedRdoManualIndex();
	afx_msg void OnBnClickedRdoManualCap();
	afx_msg void OnBnClickedRdoManualUnload();
	afx_msg void OnBnClickedRdoManualDoorLock();
	afx_msg void OnBnClickedRdoManualDoorUnlock();

public:
	CManualLoadDlg		*m_pManualLoadDlg;
	CManualIndexDlg		*m_pManualIndexDlg;
	CManualCapDlg		*m_pManualCapDlg;
	CManualUnloadDlg	*m_pManualUnloadDlg;

private:
	void Initial_Controls();
	void Hide_Windows();

public:
	void Set_ManualPos(int nPos);
};

extern CManualDlg g_dlgManual;

///////////////////////////////////////////////////////////////////////////////
