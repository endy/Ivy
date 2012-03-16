///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2010-2011, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "DxApp.h"
#include "DxUtils.h"


const UINT DxApp::BufferCount = 1;

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxApp::DxApp
///////////////////////////////////////////////////////////////////////////////////////////////////
DxApp::DxApp(
    DxAppCreateInfo* pAppInfo)
    :
m_pWindow(NULL),
    m_pCamera(NULL),
    m_pDevice(NULL),
    m_pContext(NULL),
    m_pSwapChain(NULL),
    m_pDevice10_1(NULL),
    m_pD2DOverlay(NULL),
    m_hSharedD2DOverlay(0),
    m_pUIKeyedMutex_D2D(NULL),
    m_pUIKeyedMutex_D3D(NULL),
    m_pUIoverlay(NULL),
    m_pRenderTargetView(NULL),
    m_pDepthStencilBuffer(NULL),
    m_pDirect2dFactory(NULL),
    m_pRenderTarget(NULL),
    m_screenWidth(pAppInfo->screenWidth),
    m_screenHeight(pAppInfo->screenHeight),
    m_fovX(90 * (XM_PI/180)),
    m_fovY(90 * (XM_PI/180)),
    m_nearZ(pAppInfo->nearZ),
    m_farZ(pAppInfo->farZ),
    m_pITextBrush(NULL),
    m_pITextFormat(NULL),
    m_pIDWriteFactory(NULL)
{
    memset(&m_viewport, 0, sizeof(D3D11_VIEWPORT));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxApp::~DxApp
///////////////////////////////////////////////////////////////////////////////////////////////////
DxApp::~DxApp()
{

}

/**************************************************************************************************
DxApp::Create
**************************************************************************************************/
DxApp* DxApp::Create(
    DxAppCreateInfo* pAppInfo)
{
    DxApp* pApp = new DxApp(pAppInfo);

    if (pApp->Init() == false)
    {
        pApp->Destroy();
        pApp = NULL;
    }

    return pApp;
}

/**************************************************************************************************
DxApp::Destroy
**************************************************************************************************/
void DxApp::Destroy()
{
    if (m_pRenderTarget)
    {
        m_pRenderTarget->Release();
        m_pRenderTarget = NULL;
    }

    if (m_pDirect2dFactory)
    {
        m_pDirect2dFactory->Release();
        m_pDirect2dFactory = NULL;
    }

    ///@todo Shared surface destruction -- is this correct?
    if (m_pD2DOverlay != NULL)
    {
        m_pD2DOverlay->Release();
        m_pD2DOverlay = NULL;
    }

    if (m_pDevice10_1)
    {
        m_pDevice10_1->Release();
        m_pDevice10_1 = NULL;
    }

    if (m_pCamera != NULL)
    {
        delete m_pCamera;
        m_pCamera = NULL;
    }

    ///@todo Add 'Safe Release' calls or some such to release these components
    if (m_pRenderTargetView != NULL)
    {
        m_pRenderTargetView->Release();
    }

    if (m_pDepthStencilBuffer != NULL)
    {
        m_pDepthStencilBuffer->Destroy();
        m_pDepthStencilBuffer = NULL;
    }

    if (m_pContext != NULL)
    {
        m_pContext->Release();
    }

    if (m_pDevice != NULL)
    {
        m_pDevice->Release();
    }

    if (m_pSwapChain != NULL)
    {
        m_pSwapChain->Release();
    }

    delete this;
}


std::vector <IDXGIAdapter1*> EnumerateAdapters()
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

void DxEnumDisplayDevices()
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


/**************************************************************************************************
DxApp::Create
**************************************************************************************************/
bool DxApp::Init()
{
    EnumerateAdapters();
    DxEnumDisplayDevices();

    SystemEventDispatcher::Get()->RegisterReceiver(this);

    bool success = TRUE;

    // Setup Window
    m_pWindow = DxWindow::Create(m_screenWidth, m_screenHeight);

    // Create Swap Chain & Device
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory( &sd, sizeof( sd ) );
    sd.BufferCount = BufferCount;
    sd.BufferDesc.Width = m_screenWidth;
    sd.BufferDesc.Height = m_screenHeight;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = m_pWindow->GetHwnd();
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    D3D_FEATURE_LEVEL  FeatureLevelsRequested[2];
    FeatureLevelsRequested[0] = D3D_FEATURE_LEVEL_11_0;
    FeatureLevelsRequested[1] = D3D_FEATURE_LEVEL_10_1;
    UINT               numLevelsRequested = 2;
    D3D_FEATURE_LEVEL  FeatureLevelsSupported;

    if( DxFAIL (D3D11CreateDeviceAndSwapChain( NULL, 
        D3D_DRIVER_TYPE_HARDWARE, 
        NULL, 
        D3D11_CREATE_DEVICE_DEBUG,
        FeatureLevelsRequested, 
        numLevelsRequested, 
        D3D11_SDK_VERSION, 
        &sd, 
        &m_pSwapChain, 
        &m_pDevice, 
        &FeatureLevelsSupported,
        &m_pContext )))
    {
        success = false;
    }

    if (success)
    {
        ID3D11Texture2D* pBackBuffer = NULL;

        // Get a pointer to the back buffer
        if (DxFAIL(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer)))
        {
            success = false;
        }
        else
        {
            // Create a render-target view
            m_pDevice->CreateRenderTargetView(pBackBuffer,
                NULL,
                &m_pRenderTargetView );
            pBackBuffer->Release();
        }
    }


    HRESULT hr = S_OK;

    if (success)
    {
        IDXGIFactory1* pDxgiFactory = NULL;
        hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&pDxgiFactory);

        IDXGIAdapter1* pAdapter = NULL;
        pDxgiFactory->EnumAdapters1(0, &pAdapter);

        // Init D3D10.1 device for D2D interop
        if (DxFAIL(D3D10CreateDevice1(pAdapter,
            D3D10_DRIVER_TYPE_HARDWARE,
            NULL,
            D3D10_CREATE_DEVICE_BGRA_SUPPORT   |
            D3D10_CREATE_DEVICE_SINGLETHREADED,    // Remove once MT is added
            D3D10_FEATURE_LEVEL_10_1,
            D3D10_1_SDK_VERSION,
            &m_pDevice10_1)))
        {
            success = false;
        }
        else
        {
            // Create Sync Shared Surface using Direct3D10.1 Device 1.
            D3D10_TEXTURE2D_DESC desc;
            ZeroMemory( &desc, sizeof(desc) );
            desc.Width = m_screenWidth;
            desc.Height = m_screenHeight;
            desc.MipLevels = 1;
            desc.ArraySize = 1;
            // must match swapchain format in order to CopySubresourceRegion.
            desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            desc.SampleDesc.Count = 1;
            desc.Usage = D3D10_USAGE_DEFAULT;
            // creates 2D texture as a Synchronized Shared Surface.
            desc.MiscFlags = D3D10_RESOURCE_MISC_SHARED_KEYEDMUTEX;
            desc.BindFlags = D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;

            HRESULT hr = m_pDevice10_1->CreateTexture2D( &desc, NULL, &m_pD2DOverlay);

            // QI IDXGIResource interface to synchronized shared surface.
            IDXGIResource* pDXGIResource = NULL;
            m_pD2DOverlay->QueryInterface(__uuidof(IDXGIResource), (LPVOID*) &pDXGIResource);

            // obtain handle to IDXGIResource object.
            pDXGIResource->GetSharedHandle(&m_hSharedD2DOverlay);
            pDXGIResource->Release();

            if (m_hSharedD2DOverlay == 0)
            {
                success = false;
            }
            else if (DxFAIL(m_pD2DOverlay->QueryInterface(__uuidof(IDXGIKeyedMutex),
                (LPVOID*)&m_pUIKeyedMutex_D2D)))
            {
                success = false;
            }

        }

    }

    if (success)
    {
        // get surface for D3D11
        // Obtain handle to Sync Shared Surface created by Direct3D10.1 Device 1.
        hr = m_pDevice->OpenSharedResource(m_hSharedD2DOverlay,
            __uuidof(ID3D11Texture2D),
            (LPVOID*) &m_pUIoverlay);

        if (DxFAIL(hr))
        {
            success = false;
        }

        if (DxFAIL(m_pUIoverlay->QueryInterface(__uuidof(IDXGIKeyedMutex),
            (LPVOID*) &m_pUIKeyedMutex_D3D)))
        {
            success = false;
        }
    }


    if (success)
    {
        // Direct2D

        D2D1_FACTORY_OPTIONS factoryOptions;
        factoryOptions.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;

        HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, factoryOptions, &m_pDirect2dFactory);

        FLOAT dpiX;
        FLOAT dpiY;
        m_pDirect2dFactory->GetDesktopDpi(&dpiX, &dpiY);

        D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
            D2D1_RENDER_TARGET_TYPE_DEFAULT,
            D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
            dpiX,
            dpiY);

        IDXGISurface* pSurface = NULL;
        m_pD2DOverlay->QueryInterface(__uuidof(IDXGISurface), (void**)&pSurface);

        // Create a render-target view
        hr = m_pDirect2dFactory->CreateDxgiSurfaceRenderTarget(
            pSurface,
            &props,
            &m_pRenderTarget);

    }

    if (success)
    {
        DxTextureCreateInfo depthStencilCreateInfo;
        memset(&depthStencilCreateInfo, 0, sizeof(DxTextureCreateInfo));
        depthStencilCreateInfo.flags.DepthStencil = TRUE;
        depthStencilCreateInfo.flags.ShaderInput = TRUE;
        depthStencilCreateInfo.format = DXGI_FORMAT_D32_FLOAT;  //@todo texture doesnt read format currently
        depthStencilCreateInfo.width = m_screenWidth;
        depthStencilCreateInfo.height = m_screenHeight;

        m_pDepthStencilBuffer = DxTexture::Create(m_pDevice, &depthStencilCreateInfo);
    }

    // Setup viewport
    m_viewport.Width = static_cast<FLOAT>(m_screenWidth);
    m_viewport.Height = static_cast<FLOAT>(m_screenHeight);
    m_viewport.MinDepth = 0.0f;
    m_viewport.MaxDepth = 1.0f;
    m_viewport.TopLeftX = 0;
    m_viewport.TopLeftY = 0;

    IvyPerspectiveCameraInfo cameraInfo;
    memset(&cameraInfo, 0, sizeof(IvyPerspectiveCameraInfo));
    cameraInfo.viewport.left = 0;
    cameraInfo.viewport.top = 0;
    cameraInfo.viewport.right = m_screenWidth;
    cameraInfo.viewport.left = m_screenHeight;
    cameraInfo.nearZ = m_nearZ;
    cameraInfo.farZ = m_farZ;
    cameraInfo.fovX = m_fovX;
    cameraInfo.fovY = m_fovY;

    m_pCamera = new IvyPerspective(&cameraInfo);

    // DWrite

    DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(IDWriteFactory),
        reinterpret_cast<IUnknown**>(&m_pIDWriteFactory));

    m_pIDWriteFactory->CreateTextFormat(
        L"Arial", 
        NULL,
        DWRITE_FONT_WEIGHT_NORMAL, 
        DWRITE_FONT_STYLE_NORMAL, 
        DWRITE_FONT_STRETCH_NORMAL, 
        10.0f * 96.0f/72.0f, 
        L"en-US", 
        &m_pITextFormat);

    m_pRenderTarget->CreateSolidColorBrush(
        D2D1:: ColorF(D2D1::ColorF::Red),
        &m_pITextBrush);

    return success;
}

/**************************************************************************************************
DxApp::Run
**************************************************************************************************/
void DxApp::Run()
{

}

void DxApp::ReceiveEvent(
    const Event* pEvent)
{
    switch (pEvent->GetType())
    {
    case EventTypeWindowResize:
        m_pWindow->Resize();
        UpdateSwapChain();
        break;
    case EventTypeMouseMove:
        UpdateMousePosition();
        break;
    default:
        // Do nothing
        break;
    }
}

void DxApp::UpdateMousePosition()
{
    POINT point;

    GetCursorPos(&point);
    ScreenToClient(m_pWindow->GetHwnd(), &point);

    m_mousePos.x = static_cast<FLOAT>(point.x);
    m_mousePos.y = static_cast<FLOAT>(point.y);
}

void DxApp::UpdateSwapChain()
{
    Rect viewportRect = m_pWindow->GetDrawableArea();
    m_pCamera->UpdateViewport(viewportRect);

    UINT width = viewportRect.right - viewportRect.left;
    UINT height = viewportRect.bottom - viewportRect.top;

    // Setup the viewport
    m_viewport.Width = static_cast<FLOAT>(width);
    m_viewport.Height = static_cast<FLOAT>(height);
    m_viewport.MinDepth = 0.0f;
    m_viewport.MaxDepth = 1.0f;
    m_viewport.TopLeftX = 0;
    m_viewport.TopLeftY = 0;

    m_pContext->OMSetRenderTargets(0, 0, 0);

    // Release all outstanding references to the swap chain's buffers.
    m_pRenderTargetView->Release();

    HRESULT hr;
    hr = m_pSwapChain->ResizeBuffers(BufferCount,
        width,
        height,
        DXGI_FORMAT_R8G8B8A8_UNORM,
        0);

    Assert(!DxFAIL(hr));

    // Get buffer and create a render-target-view.
    ID3D11Texture2D* pBackBuffer;
    hr = m_pSwapChain->GetBuffer(0,
        __uuidof( ID3D11Texture2D),
        (void**) &pBackBuffer );
    // Perform error handling here!

    hr = m_pDevice->CreateRenderTargetView(pBackBuffer,
        NULL,
        &m_pRenderTargetView);
    // Perform error handling here!
    pBackBuffer->Release();

    if (m_pDepthStencilBuffer != NULL)
    {
        m_pDepthStencilBuffer->Destroy();
        m_pDepthStencilBuffer = NULL;

        DxTextureCreateInfo depthStencilCreateInfo;
        memset(&depthStencilCreateInfo, 0, sizeof(DxTextureCreateInfo));
        depthStencilCreateInfo.flags.DepthStencil = TRUE;
        depthStencilCreateInfo.flags.ShaderInput = TRUE;
        depthStencilCreateInfo.format = DXGI_FORMAT_D32_FLOAT;	//@todo texture doesnt read format currently
        depthStencilCreateInfo.width = width;
        depthStencilCreateInfo.height = height;

        m_pDepthStencilBuffer = DxTexture::Create(m_pDevice, &depthStencilCreateInfo);
    }
}

void DxApp::BeginFrame()
{
    m_framerateTracker.BeginFrame();
}

void DxApp::EndFrame()
{
    m_framerateTracker.EndFrame();
}

