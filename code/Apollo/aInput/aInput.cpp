
// Make sure you include headers for modules you intend to use. They may
// have types or structures you may need to cast pointers to.

// MS SHITTY TEMPLATE SUPPORT!
#pragma warning(disable:4786)

// Std template gear
#include <map>

#include <windows.h>
#include <winuser.h>

#include <dinput.h>

#include "..\\..\\System\System.h"
#include "aInput.h"

using namespace std;

ApolloClass	apollo;

namespace aInput
{
	long	time;
	long	lastupdatetime;
	long	InputUpdate;		// update period in ms

	HWND					win;	
	HINSTANCE				hInstance;
	LPDIRECTINPUT8			directInput;

	int						buttons;
	unsigned int			prevbuttons;

	// Keyboard input
	LPDIRECTINPUTDEVICE8	directInputKb;
	HHOOK					KeyHOOK;		
	KeymapType				keymap;			// The mappings for keys are done here

	map<int,int>			keyDown;		// Key id, key state (pressed/released)

	// Mouse input
	LPDIRECTINPUTDEVICE8	directInputMouse;
	HHOOK					MouseHOOK;
	MouseType				mousemap;		// Mapping for buttons and movements here

	// Function prototypes
	int ProcessKeyboardInputs(int wParam, int lParam);
	int ProcessMouseInputs(int wParam, int lParam);


	// These will more than likely become defunct.
	int				LastKeyDown();
	unsigned char	GetKeymapKey(int);
	bool			GetKeyState(int);

	int		GetButtons();
	bool	GetMiddleButtonDown();
	bool	GetRightButtonDown();
	bool	GetLeftButtonDown();
	long	GetMouseY();
	long	GetMouseX();
	void	ProcessInputs(void);
	void	CheckKeysDown();

	void Broadcast(Packet::PackClass *input);

	void CheckErr(HRESULT err)
	{
		switch(err)
		{
			case DI_BUFFEROVERFLOW: ASSERT (1,"BUFFOVER"); break;
			case DIERR_INPUTLOST: ASSERT (1,"INPUTLOST"); 
					err = directInputKb->Acquire();	// This is frikkin nasty!
					CheckErr(err);
					break;
			case DIERR_OTHERAPPHASPRIO: ASSERT (1,"OTHER APP HAS PRIORITY"); break;
			case DIERR_INVALIDPARAM: ASSERT (1,"INVALIDPARAM"); break;
			case DIERR_NOTACQUIRED: ASSERT (1,"NOTACQUIRED"); break;
			case DIERR_NOTBUFFERED: ASSERT (1,"NOTBUFFERED"); break;
			case DIERR_NOTINITIALIZED: ASSERT (1,"NOTINITIALIZED"); break;
		}
	}

	// Update the input informaton - check the DInput event buffer
	void Update(Packet::PackClass *in)
	{
		DIDEVICEOBJECTDATA eventKb; 
		DIDEVICEOBJECTDATA eventMouse; 
		DWORD noEventsMouse;		// 1 event at a time
		DWORD noEventsKb;				// 1 event at a time

		HRESULT err;

		noEventsMouse = 1;
		noEventsKb = 1;

		// Poll the device
//		directInputKb->Poll();

		// take data once from each device, so you can have concurrent device inputs.
		// ie joystick + mouse + keyboard, without order.

		CheckKeysDown();

		// Get all the events in the buffer
		while( (noEventsKb > 0) || (noEventsMouse > 0) )
		{
			// Get one from keyboard
			err = directInputKb->GetDeviceData( sizeof(DIDEVICEOBJECTDATA), &eventKb, &noEventsKb, 0); 

			CheckErr(err);

			if((noEventsKb > 0) && (err == DI_OK))
			{
				ProcessKeyboardInputs( eventKb.dwOfs, eventKb.dwData);
//				DebugLog("Input: Keys processed.\n");
//				PRINTF("Input: Keys processed:%d",event.dwOfs);
			}
		
			// Get one from  mouse
			err = directInputMouse->GetDeviceData( sizeof(DIDEVICEOBJECTDATA), &eventMouse, &noEventsMouse, 0); 

			CheckErr(err);

			if((noEventsMouse > 0) && (err == DI_OK))
			{
				ProcessMouseInputs( eventMouse.dwOfs, eventMouse.dwData);
//				DebugLog("Input: Mouse processed.\n");
			}

			// check the 'other' devices and remove one event if available (ie joystick)

		}
	}

	int ProcessMouseInputs(int wParam, int lParam)
	{
		return -1;
	}

	int ProcessKeyboardInputs(int wParam, int lParam)
	{
		static Packet::PackClass	packet;
		int error = -1;

		// We will have a mapper here! Key will be mapped to crc dests.. :-)
		// A module wishing to recieve an input mus submit a mapping.
		// We will have a multi mapping set of up to maybe 10 or 20 
		// coincidental maps. So one key can activate multiple events.

		if(lParam & 0x80)
		// WM_KEYDOWN recieved
		{
			if(keymap.m[wParam].event & 0x001)
			{
				packet.Clear();
				packet.destcrc.SetFuncId(keymap.m[wParam].crcfunc);
				packet.destcrc.SetModId(keymap.m[wParam].crcmod);
				packet.srccrc = "input.keypressed";
				apollo.sendpacketfunc.proc(&packet);
				error = 0;
			}

			// Add keydown to the list
			keyDown[wParam] = 0xf52b292c; // keypressed
		}
		else
		// WM_KEYUP recieved
		{
			if(keymap.m[wParam].event & 0x100)
			{
				packet.Clear();
				packet.destcrc.SetFuncId(keymap.m[wParam].crcfunc);
				packet.destcrc.SetModId(keymap.m[wParam].crcmod);
				packet.srccrc = "input.keyreleased";
				apollo.sendpacketfunc.proc(&packet);
				error = 0;
			}

			// Set keydown to up
			keyDown[wParam] = 0x79a3ea46; // keyreleased
		}
		return error;
	}


	void CheckKeysDown()
	{
		static Packet::PackClass	packet;
		int wParam = 0;
		int kdSize = 0;
		map<int,int>::iterator i = keyDown.begin();

		while(i!=keyDown.end())
		{
			wParam = i->first;
			if(keyDown[wParam] == 0xf52b292c) // keypressed
			{
				if(keymap.m[wParam].event & 0x010)
				{
					packet.Clear();
					packet.destcrc.SetFuncId(keymap.m[wParam].crcfunc);
					packet.destcrc.SetModId(keymap.m[wParam].crcmod);
					packet.srccrc = "input.keydown";
					apollo.sendpacketfunc.proc(&packet);
					kdSize++;
				}
			}
			i++;
		}

		// Clear the keyDown map if there are no keys down!
		if(kdSize==0)
		{
			keyDown.clear();
		}
	}

	// *******************************************************************************
	// Keyboard stuff
	// *******************************************************************************

	bool GetKeyState(int testKey)
	{
		return false;
	}

	unsigned char GetKeymapKey(int key)
	{
		return 0;
	}

	void KeyboardInit()
	{
		// If these fail then how the hell did you start your computer 8-)
		// Check to see if direct input has been created
		if(!directInput) 
			DirectInput8Create( hInstance,DIRECTINPUT_VERSION, IID_IDirectInput8,(void **)&directInput, NULL );

		directInput->CreateDevice( GUID_SysKeyboard, &directInputKb, NULL );
		directInputKb->SetDataFormat( &c_dfDIKeyboard );
		directInputKb->SetCooperativeLevel( win,DISCL_NONEXCLUSIVE | DISCL_BACKGROUND );

        DIPROPDWORD dipdw;

        dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
        dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        dipdw.diph.dwObj        = 0;
        dipdw.diph.dwHow        = DIPH_DEVICE;
        dipdw.dwData            = 8; // Arbitary buffer size

        directInputKb->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph );
		directInputKb->Acquire();
	}

	// *******************************************************************************
	// Mouse stuff
	// *******************************************************************************

	long GetMouseX()
	{
		return mousemap.x;
	}

	long GetMouseY()
	{
		return mousemap.y;
	}

	bool GetLeftButtonDown()
	{
		return	mousemap.buttons&&0x01;
	}

	bool GetRightButtonDown()
	{
		return	mousemap.buttons&&0x04;
	}

	bool GetMiddleButtonDown()
	{
		return mousemap.buttons&&0x02;
	}

	int GetButtons()
	{
		return mousemap.buttons;
	}

	void MouseInit()
	{
		POINT	mousepos;

		// Check to see if direct input has been created
		if(!directInput)
			DirectInput8Create( hInstance,DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&directInput, NULL );

		directInput->CreateDevice( GUID_SysMouse, &directInputMouse, NULL );
		directInputMouse->SetDataFormat( &c_dfDIMouse );
		directInputMouse->SetCooperativeLevel( win, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE );

		GetCursorPos(&mousepos);
		mousemap.x=mousepos.x;
		mousemap.y=mousepos.y;

        DIPROPDWORD dipdw;

        dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
        dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        dipdw.diph.dwObj        = 0;
        dipdw.diph.dwHow        = DIPH_DEVICE;
        dipdw.dwData            = 100; // Arbitary buffer size

        directInputMouse->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph );
		directInputMouse->Acquire();
	}

	// *******************************************************************************
	// Module stuff
	// *******************************************************************************

	void PreInit()
	{
		// Without this there is no way to Init the module, since the recieve will fail.
		apollo.SubmitFunction(aInput::Init,"Init");
		apollo.SubmitFunction(aInput::Kill,"Kill");

		apollo.SubmitFunction(aInput::Broadcast,"Broadcast");

		apollo.SubmitFunction(aInput::SubmitCRCKeymap,"SubmitCRCKeymap");
		apollo.SubmitFunction(aInput::SubmitCRCMousemap,"SubmitCRCMousemap");
		apollo.SubmitFunction(aInput::GetMouseCoords,"GetMouseCoords");
	}

	// This is the recieving of a zeus broadcast
	void Broadcast(Packet::PackClass *input)
	{
		switch(input->GetData(D_UINT))
		{
		case 0xcda2dfc9: // window.handle
			win = (HWND)input->GetData(D_ULONG);
		}
	}

	// Use this to submit your calls - external submission not available yet
	void Init(Packet::PackClass *p)
	{
		time = 0;

		InputUpdate = 5;
		lastupdatetime = 0;

		buttons = 0;
		prevbuttons = 0;

		{
		Packet("Input","Zeus.GetInstance");
		Psend;

		hInstance = (HINSTANCE)packet.GetData(D_ULONG);
		}

		if(hInstance)
		{
			MouseInit();
			KeyboardInit();

			{
			Packet("Input", "Zeus.PolledFunc");
			Padd(unsigned long)&Update;
			Padd(unsigned long)InputUpdate;
			Psend
			}
		}
	}

	// Use this to cleanup anything nasty at the end of the module runtime
	void Kill(Packet::PackClass *p)
	{
		// If the keyboard device is still acquired the release it.
		if( directInputKb ) 
		{
			directInputKb->Unacquire();
			directInputKb->Release();
			directInputKb = 0;
		}

		// If the mouse device is still acquired the release it.
		if( directInputMouse ) 
		{
			directInputMouse->Unacquire();
			directInputMouse->Release();
			directInputMouse = 0;
		}

		// If the directx interface is still acquired the release it.
		if( directInput ) 
		{
			directInput->Release();
			directInput = 0;
		}
	}

	void SubmitCRCKeymap(Packet::PackClass *input)
	{
		// This is a list of indexes.. followed by function ids, followed by module ids..
		// You can submit up to about 50 odd indexes.. if more are needed then
		// A second submit can be called.
//		SubmitCRCKeyMap((unsigned int *)(input->GetDataUL(0)));
		if(input->Size()<=0) return;

		unsigned int funcid;
		unsigned int modid;
		unsigned int eventid;
		unsigned int index;

		for(int i = 1; i<input->Params(); i+=4)
		{
			index = input->GetData(D_UINT);
			funcid = input->GetData(D_UINT); 
			modid = input->GetData(D_UINT);
			eventid = input->GetData(D_UINT);

			// If the crckeymap is empty then you can use it
			if(!keymap.m[index].crcfunc && !keymap.m[index].crcmod)
			{
				keymap.m[index].crcfunc = funcid;
				keymap.m[index].crcmod = modid;
				keymap.m[index].event = eventid;
			}
		}
	}

	void SubmitCRCMousemap(Packet::PackClass *input)
	{
		// This is a list of indexes.. followed by function ids, followed by module ids..
		// You can submit up to about 50 odd indexes.. if more are needed then
		// A second submit can be called.
//		SubmitCRCMouseMap((unsigned int *)(input->GetDataUL(0)));

		// This is very diff to keyboard one, although it looks the same
		// Mouse map only has 5 indexes - 3 buttons, 2 axis movements
		unsigned int funcid;
		unsigned int modid;
		unsigned int index;

		for(int i = 1; i<input->Params(); i+=3)
		{
			index = input->GetData(D_UINT);
			funcid = input->GetData(D_UINT);
			modid = input->GetData(D_UINT);

			// If the crckeymap is empty then make one and fill it
			if(mousemap.e.find(index)==mousemap.e.end())
			{
				mousemap.e[index] = new CRCmapType;
			}

			// Other wise overwrite the one thats there :-) For the time being
			mousemap.e[index]->crcfunc = funcid;
			mousemap.e[index]->crcmod = modid;
		}
	}

	void GetMouseCoords(Packet::PackClass *input)
	{
		static Packet::PackClass	packet;
		packet.Clear();
		packet.destcrc = input->srccrc;
		packet.srccrc << apollo.id.String();
		Padd mousemap.x;
		Padd mousemap.y;
		Psend;
	}
}
