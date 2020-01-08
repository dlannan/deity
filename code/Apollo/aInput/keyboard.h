//KeyB.h - the Header file for the KeyBoard class.
//NOTE: When using, ensure that the "No Stack" warning is off and that
// Register variables are disabled.

#ifndef __KEYB__H__
#include <dos.h>

//Includes

//Defines
#define KEY_PRESSED  1
#define KEY_RELEASED 0

#define KEYB_INT    0x09
#define KEY_BUFF    0x60
#define KEY_CONTROL 0x61
#define PIC_PORT    0x20

#define KEYB_ESC       1
#define KEYB_1         2
#define KEYB_2         3
#define KEYB_3         4
#define KEYB_4         5
#define KEYB_5         6
#define KEYB_6         7
#define KEYB_7         8
#define KEYB_8         9
#define KEYB_9        10
#define KEYB_0        11
#define KEYB_MINUS    12
#define KEYB_EQUALS   13
#define KEYB_BKSP     14
#define KEYB_TAB      15
#define KEYB_Q        16
#define KEYB_W        17
#define KEYB_E        18
#define KEYB_R        19
#define KEYB_T        20
#define KEYB_Y        21
#define KEYB_U        22
#define KEYB_I        23
#define KEYB_O        24
#define KEYB_P        25
#define KEYB_LBRACKET 26
#define KEYB_RBRACKET 27
#define KEYB_ENTER    28
#define KEYB_CRTL     29
#define KEYB_A        30
#define KEYB_S        31
#define KEYB_D        32
#define KEYB_F        33
#define KEYB_G        34
#define KEYB_H        35
#define KEYB_J        36
#define KEYB_K        37
#define KEYB_L        38
#define KEYB_SEMI     39
#define KEYB_APOS     40
#define KEYB_TILDE    41
#define KEYB_LSHIFT   42
#define KEYB_BSLASH   43
#define KEYB_Z        44
#define KEYB_X        45
#define KEYB_C        46
#define KEYB_V        47
#define KEYB_B        48
#define KEYB_N        49
#define KEYB_M        50
#define KEYB_COMMA    51
#define KEYB_PERIOD   52
#define KEYB_FSLASH   53
#define KEYB_RSHIFT   54
#define KEYB_PRTSCRN  55
#define KEYB_ALT      56
#define KEYB_SPACE    57
#define KEYB_CAPS     58
#define KEYB_F1       59
#define KEYB_F2       60
#define KEYB_F3       61
#define KEYB_F4       62
#define KEYB_F5       63
#define KEYB_F6       64
#define KEYB_F7       65
#define KEYB_F8       66
#define KEYB_F9       67
#define KEYB_F10      68
#define KEYB_F11      87
#define KEYB_F12      88
#define KEYB_NUMLOCK  69
#define KEYB_SCRLOCK  70
#define KEYB_HOME     71
#define KEYB_UP       72
#define KEYB_PGUP     73
#define KEYB_KPMINUS  74
#define KEYB_LEFT     75
#define KEYB_CENTRE   76
#define KEYB_RIGHT    77
#define KEYB_KPPLUS   78
#define KEYB_END      79
#define KEYB_DOWN     80
#define KEYB_PGDOWN   81
#define KEYB_INS      82
#define KEYB_DEL      83

//Global KeyBoard interrupt
void interrupt KeyBoardDriver(...);

//Class Definition
class KeyBoard{
 public:
  KeyBoard();
  ~KeyBoard();

  int GetKey(int key) { return Keys[key]; };
  int KeysPressed() { return NumberOfKeysPressed; };
  void FlushBuffer();
 private:

  void InstallDriver();
  void RemoveDriver();

  void interrupt (void *OldKeyBoardDriver)(...);
  friend void interrupt KeyBoardDriver(...);
  int Keys[128];
  int NumberOfKeysPressed;
};

//Global KeyBoard Instance
extern KeyBoard KeyB;

#define __KEYB__H__
#endif