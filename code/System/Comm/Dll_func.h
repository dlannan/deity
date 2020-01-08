// Generic DLL binding defines
// Supporting Win32 and GCC hopefully.

#ifndef _DLL_FUNC_H_
#define _DLL_FUNC_H_

#if defined(WIN32)
#define	DLLEXPORT __declspec(dllexport)
#define	DLLENTRY __stdcall
typedef void (* DLL_EP)(void);
#define	DLL_EP_PTR __stdcall *

#else
#define	DLLEXPORT
#define	DLLENTRY
typedef void (* DLL_EP)(void);
#define	DLL_EP_PTR *
#endif

typedef void DLL;

DLL_EP dll_entrypoint(DLL *dll, const char *name, const char *args);
DLL   *dll_load(const char *name);
void   dll_unload(DLL *dll);

#endif
