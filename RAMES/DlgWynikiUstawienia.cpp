// DlgWynikiUstawienia.cpp : implementation file
//

#include "pch.h"
#include "RAMES.h"
#include "DlgWynikiUstawienia.h"
#include "afxdialogex.h"



// DlgWynikiUstawienia dialog

IMPLEMENT_DYNAMIC(DlgWynikiUstawienia, CDialog)

DlgWynikiUstawienia::DlgWynikiUstawienia(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_WYNIKI_USTAWIENIA, pParent)
	, mRadioZakres(0)
	, mEditZakresOd(0)
	, mEditZakresDo(1)
	, mEditRozkladGestoscAproksymacji(2)
	, mRadioRozklad(0)
	, mEditRozkladLiczbaPunktow(100)
	, mEditRozkladStalyKrok(0.1)
{

}

DlgWynikiUstawienia::~DlgWynikiUstawienia()
{
}

void DlgWynikiUstawienia::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_RADIO_ZAKRES, mCtrlRadioZakres);
	DDX_Radio(pDX, IDC_RADIO_ZAKRES, mRadioZakres);
	DDV_MinMaxInt(pDX, mRadioRozklad, 0, 1);
	DDX_Text(pDX, IDC_EDIT_ZAKRES_OD, mEditZakresOd);
	DDV_MinMaxDouble(pDX, mEditZakresOd, mdXmin, mdXmax);
	DDX_Text(pDX, IDC_EDIT_ZAKRES_DO, mEditZakresDo);
	DDV_MinMaxDouble(pDX, mEditZakresDo, mdXmin, mdXmax);

	DDX_Control(pDX, IDC_RADIO_ROZKLAD, mCtrlRadioRozklad);
	DDX_Radio(pDX, IDC_RADIO_ROZKLAD, mRadioRozklad);
	DDV_MinMaxInt(pDX, mRadioRozklad, 0, 3);

	DDX_Text(pDX, IDC_EDIT_GESTOSC_APROKSYMACJI, mEditRozkladGestoscAproksymacji);
	DDV_MinMaxInt(pDX, mEditRozkladGestoscAproksymacji, 1, 100);
	DDX_Text(pDX, IDC_EDIT_LICZBA_PUNKTOW, mEditRozkladLiczbaPunktow);
	DDX_Text(pDX, IDC_EDIT_STALY_KROK, mEditRozkladStalyKrok);
	DDV_MinMaxDouble(pDX, mEditZakresOd, 0, mdXmax);
}

BEGIN_MESSAGE_MAP(DlgWynikiUstawienia, CDialog)
END_MESSAGE_MAP()

// DlgWynikiUstawienia message handlers

BOOL DlgWynikiUstawienia::OnInitDialog()
{
	CDialog::OnInitDialog();
	pDoc = (CRAMESDoc*)(((CFrameWnd*)AfxGetMainWnd())->GetActiveView()->GetDocument());

	mdXmin = pDoc->mDane.PobierzSiatke()->PobierzXmin();
	mdXmax = pDoc->mDane.PobierzSiatke()->PobierzXmax();
	mEditZakresOd = mdXmin;
	mEditZakresDo = mdXmax;
	mEditRozkladStalyKrok = (mdXmax - mdXmin) / 100;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
