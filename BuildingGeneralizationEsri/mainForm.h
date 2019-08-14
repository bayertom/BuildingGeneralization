#pragma once


// mainForm dialog

class mainForm : public CDialog
{
	DECLARE_DYNAMIC(mainForm)

public:
	mainForm(CWnd* pParent = NULL);   // standard constructor
	virtual ~mainForm();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
