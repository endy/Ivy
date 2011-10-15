///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2010-2011, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "IvyInput.h"

#include <windows.h>
#include <XInput.h>
#include <iostream>

UINT input()
{
    UINT b;
    UINT max = 4;
    UINT dwResult;    
    for (DWORD i=0; i< max; i++ )
    {
        XINPUT_STATE state;
        memset(&state, 0, sizeof(XINPUT_STATE));
        //ZeroMemory( &state, sizeof(XINPUT_STATE) );

        // Simply get the state of the controller from XInput.
        dwResult = XInputGetState( i, &state );

        b = 2;

        if( dwResult == ERROR_SUCCESS )
        { 
            // Controller is connected 
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_A)
            {
                std::cout << "A Button" << std::endl;
            }

            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_B)
            {
                std::cout << "B Button" << std::endl;
            }
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_X)
            {
                std::cout << "X Button" << std::endl;
            }
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y)
            {
                std::cout << "Y Button" << std::endl;
            }

        }
        else
        {
            // Controller is not connected 
        }
    }

    return b;
}