// GridCS.h : 헤더 파일입니다.
//
#pragma once

///////////////////////////////////////////////////////////////////////////////
// Cell ID
class CCellID
{	
public:
	int m_nRow;
	int m_nCol;

public:
	CCellID(int row=-1, int col=-1) { m_nRow = row; m_nCol = col; }

	int IsValid() { return (m_nRow >= 0 && m_nCol >= 0); }
	int operator==(CCellID& id) { return (m_nRow == id.m_nRow && m_nCol == id.m_nCol); }
	int operator!=(CCellID& id) { return !operator==(id); }
};

///////////////////////////////////////////////////////////////////////////////
// Cell Range
class CCellRange
{ 
protected:
	int m_nMinRow;
	int m_nMinCol;
	int m_nMaxRow;
	int m_nMaxCol;

public:
	CCellRange(int minRow=-1, int minCol=-1, int maxRow=-1, int maxCol=-1) { Set(minRow, minCol, maxRow, maxCol); }

	void Set(int minRow, int minCol, int maxRow, int maxCol) { m_nMinRow = minRow; m_nMinCol = minCol; m_nMaxRow = maxRow; m_nMaxCol = maxCol; }
	int  IsValid() { return (m_nMinRow >= 0 && m_nMinCol >= 0 && m_nMaxRow >= 0 && m_nMaxCol >= 0 && m_nMinRow <= m_nMaxRow && m_nMinCol <= m_nMaxCol); }

	int GetMinRow() { return m_nMinRow; }
	int GetMinCol() { return m_nMinCol; }
	int GetMaxRow() { return m_nMaxRow; }
	int GetMaxCol() { return m_nMaxCol; }

	void SetMinRow(int minRow) { m_nMinRow = minRow; }
	void SetMinCol(int minCol) { m_nMinCol = minCol; }
	void SetMaxRow(int maxRow) { m_nMaxRow = maxRow; }
	void SetMaxCol(int maxCol) { m_nMaxCol = maxCol; }

	void operator=(CCellRange& range) { if (this != &range) Set(range.m_nMinRow, range.m_nMinCol, range.m_nMaxRow, range.m_nMaxCol); }
	BOOL operator==(CCellRange& range) { return (m_nMinRow == range.m_nMinRow && m_nMinCol == range.m_nMinCol && m_nMaxRow == range.m_nMaxRow && m_nMaxCol == range.m_nMaxCol); }
	int  operator!=(CCellRange& range) { return !operator==(range); }
};

///////////////////////////////////////////////////////////////////////////////
// Cell Base

// Cell states
#define CELL_STATE_NONE		0x00
#define CELL_STATE_FIXED	0x01

// Cell Align
#define CELL_ALIGN_CENTER	0
#define CELL_ALIGN_LEFT		1
#define CELL_ALIGN_RIGHT	2
#define CELL_ALIGN_MULTI	3

// Cell Size
#define CELL_SIZE_CX		50
#define CELL_SIZE_CY		20

class CGridCS;

class CCellBase
{
public:
	CCellBase();
	virtual ~CCellBase();

protected:
	DWORD		m_nState;
	int			m_nAlign;
	CSize		m_Size;
	CString		m_strText;
	DWORD		m_dwFormat;
	int			m_nMargin;
	COLORREF	m_BackClr;
	COLORREF	m_TextClr;
	LOGFONT*	m_plgFont;
	CGridCS*	m_pGrid;	// Parent Grid

public:
	void operator=(CCellBase& cell);
	virtual BOOL Draw_Cell(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBkgnd = TRUE);

	virtual void Set_State(DWORD nState)    { m_nState = nState; }
	virtual void Add_State(DWORD nState)    { m_nState |= nState; }
	virtual void Del_State(DWORD nState)    { m_nState &= ~nState; }
	virtual void Set_Align(int nAlign)		{ m_nAlign = nAlign; }
	virtual void Set_Width(int nWidth)      { m_Size.cx = nWidth; }
	virtual void Set_Height(int nHeight)    { m_Size.cy = nHeight; }
	virtual void Set_Text(CString strText)  { m_strText = strText; }
	virtual void Set_Format(DWORD dwFormat) { m_dwFormat = dwFormat; }
	virtual void Set_Margin(UINT nMargin)   { m_nMargin = nMargin; }
	virtual void Set_BackClr(COLORREF clr)  { m_BackClr = clr; }
	virtual void Set_TextClr(COLORREF clr)  { m_TextClr = clr; }
	virtual void Set_Font(LOGFONT* plf)     { memcpy(m_plgFont, plf, sizeof(LOGFONT)); }
	virtual void Set_Grid(CGridCS* pGrid)	{ m_pGrid = pGrid; }

	virtual DWORD    Get_State()   { return m_nState; }
	virtual int      Get_Align()   { return m_nAlign; }
	virtual int      Get_Width()   { return m_Size.cx; }
	virtual int      Get_Height()  { return m_Size.cy; }
	virtual CString	 Get_Text()    { return m_strText; }
	virtual DWORD    Get_Format()  { return m_dwFormat; }
	virtual UINT     Get_Margin()  { return m_nMargin; }
	virtual COLORREF Get_BackClr() { return m_BackClr; }
	virtual COLORREF Get_TextClr() { return m_TextClr; }
	virtual LOGFONT* Get_Font()    { return m_plgFont; }
	virtual CGridCS* Get_Grid()    { return m_pGrid; }

	virtual BOOL Is_Fixed() { return (m_nState & CELL_STATE_FIXED); }
};

///////////////////////////////////////////////////////////////////////////////
// GridCS

#include <afxtempl.h>

typedef CArray<CCellBase*, CCellBase*>	GRID_ROW;
typedef CArray<GRID_ROW*, GRID_ROW*>	GRID_CELL;
typedef CArray<int, int>				CELL_SIZE;

#define SCROLL_BAR_NONE		0	  // Neither
#define SCROLL_BAR_HORZ		1	  // Horz Scrollbar
#define SCROLL_BAR_VERT		2	  // Vert Scrollbar

class CGridCS : public CButton
{
	DECLARE_DYNAMIC(CGridCS)

public:
	CGridCS();
	virtual ~CGridCS();

protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

protected:
	int			m_nRows;
	int			m_nCols;
	int			m_nFixRows;
	int			m_nFixCols;

	int			m_nBarState;
	int			m_nHScrollMax;
	int			m_nVScrollMax;

	CELL_SIZE	m_arRowHeights;
	CELL_SIZE	m_arColWidths;
	GRID_CELL	m_arGridCell;

	COLORREF	m_FixTextClr, m_FixBackClr;
	COLORREF	m_CellTextClr, m_CellBackClr;
	COLORREF	m_GridBackClr, m_GridLineClr;

	CCellID		m_idTopLeft;

protected:
	void Reset_ScrollBars();
	void Enable_ScrollBars(int nBar, BOOL bEnable = TRUE);
	BOOL Set_ScrollPos(int nBar, int nPos, BOOL bRedraw = TRUE);
	int  Get_ScrollPos(int nBar, BOOL bGetTrackPos = FALSE);
	BOOL IsVisibleVScroll() { return ( (m_nBarState & SCROLL_BAR_VERT) > 0); } 
	BOOL IsVisibleHScroll() { return ( (m_nBarState & SCROLL_BAR_HORZ) > 0); }

	CCellBase* Create_Cell(int nRow, int nCol);
	void Destroy_Cell(int nRow, int nCol);
	BOOL Set_Cell(int nRow, int nCol, CCellBase* pCell);
	CCellBase* Get_Cell(int nRow, int nCol);

	BOOL Invalidate_CellRect(CCellID& id) { return Invalidate_CellRect(id.m_nRow, id.m_nCol); }
	BOOL Invalidate_CellRect(int nRow, int nCol);

	void Redraw_Cell(CCellID& id, BOOL bRedraw) { Redraw_Cell(id.m_nRow, id.m_nCol, bRedraw); }
	void Redraw_Cell(int nRow, int nCol, BOOL bRedraw);

	BOOL Get_CellOrigin(CCellID& id, LPPOINT pPt) { return Get_CellOrigin(id.m_nRow, id.m_nCol, pPt); }
	BOOL Get_CellOrigin(int nRow, int nCol, LPPOINT pPt);
	BOOL Get_CellRect(CCellID& id, LPRECT pRect) { return Get_CellRect(id.m_nRow, id.m_nCol, pRect); }
	BOOL Get_CellRect(int nRow, int nCol, LPRECT pRect);

	CCellID Get_TopLeftNoFixCell(BOOL bRecalculation = FALSE);
	CCellRange Get_VisibleFixCellRange(LPRECT pRect = NULL, BOOL bRecalculation = FALSE);
	CCellRange Get_VisibleNoFixCellRange(LPRECT pRect = NULL, BOOL bRecalculation = FALSE);

public:
	void Set_RowCount(int nRows);
	void Set_ColCount(int nCols);
	void Set_FixRowCount(int nFixRows);
	void Set_FixColCount(int nFixCols);
	void Set_RowHeight(int nRow, int nHeight);
	void Set_ColWidth(int nCol, int nWidth);

	int  Get_RowCount() { return m_nRows; }
	int  Get_ColCount() { return m_nCols; }
	int  Get_FixRowCount() { return m_nFixRows; }
	int  Get_FixColCount() { return m_nFixCols; }
	int  Get_RowHeight(int nRow) { return m_arRowHeights[nRow]; }
	int  Get_ColWidth(int nCol) { return m_arColWidths[nCol]; }
	int  Get_FixRowHeight();
	int  Get_FixColWidth();
	long Get_RowTotalHeight();
	long Get_ColTotalWidth();
	int  Get_GridWidth(BOOL bVScrBar = FALSE);
	int  Get_GridHeight(BOOL bHScrBar = FALSE);

	void Set_FixTextClr(COLORREF clr) { m_FixTextClr = clr; }
	COLORREF Get_FixTextClr() { return m_FixTextClr; }
	void Set_FixBackClr(COLORREF clr) { m_FixBackClr = clr; }
	COLORREF Get_FixBackClr() { return m_FixBackClr; }

	void Set_CellTextClr(COLORREF clr) { m_CellTextClr = clr; }
	COLORREF Get_CellTextClr() { return m_CellTextClr; }
	void Set_CellBackClr(COLORREF clr) { m_CellBackClr = clr; }
	COLORREF Get_CellBackClr() { return m_CellBackClr; }

	void Set_GridBackClr(COLORREF clr) { m_GridBackClr = clr; }
	COLORREF Get_GridBackClr() { return m_GridBackClr; }
	void Set_GridLineClr(COLORREF clr) { m_GridLineClr = clr; }
	COLORREF Get_GridLineClr() { return m_GridLineClr; }

	BOOL IsValid(CCellID& id) { return IsValid(id.m_nRow, id.m_nCol); }
	BOOL IsValid(int nRow, int nCol) { return (nRow >= 0 && nRow < m_nRows && nCol >= 0 && nCol < m_nCols); }
	BOOL IsValidNoFix(CCellID& id) { return IsValidNoFix(id.m_nRow, id.m_nCol); }
	BOOL IsValidNoFix(int nRow, int nCol) { return (nRow >= m_nFixRows && nRow < m_nRows && nCol >= m_nFixCols && nCol < m_nCols); }

	BOOL Delete_AllItems();

	BOOL Set_CellText(int nRow, int nCol, CString str);
	CString Get_CellText(int nRow, int nCol);
	BOOL Set_CellState(int nRow, int nCol, UINT state);
	BOOL Add_CellState(int nRow, int nCol, UINT state);
	BOOL Del_CellState(int nRow, int nCol, UINT state);
	UINT Get_CellState(int nRow, int nCol);
	BOOL Set_CellBackClr(int nRow, int nCol, COLORREF clr);
	COLORREF Get_CellBackClr(int nRow, int nCol);
	BOOL Set_CellTextClr(int nRow, int nCol, COLORREF clr);
	COLORREF Get_CellTextClr(int nRow, int nCol);
	BOOL Set_CellFont(int nRow, int nCol, CString strName, int nSize, BOOL bBold);
	LOGFONT* Get_CellFont(int nRow, int nCol);

	BOOL Set_CellAlign(int nRow, int nCol, int nAlign);
};

///////////////////////////////////////////////////////////////////////////////
