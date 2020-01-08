// ZEUS debug module

// MS SHITTY TEMPLATE SUPPORT!
#pragma warning(disable:4786)

#include "..\\System\\System.h"

#include "ZEUS_debug.h"
#include "ZEUS.h"

#include "resource.h"

namespace ZEUS
{
	namespace Debug
	{
		Data::PageType		debuglog;	// Here we have a String class vector.. page of info

//		ApolloVec			*apolloList;
		ApolloMap			*apolloList;
		Packet::PackClass	*inputbuffer;
		int					inputbufferSize = 0;

		unsigned int	timestamp;		// Timestamp on or off...
		unsigned long	accumulator;	// Accumulated millisecond counter - we stop when the debugger stops
		LARGE_INTEGER	countsPerSec;
		LARGE_INTEGER	startTime;

		void TimeStamp(void);

		HTREEITEM AddChildNode(char *text, HTREEITEM parent, DWORD res);
		HTREEITEM AddParentNode(char *text, HTREEITEM parent, DWORD res);

		void AddModuleToTree(int index);
		void AddPacketToTree(int index);

//		Window::WindowClass *win;			// Main game window!
//		Window::WindowClass	*debugwin;

		HWND	DebugDialog;
		HWND	ProfileDialog;

		HIMAGELIST himl;	// Button list
		int disIcon;
		int enaIcon;

		vector<unsigned int> modList;

		#define ARRAYSIZE(a)    (sizeof(a)/sizeof((a)[0]))  

		// ---------------------------------------------------------------------------------------
		// Function prototypes
		void AddModulesToTree(void);
		

		// ---------------------------------------------------------------------------------------
		// HTML window for profiling, and module info

		int GetHeightFromPoints( int pts) 
		{    
			HDC hdc = GetDC (NULL);
			int height = MulDiv(-pts, GetDeviceCaps (hdc, LOGPIXELSY), 72);
			ReleaseDC (NULL, hdc);
			return height; 
		}
		
		BOOL CALLBACK Dialog2Proc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
		{
			switch(umsg)
			{ 
			case WM_INITDIALOG: return FALSE; 
			case WM_CLOSE:
			case WM_DESTROY: EndDialog(hwnd, FALSE); return TRUE;
			case WM_COMMAND: switch(LOWORD(wparam)) 
				{ 
				case IDOK: 
					EndDialog(hwnd, TRUE); return TRUE;
					break;
				case IDCANCEL: EndDialog(hwnd, FALSE); return TRUE;
					break;
				}
				break;
			}
			return FALSE;
		}

		void ProfileWindow(char *name, Data::PageType *page)
		{
			ProfileDialog = CreateDialog(ZEUS::instance, MAKEINTRESOURCE(IDD_DIALOG2), (HWND)NULL, Dialog2Proc);
			SetWindowText(ProfileDialog,name);

			HFONT hfont = CreateFont (GetHeightFromPoints(8), 0, 0, 0, 400,
										0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
										CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
										DEFAULT_PITCH | FF_SWISS,
										TEXT("Courier")); 

			// Set the packet list font
			SendMessage( GetDlgItem(ProfileDialog,IDC_PROFILELIST1),
							WM_SETFONT, (WPARAM) hfont,
							MAKELPARAM(TRUE, 0));


			for(int i=0; i<page->size(); i++)
			{
				SendMessage( GetDlgItem(ProfileDialog,IDC_PROFILELIST1),
								LB_ADDSTRING,0,(LPARAM) (LPCTSTR) (*page)[i].String() );
			}
			
			SendMessage( GetDlgItem(ProfileDialog,IDC_PROFILELIST1), LB_SETHORIZONTALEXTENT,
							1000, 0L);
				
			ShowWindow(ProfileDialog,SW_SHOW);
		}
				
		// ---------------------------------------------------------------------------------------
		// Debugging - Cycle stepping
		unsigned long	cyclecounter = 0;
		bool			cyclecount = false;

		bool CycleCount(void)
		{
			return cyclecount;
		}

		void CycleCountDec(void)
		{
			cyclecounter--;
			if(cyclecounter<=0)
			{
				cyclecount = false;
				cyclecounter = 0;

				// Count complete update the debug window
				// Clear the packet window
				TreeView_DeleteAllItems( GetDlgItem(DebugDialog,IDC_TREE1));	

				// Fill input buffer packets into the input buffer window
				for(int j=0; j< inputbufferSize; j++)
				{
					// Step into each packet and add all the relevant data fields
					AddPacketToTree(j);
				}
				SetWindowText(DebugDialog,"Apollonious V1.0 - Stopped");
			}
		}

		// ---------------------------------------------------------------------------------------
		// Debugging window

		BOOL CALLBACK DebugProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
		{
			switch(umsg)
			{ 
			case WM_NOTIFY:
				{
					LPNMLISTVIEW	ninfo = (LPNMLISTVIEW) lparam;
					if((ninfo->hdr.code == NM_DBLCLK) && (ninfo->hdr.idFrom == IDC_LIST2))
					{
						ApolloMap::iterator fclick;
						fclick = (*apolloList).find(modList[ninfo->iItem]);
						if(fclick != (*apolloList).end())
						{
							// Toggle the Module state that was double-clicked
							(*fclick).second->state = ( (*fclick).second->state==true )?false:true;
							// Redo the whole
							AddModulesToTree();
						}
					}
					break;
				}
			case WM_INITDIALOG: return FALSE; 
			case WM_COMMAND: switch(LOWORD(wparam)) 
				{ 
				case IDOK: 
					EndDialog(hwnd, TRUE); return TRUE;
					break;
				case IDCANCEL: EndDialog(hwnd, FALSE); return TRUE;
					break;

				case IDCYCLERUN:
				case IDCYCLESTEP:
					cyclecount = true;
					cyclecounter = 1;
					QueryPerformanceCounter(&startTime);	// Reset the debug pc 
					SetWindowText(DebugDialog,"Apollonious V1.0 - Running");
					break;
				case IDPROFILE:
					{
						File::FileClass	profile;
						Data::StringClass	profname;
						Data::PageType		profpage;
						Data::StringClass	data;

						char				buff[512];
						char				label[41];

						// Save any profile info we view. 
						// Only keep the last opened though - may add extra func later
						profname << "profile.txt";							

						int error = profile.Open(&profname,FILE_WRITE);

						sprintf(buff,"%-40s%12s%12s%12s%12s%12s%12s\n","Profile Name",
								"Count","Acc.(s)","Max.(s)","Min.(s)","Av. (s)","Last (s)");
						data.Clear(); data = buff; profpage.push_back(data);

						map<const char *,Profile::Sample *>::iterator pi = Profile::ProfileMap.begin();
						// Dump profile stuff to a html file first.
						for(int i = 0; i<Profile::ProfileMap.size(); i++)
						{
							if(pi != Profile::ProfileMap.end())
							{
								strncpy(label,(*pi).first,40); label[41]='\0';
								sprintf(buff,"%-40s %11d %011l %011l %011l %011l %011l\n",
										label,
										(*pi).second->count,
										(*pi).second->acc,
										(*pi).second->max,
										(*pi).second->min,
										(*pi).second->last );
								data.Clear(); data << buff;profpage.push_back(data);
							}
							pi++;
						}

						profile << &profpage;
						profile.Close();

						ProfileWindow("Profile", &profpage);
					}
					break;
				default: return FALSE; 
				} break; 
			case WM_DESTROY:
				cyclecount = false;
				cyclecounter = 0;
				QueryPerformanceCounter(&startTime);	// Reset the debug pc 
														// Dont count the time spent in the debugger!
				break;
			} 
			return FALSE; 	
		}

		HTREEITEM AddChildNode(char *text, HTREEITEM parent, DWORD res)
		{
			TV_INSERTSTRUCT tvi;
			tvi.hParent				= parent;
			tvi.hInsertAfter        = TVI_LAST;
			tvi.item.mask           = TVIF_TEXT | TVIF_PARAM;
			tvi.item.lParam			= 0;
			tvi.item.cChildren		= 0;
			tvi.item.pszText		= text;
			return TreeView_InsertItem(GetDlgItem(DebugDialog,res), &tvi);
		}

		HTREEITEM AddParentNode(char *text, HTREEITEM parent, DWORD res)
		{
			TV_INSERTSTRUCT tvi;
			tvi.hParent				= parent;
			tvi.hInsertAfter        = TVI_LAST;
			tvi.item.mask           = TVIF_TEXT | TVIF_PARAM | TVIF_CHILDREN;
			tvi.item.lParam			= 0;
			tvi.item.cChildren		= 1;
			tvi.item.pszText		= text;
			return TreeView_InsertItem(GetDlgItem(DebugDialog,res), &tvi);
		}

		int AddLParentNode(char *text, DWORD res, int image, int listid)
		{
			LVITEM lvi;
			lvi.mask		= LVIF_TEXT  | LVIF_PARAM | LVIF_IMAGE ;
			lvi.iItem		= listid;
			lvi.iSubItem	= 0;
			lvi.lParam		= 0;
			lvi.iImage		= image;
			lvi.pszText		= text;
			return ListView_InsertItem(GetDlgItem(DebugDialog,res), &lvi);
		}

		int AddLChildNode(char *text, DWORD res, int image, int listid, int subid)
		{
			LVITEM lvi;
			lvi.mask		= LVIF_TEXT  | LVIF_PARAM | LVIF_IMAGE ;
			lvi.iItem		= listid;
			lvi.iSubItem	= subid;
			lvi.lParam		= 0;
			lvi.iImage		= image;
			lvi.pszText		= text;
			return ListView_InsertItem(GetDlgItem(DebugDialog,res), &lvi);
		}

		void AddDataPacketToTree(int index, HTREEITEM parent)
		{
			Data::StringClass	packetname;
			packetname = "Data";

			Packet::PackClass	*p = &inputbuffer[index];

			if(p->Size())
			{
				// Add Node to treeview     
				HTREEITEM proot = AddParentNode((char *)packetname.String(),parent, IDC_TREE1);

				for(int i=0; i<(p->Size()/4); i++)
				{
					// Add Data
					packetname.Clear(); packetname << p->GetDataIndex(i,D_UINT); 
					AddChildNode((char *)packetname.String(),proot, IDC_TREE1);
				}	
			}
		}

		void AddPacketToTree(int index)
		{
			Data::StringClass	packetname;
			packetname = "Packet:";
			packetname << index;

			Packet::PackClass	*p = &inputbuffer[index];

			// Add Node to treeview     
			HTREEITEM proot = AddParentNode((char *)packetname.String(),NULL, IDC_TREE1);

			// Add src
			packetname.Clear(); packetname << "srccrc"; 
			HTREEITEM pnext = AddParentNode((char *)packetname.String(),proot, IDC_TREE1);
			packetname.Clear(); packetname << "id:"; packetname << p->srccrc.Id(); 
			AddChildNode((char *)packetname.String(),pnext, IDC_TREE1);
			packetname.Clear(); packetname << "orig:"; packetname << p->srccrc.String(); 
			AddChildNode((char *)packetname.String(),pnext, IDC_TREE1);
			packetname.Clear(); packetname << "modid:"; packetname << p->destcrc.GetModId(); 
			AddChildNode((char *)packetname.String(),pnext, IDC_TREE1);
			packetname.Clear(); packetname << "funcid:"; packetname << p->destcrc.GetFuncId(); 
			AddChildNode((char *)packetname.String(),pnext, IDC_TREE1);
			
			// Add dest
			packetname.Clear(); packetname << "destcrc"; 
			pnext = AddParentNode((char *)packetname.String(),proot, IDC_TREE1);
			packetname.Clear(); packetname << "id:"; packetname << p->destcrc.Id(); 
			AddChildNode((char *)packetname.String(),pnext, IDC_TREE1);
			packetname.Clear(); packetname << "orig:"; packetname << p->destcrc.String(); 
			AddChildNode((char *)packetname.String(),pnext, IDC_TREE1);
			packetname.Clear(); packetname << "modid:"; packetname << p->destcrc.GetModId(); 
			AddChildNode((char *)packetname.String(),pnext, IDC_TREE1);
			packetname.Clear(); packetname << "funcid:"; packetname << p->destcrc.GetFuncId(); 
			AddChildNode((char *)packetname.String(),pnext, IDC_TREE1);

			// Add timestamp
			packetname.Clear(); packetname << "timestamp"; 
			pnext = AddParentNode((char *)packetname.String(),proot, IDC_TREE1);
			packetname.Clear(); packetname << "id:"; packetname << p->TimeStamp().Id(); 
			AddChildNode((char *)packetname.String(),pnext, IDC_TREE1);
			packetname.Clear(); packetname << "timeval:"; packetname << p->TimeStamp().Time(); 
			AddChildNode((char *)packetname.String(),pnext, IDC_TREE1);

			AddDataPacketToTree(index,proot);
		}

		void AddModuleFuncsToTree(int index, HTREEITEM parent)
		{
			Data::StringClass	packetname;
			packetname = "Functions";

			// Add Node to treeview     
//			HTREEITEM proot = AddChildNode((char *)packetname.String(),parent, IDC_TREE2);

			// Loop through the apollo functions and add each
//			FuncTable *ftable = (*apolloList)[index]->GetFMap();
//			for(int i = 0; ftable->Size(); i++)
//			{
//				packetname.Clear(); packetname << ftable->At(i);
//				HTREEITEM pnext = AddChildNode((char *)packetname.String(),proot, IDC_TREE2);
//				ftable++;
//			}
		}

		void AddModulesToTree(void)
		{
			int mctr = 0;
			// Clear the list view first
			ListView_DeleteAllItems(GetDlgItem(DebugDialog,IDC_LIST2));
			modList.clear();

			// Fill input buffer packets into the input buffer window
			ApolloMap::iterator j = apolloList->begin();
			for(j = apolloList->begin(); j != apolloList->end(); j++)
			{	
				if((*j).second)
				{
					mctr++;
					Data::StringClass	packetname;
					packetname << (*j).second->id.String();

					// Get the module state
					int icon = ( (*j).second->state==true )?enaIcon:disIcon;

					// Add Node to treeview     
					int modparent = AddLParentNode((char *)packetname.String(), IDC_LIST2, icon, mctr);

					modList.push_back((*j).first);
					// Set the icon - enabled or disabled

//					AddModuleFuncsToTree(mctr,modparent);
				}
				j++;
			}
		}

		HWND InitWindow(void)
		{
			HICON hicon;

			InitCommonControls();
			DebugDialog = CreateDialog(ZEUS::instance, MAKEINTRESOURCE(IDD_DIALOG1), (HWND)NULL,DebugProc);

			SetWindowText(DebugDialog,"Apollonious V1.0 - Stopped");

			// Create a masked image list large enough to hold the icons. 
			himl = ImageList_Create(16, 16, ILC_COLORDDB , 0, 4); 
 
			// Load the icon resources, and add the icons to the image list. 
			hicon = LoadIcon(ZEUS::instance, MAKEINTRESOURCE(IDI_DISABLE)); 
			disIcon = ImageList_AddIcon(himl, hicon); 
 
			hicon = LoadIcon(ZEUS::instance, MAKEINTRESOURCE(IDI_ENABLE)); 
			enaIcon = ImageList_AddIcon(himl, hicon); 

			HFONT hfont = CreateFont (GetHeightFromPoints(8), 0, 0, 0, 400,
										0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
										CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
										DEFAULT_PITCH | FF_SWISS,
										TEXT("Courier")); 
			// Set the packet list font
			SendMessage( GetDlgItem(DebugDialog,IDC_TREE1),
							WM_SETFONT, (WPARAM) hfont,
							MAKELPARAM(TRUE, 0));

			// Set the debug output font
			SendMessage( GetDlgItem(DebugDialog,IDC_LIST1),
							WM_SETFONT, (WPARAM) hfont,
							MAKELPARAM(TRUE, 0));

			// Set the packet list font
			SendMessage( GetDlgItem(DebugDialog,IDC_LIST2),
							WM_SETFONT, (WPARAM) hfont,
							MAKELPARAM(TRUE, 0));

			// Set the packet list font
			SendMessage( GetDlgItem(DebugDialog,IDC_LIST4),
							WM_SETFONT, (WPARAM) hfont,
							MAKELPARAM(TRUE, 0));

			ListView_SetImageList(GetDlgItem(DebugDialog,IDC_LIST2),himl,LVSIL_SMALL );

			// Fill in the lines of text into the debug window
			for(int i=0; i<debuglog.size(); i++)
			{
				if(debuglog[i].Size() > 0)
					SendMessage( GetDlgItem(DebugDialog,IDC_LIST1),
								LB_ADDSTRING,0,(LPARAM) (LPCTSTR) debuglog[i].String() );
			}

			// Fill input buffer packets into the input buffer window
			for(int j = 0; j<inputbufferSize; j++)
			{
				// Step into each packet and add all the relevant data fields
				AddPacketToTree(j);
			}

			AddModulesToTree();
			ShowWindow(DebugDialog,SW_SHOW);

			return DebugDialog;
		}

		void SetApolloList(ApolloMap *alist)
		{
			apolloList = alist;
		}

		void SetInputBuffer(Packet::PackClass *buf, int sz)
		{
			inputbuffer = buf;
			inputbufferSize = sz;
		}
		
		void InitDebug()
		{
			// TODO:
			// InstallDebugHandler();

			Data::StringClass	*sptr = new Data::StringClass;
			debuglog.push_back(*sptr);

			// By default timestamp is on
			timestamp = TRUE;

			QueryPerformanceFrequency(&countsPerSec);
			QueryPerformanceCounter(&startTime);
		}

		void KillDebug()
		{
			// TODO:
			// RemoveDebugHandler();

		// Dont dump logs on release version
		#if defined _DEBUG
			// Dump the debug log to a file for reading later..
			char fname[] = "c:\\debuglog.txt";

			// Empty log? Not likely!
			if(!debuglog.empty())
			{
				FILE *debugfile = fopen(fname,"w");
				if(!debugfile) return;

				// clear the debug page page
				for(unsigned int i = 0; i < debuglog.size(); i++)
				{
					if(debuglog.at(i).String())
					{
						fprintf(debugfile,"%s\n",debuglog.at(i).String());
					}
				}

				fclose(debugfile);
			}
		#endif

			debuglog.clear();
		}

		unsigned int SetTimeStamp(unsigned int ts)
		{
			unsigned int old = timestamp;
			timestamp = ts;
			return old;
		}

		void TimeStamp()
		{
			if(!timestamp) return;
			// We are adding text to a new line - timestamp?
			Data::StringClass *sptr = &debuglog.back();
			if(sptr->Empty())
			{
				LARGE_INTEGER	pc;
				char			sbuf[20];

				// Calc Time since start
				QueryPerformanceCounter(&pc);

				// WARNING: using floats here can induce loss of data, some finite timings may be inaccurate
				float secs = ((float)( pc.QuadPart - startTime.QuadPart ) / countsPerSec.QuadPart);
				unsigned long millisec = (unsigned long)(1000000.0f * secs);
				accumulator += millisec;

				sprintf(sbuf,"%06d.%06d",accumulator/1000000,accumulator%1000000);

				*sptr << "[";
				*sptr << sbuf;
				*sptr << "]|";
			}
		}

		void Out(char *txt)
		{
			if(!txt) return;
			char *npos;
			
			TimeStamp();
			Data::StringClass *sptr = &debuglog.back();

			if(npos = strchr(txt,'\n'))
			{
				if(txt[0]=='\n')
				{
					sptr = new Data::StringClass;
					debuglog.insert(debuglog.end(),*sptr);

					return;
				}
				else
				{
					int i;
					for(i=0;i<(npos-txt);i++)
						*sptr << txt[i];

					sptr = new Data::StringClass;
					debuglog.insert(debuglog.end(),*sptr);
				
//					Debug::Out( &txt[i+1] );
				}
			}
			else
			{
				*sptr << txt;
			}
		}

		void Out(const char *txt)
		{
			Out((char *)txt);
		}

		void Out(Data::StringClass *str)
		{
			Out(str->String());
		}

		void Out(Data::StringClass str)
		{
			Out(str.String());
		}

		void Out(char txt)
		{
			TimeStamp();
			Data::StringClass *sptr = &debuglog.back();
			*sptr << txt;
		}

		void Out(int txt)
		{
			TimeStamp();
			Data::StringClass *sptr = &debuglog.back();
			*sptr << txt;
		}

		void Out(long txt)
		{
			TimeStamp();
			Data::StringClass *sptr = &debuglog.back();
			*sptr << txt;
		}

		void Out(float txt)
		{
			TimeStamp();
			Data::StringClass *sptr = &debuglog.back();
			*sptr << txt;
		}

		void Out(double txt)
		{
			TimeStamp();
			Data::StringClass *sptr = &debuglog.back();
			*sptr << txt;
		}

		void Out(unsigned char txt)
		{
			TimeStamp();
			Data::StringClass *sptr = &debuglog.back();
			*sptr << txt;
		}

		void Out(unsigned int txt)
		{
			TimeStamp();
			Data::StringClass *sptr = &debuglog.back();
			*sptr << txt;
		}

		void Out(unsigned long txt)
		{
			TimeStamp();
			Data::StringClass *sptr = &debuglog.back();
			*sptr << txt;
		}

	}
}