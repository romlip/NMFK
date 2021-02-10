#pragma once


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
	unsigned mKrotnoscZageszczenia;
	BOOL mbZagescWstepnie;
	unsigned mbLiczbaWezlow;
	unsigned mbLiczbaElementow;
	BOOL mbWarunkiIanalityczne;
	BOOL mbWarunkiIpayneIrons;
	afx_msg void OnCbnSelchangeCombo1();
	CComboBox mComboBoxCtrl;
	virtual BOOL OnInitDialog();
	int mComboBoxWarunkiSelection;
	CComboBox mComboBoxRozwiazanieUr;
	afx_msg void OnCbnSelchangeComboMetodaRozwiazaniaUr();
	int mComboRozwiazanieUrSelection;
};
