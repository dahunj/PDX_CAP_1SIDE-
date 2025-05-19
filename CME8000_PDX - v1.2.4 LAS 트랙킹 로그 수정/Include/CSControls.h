// CSControls.h : 헤더 파일
//
#pragma once

#define COLOR_DEFAULT		0xFFFFFFFE

#define UM_CS_LBUTTON_DOWN	WM_USER+9001
#define UM_CS_LBUTTON_UP	WM_USER+9002

///////////////////////////////////////////////////////////////////////////////
class CButtonCS : public CButton
{
	DECLARE_DYNAMIC(CButtonCS)

public:
	CButtonCS();
	virtual ~CButtonCS();

protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

private:
	CFont		m_Font;		// Font
	COLORREF	m_crText;	// Text Color
	COLORREF	m_crBack;	// Background Color
	HICON		m_hIcon;	// Icon
	CSize		m_szIcon;	// Icon Size
	int			m_nIconPos;	// Icon Position
	DWORD		m_dwStyle;	// Style

	COLORREF Get_SomeColor(COLORREF crOrigin, int nSome);

public:
	enum { emTop, emLeft, emRight };	// Icon Position

	void Set_Text(CString sText);
	void Set_Font(CString strName, int nSize, BOOL bBold = FALSE);
	void Set_Color(COLORREF crText, COLORREF crBack = COLOR_DEFAULT);
	void Set_Icon(UINT nIconId, int nIconPos = emTop);
	void Init_Ctrl(CString sFontName, int nFontSize, BOOL bFontBold, COLORREF crText, COLORREF crBack, UINT nIconId, int nIconPos);
};

///////////////////////////////////////////////////////////////////////////////
class CCheckCS : public CButton
{
	DECLARE_DYNAMIC(CCheckCS)

public:
	CCheckCS();
	virtual ~CCheckCS();

protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg LRESULT OnBmGetCheck(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBmSetCheck(WPARAM wParam, LPARAM lParam);

private:
	CFont		m_Font;		// Font
	COLORREF	m_crText;	// Text Color
	COLORREF	m_crBack;	// Background Color
	HICON		m_hIcon;	// Icon
	CSize		m_szIcon;	// Icon Size
	int			m_nIconPos;	// Icon Position
	DWORD		m_dwStyle;	// Style

	HICON		m_hIconOn;	// On Icon
	HICON		m_hIconOff;	// Off Icon

	BOOL		m_bLeftText;
	BOOL		m_bPushLike;
	BOOL		m_bChecked;
	BOOL		m_bLBtnDown;

	COLORREF Get_SomeColor(COLORREF crOrigin, int nSome);

public:
	enum { emTop, emLeft, emRight };			// Icon Position
	enum { emBlack=1, emRed, emGreen, emBlue };	// Icon Color

	void Set_Text(CString sText);
	void Set_Font(CString strName, int nSize, BOOL bBold = FALSE);
	void Set_Color(COLORREF crText, COLORREF crBack = COLOR_DEFAULT);
	void Set_Icon(UINT nIconId, int nIconPos = emTop);
	void Init_Ctrl(CString sFontName, int nFontSize, BOOL bFontBold, COLORREF crText, COLORREF crBack, UINT nIconId, int nIconPos);
};

///////////////////////////////////////////////////////////////////////////////
class CComboCS : public CComboBox
{
	DECLARE_DYNAMIC(CComboCS)

public:
	CComboCS();
	virtual ~CComboCS();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT /*nCtlColor*/);

private:
	CFont		m_Font;		// Font
	COLORREF	m_crText;	// Text Color
	COLORREF	m_crBack;	// Background Color
	CBrush		m_brBack;

public:
	void Set_Text(CString sText);
	void Set_Font(CString strName, int nSize, BOOL bBold = FALSE);
	void Set_Color(COLORREF crText, COLORREF crBack = COLOR_DEFAULT);
	void Init_Ctrl(CString sFontName, int nFontSize, BOOL bFontBold, COLORREF crText, COLORREF crBack);
};

///////////////////////////////////////////////////////////////////////////////
class CEditCS : public CEdit
{
	DECLARE_DYNAMIC(CEditCS)

public:
	CEditCS();
	virtual ~CEditCS();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT /*nCtlColor*/);

private:
	CFont		m_Font;		// Font
	COLORREF	m_crText;	// Text Color
	COLORREF	m_crBack;	// Background Color
	CBrush		m_brBack;

public:
	void Set_Text(CString sText);
	void Set_Font(CString strName, int nSize, BOOL bBold = FALSE);
	void Set_Color(COLORREF crText, COLORREF crBack = COLOR_DEFAULT);
	void Init_Ctrl(CString sFontName, int nFontSize, BOOL bFontBold, COLORREF crText, COLORREF crBack);
};

///////////////////////////////////////////////////////////////////////////////
class CGroupCS : public CStatic
{
	DECLARE_DYNAMIC(CGroupCS)

public:
	CGroupCS();
	virtual ~CGroupCS();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT /*nCtlColor*/);

private:
	CFont		m_Font;		// Font
	COLORREF	m_crText;	// Text Color
	COLORREF	m_crBack;	// Background Color
	CBrush		m_brBack;

public:
	void Set_Text(CString sText);
	void Set_Font(CString strName, int nSize, BOOL bBold = FALSE);
	void Set_Color(COLORREF crText, COLORREF crBack = COLOR_DEFAULT);
	void Init_Ctrl(CString sFontName, int nFontSize, BOOL bFontBold, COLORREF crText, COLORREF crBack);
};

///////////////////////////////////////////////////////////////////////////////
class CLabelCS : public CStatic
{
	DECLARE_DYNAMIC(CLabelCS)

public:
	CLabelCS();
	virtual ~CLabelCS();

protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT /*nCtlColor*/);

private:
	CFont		m_Font;		// Font
	COLORREF	m_crText;	// Text Color
	COLORREF	m_crBack;	// Background Color
	CBrush		m_brBack;

public:
	void Set_Text(CString sText);
	void Set_Font(CString strName, int nSize, BOOL bBold = FALSE);
	void Set_Color(COLORREF crText, COLORREF crBack = COLOR_DEFAULT);
	void Init_Ctrl(CString sFontName, int nFontSize, BOOL bFontBold, COLORREF crText, COLORREF crBack);
};

///////////////////////////////////////////////////////////////////////////////
class CLedCS : public CButton
{
	DECLARE_DYNAMIC(CLedCS)

public:
	CLedCS();
	virtual ~CLedCS();

protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

private:
	CFont		m_Font;		// Font
	COLORREF	m_crText;	// Text Color
	COLORREF	m_crBack;	// Background Color

	int			m_nSize;	// Image Size
	CDC			m_dcMem;
	HBITMAP		m_hBitmap;
	HBITMAP		m_hOldBitmap;

	BOOL		m_bOn;

	BOOL		m_bLeftText;

public:
	enum { emRed=1, emGreen, emBlue };
	enum { em16, em24, em32, em40, em48 };

	void Set_On(BOOL bOn) { m_bOn = bOn; Invalidate(FALSE); }
	BOOL Get_On() { return m_bOn; }

	void Set_Text(CString sText);
	void Set_Font(CString strName, int nSize, BOOL bBold = FALSE);
	void Set_Color(COLORREF crText, COLORREF crBack = COLOR_DEFAULT);
	void Set_Image(int nColor, int nSize);
	void Init_Ctrl(CString sFontName, int nFontSize, BOOL bFontBold, COLORREF crText, COLORREF crBack, int nImgColor, int nImgSize);
};

///////////////////////////////////////////////////////////////////////////////
class CLedRectCS : public CStatic
{
	DECLARE_DYNAMIC(CLedRectCS)

public:
	CLedRectCS();
	virtual ~CLedRectCS();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();

private:
	BOOL		m_bStatus;		// On=TRUE, Off=FALSE
	COLORREF	m_colorOn;
	COLORREF	m_colorOff;

public:
	void On();
	void Off();
	BOOL Get_Status();
	void Init_Ctrl(COLORREF colorOn, COLORREF colorOff);
};

///////////////////////////////////////////////////////////////////////////////
class CListCS : public CListBox
{
	DECLARE_DYNAMIC(CListCS)

public:
	CListCS();
	virtual ~CListCS();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT /*nCtlColor*/);

private:
	CFont		m_Font;		// Font
	COLORREF	m_crText;	// Text Color
	COLORREF	m_crBack;	// Background Color
	CBrush		m_brBack;

public:
	void Set_Text(CString sText);
	void Set_Font(CString strName, int nSize, BOOL bBold = FALSE);
	void Set_Color(COLORREF crText, COLORREF crBack = COLOR_DEFAULT);
	void Init_Ctrl(CString sFontName, int nFontSize, BOOL bFontBold, COLORREF crText, COLORREF crBack);
};

///////////////////////////////////////////////////////////////////////////////
class CPictureCS : public CStatic
{
	DECLARE_DYNAMIC(CPictureCS)

public:
	CPictureCS();
	virtual ~CPictureCS();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT /*nCtlColor*/);

private:
	COLORREF	m_crText;	// Text Color
	COLORREF	m_crBack;	// Background Color
	CBrush		m_brBack;

public:
	void Set_Color(COLORREF crText, COLORREF crBack = COLOR_DEFAULT);
	void Init_Ctrl(COLORREF crText, COLORREF crBack);
};

///////////////////////////////////////////////////////////////////////////////
class CRadioCS : public CButton
{
	DECLARE_DYNAMIC(CRadioCS)

public:
	CRadioCS();
	virtual ~CRadioCS();

protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg LRESULT OnBmGetCheck(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBmSetCheck(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCsRadioUncheck(WPARAM wParam, LPARAM lParam);

private:
	CFont		m_Font;		// Font
	COLORREF	m_crText;	// Text Color
	COLORREF	m_crBack;	// Background Color
	HICON		m_hIcon;	// Icon
	CSize		m_szIcon;	// Icon Size
	int			m_nIconPos;	// Icon Position
	DWORD		m_dwStyle;	// Style

	HICON		m_hIconOn;	// On Icon
	HICON		m_hIconOff;	// Off Icon

	BOOL		m_bLeftText;
	BOOL		m_bPushLike;
	BOOL		m_bChecked;
	BOOL		m_bLBtnDown;

	COLORREF Get_SomeColor(COLORREF crOrigin, int nSome);

public:
	enum { emTop, emLeft, emRight };			// Icon Position
	enum { emBlack=1, emRed, emGreen, emBlue };	// Icon Color

	void Set_Text(CString sText);
	void Set_Font(CString strName, int nSize, BOOL bBold = FALSE);
	void Set_Color(COLORREF crText, COLORREF crBack = COLOR_DEFAULT);
	void Set_Icon(UINT nIconId, int nIconPos = emTop);
	void Check_Radio(UINT nStart, UINT nEnd);
	void Check_Radio(CRadioCS &rdoStart, CRadioCS &rdoEnd);
	void Init_Ctrl(CString sFontName, int nFontSize, BOOL bFontBold, COLORREF crText, COLORREF crBack, UINT nIconId, int nIconPos);
};

///////////////////////////////////////////////////////////////////////////////
class CStaticCS : public CStatic
{
	DECLARE_DYNAMIC(CStaticCS)

public:
	CStaticCS();
	virtual ~CStaticCS();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT /*nCtlColor*/);

private:
	CFont		m_Font;		// Font
	COLORREF	m_crText;	// Text Color
	COLORREF	m_crBack;	// Background Color
	CBrush		m_brBack;
	BOOL		m_bCircle;

public:
	void Set_Text(CString sText);
	void Set_Font(CString strName, int nSize, BOOL bBold = FALSE);
	void Set_Color(COLORREF crText, COLORREF crBack = COLOR_DEFAULT);
	void Init_Ctrl(CString sFontName, int nFontSize, BOOL bFontBold, COLORREF crText, COLORREF crBack, BOOL bCircle = FALSE);
};

///////////////////////////////////////////////////////////////////////////////
class CTabCS : public CTabCtrl
{
	DECLARE_DYNAMIC(CTabCS)

public:
	CTabCS();
	virtual ~CTabCS();

protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

private:
	CFont		m_Font;		// Font
	COLORREF	m_crSelect;	// Select Tab Color
	COLORREF	m_crNormal;	// Normal Tab Color

public:
	void Set_Text(CString sText);
	void Set_Font(CString strName, int nSize, BOOL bBold = FALSE);
	void Set_Color(COLORREF crSelect, COLORREF crNormal = COLOR_DEFAULT);
	void Init_Ctrl(CString sFontName, int nFontSize, BOOL bFontBold, COLORREF crSelect, COLORREF crNormal);
};

///////////////////////////////////////////////////////////////////////////////
class CTabCtrlCS : public CTabCtrl
{
public:
	CTabCtrlCS();
	virtual ~CTabCtrlCS();

protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

private:
	CFont		m_Font;			// Font
	COLORREF	m_crSelectText;	// Select Text
	COLORREF	m_crNormalText;	// Normal Text
	COLORREF	m_crSelectBack;	// Select Background
	COLORREF	m_crNormalBack;	// Normal Background

public:
	void Set_Text(CString sText);
	void Set_Font(CString strName, int nSize, BOOL bBold = FALSE);
	void Set_TextColor(COLORREF crSelectText, COLORREF crNormalText = COLOR_DEFAULT);
	void Set_BackColor(COLORREF crSelectBack, COLORREF crNormalBack = COLOR_DEFAULT);
	void Init_Ctrl(CString sFontName, int nFontSize, BOOL bFontBold, COLORREF crSelectText, COLORREF crNormalText, COLORREF crSelectBack, COLORREF crNormalBack);
};

///////////////////////////////////////////////////////////////////////////////
