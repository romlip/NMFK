#pragma once
#include "RAMESDoc.h"

// DlgObliczeniaUstawienia dialog

class DlgObliczeniaUstawienia : public CDialog
{
	DECLARE_DYNAMIC(DlgObliczeniaUstawienia)

public:
	DlgObliczeniaUstawienia(CWnd* pParent = nullptr);   // standard constructor
	virtual ~DlgObliczeniaUstawienia();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_OBLICZENIA_USTAWIENIA };
#endif

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CRAMESDoc *pDoc;
	BOOL mbCheckZagescWstepnie;
	unsigned mLiczbaWezlow;
	unsigned mLiczbaElementow;
	unsigned mKrotnoscZageszczenia;

	CComboBox mComboBoxCtrl;
	int mComboBoxWarunkiSelection;

	CComboBox mComboBoxRozwiazanieUr;
	int mComboRozwiazanieUrSelection;

	virtual BOOL OnInitDialog();
	void InicjalizujComboBoxy();
	void AktualizujSiatkaInfo();
	afx_msg void OnCbnSelchangeComboMetodaRozwiazaniaUr();
	afx_msg void OnBnClickedButtonGeneruj();
	void InicjalizujRadioLiczbaWezlow();
	void InicjalizujEditKrotnoscZageszczenia();
	BOOL mbCheckZagescXkrotnie;
	int mRadioLiczbaWezlow;
};
