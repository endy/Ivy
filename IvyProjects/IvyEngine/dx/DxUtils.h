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

#include <vector>

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

static std::vector <IDXGIAdapter1*> EnumerateAdapters()
{
    IDXGIAdapter1 * pAdapter; 
    std::vector <IDXGIAdapter1*> vAdapters; 
    IDXGIFactory1* pFactory = NULL; 

    // Create a DXGIFactory object.
    if(FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1) ,(void**)&pFactory)))
    {
        return vAdapters;
    }


    for (UINT i = 0; ; ++i)
    {
        HRESULT hr = pFactory->EnumAdapters1(i, &pAdapter);

        if (hr != DXGI_ERROR_NOT_FOUND)
        {
            DXGI_ADAPTER_DESC1 desc = {0};
            pAdapter->GetDesc1(&desc);
            // pAdapter->

            std::wcout << "Description: " << desc.Description << std::endl;
            std::cout << "Device Id: " << desc.DeviceId << std:: endl;
            std::cout << "Revision: " << desc.Revision << std:: endl;
            std::cout << "Video Memory: " << desc.DedicatedVideoMemory << std:: endl;
            std::cout << "System Memory: " << desc.DedicatedSystemMemory << std:: endl;

            vAdapters.push_back(pAdapter); 
        }
        else
        {
            break;
        }
    }

    if(pFactory)
    {
        pFactory->Release();
    }

    return vAdapters;

}

static void DxEnumDisplayDevices()
{
    UINT devId = 0;
    DISPLAY_DEVICE dev;
    dev.cb = sizeof(DISPLAY_DEVICE);
    while(EnumDisplayDevices(NULL, devId, &dev, 0))
    {
        std::cout << "Device " << devId << ": " << dev.DeviceName << std::endl;

        devId++;
    }
}

#endif // _DXUTILS_H_

