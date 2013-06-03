///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2012, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "DxUI.h"
#include "DxUtils.h"
#include "IvyApp.h"
#include "IvyConfig.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxUI::DxUI
///
/// @brief
///     Constructor
/// @return
///     N/A
///////////////////////////////////////////////////////////////////////////////////////////////////
DxUI::DxUI()
    :
    m_pDirect2dFactory(NULL),
    m_pDevice10_1(NULL),
    m_pRenderTarget(NULL),
    m_pITextForeground(NULL),
    m_pITextBackground(NULL),
    m_pITextFormat(NULL),
    m_pIDWriteFactory(NULL),
    m_pD2DOverlay(NULL),
    m_hSharedD2DOverlay(0),
    m_pUIKeyedMutex_D2D(NULL),
    m_AcquireKey(0),
    m_ReleaseKey(1),
    m_Timeout(5)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxUI::~DxUI
///
/// @brief
///     Destructor
/// @return
///     N/A
///////////////////////////////////////////////////////////////////////////////////////////////////
DxUI::~DxUI()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxUI::Create
///
/// @brief
///     Create an instance of the DxUI object
/// @return
///     Pointer to new instance of DxUI else NULL for failure
///////////////////////////////////////////////////////////////////////////////////////////////////
DxUI* DxUI::Create()
{
    DxUI* pNewDxUI = new DxUI();

    if (pNewDxUI->Init() == false)
    {
        pNewDxUI->Destroy();
        pNewDxUI = NULL;
    }

    return pNewDxUI;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxUI::Destroy
///
/// @brief
///     Destroys DxUI object after cleaning up its resources
/// @return
///     N/A
///////////////////////////////////////////////////////////////////////////////////////////////////
void DxUI::Destroy()
{
    m_pUIKeyedMutex_D2D->Release();

    if (m_pITextForeground)
    {
        m_pITextForeground->Release();
        m_pITextForeground = NULL;
    }

    if (m_pITextBackground)
    {
        m_pITextBackground->Release();
        m_pITextBackground = NULL;
    }

    if (m_pITextFormat)
    {
        m_pITextFormat->Release();
        m_pITextFormat = NULL;
    }

    if (m_pIDWriteFactory)
    {
        m_pIDWriteFactory->Release();
        m_pIDWriteFactory = NULL;
    }

    if (m_pRenderTarget)
    {
        m_pRenderTarget->Release();
        m_pRenderTarget = NULL;
    }

    if (m_pDevice10_1)
    {
        m_pDevice10_1->Release();
        m_pDevice10_1 = NULL;
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

    delete this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxUI::Init
///
/// @brief
///     Initializes a DxUI object
/// @return
///     N/A
///////////////////////////////////////////////////////////////////////////////////////////////////
bool DxUI::Init()
{
    HRESULT hr = S_OK;
    bool success = true;

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
            desc.Width = *IvyApp::QueryConfig("uScreenWidth")->puValue;
            desc.Height = *IvyApp::QueryConfig("uScreenWidth")->puValue;
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
            D2D1:: ColorF(D2D1::ColorF::Black),
            &m_pITextForeground);

        m_pRenderTarget->CreateSolidColorBrush(
            D2D1:: ColorF(D2D1::ColorF::Gray),
            &m_pITextBackground);
    }

    return success;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxUI::CreateSharedTextureOverlay
///
/// @brief
///     
/// @return
///     Pointer to a ID3D11Texture2D struct for the UI shared surface
///////////////////////////////////////////////////////////////////////////////////////////////////
ID3D11Texture2D* DxUI::CreateSharedTextureOverlay(
    ID3D11Device* pDevice)   ///< D3D Device
{
    bool success = true;
    HRESULT hr = S_OK;

    ID3D11Texture2D* pTexture = NULL;

    if (success)
    {
        // get surface for D3D11
        // Obtain handle to Sync Shared Surface created by Direct3D10.1 Device 1.
        hr = pDevice->OpenSharedResource(m_hSharedD2DOverlay,
            __uuidof(ID3D11Texture2D),
            (LPVOID*) &pTexture);

        if (DxFAIL(hr))
        {
            success = false;
        }
    }

    return pTexture;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxUI::Begin
///
/// @brief
///     
/// @return
///     N/A
///////////////////////////////////////////////////////////////////////////////////////////////////
void DxUI::Begin()
{
    
    HRESULT hr = m_pUIKeyedMutex_D2D->AcquireSync(m_AcquireKey, m_Timeout);

    // (hr == WAIT_OBJECT_0)

    m_pRenderTarget->BeginDraw();
    D2D1_COLOR_F clearColor = {0};
    m_pRenderTarget->Clear(clearColor);
    m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
    //*/
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxUI::End
///
/// @brief
///     
/// @return
///     N/A
///////////////////////////////////////////////////////////////////////////////////////////////////
void DxUI::End()
{
    m_pRenderTarget->EndDraw();

    m_pUIKeyedMutex_D2D->ReleaseSync(m_ReleaseKey);

}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxUI::DrawRect
///
/// @brief
///     
/// @return
///     N/A
///////////////////////////////////////////////////////////////////////////////////////////////////
void DxUI::RenderRect(
    FLOAT left,
    FLOAT top,
    FLOAT right,
    FLOAT bottom)
{
    D2D1_RECT_F rect = { left, top, right, bottom};

    m_pRenderTarget->DrawRectangle(&rect, m_pITextForeground, 3.0f);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxUI::RenderText
///
/// @brief
///     
/// @return
///     N/A
///////////////////////////////////////////////////////////////////////////////////////////////////
void DxUI::RenderText(std::wstring s) //wchar_t* text, textSize)
{
    D2D1_RECT_F layoutRect = D2D1::RectF(0.f, 0.f, 400.f, 60);

    m_pRenderTarget->FillRectangle(&layoutRect, m_pITextBackground);

    m_pRenderTarget->DrawText(
        s.c_str(),
        s.length(),
        m_pITextFormat,
        layoutRect, 
        m_pITextForeground);

    ///@TODO Break this out into generic RenderLine(s) function
    D2D1_POINT_2F p0 = {250.0, 256.0};
    D2D1_POINT_2F p1 = {262.0, 256.0};
    m_pRenderTarget->DrawLine(p0, p1, m_pITextForeground, 1.0f);

    D2D1_POINT_2F p2 = {256.0, 250.0};
    D2D1_POINT_2F p3 = {256.0, 262.0};
    m_pRenderTarget->DrawLine(p2, p3, m_pITextForeground, 1.0f);
}


