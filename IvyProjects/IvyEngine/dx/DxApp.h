///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2010-2011, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DXAPP_H_
#define _DXAPP_H_

///@todo: Support multiple adapters
///@todo: Support framerate capping
///@todo: Create generic config manager

#include "IvyApp.h"
#include "DxTexture.h"

#include <D3DX11.h>
#include <D2D1.h>
#include <DWrite.h>

#include <vector>

struct DxAppCreateInfo
{
    UINT screenWidth;
    UINT screenHeight;
    FLOAT nearZ;
    FLOAT farZ;
};

class DxApp :
    public IvyApp
{
public:
    static DxApp* Create(DxAppCreateInfo* pAppInfo);

    virtual void Destroy();

    virtual void ReceiveEvent(const Event* pEvent);

protected:
    DxApp(DxAppCreateInfo* pAppInfo);
    virtual ~DxApp();

    virtual bool Init();

    void UpdateSwapChain();

    // D3D
    ID3D11Device* m_pDevice;
    ID3D11DeviceContext* m_pContext;
    IDXGISwapChain* m_pSwapChain;


    // D2D Shared Surface
    ID3D10Device1* m_pDevice10_1;
    ID3D10Texture2D* m_pD2DOverlay;

    HANDLE m_hSharedD2DOverlay;

    IDXGIKeyedMutex* m_pUIKeyedMutex_D2D;
    IDXGIKeyedMutex* m_pUIKeyedMutex_D3D;
    ID3D11Texture2D* m_pUIoverlay;


    ///@todo Refactor into a group
    ID3D11RenderTargetView* m_pRenderTargetView;
    DxTexture* m_pDepthStencilBuffer;

    ///@todo Create better mapping between viewport & camera
    D3D11_VIEWPORT m_viewport;

    // D2D
    ID2D1Factory* m_pDirect2dFactory;
    ID2D1RenderTarget* m_pRenderTarget;

    ID2D1SolidColorBrush* m_pITextBrush;

    // DWrite
    IDWriteTextFormat* m_pITextFormat;
    IDWriteFactory* m_pIDWriteFactory;

private:
    DxApp(const DxApp& copy);               // Disallow copy constructor
    DxApp& operator=(const DxApp& copy);    // Disallow assignment operator
};

#endif // _DXAPP_H_
