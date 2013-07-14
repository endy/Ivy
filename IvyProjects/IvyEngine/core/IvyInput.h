///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2010-2011, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef _IVYINPUT_H_
#define _IVYINPUT_H_

#include "IvyTypes.h"

// Supported Keys
enum KeyboardKey
{
    // Number keys, using ASCII value for convenience
    Key_0 = 0x30,
    Key_1,
    Key_2,
    Key_3,
    Key_4,
    Key_5,
    Key_6,
    Key_7,
    Key_8,
    Key_9,

    // Letter keys, using ASCII value for convenience
    Key_A = 0x41,
    Key_B,
    Key_C,
    Key_D,
    Key_E,
    Key_F,
    Key_G,
    Key_H,
    Key_I,
    Key_J,
    Key_K,
    Key_L,
    Key_M,
    Key_N,
    Key_O,
    Key_P,
    Key_Q,
    Key_R,
    Key_S,
    Key_T,
    Key_U,
    Key_V,
    Key_W,
    Key_X,
    Key_Y,
    Key_Z,

    KeyboardKeyEnumMax
};

struct KeyboardState
{
    BOOL Pressed[KeyboardKeyEnumMax];

    KeyboardState() { memset(&Pressed, 0, sizeof(Pressed)); }
};

enum MouseButton
{
    MouseRight = 0,
    MouseLeft,

    MouseButtonEnumMax
};

struct MouseState
{
    INT x;
    INT y;

    BOOL Pressed[MouseButtonEnumMax];
};

enum IvyGamepadButtons
{
    ButtonA = 0,
    ButtonB,
    ButtonX,
    ButtonY,
    NumButtons
};

struct IvyGamepadState
{
    bool ButtonPressed[IvyGamepadButtons::NumButtons];

    FLOAT ThumbLX;
    FLOAT ThumbLY;
    FLOAT ThumbRX;
    FLOAT ThumbRY;
};

VOID IvyGetGamepadStates(IvyGamepadState* pGamepadStates, UINT numGamepads);

#endif // _IVYINPUT_H_
