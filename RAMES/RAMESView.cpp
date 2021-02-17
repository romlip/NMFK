
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
		VERIFY(mFont.CreateFont(
			20,                       // nHeight
			0,                        // nWidth
			0,                        // nEscapement
			0,                        // nOrientation
			FW_NORMAL,                // nWeight
			FALSE,                    // bItalic
			FALSE,                    // bUnderline
			0,                        // cStrikeOut
			ANSI_CHARSET,             // nCharSet
			OUT_DEFAULT_PRECIS,       // nOutPrecision
			CLIP_DEFAULT_PRECIS,      // nClipPrecision
			DEFAULT_QUALITY,          // nQuality
			DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily
			_T("Arial")));            // lpszFacename
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

void CRAMESView::OnDraw(CDC* pDC)
{
	pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	mpDC = pDC;

	if (pDoc->bDaneFlag) // jesli dane sa wczytane
	{
		//ustaw granice rysowania w oknie
		GetWindowRect(rectOkna);
		xc = rectOkna.Width() - xp;
		yc = yp + 80;
		int odstepStrukturaWykres = 80;
		//okreslenie wielkosci obszarow do rysowania obiektow
		rectStruktura = CRect(xp, yp, xc, yc);
		rectWykres = CRect(xp, yc + odstepStrukturaWykres, xc, rectOkna.Height() - 50);

		//pobierz xmin i xmax do rysowania struktury
		xMinStrukt = pDoc->mDane.PobierzSiatke()->PobierzXmin();
		xMaxStrukt = pDoc->mDane.PobierzSiatke()->PobierzXmax();
		skalaX = (xc - xp) / (xMaxStrukt - xMinStrukt);

		RysujInfo();

		if (pDoc->bObliczeniaFlag) // jesli obliczenia zostaly wykonane
		{
			//pobierz rozklad temperatury z obliczen
			liczbaPikseliX = xc - xp + 1;
			liczbaPikseliY = yc - yp + 1;
			tArray = new double[liczbaPikseliX]; // tblica z wartosciami temperatury w osi X do rysowania
			pDoc->mObliczenia.WyznaczTwZakresie(xMinStrukt, xMaxStrukt, liczbaPikseliX, tArray);
			UstawZakresT();

			RysujGradientTemperatury();
			RysujWykres();

			delete[] tArray;
		}
		else // jesli dane sa wczytane, ale nie wykonano obliczen
		{
			RysujSiatke();
		}
		RysujStrukture();

	}
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
		pDoc->mObliczenia.UstawUwzglednianieWarunkowI(dlgObliczeniaUstawienia.mComboBoxWarunkiSelection);
		pDoc->bObliczeniaFlag = false;
		Invalidate();
	}
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
		pDoc->mObliczenia.UstawWynikiCzyZakres(dlgWynikiUstawienia.mRadioZakres);
		pDoc->mObliczenia.UstawWynikiZakres(dlgWynikiUstawienia.mEditZakresOd, dlgWynikiUstawienia.mEditZakresDo);

		pDoc->mObliczenia.UstawWynikiRozklad(dlgWynikiUstawienia.mRadioRozklad);
		pDoc->mObliczenia.UstawWynikiGestoscAproksymacji(dlgWynikiUstawienia.mEditRozkladGestoscAproksymacji);
		pDoc->mObliczenia.UstawWynikiLiczbaPunktow(dlgWynikiUstawienia.mEditRozkladLiczbaPunktow);
		pDoc->mObliczenia.UstawWynikiStalyKrok(dlgWynikiUstawienia.mEditRozkladStalyKrok);

		//Invalidate();
	}
}

void CRAMESView::RysujStrukture()
{
	CPen penStruktura(PS_SOLID, 3, RGB(0, 0, 0)); //czarne linie struktury
	CBrush brStruktura(RGB(0, 0, 0));
	Ramka(rectStruktura, penStruktura);
	double skala = rectStruktura.Width() / (xMaxStrukt - xMinStrukt);
	for (auto it_obszar = pDoc->mDane.PobierzSiatke()->PobierzStrukture()->begin(); it_obszar != pDoc->mDane.PobierzSiatke()->PobierzStrukture()->end() - 1; ++it_obszar)
	{
		mpDC->MoveTo(rectStruktura.TopLeft().x + (*(it_obszar->PobierzWezly()->end() - 1))->PobierzX() * skala, rectStruktura.BottomRight().y);
		mpDC->LineTo(rectStruktura.TopLeft().x + (*(it_obszar->PobierzWezly()->end() - 1))->PobierzX() * skala, rectStruktura.BottomRight().y - rectStruktura.Height());
	}
	//os gradientu
	OznaczOsX(rectStruktura);
	//CString strXmin, strXmax;
	//strXmin.Format(_T("%f"), xMinStrukt);
	//strXmax.Format(_T("%f"), xMaxStrukt);
	//mpDC->SelectObject(&mFont);
	//mpDC->SetTextColor(RGB(0, 0, 0));
	//mpDC->SetBkMode(TRANSPARENT);
	//mpDC->TextOut(xp, yc, strXmin);
	//mpDC->TextOut(xc, yc, strXmax);
}

void CRAMESView::RysujSiatke()
{
	CPen penSiatka(PS_SOLID, 1, RGB(0, 200, 0)); // zielone linie siatki
	CBrush brushElementy(COLORREF(RGB(220, 220, 220))); //szare elementy 
	mpDC->SelectObject(&brushElementy);
	mpDC->SelectObject(&penSiatka);
	mpDC->Rectangle(rectStruktura);
	double skala = rectStruktura.Width() / (xMaxStrukt - xMinStrukt);
	for (auto it_wezly = pDoc->mDane.PobierzSiatke()->PobierzWezly()->begin(); it_wezly != pDoc->mDane.PobierzSiatke()->PobierzWezly()->end(); ++it_wezly)
	{
		mpDC->MoveTo(rectStruktura.TopLeft().x + (*it_wezly)->PobierzX() * skala, rectStruktura.BottomRight().y);
		mpDC->LineTo(rectStruktura.TopLeft().x + (*it_wezly)->PobierzX() * skala, rectStruktura.BottomRight().y - rectStruktura.Height());
	}
}

void CRAMESView::Ramka(const CRect &rect, const CPen &pen)
{
	mpDC->SelectObject(pen);
	mpDC->MoveTo(rect.TopLeft());
	mpDC->LineTo(rect.TopLeft().x + rect.Width(), rect.TopLeft().y);
	mpDC->LineTo(rect.BottomRight());
	mpDC->LineTo(rect.BottomRight().x - rect.Width(), rect.BottomRight().y);
	mpDC->LineTo(rect.TopLeft());
}

void CRAMESView::RysujGradientTemperatury()
{	//rysowanie gradientu
	for (int i(0); i <liczbaPikseliX; ++i)
	{
		CPen gradient;
		gradient.CreatePen(PS_SOLID, 1, KonwertujTnaGradient(tArray[i], Tmin, Tmax));
		mpDC->SelectObject(&gradient);
		mpDC->MoveTo(xp + i, yp);
		mpDC->LineTo(xp + i, yc);
	}

	RysujSkaleGradientu();
}

COLORREF CRAMESView::KonwertujTnaGradient(double t,double tMin, double tMax)
{
	int czarnyOffset = 100;
	double gr = (t - tMin) * (5*255 - 2*czarnyOffset) / (tMax - tMin) + czarnyOffset;
	if (gr <= 255)
		return RGB(0, 0, gr);
	else if (gr <= 2 * 255)
		return RGB(0, gr - 255, 255);
	else if (gr <= 3 * 255)
		return RGB(gr - 2*255, 255, 3*255 - gr);
	else if (gr <= 4 * 255)
		return RGB(255, 4*255 - gr, 0);
	else if (gr <= 5 * 255)
		return RGB(5*255 - gr, 0, 0);
}

void CRAMESView::RysujWykres()
{
	//tlo i ramka wykresu
	CPen penWykres;
	CBrush brushWykres(RGB(230, 230, 230));
	mpDC->SelectObject(&brushWykres);
	mpDC->Rectangle(rectWykres);

	//wykres temperatury
	double wykresSkala = rectWykres.Height() / (Tmax - Tmin);
	penWykres.CreatePen(PS_SOLID, 1, RGB(50, 200, 50));
	mpDC->SelectObject(&penWykres);
	for (int px = rectWykres.TopLeft().x; px < rectWykres.TopLeft().x + liczbaPikseliX; ++px)
	{
		mpDC->MoveTo(px, rectWykres.BottomRight().y - (tArray[px - rectWykres.TopLeft().x] - Tmin) * wykresSkala);
		mpDC->LineTo(px, rectWykres.BottomRight().y - (tArray[px - rectWykres.TopLeft().x] - Tmin) * wykresSkala - 2);
	}
	OznaczOsX(rectWykres);
	OznaczOsY(rectWykres);

	//linie obszarow
	CPen penObszary;
	penObszary.CreatePen(PS_DASH, 1, RGB(100, 200, 100));
	mpDC->SelectObject(&penObszary);
	for (auto it_obszar = pDoc->mDane.PobierzSiatke()->PobierzStrukture()->begin(); it_obszar != pDoc->mDane.PobierzSiatke()->PobierzStrukture()->end() - 1; ++it_obszar)
	{
		mpDC->MoveTo(rectWykres.TopLeft().x + (*(it_obszar->PobierzWezly()->end() - 1))->PobierzX() * skalaX, rectWykres.TopLeft().y);
		mpDC->LineTo(rectWykres.TopLeft().x + (*(it_obszar->PobierzWezly()->end() - 1))->PobierzX() * skalaX, rectWykres.BottomRight().y);
	}
}

void CRAMESView::RysujOsie()
{
	//osie
	CPen penOsie;
	penOsie.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	mpDC->SelectObject(&penOsie);

	int szerokoscStrzalki = 5;
	int dlugoscStrzalki = 20;
	//os x
	mpDC->MoveTo(xp, ycWykres);
	mpDC->LineTo(xc, ycWykres);
	mpDC->LineTo(xc - dlugoscStrzalki, ycWykres - szerokoscStrzalki);
	mpDC->MoveTo(xc, ycWykres);
	mpDC->LineTo(xc - dlugoscStrzalki, ycWykres + szerokoscStrzalki);

	//os y
	mpDC->MoveTo(xp, ycWykres);
	mpDC->LineTo(xp, ypWykres);
	mpDC->LineTo(xp - szerokoscStrzalki, ypWykres + dlugoscStrzalki);
	mpDC->MoveTo(xp, ypWykres);
	mpDC->LineTo(xp + szerokoscStrzalki, ypWykres + dlugoscStrzalki);
}

void CRAMESView::RysujInfo()
{
	CString nazwa(pDoc->mPlik.PobierzNazwe()->c_str());
	CString elementyInfo, wezlyInfo;
	elementyInfo.Format(_T("Liczba elementow %d-wezlowych: %d "),pDoc->mDane.PobierzSiatke()->PobierzLiczbeWezlowWelemencie(), pDoc->mDane.PobierzSiatke()->PobierzElementy()->size());
	wezlyInfo.Format(_T("Liczba wezlow: %d"), pDoc->mDane.PobierzSiatke()->PobierzWezly()->size());
	int xInfo, yInfo, odstepY = 5;
	xInfo = 10;
	yInfo = 10;
	mpDC->TextOut(xInfo, yInfo, nazwa);
	mpDC->TextOut(xInfo, yInfo + mpDC->GetTextExtent(nazwa).cy + odstepY, elementyInfo);
	mpDC->TextOut(xInfo, yInfo + +mpDC->GetTextExtent(nazwa).cy + mpDC->GetTextExtent(elementyInfo).cy + 2* odstepY, wezlyInfo);
}

void CRAMESView::RysujSkaleGradientu()
{	
	//rysowanie gradientu
	int offsetSkali = 20;
	int szerokoscSkali = 30;
	rectSkala = CRect(xp - szerokoscSkali - offsetSkali, yp, xp - offsetSkali, yc);

	CPen penSkali;
	penSkali.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	Ramka(rectSkala, penSkali);
	for (int i(0); i < liczbaPikseliY; i++)
	{
		CPen gradient;
		gradient.CreatePen(PS_SOLID, 1, KonwertujTnaGradient(i*(Tmax-Tmin)/liczbaPikseliY + Tmin, Tmin, Tmax));
		mpDC->SelectObject(&gradient);
		mpDC->MoveTo(rectSkala.BottomRight().x, rectSkala.BottomRight().y -i);
		mpDC->LineTo(rectSkala.TopLeft().x, rectSkala.BottomRight().y -i);
	}
	OznaczOsY(rectSkala);
}

void CRAMESView::OznaczOsX(CRect &rect)
{
	CString strXmin, strXmax, strOsX;
	int podpisOffset = 10;

	mpDC->SelectObject(&mFont);
	mpDC->SetTextColor(RGB(0, 0, 0));
	mpDC->SetBkMode(TRANSPARENT);

	strXmin.Format(_T("%4.2f"), xMinStrukt);
	strXmax.Format(_T("%4.2f"), xMaxStrukt);
	strOsX = _T("x [m]");
	mpDC->TextOut(rect.TopLeft().x - mpDC->GetTextExtent(strXmin).cx / 2, rect.BottomRight().y + podpisOffset, strXmin);
	mpDC->TextOut(rect.BottomRight().x - mpDC->GetTextExtent(strXmax).cx / 2, rect.BottomRight().y + podpisOffset, strXmax);
	mpDC->TextOut(rect.Width() / 2 + rect.TopLeft().x + mpDC->GetTextExtent(strOsX).cx / 2, rect.BottomRight().y + podpisOffset, strOsX);
}

void CRAMESView::OznaczOsY(CRect& rect)
{
	CString 	strTmin, strTmax, strOsT;
	int podpisOffset = 10;
	mpDC->SelectObject(&mFont);
	mpDC->SetTextColor(RGB(0, 0, 0));
	mpDC->SetBkMode(TRANSPARENT);

	strTmin.Format(_T("%4.2f"), Tmin);
	strTmax.Format(_T("%4.2f"), Tmax);
	strOsT = _T("T [K]");
	mpDC->TextOut(rect.TopLeft().x - mpDC->GetTextExtent(strTmin).cx - podpisOffset, rect.BottomRight().y - mpDC->GetTextExtent(strTmin).cy / 2, strTmin);
	mpDC->TextOut(rect.TopLeft().x - mpDC->GetTextExtent(strTmax).cx - podpisOffset, rect.TopLeft().y - mpDC->GetTextExtent(strTmax).cy / 2, strTmax);
	mpDC->TextOut(rect.TopLeft().x - mpDC->GetTextExtent(strOsT).cx - podpisOffset, rect.Height()/ 2 + rect.TopLeft().y, strOsT);
}

void CRAMESView::UstawZakresT()
{
	Tmin = tArray[0];
	Tmax = Tmin;

	for (int i(0); i < liczbaPikseliX; ++i)
	{
		if (tArray[i] < Tmin) Tmin = tArray[i];
		else if (tArray[i] > Tmax) Tmax = tArray[i];
	}
}

//////////////////////////////

//for (int i(0); i <= 510; i++)
//{
//	CPen pen;
//	if (i <= 255)
//		pen.CreatePen(PS_SOLID, 2, RGB(i / 2, i, 255 - i / 2));
//	else
//		pen.CreatePen(PS_SOLID, 2, RGB(i / 2, 255 - i, 255 - i / 2));

//	mpDC->SelectObject(&pen);
//	mpDC->MoveTo(xp + 2 * i, yp);
//	mpDC->LineTo(xp + 2 * i, 500);
//}

//for (int i(0); i <5; i++)
//{
//	for (int j(1); j <= 255; j++)
//	{
//		CPen pen;
//		if (i  == 0)
//			pen.CreatePen(PS_SOLID, 1, RGB(0, 0, j));
//		else if (i == 1)
//			pen.CreatePen(PS_SOLID, 1, RGB(0, j, 255));
//		else if (i == 2)
//			pen.CreatePen(PS_SOLID, 1, RGB(j, 255, 255 - j));
//		else if (i == 3)
//			pen.CreatePen(PS_SOLID, 1, RGB(255, 255 - j, 0));
//		else if (i == 4)
//			pen.CreatePen(PS_SOLID, 1, RGB(255 - j, 0, 0));

//		mpDC->SelectObject(&pen);
//		mpDC->MoveTo(1 + i*255+j, yp);
//		mpDC->LineTo(1+ i*255+j, 500);
//	}
//}