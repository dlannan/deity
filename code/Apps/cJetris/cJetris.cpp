
// Make sure you include headers for modules you intend to use. They may
// have types or structures you may need to cast pointers to.

#include "cJetris.h"

#include "..\\..\\System\\System.h"

#include <vector>
#include <time.h>

using namespace std;

ApolloClass	apollo;

namespace cJetris
{

	// Game data
	int					speed,frames;
	int					counter;
	int					frame_complete;
	Data::StringClass	playername;


	// piece information
	int		pieces,piecez,piecex,piecey;
	int		nexts,nextz;
	int		olds,oldz,oldx,oldy;

	// This is sort of a game state..
	unsigned int	runcodes;
	unsigned long	mousex;
	unsigned long	mousey;

	long			score;		// Current score
	float			tilespeed;	// How fast a tile travels down screen

	int				newpiece;	// New random piece - shown on console
	int				currpiece;	// Piece travelling down screen

	// Test verts!
	VertexType consoleverts[6];
	void InitConsoleVerts()
	{
		consoleverts[0].p.x = - 100.0f + 0.0f; consoleverts[0].p.y = - 200.0f + 0.0f; consoleverts[0].p.z = 600.0f;
		consoleverts[0].c = ColorChar4(1.0f,1.0f,1.0f,1.0f);
		consoleverts[0].t = Texel(0.0f, 1.0f);

		consoleverts[1].p.x = - 100.0f + 300.0f; consoleverts[1].p.y = - 200.0f + 420.0f; consoleverts[1].p.z = 600.0f;
		consoleverts[1].c = ColorChar4(1.0f,1.0f,1.0f,1.0f);
		consoleverts[1].t = Texel(1.0f, 0.0f);

		consoleverts[2].p.x = - 100.0f + 300.0f; consoleverts[2].p.y = - 200.0f + 0.0f; consoleverts[2].p.z = 600.0f;
		consoleverts[2].c = ColorChar4(1.0f,1.0f,1.0f,1.0f);
		consoleverts[2].t = Texel(1.0f, 1.0f);

		consoleverts[3].p.x = - 100.0f + 0.0f; consoleverts[3].p.y = - 200.0f + 0.0f; consoleverts[3].p.z = 600.0f;
		consoleverts[3].c = ColorChar4(1.0f,1.0f,1.0f,1.0f);
		consoleverts[3].t = Texel(0.0f, 1.0f);

		consoleverts[4].p.x = - 100.0f + 0.0f; consoleverts[4].p.y = - 200.0f + 420.0f; consoleverts[4].p.z = 600.0f;
		consoleverts[4].c = ColorChar4(1.0f,1.0f,1.0f,1.0f);
		consoleverts[4].t = Texel(0.0f, 0.0f);

		consoleverts[5].p.x = - 100.0f + 300.0f; consoleverts[5].p.y = - 200.0f + 420.0f; consoleverts[5].p.z = 600.0f;
		consoleverts[5].c = ColorChar4(1.0f,1.0f,1.0f,1.0f);
		consoleverts[5].t = Texel(1.0f, 0.0f);
	};

	VertexType tileverts[6];
	/*
	{
		{ 0.0f,  0.0f, 0.0f, 0x88888888, 0.0f, 0.0f }, // x, y, z, color
		{ 20.0f, 20.0f, 0.0f, 0x88888888, 1.0f, 1.0f },
		{ 20.0f, 0.0f, 0.0f, 0x88888888, 1.0f, 0.0f },

		{ 0.0f, 0.0f, 0.0f, 0x88888888, 0.0f, 0.0f },
		{ 0.0f, 20.0f, 0.0f, 0x88888888, 0.0f, 1.0f },
		{ 20.0f, 20.0f, 0.0f, 0x88888888, 1.0f, 1.0f },
	};
	*/

	// MaterialType vector
	typedef vector<MaterialType *>	MaterialVec;
	typedef vector<MaterialIndexType *>	MaterialIndexVec;

	MaterialVec			matindex;
	MaterialIndexVec	meshindex;

	int					txconsole;
	int					txtile;

	int					txconsoleMat;
	int					txtileMat;

	MaterialIndexType	*msconsole;
	MaterialIndexType	*mstile;

	// Testing stuff here
	int meshId = -1;
	int meshRenderId = -1;

	int		vieww=10;
	int		viewh=18;
	int		saved[190];
	int		tmap[190];
	int		view[19];

	unsigned char size[]={4,1,1,4,4,1,1,4,
					 3,2,2,3,3,2,2,3,
					 3,2,2,3,3,2,2,3,
					 3,2,2,3,3,2,2,3,
					 3,2,2,3,3,2,2,3,
					 3,2,2,3,3,2,2,3,
					 2,2,2,2,2,2,2,2};

	int shapes[]   ={
				//straight
					 0x0F,0x00,0x00,0x00,
					 0x08,0x08,0x08,0x08,
					 0x0F,0x00,0x00,0x00,
					 0x08,0x08,0x08,0x08,
				//tee
					 0x0E,0x04,0x00,0x00,
					 0x04,0x0C,0x04,0x00,
					 0x04,0x0E,0x00,0x00,
					 0x08,0x0C,0x08,0x00,
				//lel
					 0x08,0x0E,0x00,0x00,
					 0x0C,0x08,0x08,0x00,
					 0x0E,0x02,0x00,0x00,
					 0x04,0x04,0x0C,0x00,
				//rel
					 0x02,0x0E,0x00,0x00,
					 0x08,0x08,0x0C,0x00,
					 0x0E,0x08,0x00,0x00,
					 0x0C,0x04,0x04,0x00,
				//lzee
					 0x0C,0x06,0x00,0x00,
					 0x04,0x0C,0x08,0x00,
					 0x0C,0x06,0x00,0x00,
					 0x04,0x0C,0x08,0x00,
				//rzee
					 0x06,0x0C,0x00,0x00,
					 0x08,0x0C,0x04,0x00,
					 0x06,0x0C,0x00,0x00,
					 0x08,0x0C,0x04,0x00,
				//square
					 0x0C,0x0C,0x00,0x00,
					 0x0C,0x0C,0x00,0x00,
					 0x0C,0x0C,0x00,0x00,
					 0x0C,0x0C,0x00,0x00
					};
	
//	unsigned char map[22][12]; // One block either side and above..

	// Function prototypes

	void Init();
	void InitMeshes();
	void RunMainFunc(Packet::PackClass *in);

	void Update();
	void RenderConsole(void);

	void ClearArrays();
	void ClearMap();
	void MapRender();
	void DrawTile(VertexType &v,unsigned int *flag);

	void NewPiece();
	void DeletePiece();
	void TurnCW();
	void TurnCCW();
	void MoveDown();
	void MoveLeft();
	void MoveRight();

	void PieceUpdate();
	bool CheckPiece(int ss,int sz,int x,int y);
	void CheckLine();
	void MapPiece(int ss,int sz,int x,int y);


	void PreInit()
	{
		// Without this there is no way to Init the module, since the recieve will fail.
		apollo.SubmitFunction(cJetris::Init,"Init");
		apollo.SubmitFunction(cJetris::Kill,"Kill");
		apollo.SubmitFunction(cJetris::RunMainFunc,"RunMain");

		apollo.SubmitFunction(cJetris::ShutDown,"ShutDown");
		apollo.SubmitFunction(cJetris::SetMesh,"SetMesh");
		apollo.SubmitFunction(cJetris::LeftMouse,"LeftMouse");
		apollo.SubmitFunction(cJetris::NewGame,"NewGame");
		apollo.SubmitFunction(cJetris::RotatePiece,"RotatePiece");
		apollo.SubmitFunction(cJetris::MoveLeft,"MoveLeft");
		apollo.SubmitFunction(cJetris::MoveRight,"MoveRight");
		apollo.SubmitFunction(cJetris::DownFast,"DownFast");
		apollo.SubmitFunction(cJetris::Pause,"Pause");

		apollo.SubmitFunction(cJetris::RenderDone,"RenderDone");
		apollo.SubmitFunction(cJetris::InputDone,"InputDone");
	}

	// Use this to submit your calls - external submission not available yet
	void Init(Packet::PackClass *p)
	{
		runcodes = NOTHING;
		frame_complete = 0;
		mousex = 0;
		mousey = 0;

		// Clear Map
		ClearMap();

//		pause_flag=false;
//		g.drawImage(im_con,0,0,this);
//		ClearView();

		nexts=(int)(rand()/RAND_MAX*7.0f);
		nextz=(int)(rand()/RAND_MAX*4.0f);

		NewPiece();
		speed=500;
		frames=0;
		score=0;
		counter=0;
//		draw_score();
//		draw_linecount();

		srand( (unsigned)time( NULL ) );

		InitConsoleVerts();

		// Start the first default Homer script here
		{
		Packet("Jetris.Init","Zeus.LoadScript");
		Padd "data\\Apps\\Jetris\\Scripts\\init.hsf";
		Padd "JetrisInit";
		Psend;
		}

		{
		Packet("Jetris.Init","Zeus.StartRec");
		Padd "JetrisInit";
		Psend;
		}

		InitMeshes();

		runcodes = INIT;

		// Register a main loop function - call back like i guess..
		{
		Packet("Jetris.RunMain", "Zeus.RegisterFunc");
		Padd (unsigned long)&RunMainFunc;
		Psend;
		}
	}

	// Use this to cleanup anything nasty at the end of the module runtime
	void Kill(Packet::PackClass *p)
	{
		ProfileDumpAll();
		meshindex.clear();
	}

	void RenderDone(Packet::PackClass *p)
	{
		frame_complete = 1;
	}

	void InputDone(Packet::PackClass *p)
	{
	}

	void ClearMap()
	{
		memset(tmap,0, sizeof(tmap));
		ClearArrays();
	}

	void ClearArrays()
	{
		int	x,y;

		for(y=0;y<viewh;y++)
		{
			view[y]=0x801;
			for(x=0;x<vieww;x++)
				saved[y*vieww+x]=0xFF;
		}
		view[18]=0xFFFF;
	}

// Loop through all the material/texture files to load
// Could put a list of materials to load in a cfg file and parse it
	void InitMeshes()
	{
		// Load in the console texture
		{
		Packet("Jetris","Material.CreateMaterial");
		Psend;

		txconsoleMat = packet.GetData(D_INT);
		}

		{
		Packet("Jetris","Material.LoadTexture");
		Padd "Data\\Apps\\Jetris\\window.bmp"; 
		Psend;

		txconsole = packet.GetData(D_INT);
		}

		{
		Packet("Jetris", "Material.AddTexture");
		Padd (int)txconsole;
		Padd (int)txconsoleMat;
		Psend;
		}

		// Add the meshes
		MaterialIndexType	*newmesh = new MaterialIndexType;
		newmesh->name << "ConsoleMesh";
		meshindex.push_back(newmesh);
		msconsole = newmesh;

		{
		Packet("Jetris","Render.AddMesh");
		Padd (unsigned long)consoleverts;		// console vert data
		Padd (unsigned int) 6;					// 6 verts 
		Padd (int)txconsoleMat;			// associated material index
		Padd (unsigned int) VERTEX_XYZ | VERTEX_COLOR | VERTEX_TEX1;
		Psend;

		// Grab the mesh handle
		msconsole->index = packet.GetData(D_INT);
		}

		{
		Packet("Jetris", "Material.CreateMaterial");
		Psend;

		txtileMat = packet.GetData(D_INT);
		}

		{
		Packet("Jetris", "Material.LoadTexture");
		Padd "Data\\Apps\\Jetris\\tile.bmp"; 
		Psend;

		txtile = packet.GetData(D_INT);
		}

		{
		Packet("Jetris", "Material.AddTexture");
		Padd (int)txtile;
		Padd (int)txtileMat;
		Psend;
		}

		newmesh = new MaterialIndexType;
		newmesh->name << "TileMesh";
		meshindex.push_back(newmesh);
		mstile = newmesh;

		{
		Packet("Jetris","Render.AddMesh");
		Padd (unsigned long)tileverts;		// tile vert data
		Padd (unsigned int) 6;				// 6 verts 
		Padd (int)txtileMat;				// associated material index
		Padd (unsigned int) VERTEX_XYZ | VERTEX_COLOR | VERTEX_TEX1;
		Psend;

		// Grab the mesh handle
		mstile->index = packet.GetData(D_INT);
		}

		// MeshManager Test
		{
		Packet("Jetris", "MeshManager.LoadMesh");
		Padd "Data\\Apps\\Jetris\\complexmesh.cob";
		Psend;

		meshId = packet.GetData(D_INT);
		}

		// MeshManager Test
		{
		Packet("Jetris", "MeshManager.AddRenderMesh");
		Padd (int)meshId;
		Psend;

		meshRenderId = packet.GetData(D_INT);
		}

	}

	void RunMainFunc(Packet::PackClass *in)
	{
		// Check runcodes.. do what you gotta do.. then return..
		// More likely to be filled with a heap of tests and function calls..

		switch(runcodes)
		{
		case INIT:
			{
				// Init camera and view space
				// View/Camera matrix
				static D3DXMATRIX matView;
				D3DXMatrixLookAtLH( &matView, &D3DXVECTOR3( 0.0f, 0.0f, -1.0f ),
											  &D3DXVECTOR3( 0.0f, 0.0f, 0.0f ),
											  &D3DXVECTOR3( 0.0f, 1.0f, 0.0f ) );

				{
				Packet("Jetris","Render.SetCamera");
				Padd (unsigned long)&matView;
				Psend;
				}

				// Ortho Projection
				static D3DXMATRIX matProj;
				//D3DXMatrixOrthoLH( &matProj, 800, 600, 1.0f, 500.0f );
				D3DXMatrixPerspectiveFovLH(  &matProj, 1.047198f, 800.0f/600.0f,1.0f,5000.0f );

				{
				Packet("Jetris","Render.SetProjection");
				Padd (unsigned long)&matProj;
				Psend;
				}

				runcodes = STARTUP;
			}
			break;

		case STARTUP:
			Update();		// Input and Render are updated
			RenderConsole();
			break;
		case GAME:
			Update();		// Input and Render are updated
			RenderConsole();
			PieceUpdate();
			break;
		case PAUSE:
			Update();		// Input and Render are updated
			RenderConsole();
			break;
		case SHUTDOWN:
	// Kill all processes.. and inform zeus..
			Packet("Jetris","Zeus.Shutdown");
			Psend;
			break;
		}
	}

	void Update()
	{
	}		

	void PieceUpdate()
	{
		static long oldtime = 0;

		if((GetTickCount()-oldtime)>speed)
		{
			oldtime = GetTickCount();
			frames=0;
			DeletePiece();
			if(CheckPiece(pieces,piecez,piecex,piecey+20)==true)
			{
//				if((buttons_on&0x10)==0x10)
//					score+=1;
				piecey+=20;
				MapPiece(pieces,piecez,piecex,piecey);
			}
			else
			{
				MapPiece(pieces,piecez,piecex,piecey);
				CheckLine();
				NewPiece();
				if(CheckPiece(pieces,piecez,piecex,piecey)==false)
					runcodes = STARTUP;
				else
					MapPiece(pieces,piecez,piecex,piecey);
			}
		}
		frames++;
	}

	void RenderConsole(void)
	{
		if(txconsole != 0)
		{
			// Set the console mesh
			Packet("Jetris","Render.RenderMesh");
			Padd (int)msconsole->index;
			Padd -100.0f; Padd -200.0f;Padd 600.0f;
			Padd (unsigned int)0xFFFFFFFF;
			Psend;

			{
			D3DXMATRIX matWorld;
			D3DXMATRIX matScale;

			D3DXMatrixScaling( &matScale, 200.0f, 200.0f, 200.0f);
			D3DXMatrixRotationY( &matWorld, timeGetTime()/1500.0f );
			D3DXMatrixMultiply( &matWorld, &matWorld, &matScale);

			{
			Packet("Jetris","MeshManager.MultMeshTransform");
			Padd (int)meshId;
			Padd (unsigned long)&matWorld;
			Psend;
			}

			// Test mesh
			Packet("Jetris","MeshManager.RenderMesh");
			Padd (int)meshId;
			Psend;
			}
		}

		MapRender();

		Data::StringClass sc;
		sc = "Score: ";
		sc << score;

		{
		Packet("Jetris", "Font.RenderChars")
		Padd sc.String();
		Padd (int)0xe57f4e2f; // arial
		Padd -100.0f; Padd -220.0f; Padd 589.0f;
		Padd 0.5f;
		Psend;
		}

		// Call a Render Frame!
		Packet("Jetris","Render.DoRenderFrame");
		Psend;
	}

	void MapRender()
	{
		Profile("Jetris::MapRender");

		unsigned int flag = 1;
		// Render the entire map :-)
		for(int r=0;r<18;r++)
		{
			for(int c=0;c<10;c++)
			{
				VertexType v;
				switch(saved[r*vieww+c])
				{
				case 0:
					v.c.rgba = 0xFF0000FF;
					v.p.x = (float)(c+1)*20.0f;
					v.p.y = (float)(19-r)*20.0f+1.0f;
					DrawTile(v,&flag);
					break;
				case 1:
					v.c.rgba = 0xFF00AAFF;
					v.p.x = (float)(c+1)*20.0f;
					v.p.y = (float)(19-r)*20.0f+1.0f;
					DrawTile(v,&flag);
					break;
				case 2:
					v.c.rgba = 0xFF00FFFF;
					v.p.x = (float)(c+1)*20.0f;
					v.p.y = (float)(19-r)*20.0f+1.0f;
					DrawTile(v,&flag);
					break;
				case 3:
					v.c.rgba = 0xFF00FF00;
					v.p.x = (float)(c+1)*20.0f;
					v.p.y = (float)(19-r)*20.0f+1.0f;
					DrawTile(v,&flag);
					break;
				case 4:
					v.c.rgba = 0xFFFF00FF;
					v.p.x = (float)(c+1)*20.0f;
					v.p.y = (float)(19-r)*20.0f+1.0f;
					DrawTile(v,&flag);
					break;
				case 5:
					v.c.rgba = 0xFFFF0000;
					v.p.x = (float)(c+1)*20.0f;
					v.p.y = (float)(19-r)*20.0f+1.0f;
					DrawTile(v,&flag);
					break;
				case 6:
					v.c.rgba = 0xFFFFFF00;
					v.p.x = (float)(c+1)*20.0f;
					v.p.y = (float)(19-r)*20.0f+1.0f;
					DrawTile(v,&flag);
					break;
				}
			}
		}
	}

	void DrawTile(VertexType &v, unsigned int *flag)
	{
//		Profile("Jetris::DrawTile");

		{
		Packet("Jetris","Render.PrimRectangle");
		Padd txtileMat;
		// x,y,z
		Padd (v.p.x-100.0f);Padd (v.p.y-200.0f);Padd 599.9f;
		// size x, size y
		Padd 20.0f; Padd 20.0f;
		// uv Topleft and uv Bottomright
		Padd 0.0f; Padd 0.0f; Padd 1.0f; Padd 1.0f;
		// r,g,b,a
		Padd v.c.rgba;
		Psend;
		}
	}

	bool CheckPiece(int ss,int sz,int x,int y)
	{
		int yloop;
		int temp,pos;

		int shape=ss*16+sz*4;
		int	ysize=size[ss*8+sz*2+1];
		int	xsize=size[ss*8+sz*2];

		x=x/20;
		y=y/20;

		for(yloop=0;yloop<ysize;yloop++)
		{
			temp=shapes[shape+yloop]>>(4-xsize);
			pos=(12-xsize-x);
			if(pos>=0)
				temp=temp<<pos;
			else
				temp=temp>>abs(pos);
			if((temp&(view[y+yloop-1]))>0) 
				return false;				  
		}
		return true;
	}

	void MapPiece(int ss,int sz,int x,int y)
	{
		int xloop,yloop;
		int	temp;

		for(yloop=0;yloop<size[ss*8+sz*2+1];yloop++)
		{
			temp=shapes[ss*16+sz*4+yloop]>>(4-size[ss*8+sz*2]);
			temp=temp<<(12-size[ss*8+sz*2]-(x/20));
			view[y/20+yloop-1]|=temp;
			for(xloop=0;xloop<4;xloop++)
				if(( (8>>xloop) & (shapes[ss*16+sz*4+yloop]) )>0)
					saved[(y/20+yloop-1)*vieww+x/20+xloop-1]=ss;
		}
	}

	void NewPiece()
	{
		pieces=nexts;
		piecez=nextz;
		piecex=(size[nexts*8+nextz*2]<3) ? 100 : 80;
		piecey=20;
		
		nexts=(int)(((float)rand()/RAND_MAX)*7.0f);
		nextz=(int)(((float)rand()/RAND_MAX)*4.0f);

//		draw_score();
//		draw_next();
//		status=3;
	}

	void DeletePiece()
	{
		int xloop,yloop;
		int	temp;

		for(yloop=0;yloop<size[pieces*8+piecez*2+1];yloop++)
		{
			temp=shapes[pieces*16+piecez*4+yloop]>>(4-size[pieces*8+piecez*2]);
			temp=temp<<(12-size[pieces*8+piecez*2]-(piecex/20));
			view[piecey/20+yloop-1]^=temp;
			for(xloop=0;xloop<4;xloop++)
				if(( (8>>xloop) & (shapes[pieces*16+piecez*4+yloop]) )>0)
					saved[(piecey/20+yloop-1)*vieww+piecex/20+xloop-1]=0xFF;
		}
	}

	void TurnCW()
	{
		int tempz;

//		g.drawImage(im_cw,230,300,this);
		if(piecez>2)
			tempz=0;
		else
			tempz=piecez+1;

		DeletePiece();
		if(CheckPiece(pieces,tempz,piecex,piecey))
		{
			piecez=tempz;
			MapPiece(pieces,piecez,piecex,piecey);
		}
		else
		{
			MapPiece(pieces,piecez,piecex,piecey);
		}
	}

	void TurnCCW()
	{
		int tempz;

//		g.drawImage(im_ccw,230,340,this);
		if(piecez<1)
			tempz=3;
		else
			tempz=piecez-1;

		DeletePiece();
		if(CheckPiece(pieces,tempz,piecex,piecey))
		{
			piecez=tempz;
			MapPiece(pieces,piecez,piecex,piecey);
		}	
		else
		{
			MapPiece(pieces,piecez,piecex,piecey);
		}
	}

	void MoveDown()
	{
		int tempy=0;

//		g.drawImage(im_dn,230,260,this);
		tempy=piecey+20;
		DeletePiece();
		if(CheckPiece(pieces,piecez,piecex,tempy))
		{
			piecey=tempy;
			MapPiece(pieces,piecez,piecex,piecey);
		}
		else
		{
			MapPiece(pieces,piecez,piecex,piecey);
		}
	}
	
	void MoveLeft()
	{
		int tempx;

		tempx = piecex-20;

//		g.drawImage(im_left,230,220,this);
		DeletePiece();
		if(CheckPiece(pieces,piecez,tempx,piecey))
		{
			piecex=tempx;
			MapPiece(pieces,piecez,piecex,piecey);
		}
		else
		{
			MapPiece(pieces,piecez,piecex,piecey);
		}
	}

	void MoveRight()
	{
		int tempx;

		tempx = piecex+20;
//		g.drawImage(im_right,260,220,this);
		DeletePiece();
		if(CheckPiece(pieces,piecez,tempx,piecey))
		{
			piecex=tempx;
			MapPiece(pieces,piecez,piecex,piecey);
		}
		else
		{
			MapPiece(pieces,piecez,piecex,piecey);
		}
	}

	void CheckLine()
	{
		int yloop;
		int	lines_removed;
		int tview[20];
		int	tsaved[190];

		lines_removed=0;
		for (yloop=0;yloop<18;yloop++)
		{
			if(view[yloop]==0xFFF)
			{
				lines_removed++;
				if(yloop>0)
				{
					memcpy(tview,view,sizeof(view[0])*(yloop));
					memcpy(&view[1],tview,sizeof(view[0])*(yloop));
					memcpy(tsaved, saved,sizeof(saved[0])*(yloop)*vieww);
					memcpy(&saved[vieww],tsaved,sizeof(saved[0])*(yloop)*vieww);
				}
				view[0]=0x801;
			}
		}

		switch(lines_removed)
		{
		case 1: score+=100;break;
		case 2: score+=200;break;
		case 3: score+=500;break;
		case 4: score+=1000;break;
		default:;
		}

		if(lines_removed>0)
		{
			counter+=lines_removed;
			if((counter%10)==9) speed=(speed>0)? speed-1 : 0;
//				draw_score(); 
//				draw_linecount();
		}
	}						

	void ShutDown(Packet::PackClass *input)
	{
		runcodes = SHUTDOWN;
	}

	void SetMesh(Packet::PackClass *input)
	{
	}

	void LeftMouse(Packet::PackClass *input)
	{
	}

	void NewGame(Packet::PackClass *input)
	{
		ClearMap();
		ClearArrays();
		NewPiece();
		MapPiece(pieces,piecez,piecex,piecey);
		runcodes = GAME;
		score = 0;
	}

	void RotatePiece(Packet::PackClass *input)
	{
		if(runcodes==GAME)
			TurnCW();
	}

	void MoveLeft(Packet::PackClass *input)
	{
		if(runcodes==GAME)
			MoveLeft();
	}

	void MoveRight(Packet::PackClass *input)
	{
		if(runcodes==GAME)
			MoveRight();
	}

	void DownFast(Packet::PackClass *input)
	{
		if(runcodes==GAME)
			MoveDown();
	}

	void Pause(Packet::PackClass *input)
	{
		if(runcodes==GAME)
			runcodes = PAUSE;
		else if(runcodes==PAUSE)
			runcodes = GAME;
	}

}
