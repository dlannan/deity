// FontMakerDlg.h : header file
//

#if !defined(AFX_FONTMAKERDLG_H__C468F939_EC1A_4CA6_AA9D_BAF492BE932D__INCLUDED_)
#define AFX_FONTMAKERDLG_H__C468F939_EC1A_4CA6_AA9D_BAF492BE932D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CFontMakerDlg dialog

class CFontMakerDlg : public CDialog
{
// Construction
public:
	CFontMakerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CFontMakerDlg)
	enum { IDD = IDD_FONTMAKER_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFontMakerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON	m_hIcon;
	CFont	m_font;
	CDC		*dlgdc;
	int		widthTable[256];

	// Generated message map functions
	//{{AFX_MSG(CFontMakerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLoadfont();
	afx_msg void OnSavefont();
	afx_msg void OnViewfont();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FONTMAKERDLG_H__C468F939_EC1A_4CA6_AA9D_BAF492BE932D__INCLUDED_)
