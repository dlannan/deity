// LoadProgress.cpp : implementation file
//

#include "stdafx.h"
#include "FontMaker.h"
#include "LoadProgress.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// LoadProgress dialog


LoadProgress::LoadProgress(CWnd* pParent /*=NULL*/)
	: CDialog(LoadProgress::IDD, pParent)
{
	//{{AFX_DATA_INIT(LoadProgress)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void LoadProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(LoadProgress)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(LoadProgress, CDialog)
	//{{AFX_MSG_MAP(LoadProgress)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// LoadProgress message handlers
