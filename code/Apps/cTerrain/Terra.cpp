#include <time.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "..\\..\\Utils\\Image\\TGALoader.h"

#include <gl/gl.h>
#include <gl/glaux.h>
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glaux.lib")
#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"advapi32.lib")

typedef unsigned char byte;

// Pointer to TGA data
TGAFile	*world = NULL;

unsigned long sea[17000];
unsigned long land[17000];

typedef struct
{
	union
	{
		struct 
		{
			unsigned char b;
			unsigned char g;
			unsigned char r;
		} col;
		struct 
		{
			unsigned char	p;	// height value
			unsigned short	h;	// pad (should be 0)
		} ht;
	};

} RGBType;

inline float Height(int x, int y) 
{ 
	if(world)
		return ((RGBType *)world->GetImageData())[x + y * world->GetHeader()->image.width].ht.h/1000.0f; 
	else
		return 0.0f;
}

inline RGBType *Colour(int x, int y) 
{ 
	if(world)
		return &((RGBType *)world->GetImageData())[x + y * world->GetHeader()->image.width]; 
	else
		return NULL;
}

//
// Reduces a value to 0..255 (used in height field computation)
//
inline int Clamp(int x) { return (x<0 ? 0 : (x>255 ? 255 : x)); }

void make_colour_tables()
{
	int i;
//	for(i=0;i<5500;i++)
//	{
//		land[i]=(i*256/5500)<<8;
//		land[i+5500]=((i*256/5500)<<8)|((i*256/5500));
//		land[i+11000]=((i*256/5500)<<8)|((i*256/5500))|((i*256/5500)<<16);
//	}
	for(i=0;i<17000;i++)
	{
		sea[i]=(255-(unsigned int)((float)i*255/11000))*0xFF;
		land[i]=(255-(unsigned int)((float)i*255/8000))*0xFF;
	}
}

void DrawWorld()
{
	int h = 0;
	RGBType *col;

	for(int y=0; y<world->GetHeader()->image.height-1; y++)
	{
	glBegin( GL_QUAD_STRIP );
		col = Colour(0,y);
		h = Height(0,y);
		glColor3f( col->col.r/255.0f, col->col.g/255.0f, col->col.b/255.0f );
//		glVertex3f( (float)0.0f, h, (float)y );
		glVertex3f( (float)0.0f, h, (float)y );
		col = Colour(0,y+1);
		glColor3f( col->col.r/255.0f, col->col.g/255.0f, col->col.b/255.0f );
		h = Height(0,y+1);
//		glVertex3f( (float)0.0f, h, (float)y+1);
		glVertex3f( (float)0.0f, h, (float)y+1 );

		for(int x=1; x<world->GetHeader()->image.width; x++)
		{
			col = Colour(x,y);
			glColor3f( col->col.r/255.0f, col->col.g/255.0f, col->col.b/255.0f );
			h = Height(x,y);
//			glVertex3f( (float)x, h, (float)y );
			glVertex3f( (float)x, h, (float)y );
			col = Colour(x,y+1);
			glColor3f( col->col.r/255.0f, col->col.g/255.0f, col->col.b/255.0f );
			h = Height(x,y+1);
//			glVertex3f( (float)x, h, (float)y+1);
			glVertex3f( (float)x, h, (float)y+1 );
		}
	glEnd();
	}
}


void LoadWorld()
{
	world = new TGAFile("Terra.tga");

	make_colour_tables();
}


void Radii(float x0,float y0,float sr, float er)
{
}

//
// Draw the view from the point x0,y0 (16.16) looking at angle a
//
void View(int x0,int y0,float aa)
{
	// Fire a vector for left hand side view frustum start positions

	// Traverse a radii moving out in radius steps - preferably half the size of a heightfield cell

}

static float gl_ss, gl_sa, gl_a, gl_s;
static int gl_x0, gl_y0;
void rotate_left( void )
{
        glRotatef( -0.9, 0, 1, 0 );
}
void rotate_right( void )
{
        glRotatef( +0.9, 0, 1, 0 );
}
void key_1( void )
{
	static int line_smooth = 0;
	if( line_smooth )
	{
			glDisable( GL_LINE_SMOOTH );
			line_smooth = 0;
	}
	else
	{
			glEnable( GL_LINE_SMOOTH );
			line_smooth = 1;
	}
}
void key_2( void )
{
    static int flag = 1;
    if( flag )
    {
            glDisable( GL_TEXTURE_2D );
            flag = 0;
    }
    else
    {
            glEnable( GL_TEXTURE_2D );
            flag = 1;
    }
}
void key_4( void )
{
	glBlendFunc( GL_ZERO, GL_ONE );
}
void key_5( void )
{
	glBlendFunc( GL_ONE, GL_ZERO );
}
void key_6( void )
{
	glBlendFunc( GL_ZERO, GL_ZERO );
}
void key_7( void )
{
	glBlendFunc( GL_ONE, GL_ONE );
}
void key_8( void )
{
	glBlendFunc( GL_ONE_MINUS_DST_COLOR, GL_ONE );
//	glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_COLOR );
}
void key_9( void )
{
	glBlendFunc( GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_COLOR );
}
void space_key( void )
{
        gl_ss = 0;
}
void up_key( void )
{
        glRotatef( -0.9, 1, 0, 0 );
        gl_ss += gl_s;
}
void down_key( void )
{
        glRotatef( +0.9, 1, 0, 0 );
        gl_ss -= gl_s;
}
void left_key( void )
{
        glRotatef( -0.9, 0, 1, 0 );
        gl_sa -= 0.005;
}
void right_key( void )
{
        glRotatef( +0.9, 0, 1, 0 );
        gl_sa += 0.005;
}
void main( void )
{
    static int frame=0;
    static int prev_time=0;
    static float fps_time=0;
    static char buf[ 100 ]="fps is now being calculated";
    static float angle = 0.0;

	glDrawBuffer(GL_BACK);
	glClearColor( 0.5f, 0.5f, 0.5f, 0.5f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    angle += 0.3;
#if 0
    glBegin( GL_QUADS );

      glTexCoord2f(   0.0,   0.0 );
      glVertex3f(     0.0,   0.0, 1.0f );

      glTexCoord2f(   0.0,   1.0 );
      glVertex3f(     0.0, 480.0, 1.0f);

      glTexCoord2f(   1.0,   1.0 );
      glVertex3f(   640.0, 480.0, 1.0f);

      glTexCoord2f(   1.0,   0.0 );
      glVertex3f(   640.0,   0.0, 1.0f);

    glEnd( );
#endif

	glEnable( GL_BLEND );
//    View(gl_x0,gl_y0,gl_a);
	DrawWorld();
	glDisable( GL_BLEND );

    //
    // Update position/angle
    //
	gl_x0+=gl_ss*cos(gl_a);
	gl_y0+=gl_ss*sin(gl_a);
	gl_a+=gl_sa;
	gl_sa*=0.975;

//	printf("x:%4d y:%4d a:%4d\n",gl_x0,gl_y0,gl_a);

//	glRasterPos2i ( 0, 0 );

//	glBegin( GL_POINTS );
//	glColor3f( 1.0, 1.0, 1.0 );
//	glVertex2f( 0.0, 0.0 );
//	glEnd();
	if( frame % 64 == 0 )
	{
		fps_time        = clock() - prev_time;
		prev_time       = clock();
		sprintf(buf, "fps %3.2f", (double)((double)64.0 * (double)CLOCKS_PER_SEC)/((double)fps_time) );
	}
	auxDrawStr( buf );
	auxSwapBuffers();

	frame++;
}
void idle( void )
{
     main();
}

void dupe( void )
{
//      printf("%4.4x\n",glGetError());
}

void Init( void )
{
        AUX_RGBImageRec* img;
        //
        // Main loop
        //
        //   a     = angle
        //   x0,y0 = current position
        //   s     = speed constant
        //   ss    = current forward/backward speed
        //   sa    = angular speed
        //
        gl_a=0;
		gl_x0=0x8610000;
		gl_y0=0x3750000;
        gl_s=4096;
        gl_ss=10000.0;
        gl_sa=0;

        //
        // Compute the height map
        //
		LoadWorld();
//        ComputeMap();

        auxInitPosition( 0, 0, 640, 480 );
        auxInitDisplayMode( AUX_RGB | AUX_DOUBLE | AUX_DEPTH );
        auxInitWindow( "radii - terrain - dlannan@senet.com.au" );

        auxCreateFont( );
//        auxIdleFunc( main );	// Register with Zeus
		glEnable( GL_TEXTURE_2D );
//        glEnable( GL_ALPHA_TEST );
//        glEnable( GL_BLEND );
//        glDisable( GL_DEPTH_TEST );
//		glEnable( GL_FOG );
		
		img = auxDIBImageLoad( "water1.bmp" );
        glBindTexture(   GL_TEXTURE_2D, 1 ); dupe();
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);dupe();
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);dupe();
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );dupe();
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );dupe();
//        glTexImage2D(    GL_TEXTURE_2D, 0, 1, 256, 256, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, HMap );dupe();
		glTexImage2D(	 GL_TEXTURE_2D, 0, 3, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE,  img->data );dupe();
        /*
        glTexImage2D(    GL_TEXTURE_2D, 1, 1, 128, 128, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, HMap );dupe();
        glTexImage2D(    GL_TEXTURE_2D, 2, 1,  64,  64, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, HMap );dupe();
        glTexImage2D(    GL_TEXTURE_2D, 3, 1,  32,  32, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, HMap );dupe();
        glTexImage2D(    GL_TEXTURE_2D, 4, 1,  16,  16, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, HMap );dupe();
        glTexImage2D(    GL_TEXTURE_2D, 5, 1,   8,   8, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, HMap );dupe();
        glTexImage2D(    GL_TEXTURE_2D, 6, 1,   4,   4, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, HMap );dupe();
        glTexImage2D(    GL_TEXTURE_2D, 7, 1,   2,   2, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, HMap );dupe();
        glTexImage2D(    GL_TEXTURE_2D, 8, 1,   1,   1, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, HMap );dupe();
        glTexImage2D(    GL_TEXTURE_2D, 9, 1,   0,   0, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, HMap );dupe();
        */

//      glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);dupe();
//      glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);dupe();
//      glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );dupe();
//      glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );dupe();

//        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
//		glBlendFunc( GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_COLOR );

		glViewport(0,0,640,480);
		
		glEnable(GL_DEPTH_TEST);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glFrustum(0.0,640,480,0.0,0.0,100.0);
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
		glFrustum(0.0,640,480,0.0,0.0,100.0);

        auxMainLoop( idle );
        auxQuit();
}
