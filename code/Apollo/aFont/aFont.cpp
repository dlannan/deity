
// Make sure you include headers for modules you intend to use. They may
// have types or structures you may need to cast pointers to.

// Depends on Material - load Material first to make life easier :-)

#include "..\\..\\System\\System.h"
#include "aFont.h"
#include "FontType.h"

ApolloClass	apollo;

namespace aFont
{
	int	fontMatId = 0;
	int	fontMeshId = -1;

	map<int, FontType *>	FontList;

	void Load(Packet::PackClass *p);
	void Unload(Packet::PackClass *p);
	void Find(Packet::PackClass *p);

	void Set(Packet::PackClass *p);

	void RenderChars(Packet::PackClass *p);
	void RenderLine(Packet::PackClass *p);
	void RenderPage(Packet::PackClass *p);

	void PreInit()
	{
		// Without this there is no way to Init the module, since the recieve will fail.
		apollo.SubmitFunction(aFont::Init,"Init");
		apollo.SubmitFunction(aFont::Kill,"Kill");

		apollo.SubmitFunction(aFont::Load,"Load");
		apollo.SubmitFunction(aFont::Unload,"Unload");
		apollo.SubmitFunction(aFont::Find,"Find");

		apollo.SubmitFunction(aFont::Set,"Set");

		// Has no CR or LF - string to current window
		apollo.SubmitFunction(aFont::RenderChars,"RenderChars");
		// Has a CR or LF - string to current window
		apollo.SubmitFunction(aFont::RenderLine,"RenderLine");

		// Entire page output to current window
		apollo.SubmitFunction(aFont::RenderPage,"RenderPage");
	}

	// Use this to submit your calls - external submission not available yet
	void Init(Packet::PackClass *p)
	{
		{
		Packet("Font","Render.PrepareFontMesh");
		Padd (int)fontMatId;					// associated material index
		Psend;

		// Grab the mesh handle
		fontMeshId = packet.GetData(D_INT);
		}
	}

	// Use this to cleanup snything nasty at the end of the module runtime
	void Kill(Packet::PackClass *p)
	{
		// Remove all the fonts
		map<int, FontType *>::iterator i = FontList.begin();
		while(i!=FontList.end())
		{
			delete i->second;
			i++;
		}

		FontList.clear();
	}

	void Load(Packet::PackClass *p)
	{
		Data::StringClass fname;
		Data::StringClass fpath;

		// Name to identify the font
		fname = p->GetData(D_STRING);
		// Full path name
		fpath = p->GetData(D_STRING);
		CRC::StringClass sid((char *)fname.String());

		// Check to see if its already loaded?
		if(FontList.find((int)sid.Id())==FontList.end())
		{

			// Sending it elsewhere for use.. must be accessible
			Data::BlockType		font;
			File::FileClass		fontfile;

			fontfile.Open(fpath.String(), FILE_READ | FILE_BINARY);
			fontfile >> &font;
			fontfile.Close();

			FontType	*newfont = new FontType;

			newfont->name = fname.String();
			newfont->path = fpath.String();
		
			// Get the font header - list of char widths
			memcpy(newfont->widths,font.GetData(),1024);
			newfont->size = newfont->widths[0];
			newfont->Bcolour = 0xFF000000;
			newfont->Fcolour = 0xFFFFFFFF;
			
			static Data::BlockType	rawfont;
			rawfont.AddData(font.GetData(1024),font.GetUsed()-1024);

			{
			Packet("Font","Material.LoadFontMaterial");
			Padd fpath.String();
			Padd (unsigned long)&rawfont;
			Psend;

			newfont->matid = packet.GetData(D_INT);
			}

			FontList[sid.Id()] = newfont;
		}

		p->Clear();
		(*p) += (int)sid.Id();
	}

	void Unload(Packet::PackClass *p)
	{
	}

	// Look for a particular font
	void Find(Packet::PackClass *p)
	{
	}

	void Set(Packet::PackClass *p)
	{
	}

	void RenderChars(Packet::PackClass *p)
	{
		ASSERT( FontList.size() <= 0, "No Fonts Loaded Butt Head!");
		Data::StringClass	str;
		str << p->GetData(D_STRING);

		int fontid = p->GetData(D_INT);

		ASSERT( FontList.find(fontid) == FontList.end(), "Invalid Font index");

		float x = p->GetData( D_FLOAT );
		float y = p->GetData( D_FLOAT );
		float z = p->GetData( D_FLOAT );

		float sz = p->GetData( D_FLOAT );

		{
		Packet("Font","Render.SetMeshMaterial");
		Padd (int)(fontMeshId);					// Same mesh as always
		Padd (int)(FontList[fontid]->matid);	// Possibly Different material (font)
		Psend;
		}

		float sy = FontList[fontid]->size * sz;

		float xp = x;
		float yp = y;
		float zp = z;

		float u0,u1,v0,v1;

		for (int chars = 0; chars < str.Size(); chars++)
		{
			float sx = FontList[fontid]->widths[str.Char(chars)] * sz;
			u0 = 0.0625f * (str.Char(chars) % 16);
			u1 = u0 + 0.001953125f * FontList[fontid]->widths[str.Char(chars)];

			v0 = 0.0625f * (int)(str.Char(chars) / 16);
			v1 = v0 + 0.001953125f * FontList[fontid]->size;

			{
			Packet("Font","Render.PrimFontChar");	// Textured rectangle
			// Mesh id
			Padd (int) fontMeshId;
			// position
			Padd (float) xp;
			Padd (float) yp;
			Padd (float) zp;
			// Size
			Padd (float) sx;
			Padd (float) sy;
			// UVs
			Padd (float) u0;
			Padd (float) v1;
			Padd (float) u1;
			Padd (float) v0;

			Padd (unsigned int)FontList[fontid]->Fcolour;

			Psend

			xp += sx;
			}
		}
	}

	void RenderLine(Packet::PackClass *p)
	{
	}

	void RenderPage(Packet::PackClass *p)
	{
	}

}
