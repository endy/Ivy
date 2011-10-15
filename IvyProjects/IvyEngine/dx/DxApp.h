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

#include "IvyObject.h"
#include "DxWindow.h"
#include "DxCamera.h"
#include "DxTexture.h"

#include "SystemEventDispatcher.h"
#include "IvyInput.h"
#include "IvyPerf.h"

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
    public IvyObject,
    public IEventReceiver
{
public:
    static DxApp* Create(DxAppCreateInfo* pAppInfo);

    virtual void Destroy();

    virtual void Run();

    virtual void ReceiveEvent(const Event* pEvent);

    Point2 GetMousePos() { return m_mousePos; }

protected:
    DxApp(DxAppCreateInfo* pAppInfo);
    virtual ~DxApp();

    virtual bool Init();

    void UpdateSwapChain();
    void UpdateMousePosition();

    void BeginFrame();
    void EndFrame();

    DxWindow* m_pWindow;
    DxCamera* m_pCamera;

    Point2 m_mousePos;

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
    DxDepthTexture* m_pDepthStencilBuffer;

    ///@todo Create better mapping between viewport & camera
    D3D11_VIEWPORT m_viewport;

    // D2D
    ID2D1Factory* m_pDirect2dFactory;
    ID2D1RenderTarget* m_pRenderTarget;

    ID2D1SolidColorBrush* m_pITextBrush;

    // DWrite
    IDWriteTextFormat* m_pITextFormat;
    IDWriteFactory* m_pIDWriteFactory;

    FLOAT m_fovX;           // FOV X
    FLOAT m_fovY;           // FOV Y
    FLOAT m_nearZ;          // near plane
    FLOAT m_farZ;           // far plane
    UINT  m_screenWidth;    // viewport width
    UINT  m_screenHeight;   // viewport height

    static const UINT BufferCount;

private:
    DxApp(const DxApp& copy);               // Disallow copy constructor
    DxApp& operator=(const DxApp& copy);    // Disallow assignment operator

    FramerateTracker m_framerateTracker;

    KeyboardState m_keys;
};

#endif // _DXAPP_H_
