// DlgObliczeniaUstawienia.cpp : implementation file
//

#include "pch.h"
#include "RAMES.h"
#include "DlgObliczeniaUstawienia.h"
#include "afxdialogex.h"

// DlgObliczeniaUstawienia dialog

IMPLEMENT_DYNAMIC(DlgObliczeniaUstawienia, CDialog)

DlgObliczeniaUstawienia::DlgObliczeniaUstawienia(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_OBLICZENIA_USTAWIENIA, pParent)
	, mbCheckZagescWstepnie(TRUE)
	, mKrotnoscZageszczenia(1)
	, mComboBoxWarunkiSelection(2) // Payne-Ironsa domyslna metoda
	, mComboRozwiazanieUrSelection(0)
	, mbCheckZagescXkrotnie(TRUE)
	, mRadioLiczbaWezlow(0)
{

}

DlgObliczeniaUstawienia::~DlgObliczeniaUstawienia()
{
}

void DlgObliczeniaUstawienia::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_KROTNOSC_ZAGESZCZENIA, mKrotnoscZageszczenia);
	DDX_Check(pDX, IDC_ZAGESC_WSTEPNIE, mbCheckZagescWstepnie);
	DDX_Text(pDX, IDC_EDIT_LICZBA_WEZLOW, mLiczbaWezlow);
	DDX_Text(pDX, IDC_EDIT_LICZBA_ELEMENTOW, mLiczbaElementow);
	DDX_Control(pDX, IDC_COMBO_WARUNKI_I, mComboBoxCtrl);
	DDX_CBIndex(pDX, IDC_COMBO_WARUNKI_I, mComboBoxWarunkiSelection);
	DDX_Control(pDX, IDC_COMBO_METODA_ROZWIAZANIA_UR, mComboBoxRozwiazanieUr);
	DDX_CBIndex(pDX, IDC_COMBO_METODA_ROZWIAZANIA_UR, mComboRozwiazanieUrSelection);
	DDX_Check(pDX, IDC_CHECK_ZAGESC_XKROTNIE, mbCheckZagescXkrotnie);
	DDX_Radio(pDX, IDC_RADIO_LICZBA_WEZLOW, mRadioLiczbaWezlow);
}


BEGIN_MESSAGE_MAP(DlgObliczeniaUstawienia, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_METODA_ROZWIAZANIA_UR, &DlgObliczeniaUstawienia::OnCbnSelchangeComboMetodaRozwiazaniaUr)
	ON_BN_CLICKED(IDC_BUTTON_GENERUJ, &DlgObliczeniaUstawienia::OnBnClickedButtonGeneruj)
END_MESSAGE_MAP()


// DlgObliczeniaUstawienia message handlers

void DlgObliczeniaUstawienia::OnCbnSelchangeComboMetodaRozwiazaniaUr()
{
	if (mComboBoxRozwiazanieUr.GetCurSel() == 1)
		mComboBoxRozwiazanieUr.SetCurSel(0);
	//mComboRozwiazanieUrSelection = mComboBoxRozwiazanieUr.GetCurSel();
}

BOOL DlgObliczeniaUstawienia::OnInitDialog()
{
	CDialog::OnInitDialog();
	pDoc = (CRAMESDoc*)(((CFrameWnd*)AfxGetMainWnd())->GetActiveView()->GetDocument());

	InicjalizujComboBoxy();
	InicjalizujRadioLiczbaWezlow();
	InicjalizujEditKrotnoscZageszczenia();
	UpdateData(FALSE); // inicjalizuj kontrolki wartosciami poczatkowymi

	AktualizujSiatkaInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void DlgObliczeniaUstawienia::InicjalizujComboBoxy()
{
	//ComboBox Warunki I
	mComboBoxCtrl.InsertString(0, _T("Redukcja układu"));
	mComboBoxCtrl.InsertString(1, _T("Analityczne (nie działa)"));
	mComboBoxCtrl.InsertString(2, _T("Payne'a-Irons'a"));
	mComboBoxWarunkiSelection = 2;

	// ComboBox metoda rozwiazania ukladu rownan
	mComboBoxRozwiazanieUr.InsertString(0, _T("Rozkład Cholesky'ego (dokładna)"));
	mComboBoxRozwiazanieUr.InsertString(1, _T("Nadrelaksacja (iteracyjna, nie zaimplementowano)"));
	mComboRozwiazanieUrSelection = 0;
}

void DlgObliczeniaUstawienia::AktualizujSiatkaInfo()
{
	UpdateData(TRUE);
	mLiczbaWezlow = pDoc->mDane.PobierzSiatke()->PobierzWezly()->size();
	mLiczbaElementow = pDoc->mDane.PobierzSiatke()->PobierzElementy()->size();
	UpdateData(FALSE);
}

void DlgObliczeniaUstawienia::OnBnClickedButtonGeneruj()
{
	UpdateData(TRUE); // zapisz wartosci kontrolek do ich zmiennych
	pDoc->mDane.PobierzSiatke()->UstawLiczbeWezlowWelemencie(mRadioLiczbaWezlow + 2);
	pDoc->mDane.PobierzSiatke()->UstawZageszczanieWstepne(mbCheckZagescWstepnie);
	if(mbCheckZagescXkrotnie)
		pDoc->mDane.PobierzSiatke()->UstawKrotnoscZageszczenia(mKrotnoscZageszczenia);
	else
		pDoc->mDane.PobierzSiatke()->UstawKrotnoscZageszczenia(0);
	pDoc->mDane.PobierzSiatke()->Generuj();
	pDoc->mDane.DodajWezlyWarunkowBrzegowych();

	pDoc->bObliczeniaFlag = false;
	AktualizujSiatkaInfo();

	pDoc->UpdateAllViews(NULL);
	// TODO: Add your control notification handler code here
}

void DlgObliczeniaUstawienia::InicjalizujRadioLiczbaWezlow()
{
	pDoc = (CRAMESDoc*)(((CFrameWnd*)AfxGetMainWnd())->GetActiveView()->GetDocument());
	mRadioLiczbaWezlow = pDoc->mDane.PobierzSiatke()->PobierzLiczbeWezlowWelemencie() - 2;
}

void DlgObliczeniaUstawienia::InicjalizujEditKrotnoscZageszczenia()
{
	pDoc = (CRAMESDoc*)(((CFrameWnd*)AfxGetMainWnd())->GetActiveView()->GetDocument());
	mLiczbaWezlow = pDoc->mDane.PobierzSiatke()->PobierzKrotnoscZageszczenia();
}