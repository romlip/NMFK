#pragma once


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
	CButton mCtrlRadioTylkoWezly;
	CButton mCtrlRadioAproksymacja;
	int mEditGestoscAproksymacji;
	int mRadioWyniki;
};
