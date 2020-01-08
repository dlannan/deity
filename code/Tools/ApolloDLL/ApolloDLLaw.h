#if !defined(AFX_APOLLODLLAW_H__604AC0A7_6238_11D5_AE6A_444553540000__INCLUDED_)
#define AFX_APOLLODLLAW_H__604AC0A7_6238_11D5_AE6A_444553540000__INCLUDED_

// ApolloDLLaw.h : header file
//

class CDialogChooser;

// All function calls made by mfcapwz.dll to this custom AppWizard (except for
//  GetCustomAppWizClass-- see ApolloDLL.cpp) are through this class.  You may
//  choose to override more of the CCustomAppWiz virtual functions here to
//  further specialize the behavior of this custom AppWizard.
class CApolloDLLAppWiz : public CCustomAppWiz
{
public:
	virtual CAppWizStepDlg* Next(CAppWizStepDlg* pDlg);
		
	virtual void InitCustomAppWiz();
	virtual void ExitCustomAppWiz();
	virtual void CustomizeProject(IBuildProject* pProject);
};

// This declares the one instance of the CApolloDLLAppWiz class.  You can access
//  m_Dictionary and any other public members of this class through the
//  global ApolloDLLaw.  (Its definition is in ApolloDLLaw.cpp.)
extern CApolloDLLAppWiz ApolloDLLaw;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_APOLLODLLAW_H__604AC0A7_6238_11D5_AE6A_444553540000__INCLUDED_)
