#ifndef MainDialog_H
#define MainDialog_H

#include "resource.h"       // main symbols
#include <atlhost.h>


// CMainDialog
class CMainDialog : 
	public CAxDialogImpl<CMainDialog>
{
private:
	bool split;
	double sigma;

public:
	CMainDialog()
	{
		this->split=true;
		this->sigma=5;
	}

	~CMainDialog()	{}

	bool getSplit(){return this->split;};

	double getSigma(){return this->sigma;};

	enum { IDD = IDD_MAINDIALOG };

BEGIN_MSG_MAP(CMainDialog)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDOK, BN_CLICKED, OnClickedOK)
	COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnClickedCancel)
	COMMAND_HANDLER(IDC_CHECK1, BN_CLICKED, OnBnClickedCheck1)
	NOTIFY_HANDLER(IDC_SLIDER1, NM_CUSTOMDRAW, OnNMCustomdrawSlider1)
	COMMAND_HANDLER(IDYES, BN_CLICKED, OnBnClickedYes)
	CHAIN_MSG_MAP(CAxDialogImpl<CMainDialog>)
END_MSG_MAP()

// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CAxDialogImpl<CMainDialog>::OnInitDialog(uMsg, wParam, lParam, bHandled);
		bHandled = TRUE;
		SendDlgItemMessage(IDC_SLIDER1,TBM_SETRANGE,(WPARAM)TRUE, (LPARAM) MAKELONG(1,50));
		SendDlgItemMessage(IDC_SLIDER1,TBM_SETPOS,(WPARAM)TRUE, (LPARAM) 7);
		SendDlgItemMessage(IDC_CHECK1,BM_SETCHECK,(WPARAM)BST_CHECKED, (LPARAM) 0);

		return 1;  // Let the system set the focus
	}

	LRESULT OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}

	LRESULT OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}
	LRESULT OnBnClickedCheck1(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnNMCustomdrawSlider1(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
	LRESULT OnBnClickedYes(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};


#endif