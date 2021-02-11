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
	, mEditGestoscAproksymacji(2)
	, mRadioWyniki(0)
{

}

DlgWynikiUstawienia::~DlgWynikiUstawienia()
{
}

void DlgWynikiUstawienia::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO_TYLKO_WEZLY, mCtrlRadioTylkoWezly);
	DDX_Radio(pDX, IDC_RADIO_TYLKO_WEZLY, mRadioWyniki);
	DDX_Control(pDX, IDC_RADIO_APROKSYMACJA, mCtrlRadioAproksymacja);
	DDX_Text(pDX, IDC_EDIT_GESTOSC_APROKSYMACJI, mEditGestoscAproksymacji);
	DDV_MinMaxInt(pDX, mEditGestoscAproksymacji, 2, 100);
}


BEGIN_MESSAGE_MAP(DlgWynikiUstawienia, CDialog)
END_MESSAGE_MAP()


// DlgWynikiUstawienia message handlers


BOOL DlgWynikiUstawienia::OnInitDialog()
{
	CDialog::OnInitDialog();

	//mCtrlRadioTylkoWezly.SetCheck(1);
	//mCtrlRadioAproksymacja.SetCheck(0);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
