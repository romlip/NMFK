
// RAMESDoc.cpp : implementation of the CRAMESDoc class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "RAMES.h"
#endif

#include "RAMESDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <string>

// CRAMESDoc

IMPLEMENT_DYNCREATE(CRAMESDoc, CDocument)

BEGIN_MESSAGE_MAP(CRAMESDoc, CDocument)
	ON_COMMAND(ID_DANE_WCZYTAJ, &CRAMESDoc::OnDaneWczytaj)
	ON_COMMAND(ID_WYNIKI_ZAPISZ, &CRAMESDoc::OnWynikiZapisz)
	ON_COMMAND(ID_OBLICZENIA_WYKONAJ, &CRAMESDoc::OnObliczeniaWykonaj)
END_MESSAGE_MAP()


// CRAMESDoc construction/destruction

CRAMESDoc::CRAMESDoc() noexcept
{
	// TODO: add one-time construction code here
	dane = new KDane();
	plik = new KPlik();
	obliczenia = new KObliczenia();

	KMacierz mojamacierz;
}

CRAMESDoc::~CRAMESDoc()
{
	if (plik)
		delete plik;
	if (dane)
		delete dane;
}

BOOL CRAMESDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CRAMESDoc serialization

void CRAMESDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CRAMESDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CRAMESDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CRAMESDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CRAMESDoc diagnostics

#ifdef _DEBUG
void CRAMESDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRAMESDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CRAMESDoc commands


void CRAMESDoc::OnDaneWczytaj()
{
	// TODO: Add your command handler code here
	//CString fileName_CS;
	CString pathName_CS;
	char nazwa_pliku[250];

	TCHAR szFilters[] = _T("Pliki danych(*.txt)|*.txt|Syckie pliki (*.*)|*.*||");

	CFileDialog fileDlg(TRUE, _T("txt"), _T("*.txt"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);

	if (fileDlg.DoModal() == IDOK) {
		pathName_CS = fileDlg.GetPathName();
		//fileName_CS = fileDlg.GetFileTitle();

		sprintf_s(nazwa_pliku, "%S", pathName_CS);

		plik->WczytajDane(nazwa_pliku, dane);
	}
}


void CRAMESDoc::OnWynikiZapisz()
{
	// TODO: Add your command handler code here
	//CString pathName_CS;
	//char nazwa_pliku[250];

	//TCHAR szFilters[] = _T("Pliki struktur (*.txt)|*.txt|Syckie pliki (*.*)|*.*||");

	//CFileDialog fileDlg(TRUE, _T("wyniki.txt"), _T("*.txt"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);

	//if (fileDlg.DoModal() == IDOK) {
	//	pathName_CS = fileDlg.GetPathName();
	//	//fileName_CS = fileDlg.GetFileTitle();

	//	sprintf_s(nazwa_pliku, "%S", pathName_CS);

	//	
	//}
	plik->ZapiszWynik("wyniki.txt", dane, obliczenia);
}


void CRAMESDoc::OnObliczeniaWykonaj()
{
	wektorWynikowy = obliczenia->Licz(dane);
	// TODO: Add your command handler code here
}
