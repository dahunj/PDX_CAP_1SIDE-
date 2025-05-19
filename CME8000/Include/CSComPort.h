// ComPortCS.h : 헤더 파일
//
#pragma once

#define UM_PORT_RECEIVE		WM_USER+9003

class CComPortCS
{
public:
	CComPortCS();
	virtual ~CComPortCS();

private:
	int			m_nPort;
	int			m_nInterval;	// TimeOut : Char To Char Interval
	int			m_nMultiplier;	// TimeOut
	int			m_nConstant;	// TimeOut
	int			m_nDtrControl;	// DTR Control
	int			m_nRtsControl;	// RTS Control

	HANDLE		m_hComm;
	CWnd*		m_pParent;

	OVERLAPPED	m_ovRead;
	OVERLAPPED	m_ovWrite;

	int			m_nReadLen;
	BYTE*		m_pRecvBuff;

	BOOL		m_bConnected;
	CWinThread*	m_pThreadFun;
	static UINT Thread_Receive(LPVOID lpVoid);

	void Recv_Byte();

public:
	int	 Get_PortNum() { return m_nPort; }
	void Set_TimeOut(int nInterval, int nMultiplier, int nConstant);	// Open_ComPort 전에 호출할것

	// DTR Control : 0(DTR_CONTROL_DISABLE), 1(DTR_CONTROL_ENABLE), 2(DTR_CONTROL_HANDSHAKE)
	void Set_DtrControl(int nDtr) { m_nDtrControl = nDtr; }				// Open_ComPort 전에 호출할것

	// RTS Control : 0(RTS_CONTROL_DISABLE), 1(RTS_CONTROL_ENABLE), 2(RTS_CONTROL_HANDSHAKE), 3(RTS_CONTROL_TOGGLE)
	void Set_RtsControl(int nRts) { m_nRtsControl = nRts; }				// Open_ComPort 전에 호출할것

	// nData(4, 5, 6, 7, 8), nStop(1, 15, 2), nParity(0:None, 1:Odd, 2:Even, 3:Mark, 4:Space)
	BOOL Open_Port(int nPort, int nBaud, int nData, int nStop, int nParity, CWnd* pParent = NULL);
	void Close_Port();

	int Read_Byte(BYTE* pBuffer);
	int Write_Byte(BYTE* pBuffer, int nLength);
};

///////////////////////////////////////////////////////////////////////////////
