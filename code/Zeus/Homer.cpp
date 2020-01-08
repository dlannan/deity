// HOMER
// Scripting system - first great literary person.

#include <math.h>

#include "..\\System\\System.h"
#include "Homer.h"
#include "lua.h"

namespace HOMER
{
	#define		MAX_SCRIPT_BUFFER_SIZE		100*1024		// 100K of script? anyone?

	struct Fcmp
	{
		bool operator()(CRC::StringClass s1, CRC::StringClass s2) const
	  {
		return (s1.Id() < s2.Id());
	  }
	};

	// Lua name, buffer mapping. When LoadScript is called it is mapped to its CRC file name
	// Then it can be called by calling RunScript which executes lua_dobuffer
	typedef map<CRC::StringClass, char *,Fcmp> HomerScriptType;
	HomerScriptType		scriptList;

	// Current scriptid being run - map index
	const char *scriptRunning = NULL;

	// We might support out of order script lists
	vector<int> scriptIndexList;
	int scriptNext;
	int scriptStart;
	
	// This is the page of script in use
	Data::PageType	scriptpage;

	// *********************************************************************************8
	// Lua Stuff

	// Lua state machine - Scripting, beautiful scripting
	lua_State		*lua;

	// Lua CRC conversion call
	static int ScriptCRC(lua_State *L)
	{
		CRC::StringClass	crc;
		int n=lua_gettop(L);	// Get the number of parameters
		ASSERT(n!=1, "ScriptCRC::Incorrect parameters.");
		
		switch(lua_type(L, 1))
		{
		case LUA_TSTRING:
			{
				const char *str = lua_tostring(L,1);
				unsigned int no = CRC::Str(str);
				lua_pushnumber( L, (double)no);
				return 1;
				break;
			}
		case LUA_TNUMBER:
			{
				double in = lua_tonumber(L,1);
				unsigned int no = CRC::Int((unsigned int)in);
				lua_pushnumber( L, (double)no);
				return 1;
				break;
			}
		}
		
		return 0;
	}

	// Lua Packet function call
	static int ScriptPacket(lua_State *L)
	{
		int n=lua_gettop(L);	// Get the number of parameters
		ASSERT(n<2, "Not enough parameters for Packet function.");

		const char *srcC = (lua_tostring(L,1));
		const char *dstC = (lua_tostring(L,2));

		static Packet::PackClass	packet;

		packet.Build(srcC, dstC);

//		PRINTF("Src:%s",lua_tostring(L,1));
//		PRINTF("Dst:%s",lua_tostring(L,2));

		for (int i=3; i<=n; i++)
		{
			switch(lua_type(L, i))
			{
			case LUA_TSTRING:
				Padd (lua_tostring(L,i));
				break;
			case LUA_TNUMBER:
				{
					// Figure out the type - int or float
					double num = lua_tonumber(L,i);
					double numI;
					if(fabs(modf(num, &numI)) > 0.0000000)
					{
						Padd (float)num;
					}
					else
					{
						Padd (int)numI;
					}
				}
				break;
			case LUA_TUSERDATA:
				break;
			case LUA_TFUNCTION:
				break;
			case LUA_TNIL:
				break;
			}

//			PRINTF("%s:%p",lua_typename(L,lua_type(L,i)),lua_topointer(L,i));
		}

		apollo.Send(&packet);
		if(n==2) return 0;

		// Return all values in the packet
		for(int i=0; i<packet.Params(); i++)
		{
			switch (packet.GetDataType(i))
			{
			case	Packet::P_VOID:
				lua_pushnil(L); break;
			case	Packet::P_STRING:
				lua_pushstring(L,packet.GetDataIndex(i,D_STRING)); break;
			case	Packet::P_UCHAR:
				lua_pushnumber(L,packet.GetDataIndex(i,D_UCHAR)); break;
			case	Packet::P_CHAR:
				lua_pushnumber(L,packet.GetDataIndex(i,D_CHAR)); break;
			case	Packet::P_UINT:
				lua_pushnumber(L,packet.GetDataIndex(i,D_UINT)); break;
			case	Packet::P_INT:
				lua_pushnumber(L,packet.GetDataIndex(i,D_INT)); break;
			case	Packet::P_ULONG:
				lua_pushnumber(L,packet.GetDataIndex(i,D_ULONG)); break;
			case	Packet::P_LONG:
				lua_pushnumber(L,packet.GetDataIndex(i,D_LONG)); break;
			case	Packet::P_FLOAT:
				lua_pushnumber(L,packet.GetDataIndex(i,D_FLOAT)); break;
			}
		}

		return packet.Params();
	}

	void LoadScript(Packet::PackClass *in)
	{
		ASSERT(in == NULL, "LoadScript:Need a valid packet please.")

		File::FileClass		scriptFile;
		Data::StringClass	scriptFileName;
		CRC::StringClass	scriptName;

		scriptFileName << in->GetData(D_STRING);
		scriptName << in->GetData(D_STRING);

		scriptpage.clear();

		// Check the list - If its here then dont load again!
		if(scriptList.find(scriptName)==scriptList.end())
		{
			char *buffer = new char[MAX_SCRIPT_BUFFER_SIZE];

			int error = 0;
			error = scriptFile.Open(&scriptFileName,FILE_READ);
			
			// This is pretty drastic.. but without the cfg file.. zeus aint much use
			if(error > 0) 
			{
				return;
			}

			// Read the file into zeuscfg
			scriptFile >> &scriptpage;
			scriptFile.Close();

			if(scriptpage.size()>0)
			{
				strcpy(buffer,scriptpage[0].String());
				// Build a nice big buffer with all the script in it.
				for(int i=1;i<scriptpage.size();i++)
				{
					// Add the strings to the big buffer
					strcat(buffer,scriptpage[i].String());
				}
			}
			
			// Add scriptpage to the scripts 
			scriptList[scriptName] = buffer;
		}

		in->Clear();
		(*in)+= scriptName.String();
	}

	void ParseScript(Data::PageType &page)
	{
	}

	void SaveScript(Packet::PackClass *in)
	{
	}

	void EditScript(Packet::PackClass *in)
	{
	}

	void Interactive(Packet::PackClass *in)
	{
	}

	void StartRecScript(Packet::PackClass *in)
	{
		// Get script index
		CRC::StringClass index;
		index << in->GetData(D_STRING);

		ASSERT(scriptList.find(index)==scriptList.end(), "StartRecScript: Invalid script id.");
		scriptRunning = index.String();

		int status = lua_dobuffer(lua,scriptList[index], strlen((const char *)scriptList[index]) ,index.String());
		ASSERT(status != 0, "Problem parsing script.");
		scriptRunning = NULL;
	}

	void StopRecScript(Packet::PackClass *in)
	{
	}

	// Init Homer.. get ready to get happy and make some lovely apps
	void Init()
	{
		// Init Lua environment
		lua=lua_open(0);
		
		// Register extra lua functions - Packet
		lua_register(lua,"Packet",ScriptPacket);
		lua_register(lua,"CRC",ScriptCRC);
	}

	void Kill()
	{
		map<CRC::StringClass,char *,Fcmp>::iterator i = scriptList.begin();
		while(i!=scriptList.end())
		{
			delete i->second;
			i++;
		}

		scriptList.clear();
	}
}