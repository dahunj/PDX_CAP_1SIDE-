/////////////////////////////////////////////////////////////////////////////
// CSClientSocket.h
//
#pragma once

#include <afxsock.h>		// MFC socket extensions

#define UM_CLIENT_CONNECT	WM_USER+9005
#define UM_CLIENT_RECEIVE	WM_USER+9006
#define UM_CLIENT_CLOSE		WM_USER+9007

class CClientSocketCS : public CAsyncSocket
{
public:
	CClientSocketCS();
	virtual ~CClientSocketCS();
	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);

private:
	CWnd*		m_pParent;
	int			m_nReadLen;
	BYTE*		m_pRecvBuff;

public:
	BOOL Open_Socket(CString strHostIp, UINT nHostPort, CWnd* pParent = NULL);
	void Close_Socket();

	int Read_Socket(BYTE* pBuffer);
	BOOL Write_Socket(BYTE* pBuffer, int nLength);
	BOOL Get_ServerInfo(CString& strIp, UINT& nPort);
};

///////////////////////////////////////////////////////////////////////////////
