// mainForm.cpp : implementation file
//

#include "stdafx.h"
#include "mainForm.h"


// mainForm dialog

IMPLEMENT_DYNAMIC(mainForm, CDialog)

mainForm::mainForm(CWnd* pParent /*=NULL*/)
	: CDialog(mainForm::IDD, pParent)
{

}

mainForm::~mainForm()
{
}

void mainForm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(mainForm, CDialog)
END_MESSAGE_MAP()


// mainForm message handlers
