

// Make sure you include headers for modules you intend to use. They may
// have types or structures you may need to cast pointers to.

#include "..\\..\\System\\System.h"
#include "aRenderOGL.h"

ApolloClass	apollo;

namespace aRenderOGL
{
	// Variables
	typedef vector<VertexPtrType *>	VertexListVec;
	VertexListVec	vlist;

	typedef vector<RenderPtrType *>	RenderListVec;
	RenderListVec	rlist;

	HWND			win;	// Main game window!
	HINSTANCE		inst;

	unsigned int		numvertices;
	unsigned int		rendermode;

	HDC					g_glDC;
	HGLRC				g_oglDevice;
	
	long				rdFrameCount;

	Mesh::ColourF		clear;	

	// Functions declarations
	void Begin(Packet::PackClass *p);
	void End(Packet::PackClass *p);


	void SetInstance(Packet::PackClass *in)
	{
		inst = (HINSTANCE)in->GetData(D_ULONG);
	}
	
	// Functions
	void PreInit()
	{
		// Without this there is no way to Init the module, since the recieve will fail.
		apollo.SubmitFunction(aRenderOGL::Init,"Init");
		apollo.SubmitFunction(aRenderOGL::Kill,"Kill");
		apollo.SubmitFunction(aRenderOGL::SetInstance,"SetInstance");

		apollo.SubmitFunction(aRenderOGL::Begin,"Begin");
		apollo.SubmitFunction(aRenderOGL::End,"End");

		apollo.SubmitFunction(aRenderOGL::DoRenderFrame,"DoRenderFrame");
		apollo.SubmitFunction(aRenderOGL::GetRenderDevice,"GetRenderDevice");
		apollo.SubmitFunction(aRenderOGL::SetScreenMode,"SetScreenMode");
		apollo.SubmitFunction(aRenderOGL::InitRender,"InitRender");
		apollo.SubmitFunction(aRenderOGL::AddMesh,"AddMesh");
		apollo.SubmitFunction(aRenderOGL::RenderMesh,"RenderMesh");
		apollo.SubmitFunction(aRenderOGL::SetCamera,"SetCamera");
		apollo.SubmitFunction(aRenderOGL::SetProjection,"SetProjection");
	}

	void CreateDefaultWindow()
	{
		// Get the instance from Zeus for the window
		Packet("Zeus.GetInstance","Render.SetInstance");
		Psend;
		
		WNDCLASS		wc;
		BOOL			regc;

		wc.style = CS_OWNDC;
		wc.lpfnWndProc = DefWindowProc; 
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = inst;
		wc.hCursor = LoadCursor(inst,IDC_ARROW);
		wc.hIcon = NULL; 
		wc.hbrBackground = CreateSolidBrush(0);
		wc.lpszMenuName =  NULL;
		wc.lpszClassName = "RenderOGL";
		regc = RegisterClass( &wc );
		if( !regc ) {
			return;
		}

		if(win) win = NULL;

		win =  CreateWindowEx(
				NULL,
				wc.lpszClassName,
				"Render OGL",
				WS_POPUP | WS_MAXIMIZE,
				0, //CW_USEDEFAULT, 
				0, //CW_USEDEFAULT,
				GetSystemMetrics(SM_CXSCREEN),
				GetSystemMetrics(SM_CYSCREEN),
				NULL,NULL,
				inst,
				NULL
				);

		if( !win ) {
			return;
		}

		ShowWindow(win,SW_SHOW);
		UpdateWindow(win);
		return;	
	}

	// Use this to submit your calls - external submission not available yet
	void Init(Packet::PackClass *p)
	{
		rdFrameCount = 0;

		// Some normal Init gear
		win = NULL;

		rendermode = RENDER_TRILIST;
		numvertices = 0;

		g_glDC = NULL;
		g_oglDevice = NULL;

		vlist.clear();
		rlist.clear();

		clear.r = 0.0f;clear.g = 0.0f;clear.b = 0.0f;clear.a = 0.0f;

		CreateDefaultWindow();

		// Broadcast default window to all modules
		Packet("Render","Zeus.Broadcast");
		Padd ((unsigned int)0xcda2dfc9); // window.handle
		Padd ((unsigned int)0x0); // unsigned long padding
		Padd (unsigned long)(win);
		Psend;

		InitRender(NULL);

		{
			Packet("Render","Material.SetAPIMode");
			Padd (int) API::OGL;
			Psend;
		}
	}

	// Use this to cleanup snything nasty at the end of the module runtime
	void Kill(Packet::PackClass *p)
	{
		// Free up the material and vertex memory
		vlist.clear();

		if(g_glDC) 
		{
			ReleaseDC(win,g_glDC);

			// make the rendering context not current
			wglMakeCurrent(NULL, NULL) ;
			if (g_oglDevice!=NULL)
			{
				wglDeleteContext(g_oglDevice);
				g_oglDevice = NULL;
			}
		}
	}

	void DoRenderFrame(Packet::PackClass *p)
	{
		// Internal call.. just call it!!
		if(g_oglDevice)
			DoRenderFrame();
	}

	void GetRenderDevice(Packet::PackClass *p)
	{
		static Packet::PackClass	packet;
		if(g_oglDevice)
		{
			p->Clear();
			(*p) += (unsigned long)g_oglDevice;
		}
	}

	void SetScreenMode(Packet::PackClass *p)
	{
		static Packet::PackClass	packet;
		if(g_oglDevice)
		{
			ScreenMode(p->GetData(D_INT),p->GetData(D_INT),p->GetData(D_INT),p->GetData(D_INT));

			p->Clear();
//			(*p) += (unsigned long)g_pd3dDevice;
		}
	}

	void InitRender(Packet::PackClass *p)
	{
		// Do we have a window?
		if(win)
		{
			InitOpenGL();	// *Change this per render type!
		}
	}

	void AddMesh(Packet::PackClass *p)
	{
		static Packet::PackClass	packet;
		if(g_oglDevice)
		{
			int handle = PrepareMesh((void*)p->GetData(D_LONG), p->GetData(D_INT), p->GetData(D_INT), p->GetData(D_ULONG));

			// Send back an index of the mesh added
			p->Clear();
			(*p) += (unsigned int)handle;
		}
	}

	void RenderMesh(Packet::PackClass *p)
	{
		// Submit mesh index, mat index, posx, posy, posz, and color(r,g,b)
		RenderMesh(p->GetData(D_INT),p->GetData(D_FLOAT),p->GetData(D_FLOAT),p->GetData(D_FLOAT), p->GetData(D_UINT) );
	}

	void SetCamera(Packet::PackClass *p)
	{
		if(g_oglDevice)
		{
			D3DXMATRIX	mat;
			//Fill out the mat
			memcpy(&mat, (void *)p->GetData(D_ULONG), sizeof(D3DXMATRIX));
			SetCamera(mat);
		}
	}

	void SetProjection(Packet::PackClass *p)
	{
		if(g_oglDevice)
		{
			D3DXMATRIX	mat;
			//Fill out the mat
			memcpy(&mat, (void *)p->GetData(D_ULONG), sizeof(D3DXMATRIX));
			SetProjection(mat);
		}
	}

/*		case 0x233e6642: // zeus.broadcast
			{
				switch(input->datai[0])
				{
				case 0xcda2dfc9: // window.handle
					win = (Window::WindowClass *)input->datal[1];
				}
			}
			break;
*/
	void InitOpenGL()
	{
		g_glDC = GetDC(win);
		if(!g_glDC) 
		{
			MessageBox(GetDesktopWindow(),"No DC handle for context.", "aRenderOGL::InitOpenGL", MB_OK);
			return;
		}

		if(g_oglDevice)
		{
			// Already have an OGL Device handler - assume already initialised!
			MessageBox(GetDesktopWindow(),"Already have an oglDevice.", "aRenderOGL::InitOpenGL", MB_OK);
			return;
		}

		HDC			tempdc;
		HPALETTE	hpalette;
		LOGPALETTE* lpPal;
		int			GLPixelIndex;
		int			n;


		
		static PIXELFORMATDESCRIPTOR pfd = {
			sizeof(PIXELFORMATDESCRIPTOR),	// Size of this structure
			1,								// Version of this structure	
			PFD_DRAW_TO_WINDOW |			// Draw to Window (not to bitmap)
			PFD_SUPPORT_OPENGL |			// Support OpenGL calls in window
			PFD_DOUBLEBUFFER,	  			// Double buffered mode
			PFD_TYPE_RGBA,					// RGBA Color mode
			32,								// Want 32bit color if available
			0,0,0,0,0,0,0,0,				// Red, Green, Blue, Alpha
			0,0,0,0,0,						// Not used to select mode
			16,								// Size of depth buffer
			0,								// Not used to select mode
			0,								// Not used to select mode
			PFD_MAIN_PLANE,
			0,0,0,0 };						// Not used to select mode

		tempdc = CreateDC("DISPLAY",NULL,NULL,NULL);
		GLPixelIndex = ChoosePixelFormat(tempdc,&pfd);
		DeleteDC(tempdc);

		if(GLPixelIndex==0)return ;
		SetPixelFormat(g_glDC, GLPixelIndex, &pfd);

		DescribePixelFormat(g_glDC, GLPixelIndex, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

		if ((pfd.dwFlags & PFD_NEED_PALETTE)) 
		{
			int redMask, greenMask, blueMask, i;

			n = 1 << pfd.cColorBits;
			if (n > 256) n = 256;

			lpPal = (LOGPALETTE*)malloc(sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * n);
			memset(lpPal, 0, sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * n);
			lpPal->palVersion = 0x300;
			lpPal->palNumEntries = n;

			GetSystemPaletteEntries(g_glDC, 0, n, &lpPal->palPalEntry[0]);
    
			/* assume an RGBA pixel type. */
			redMask = (1 << pfd.cRedBits) - 1;
			greenMask = (1 << pfd.cGreenBits) - 1;
			blueMask = (1 << pfd.cBlueBits) - 1;
			
			/* fill in the entries with an RGB color ramp. */
			for (i = 0; i < n; ++i) {
				lpPal->palPalEntry[i].peRed = 
				(((i >> pfd.cRedShift)   & redMask)   * 255) / redMask;
				lpPal->palPalEntry[i].peGreen = 
				(((i >> pfd.cGreenShift) & greenMask) * 255) / greenMask;
				lpPal->palPalEntry[i].peBlue = 
				(((i >> pfd.cBlueShift)  & blueMask)  * 255) / blueMask;
				lpPal->palPalEntry[i].peFlags = 0;
			}

			hpalette = CreatePalette(lpPal);
			if (hpalette) {
				SelectPalette(g_glDC, hpalette, FALSE);
				RealizePalette(g_glDC);
			}

			free(lpPal);
		}

		g_oglDevice = wglCreateContext(g_glDC);
		if(!g_oglDevice)
		{
			MessageBox(GetDesktopWindow(),"No WGL handle for context.",	"aRenderOGL::InitOpenGL", MB_OK);
			return;
		}

		wglMakeCurrent(g_glDC, g_oglDevice);

		glClearColor(clear.r,clear.g,clear.b,clear.a);
		glDrawBuffer(GL_BACK);

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		SwapBuffers(g_glDC);
	}

	void ScreenMode(int width, int height, int depth, int window)
	{
	}

	void SetCamera(D3DXMATRIX &mat)
	{
		glMatrixMode( GL_MODELVIEW );
//		glLoadIdentity();
		glLoadMatrixf((const float *)&mat);
	}

	void SetProjection(D3DXMATRIX &mat)
	{
		RECT	rect;
		GetWindowRect(win,&rect);
		glViewport(0,0,rect.right-rect.left,rect.bottom-rect.top);
		glMatrixMode(GL_PROJECTION);
//		glLoadIdentity();
//		glFrustum(-space.w/2.0f,space.w/2.0f,-space.h/2.0f,space.h/2.0f,
//				  space.f,space.b);
		glLoadMatrixf((const float *)&mat);
	}

	// We now associate a material with a mesh
	int PrepareMesh(void *verts,unsigned int numverts, unsigned int max, unsigned long mat)
	{
		// One material per mesh - for the moment
/*		aMaterial::MaterialType *mptr ;//= new aMaterial::MaterialType;
		if(mat)
		{
			*mptr = *((aMaterial::MaterialType *)mat);
		}
		else
		{
			mptr = NULL;
		}
*/
		// numverts is maximum number of verts likely to be pushed into the buffer, with the same material
		if(verts && max >0)
		{
			VertexPtrType *temp = new VertexPtrType;
			temp->num = numverts;
			temp->max = max;
			temp->vptr = verts;
			temp->vertsInBuff = 0;

			numvertices = numverts;
			// Vertex buffer.. pretty straight forward really..
			temp->glptr = (CUSTOMVERTEX *) new CUSTOMVERTEX[max];
//			temp->mat = mptr;

			vlist.push_back(temp);
		}
		return vlist.size();
	}

	void RenderMesh(unsigned int index,	float x, float y, float z,unsigned int col)
	{
		if(index <= 0) return;				// Bad index.. fuk off
		if(index > vlist.size()) return;	// Too big..

ASSERT(1,"Rendermesh")

		D3DXMATRIX		matrix;
		VertexPtrType	*vptr = vlist[index-1];
		CUSTOMVERTEX	*verts = (CUSTOMVERTEX *)vptr->vptr;

		if(vptr->vertsInBuff >= vptr->max) return;

		CUSTOMVERTEX* pVertices = vptr->glptr;
		for(int i = 0; i<vptr->num; i++)
		{
//			D3DXMatrixRotationY( &rptr->matrix, timeGetTime()/1500.0f );	
//			D3DXMatrixTranslation( &matrix, x,y,z );

			pVertices[vptr->vertsInBuff].x = x+verts[i].x;
			pVertices[vptr->vertsInBuff].y = y+verts[i].y;
			pVertices[vptr->vertsInBuff].z = z+verts[i].z;

			pVertices[vptr->vertsInBuff].color = col ;

			pVertices[vptr->vertsInBuff].u = verts[i].u;
			pVertices[vptr->vertsInBuff].v = verts[i].v;
			
			vptr->vertsInBuff++;
		}

	}

	// A single Render frame is processes here. zeus sends this.
	void DoRenderFrame(void)
	{
		if(aRenderOGL::g_oglDevice)
		{
			RenderBegin();

			VertexListVec::iterator i = vlist.begin();
			for(i = vlist.begin(); i!=vlist.end(); i++)
			{
				glBegin(GL_TRIANGLE_STRIP);
				if((*i)->vertsInBuff>3)
				{
					CUSTOMVERTEX *vptr = (*i)->glptr;
					for (int j = 0; j<(*i)->vertsInBuff ; j+=(*i)->num)
					{
						// SLOW - But quick to implement - will change.
						for(int k = 0; k<(*i)->num; k++)
						{
							// Texture coord calcs...
							glTexCoord2f(vptr[j+k].u,vptr[j+k].v);
							glColor3ub(vptr[j+k].color & 0xFF,(vptr[j+k].color & 0xFF00)>>8,(vptr[j+k].color & 0xFF0000)>>16);

						// Vertices and Normal calcs..
//						glNormal3f(o->plist[ptr].n.x,o->plist[ptr].n.y,o->plist[ptr].n.z);
							glVertex3f(vptr[j+k].x,vptr[j+k].y,vptr[j+k].z);
						}
					}
				}
				glEnd();

				(*i)->vertsInBuff = 0;
			}
	
			RenderEnd();
		}
	}

	inline void RenderBegin()
	{
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//	glClear(GL_DEPTH_BUFFER_BIT);
	}

	inline void RenderEnd()
	{
		SwapBuffers(g_glDC);

		rdFrameCount++;
	}

	void Begin(Packet::PackClass *p)
	{
		RenderBegin();
	}

	void End(Packet::PackClass *p)
	{
		RenderEnd();
	}
}


