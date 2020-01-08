// TP_cob_class.cpp: implementation of the TP_cob_class class.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "..\\..\\..\\System\\System.h"

#include "Loader.h"
#include "..\\MeshClass.h"

namespace aMmCOB 
{
	// We map parent id to a map of index<->our index 
	typedef struct
	{
		map<int,int>	matmap;
	} MatMap;

	map<int, material_type *> ourMatMap;
	map<int, MatMap*> pidMatMap;

	void print_chunk(COBChunkHeaderType * chunk)
	{
		//	Test chunk header
		PRINTF("%c%c%c%c V%d.%d Id %ld Parent %ld Size %ld\n",
			chunk->ch_type[0],
			chunk->ch_type[1],
			chunk->ch_type[2],
			chunk->ch_type[3],
			chunk->ch_majorver,
			chunk->ch_minorver,
			chunk->ch_chunkid,
			chunk->ch_parentid,
			chunk->ch_length);
	}

	MeshGroup *add_group_chunk(MeshType *mesh, long gid)
	{
		return mesh->NewGroup(gid);
	}

	void sort_group_chunk(MeshGroup *group, unsigned char * data, long length)
	{
		unsigned long		index=0;

		typedef	float			Matrix43[12];
		Matrix43				tm;
		Matrix					m;
		COBNameType				name;
		COBLocalAxesType		localaxes;

		memcpy(&name.n_dcount,&data[index],sizeof(name.n_dcount));
		index+=2;
		memcpy(&name.n_string.s_length,&data[index],sizeof(name.n_string.s_length));
		index+=2;

		name.n_string.s_string=new char[name.n_string.s_length+1];
		memcpy(name.n_string.s_string,&data[index],name.n_string.s_length);
		index+=name.n_string.s_length;

		name.n_string.s_string[name.n_string.s_length]='\0';
		group->SetLabel(name.n_string.s_string);

	//	PRINTF("%s\n",name.n_string.s_string);

		memcpy(&localaxes,&data[index],sizeof(localaxes));
		index+=sizeof(localaxes);

		group->matrix.pos.x = localaxes.la_center.x;
		group->matrix.pos.y = localaxes.la_center.y;
		group->matrix.pos.z = localaxes.la_center.z;
		group->matrix.pos.w = 1.0f;

		group->matrix.right.x = localaxes.la_xaxis.x;
		group->matrix.right.y = localaxes.la_xaxis.y;
		group->matrix.right.z = localaxes.la_xaxis.z;
		group->matrix.right.w = 0.0f;

		group->matrix.up.x = localaxes.la_yaxis.x;
		group->matrix.up.y = localaxes.la_yaxis.y;
		group->matrix.up.z = localaxes.la_yaxis.z;
		group->matrix.up.w = 0.0f;

		group->matrix.view.x = localaxes.la_zaxis.x;
		group->matrix.view.y = localaxes.la_zaxis.y;
		group->matrix.view.z = localaxes.la_zaxis.z;
		group->matrix.view.w = 0.0f;

	//******	memcpy(&(glist[nog-1].orient[0][0]),&m,sizeof(Matrix));

		memcpy(&tm[0],&data[index],sizeof(Matrix43));
		index+=sizeof(Matrix43);

		group->trans.right.x = tm[0];
		group->trans.right.y = tm[1];
		group->trans.right.z = tm[2];
		group->trans.right.w = 0.0f;

		group->trans.up.x = tm[4];
		group->trans.up.y = tm[5];
		group->trans.up.z = tm[6];
		group->trans.up.w = 0.0f;

		group->trans.view.x = tm[8];
		group->trans.view.y = tm[9];
		group->trans.view.z = tm[10];
		group->trans.view.w = 0.0f;

		group->trans.pos.x = tm[3];
		group->trans.pos.y = tm[7];
		group->trans.pos.z = tm[11];
		group->trans.pos.w = 1.0f;

	// ******	memcpy(&(glist[nog-1].trans[0][0]),&m,sizeof(Matrix));
	}

	void apply_group_chunk(MeshType *mesh, long gid)
	{
	//	PRINTF("%ld\n",gid);

		MeshGroup *group = mesh->FindGroup(gid);

		// Valid group id - apply group properties to the facets
		if(group != NULL)
		{
			// Apply orient, and position of group to the facets
		}

	//	memcpy(&model->trans[0][0],&glist[id].trans[0][0],sizeof(MatrixType));
	//	memcpy(&model->orient[0][0],&glist[id].orient[0][0],sizeof(MatrixType));
	//	memcpy(&model->move,&glist[id].mv,sizeof(VertexType));

	//	PRINTF("ApplyGroupChunk complete.\n");
	}

	void sort_poly_chunk(unsigned char * data, long length,
									   FacetObject *mesh)
	{
		unsigned long		index=0;
		long   	i,j;		// iterative variables

		float					tm[12];

		COBNameType				name;
		COBLocalAxesType		localaxes;
		COBLocalVertexType		localv;
		COBUVVertexListType		localuv;

		long					faces=0;
		float					temp=0.0f;

		unsigned char			flags=0;
		short					vertinface=0;
		short					matinface=0;

		COBIndexType			face;
		COBCoordType			*coordptr=NULL;

		Vector2					*uvptr=NULL;

		// Our data stepping index
		index=0;

		// String name of the poly chunk
		memcpy(&name.n_dcount,&data[index],sizeof(name.n_dcount));
		index+=2;
		memcpy(&name.n_string.s_length,&data[index],sizeof(name.n_string.s_length));
		index+=2;

		name.n_string.s_string= new char[name.n_string.s_length+1];
		memcpy(name.n_string.s_string,&data[index],name.n_string.s_length);
		index+=name.n_string.s_length;

		name.n_string.s_string[name.n_string.s_length]='\0';
		if(name.n_string.s_length == 0)
		{
			mesh->SetLabel("Label");
		}
		else
		{
			mesh->SetLabel(name.n_string.s_string);
		}

	//	PRINTF("%s\n",name.n_string.s_string);

		memcpy(&localaxes,&data[index],sizeof(localaxes));
		index+=sizeof(localaxes);

		mesh->matrix.pos.x = localaxes.la_center.x;
		mesh->matrix.pos.y = localaxes.la_center.y;
		mesh->matrix.pos.z = localaxes.la_center.z;
		mesh->matrix.pos.w = 0.0f;

		mesh->matrix.right.x = localaxes.la_xaxis.x; 
		mesh->matrix.right.y = localaxes.la_xaxis.y; 
		mesh->matrix.right.z = localaxes.la_xaxis.z; 
		mesh->matrix.right.w = 0.0f;

		mesh->matrix.up.x = localaxes.la_yaxis.x; 
		mesh->matrix.up.y = localaxes.la_yaxis.y; 
		mesh->matrix.up.z = localaxes.la_yaxis.z; 
		mesh->matrix.up.w = 0.0f;

		mesh->matrix.view.x = localaxes.la_zaxis.x; 
		mesh->matrix.view.y = localaxes.la_zaxis.y; 
		mesh->matrix.view.z = localaxes.la_zaxis.z; 
		mesh->matrix.view.w = 1.0f;

		/************** test printfs ***************
		PRINTF("%f %f %f\n",localaxes.la_center.x,
							localaxes.la_center.y,
							localaxes.la_center.z);

		PRINTF("%f %f %f\n",localaxes.la_xaxis.x,
							localaxes.la_xaxis.y,
							localaxes.la_xaxis.z);

		PRINTF("%f %f %f\n",localaxes.la_yaxis.x,
							localaxes.la_yaxis.y,
							localaxes.la_yaxis.z);

		PRINTF("%f %f %f\n",localaxes.la_zaxis.x,
							localaxes.la_zaxis.y,
							localaxes.la_zaxis.z);
		*******************************************/

		memcpy(&tm[0],&data[index],sizeof(float) * 12);
		index+=sizeof(float) * 12;

		mesh->trans.right.x = tm[0];
		mesh->trans.right.y = tm[1];
		mesh->trans.right.z = tm[2];
		mesh->trans.right.w = 0.0f;

		mesh->trans.up.x = tm[4];
		mesh->trans.up.y = tm[5];
		mesh->trans.up.z = tm[6];
		mesh->trans.up.w = 0.0f;

		mesh->trans.view.x = tm[8];
		mesh->trans.view.y = tm[9];
		mesh->trans.view.z = tm[10];
		mesh->trans.view.w = 0.0f;

		mesh->trans.pos.x = tm[3];
		mesh->trans.pos.y = tm[7];
		mesh->trans.pos.z = tm[11];
		mesh->trans.pos.w = 1.0f;

		/************** test printfs ***************
		for(j=0;j<16;j++)
		{
			PRINTF("%f\t",	tm[j]);
			if(j%4 == 0) PRINTF("\n");
		}
		/********************************************/

		memcpy(&localv.lv_number,&data[index],sizeof(localv.lv_number));
		index+=sizeof(localv.lv_number);
		
		// Number of vertices in the mesh
		VertType *verts = mesh->NewVerts(localv.lv_number);

		coordptr= new COBCoordType[localv.lv_number];
		memcpy(coordptr,&data[index],(localv.lv_number*sizeof(COBCoordType)));
		index+=(localv.lv_number*sizeof(COBCoordType));

		PRINTF("Number Of Vertices:%ld",localv.lv_number);
		for(i=0;i<localv.lv_number;i++)
		{
			verts[i].pos.x=coordptr[i].x;
			verts[i].pos.y=coordptr[i].y;
			verts[i].pos.z=coordptr[i].z;
			verts[i].pos.w=1.0f;

			if(verts[i].pos.x < mesh->bounds.min.x) mesh->bounds.min.x = verts[i].pos.x;
			if(verts[i].pos.y < mesh->bounds.min.y) mesh->bounds.min.x = verts[i].pos.y;
			if(verts[i].pos.z < mesh->bounds.min.z) mesh->bounds.min.x = verts[i].pos.z;

			if(verts[i].pos.x > mesh->bounds.max.x) mesh->bounds.max.x = verts[i].pos.x;
			if(verts[i].pos.y > mesh->bounds.max.y) mesh->bounds.max.x = verts[i].pos.y;
			if(verts[i].pos.z > mesh->bounds.max.z) mesh->bounds.max.x = verts[i].pos.z;
		}

		memcpy(&localuv.uv_number,&data[index],sizeof(localuv.uv_number));
		index+=sizeof(localuv.uv_number);

		uvptr = new Vector2[localuv.uv_number];
		memcpy(uvptr,&data[index],(localuv.uv_number*sizeof(Vector2)));

	//	for(i=0;i<localuv.uv_number;i++)
	//	{
	//		model->tlist[i].u=uvptr[i].u;
	//		model->tlist[i].v=uvptr[i].v;
	//	}

		index+=(localuv.uv_number*sizeof(Vector2));

		/************** test printfs ***************
		for(i=0;i<localuv.uv_number;i++)
		{
			printf("%f %f\n",uvptr[i].u,
							 uvptr[i].v);
			if (i%25==0) getch();
		}
		*******************************************/

		memcpy(&faces,&data[index],sizeof(faces));
		
		// Allocate new space for all the facets
		FacetType *facets = mesh->NewFacets(faces);

		PRINTF("Number Of Faces:%d",faces);
		index+=sizeof(faces);

		// Iterate through all the facets - setting each facets info
		for(i=0;i<faces;i++)
		{
			memcpy(&flags,&data[index],sizeof(flags));
			index+=sizeof(flags);

	//		PRINTF("%d out of %d     %ld\n",i,faces,index);
	//		PRINTF("%xd \n",flags);
			
			// Check for hole or face type....
			if(flags  & 0x8) 
			{
				// Just skip it - we dont want the info at this stage - TODO.
				memcpy(&vertinface,&data[index],sizeof(vertinface));
				index+=sizeof(vertinface);
				index+=(sizeof(face)*vertinface);
			}
			else
			{
				memcpy(&vertinface,&data[index],sizeof(vertinface));
				index+=sizeof(vertinface);

				ASSERT(vertinface != 3, "Mesh has non-tri poly! Only 3 vertex polys supported currently!");

				memcpy(&matinface,&data[index],sizeof(matinface));
				index+=sizeof(matinface);

				// Set the material Id (thats our internal material handle)
				// the matMap[] maps COB mat ids, to our internal material handles.
				facets[i].matId = matinface;

//				PRINTF("%d  %d\n",vertinface,matinface);

				for(j=0;j<vertinface;j++)
				{
					memcpy(&face,&data[index],sizeof(face));
					index+=sizeof(face);
					
					// Adds a facet index to the effected point
					facets[i].vert[j] = (long)face.fh_vertex;
					facets[i].uv[j].u = uvptr[(long)face.fh_uvindex].u;
					facets[i].uv[j].v = uvptr[(long)face.fh_uvindex].v;
				}
			}
		}

		if(name.n_string.s_string) delete [] name.n_string.s_string;
		if(coordptr) delete [] coordptr;
		if(uvptr) delete [] uvptr;
//		PRINTF("SortPolyChunk complete.\n");
	}

	void sort_mat_chunk(unsigned char * data, unsigned int parentid)
	{
		char			ext[4];
		char			*tname;
		long			index=0;
		short			slength;
		char			buff[20];
		material_type	*material = NULL;

		int				matId = -1;
		int				texId = -1;
		
		{
		Packet("MM_COB","Material.CreateMaterial");
		Psend;

		matId = packet.GetData(D_INT);
		}

		index=0;
		material = new material_type;
		memcpy(material,data,sizeof(material_type));
		index+=sizeof(material_type);

		// Send the material info to the material manager
		{
		Packet("MM_COB", "Material.SetMaterialDiffuse");
		Padd (int)matId;
		Padd material->r; Padd material->g; Padd material->b; Padd material->alpha;
		Psend;
		}

		{
		Packet("MM_COB", "Material.SetMaterialAmbient");
		Padd (int)matId;
		Padd material->r * material->ambient; 
		Padd material->g * material->ambient;  
		Padd material->b * material->ambient; 
		Padd material->alpha * material->ambient; 
		Psend;
		}
		
		{
		Packet("MM_COB", "Material.SetMaterialSpecular");
		Padd (int)matId;
		Padd material->r * material->specular; 
		Padd material->g * material->specular;  
		Padd material->b * material->specular; 
		Padd material->alpha * material->specular; 
		Psend;
		}
		
//		PRINTF("material size: %d\n",sizeof(material_type));
		if((data[index]=='t')&&(data[index+1]==':'))
		{
			index+=3;
			memcpy(&slength,&data[index],sizeof(short));

//			PRINTF("slength :%d\n",slength);
			index+=2;

			tname = new char[slength+1];
			strncpy(tname,(const char*)&data[index],slength);
			tname[slength]=NULL;

			{
			Packet("MM_COB","Material.LoadTexture");
			Padd (const char *)tname; 
			Psend;

			texId = packet.GetData(D_INT);
			}

			{
			Packet("MM_COB", "Material.AddTexture");
			Padd (int)texId;
			Padd (int)matId;
			Psend;
			}

//			PRINTF("texture path: %s",tname);

			index+=slength;
//			memcpy(&tx->tx,&data[index],sizeof(temp_tex_type));

	//_gcvt(texture->t_uoff,10,tb);
	//printf("%f %f %f %f\n",texture->t_uoff,texture->t_voff,
	//						texture->t_uscale,texture->t_vscale);
	//		MessageBox(hWndMain," ",tb,MB_OK);
	//		texture->t_uscale=texture->t_uscale*256.0f;
	//		texture->t_vscale=texture->t_vscale*256.0f;

		}
		else
		{
		}

		// Map COB material index to our index
		if(pidMatMap.find(parentid) != pidMatMap.end())
		{
			pidMatMap[parentid]->matmap[material->number] = matId;
		}
		else
		{
			MatMap *temp = new MatMap;
			temp->matmap[material->number] = matId;
			pidMatMap[parentid] = temp;
		}
		
		ourMatMap[matId] = material;
	}

	FacetObject *add_poly_chunk(MeshType *mesh)
	{
		return mesh->NewObject();
	}

	// Fix up all the material indexes
	void calculate_textures(MeshType *mesh)
	{
		vector<FacetObject *> *objs = mesh->Objects();
		
		for(vector<FacetObject *>::iterator t = objs->begin(); t<objs->end();t++)
		{
			FacetType *flist = (*t)->GetFacets();

			for(int s=0;s<(*t)->GetFacetCount();s++)
			{
				ASSERT( pidMatMap.empty(), "Empty material map - No materials?" ); 
				ASSERT( pidMatMap.find((*t)->polyId)==pidMatMap.end(), "No material of this parent Id?" );

				flist[s].matId = pidMatMap[(*t)->polyId]->matmap[flist[s].matId];

				unsigned int rgba = (unsigned char)(ourMatMap[flist[s].matId]->r * 255);
				rgba |= (unsigned char)(ourMatMap[flist[s].matId]->g * 255) << 8;
				rgba |= (unsigned char)(ourMatMap[flist[s].matId]->b * 255) << 16;
				rgba |= (unsigned char)(ourMatMap[flist[s].matId]->alpha * 255) << 24;

				// Loop through the verts and reassign the right colour
				for(int r=0;r<3;r++)
				{
					flist[s].col[r] = rgba;
				}
			}
		}
	}

	void calculate_vertex_normals(MeshType * mesh)
	{
		// This is where angled faces could be smoothed.( > 45 degs do not smooth?? )
		FacetObject	*ob;
	//	FacetType	*fl;
	//	PointType	*pl;

		vector<FacetObject *> *objs = mesh->Objects();
		
		for(vector<FacetObject *>::iterator t = objs->begin(); t<objs->end();t++)
		{
			FacetType *flist = (*t)->GetFacets();

			for(int s=0;s<(*t)->GetFacetCount();s++)
			{
			}
		}
	}

	int LoadCOB(MeshType *mesh)
	{
		ASSERT( (mesh->name.String()==NULL), "LoadCOB:No filename?!");

		File::FileClass	fin;	// File In
		Data::BlockType	fdata;

		fin.Open(mesh->name.String(),FILE_READ | FILE_BINARY);
		ASSERT( fin.handle <= 0, "Couldnt open file!");
		fin >> &fdata;

		long			matindex=0;
		long			polyindex=0;
		unsigned long	offset = 0;
		unsigned char	*chunkData = NULL;

		COBFileHeaderType	hdr;
		COBChunkHeaderType	chunk;

		PRINTF("%s",mesh->name.String());
		memcpy(&hdr, fdata.GetData(offset), sizeof(hdr));
		offset += sizeof(hdr);

		// Check for binary format else exit..
		ASSERT( (hdr.fh_format!='B') ,"Not Binary COB file.");

		// Test first chunk header...
		memcpy(&chunk, fdata.GetData(offset), sizeof(chunk));
		offset += sizeof(chunk);
//		print_chunk(&chunk);

		// Iterate while we havent reached an end chunk
		while( (strncmp(chunk.ch_type,CH_TYPE_END,4)!=0) && (offset < fdata.GetUsed()) )
		{
			chunkData=new unsigned char[chunk.ch_length];
			memcpy(chunkData,fdata.GetData(offset),chunk.ch_length);
			offset += chunk.ch_length;

			PRINTF("Loaded chunk");
			print_chunk(&chunk);

			if(strncmp(chunk.ch_type,CH_TYPE_POLY,4)==0)
			{
				// Add a data structure so we can fill it out
				FacetObject *fgroup = add_poly_chunk(mesh);
				fgroup->parentId = chunk.ch_parentid;
				fgroup->polyId = chunk.ch_chunkid;

				// Place poly data into structure
				sort_poly_chunk(chunkData,chunk.ch_length,fgroup);

				// Place poly into correct group - and apply group properties
				apply_group_chunk(mesh,chunk.ch_parentid);

//				PRINTF("polyindex %d\n",polyindex);
				PRINTF("Finished Poly Add");
			}
			else

			// Add material to the material manager - get the index for it.. to map to the model
			if(strncmp(chunk.ch_type,CH_TYPE_MAT,4)==0)
			{
				sort_mat_chunk(chunkData,chunk.ch_parentid);
				
				PRINTF("Finished Mat Add");
			}
			else

			// A group type is added - make the group and set attribs
			if(strncmp(chunk.ch_type,CH_TYPE_GROUP,4)==0)
			{
				MeshGroup *group = add_group_chunk(mesh,chunk.ch_chunkid);
				sort_group_chunk(group,chunkData,chunk.ch_length);

				PRINTF("Finished Group Add \n");
			}

			memcpy(&chunk, fdata.GetData(offset), sizeof(chunk));
			offset += sizeof(chunk);

			delete chunkData;
		}

	//	if(glist)free(glist);

	//	char	bf[33];
	//	for(int t=0;t<mesh->get_NOOBJS();t++)
	//	{
	//		object_type *ob=mesh->get_object(t);
	//		MessageBox(GetTopWindow(NULL),chunk.ch_type,_ltoa(ob->nop,bf,10),MB_OK);
	//	}
	//	fclose(fin);

//		if(polyindex==0) exit(4);
		calculate_textures(mesh);
		calculate_vertex_normals(mesh);

		return 1;
	}

}