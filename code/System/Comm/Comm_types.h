
#ifndef _ZEUS_COMM_H_
#define _ZEUS_COMM_H_

#include <map>

#include "..\\Types\\Func_types.h"
#include "..\\Types\\String_types.h"
#include "..\\Types\\Packet_types.h"

using namespace std;

namespace Comm
{
	class BaseClass
	{
	private:
		HFuncTable	fMap;	// Submit functions for external use 
		HVarTable	vMap;	// Submit variables for external use

		// Future use
		unsigned int	send_counter;
		unsigned int	recv_counter;

	public:
		// This is the ZEUS attached function here.. all roads lead to ZEUS! :-)
		Function	sendpacketfunc;

		BaseClass() 
		{
			recv_counter = 0;
			send_counter = 0;
		}
		
		~BaseClass() 
		{
			// Empty the fMap
			fMap.clear();
			vMap.clear();
		}

		inline void Send(Packet::PackClass *p)
		{
			sendpacketfunc.proc(p);
		}

		// ------------------------------------------------------------------------------
		// Function mapping calls

		// ZEUS calls this one back - now internal
		void recvpacket(Packet::PackClass *p)
		{
//			Profile("Comm::BaseClass::recvpacket");
			if(fMap.empty())
			{
				// No point testing an empty function table
				return;
			}

			unsigned int key = p->destcrc.GetFuncId();

			// Go through the function list to match the packet call
			if(key)
			{
				HFuncTable::iterator i = fMap.find(key);
				if(i != fMap.end())
				{
	//				Profile("Comm::BaseClass::fMap.Find");
					(*i).second->proc(p);
				}
			}
			recv_counter++;
		}

		void operator << (Packet::PackClass *packet)  { sendpacketfunc.proc(packet); }

		void SubmitFunction(void (*_proc) (Packet::PackClass *p), const char * _name)
		{
			Function *newfunc = new Function;
			newfunc->name << _name;
			newfunc->proc = _proc;

			fMap[newfunc->name.Id()] = newfunc;
		}

		// TODO:
		// Make this accessible only by zeus - ie embed into packet system.
		HFuncTable *GetFMap(void)
		{
			return (&fMap);
		}

		// ------------------------------------------------------------------------------
		// Variable mapping calls

		// Variables here are very specific. These vars would mainly be used for scripting 
		// and external module control/access. Its important to note that the variables are
		// ONE-WAY, they are either READONLY or WRITEONLY. So we dont have to worry about
		// possible contention problems. Potentially these variables can be used to communicate
		// also but that would be discouraged.
		
		// Zeus will have direct access to these maps, so you will be able to have functions
		// directly manipulate them. ie 
		//							- Zeus.SetVarF("Module.WFloatVar",1.2f);
		//							- Zeus.GetVarF("Module.RFloatVar");

//		void SubmitVariable(Variable *v, const char * _name)
//		{
//			v->name << _name;
//			vMap[v->name.Id()] = v;
//		}

		// TODO:
		// Make this accessible only by zeus - ie embed into packet system.
		HVarTable *GetVMap(void)
		{
			return (&vMap);
		}

	};
}

#endif