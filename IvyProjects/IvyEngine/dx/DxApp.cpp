///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2010-2012, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "DxApp.h"
#include "DxUtils.h"
#include "DxUI.h"

#include "IvyWindow.h"
#include "IvyCamera.h"

#include "DxMesh.h"
#include "DxShader.h"
#include "DxBuffer.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxApp::DxApp
///////////////////////////////////////////////////////////////////////////////////////////////////
DxApp::DxApp()
    :
    m_pDevice(NULL),
    m_pContext(NULL),
    m_pSwapChain(NULL),
    m_pRenderTargetView(NULL),
    m_pDepthStencilBuffer(NULL),
    m_pUI(NULL),
    m_pUIVS(NULL),
    m_pUIPS(NULL),
    m_pUICameraBuffer(NULL),
    m_pUIQuadMesh(NULL),
    m_pUIBlendState(NULL),
    m_pUIoverlay(NULL),
    pUI_SRV(NULL),
    m_pUIKeyedMutex_D3D(NULL)
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
DxApp* DxApp::Create()
{
    DxApp* pApp = new DxApp();

    if (pApp->Init() == false)
    {
        pApp->Destroy();
        pApp = NULL;
    }

    return pApp;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxApp::Destroy
///
/// @brief
///     Destroys DxApp object after cleaning up its resources
/// @return
///     N/A
///////////////////////////////////////////////////////////////////////////////////////////////////
void DxApp::Destroy()
{
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

    /*
    m_pUI(NULL),
    m_pUIVS(NULL),
    m_pUIPS(NULL),
    m_pUICameraBuffer(NULL),
    m_pUIQuadMesh(NULL),
    m_pUIBlendState(NULL),
    pUI_SRV(NULL)
    */

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
    bool success = IvyApp::Init();

    EnumerateAdapters();
    DxEnumDisplayDevices();

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

    // Create UI
    ///@TODO Move UI creation up into IvyApp once the creation of IvyUI exists
    m_pUI = DxUI::Create();

    m_pUIVS = DxShader::CreateFromFile(m_pDevice, "PosTexTri", "Content/shaders/DxUI.hlsl", PosTexVertexDesc, PosTexElements);
    m_pUIPS = DxShader::CreateFromFile(m_pDevice, "ApplyTex", "Content/shaders/DxUI.hlsl");

    CameraBufferData cameraData;
    memset(&cameraData, 0, sizeof(CameraBufferData));

    DxBufferCreateInfo cameraBufferCreateInfo = {0};
    cameraBufferCreateInfo.flags.cpuWriteable = TRUE;
    cameraBufferCreateInfo.elemSizeBytes = sizeof(CameraBufferData);
    cameraBufferCreateInfo.pInitialData = &cameraData;
    m_pUICameraBuffer = DxBuffer::Create(m_pDevice, &cameraBufferCreateInfo);

    Plane p;
    DxMeshCreateInfo planeMeshInfo;
    memset(&planeMeshInfo, 0, sizeof(planeMeshInfo));
    planeMeshInfo.pVertexArray = p.GetVB();
    planeMeshInfo.vertexCount = p.NumVertices();
    planeMeshInfo.vertexElementSize = sizeof(VertexPTN);

    m_pUIQuadMesh = DxMesh::Create(m_pDevice, &planeMeshInfo);

    m_pUIoverlay = m_pUI->CreateSharedTextureOverlay(m_pDevice);

    if (DxFAIL(m_pUIoverlay->QueryInterface(__uuidof(IDXGIKeyedMutex),
        (LPVOID*) &m_pUIKeyedMutex_D3D)))
    {
        success = false;
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvDesc.Texture2D.MipLevels = 1;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

    m_pDevice->CreateShaderResourceView(m_pUIoverlay, &srvDesc, &pUI_SRV);

    D3D11_BLEND_DESC uiBlendDesc;
    memset(&uiBlendDesc, 0, sizeof(D3D11_BLEND_DESC));

    uiBlendDesc.AlphaToCoverageEnable = FALSE;
    uiBlendDesc.IndependentBlendEnable = FALSE;
    uiBlendDesc.RenderTarget[0].BlendEnable = TRUE;

    uiBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
    uiBlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    uiBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    uiBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    uiBlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    uiBlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

    uiBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    ID3D11BlendState* pUIBlendState = NULL;
    m_pDevice->CreateBlendState(&uiBlendDesc, &m_pUIBlendState);

    return success;
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

    IvyAssert(!DxFAIL(hr));

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

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxApp::DrawUI
///
/// @brief
///     
/// @return
///     N/A
///////////////////////////////////////////////////////////////////////////////////////////////////
void DxApp::DrawUI()
{
    UINT acquireKey = 1;
    UINT releaseKey = 0;
    UINT timeout = 5;

    HRESULT hr = m_pUIKeyedMutex_D3D->AcquireSync(acquireKey, timeout);

    FLOAT blendFactors[4];
    UINT sampleMask = 0xFFFFFFFF;

    ID3D11BlendState* pLastBlendState = NULL;
    m_pContext->OMGetBlendState(&pLastBlendState, blendFactors, &sampleMask);

    if (hr == WAIT_OBJECT_0)
    {
        m_pContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);

        CameraBufferData* pCameraData = reinterpret_cast<CameraBufferData*>(m_pUICameraBuffer->Map(m_pContext));
        pCameraData->worldMatrix = XMMatrixRotationX(-3.14f/2.0f) * XMMatrixScaling(2, 2, 1);
        pCameraData->viewMatrix = XMMatrixTranslation(0, 0, 2.0f) * m_pCamera->W2C();
        pCameraData->projectionMatrix = m_pCamera->C2S();

        m_pUICameraBuffer->Unmap(m_pContext);

        m_pUIVS->Bind(m_pContext);
        m_pUICameraBuffer->BindVS(m_pContext, 0);

        m_pContext->PSSetShaderResources(0, 1, &pUI_SRV);
        m_pUIPS->Bind(m_pContext);

        m_pContext->OMSetBlendState(m_pUIBlendState, blendFactors, sampleMask);

        m_pUIQuadMesh->Bind(m_pContext);
        m_pUIQuadMesh->Draw(m_pContext);
    }

    hr = m_pUIKeyedMutex_D3D->ReleaseSync(releaseKey);

    m_pContext->OMSetBlendState(pLastBlendState, blendFactors, sampleMask);

}
