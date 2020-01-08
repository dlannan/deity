
#ifndef _aInput_H_
#define _aInput_H_

#include "..\\..\\System\System.h"

// Dont forget to add includes if necessary!
extern ApolloClass	apollo;

namespace aInput 
{
	#define		INPUT_MAX_MAPPED_KEYS		256		

	// This is how we attach packets to key events. Empty CRC's indicate a null map.
	// This could cause 'packet swamping'.. it may be better to pass the keymap
	typedef struct {
		unsigned int	crcfunc;		// function attached to key
		unsigned int	crcmod;			// dest module of the mapped function
		unsigned int	event;			// event the function packet should be called on
	} CRCmapType;

	// Need a mapping method for combined keys too - to cover special keys - Shift, Ctrl, Alt etc

	typedef struct {
		CRCmapType		m[INPUT_MAX_MAPPED_KEYS];	// all 256 keys can be mapped once (at the moment)

	}KeymapType;

	typedef struct {
		map<int,CRCmapType *>	e;		// Map events to mouse indexes

		unsigned long			x,y;
		unsigned int			dx,dy,dz;
		unsigned int			buttons;
	}MouseType;

	// A region type allows a mouse to deliver events to registered functions
	typedef struct {
		RectType				box;	// Region to test
		vector<CRCmapType *>	e;		// Mapped events
	} RegionType;

	void Init(Packet::PackClass *p);
	void Kill(Packet::PackClass *p);
	void PreInit();

	void InitMouse(Packet::PackClass *p);
	void InitKeyboard(Packet::PackClass *p);
	void SubmitCRCKeymap(Packet::PackClass *p);
	void SubmitCRCMousemap(Packet::PackClass *p);
	void GetMouseCoords(Packet::PackClass *p);
}

#endif
