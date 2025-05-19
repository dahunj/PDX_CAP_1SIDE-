// BarcodeLot.h : 헤더 파일
//
#pragma once

#define BARCODE_BUFFER_MAX	1024

class CBarcodeLot : public CWnd
{
	DECLARE_DYNAMIC(CBarcodeLot)

public:
	CBarcodeLot();
	virtual ~CBarcodeLot();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnPortReceive(WPARAM nPort, LPARAM lParam);

private:
	CComPortCS m_Port;

	BOOL	m_bConnected;
	BOOL	m_bReadOk;
	CString	m_strBarcodeLot;

	int		m_nRecvPos;
	char	m_chRecv[BARCODE_BUFFER_MAX];

public:
	BOOL Initialize();
	void Terminate();

	BOOL Is_Connected() { return m_bConnected; }

	CString Get_BarcodeLot() { return m_strBarcodeLot; }
};

extern CBarcodeLot g_objBarcodeLot;

///////////////////////////////////////////////////////////////////////////////
