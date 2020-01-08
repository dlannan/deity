
// Make sure you include headers for modules you intend to use. They may
// have types or structures you may need to cast pointers to.

// aWindow description
//
// Abstract window system. Works within a render context (ie DX Render surface or OGL window)
// Must have a render module available, otherwise why bother.
// Initially will only support Fullscreen render contexts. 
// Windowed contexts have various issues to solve:
//	-	Ideally windowed contexts will be transparent Fullscreen contexts, which when a non aWindow
//		object is selected (ie background) then the context is switch to the background and the 
//		user can then normally work with the underlying OS.
//	-	Needs to be thoroughly tested and designed
//	-	Layered contexts (ie seperate Zeus activated app) how thwy should be handled in windowed mode
//	-	aWindow alpha setting - should this allow the OS to bee seen through?
//	-	Various other misc problems.

// WARNING NOTE: map templates are NOT standard windows maps. Windows map templates are broken.
//				 get the latest maps STL compliant from internet.

#include "..\\..\\System\\System.h"

#include "aWindow.h"
#include "WindowClass.h"

// MS SHITTY TEMPLATE SUPPORT!
#pragma warning(disable:4786)

ApolloClass	apollo;

namespace aWindow
{
	// Some globals - these are temporary
	int		gWindowMat = 0;
	int		gWindowTex = 0;

	// Our windows are stored in a simple map - with unique id keys
	typedef map<int, WindowClass *> WindowMap;

	// Zsorting map - simple quick retrieval of z ordered windows
	// <zvalue, Window handle index>
	typedef map<float, unsigned int> WindowZSort;

	// All the windows we make will live here
	WindowMap		windowList;	
	WindowZSort		windowZSort;

	// Function declarations
	void Single(unsigned int id);

	// Submission functions	
	void Create(Packet::PackClass *p);
	void Render(Packet::PackClass *p);
	void RenderAll(Packet::PackClass *p);

	void PreInit()
	{
		// Without this there is no way to Init the module, since the recieve will fail.
		apollo.SubmitFunction(aWindow::Init,"Init");
		apollo.SubmitFunction(aWindow::Kill,"Kill");

		apollo.SubmitFunction(aWindow::Create,"Create");
		apollo.SubmitFunction(aWindow::RenderAll,"RenderAll");
		apollo.SubmitFunction(aWindow::Render,"Render");
	}

	// Use this to submit your calls - external submission not available yet
	void Init(Packet::PackClass *p)
	{
		// Get the current render context - and make sure its Fullscreen

		// ********** Temporary ***********
		// Use a default Window material for all windows (ie texture too)
		// Load in the console texture 
		{
		Packet("Window","Material.CreateMaterial");
		Psend;

		gWindowMat = packet.GetData(D_INT);
		}

		{
		Packet("Window","Material.LoadTexture");
		Padd "Data\\System\\Windows\\window.bmp"; 
		Psend;

		gWindowTex = packet.GetData(D_INT);
		}

		{
		Packet("Jetris", "Material.AddTexture");
		Padd gWindowTex;
		Padd gWindowMat;
		Psend;
		}
	}

	// Use this to cleanup snything nasty at the end of the module runtime
	void Kill(Packet::PackClass *p)
	{
		// Clean up any outstanding windows
		WindowMap::iterator	i = windowList.begin();
		while(i != windowList.end())
		{
			delete i->second;
			i++;
		}

		windowList.clear();
		windowZSort.clear();
	}

	void Create(Packet::PackClass *p)
	{
		// Get packet info
		float left = p->GetData(D_FLOAT);
		float top = p->GetData(D_FLOAT);		
		float width = p->GetData(D_FLOAT);		
		float height = p->GetData(D_FLOAT);		
		float depth = p->GetData(D_FLOAT);				// zdepth
		unsigned int pushVal = p->GetData(D_UINT);		// Push Z back or forward (0 = back, any other = forward)

		WindowClass *newwin = new WindowClass(left,top,depth,width,height);

		unsigned int val = CRC::Int(windowList.size()+1);
		// Add it into the window list
		newwin->SetWindowHandle(val);
		newwin->SetWindowMaterial(gWindowMat);
		windowList[val] = newwin;

		// Make sure this window has unique z. Windows cannot share z.
		// We can push back or push forward a window by 0.001 z..
		while(windowZSort.find(depth) != windowZSort.end())
		{
			if(pushVal) depth -= 0.001f;		// Push forward
			else depth += 0.001f;				// Push backward
		}
		newwin->windowPos.z = depth;
		windowZSort.insert(pair<float, unsigned int>(depth, newwin->GetWindowHandle()) );

		// Return a window id handle
		p->Clear();
		(*p) += val;
	}

	// Render all the windows in the module - probably most common call
	void RenderAll(Packet::PackClass *p)
	{
		// I guess we could force a starting depth to start window rendering from?
		float startz = p->GetData(D_FLOAT); // ?? maybe not necessary

		map<float,unsigned int>::iterator	i = windowZSort.begin();
		while(i != windowZSort.end())
		{
			Single(i->second);
			i++;
		}
	}

	// Render a particular window - for special cases - debugging pane, command console etc.
	// Remember we are using a zbuffer so if it is behind something then it wont work,
	// you will need to force a new z buffer value
	void Render(Packet::PackClass *p)
	{
		// Get id to render
		unsigned int	id = p->GetData(D_UINT);
		float			depth = p->GetData(D_FLOAT);
		unsigned int	pushVal = p->GetData(D_UINT);	// Which way to push the window

		// z depth changed? remove from zsort, then add again
		if(depth != windowList[id]->windowPos.z)
		{
			map<float,unsigned int>::iterator oldwin = windowZSort.find(windowList[id]->windowPos.z);
			// Found the old window - removing and adding again
			if(oldwin != windowZSort.end())
			{
				windowZSort.erase(windowList[id]->windowPos.z);

				// Make sure this window has unique z. Windows cannot share z.
				// We can push back or push forward a window by 0.001 z..
				while(windowZSort.find(depth) != windowZSort.end())
				{
					if(pushVal) depth -= 0.001f;		// Push forward
					else depth += 0.001f;				// Push backward
				}

				windowZSort.insert(pair<float, unsigned int>(depth, windowList[id]->GetWindowHandle()) );
			}
		}

		// Unchanged, just render it
		Single(id);
	}

	// Single window render operation - so we can call it from renderall as well
	void Single(unsigned int id)
	{
		windowList[id]->Render();
	}
}
