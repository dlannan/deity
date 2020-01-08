#ifndef _ERROR_TYPES_H_
#define _ERROR_TYPES_H_

namespace Error
{
#if defined (_DEBUG)
	// Neat little compile time assert
	#define ASSERTC(expn) typedef char __C_ASSERT__[(expn)?1:-1]

	// Custom assert
	bool CustomAssertFunc( bool, char *, char *, int, char *, bool * );
	
	#define ASSERT_STRING(expr) #expr

	#define ASSERT( exp, description )											\
	{	static bool ignoreAlways = false;										\
		if( ! ignoreAlways )  {													\
		if( Error::CustomAssertFunc( int(exp),									\
						ASSERT_STRING(exp), description,						\
						__LINE__, __FILE__, &ignoreAlways ) ) 					\
			{ _asm { int 3 } }													\
		}																		\
	}

	
	// Reporting calls
	// Custom error
	void CustomWarnFunc( char * , int, char *);

	#define WARNING( str ) Error::CustomWarnFunc( str, __LINE__, __FILE__ );

#else
	#define ASSERT( exp, description )

	#define WARNING( str ) 
#endif

	void CustomPrintfFunc( char *, ... );

	// Window Printfs - just for James :-)
	#define PRINTF	Error::CustomPrintfFunc

	void CustomPrintfWindowScrollDown();
	void CustomPrintfWindowScrollUp();
	void CustomPrintfWindowToggleHide();
}

#endif