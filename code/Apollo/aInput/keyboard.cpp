//Includes
#include "keyboard.h"

//KeyBoard Global
KeyBoard KeyB;

//Class Member Function Definitions
KeyBoard::KeyBoard()
{
 for(int j = 0; j < 128; j++)
  Keys[j] = 0;
 NumberOfKeysPressed = 0;
 InstallDriver();
}

KeyBoard::~KeyBoard()
{
 RemoveDriver();
}

void KeyBoard::InstallDriver()
{
 OldKeyBoardDriver = _dos_getvect(KEYB_INT);
 _dos_setvect(KEYB_INT, KeyBoardDriver);
}

void KeyBoard::RemoveDriver()
{
 _dos_setvect(KEYB_INT, OldKeyBoardDriver);
}

void KeyBoard::FlushBuffer()
{
 for(int j = 0; j < 128; j++)
  {
   Keys[j] = 0;
  }
 NumberOfKeysPressed = 0;
}

void interrupt KeyBoardDriver(...)
{
 int ScanCode = 0;

 asm{
  sti
  in al, KEY_BUFF
  xor ah, ah
  mov ScanCode, ax
  in al, KEY_CONTROL
  or al, 82h
  out KEY_CONTROL, al
  and al, 7fh
  out KEY_CONTROL, al
  mov al, 20h
  out PIC_PORT, al
 }

 if(ScanCode < 128)       //Key has been pressed
  {
   if(KeyB.Keys[ScanCode] != KEY_PRESSED)
    {
     KeyB.NumberOfKeysPressed++;
     KeyB.Keys[ScanCode] = KEY_PRESSED;
    }
  }
 else                         //Key has been released
  {
   if(KeyB.Keys[ScanCode - 128]){
    KeyB.NumberOfKeysPressed--;
    KeyB.Keys[ScanCode - 128] = KEY_RELEASED;
   }
  }
}