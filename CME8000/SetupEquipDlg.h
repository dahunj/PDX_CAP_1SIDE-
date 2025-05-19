// SetupEquipDlg.h : 헤더 파일
//
#pragma once

// CSetupEquipDlg 대화 상자입니다.

class CSetupEquipDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetupEquipDlg)

public:
	CSetupEquipDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupEquipDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SETUP_EQUIP_DLG };
	CGroupCS	m_Group[11];
	CLabelCS	m_Label[36];
	CStaticCS	m_stcEquipName;
	CComboCS	m_cboLotBarcodePort;
	CComboCS	m_cboAssyLoadCellPort;
	CComboCS	m_cboUnloadLoadCellPort;
	CCheckCS	m_chkJobListTaktLog;
	CStaticCS	m_stcScreenOff;
	CStaticCS	m_stcNoWorkTime;
	CStaticCS	m_stcMotionCheck;
	CCheckCS	m_chkManualTaktTest;
	CLabelCS	m_lblDoorLock;
	CRadioCS	m_rdoDoorLock[2];
	CLabelCS	m_lblDoorLock2;
	CStaticCS	m_stcDoorLockTime;
	
	CRadioCS	m_rdoPickCnt[3];
	CCheckCS	m_chkUseInlineMode;
	CCheckCS	m_chkUseVisionCmAlign;
	CCheckCS	m_chkUseVisionCapAlign;
	CCheckCS	m_chkUseTrayPickerTurn;
	CCheckCS	m_chkUseCapPickerTurn;
	CCheckCS	m_chkUseAssyPickerVac;
	CCheckCS	m_chkUseIndexAssyVac;
	CCheckCS	m_chkUseAssyPickerTiltChk;
	CCheckCS	m_chkUseVisionAlignAlarm;
	CCheckCS	m_chkUseVisionAlignOffset;
	CCheckCS	m_chkUseLoadTraySortDir;
	CCheckCS	m_chkCapPickUpMulti;
	CCheckCS	m_chkShipTrayOneEmpty;
	CCheckCS	m_chkUseMesCapReg;
	CCheckCS	m_chkUseMesShipReg;

	CStaticCS	m_stcLoadTrayData[4];
	CStaticCS	m_stcCapTrayData[4];
	CStaticCS	m_stcUnloadTrayData[4];
	CStaticCS	m_stcIndexData[1];
	CStaticCS	m_stcVacOffDelay[4];
	CStaticCS	m_stcVacOffRepeat[4];
	CStaticCS	m_stcDelayAdd[6];
	CStaticCS	m_stcAlignOffset;
	CStaticCS	m_stcLoadCellData[2];
	CGroupCS	m_grpAvi;
	CLabelCS	m_lblAvi;
	CIPAddressCtrl	m_ipaAviIp;
	CGroupCS	m_grpTower;
	CLabelCS	m_lblTower[10];
	CCheckCS	m_chkTower[6][4];
	CGroupCS	m_grpBuzzer;
	CLabelCS	m_lblBuzzer[11];
	CCheckCS	m_chkBuzzer[5][6];
	CGroupCS	m_grpHidden;
	CLabelCS	m_lblPasswordMt;
	CStaticCS	m_stcPasswordMt;
	CLabelCS	m_lblPasswordSi;
	CEditCS		m_edtPasswordSi;
	CGroupCS	m_grpResultTest;
	CCheckCS	m_chkResultTestUse;
	CLabelCS	m_lblResultTest[3];
	CEditCS		m_edtResultTest[3];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnStnClickedStcEquipName();
	afx_msg void OnStnClickedStcScreenOff();
	afx_msg void OnStcLoadTrayDataClick(UINT nID);
	afx_msg void OnStcCapTrayDataClick(UINT nID);
	afx_msg void OnStcUnloadTrayDataClick(UINT nID);
	afx_msg void OnStcIndexDataClick(UINT nID);
	afx_msg void OnStcVacOffDelayClick(UINT nID);
	afx_msg void OnStcVacOffRepeatClick(UINT nID);
	afx_msg void OnStcDelayAddClick(UINT nID);
	afx_msg void OnStcAlignOffsetClick();
	afx_msg void OnStnClickedStcShowHidden();
	afx_msg void OnStnClickedStcPasswordMt();
	afx_msg void OnStnClickedStcNoworkTime();
	afx_msg void OnStcLoadCellDataClick(UINT nID);
	afx_msg void OnStnClickedStcMotionCheck();
	afx_msg void OnStnClickedstcRockTime();

private:
	CString m_strLog;
	int		m_nPreModel;
	void Initial_Controls();
	void Display_EquipData();

public:
	void Save_EquipData();
	void Cancel_EquipData();
	afx_msg void OnBnClickedBtnAviConnect();
	afx_msg void OnBnClickedBtnAviDisconnect();
};

///////////////////////////////////////////////////////////////////////////////
