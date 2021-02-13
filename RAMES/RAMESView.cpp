
// RAMESView.cpp : implementation of the CRAMESView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "RAMES.h"
#endif

#include "RAMESDoc.h"
#include "RAMESView.h"
#include "DlgObliczeniaUstawienia.h"
#include "DlgWynikiUstawienia.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRAMESView

IMPLEMENT_DYNCREATE(CRAMESView, CView)

BEGIN_MESSAGE_MAP(CRAMESView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CRAMESView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_UPDATE_COMMAND_UI(ID_OBLICZENIA_WYKONAJ, &CRAMESView::OnUpdateObliczeniaWykonaj)
	ON_UPDATE_COMMAND_UI(ID_WYNIKI_ZAPISZ, &CRAMESView::OnUpdateWynikiZapisz)
	ON_UPDATE_COMMAND_UI(ID_WYNIKI_ZAPISZJAKO, &CRAMESView::OnUpdateWynikiZapiszjako)
	ON_COMMAND(ID_OBLICZENIA_USTAWIENIA, &CRAMESView::OnObliczeniaUstawienia)
	ON_UPDATE_COMMAND_UI(ID_OBLICZENIA_USTAWIENIA, &CRAMESView::OnUpdateObliczeniaUstawienia)
	ON_UPDATE_COMMAND_UI(ID_WYNIKI_USTAWIENIA, &CRAMESView::OnUpdateWynikiUstawienia)
	ON_COMMAND(ID_WYNIKI_USTAWIENIA, &CRAMESView::OnWynikiUstawienia)
END_MESSAGE_MAP()

// CRAMESView construction/destruction

CRAMESView::CRAMESView() noexcept
{
	// TODO: add construction code here

}

CRAMESView::~CRAMESView()
{
}

BOOL CRAMESView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CRAMESView drawing

void CRAMESView::OnDraw(CDC* /*pDC*/)
{
	CRAMESDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CRAMESView printing


void CRAMESView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CRAMESView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CRAMESView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CRAMESView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CRAMESView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CRAMESView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CRAMESView diagnostics

#ifdef _DEBUG
void CRAMESView::AssertValid() const
{
	CView::AssertValid();
}

void CRAMESView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRAMESDoc* CRAMESView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRAMESDoc)));
	return (CRAMESDoc*)m_pDocument;
}
#endif //_DEBUG


// CRAMESView message handlers


void CRAMESView::OnUpdateObliczeniaWykonaj(CCmdUI* pCmdUI)
{
	CRAMESDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pCmdUI->Enable(pDoc->bDaneFlag);
	// TODO: Add your command update UI handler code here
}


void CRAMESView::OnUpdateWynikiZapisz(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CRAMESDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pCmdUI->Enable(pDoc->bObliczeniaFlag);
	// TODO: Add your command update UI handler code here
}


void CRAMESView::OnUpdateWynikiZapiszjako(CCmdUI* pCmdUI)
{
	CRAMESDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pCmdUI->Enable(pDoc->bObliczeniaFlag);
}


void CRAMESView::OnObliczeniaUstawienia()
{
	CRAMESDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	DlgObliczeniaUstawienia dlgObliczeniaUstawienia;

	int result = dlgObliczeniaUstawienia.DoModal();

	if (result == IDOK)
	{
		// zageszczanie siatki
		pDoc->mDane.PobierzSiatke()->UstawZageszczanieWstepne(dlgObliczeniaUstawienia.mbZagescWstepnie);
		pDoc->mDane.PobierzSiatke()->UstawKrotnoscZageszczenia(dlgObliczeniaUstawienia.mKrotnoscZageszczenia);
		pDoc->mDane.PobierzSiatke()->Generuj();

		// ustawienia obliczen
		pDoc->obliczenia->UstawUwzglednianieWarunkowI(dlgObliczeniaUstawienia.mComboBoxWarunkiSelection);
		//pDoc->dane->

		Invalidate();
	}
	// TODO: Add your command handler code here
}

void CRAMESView::OnUpdateObliczeniaUstawienia(CCmdUI* pCmdUI)
{
	CRAMESDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pCmdUI->Enable(pDoc->bDaneFlag);
}


void CRAMESView::OnUpdateWynikiUstawienia(CCmdUI* pCmdUI)
{
	CRAMESDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pCmdUI->Enable(pDoc->bObliczeniaFlag);
}

void CRAMESView::OnWynikiUstawienia()
{
	CRAMESDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	DlgWynikiUstawienia dlgWynikiUstawienia;

	int result = dlgWynikiUstawienia.DoModal();

	if (result == IDOK)
	{
		pDoc->obliczenia->UstawWynikiTylkoWezly(dlgWynikiUstawienia.mRadioWyniki);
		pDoc->obliczenia->UstawWynikiGestoscAproksymacji(dlgWynikiUstawienia.mEditGestoscAproksymacji);

		Invalidate();
	}
}
