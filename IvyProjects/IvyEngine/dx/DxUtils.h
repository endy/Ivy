///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2010-2011, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DXUTILS_H_
#define _DXUTILS_H_

#include "IvyUtils.h"
#include "DxTypes.h"

#include <DXGI.h>

inline bool DxOK(HRESULT hr)
{
    return (hr >= 0);
}

inline bool DxFAIL(HRESULT hr)
{
    return (hr < 0);
}

inline void DxMessage(const CHAR* pMessage)
{
    MessageBox(NULL, pMessage, "DxMessageBox", 0);
}

inline UINT DxgiFormatElementSize(DXGI_FORMAT format)
{
    UINT bytesPerElement = 0;

    switch(format)
    {
    case DXGI_FORMAT_R32_UINT:
    case DXGI_FORMAT_R8G8B8A8_UNORM:
        bytesPerElement = 4;
        break;
    case DXGI_FORMAT_D24_UNORM_S8_UINT:
    case DXGI_FORMAT_R24G8_TYPELESS:
    case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
    case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
    case DXGI_FORMAT_BC1_UNORM:
    default:
        // Invalid or unknown format
        break;
    }

    return bytesPerElement;
}

inline BOOL DxgiHasDepth(DXGI_FORMAT format)
{
    BOOL hasDepth = FALSE;

    switch(format)
    {
    case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
    case DXGI_FORMAT_D24_UNORM_S8_UINT:
    case DXGI_FORMAT_D32_FLOAT:
    case DXGI_FORMAT_D16_UNORM:
        hasDepth = TRUE;
    default:
        break;
    }

    return hasDepth;
}

inline BOOL DxgiHasStencil(DXGI_FORMAT format)
{
    BOOL hasStencil = FALSE;

    switch(format)
    {
        case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
        case DXGI_FORMAT_D24_UNORM_S8_UINT:
            hasStencil = TRUE;
        default:
            break;
    }

    return hasStencil;
}

#endif // _DXUTILS_H_

