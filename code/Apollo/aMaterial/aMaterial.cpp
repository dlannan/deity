
// Make sure you include headers for modules you intend to use. They may
// have types or structures you may need to cast pointers to.

// Materials are reffered to from other modules with handle id's only.
// It should not be neccessary to return a material's pointer.

#include "aMaterial.h"
#include "..\\..\\System\\System.h"

ApolloClass	apollo;

namespace aMaterial
{
	map<int, TextureType *>		textures;	// Texture manager - texture name id crc
	map<int, MaterialType *>	materials;	// Material manager	- material unique id (time)

	IDirect3DDevice8		*g_pd3dDevice;	// DX8
	HGLRC					g_oglDevice;	// OGL

	void					*g_rDevice;			// Render device
	int						mode = API::NONE;	// Gives a meaningfull enum of current device type

	void LoadFontMaterial(Packet::PackClass *p);
	void SetAPIMode(Packet::PackClass *p);
	void SetMaterial(Packet::PackClass *p);

	void SetMaterialEmissive(Packet::PackClass *p);
	void SetMaterialSpecular(Packet::PackClass *p);
	void SetMaterialDiffuse(Packet::PackClass *p);
	void SetMaterialAmbient(Packet::PackClass *p);

	
	void PreInit()
	{
		// Without this there is no way to Init the module, since the recieve will fail.
		apollo.SubmitFunction(aMaterial::Init,"Init");
		apollo.SubmitFunction(aMaterial::Kill,"Kill");

		// Supports multiple device types - OGL, DX8, DX7(eventually)
		apollo.SubmitFunction(aMaterial::SetAPIMode,"SetAPIMode");	
		apollo.SubmitFunction(aMaterial::SetRenderDevice,"SetRenderDevice");	
		
		apollo.SubmitFunction(aMaterial::CreateMaterial,"CreateMaterial");
		apollo.SubmitFunction(aMaterial::RemoveMaterial,"RemoveMaterial");
		apollo.SubmitFunction(aMaterial::FindMaterial,"FindMaterial");
		apollo.SubmitFunction(aMaterial::SetMaterial,"SetMaterial");

		// Send a texture to texture manager
		apollo.SubmitFunction(aMaterial::LoadFontMaterial,"LoadFontMaterial"); 
		apollo.SubmitFunction(aMaterial::LoadTexture,"LoadTexture"); 
		// Remove a texture from texture manager
		apollo.SubmitFunction(aMaterial::UnloadTexture,"UnloadTexture");
		// Find a texture in the texture manager
		apollo.SubmitFunction(aMaterial::FindTexture,"FindTexture");

		// Add a texture to a material 
		apollo.SubmitFunction(aMaterial::AddTexture,"AddTexture");	 
		// Remove a texture from a material 
		apollo.SubmitFunction(aMaterial::RemoveTexture,"RemoveTexture");

		// Material functions
		apollo.SubmitFunction(aMaterial::SetProperties,"SetProperties");

		apollo.SubmitFunction(aMaterial::SetMaterialAmbient,"SetMaterialAmbient");
		apollo.SubmitFunction(aMaterial::SetMaterialDiffuse,"SetMaterialDiffuse");
		apollo.SubmitFunction(aMaterial::SetMaterialSpecular,"SetMaterialSpecular");
		apollo.SubmitFunction(aMaterial::SetMaterialEmissive,"SetMaterialEmissive");
	}

	// Functions - always after submit - since the header ones are the ones its looking for
	void SetRenderDevice(LPDIRECT3DDEVICE8 dev);
	unsigned int AddMaterial(Data::StringClass filename);
	void SetMaterial(int matno);

	void SetAPIMode(Packet::PackClass *p)
	{
		mode = p->GetData(D_INT);
	}
	
	// Use this to submit your calls - external submission not available yet
	void Init(Packet::PackClass *p)
	{
		g_pd3dDevice = NULL;
		materials.clear();
		textures.clear();

		Packet("Material", "Render.GetRenderDevice");	// Hrm not good!
		Psend;

		SetRenderDevice(&packet);

		DebugLog("Material Init Complete \n");
	}

	void SetRenderDevice(Packet::PackClass *p)
	{
		switch(mode)
		{
		case API::DX8: 
			g_pd3dDevice = (LPDIRECT3DDEVICE8)p->GetData(D_LONG);
			g_rDevice = (void *)g_pd3dDevice;
			break;
		case API::OGL: 
			g_oglDevice = (HGLRC)p->GetData(D_LONG); 
			g_rDevice = (void *)g_oglDevice;
			break;
		}
	}

	void SetMaterialDX8(int matno)
	{
		ASSERT( materials.find(matno) == materials.end(), "Invalid Material ID" )

		g_pd3dDevice->SetMaterial(materials[matno]->dxmat);
		// Dont set if there isnt one!
		if(!materials[matno]->tex.empty())
		{
			g_pd3dDevice->SetTexture( 0, textures[materials[matno]->tex[0]]->dxtex->dxtex );
		}
	}

	void SetMaterialOGL(int matno)
	{
		ASSERT( materials.find(matno) == materials.end(), "Invalid Material ID" )
		// Whole heap of ogl calls to set material properties.
	}

	void SetMaterial(Packet::PackClass *p)
	{
		switch(mode)
		{
		case API::DX8:
			SetMaterialDX8(p->GetData(D_INT));
			break;
		case API::OGL:
			SetMaterialOGL(p->GetData(D_INT));
			break;
		}
	}

	void SetDefaultValues(int index)
	{
		Packet::PackClass packet;
		
		packet += (int) index;
		packet += (float) 1.0f;
		packet += (float) 1.0f;
		packet += (float) 1.0f;
		packet += (float) 1.0f;

		SetMaterialAmbient(&packet);
		SetMaterialDiffuse(&packet);
	}


	// Returns a material handle
	int CreateMaterial(void)
	{
		MaterialType *mat = new aMaterial::MaterialType;
		switch(mode)
		{
		case API::DX8:
			{
			mat->dxmat = new D3DMATERIAL8;
			break;
			}
		case API::OGL:
			{
			mat->oglmat = new GENMATERIAL;
			break;
			}
		}

		unsigned int val = CRC::Int(materials.size()+1);

		// Add a material to material manager
		materials[val] = mat;
		
		SetDefaultValues(val);
		return val;
	}

	// Returns a material handle
	void RemoveMaterial(int handle)
	{
		ASSERT( materials.find(handle) == materials.end(), "Invalid Material ID" )

		switch(mode)
		{
		case API::DX8:
			delete materials[handle]->dxmat;
			delete materials[handle];
			break;
		case API::OGL:
			delete materials[handle]->oglmat;
			delete materials[handle];
			break;
		}
	}

	void RemoveMaterial(Packet::PackClass *p)
	{
		static Packet::PackClass	packet;

		if(g_rDevice)
		{
			RemoveMaterial(p->GetData(D_INT));
		}
		else
		{
			packet.Clear();
			packet.destcrc << "Render.GetRenderDevice";
			packet.srccrc << "Material.SetRenderDevice";
			apollo.sendpacketfunc.proc(&packet);

			apollo.sendpacketfunc.proc(p);
		}
	}

	void CreateMaterial(Packet::PackClass *p)
	{
		if(g_rDevice)
		{
			int matid;
			matid = CreateMaterial();

			p->Clear();
			(*p) += (int)matid;
		}
		else
		{
			{
			Packet("Material.SetRenderDevice", "Render.GetRenderDevice");
			Psend;
			}

			apollo.sendpacketfunc.proc(p);
		}
	}

	void SetMaterialAmbient(Packet::PackClass *p)
	{
		int matid = p->GetData(D_INT);
		ASSERT( materials.find(matid) == materials.end(), "Invalid Material ID" )

		switch(mode)
		{
		case API::DX8:
			materials[matid]->dxmat->Ambient.r = p->GetData(D_FLOAT);
			materials[matid]->dxmat->Ambient.g = p->GetData(D_FLOAT);
			materials[matid]->dxmat->Ambient.b = p->GetData(D_FLOAT);
			materials[matid]->dxmat->Ambient.a = p->GetData(D_FLOAT);
			break;
		case API::OGL:
			break;
		}
	}

	void SetMaterialDiffuse(Packet::PackClass *p)
	{
		int matid = p->GetData(D_INT);
		ASSERT( materials.find(matid) == materials.end(), "Invalid Material ID" )

		switch(mode)
		{
		case API::DX8:
			materials[matid]->dxmat->Diffuse.r = p->GetData(D_FLOAT);
			materials[matid]->dxmat->Diffuse.g = p->GetData(D_FLOAT);
			materials[matid]->dxmat->Diffuse.b = p->GetData(D_FLOAT);
			materials[matid]->dxmat->Diffuse.a = p->GetData(D_FLOAT);
			break;
		case API::OGL:
			break;
		}
	}

	void SetMaterialSpecular(Packet::PackClass *p)
	{
		int matid = p->GetData(D_INT);
		ASSERT( materials.find(matid) == materials.end(), "Invalid Material ID" )

		switch(mode)
		{
		case API::DX8:
			materials[matid]->dxmat->Specular.r = p->GetData(D_FLOAT);
			materials[matid]->dxmat->Specular.g = p->GetData(D_FLOAT);
			materials[matid]->dxmat->Specular.b = p->GetData(D_FLOAT);
			materials[matid]->dxmat->Specular.a = p->GetData(D_FLOAT);
			break;
		case API::OGL:
			break;
		}
	}

	void SetMaterialEmissive(Packet::PackClass *p)
	{
		int matid = p->GetData(D_INT);
		ASSERT( materials.find(matid) == materials.end(), "Invalid Material ID" )

		switch(mode)
		{
		case API::DX8:
			materials[matid]->dxmat->Emissive.r = p->GetData(D_FLOAT);
			materials[matid]->dxmat->Emissive.g = p->GetData(D_FLOAT);
			materials[matid]->dxmat->Emissive.b = p->GetData(D_FLOAT);
			materials[matid]->dxmat->Emissive.a = p->GetData(D_FLOAT);
			break;
		case API::OGL:
			break;
		}
	}

	// Returns a material handle
	void RemoveTexture(int handle)
	{
		// Dont try this without valid handles!
		ASSERT( textures.find(handle) == textures.end(), "Invalid Texture ID" )

		switch(mode)
		{
		case API::DX8:
//			if(textures[handle]->dxtex->dxtex) delete textures[handle]->dxtex->dxtex;
			if(textures[handle]->dxtex) delete textures[handle]->dxtex;
			if(textures[handle]) delete textures[handle];
			break;
		case API::OGL:
			if(textures[handle]->ogltex->texdata) delete textures[handle]->ogltex->texdata;
			if(textures[handle]->ogltex->paldata) delete textures[handle]->ogltex->paldata;
			if(textures[handle]->ogltex) delete textures[handle]->ogltex;
			if(textures[handle]) delete textures[handle];
			break;
		}
	}

	void SetProperties(Packet::PackClass *p)
	{
	}

	void FindMaterial(Packet::PackClass *p)
	{
	}

	// Easy one first :-)
	int AddRAWTexture(const char *fname, Data::BlockType &d, int w, int h)
	{
		int error = -1;
		unsigned int	*raw = new unsigned int[w * h];
		unsigned int	*rawptr = raw;

		switch(mode)
		{
		case API::DX8:
			{
				DXTEXTURE	*newtex = new DXTEXTURE;

				// Copy the data into a temp raw buffer
				memcpy(rawptr, d.GetData(), d.GetMax());
				rawptr += d.GetMax() / sizeof(unsigned int);

				rawptr = raw;

				D3DLOCKED_RECT	rawDesc;
				rawDesc.Pitch = 1024;
				rawDesc.pBits = NULL;

				// TODO - ERROR CHECK!!
				error = D3DXCreateTexture(g_pd3dDevice,w,h,1,0,D3DFMT_A8R8G8B8,D3DPOOL_MANAGED,&newtex->dxtex);
				ASSERT(error!=D3D_OK, "Ahhh for fuks sake..");

				newtex->dxtex->LockRect(0,&rawDesc,NULL,D3DLOCK_NO_DIRTY_UPDATE);
				// Iterate through each rgb,setting texels!! heheeh
				memcpy(rawDesc.pBits,rawptr,w*h*sizeof(unsigned int));

//				for(i = 0; i< (w * h); i++)
//				{
//					((unsigned int *)rawDesc.pBits)[i] = rawptr[i] | 0xFF000000;
//				}

				newtex->dxtex->UnlockRect(0);

				TextureType *newtextype	= new TextureType;

				newtextype->dxtex = newtex;
				newtextype->texfilename << fname;
				
				CRC::StringClass sid((char*) fname);

				// Add it to the texture list and return a valid id
				textures[sid.Id()]=newtextype;
				error = sid.Id();
			}
			break;
		case API::OGL:
			break;
		}

		if(raw) delete raw;
		return error;
	}

	int AddPPMTexture(const char *fname, Data::BlockType &d)
	{
		int error = -1;
		unsigned int	*raw = NULL; 
		unsigned int	*rawptr = NULL; 

		switch(mode)
		{
		case API::DX8:
			{
				char buff[32];
				char temp[512];
				int	width, height, range;
				int index = 0;

				DXTEXTURE	*newtex = new DXTEXTURE;

				char *header = (char *)d.GetData();
				sscanf( header, "%s\n%d %d\n%d\n", buff, &width, &height, &range );

				sprintf( temp, "%s\n%d %d\n%d\n", buff, width, height, range );
				raw = new unsigned int[width * height];
				index += strlen(temp) + 1;

				// Copy the data into a temp raw buffer
				memcpy(rawptr, d.GetData(index), width * height * sizeof(unsigned int));
				rawptr = raw;

				D3DLOCKED_RECT	rawDesc;
				rawDesc.Pitch = 1024;
				rawDesc.pBits = NULL;

				// TODO - ERROR CHECK!!
				error = D3DXCreateTexture(g_pd3dDevice,width,height,1,0,D3DFMT_A8R8G8B8,D3DPOOL_MANAGED,&newtex->dxtex);
				ASSERT(error!=D3D_OK, "Ahhh for fuks sake..");

				newtex->dxtex->LockRect(0,&rawDesc,NULL,D3DLOCK_NO_DIRTY_UPDATE);
				// Iterate through each rgb,setting texels!! heheeh
				memcpy(rawDesc.pBits,rawptr,width*height*sizeof(unsigned int));

//				for(i = 0; i< (w * h); i++)
//				{
//					((unsigned int *)rawDesc.pBits)[i] = rawptr[i] | 0xFF000000;
//				}

				newtex->dxtex->UnlockRect(0);

				TextureType *newtextype	= new TextureType;

				newtextype->dxtex = newtex;
				newtextype->texfilename << fname;

				CRC::StringClass sid((char*) fname);

				// Add it to the texture list and return a valid id
				textures[sid.Id()]=newtextype;
				error = sid.Id();
			}
			break;
		case API::OGL:
			break;
		}

		if(raw) delete raw;
		return error;
	}

	int AddTGATexture(const char *fname, Data::BlockType &d)
	{
		int error = -1;
		unsigned int	*raw = NULL;
		unsigned int	*rawptr = NULL;

		switch(mode)
		{
		case API::DX8:
			{
				// 32bit TGAs first - indexed ones later
				DXTEXTURE	*newtex = new DXTEXTURE;

				TGAHeader	header;
				memcpy(&header,d.GetData(), sizeof(TGAHeader));
				unsigned int index = sizeof(TGAHeader);

				// Indexed colour palette - treat a little differently
				if(header.imageTypeCode == 0x01) 
				{
					raw = new unsigned int[header.image.width * header.image.height];
					rawptr = raw;

					unsigned int *colortable = new unsigned int[header.cmap.length];
					unsigned char *tmp = new unsigned char[header.cmap.entrySize];
					for(int cc = 0; cc<header.cmap.length; cc++)
					{
						for(int cct = (header.cmap.entrySize)-1; cct>=0; cct--)
						{
							tmp[cct] = *d.GetData(index++);
						}
						colortable[cc] = *(unsigned int*)tmp;
					}
					delete [] tmp;

					memcpy(colortable,d.GetData(index),header.cmap.entrySize * header.cmap.length);
					index += header.cmap.entrySize * header.cmap.length;

					if( (header.image.pixelSize >> 3) > 0 )
					{
						unsigned char *tmp = new unsigned char[header.image.pixelSize >> 3];

						// Copy the data into a temp raw buffer
						for(int y = 0; y<header.image.height; y++)
						{
							for(int x = 0; x<header.image.height; x++)
							{
								*rawptr = colortable[ *(unsigned int*)d.GetData(index) ];
								index += (header.cmap.entrySize >> 3);
								rawptr++;
							}
						}
						delete [] tmp;
						delete [] colortable;
					}
					else
					{
						return error;
					}
				}
				// 16, 24, 32bit rgb - 16 will prolly look odd - not handled correctly yet
				else if(header.imageTypeCode == 0x02) 
				{
					raw = new unsigned int[header.image.width * header.image.height];
					rawptr = raw;

					if( (header.image.pixelSize >> 3) > 0 )
					{
						unsigned char *tmp = new unsigned char[header.image.pixelSize >> 3];

						// Copy the data into a temp raw buffer
						for(int y = 0; y<header.image.height; y++)
						{
							for(int x = 0; x<header.image.height; x++)
							{
								for(int c = (header.image.pixelSize >> 3)-1; c>=0; c--)
								{
									tmp[c] = *d.GetData(index++);
								}
								*rawptr = *(unsigned int *)tmp;
								rawptr++;
							}
						}
						delete [] tmp;
					}
					else return error;
				}
				else return error;

				rawptr = raw;

				D3DLOCKED_RECT	rawDesc;
				rawDesc.Pitch = 1024;
				rawDesc.pBits = NULL;

				// TODO - ERROR CHECK!!
				error = D3DXCreateTexture(g_pd3dDevice,header.image.width,header.image.height,1,0,D3DFMT_A8R8G8B8,D3DPOOL_MANAGED,&newtex->dxtex);
				ASSERT(error!=D3D_OK, "Ahhh for fuks sake..");

				newtex->dxtex->LockRect(0,&rawDesc,NULL,D3DLOCK_NO_DIRTY_UPDATE);
				// Iterate through each rgb,setting texels!! heheeh
				memcpy(rawDesc.pBits,rawptr,header.image.width*header.image.height*sizeof(unsigned int));
				newtex->dxtex->UnlockRect(0);

				TextureType *newtextype	= new TextureType;

				newtextype->dxtex = newtex;
				newtextype->texfilename << fname;

				CRC::StringClass sid((char*) fname);

				// Add it to the texture list and return a valid id
				textures[sid.Id()]=newtextype;
				error = sid.Id();
			}
			break;
		case API::OGL:
			break;
		}

		if(raw) delete raw;
		return error;
	}

	int AddBMPTexture(const char *fname, Data::BlockType &d)
	{
		switch(mode)
		{
		case API::DX8:
			{
				DXTEXTURE	*newtex = new DXTEXTURE;
				// Bypasses the file loading we did
				if( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, fname, &newtex->dxtex ) ) )
				{
					delete newtex;
					// Clear the texture if you couldnt find it
					DebugLog("Material Load Fail:");
					DebugLog(fname);
					DebugLog("\n");
					return -1;	// Error
				}
				TextureType *newtextype	= new TextureType;

				newtextype->dxtex = newtex;
				newtextype->texfilename << fname;

				CRC::StringClass sid((char*) fname);

				// Add it to the texture list and return a valid id
				textures[sid.Id()]=newtextype;
				return sid.Id();
			}
			break;
		case API::OGL:
			// TODO:
			WARNING("OGL Bmp Textures not supported yet.");
			return -1;
			break;
		}

		return -1;
	}
	
	// Fonts contain embedded data, so its a little different
	void LoadFontMaterial(Packet::PackClass *p)
	{
		int error = -1;

		// Grab filename first (or font name)
		Data::StringClass	fname;
		// Grab the BlockType ptr of the RAW data
		Data::BlockType		*tex;

		fname = p->GetData(D_STRING);
		tex = (Data::BlockType	*)p->GetData(D_ULONG);

		// Default material created for each font
		int matid = CreateMaterial();
		error = AddRAWTexture(fname.String(),*tex,512,512);

		materials[matid]->tex.push_back(error);

		p->Clear();
		(*p) += (int)matid;
	}

	void LoadTexture(Packet::PackClass *p)
	{
		Data::BlockType	tex;
		File::FileClass	ftex;

		Data::StringClass	fname;
		fname = p->GetData(D_STRING);

		int error = 0;

		// If the texture is already assigned somewhere
		CRC::StringClass	sname((char *)fname.String());
		if(textures.find(sname.Id())!=textures.end())
		{
			error = sname.Id();
		}
		else
		{
			error = ftex.Open(fname.String(),FILE_READ | FILE_BINARY);
			ASSERT( (ftex.handle <= 0), "aMaterial::LoadTexture" );

			PRINTF("Loading.. %s",fname.String());

			ftex >> &tex;
			ftex.Close();

			DebugLog("Texture Loaded:");
			DebugLog(fname.String());
			DebugLog("\n");

			// All the texture data is in the BlockType - process it depending on file extension
			Data::StringClass   extS;
			CRC::StringClass	ext;
			extS = &((char *)fname.String())[fname.Size()-3];
			ext << extS;

			DebugLog("Texture extension:");
			DebugLog(ext.String());
			DebugLog("\n");

			error = -1;			// Already an error

			switch(ext.Id())
			{
			case 0xbc0e93dd: // tga
				error = AddTGATexture(fname.String(),tex);
				break;
			case 0xb732f3bc: // ppm
				error = AddPPMTexture(fname.String(),tex);
				break;
			case 0x34fa8467: // bmp
				error = AddBMPTexture(fname.String(),tex);
				break;
			case 0x1a0dfdb8: // raw
				{
				// For RAW format only - needs a width and height setting
				int w, h;

				w = p->GetData( D_INT );
				h = p->GetData( D_INT );

				error = AddRAWTexture(fname.String(),tex,w,h);
				}
				break;
			}
		}

		p->Clear();
		(*p) += (int)error;
	}

	void UnloadTexture(Packet::PackClass *p)
	{
	}

	void FindTexture(Packet::PackClass *p)
	{
	}

	void AddTexture(Packet::PackClass *p)
	{
		int texid = p->GetData(D_INT);
		int matid = p->GetData(D_INT);

		ASSERT( materials.find(matid) == materials.end(), "Invalid Material ID" )
		ASSERT( textures.find(texid) == textures.end(), "Invalid Texture ID");
		materials[matid]->tex.push_back(texid);
	}

	void RemoveTexture(Packet::PackClass *p)
	{
	}

	// Use this to cleanup snything nasty at the end of the module runtime
	void Kill(Packet::PackClass *p)
	{
		map<int, MaterialType *>::iterator i=materials.begin();
		while(i!= materials.end())
		{
			RemoveMaterial(i->first);
			i++;
		}

		map<int, TextureType *>::iterator j=textures.begin();
		while(j!= textures.end())
		{
			RemoveTexture(j->first);
			j++;
		}

		materials.clear();
		textures.clear();
	}
	
//		if( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, filename.String(), &mat->dxtex ) ) )
//		{
			// Clear the texture if you couldnt find it
			//	zeus.debug << "Material Load Fail:";
			//	zeus.debug << filename + "\n";
//		}

	// Submission functions
/*	
	void	CreateMaterial(Packet::PackClass *p);
	void	RemoveMaterial(Packet::PackClass *p);
	void	FindMaterial(Packet::PackClass *p);

	void	SetProperties(Packet::PackClass *p);

	void	LoadTexture(Packet::PackClass *p);
	void	UnloadTexture(Packet::PackClass *p);
	void	FindTexture(Packet::PackClass *p);
	
	void	AddTexture(Packet::PackClass *p);
	void	RemoveTexture(Packet::PackClass *p);
*/

}
