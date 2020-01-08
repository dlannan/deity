// FontMakerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FontMaker.h"
#include "FontMakerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFontMakerDlg dialog

CFontMakerDlg::CFontMakerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFontMakerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFontMakerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFontMakerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFontMakerDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFontMakerDlg, CDialog)
	//{{AFX_MSG_MAP(CFontMakerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_LOADFONT, OnLoadfont)
	ON_COMMAND(ID_SAVEFONT, OnSavefont)
	ON_COMMAND(ID_VIEWFONT, OnViewfont)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFontMakerDlg message handlers

BOOL CFontMakerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	RECT fs; CDialog::GetWindowRect(&fs);	// Full windows size
	RECT cs; CDialog::GetClientRect(&cs);

	int wd = (fs.right - fs.left) - cs.right;
	int ht = (fs.bottom - fs.top) - cs.bottom;

	CDialog::SetWindowPos(NULL, fs.left,fs.top,512 + wd,512 + ht,NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFontMakerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFontMakerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this); // device context for painting
		// Draw the current font
		CBrush col;
		CRect rect;
		GetClientRect(&rect);

		CDialog::SetFont(&m_font,TRUE); 
		dc.SelectObject(m_font);

		col.CreateSolidBrush(0x000000);
		dc.SetTextColor(0xFFFFFF);
		dc.SetBkColor(0x000000);
		dc.FillRect(&rect,&col);

		// Print out the 255 char set to the window
		int largest = 0;
		for(int i = 0; i<256; i++)
		{
			char	str[2];
			str[0] = i;
			str[1] = 0;
			dc.TextOut(rect.right / 16 * (i%16), (rect.bottom)/16 * (i/16), (LPCTSTR)str, 1);
		}
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFontMakerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CFontMakerDlg::OnLoadfont() 
{
	// Open file dialog to select a font
	dlgdc = CFontMakerDlg::GetDC();

	LOGFONT		fontinf;
	fontinf.lfHeight = 10;
	strcpy(fontinf.lfFaceName,"Arial");

	CFontDialog fonts (&fontinf, CF_SCREENFONTS|CF_INITTOLOGFONTSTRUCT); 
	
	if(fonts.DoModal() == IDCANCEL) return;

	m_font.DeleteObject(); 
	// Set the current client window font to the loaded font
	m_font.CreateFontIndirect(&fontinf); 
	

	CDialog::SetFont(&m_font,TRUE); 
	dlgdc->SelectObject(m_font);

	RECT	winsz;
	CDialog::GetClientRect(&winsz);

	CBrush col;
	col.CreateSolidBrush(0x000000);
	dlgdc->SetTextColor(0xFFFFFF);
	dlgdc->SetBkColor(0x000000);
	dlgdc->FillRect(&winsz,&col);

	dlgdc->GetOutputCharWidth(0,255,widthTable);

	// Print out the 255 char set to the window
	int largest = 512/16;
	for(int i = 0; i<256; i++)
	{
		char	str[2];
		str[0] = i;
		str[1] = 0;
		dlgdc->TextOut(winsz.right / 16 * (i%16), (winsz.bottom)/16 * (i/16), (LPCTSTR)str, 1);
	}

	widthTable[0] = largest;
	CDialog::ReleaseDC(dlgdc);
}

void CFontMakerDlg::OnSavefont() 
{
	// Write out the widthTable, along with the Texture data

	dlgdc = CFontMakerDlg::GetDC();
    InitCommonControls(); 

	char bigBuff[2048] = "";  // maximum common dialog buffer size
	char szFilter[] = "Homer Font Files (*.hff)|*.hff||";
	CFileDialog dlg(FALSE, NULL, NULL, OFN_HIDEREADONLY, szFilter);

	// Modify OPENFILENAME members directly to point to bigBuff
	dlg.m_ofn.lpstrFile = bigBuff;
	dlg.m_ofn.nMaxFile = sizeof(bigBuff);

	if(dlg.DoModal() == IDCANCEL) return;

	FILE *handle = fopen(dlg.m_ofn.lpstrFile,"wb");
	ASSERT(handle != NULL);

	// Font width table first
	fwrite(widthTable,sizeof(int),256,handle);

	RECT	winsz;
	CDialog::GetClientRect(&winsz);

	CString winText = "Font Maker - Saving";
	CFontMakerDlg::SetWindowText(winText);

	// Pixel data info in RGBA format - 640 x 512
	for(int y = 0; y<winsz.bottom; y++)
	{
		for(int x = 0; x<winsz.right; x++)
		{
			unsigned int rgba = dlgdc->GetPixel(x,y);
			fwrite(&rgba,sizeof(unsigned int),1,handle);
		}

		if((y % 6) == 0)
		{
			winText = "Font Maker - Saving "; 
			itoa((100 * y) / winsz.bottom,bigBuff,10); 
			winText += bigBuff;
			winText += "% ";
			CFontMakerDlg::SetWindowText(winText);
		}
	}
	fclose(handle);

	winText = "Font Maker";
	CFontMakerDlg::SetWindowText(winText);

	CDialog::ReleaseDC(dlgdc);
}

void CFontMakerDlg::OnViewfont() 
{
	// View a previous created HFF
	dlgdc = CFontMakerDlg::GetDC();

	char bigBuff[2048] = "";  // maximum common dialog buffer size
	char szFilter[] = "Homer Font Files (*.hff)|*.hff||";
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);

	// Modify OPENFILENAME members directly to point to bigBuff
	dlg.m_ofn.lpstrFile = bigBuff;
	dlg.m_ofn.nMaxFile = sizeof(bigBuff);

	if(dlg.DoModal() == IDCANCEL) return;

	FILE *handle = fopen(dlg.m_ofn.lpstrFile,"rb");
	ASSERT(handle != NULL);

	// Font width table first
	fread(widthTable,1,sizeof(int) * 256,handle);

	CString winText = "Font Maker - Loading";
	CFontMakerDlg::SetWindowText(winText);

	// Pixel data info in RGBA format - 512 x 512
	for(int y = 0; y<512; y++)
	{
		for(int x = 0; x<512; x++)
		{
			unsigned int rgba;
			fread(&rgba,sizeof(unsigned int),1,handle);
			dlgdc->SetPixel(x,y,rgba);
		}

		if((y % 6) == 0)
		{
			winText = "Font Maker - Loading "; 
			itoa((100 * y) / 512,bigBuff,10); 
			winText += bigBuff;
			winText += "% ";
			CFontMakerDlg::SetWindowText(winText);
		}
	}

	winText = "Font Maker";
	CFontMakerDlg::SetWindowText(winText);

	fclose(handle);
	CDialog::ReleaseDC(dlgdc);
}
