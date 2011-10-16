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
    // Letter keys
    EKeyW = 0,
    EKeyA,
    EKeyS,
    EKeyD,

    EKeyR,

    // Number keys
    EKey1,
    EKey2,
    EKey3,
    EKey4,
    EKey5,
    EKey6,
    EKey7,
    EKey8,
    EKey9,
    EKey0,

    EKeyNumKeys
};

struct KeyboardState
{
    KeyboardKey keys[EKeyNumKeys];
};


UINT input();

#endif // _IVYINPUT_H_