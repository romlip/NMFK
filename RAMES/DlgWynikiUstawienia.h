#pragma once

#include "RAMESDoc.h"

// DlgWynikiUstawienia dialog

class DlgWynikiUstawienia : public CDialog
{
	DECLARE_DYNAMIC(DlgWynikiUstawienia)

public:
	DlgWynikiUstawienia(CWnd* pParent = nullptr);   // standard constructor
	virtual ~DlgWynikiUstawienia();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_WYNIKI_USTAWIENIA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	CButton mCtrlRadioZakres;
	int mRadioZakres;
	double mEditZakresOd;
	double mEditZakresDo;

	CButton mCtrlRadioRozklad;
	int mRadioRozklad;
	int mEditRozkladGestoscAproksymacji;
	unsigned mEditRozkladLiczbaPunktow;
	double mEditRozkladStalyKrok;

private:
	CRAMESDoc* pDoc;
	double mdXmin, mdXmax;
	double mdWynikiXmin, mdWynikiXmax;

};
