///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2010-2011, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "IvyInput.h"
#include "IvyUtils.h"

#ifdef _WIN32
#include <XInput.h>
#endif // _WIN32

#include <iostream>

#ifndef _WIN32
#define STUB_INPUT 1
#endif // _WIN32

///////////////////////////////////////////////////////////////////////////////////////////////////
/// IvyGetGamepadState
///
/// @brief
///     
/// @return
///     N/A
///////////////////////////////////////////////////////////////////////////////////////////////////
VOID IvyGetGamepadStates(
    IvyGamepadState* pGamepadStates,
    UINT numGamepads)
{
    static const UINT MaxGamepadsSupported = 4;
    static const SHORT AnalogDeadZone = 10000;
    static const FLOAT MaxSignedShort = (FLOAT) 0x7FFF;

    UINT gamepadQueryCount = IVY_MIN(numGamepads, MaxGamepadsSupported);

#ifndef STUB_INPUT
  
    for (DWORD i=0; i < gamepadQueryCount; i++ )
    {
        XINPUT_STATE state;
        memset(&state, 0, sizeof(XINPUT_STATE));
        //ZeroMemory( &state, sizeof(XINPUT_STATE) );

        // Simply get the state of the controller from XInput.
        DWORD dwResult = XInputGetState( i, &state );
        
        if( dwResult == ERROR_SUCCESS )
        { 
            // Controller is connected 
            pGamepadStates[i].ButtonPressed[IvyGamepadButtons::ButtonA] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_A);
            pGamepadStates[i].ButtonPressed[IvyGamepadButtons::ButtonB] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_B);
            pGamepadStates[i].ButtonPressed[IvyGamepadButtons::ButtonX] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_X);
            pGamepadStates[i].ButtonPressed[IvyGamepadButtons::ButtonY] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y);

            if (!(-AnalogDeadZone < state.Gamepad.sThumbLX && state.Gamepad.sThumbLX < AnalogDeadZone))
            {
                pGamepadStates[i].ThumbLX = state.Gamepad.sThumbLX / MaxSignedShort;
            }

            if (!(-AnalogDeadZone < state.Gamepad.sThumbLY && state.Gamepad.sThumbLY < AnalogDeadZone))
            {
                pGamepadStates[i].ThumbLY = state.Gamepad.sThumbLY / MaxSignedShort;
            }

            if (!(-AnalogDeadZone < state.Gamepad.sThumbRX && state.Gamepad.sThumbRX < AnalogDeadZone))
            {
                pGamepadStates[i].ThumbRX = state.Gamepad.sThumbRX / MaxSignedShort;
            }

            if (!(-AnalogDeadZone < state.Gamepad.sThumbRY && state.Gamepad.sThumbRY < AnalogDeadZone))
            {
                pGamepadStates[i].ThumbRY = state.Gamepad.sThumbRY / MaxSignedShort;
            }
        }
        else
        {
            //  no controller connected
            ///@todo Signal Error
        }
    }

#endif // STUB_INPUT

   
}
