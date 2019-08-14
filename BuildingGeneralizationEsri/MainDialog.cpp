#include "stdafx.h"
#include "MainDialog.h"
#include <stdio.h>
#include <string>
#include <stdio.h>

// CMainDialog
LRESULT CMainDialog::OnBnClickedCheck1(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	if (IsDlgButtonChecked(IDC_CHECK1)==BST_CHECKED)
	{
		split=true;
	}
	else split=false;
	return 0;
}

LRESULT CMainDialog::OnNMCustomdrawSlider1(int idCtrl, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	sigma=SendDlgItemMessage(IDC_SLIDER1,TBM_GETPOS);
	TCHAR buf[20];
	wsprintf(buf,TEXT("%d"),(int)sigma);
	SetDlgItemText(IDC_STATIC1,buf);
	return 0;
}

LRESULT CMainDialog::OnBnClickedYes(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// TODO: Add your control notification handler code here
	EndDialog(wID);
	return 0;
}
