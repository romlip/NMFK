
// RAMESView.h : interface of the CRAMESView class
//

#pragma once


class CRAMESView : public CView
{
protected: // create from serialization only
	CRAMESView() noexcept;
	DECLARE_DYNCREATE(CRAMESView)

// Attributes
public:
	CRAMESDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CRAMESView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnUpdateObliczeniaWykonaj(CCmdUI* pCmdUI);
	afx_msg void OnUpdateWynikiZapisz(CCmdUI* pCmdUI);
	afx_msg void OnUpdateWynikiZapiszjako(CCmdUI* pCmdUI);
	afx_msg void OnObliczeniaUstawienia();
	afx_msg void OnUpdateObliczeniaUstawienia(CCmdUI* pCmdUI);
	afx_msg void OnUpdateWynikiUstawienia(CCmdUI* pCmdUI);
	afx_msg void OnWynikiUstawienia();

private:
	CRAMESDoc* pDoc;
	CDC* mpDC;
	int xp = 150, yp = 50;
	int xc, yc, ycWykres, ypWykres;
	double xMinStrukt, xMaxStrukt;
	double Tmin, Tmax;
	double skalaX;
	double* tArray;
	int liczbaPikseliX, liczbaPikseliY;
	CFont mFont;

	CRect rectWykres;
	CRect rectSkala;
	CRect rectStruktura;

	void RysujStrukture();
	void RysujSiatke();
	void RysujWykres();
	void RysujOsie();
	void OznaczOsie();
	void RysujSkaleGradientu();
	void OznaczOsX(CRect& rect);
	void OznaczOsY(CRect& rect);
	void Ramka(const CRect& rect, const CPen& pen);
	void RysujGradientTemperatury();
	COLORREF KonwertujTnaGradient(double t, double tMin, double tMax);

};

#ifndef _DEBUG  // debug version in RAMESView.cpp
inline CRAMESDoc* CRAMESView::GetDocument() const
   { return reinterpret_cast<CRAMESDoc*>(m_pDocument); }
#endif

