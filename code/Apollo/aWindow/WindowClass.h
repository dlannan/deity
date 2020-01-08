// WindowClass

#ifndef _WINDOWCLASS_H_
#define _WINDOWCLASS_H_

#include "..\\..\\System\\System.h"

// Should label as Window module - thats what it is.

namespace aWindow
{
	class WindowClass
	{
	private:
		unsigned int		parent;			// Null parent is Top window
		unsigned int		child;			// So we can have chained windows

		unsigned int		windowHandle;	// This handles index
		Data::StringClass	windowName;

		int					windowMat;		// This is the material id for the window
		
		unsigned int		windowFGcolour;
		unsigned int		windowBGcolour;

		unsigned int		windowEXstyle;
		unsigned int		windowStyle;

		// Text formatting and storage
		Data::PageType		windowText;			// All text for the page goes here
		unsigned int		windowFont;			// Font index
		unsigned int		windowTextAlign;	// left, right, center

		unsigned int		windowScrollPos;	// Position of text scrolling

	public:
		int					windowZDepth;		// Zbuffer depth
		int					windowCDepth;		// Colour buffer depth (should be same as render context)

		float				windowHeight;
		float				windowWidth;

		Vector4				windowPos;
		Vector4				moveTo;
		Vector4				moveFrom;

		unsigned int		windowExit:1,
							windowVisible:1;

		CRC::TimeClass		GID;			// Window unique identifier

		WindowClass(float x, float y, float z, int width, int height)
		{
			// Default window settings
			windowPos.Set(x, y, z, 0.0f);
			moveFrom.Set(windowPos.x,windowPos.y,windowPos.z,0.0f);
			moveTo.Set(windowPos.x,windowPos.y,windowPos.z,0.0f);

			windowWidth=width;	
			windowHeight=height;

			windowName << "Default Window Name";
			windowExit=false;

			windowStyle = 0;		// Have our own styles - min, max, etc
			windowEXstyle = 0;		// Extended ones - similar to Windows - top, bottom, etc

			windowScrollPos = 0;	// A zero pos will not draw a line of text

			windowBGcolour = 0xFFFFFFFF;
			windowFGcolour = 0xFFFFFFFF;
		}

		~WindowClass()
		{
		}

		unsigned int GetWindowHandle()
		{
			return windowHandle;
		}

		void SetWindowHandle(unsigned int handle)
		{
			windowHandle = handle;
		}

		void SetWindowMaterial(int mat)
		{
			windowMat = mat;
		}

		void SetWindowName(const char *name)
		{
			windowName.Clear();
			windowName << name;
			Render();				// This redraws the window
		}

		void SetWindowName(char *name)
		{
			windowName.Clear();
			windowName << name;
			Render();				// This redraws the window
		}

		const char *GetWindowName()
		{
			return windowName.String();
		}

		void SetWindowStyles(unsigned int exstyle, unsigned int style)
		{
			windowEXstyle=exstyle;
			windowStyle=style;
		}

		unsigned int GetWindowStyle()
		{
			return windowStyle;
		}

		unsigned int GetWindowEXstyle()
		{
			return windowEXstyle;
		}

		unsigned int GetWindowFont()
		{
			return windowFont;
		}

		unsigned int GetWindowTextAlign()
		{
			return windowTextAlign;
		}

		Data::PageType *GetWindowText()
		{
			return &windowText;
		}

		void SetWindowFont(unsigned int font)
		{
			windowFont = font;
		}

		void SetWindowTextAlign(unsigned int align)
		{
			windowTextAlign = align;
		}

		void AddWindowText(const char *text)
		{
			Data::StringClass	temp;
			temp << text;
			windowText.push_back(temp);
		}

		void SetWindowColours(unsigned int fgcolour, unsigned int bgcolour)
		{
			windowFGcolour = fgcolour;
			windowBGcolour = bgcolour;
		}

		void Move(int x, int y)
		{
			windowPos.x = (float)x;
			windowPos.y = (float)y;
			// Update Window drawing
		}

		void Move(float x, float y)
		{
			windowPos.x = x;
			windowPos.y = y;
			// Update Window drawing
		}

		void Resize(int width, int height)
		{
			windowWidth = (float)width; 
			windowHeight = (float)height;
			// Update Window drawing
		}

		void Resize(float width, float height)
		{
			windowWidth = width; 
			windowHeight = height;
			// Update Window drawing
		}

		bool WindowVisible()
		{
			return windowVisible;
		}

		void SetWindowExit(bool exit)
		{
			windowExit=exit;
		}

		unsigned int FGcolour()
		{
			return windowFGcolour;
		}

		unsigned int BGcolour()
		{
			return windowBGcolour;
		}

		void Render()
		{
			RenderRect();
//			RenderText();
		}

		void RenderRect()
		{
			// Window backplane
			{
				Packet("Window.Render", "Render.PrimRectangle");
				Padd windowMat;
				// x,y,z
				Padd (float) windowPos.x; Padd (float) windowPos.y; Padd (float) windowPos.z;
				// size x, size y
				Padd (float) windowWidth; Padd (float) windowHeight;
				// uv Topleft and uv Bottomright
				Padd 0.0f; Padd 0.0f; Padd 1.0f; Padd 1.0f;
				// r,g,b,a
				Padd (unsigned int) BGcolour();	// Set colour
				Psend;
			}

			// Window border
			{
				Packet("Window.Render", "Render.PrimRectangle");
				Padd windowMat;
				// x,y,z
				Padd (float) windowPos.x; Padd (float) windowPos.y; Padd (float) windowPos.z;
				// size x, size y
				Padd (float) windowWidth; Padd (float) windowHeight;
				// uv Topleft and uv Bottomright
				Padd 0.0f; Padd 0.0f; Padd 1.0f; Padd 1.0f;
				// r,g,b,a
				Padd (unsigned int) FGcolour();	// Set colour
				Psend;
			}
		}

		void RenderText()
		{
			// Set the window clipping rect
			{
			Packet("Window.RenderText", " Render.SetClipRect")
			Padd (float) windowPos.x;
			Padd (float) windowPos.y;
			Padd (float) (windowPos.x + windowWidth);
			Padd (float) (windowPos.y + windowHeight);
			Psend;
			}

			// Render Text from scroll pos backwards
			//		- At lowest scroll pos, the first line is only viewable
			for(int i = windowScrollPos; i>0; i--)
			{
				{
				Packet("Window.RenderText", "Font.RenderLine");
				Padd windowText[i].String();

				Psend
				}

			}
		}

	};	// End class
}

#endif