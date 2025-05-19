// ErrorDlg.h : 헤더 파일
//
#pragma once

const int BASE_ERR_CODE = 10000;

// CErrorDlg 대화 상자입니다.

class CErrorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CErrorDlg)

public:
	CErrorDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CErrorDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ERROR_DLG };
	CGroupCS	m_Group[4];
	CLabelCS	m_Label[8];
	CStaticCS	m_stcErrBack[4];
	CStaticCS	m_stcErrTitle;
	CPictureCS	m_Image;
	CLedCS		m_ledMainAir[2];
	CLedCS		m_ledEmgSw[3];
	CLedCS		m_ledDoorOpen[13];
	CStaticCS	m_stcErrPos[14];
	CStaticCS	m_stcErrNo;
	CStaticCS	m_stcMsgBack;
	CStaticCS	m_stcErrMsg;
	CButtonCS	m_btnErrBuzzOff;
	CButtonCS	m_btnErrSkip;
	CButtonCS	m_btnErrRetry;
// 	CButtonCS	m_btnErrToUnloadTray;
// 	CButtonCS	m_btnErrToDropTable;
	CButtonCS	m_btnErrOK;
	CButtonCS	m_btnErrSystemExit;
	CButtonCS	m_btnErrToManual;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnErrBuzzOff();
	afx_msg void OnBnClickedBtnErrSkip();
	afx_msg void OnBnClickedBtnErrRetry();
	afx_msg void OnBnClickedBtnErrOk();
	afx_msg void OnBnClickedBtnErrSystemExit();
	afx_msg void OnBnClickedBtnErrToManual();

private:
	CBitmap		m_bmpImage;
	int			m_nErrNo, m_nErrCode;
	CString		m_strErrMsg;
	CString		m_strErrSubMsg;
	int			m_nBackColorLoop;
	int			m_nErrorPos;

private:
	void Initial_Controls();
	void Set_AlarmLog(int nErrNo, CString sErrMsg);

public:
	void Set_ErrorNumber(int nErrNo) { m_nErrNo = nErrNo; m_nErrCode = BASE_ERR_CODE + nErrNo; }
	void Set_ErrorSubMessage(CString sErrMsg) { m_strErrSubMsg = sErrMsg; }
};

extern CErrorDlg g_dlgError;

///////////////////////////////////////////////////////////////////////////////
