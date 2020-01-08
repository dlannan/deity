// API header

#ifndef _API_CPUTYPES_H_
#define _API_CPUTYPES_H_

namespace API 
{

#if defined (_RELEASE)
	enum { RELEASE = 1 };
#else
	enum { RELEASE = 0 };
#endif
	
	// Supported device types
	enum cpuType { AMD, INTEL, MIPS };

	static void checkCpu()
	{
		struct {
			union {
				unsigned long value;
				struct {
					bool FPU : 1;	//Is there an FPU present?
					bool VME : 1;	//Virtual 8086 mode extensions
					bool DE : 1;	//Debugging extensions
					bool PSE : 1;	//Page size extensions
					bool TSC : 1;	//Time stamp counter (supports RDTSC instruction)
					bool MSR : 1;	//Model specific registers
					bool PAE : 1;	//Physical address extension
					bool MCE : 1;	//Machine check exception
					bool CX8 : 1;	//Supports CMPXCHG8B instruction (compare and exchange 8 bytes)	
					bool APIC : 1;	//On-chip advanced programmable interrupt controller
					unsigned long _res1 : 1;
					bool SEP : 1;	//Supports fast system call - SYSENTER and SYSEXIT			
					bool MTRR : 1;	//Memory type range registers
					bool PGE : 1;	//PTE Global flag	
					bool MCA : 1;	//MAchine check architecture
					bool CMOV : 1;	//Supports CMOVcc, and if FPU is present, FMOVcc and FCOMI
					bool FGPAT : 1;	//Page attribute table	
					bool PSE36 : 1;	//Supports 4MB pages and 36 bit physical addresses	
					bool PN : 1;	//Processor number
					unsigned long _res2 : 4;	
					bool MMX : 1;	//MMX available	
					bool FXSR : 1;	//FXSAVE and FXRSTOR available
					bool XMM : 1;	//Streaming SIMD instructions
					unsigned long _res3 : 6;	
				};		
			};	
		} cpuInfo;	
	
		__asm {
			mov eax, 0x01		//for cpuid, eax == 0, 1, or 2
			cpuid		
			mov cpuInfo.value, edx	
		}
	}

}

#endif 