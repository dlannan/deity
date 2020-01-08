
// Make sure you include headers for modules you intend to use. They may
// have types or structures you may need to cast pointers to.

#include "cCrossEyed.h"
#include "..\\..\\System\\System.h"

#include "plib\\sg.h"
#include "plib\\ssg.h"

#include <vector>

namespace cCrossEyed
{
	ApolloClass	apollo;

	// scene graph
	ssgRoot *scene = NULL ;
	ssgTransform *g_obj = NULL;

	// Each block
	ssgEntity *TL,*T,*TR;
	ssgEntity *CL,*C,*CR;
	ssgEntity *BL,*B,*BR;

	vector<ssgEntity*>	bList;	// List of our above entities, ordered top-left to bottom-right
	ssgEntity* selected;
	static int selectedI = 0;

	// Local clock for all sorts of things
	Timer::Clock	clock;

	// polar movement parameters
	GLfloat EyeDist= 300.0f;
	GLfloat EyeAz  = 90.0f;
	GLfloat EyeEl  = 14.0f;
	GLfloat Ex     = 0.0f;
	GLfloat Ey     = 0.0f;
	GLfloat Ez     = 50.0f;

	#define FOV 45.0f

	// Function declarations
	void RunMainFunc(Packet::PackClass *p);
	void Shutdown(Packet::PackClass *p);
	
	void ZoomIn(Packet::PackClass *p);
	void ZoomOut(Packet::PackClass *p);
	void MoveLeft(Packet::PackClass *p);
	void MoveRight(Packet::PackClass *p);
	
	void PreInit()
	{
		// Without this there is no way to Init the module, since the recieve will fail.
		apollo.SubmitFunction(cCrossEyed::Init,"Init");
		apollo.SubmitFunction(cCrossEyed::Kill,"Kill");

		apollo.SubmitFunction(cCrossEyed::ZoomIn,"ZoomIn");
		apollo.SubmitFunction(cCrossEyed::ZoomOut,"ZoomOut");
		apollo.SubmitFunction(cCrossEyed::MoveRight,"MoveRight");
		apollo.SubmitFunction(cCrossEyed::MoveLeft,"MoveLeft");

		apollo.SubmitFunction(cCrossEyed::Shutdown,"Shutdown");
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
		ssgModelPath   ( "E:\\DEVELOP\\DEITY2\\DATA\\Apps\\CrossEyed\\mesh" ) ;
		ssgTexturePath ( "E:\\DEVELOP\\DEITY2\\DATA\\Apps\\CrossEyed\\mesh" ) ;

		// Create a root node
		scene = new ssgRoot      ;
		g_obj = new ssgTransform ;

		ssgEntity *obj = ssgLoad ( p->GetData(D_STRING) ) ;

//		g_obj  -> addKid ( obj  ) ;

//		ssgFlatten         ( obj  ) ;
//		ssgStripify        ( g_obj  ) ;

//		if ( obj )
//		{
//			scene -> addKid ( g_obj ) ;
//		}

		TL = obj->getByName("TL");
		T = obj->getByName("T");
		TR = obj->getByName("TR");

		CL = obj->getByName("CL");
		C = obj->getByName("C");
		CR = obj->getByName("CR");

		BL = obj->getByName("BL");
		B = obj->getByName("B");
		BR = obj->getByName("BR");

		ssgTransform* t;
		// Add to a nice vector for easy control
		t = new ssgTransform;
		t  -> addKid ( TL  ) ;
		scene -> addKid ( t );
		bList.push_back(t);

		t = new ssgTransform;
		t  -> addKid ( T  ) ;
		scene -> addKid ( t );
		bList.push_back(t);

		t = new ssgTransform;
		t  -> addKid ( TR  ) ;
		scene -> addKid ( t );
		bList.push_back(t);
		
		t = new ssgTransform;
		t  -> addKid ( CL  ) ;
		scene -> addKid ( t );
		bList.push_back(t);

		t = new ssgTransform;
		t  -> addKid ( C  ) ;
		scene -> addKid ( t );
		bList.push_back(t);

		t = new ssgTransform;
		t  -> addKid ( CR  ) ;
		scene -> addKid ( t );
		bList.push_back(t);

		t = new ssgTransform;
		t  -> addKid ( BL  ) ;
		scene -> addKid ( t );
		bList.push_back(t);

		t = new ssgTransform;
		t  -> addKid ( B  ) ;
		scene -> addKid ( t );
		bList.push_back(t);

		t = new ssgTransform;
		t  -> addKid ( BR  ) ;
		scene -> addKid ( t );
		bList.push_back(t);

		selected = bList.at(0);

//		ssgSave("NewAse.ASE",obj);

		num_anim_frames = 0 ;
//		count_anim_frames ( scene ) ;

		int HSF_CrossEyedInit = -1;

		// Start the first default Homer script here
		{
		Packet("CrossEyed.Init","Zeus.LoadScript");
		Padd "data\\Apps\\CrossEyed\\Scripts\\init.hsf";
		Psend;
		HSF_CrossEyedInit = packet.GetData(D_INT);
		}

		{
		Packet("CrossEyed.Init","Zeus.StartRec");
		Padd (int) HSF_CrossEyedInit;
		Psend;
		}

		// Register a main loop function - call back like i guess..
		{
		Packet("CrossEyed.RunMain", "Zeus.RegisterFunc");
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

	void PulseSelected(void)
	{
		static unsigned char pulse = 0;
		static unsigned long lasttime = 0;

		if(lasttime == 0) lasttime = clock.Ms();

		// Animate the selected mesh - use a local clock for it
		// Make the size increase and decrease every 2 seconds
		float size = ((float)(clock.Ms() % 3141)) / 1000.0f;

		ssgTransform* p = (ssgTransform*) selected ;
		sgMat4 pT;
		float scale = 0.0f;
		p->getTransform(pT);

		scale = sinf(size) * 0.1 + 1.0f;

		pT[0][0] = scale;
		pT[1][1] = scale;
		pT[2][2] = scale;
		pT[3][3] = 1.0f;

		p->setTransform(pT);
	}

	void DrawMesh(void)
	{
		PulseSelected();
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
		// Update the local clock
		clock.Update();

		// Draw the loaded mesh and keep ticking
		{
		Packet("CrossEyed","Render.Begin");
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
		Packet("CrossEyed","Render.End");
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
	
	void MoveRight(Packet::PackClass *p)
	{
		if(p->srccrc.GetFuncId() == 0x79a3ea46) // keyreleased
		{
			// Restore selected transform original
		ssgTransform* p = (ssgTransform*) selected ;
		sgMat4 pT;
		p->getTransform(pT);

		pT[0][0] = 1.0f;
		pT[1][1] = 1.0f;
		pT[2][2] = 1.0f;
		pT[3][3] = 1.0f;

		p->setTransform(pT);

		selectedI++;
		if((selectedI % 3) == 0) selectedI -=3; 
		selected = bList.at(selectedI%3);
		}
	}

	void MoveLeft(Packet::PackClass *p)
	{
		if(p->srccrc.GetFuncId() == 0x79a3ea46) // keyreleased
		{
			// Restore selected transform original
		ssgTransform* p = (ssgTransform*) selected ;
		sgMat4 pT;
		p->getTransform(pT);

		pT[0][0] = 1.0f;
		pT[1][1] = 1.0f;
		pT[2][2] = 1.0f;
		pT[3][3] = 1.0f;

		p->setTransform(pT);

		selectedI--;
		if((selectedI % 3) == 2) selectedI +=3; 
		if(selectedI < 0) selectedI +=3;

		selected = bList.at(selectedI%3);
		}
	}

	void ReloadMesh(Packet::PackClass *p)
	{
	}

	// Close the app nicely
	void Shutdown(Packet::PackClass *p)
	{
		// Tells Zeus to finish up.
		Packet("CrossEyed","Zeus.Shutdown");
		Psend;
	}

}