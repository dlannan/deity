// System header

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

// UGLY windows include
#include <windows.h>

// Some pretty whack defines :-)
#define		TRUE	1
#define		FALSE	0

#define		ON		1
#define		OFF		0

// Debugging types - must be first so debugging is available in all types
#include "Debugging\\Error_types.h"
#include "Debugging\\Profile.h"

// System Settings - CPU and API related
#include "API\\CPU_types.h"
#include "API\\Render_types.h"

// Data Types 
#include "Types\\Crc_types.h"
#include "Types\\String_types.h"
#include "Types\\Binary_types.h"
#include "Types\\Nodelist_types.h"
#include "Types\\Func_types.h"
#include "Types\\File_types.h"
#include "Types\\Timer_types.h"
#include "Types\\Util_types.h"


// Communication types
#include "Types\\Packet_types.h"

#include "Comm\\Apollo.h"
#include "Comm\\Comm_types.h"
#include "Comm\\Dll_func.h"

// Geometry types
#include "Geometry\\Math_types.h"

#include "Geometry\\Matrix.h"
#include "Geometry\\Vertex.h"
#include "Geometry\\Mesh_types.h"

// Some more debugging - general Apolonius output defines
// Rely on some previous includes
#include "Debugging\\Logging.h"

#endif 