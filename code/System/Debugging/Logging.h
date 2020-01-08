// System header

#ifndef _DEBUG_LOGGING_H_
#define _DEBUG_LOGGING_H_

// THIS IS TOTAL SHIT - FIX IT DAMMIT!

#define  DebugLog(sc)			{											\
								Packet("This","Zeus.Log");					\
								Padd(sc);									\
								Psend;										\
								}

namespace Debugging
{
}

#endif