// ZEUS Classes

#include <windows.h>
#include <winbase.h>

#include <process.h>

// Types, Definitions and misc
#include "..\\System\\System.h"

// Zeus and module related
#include "ZEUS.h"
#include "ZEUS_debug.h"

#include "Appolonius.h"
#include "Homer.h"

using namespace std;

// This is the interface for all objects in zeus.. to the outside world..
ApolloClass	apollo;

namespace ZEUS 
{
//-----------------------------------------------------------------------------------------------
//
// Some local variables not to be externally shared!

	//Comm::BaseClass	comm;
	//#define apollo	comm
	
	// Some nice lists of data.. woohoo templates rok!
	ApolloMap	apolloList;
	ApolloMap	apolloListIdle;	// Special list for disabled modules

	// List of functions to be called during main loop
	vector<Function*>	RegisteredFunctions;

	// Polled Function type - the timer keeps track of when to call the function
	typedef struct {
		Function		*functor;
		Timer::Alarm	*timer;
	} PolledType;

	// These are timer driven function calls
	vector<PolledType*>	PolledFunctions;

	// The startup order of the apollos 
	//		-- we Init in this order and Kill in the reverse order
	vector<ApolloClass *> apolloStartUp;	

	unsigned int	shutdown;
	int				priority;

	unsigned int	debugmode;
	unsigned int	debugwindow;

	int				debugstarted = 0;
	bool			debugcheck = false;

	Timer::Clock	masterClock;

//-----------------------------------------------------------------------------------------------
//
// Function prototypes

	void OutputBuffer();
	int Exception(int n_except);

	void CheckKeys(void);
	bool CheckDebug(void);
	void GetSettings(char **argv, int argc);
	void AddApollos(Data::StringClass line);
	void AddHercules(Data::StringClass line);

	// Interface Functions - why I didnt do this earlier I dont know!!
	// Module related calls
	void LoadModule(Packet::PackClass	*in);
	void UnLoadModule(Packet::PackClass	*in);
	void InitModule(Packet::PackClass	*in);
	void KillModule(Packet::PackClass	*in);
	void DisableModule(Packet::PackClass	*in);
	void InitAll(Packet::PackClass	*in);

//-----------------------------------------------------------------------------------------------
//
// Functions

#if defined (WIN32)
	// Bah!
	HINSTANCE	instance;

	// ---------------------------------------------------------------------------------------
	// DLL gear
	DLL_EP dll_entrypoint(DLL *dll, const char *name, const char *args)
	{
		HINSTANCE hInst = (HINSTANCE) dll;
		char epname[1+100+1+3+1];
		sprintf(epname, "_%s@%s", name, args);
		return (DLL_EP) GetProcAddress(hInst, epname);
	}	

	DLL *dll_load(const char *name)
	{
		// Doesnt start a thread for it - we dont need one!
		return (DLL *) LoadLibrary(name);
	}

	void dll_unload(DLL *dll)
	{
		HINSTANCE hInst = (HINSTANCE) dll;
		FreeLibrary(hInst);
	}
#endif

	void GetInstance(Packet::PackClass	*in)
	{
		in->Clear();
		(*in) += (unsigned long)instance;
	}

	void RegisterFunction(Packet::PackClass	*in)
	{
		Function  *newfunc = new Function;
		newfunc->name = in->srccrc.String();
		newfunc->proc = (void (__stdcall *)(class Packet::PackClass *))in->GetData(D_ULONG);

		RegisteredFunctions.push_back(newfunc);
	}

	void PolledFunction(Packet::PackClass *in)
	{
		PolledType *newpollfunc = new PolledType;

		Function  *newfunc = new Function;
		newfunc->name = in->srccrc.String();
		newfunc->proc = (void (__stdcall *)(class Packet::PackClass *))in->GetData(D_ULONG);

		newpollfunc->functor = newfunc;
		Timer::Alarm *newalarm = new Timer::Alarm(in->GetData(D_ULONG));
		newpollfunc->timer = newalarm;

		PolledFunctions.push_back(newpollfunc);
	}


	void SendPacket(Packet::PackClass	*ptr);

	// The main function that kickstarts ZEUS into action. 
	// Should be the primary call for any program.
	// Things ZEUS should do in init:
	//		Prepare system timers
	//		Prepare debugging system
	//		Prepare Script system - Homer
	//			- includes Var, Packet control and Builtin editor modules
	//		Check hardware dependancies
	//			- Graphics module (Default can be overidden)
	//			- Input module (keyboard, mouse or controller?)
	//			- Windows module (Builtin windows event system - can be overidden)
	//		Init Graphics, Input and Windows modules
	//		Init Debugging system
	//		Init Scripting system
	void Prep(char **argv, int argc)
	{
		Packet("Zeus","Apolonius.Init");
		Padd (unsigned long)&apolloList;

		// Create a debug class first!!
		APPOLONIUS::Init(&packet);

		// Init the Debug system
		Debug::InitDebug();

		// ZEUS needs to make sure its base class sendpacket is set
		// This is so commands like zeus << Packet are valid through the base class
		// All modules are given this function so they can call zeus 
		// without actually 'knowing' they are calling zeus.
		apollo.sendpacketfunc.name << "ZEUS.sendpacketfunc";
		apollo.sendpacketfunc.proc = ZEUS::SendPacket;

		// These are 'global' type function registration - should be used carefully
		// Register some usable calls for zeus - DONT REGISTER AN INIT OR KILL!!!
		apollo.SubmitFunction(ZEUS::LoadModule,			"LoadModule");
		apollo.SubmitFunction(ZEUS::UnLoadModule,		"UnLoadModule");
		apollo.SubmitFunction(ZEUS::InitAll,			"InitAllModules");
		apollo.SubmitFunction(ZEUS::InitModule,			"InitModule");
		apollo.SubmitFunction(ZEUS::KillModule,			"KillModule");
		apollo.SubmitFunction(ZEUS::DisableModule,		"DisableModule");

		// these functions are likely to be removed - currently unacceptable in design terms.
		apollo.SubmitFunction(ZEUS::GetInstance,		"GetInstance");
		apollo.SubmitFunction(ZEUS::RegisterFunction,	"RegisterFunc");
		apollo.SubmitFunction(ZEUS::PolledFunction,		"PolledFunc");

		// Appolonius the debugger calls
		apollo.SubmitFunction(APPOLONIUS::Disable,		"Disable");
		apollo.SubmitFunction(APPOLONIUS::Log,			"Log");
		apollo.SubmitFunction(APPOLONIUS::SetLogFile,	"SetLogFile");
		apollo.SubmitFunction(APPOLONIUS::Break,		"Break");
		apollo.SubmitFunction(APPOLONIUS::Start,		"Start");
		apollo.SubmitFunction(APPOLONIUS::Cycle,		"Cycle");
		apollo.SubmitFunction(APPOLONIUS::Report,		"Report");

		// Homer scripting calls
		apollo.SubmitFunction(HOMER::LoadScript,		"LoadScript");
		apollo.SubmitFunction(HOMER::SaveScript,		"SaveScript");
		apollo.SubmitFunction(HOMER::EditScript,		"EditScript");
		apollo.SubmitFunction(HOMER::Interactive,		"Interactive");
		apollo.SubmitFunction(HOMER::StartRecScript,	"StartRec");
		apollo.SubmitFunction(HOMER::StopRecScript,		"StopRec");

		APPOLONIUS::Out("ZEUS started\n");

		// Default priority is 0.. normal windows app
		// Priority will eventually allow the programmer to control the system
		// resource allocation to zeus. ie a value of 128 will give all resources
		// to zeus and -128 will make zeus hardly called at all.
		priority = 0;
		shutdown = 0;

		// Debug mode - on or off;
		// Debugmode on allows you to break into zeus and issue commands, off
		// disables this
		debugmode = ON;
		debugwindow = OFF;
		
		// Init any homer items - vars, scripting etc
		HOMER::Init();

		int HSF_Zeus = -1;
		{
		Profile("Zeus.LoadScript");
		Packet("Zeus.Init","Zeus.LoadScript");
		// TODO:
		// Rip this shit out - hard coded depednencies - yuk!
		Padd "zeus.hsf";
		Padd "ZeusInit";
		HOMER::LoadScript(&packet);
		}

		{
		Packet("Zeus.Init","Zeus.StartRec");
		Padd "ZeusInit";
		HOMER::StartRecScript(&packet);
		}

//		map<const char *, Profile::Sample *>::iterator si = Profile::ProfileMap.find("String");
//		PRINTF("String:\nacc %l\ncount %d\nmax %l\n",(si->second)->acc,(si->second)->count,(si->second)->max);
//		si = Profile::ProfileMap.find("StringClass");
//		PRINTF("StringClass:\nacc %l\ncount %d\nmax %l\n",(si->second)->acc,(si->second)->count,(si->second)->max);
	}

	void CheckKeys(void)
	{
		// Call is platform specific, but should be as generic as possible
		// for portability

		// Check the few keys that really matter :-)
		short shift = (short)GetAsyncKeyState(VK_SHIFT);
		short control = (short)GetAsyncKeyState(VK_CONTROL);
		short alt = (short)GetAsyncKeyState(VK_MENU);
		short debug = (short)GetAsyncKeyState(VK_F1);
		short exit = (short)GetAsyncKeyState(0x51); // Q??!

		short f1 = (short)GetAsyncKeyState(VK_F1);
		short f2 = (short)GetAsyncKeyState(VK_F2);
		short f3 = (short)GetAsyncKeyState(VK_F3);

		// Shift-Escape is the master exit for Zeus
		if( control && alt && exit )
		{
			shutdown = 1;
		}

		// Control+Alt+F1 used to activate the debugging window
		if( control && alt && debug )
		{
			debugwindow = ON;
		}

		if( f1 ) Error::CustomPrintfWindowScrollDown();
		if( f2 ) Error::CustomPrintfWindowScrollUp();
		if( f3 ) Error::CustomPrintfWindowToggleHide();
	}

	bool CheckDebug(void)
	{
		// If we are in debug, dont process anything! Only debug window info
		if(debugwindow == ON)
		{
			static HWND win;

			if(debugstarted == 0)
			{
				Debug::SetApolloList(&apolloList);
//				Debug::SetInputBuffer(
				win = Debug::InitWindow();
				debugstarted = 1;
			}
			else
			{	
				// If the debug window is closed or removed for some reason.. 
				// Allow it to be re-enabled.
				if(!IsWindowVisible(win))
				{
					debugstarted = 0;
					debugwindow = OFF;
				}

				// If the debug window has started cycle counting
				if(Debug::CycleCount())
				{
					Debug::CycleCountDec();
				}

			}
			return true;
		}
		else
		{
			return false;
		}
	}

	// This will be the function that keeps everything alive
	// Init will call this.. and Processing will begin
	void Process(char **argv, int argc)
	{
		// We always loop - game conditions break this loop
		while(!shutdown)
		{
			// Update the ZEUS clock
			masterClock.Update();
//			APPOLONIUS::Out("Clock check MS: ");
//			APPOLONIUS::Out(masterClock.Ms());
//			APPOLONIUS::Out("\n");

			// Basic Message Pump (for WIN32) - should go on a timer and be done intermittently
			MSG msg;
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT || msg.message == WM_CLOSE)
					shutdown = 1;

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			// Debug window hold control while the window is open
			// EXPENSIVE - to call this so often - ie per packet

			// Put this on a polled function - much better, for performance
			if( (debugcheck = CheckDebug()) == false )
			{
				// TODO - FIX THIS BAD SHIT DAVE!!!
				//			Should NEVER directly call functions no matter what.
				//			Reason - theres no checking on the function call,
				//					 if its not there, DEiTY breaks badly.
				if(RegisteredFunctions.size()>0)
				{
					// Call the registered functions for the main loop
					for(int i = 0; i<RegisteredFunctions.size(); i++)
					{
						static Packet::PackClass	packet;
						RegisteredFunctions[i]->proc(&packet);
					}
				}

				// Polled functions check
				// Iterate through all the registered polled functions, check their timer
				// and call the function if the timer alarms. Use the global ZEUS clock
				// to check againast always. Ensure consistancy amongst modules.
				if(PolledFunctions.size()>0)
				{
					// Call the registered functions for the main loop
					for(int i = 0; i<PolledFunctions.size(); i++)
					{
						if( PolledFunctions[i]->timer->Poll(masterClock.Ms()) )
						{
							static Packet::PackClass	packet;
							PolledFunctions[i]->functor->proc(&packet);
						}
					}
				}

				// TODO:
				// Assess input event buffer 
				// If invlaid destinations are found (quick crc check), dump the packets, or
				// return a Error packet to the source module
				// Always only process a few packets..to keep everything ticking over
				// We should do a check here.. if it looks like after a period of time
				// we are not removing enough.. we need to go to debug and stop zeus.
//				InputBuffer();
				
				// TODO:
				// Assess output event buffer
				// Move a specific number of packets out of the buffer
//				OutputBuffer();
			}

			// TODO:
//			if(ZEUSComplete()) continue;
		}
		// Soft close..
		return;
	}

	void OutputBuffer(void)
	{
	}

	// Relatively expensive - per packet esp.
	// Need to reduce or remove down the track
	void SendPacket(Packet::PackClass	*ptr)
	{
		// TODO: 
		//		Make sure this is the right check count 
		//		- 2000 is about 1 sec on a 450 AMD K6 which seems a decent supervisor check rate
		static int counter = 0;
		counter ++;
		if(counter > 2000)
		{
			// Assess the keyboard input
			// Have the ability to break into the debugger, shutdown Zeus nicely
			// or just trapped keys for specifically bound debugging events.
			CheckKeys();
			counter = 0;
		}

		if(shutdown == 1) return;

		int broadcast = FALSE;

		if(ptr->destcrc.GetModId() == 0x2e251bf9) // zeus
		{
			switch(ptr->destcrc.Id())
			{
			case 0xe71d66f5: // zeus.shutdown
				// Close nicely - finish delivering all packets
				shutdown = TRUE;
				return;
				break;

			// USE BROADCASTS WITH CARE - THESE ARE DANGEROUS CREATURES!
			// PLEASE DONT USE SELF LOOPING BROADCASTS!!!
			case 0x233e6642: // zeus.broadcast 
				broadcast = TRUE;
				// Send to all modules if we are broadcast
				if(broadcast == TRUE)
				{
					ApolloMap::iterator ct = apolloList.begin();

					while( ct != apolloList.end() ) 
					{
						if( (*ct).second )
						{
							if( (*ct).second->state == true )
							{
								(*ct).second->recvpacket(ptr);
							}
						}
						ct++;
					}
					broadcast = FALSE;
				}
				break;

			case 0xc56e6594: // zeus.debug.break
				// A packet breaks stops the debugger at the current packet
				debugwindow = ON;
				return;	// This is premature, but should be safe.
				break;

			// Zeus timers
			case 0x4fd155cb: // zeus.timer.add
				break;
			
			case 0x96bf3a1a: // zeus.timer.remove
				break;
			default:			// By default, try sending it to locally submitted functions
				apollo.recvpacket(ptr);
				break;
			}
		}

		else
		{
			// Go thru appollolist to find a dest packet match
			// Should use keys here! Save searching for them!
			ApolloMap::iterator ct = apolloList.find(ptr->destcrc.GetModId());
			if(ct != apolloList.end()) 
			{
				if( ct->second )
				{
					if( ct->second->state == true )
					{
						ct->second->recvpacket(ptr);
					}
				}
			}
		}
	}

	int FindApollo(unsigned int id)
	{
		if(apolloList.size())
		{
			ApolloMap::iterator ct = apolloList.find(id);
			if(ct != apolloList.end()) return 1;
		}

		return 0;
	}

	void AddApollos(Data::StringClass line)
	{
		char	*token = NULL;
		char	seps[] = ",";
		char	dllseps[] = ".dll";

		Data::StringClass	dllname;

		CRC::StringClass	check;
		CRC::StringClass	modulename;

		ApolloClass	*newmod = NULL;

		// Go looking for the external dll.. this is NOT a standard
		// Win32 dll..- its UNIX/ANSI based, although the entry points used are similar..
		DLL *dll;
		DLL_EP ep_setzeus;

		check << line.String();

		// Look for the comma for the dll name. Format - <Modulename>,<Module dll name>
		char *test = check.String();
		token = strtok(test,seps);
		if(token != NULL)
		{
			modulename << token;
			modulename.Orig().NoWhiteSpace();

			token = strtok(NULL,seps);

			// Check for dll name
			if(token == NULL)
			{
				dllname << modulename.Orig().String();
				dllname.NoWhiteSpace();
				dllname << ".dll";
			}
			else
			{
				dllname << token;
				dllname.NoWhiteSpace();
			}
		}
		else
		{
			modulename << check.Orig();
			modulename.Orig().NoWhiteSpace();

			dllname << check.Orig().String();
			dllname.NoWhiteSpace();
			dllname << ".dll";
		}

		// Make sure we dont have one with duplicate label!
		ASSERT (FindApollo(modulename.Id())!=NULL, "ZEUS::AddApollos")
		
		APPOLONIUS::Out("Adding Apollo:");
		APPOLONIUS::Out( line.String());
		APPOLONIUS::Out("\n");

		// Try and load the dll
		dll = dll_load((const char *)dllname.String());
		if(dll)
		{
			if ( (ep_setzeus = dll_entrypoint(dll, "SetZeusFunc", "4" )) == NULL )
			{
				dll_unload(dll);
				APPOLONIUS::Out("progname: DLL libname doesn't have required entrypoints\n");

				// This might be a bit over the top use of assert - but we do wanna check dlls
				ASSERT ( ep_setzeus == NULL, "ZEUS::AddApollos");
			}
			else
			{
				Profile("AddApollos");
				APPOLONIUS::Out(dllname.String());
				APPOLONIUS::Out(" loaded.\n");

				// Set the zeus send function
				newmod = (ApolloClass *)  ( (void *(DLL_EP_PTR)(void *)) ep_setzeus )(&apollo.sendpacketfunc) ;
				newmod->Sdll = dllname.String();
				newmod->dll = dll;
				newmod->id = modulename.String();

				// Enable by default - we will have options in cfg to disable on startup
				newmod->state = true;

				apolloList.insert(pair<unsigned int, ApolloClass *>(newmod->id.Id(),newmod));
				apolloStartUp.push_back(newmod);
			}
		}
		else
		{
			APPOLONIUS::Out(dllname.String());
			APPOLONIUS::Out(" not found.\n");
		}
	}

	void InitAll(Packet::PackClass	*in)
	{
		CRC::StringClass		tempcrc;
		Data::StringClass		tempstr;
		if(apolloList.size())
		{
			vector<ApolloClass *>::iterator	i = apolloStartUp.begin();

			// Init each apollo
			while(i != apolloStartUp.end())
			{
				Packet::PackClass	mod;
				mod += (*i)->id.String();
				InitModule(&mod);
				i++;
			}
		}
	}

	// Once certain conditions are met Kill will be called by Process
	void Kill()
	{
		ProfileDumpAll();

		// Remove the scripts first
		HOMER::Kill();

		// The destructor cleanly closes the modules and controllers by calling their destructors
		if(apolloStartUp.size())
		{
			vector<ApolloClass *>::reverse_iterator i = apolloStartUp.rbegin();
			// Kill each apollo
			while(i != apolloStartUp.rend())
			{
				if(*i)
				{
					Packet::PackClass p;
					ApolloClass	*aptr = (*i);
					p += aptr->id.String();
					KillModule(&p);
				}

				i++;
			}

			// Kill each apollo dll
			while(i != apolloStartUp.rend())
			{
				if(*i)
				{
					Packet::PackClass p;
					ApolloClass	*aptr = (*i);
					p += (unsigned long)aptr->dll;
					UnLoadModule(&p);
				}

				i++;
			}
	
		}

		// Make sure the debug system is shut down
		APPOLONIUS::Out("ZEUS complete\n");
		APPOLONIUS::Kill(NULL);
	}

	int Exception ( int n_except )
	{
		// Assess the exception.. and do some debugging to help out...
		// Probably open a debug window and console..

		// Execute some code to clean up problem?

		// See if we can re-enter without crash?
		return 0;
	}

	void LoadModule(Packet::PackClass	*in)
	{
		Profile("Zeus.LoadScript");
		Data::StringClass	line;
		Data::StringClass	label;			// Module label
		Data::StringClass	dllpathname;	// Dll name and path
//		ProfileEnd();

		label << in->GetData(D_STRING);
		dllpathname << in->GetData(D_STRING);

		line << label.String();
		line << ',';
		line << dllpathname.String();
//WARNING((char *)line.String());

		AddApollos(line);
	}

	void UnLoadModule(Packet::PackClass	*in)
	{	
		dll_unload((DLL *)in->GetData(D_ULONG));
	}

	void InitModule(Packet::PackClass *in)
	{
		CRC::ModIdClass	s;
		s << in->GetData(D_STRING);
		s << ".Init";
		Packet("Zeus",s.String());
		Psend;
	}

	void KillModule(Packet::PackClass *in)
	{
		CRC::ModIdClass	s;
		s << in->GetData(D_STRING);
		s << ".Kill";
		Packet("Zeus",s.String());
		Psend;
	}

	// Stops all packet traffic to a Module
	void DisableModule(Packet::PackClass *in)
	{
		unsigned int	id = in->GetData(D_UINT);

		ApolloMap::iterator ct = apolloList.find(id);
		if(ct != apolloList.end())
		{
			(*ct).second->state = false;
			return;
		}
	}

}

