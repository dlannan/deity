#ifndef _FONTTYPE_H_
#define _FONTTYPE_H_

#include "..\\..\\System\System.h"

// The font that is loaded in is a texture plus a width table.
// All text is in alpha channel, and colours and blend modes, determine the 
// resulting output.

class FontType
{
public:
	CRC::StringClass	name;			// Should include full path for font
	CRC::StringClass	path;			// Should include full path for font
	
	unsigned int		matid;			// Assigned material index

	int					size;			// Maximum height character
	int					widths[256];	// Each characters width in pixels
										// first width is the maximum height (char[0] should be unused)

	unsigned int		Fcolour;		// Text colour
	unsigned int		Bcolour;		// Background colour
};

#endif