// WorkDlg.h : 헤더 파일
//
#pragma once

#include "WorkInfoDlg.h"

#define UM_UPDATE_MODEL			WM_USER+1
#define UM_UPDATE_TRAY_INFO		WM_USER+2
#define UM_UPDATE_BARCODE		WM_USER+3
#define UM_RESET_CYCLE_STOP		WM_USER+4
#define UM_UPDATE_UPH			WM_USER+5
#define UM_VISION_RESULT		WM_USER+6
#define UM_LOT_START_END		WM_USER+7
#define UM_INDEX_TACK			WM_USER+8
#define UM_MODEL_CHANGE			WM_USER+9
#define UM_SHOW_MSG				WM_USER+10
#define UM_UPDATE_LOADCELL		WM_USER+11

// CWorkDlg 대화 상자입니다.

class CWorkDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWorkDlg)

public:
	CWorkDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CWorkDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_WORK_DLG };
	CGroupCS		m_Group[10];
	CLabelCS		m_Label[7];
	CLabelCS		m_lblLot[6];
	CGroupCS		m_grpLot[2];
	CStaticCS		m_stcLotIdR;
	CStaticCS		m_stcTrayCountR;
	CStaticCS		m_stcCmCountR;
	CStaticCS		m_stcLotIdS;
	CStaticCS		m_stcTrayCountS;
	CStaticCS		m_stcCmCountS;

	CLabelCS		m_lblCapLot[6];
	CStaticCS		m_stcCapLotId;
	CStaticCS		m_stcCapTrayCnt;
	CStaticCS		m_stcCapCnt;
	CStaticCS		m_stcCapLotIdS;
	CStaticCS		m_stcCapTrayCntS;
	CStaticCS		m_stcCapCntS;
	CStaticCS		m_stcShipLotId;
	CStaticCS		m_stcShipTrayCnt;
	CStaticCS		m_stcShipCnt;
	CStaticCS		m_stcShipLotIdS;
	CStaticCS		m_stcShipTrayCntS;
	CStaticCS		m_stcShipCntS;

	CPictureCS		m_imgEquipment;
	CPictureCS		m_picUphBack;
	CLedCS			m_ledEquipOption[10];
	CComboCS		m_cboStartIndexJig;
	CStaticCS		m_stcLoadNo[6];
	CStaticCS		m_stcAssyNo[6];
	CStaticCS		m_stcTransNo[6];
	CStaticCS		m_stcTStageNo[6];
	CStaticCS		m_stcUnloadNo[6];
	CLedCS			m_ledIndexDone[3];
	CStaticCS		m_stcIndexPos;
	CStaticCS		m_stcCycleTime;
	CRadioCS		m_rdoWorkStart;
	CRadioCS		m_rdoWorkStop;
	CLedCS			m_ledInitComplete;
	CLedCS			m_ledLotLoadEnable;
	CCheckCS		m_chkCycleStop;
	CCheckCS		m_chkMesUse;
	CPictureCS		m_picTrayBack[6];
	CStaticCS		m_stcLoadTrayCount[1];
	CStaticCS		m_stcCapTrayCount[1];
	CStaticCS		m_stcUnloadTrayCount[1];
	CGridCS			m_grdLoadTray[1];
	CGridCS			m_grdCapTray[1];
	CGridCS			m_grdUnloadTray[1];
	CStaticCS		m_stcWorkCase[16];
	CStaticCS		m_stcPortNo[12];
	CStaticCS		m_stcTakt[4];
	CStaticCS		m_stcUph[4];
	CStaticCS		m_stcDay[2];
	CLedCS			m_ledVisionStatus[5];
	CStaticCS		m_stcVisionResult[4];
	CButtonCS		m_btnCapClear;
	CButtonCS		m_btnShipClear;
	CButtonCS		m_btnMesCancel;

	CStaticCS		m_stcLoadCellCnt;
	CButtonCS		m_btnLoadCellChkEdit;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnStnClickedLotIdR();
	afx_msg void OnStnClickedCmCountR();
	afx_msg void OnStnClickedLotIdS();
	afx_msg void OnStnClickedCmCountS();
	afx_msg void OnStnClickedCapLotId();
	afx_msg void OnStnClickedCapTrayCount();
	afx_msg void OnStnClickedShipLotId();
	afx_msg void OnStnClickedShipTrayCount();
	afx_msg void OnBnClickedRdoWorkStart();
	afx_msg void OnBnClickedRdoWorkStop();
	afx_msg void OnBnClickedChkCycleStop();
	afx_msg void OnBnClickedChkMesUse();
	afx_msg void OnStnClickedLblLot0();
	afx_msg void OnStnClickedLblLot3();
	afx_msg void OnStnClickedLblCapLot0();
	afx_msg void OnStnClickedLblShipLot0();
	afx_msg LRESULT OnUpdateModel(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateTrayInfo(WPARAM nTray, LPARAM lParam);
	afx_msg LRESULT OnUpdateBarcode(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateLoadCell(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnResetCycleStop(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateUph(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnVisionResult(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLotStartEnd(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnIndexTack(WPARAM wParam, LPARAM lParam);	//AHN
	afx_msg LRESULT OnModelChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnShowMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedCapClear();
	afx_msg void OnBnClickedShipClear();
	afx_msg void OnBnClickedMesCancel();
	afx_msg void OnStnClickedLoadCellCnt();
	afx_msg void OnBnClickedLoadCellEdit();

private:
	CWorkInfoDlg *m_pWorkInfoDlg;

	CBitmap	m_bmpEquipment;
	BOOL	m_bAutoRunning;
	int		m_nGroupNo;

	int		m_nBuffChange;

	BOOL	m_bMesManual;

	int		m_nTimer;
	BOOL	m_bLoadSw3On;
	BOOL	m_bCapSw1On;
	BOOL	m_bCapSw2On;
	BOOL	m_bUnloadSw1On;

	BOOL	m_bCapLotIdInput;
	BOOL	m_bShipLotIdInput;

	int		n_TimerCnt;

private:
	BOOL Work_Start();
	void Initial_Controls();
	void Initial_Grid(CGridCS *pGrid, int nRows, int nCols, int nNoDir=0);
	void Check_Lamp();
	void Display_Status();
	void Reset_AlarmLog();

public:
	void Set_AutoRun(BOOL bAutoRun);
	void Set_AutoRunStatus(BOOL bAutoRun) { m_bAutoRunning = bAutoRun; }
	void Set_State(int nState);
	void Enable_UserInput(int nNo, BOOL bEnable);
	void Get_LotInfo(int nNo);
	void Set_LotInfo(int nNo);

	void Get_CapLotID();
	BOOL Check_CapLotID();
	BOOL Set_CapLotIDChange();
	void Get_ShipLotID();
	BOOL Check_ShipLotID();
	BOOL Set_ShipLotIDChange();
	int  Check_CapShipLotID(int nType, CString sBarID);
	void MachineStopLog(CString sType, CString sMsg="");
};

extern CWorkDlg g_dlgWork;

///////////////////////////////////////////////////////////////////////////////
