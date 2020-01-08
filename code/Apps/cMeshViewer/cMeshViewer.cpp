
// Make sure you include headers for modules you intend to use. They may
// have types or structures you may need to cast pointers to.

#include "..\\..\\System\\System.h"

#include "cMeshViewer.h"

#include "plib\\sg.h"
#include "plib\\ssg.h"

namespace cMeshViewer
{
	ApolloClass	apollo;

	// scene graph
	ssgRoot *scene = NULL ;
	ssgTransform *g_obj = NULL;

	// polar movement parameters
	GLfloat EyeDist= 100.0f;
	GLfloat EyeAz  = 0.0f;
	GLfloat EyeEl  = 30.0f;
	GLfloat Ex     = 0.0f;
	GLfloat Ey     = 0.0f;
	GLfloat Ez     = 0.0f;

	#define FOV 45.0f

	// Function declarations
	void RunMainFunc(Packet::PackClass *p);
	void Shutdown(Packet::PackClass *p);
	
	void ZoomIn(Packet::PackClass *p);
	void ZoomOut(Packet::PackClass *p);
	void CWYAxis(Packet::PackClass *p);
	void CCWYAxis(Packet::PackClass *p);
	
	void PreInit()
	{
		// Without this there is no way to Init the module, since the recieve will fail.
		apollo.SubmitFunction(cMeshViewer::Init,"Init");
		apollo.SubmitFunction(cMeshViewer::Kill,"Kill");

		apollo.SubmitFunction(cMeshViewer::ZoomIn,"ZoomIn");
		apollo.SubmitFunction(cMeshViewer::ZoomOut,"ZoomOut");
		apollo.SubmitFunction(cMeshViewer::CWYAxis,"CWYAxis");
		apollo.SubmitFunction(cMeshViewer::CCWYAxis,"CCWYAxis");

		apollo.SubmitFunction(cMeshViewer::Shutdown,"Shutdown");
	}

	int num_anim_frames =0;

	void count_anim_frames( ssgEntity *e )
	{
	  if ( e -> isAKindOf ( ssgTypeBranch() ) )
	  {
		ssgBranch *br = (ssgBranch *) e ;
    
		for ( int i = 0 ; i < br -> getNumKids () ; i++ )
		  count_anim_frames ( br -> getKid ( i ) ) ;
    
		if ( e -> isAKindOf ( ssgTypeSelector() ) )
		{
		  ssgSelector* p = (ssgSelector*) e ;
		  int num = p -> getMaxKids () ;
		  if ( num > num_anim_frames )
			num_anim_frames = num ;
		}
		else if ( e -> isAKindOf ( ssgTypeTransform() ) )
		{
		  ssgBase* data = e -> getUserData () ;
		  if ( data != NULL && data -> isAKindOf ( ssgTypeTransformArray() ) )
		  {
			ssgTransformArray* ta = (ssgTransformArray*) data ;
			int num = ta -> getNum () ;
			if ( num > num_anim_frames )
			  num_anim_frames = num ;
		  }
		}
	  }
	  else if ( e -> isAKindOf ( ssgTypeLeaf() ) )
	  {
		ssgLeaf* leaf = (ssgLeaf *) e ;
		ssgState* st = leaf -> getState () ;

		if ( st && st -> isAKindOf ( ssgTypeStateSelector() ) )
		{
		  ssgStateSelector* ss = (ssgStateSelector*) st ;

		  int num = ss -> getNumSteps () ;
		  if ( num > num_anim_frames )
			num_anim_frames = num ;
		}
	  }
	}


	// Use this to submit your calls - external submission not available yet
	void Init(Packet::PackClass *p)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glDepthFunc(GL_LESS);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
		glShadeModel(GL_SMOOTH);

		// Initialise PLIB
		ssgInit () ;

		// Set up the viewing parameters
		ssgSetFOV     ( FOV, 0.0f ) ;
		ssgSetNearFar ( 2.0f, 10000.0f ) ;

		// Set up the Sun.
		sgVec3 sunposn ;
		sgSetVec3 ( sunposn, 200.0f, -500.0f, 500.0f ) ;
		ssgGetLight ( 0 ) -> setPosition ( sunposn ) ;

		// Set up the path to the data files
		ssgModelPath   ( "E:\\DEVELOP\\DEITY2\\DATA\\Apps\\MeshView\\mesh" ) ;
		ssgTexturePath ( "E:\\DEVELOP\\DEITY2\\DATA\\Apps\\MeshView\\mesh" ) ;

		// Create a root node
		scene = new ssgRoot      ;
		g_obj = new ssgTransform ;

		ssgEntity *obj = ssgLoad ( p->GetData(D_STRING) ) ;

		g_obj  -> addKid ( obj  ) ;

//		ssgFlatten         ( obj  ) ;
		ssgStripify        ( g_obj  ) ;

		if ( obj )
		{
			scene -> addKid ( g_obj ) ;
		}

//		ssgSave("NewAse.ASE",obj);

		num_anim_frames = 0 ;
//		count_anim_frames ( scene ) ;

		int HSF_MeshViewInit = -1;

		// Start the first default Homer script here
		{
		Packet("MeshView.Init","Zeus.LoadScript");
		Padd "data\\Apps\\MeshView\\Scripts\\init.hsf";
		Psend;
		HSF_MeshViewInit = packet.GetData(D_INT);
		}

		{
		Packet("MeshView.Init","Zeus.StartRec");
		Padd (int) HSF_MeshViewInit;
		Psend;
		}

		// Register a main loop function - call back like i guess..
		{
		Packet("MeshView.RunMain", "Zeus.RegisterFunc");
		Padd (unsigned long)&RunMainFunc;
		Psend;
		}
	}

	// Use this to cleanup snything nasty at the end of the module runtime
	void Kill(Packet::PackClass *p)
	{
	}

	int anim_frame = 0;

	void set_anim_frame( ssgEntity *e )
	{
	  if ( e -> isAKindOf ( ssgTypeBranch() ) )
	  {
		ssgBranch *br = (ssgBranch *) e ;
    
		for ( int i = 0 ; i < br -> getNumKids () ; i++ )
		  set_anim_frame ( br -> getKid ( i ) ) ;
    
		if ( e -> isAKindOf ( ssgTypeSelector() ) )
		{
		  ssgSelector* p = (ssgSelector*) e ;
		  int num = p -> getMaxKids () ;
		  if ( num > 0 )
		  {
			int frame = anim_frame ;
			if ( frame >= num )
			  frame = num-1 ;
			p -> selectStep ( frame ) ;
		  }
		}
		else if ( e -> isAKindOf ( ssgTypeTransform() ) )
		{
		  ssgBase* data = e -> getUserData () ;
		  if ( data != NULL && data -> isAKindOf ( ssgTypeTransformArray() ) )
		  {
			ssgTransform* p = (ssgTransform*) e ;
			ssgTransformArray* ta = (ssgTransformArray*) data ;
			int num = ta -> getNum () ;
			if ( num > 0 )
			{
			  int frame = anim_frame ;
			  if ( frame >= num )
				frame = num-1 ;
			  ta -> selection = frame ;
			  p -> setTransform ( *( ta -> get ( ta -> selection ) ) ) ;
			}
		  }
		}
	  }
	  else if ( e -> isAKindOf ( ssgTypeLeaf() ) )
	  {
		ssgLeaf* leaf = (ssgLeaf *) e ;
		ssgState* st = leaf -> getState () ;

		if ( st && st -> isAKindOf ( ssgTypeStateSelector() ) )
		{
		  ssgStateSelector* ss = (ssgStateSelector*) st ;
		  int num = ss -> getNumSteps () ;
		  if ( num > 0 )
		  {
			int frame = anim_frame ;
			if ( frame >= num )
			  frame = num-1 ;
			ss -> selectStep ( frame ) ;
		  }
		}
	  }
	}

	void DrawMesh(void)
	{
      anim_frame = 0 ;
//      set_anim_frame ( scene ) ;
	}

	void make_matrix( sgMat4 mat )
	{
	  SGfloat angle = -EyeAz * SG_DEGREES_TO_RADIANS ;
	  sgVec3 eye ;
	  eye[0] = (SGfloat) cos (angle) * EyeDist ;
	  eye[1] = (SGfloat) sin (angle) * EyeDist ;
	  angle = EyeEl * SG_DEGREES_TO_RADIANS ;
	  eye[2] = (SGfloat) sin (angle) * EyeDist ;

	  sgVec3 center ;
	  sgSetVec3 ( center, Ex, Ey, Ez ) ;

	  sgVec3 up ;
	  sgSetVec3 ( up, 0.0f, 0.0f, 0.1f ) ;

	  sgMakeLookAtMat4 ( mat, eye, center, up ) ;
	}

	void RunMainFunc(Packet::PackClass *p)
	{
		// Draw the loaded mesh and keep ticking
		{
		Packet("MeshView","Render.Begin");
		Psend;
		}

		sgMat4 mat ;
		make_matrix ( mat ) ;
		ssgSetCamera ( mat );

//		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//		glEnable(GL_BLEND);
//		glEnable( GL_DEPTH_TEST ) ;

		DrawMesh();

		ssgCullAndDraw ( scene ) ;

		{
		Packet("MeshView","Render.End");
		Psend;
		}
	}

	void ZoomIn(Packet::PackClass *p)
	{
		if(p->srccrc.GetFuncId() == 0xe70eb490) // keydown
		{
		EyeDist -= 1.0f;
		}
	}

	void ZoomOut(Packet::PackClass *p)
	{
		if(p->srccrc.GetFuncId() == 0xe70eb490) // keydown
		{
		EyeDist += 1.0f;
		}
	}
	
	void CWYAxis(Packet::PackClass *p)
	{
		if(p->srccrc.GetFuncId() == 0xe70eb490) // keydown
		{
		EyeAz += 1.0f;
		}
	}

	void CCWYAxis(Packet::PackClass *p)
	{
		if(p->srccrc.GetFuncId() == 0xe70eb490) // keydown
		{
		EyeAz -= 1.0f;
		}
	}

	void ReloadMesh(Packet::PackClass *p)
	{
	}

	// Close the app nicely
	void Shutdown(Packet::PackClass *p)
	{
		// Tells Zeus to finish up.
		Packet("MeshView","Zeus.Shutdown");
		Psend;
	}

}
