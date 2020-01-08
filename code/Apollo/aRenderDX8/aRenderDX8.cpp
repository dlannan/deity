
// Make sure you include headers for modules you intend to use. They may
// have types or structures you may need to cast pointers to.

#include "..\\..\\System\\System.h"

#include "aRenderDX8.h"
#include "Primitive.h"
#include "RenderVertex.h"
#include "Utils.h"

// Main module interface
ApolloClass	apollo;

namespace aRenderDX8
{
	// List of all vertices submitted to renderer for possible rendering - sorted by unique id
	map<int, VertexPtrType *>				vlist;
	// Map Render list to materials - so we reduce texture changes
	map<int, RenderPtrType *>				rlist;

	// This is the app window - lots to do here
	HWND					win;			// Main app window!
	HINSTANCE				inst;			// Main app instance

	// DX rendering handles
	IDirect3D8*				g_pD3D;
	IDirect3DDevice8*		g_pd3dDevice;

	// Some global render settings
	ColorChar4	clear;						// clear colour for the renderer

	// Some renderstats stuff - TODO: should have seperate file for this gear
	long	time;
	long	lastupdatetime;
	long	inputframecount;

	// Render functions


	// Functions
	void PreInit()
	{
		// Without this there is no way to Init the module, since the recieve will fail.
		apollo.SubmitFunction(aRenderDX8::Init,"Init");
		apollo.SubmitFunction(aRenderDX8::Kill,"Kill");
		apollo.SubmitFunction(aRenderDX8::SetInstance,"SetInstance");

		apollo.SubmitFunction(aRenderDX8::DoRenderFrame,"DoRenderFrame");
		apollo.SubmitFunction(aRenderDX8::GetRenderDevice,"GetRenderDevice");
		apollo.SubmitFunction(aRenderDX8::SetScreenMode,"SetScreenMode");
		apollo.SubmitFunction(aRenderDX8::ReSize,"ReSize");
		apollo.SubmitFunction(aRenderDX8::InitRender,"InitRender");

		apollo.SubmitFunction(aRenderDX8::AddMesh,"AddMesh");
		apollo.SubmitFunction(aRenderDX8::SetMeshMaterial,"SetMeshMaterial");
		apollo.SubmitFunction(aRenderDX8::SetMeshTransform,"SetMeshTransform");
		apollo.SubmitFunction(aRenderDX8::MultMeshTransform,"MultMeshTransform");
		apollo.SubmitFunction(aRenderDX8::SetMeshOrient,"SetMeshOrient");
		
		apollo.SubmitFunction(aRenderDX8::RenderMesh,"RenderMesh");
		apollo.SubmitFunction(aRenderDX8::PrepareFontMesh,"PrepareFontMesh");
		apollo.SubmitFunction(aRenderDX8::PrepareWindowMesh,"PrepareWindowMesh");

		apollo.SubmitFunction(aRenderDX8::SetCamera,"SetCamera");
		apollo.SubmitFunction(aRenderDX8::SetProjection,"SetProjection");

		aRenderDX8::SubmitPrimFunctions();
	}

	void SetInstance(Packet::PackClass *in)
	{
		inst = (HINSTANCE)in->GetData(D_ULONG);
	}

	// Make a default window for rendering in
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
		wc.hCursor = LoadCursor(NULL,NULL);
		wc.hIcon = NULL; 
		wc.hbrBackground = CreateSolidBrush(0);
		wc.lpszMenuName =  NULL;
		wc.lpszClassName = "Render";
		regc = RegisterClass( &wc );

		ASSERT( regc == NULL, "Render window could not register classname" );

		if(win) win = NULL;

		win =  CreateWindowEx(
				NULL,
				wc.lpszClassName,
				"Render DX8",
				WS_POPUPWINDOW | WS_CAPTION ,	// Window with caption and border
//				WS_POPUP | WS_MAXIMIZE,			// Fullscreen no caption or border
				0, //CW_USEDEFAULT, 
				0, //CW_USEDEFAULT,
				640, //GetSystemMetrics(SM_CXSCREEN),
				480, //GetSystemMetrics(SM_CYSCREEN),
				NULL,NULL,
				inst,
				NULL
				);

		ASSERT( win == NULL, "Could not create Render window.");

		ShowWindow(win,SW_SHOW);
		UpdateWindow(win);
		return;	
	}

	// ****************************************************************************
	// Init and Kill

	void Init(Packet::PackClass *p)
	{
		// Init renderstats stuff
		inputframecount = 0;
		time = 0;
		lastupdatetime = 0;

		// Clear any window handles
		win = NULL;

		// Set DX handles to NULL
		g_pD3D = NULL;
		g_pd3dDevice = NULL;

		// Clear our vertex and renderlists
		vlist.clear();
		rlist.clear();

		// Set the clearscreen colour - blue :-)
		clear.rgba = 0xFF0000FF;	

		// Should have a renderscript check here, load in window settings
		// LoadRenderWindowScript();

		// Make the default Render window
		CreateDefaultWindow();

		// Broadcast default window to all modules
		{
		Packet("Render","Zeus.Broadcast");
		Padd ((unsigned int)0xcda2dfc9);	// window.handle
		Padd (unsigned long)(win);
		Psend;
		}

		// Init the renderer - DX setup
		InitRender(NULL);

		PrepPrimMeshes();
		
		// Inform the material module what sort of renderer we are using
		{
		Packet("Render","Material.SetAPIMode");
		Padd (int) API::DX8;
		Psend;
		}
	}

	// Use this to cleanup anything nasty at the end of the module runtime
	void Kill(Packet::PackClass *p)
	{
		ProfileDumpAll();

		// Free up the material and vertex memory
		map<int,VertexPtrType *>::iterator i = vlist.begin();
		while(i != vlist.end())
		{
			delete i->second;
			i++;
		}

		vlist.clear();
		rlist.clear();

		// Release all DX handles
		if( g_pd3dDevice != NULL)
			g_pd3dDevice->Release();
		if( g_pD3D != NULL)
			g_pD3D->Release();
	}

	// ****************************************************************************
	// Vertex info setting

	void SetMeshMaterial(Packet::PackClass *in)
	{
		int mesh = in->GetData(D_INT);
		int mat = in->GetData(D_INT);

		ASSERT(vlist.find(mesh) == vlist.end(), "Invalid Mesh Index");
		vlist[mesh]->mat = mat;
		// update status
		vlist[mesh]->status |= RENDER_STATUS_MODIFIED;
	}

	void SetMeshTransform(Packet::PackClass *in)
	{
		int mesh = in->GetData(D_INT);
		Matrix *temp = (Matrix *)in->GetData(D_ULONG);

		ASSERT(vlist.find(mesh) == vlist.end(), "Invalid Mesh Index");
		vlist[mesh]->trans = *temp;
	}

	void MultMeshTransform(Packet::PackClass *in)
	{
		int mesh = in->GetData(D_INT);
		Matrix *mul = (Matrix *)in->GetData(D_ULONG);

		ASSERT(vlist.find(mesh) == vlist.end(), "Invalid Mesh Index");
		vlist[mesh]->trans *=  *mul;
//		D3DXMatrixMultiply((D3DXMATRIX *)&vlist[mesh]->trans,(D3DXMATRIX *)&vlist[mesh]->trans, (D3DXMATRIX *)mul);
	}

	void SetMeshOrient(Packet::PackClass *in)
	{
		int mesh = in->GetData(D_INT);
		Matrix *temp = (Matrix *)in->GetData(D_ULONG);

		ASSERT(vlist.find(mesh) == vlist.end(), "Invalid Mesh Index");
		vlist[mesh]->orient = *temp;
	}

	// ****************************************************************************
	// Render Device Functions

	void SetCamera(D3DXMATRIX &mat)
	{
		ASSERT(g_pd3dDevice==NULL,"DX8 Render device handle not set. Renderer Initialised?");
		g_pd3dDevice->SetTransform( D3DTS_VIEW, &mat );
	}

	void SetProjection(D3DXMATRIX &mat)
	{
		ASSERT(g_pd3dDevice==NULL,"DX8 Render device handle not set. Renderer Initialised?");
		g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &mat );
	}

	void SetCamera(Packet::PackClass *p)
	{
		ASSERT(g_pd3dDevice==NULL,"DX8 Render device handle not set. Renderer Initialised?");
		D3DXMATRIX	mat;
		//Fill out the mat
		memcpy(&mat, (void *)p->GetData(D_ULONG), sizeof(D3DXMATRIX));
		SetCamera(mat);
	}

	void SetProjection(Packet::PackClass *p)
	{
		ASSERT(g_pd3dDevice==NULL,"DX8 Render device handle not set. Renderer Initialised?");
		D3DXMATRIX	mat;
		//Fill out the mat
		memcpy(&mat, (void *)p->GetData(D_ULONG), sizeof(D3DXMATRIX));
		SetProjection(mat);
	}

	void DoRenderFrame(Packet::PackClass *p)
	{
		// Internal call.. just call it!!
		ASSERT(g_pd3dDevice==NULL,"DX8 Render device handle not set. Renderer Initialised?");
		DoRenderFrame();
	}

	void GetRenderDevice(Packet::PackClass *p)
	{
		ASSERT ( g_pd3dDevice == NULL, "Render Device not available.");
		if(g_pd3dDevice)
		{
			p->Clear();
			(*p) += (unsigned long)g_pd3dDevice;
		}
	}

	void ReSize(Packet::PackClass *p)
	{
		ASSERT(win==NULL,"Window not valid. Renderer Initialised?");

		SetWindowPos(win, HWND_TOP, 0, 0, p->GetDataIndex(0, D_INT),
						p->GetDataIndex(1, D_INT), SWP_SHOWWINDOW | SWP_NOMOVE);
	}

	void SetScreenMode(Packet::PackClass *p)
	{
		ASSERT(g_pd3dDevice==NULL,"DX8 Render device handle not set. Renderer Initialised?");

		ScreenMode(	p->GetDataIndex(0, D_INT),
					p->GetDataIndex(1, D_INT),
					p->GetDataIndex(2, D_INT),
					p->GetDataIndex(3, D_INT));

		p->Clear();
		(*p) += (unsigned long)g_pd3dDevice;
	}

	void InitRender(Packet::PackClass *p)
	{
		// Do we have a window?
		ASSERT(win==NULL,"InitRender - No window created?")
		InitDirect3D();
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
	void InitDirect3D()
	{
		g_pD3D = Direct3DCreate8( D3D_SDK_VERSION );
		ASSERT(g_pD3D == NULL, "Couldnt Create a DX8 Render device");

		// You can fill out the d3ddm to actually choose the mode
		D3DDISPLAYMODE d3ddm;
		if( FAILED( g_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm ) ) )
			return; //zeus.debug << E_FAIL;

		D3DPRESENT_PARAMETERS d3dpp; 
		ZeroMemory( &d3dpp, sizeof(d3dpp) );
		d3dpp.Windowed   = TRUE;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.BackBufferFormat = d3ddm.Format;

		d3dpp.EnableAutoDepthStencil = TRUE;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

		d3dpp.Flags |= D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

		if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL , win,
										  D3DCREATE_HARDWARE_VERTEXPROCESSING,
										  &d3dpp, &g_pd3dDevice ) ) )
			if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL , win,
										  D3DCREATE_SOFTWARE_VERTEXPROCESSING,
										  &d3dpp, &g_pd3dDevice ) ) )
				return; //zeus.debug << E_FAIL;

		// Global light!
		D3DLIGHT8 light;
		ZeroMemory( &light, sizeof(D3DLIGHT8) );

		light.Diffuse.r  = 1.0f;
		light.Diffuse.g  = 1.0f;
		light.Diffuse.b  = 1.0f;
		light.Range      = 100.0f;
		light.Type       = D3DLIGHT_POINT;

//		g_pd3dDevice->SetLight(0, &light);
//		g_pd3dDevice->LightEnable( 0, TRUE);
//		g_pd3dDevice->SetRenderState( D3DRS_AMBIENT, 0x00800000 );
		g_pd3dDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
		g_pd3dDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL );
		g_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
		g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	}

	void ScreenMode(int width, int height, int depth, int window)
	{
		// Release current display device..
		if( g_pd3dDevice != NULL) g_pd3dDevice->Release();

		// You can fill out the d3ddm to actually choose the mode
		D3DDISPLAYMODE d3ddm;
		if( FAILED( g_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm ) ) )
			return; //zeus.debug << E_FAIL;

		D3DPRESENT_PARAMETERS d3dpp; 
		ZeroMemory( &d3dpp, sizeof(d3dpp) );
		d3dpp.BackBufferWidth = width;
		d3dpp.BackBufferHeight = height;
		d3dpp.BackBufferFormat 	= D3DFMT_A8R8G8B8 ;
		d3dpp.Windowed   = window;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.Flags |= D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;


		if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, win,
										  D3DCREATE_HARDWARE_VERTEXPROCESSING,
										  &d3dpp, &g_pd3dDevice ) ) )
		{
			if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, win,
											D3DCREATE_SOFTWARE_VERTEXPROCESSING,
											&d3dpp, &g_pd3dDevice ) ) )
			{
				// Go back to windowed mode
				if( FAILED( g_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm ) ) )
					return; //zeus.debug << E_FAIL;

				D3DPRESENT_PARAMETERS d3dpp; 
				ZeroMemory( &d3dpp, sizeof(d3dpp) );
				d3dpp.Windowed   = TRUE;
				d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
				d3dpp.BackBufferFormat = d3ddm.Format;
				d3dpp.Flags |= D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

				if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, win,
												D3DCREATE_SOFTWARE_VERTEXPROCESSING,
												&d3dpp, &g_pd3dDevice ) ) )
					return; //zeus.debug << E_FAIL;
			}
		}
	}

	// ****************************************************************************
	// Vertex Submission Functions

	void AddMesh(Packet::PackClass *p)
	{
		ASSERT(g_pd3dDevice==NULL,"DX8 Render device handle not set. Renderer Initialised?");

		int handle = PrepareMesh(	(void*)p->GetDataIndex(0,D_LONG),
									p->GetDataIndex(1,D_UINT),
									p->GetDataIndex(2,D_INT),
									p->GetDataIndex(3,D_UINT));

		// Send back an index of the mesh added
		p->Clear();
		(*p) += handle;
	}

	void RenderMesh(Packet::PackClass *p)
	{
		// Submit mesh index
		RenderMesh(	p->GetData(D_INT) );
	}

	// We now associate a material with a mesh
	int PrepareMesh(void *verts,unsigned int numverts, int mat, unsigned int format)
	{
		HRESULT err;

		VertexPtrType *temp = new VertexPtrType;
		temp->mat = mat;
		temp->mode = RENDER_TRILIST;	// default tri render mode

		temp->vbuff.shader = GetShaderType(format);
		temp->vbuff.size = GetShaderSize(format);
		temp->vbuff.max = numverts;

		temp->vptr = (VertexType *)verts;
		temp->verts = numverts;

		temp->index = 0;		
		temp->length = numverts;

		// Set the matrices to Identity
		Matrix ttrans; memset(&ttrans,0,sizeof(Matrix));
		ttrans.right.x = 1.0f; ttrans.up.y = 1.0f; ttrans.view.z = 1.0f; ttrans.pos.w = 1.0f;
		temp->orient = ttrans;
		temp->trans = ttrans;

		// Vertex buffer.. pretty straight forward really..
		err = g_pd3dDevice->CreateVertexBuffer(	temp->vbuff.max*temp->vbuff.size, D3DUSAGE_WRITEONLY,
														temp->vbuff.shader , D3DPOOL_DEFAULT, 
														&(temp->vbuff.dxptr) );
		ASSERT( FAILED(err), "Cannot create vertex buffer.");

		unsigned int val = CRC::Int(vlist.size()+1);
		vlist[val] = temp;

		// Place all the verts into the vertexbuffer
		FillVertexBuffer(temp);

		return val;
	}

	void PrepareWindowMesh(Packet::PackClass *in)
	{
		int mat = -1;		// No material for a window yet - just vert coloured to start with
		int error = -1;

		VertexPtrType *temp = new VertexPtrType;
		temp->mat = mat;
		temp->mode = RENDER_TRILIST;	// default tri render mode

		BuildWindowVerts(temp);
//		temp->vbuff.shader = format;
//		temp->vbuff.size = GetSize(format);
//		temp->vbuff.max = numverts;

//		temp->vptr = verts;
//		temp->verts = numverts;

		temp->index = 0;
		temp->length = 0;

		unsigned int val = CRC::Int(vlist.size()+1);

		// Vertex buffer.. pretty straight forward really..
		if( FAILED( g_pd3dDevice->CreateVertexBuffer(	temp->vbuff.max*temp->vbuff.size,0,
														temp->vbuff.shader , D3DPOOL_DEFAULT, 
														&(temp->vbuff.dxptr) ) ) )
		{
			error = 0; //zeus.debug << E_FAIL;
		}
		else
		{
			vlist[val] = temp;

			// Place all the verts into the vertexbuffer
			FillVertexBuffer(temp);

			error = val;
		}

		in->Clear();
		(*in) += (int) error;	// return the window list index - max of 512 windows start start with
	}

	void PrepareFontMesh(Packet::PackClass *in)
	{
		int error = -1;
		VertexPtrType *temp = new VertexPtrType;

		BuildFontVerts(temp);
		temp->mat = in->GetData(D_INT);

		temp->index = 0;
		temp->length = 0;	// Unfilled!

		// Set the matrices to Identity
		Matrix ttrans; memset(&ttrans,0,sizeof(Matrix));
		ttrans.right.x = 1.0f; ttrans.up.y = 1.0f; ttrans.view.z = 1.0f; ttrans.pos.w = 1.0f;
		temp->orient = ttrans;
		temp->trans = ttrans;
		
		unsigned int val = CRC::Int(vlist.size()+1);

		// Vertex buffer.. pretty straight forward really..
		if(FAILED ( g_pd3dDevice->CreateVertexBuffer(	temp->vbuff.max*temp->vbuff.size,0,
														temp->vbuff.shader , D3DPOOL_DEFAULT, 
														&(temp->vbuff.dxptr) ) ) )
		{
			error = 0; //zeus.debug << E_FAIL;
		}
		else
		{
			vlist[val] = temp;

			// Place all the verts into the vertexbuffer
			FillVertexBuffer(temp);

			error = val;
		}

		in->Clear();
		(*in) += (int) error;
	}

	void RenderMesh(int index)
	{
		ASSERT(vlist.find(index) == vlist.end(),"Bad Render Mesh index");

		map<int,RenderPtrType *>::iterator rptr = rlist.find(vlist[index]->mat);
		if(rptr == rlist.end())
		{
			rlist[vlist[index]->mat] = new RenderPtrType;
		}

		rlist[vlist[index]->mat]->vlist.push_back(vlist[index]);
		vlist[index]->status |= RENDER_STATUS_REQRENDER;
	}

	// ****************************************************************************
	// Frame Render

	// A single Render frame is processed here. zeus sends this.
	void DoRenderFrame(void)
	{
		ASSERT( aRenderDX8::g_pd3dDevice==NULL, "Wheres ya dx render device ya wally." );
		RenderBegin();

		// Loop through the material mapped renderverts
		map<int, RenderPtrType *>::iterator	i = rlist.begin();
		
		while(i!=rlist.end())
		{
			// Set the material
			Packet("Render","Material.SetMaterial");
			Padd (int) i->first;
			Psend
			
			// For every vertex block render primitives
			for(vector<VertexPtrType *>::iterator j = i->second->vlist.begin(); j<i->second->vlist.end(); j++)
			{
				// If we have been requested to render this vlist and we have a valid pointer
//				if( (*j)->status & RENDER_STATUS_REQRENDER )
				{
					// Set the transform for the mesh
//					g_pd3dDevice->SetTransform(D3DTS_WORLD, (D3DMATRIX *)&(*j)->orient);
					g_pd3dDevice->SetTransform(D3DTS_WORLD, (D3DMATRIX *)&(*j)->trans);

					// Rendering of scene objects happens here.
					g_pd3dDevice->SetStreamSource( 0, (*j)->vbuff.dxptr, (*j)->vbuff.size );
					g_pd3dDevice->SetVertexShader( (*j)->vbuff.shader );

					switch( (*j)->mode )
					{
					case RENDER_TRILIST:
						g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, (*j)->index, (*j)->length/3 );
						break;
					case RENDER_TRISTRIP:
						g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, (*j)->index, (*j)->length-2 );
						break;
					}
				}

				// Set the status for the vertexbuffer - clear?
				(*j)->status = 0;
			}

			// Clear the vlists in the renderlist
			i->second->vlist.clear();
			delete i->second;

			i++;
		}

		// Clear the render list
		rlist.clear();

		RenderEnd();
	}

	void RenderBegin()
	{
		// Go through active mesh list and render each :-)
		// Clear the back buffer to a blue color
		g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(clear.r,clear.g,clear.b), 1.0f, 0 );

		// Begin the scene.
		g_pd3dDevice->BeginScene();
	}

	void RenderEnd()
	{
		// End the scene.
		g_pd3dDevice->EndScene();
		
		// Put it all to the screen..
		g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
	}

}
