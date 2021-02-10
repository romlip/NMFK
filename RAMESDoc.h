
// RAMESDoc.h : interface of the CRAMESDoc class
//


#pragma once

#include "KSiatka.h"
#include "KPlik.h"
#include "KObliczenia.h"
#include "KWektor.h"

class CRAMESDoc : public CDocument
{
protected: // create from serialization only
	CRAMESDoc() noexcept;
	DECLARE_DYNCREATE(CRAMESDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CRAMESDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS

public:
	KPlik* plik;
	KDane* dane;
	KObliczenia* obliczenia;
	KWektor* wektorWynikowy;

	afx_msg void OnDaneWczytaj(); //metoda obslugujaca zdarzenie klikniecia na Dane->Wczytaj
	afx_msg void OnWynikiZapisz();

	afx_msg void OnObliczeniaWykonaj();
	void ClearFlags();
	afx_msg void OnWynikiZapiszjako();
};
