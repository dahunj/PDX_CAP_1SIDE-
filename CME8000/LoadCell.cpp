// LoadCell.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CME8000.h"
#include "LoadCell.h"

#include "DataManager.h"
#include "WorkDlg.h"
#include "Common.h"

#define LF				0x0A
#define CR				0x0D
#define EXT				0x03

CLoadCell g_objLoadCell;

IMPLEMENT_DYNAMIC(CLoadCell, CWnd)

CLoadCell::CLoadCell()
{
	m_bAssyConnected = FALSE;
	m_bUnloadConnected = FALSE;

	m_strAssyLoadCell = "";
	m_strUnloadLoadCell = "";
	m_nRecvPos = 0;
	memset(m_chRecv, 0x00, BARCODE_BUFFER_MAX);
}

CLoadCell::~CLoadCell()
{
}

BEGIN_MESSAGE_MAP(CLoadCell, CWnd)
	ON_MESSAGE(UM_PORT_RECEIVE, OnPortReceive)
END_MESSAGE_MAP()

// CLoadCell 메시지 처리기입니다.

/////////////////////////////////////////////////////////////////////////////
// Serial Port Receive Event

LRESULT CLoadCell::OnPortReceive(WPARAM nPort, LPARAM lParam)
{
	if (m_AssyPort.Get_PortNum() != (int)nPort && m_UnloadPort.Get_PortNum() != (int)nPort) return 0;

	BYTE byRecv[BARCODE_BUFFER_MAX] = { 0 };
	int nLen;
	if (m_AssyPort.Get_PortNum()   == (int)nPort) nLen = m_AssyPort.Read_Byte(byRecv);
	if (m_UnloadPort.Get_PortNum() == (int)nPort) nLen = m_UnloadPort.Read_Byte(byRecv);

	if (nLen < 1) return 0;

	int nTotalLen = m_nRecvPos + nLen;
	if (nTotalLen >= BARCODE_BUFFER_MAX || nTotalLen < 3) {

		m_strAssyLoadCell = "";
		m_strUnloadLoadCell = "";
		m_nRecvPos = 0;
		memset(m_chRecv, 0x00, BARCODE_BUFFER_MAX);
		return 0;
	}

	memcpy(m_chRecv + m_nRecvPos, byRecv, nLen);
	m_nRecvPos += nLen;

	if (m_chRecv[m_nRecvPos - 1] == EXT) {
		CString strTemp = (CString)m_chRecv;
		int nEXT = strTemp.Find(EXT);

		if (m_AssyPort.Get_PortNum() == (int)nPort) { 
			m_strAssyLoadCell = strTemp.Mid(11,8);
			m_dAssyLoadCell = atof(m_strAssyLoadCell);
		}
		if (m_UnloadPort.Get_PortNum() == (int)nPort) {
			m_strUnloadLoadCell = strTemp.Mid(11,8);
			m_dUnloadLoadCell = atof(m_strUnloadLoadCell);
		}

		m_nRecvPos = 0;
		memset(m_chRecv, 0x00, BARCODE_BUFFER_MAX);

		g_dlgWork.PostMessage(UM_UPDATE_LOADCELL, (int)nPort, NULL);
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////

BOOL CLoadCell::Initialize()
{
	if (m_bAssyConnected && m_bUnloadConnected) return TRUE;

#ifdef LOAD_CELL_USE
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	if (!m_bAssyConnected) { if (!m_AssyPort.Open_Port(pEquipData->nAssyLoadCellPort, 9600, 8, 1, 0, this)) return FALSE; }
	if (!m_bUnloadConnected) { if (!m_UnloadPort.Open_Port(pEquipData->nUnloadLoadCellPort, 9600, 8, 1, 0, this)) return FALSE; }
#endif
	if (!m_bAssyConnected) m_bAssyConnected = TRUE;
	if (!m_bUnloadConnected) m_bUnloadConnected = TRUE;

	return TRUE;
}

void CLoadCell::Terminate()
{
	m_bAssyConnected = FALSE;
	m_bUnloadConnected = FALSE;
#ifdef LOAD_CELL_USE
	m_AssyPort.Close_Port();
	m_UnloadPort.Close_Port();
#endif
}

void CLoadCell::Get_Weight(int nNo)
{
	if (nNo == ASSY_LOAD_CELL && !m_bAssyConnected) return;
	if (nNo == UNLOAD_LOAD_CELL && !m_bUnloadConnected) return;

	BYTE bySend[BARCODE_BUFFER_MAX] = { 0 };

	bySend[0] = 0x02;	//stx
	bySend[1] = 0x30;	//0
	bySend[2] = 0x31;	//1
	bySend[3] = 0x52;	//R
	bySend[4] = 0x43;	//C
	bySend[5] = 0x57;	//W
	bySend[6] = 0x54;	//T
	bySend[7] = 0x03;	//etx

	if (nNo == ASSY_LOAD_CELL)   m_AssyPort.Write_Byte(bySend, 8);
	if (nNo == UNLOAD_LOAD_CELL) m_UnloadPort.Write_Byte(bySend, 8);
	gData.bLoadCellComplete = FALSE;
}

///////////////////////////////////////////////////////////////////////////////
