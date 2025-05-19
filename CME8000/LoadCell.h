// LoadCell.h : 헤더 파일
//
#pragma once

#define BARCODE_BUFFER_MAX	1024

class CLoadCell : public CWnd
{
	DECLARE_DYNAMIC(CLoadCell)

public:
	CLoadCell();
	virtual ~CLoadCell();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnPortReceive(WPARAM nPort, LPARAM lParam);

private:
	CComPortCS m_AssyPort;
	CComPortCS m_UnloadPort;

	BOOL	m_bReadOk;
	BOOL	m_bAssyConnected;
	BOOL	m_bUnloadConnected;
	CString	m_strAssyLoadCell;
	CString m_strUnloadLoadCell;

	double	m_dAssyLoadCell;
	double	m_dUnloadLoadCell;

	int		m_nRecvPos;
	char	m_chRecv[BARCODE_BUFFER_MAX];

public:
	BOOL Initialize();
	void Terminate();
	void Get_Weight(int nNo);

	BOOL Is_AssyConnected() { return m_bAssyConnected; }
	BOOL Is_UnloadConnected() { return m_bUnloadConnected; }

	double Get_AssyLoadCell() { return m_dAssyLoadCell; }
	double Get_UnloadLoadCell() { return m_dUnloadLoadCell; }
};

extern CLoadCell g_objLoadCell;

///////////////////////////////////////////////////////////////////////////////
