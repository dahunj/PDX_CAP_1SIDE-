// BarcodeLot.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CME8000.h"
#include "BarcodeLot.h"

#include "DataManager.h"
#include "WorkDlg.h"
#include "Common.h"

#define LF				0x0A
#define CR				0x0D

CBarcodeLot g_objBarcodeLot;

IMPLEMENT_DYNAMIC(CBarcodeLot, CWnd)

CBarcodeLot::CBarcodeLot()
{
	m_bConnected = FALSE;
	m_strBarcodeLot = "";
	m_nRecvPos = 0;
	memset(m_chRecv, 0x00, BARCODE_BUFFER_MAX);
}

CBarcodeLot::~CBarcodeLot()
{
}

BEGIN_MESSAGE_MAP(CBarcodeLot, CWnd)
	ON_MESSAGE(UM_PORT_RECEIVE, OnPortReceive)
END_MESSAGE_MAP()

// CBarcodeLot 메시지 처리기입니다.

/////////////////////////////////////////////////////////////////////////////
// Serial Port Receive Event

LRESULT CBarcodeLot::OnPortReceive(WPARAM nPort, LPARAM lParam)
{
	if (m_Port.Get_PortNum() != (int)nPort) return 0;

	BYTE byRecv[BARCODE_BUFFER_MAX] = { 0 };
	int nLen = m_Port.Read_Byte(byRecv);

	if (nLen < 1) return 0;

	int nTotalLen = m_nRecvPos + nLen;
	if (nTotalLen >= BARCODE_BUFFER_MAX || nTotalLen < 3) {

		m_strBarcodeLot = "";
		m_nRecvPos = 0;
		memset(m_chRecv, 0x00, BARCODE_BUFFER_MAX);
		return 0;
	}

	memcpy(m_chRecv + m_nRecvPos, byRecv, nLen);
	m_nRecvPos += nLen;

	if (m_chRecv[m_nRecvPos - 1] == CR) {
		CString strTemp = (CString)m_chRecv;
		int nCR = strTemp.Find(CR);

		// 바코드 입력시 설비에 적용된 모델과 같은 바코드인지 확인
		CString strModelChk;
		if (nCR > 6)
		{
			strModelChk = strTemp.Mid(2,4);
			m_strBarcodeLot = strTemp.Left(nCR);
		}
		else
		{
			g_objCommon.Show_MsgBox(1, "바코드 길이가 너무 짧습니다.");
		}

		m_nRecvPos = 0;
		memset(m_chRecv, 0x00, BARCODE_BUFFER_MAX);

		g_dlgWork.PostMessage(UM_UPDATE_BARCODE, NULL, NULL);
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////

BOOL CBarcodeLot::Initialize()
{
	if (m_bConnected) return TRUE;

#ifdef LOT_BARCODE_USE
	//if (!m_Port.Open_Port(nPort, 9600, 8, 0, 0, this)) return FALSE;	// cino
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	if (!m_Port.Open_Port(pEquipData->nLotBarcodePort, 115200, 8, 1, 0, this)) return FALSE;	// hanywell
#endif
	m_bConnected = TRUE;
	return TRUE;
}

void CBarcodeLot::Terminate()
{
	m_bConnected = FALSE;
#ifdef LOT_BARCODE_USE
	m_Port.Close_Port();
#endif
}

///////////////////////////////////////////////////////////////////////////////
