// ApolloDLLaw.cpp : implementation file
//

#include "stdafx.h"
#include "ApolloDLL.h"
#include "ApolloDLLaw.h"

#include "Atlbase.h"

#include <initguid.h>
#include "ObjModel\BLDGUID.H"

#ifdef _PSEUDO_DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// This is called immediately after the custom AppWizard is loaded.  Initialize
//  the state of the custom AppWizard here.
void CApolloDLLAppWiz::InitCustomAppWiz()
{
	// There are no steps in this custom AppWizard.
	SetNumberOfSteps(0);

	// Add build step to .hpj if there is one
	m_Dictionary[_T("HELP")] = _T("1");

	// Inform AppWizard that we're making a DLL.
	m_Dictionary[_T("PROJTYPE_DLL")] = _T("1");

	// TODO: Add any other custom AppWizard-wide initialization here.
}

// This is called just before the custom AppWizard is unloaded.
void CApolloDLLAppWiz::ExitCustomAppWiz()
{
	// TODO: Add code here to deallocate resources used by the custom AppWizard
}

// This is called when the user clicks "Create..." on the New Project dialog
CAppWizStepDlg* CApolloDLLAppWiz::Next(CAppWizStepDlg* pDlg)
{
	ASSERT(pDlg == NULL);	// By default, this custom AppWizard has no steps

	// Set template macros based on the project name entered by the user.

	// Get value of $$root$$ (already set by AppWizard)
	CString strRoot;
	m_Dictionary.Lookup(_T("root"), strRoot);
	
	// Set value of $$Doc$$, $$DOC$$
	CString strDoc = strRoot.Left(6);
	m_Dictionary[_T("Doc")] = strDoc;
	strDoc.MakeUpper();
	m_Dictionary[_T("DOC")] = strDoc;

	// Set value of $$MAC_TYPE$$
	strRoot = strRoot.Left(4);
	int nLen = strRoot.GetLength();
	if (strRoot.GetLength() < 4)
	{
		CString strPad(_T(' '), 4 - nLen);
		strRoot += strPad;
	}
	strRoot.MakeUpper();
	m_Dictionary[_T("MAC_TYPE")] = strRoot;

	// Return NULL to indicate there are no more steps.  (In this case, there are
	//  no steps at all.)
	return NULL;
}

void CApolloDLLAppWiz::CustomizeProject(IBuildProject* pProject)
{
	// TODO: Add code here to customize the project.  If you don't wish
	//  to customize project, you may remove this virtual override.
	
	// This is called immediately after the default Debug and Release
	//  configurations have been created for each platform.  You may customize
	//  existing configurations on this project by using the methods
	//  of IBuildProject and IConfiguration such as AddToolSettings,
	//  RemoveToolSettings, and AddCustomBuildStep. These are documented in
	//  the Developer Studio object model documentation.

	// WARNING!!  IBuildProject and all interfaces you can get from it are OLE
	//  COM interfaces.  You must be careful to release all new interfaces
	//  you acquire.  In accordance with the standard rules of COM, you must
	//  NOT release pProject, unless you explicitly AddRef it, since pProject
	//  is passed as an "in" parameter to this function.  See the documentation
	//  on CCustomAppWiz::CustomizeProject for more information.

	IConfigurations* pConfigs = NULL;
	
	pProject->get_Configurations(&pConfigs);
	ASSERT(pConfigs);
	
	CComPtr<IUnknown> pUnk;
	CComQIPtr<IEnumVARIANT, &IID_IEnumVARIANT> pNewEnum;
	
	if (SUCCEEDED(pConfigs->get__NewEnum(&pUnk)) && pUnk != NULL)
	{
		pNewEnum = pUnk;
		VARIANT varConfig;
		CComQIPtr<IConfiguration, &IID_IConfiguration> pConfig;
		
		while (pNewEnum->Next(1, &varConfig, NULL) == S_OK)
		{
			
			ASSERT (varConfig.vt == VT_DISPATCH);
			pConfig = varConfig.pdispVal;
			
			VariantClear(&varConfig);
			
			// Add Control registration custom build step
			VARIANT reserved;

			CComBSTR bstrTool = "cl.exe";
			CComBSTR bstrOption = "/D \"_WINDLL\" /D \"_AFXDLL\"";
			
			pConfig->RemoveToolSettings(bstrTool, bstrOption, reserved);	
			
			bstrTool = "cl.exe";
			bstrOption = "/MDd";

			pConfig->RemoveToolSettings(bstrTool, bstrOption, reserved);	

			bstrTool = "link.exe";
			bstrOption = "kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib";

			pConfig->RemoveToolSettings(bstrTool, bstrOption, reserved);	

			bstrTool = "rc.exe";
			bstrOption = "/d \"_AFXDLL\"";

			pConfig->RemoveToolSettings(bstrTool, bstrOption, reserved);	

			bstrTool = "cl.exe";
			bstrOption = "/MTd";
			
			pConfig->AddToolSettings(bstrTool, bstrOption, reserved);	

			bstrTool = "cl.exe";
			bstrOption = "/Gz";

			pConfig->AddToolSettings(bstrTool, bstrOption, reserved);	
		}
	}
		
}


// Here we define one instance of the CApolloDLLAppWiz class.  You can access
//  m_Dictionary and any other public members of this class through the
//  global ApolloDLLaw.
CApolloDLLAppWiz ApolloDLLaw;

