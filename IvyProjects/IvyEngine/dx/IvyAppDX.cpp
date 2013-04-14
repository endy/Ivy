///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2010-2012, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "IvyApp.h"
#include "IvyDX.h"

#include "DxUtils.h"
#include "DxUI.h"

#include "IvyWindow.h"
#include "IvyCamera.h"

#include "DxMesh.h"
#include "DxShader.h"
#include "DxBuffer.h"
#include "DxTexture.h"

#include <Initguid.h>
#include <dxgidebug.h>

struct IvyDxUIData
{
    // UI Objects
    DxShader* pUserInterfaceVS;
    DxShader* pUserInterfacePS;
    DxBuffer* pUserInterfaceCameraBuf;
    DxMesh* pUserInterfaceQuad;
    ID3D11BlendState* pUserInterfaceBlendState;
    ID3D11Texture2D* pUserInterfaceOverlay;
    ID3D11ShaderResourceView* pUserInterfaceSRV;
    IDXGIKeyedMutex* pUserInterfaceMutexD3D;
};


///////////////////////////////////////////////////////////////////////////////////////////////////
/// IvyApp::InitDX
///
/// @brief
///     
/// @return
///     N/A
///////////////////////////////////////////////////////////////////////////////////////////////////
bool IvyApp::InitDX()
{
    bool success = true;

    m_pDxData = new IvyAppDxData();
    memset(&m_pDxData->viewport, 0, sizeof(D3D11_VIEWPORT));

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
        &m_pDxData->pDXGISwapChain, 
        &m_pDxData->pD3D11Device, 
        &FeatureLevelsSupported,
        &m_pDxData->pD3D11Context )))
    {
        success = false;
    }

    if (success)
    {
        ID3D11Texture2D* pBackBuffer = NULL;

        // Get a pointer to the back buffer
        if (DxFAIL(m_pDxData->pDXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer)))
        {
            success = false;
        }
        else
        {
            // Create a render-target view
            m_pDxData->pD3D11Device->CreateRenderTargetView(pBackBuffer,
                NULL,
                &m_pDxData->pAppRenderTargetView );
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

        m_pDxData->pAppDepthStencilTex = DxTexture::Create(m_pDxData->pD3D11Device, &depthStencilCreateInfo);
    }

    // Setup viewport
    m_pDxData->viewport.Width = static_cast<FLOAT>(m_screenWidth);
    m_pDxData->viewport.Height = static_cast<FLOAT>(m_screenHeight);
    m_pDxData->viewport.MinDepth = 0.0f;
    m_pDxData->viewport.MaxDepth = 1.0f;
    m_pDxData->viewport.TopLeftX = 0;
    m_pDxData->viewport.TopLeftY = 0;

    // Create UI
    ///@todo Move UI creation up into IvyApp once the creation of IvyUI exists
    m_pUI = DxUI::Create();
    m_pUIData = new IvyDxUIData();

    m_pUIData->pUserInterfaceVS = DxShader::CreateFromFile(m_pDxData->pD3D11Device, "PosTexTri", L"Content/shaders/DxUI.hlsl", PosTexVertexDesc, PosTexElements);
    m_pUIData->pUserInterfacePS = DxShader::CreateFromFile(m_pDxData->pD3D11Device, "ApplyTex", L"Content/shaders/DxUI.hlsl");

    CameraBufferData cameraData;
    memset(&cameraData, 0, sizeof(CameraBufferData));

    DxBufferCreateInfo cameraBufferCreateInfo = {0};
    cameraBufferCreateInfo.flags.cpuWriteable = TRUE;
    cameraBufferCreateInfo.elemSizeBytes = sizeof(CameraBufferData);
    cameraBufferCreateInfo.pInitialData = &cameraData;
    m_pUIData->pUserInterfaceCameraBuf = DxBuffer::Create(m_pDxData->pD3D11Device, &cameraBufferCreateInfo);

    Plane p;
    DxMeshCreateInfo planeMeshInfo;
    memset(&planeMeshInfo, 0, sizeof(planeMeshInfo));
    planeMeshInfo.pVertexArray = p.GetVB();
    planeMeshInfo.vertexCount = p.NumVertices();
    planeMeshInfo.vertexElementSize = sizeof(VertexPTN);

    m_pUIData->pUserInterfaceQuad = DxMesh::Create(m_pDxData->pD3D11Device, &planeMeshInfo);

    m_pUIData->pUserInterfaceOverlay = m_pUI->CreateSharedTextureOverlay(m_pDxData->pD3D11Device);

    if (DxFAIL(m_pUIData->pUserInterfaceOverlay->QueryInterface(__uuidof(IDXGIKeyedMutex),
        (LPVOID*) &m_pUIData->pUserInterfaceMutexD3D)))
    {
        success = false;
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvDesc.Texture2D.MipLevels = 1;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

    m_pDxData->pD3D11Device->CreateShaderResourceView(m_pUIData->pUserInterfaceOverlay, &srvDesc, &m_pUIData->pUserInterfaceSRV);

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
    m_pDxData->pD3D11Device->CreateBlendState(&uiBlendDesc, &m_pUIData->pUserInterfaceBlendState);

    return success;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// IvyApp::DeinitDX
///
/// @brief
///     
/// @return
///     N/A
///////////////////////////////////////////////////////////////////////////////////////////////////
bool IvyApp::DeinitDX()
{
    ///@todo Add 'Safe Release' calls or some such to release these components
    if (m_pDxData->pAppRenderTargetView != NULL)
    {
        m_pDxData->pAppRenderTargetView->Release();
    }

    if (m_pDxData->pAppDepthStencilTex != NULL)
    {
        m_pDxData->pAppDepthStencilTex->Destroy();
        m_pDxData->pAppDepthStencilTex = NULL;
    }

    if (m_pDxData->pD3D11Context != NULL)
    {
        m_pDxData->pD3D11Context->Release();
    }

    if (m_pDxData->pD3D11Device != NULL)
    {
        m_pDxData->pD3D11Device->Release();
    }

    if (m_pDxData->pDXGISwapChain != NULL)
    {
        m_pDxData->pDXGISwapChain->Release();
    }

    if (m_pUI)
    {
        m_pUI->Destroy();
        m_pUI = NULL;
    }

    if (m_pUIData->pUserInterfaceVS)
    {
        m_pUIData->pUserInterfaceVS->Destroy();
    }

    if (m_pUIData->pUserInterfacePS)
    {
        m_pUIData->pUserInterfacePS->Destroy();
    }

    if (m_pUIData->pUserInterfaceCameraBuf)
    {
        m_pUIData->pUserInterfaceCameraBuf->Destroy();
    }

    if (m_pUIData->pUserInterfaceQuad)
    {
        m_pUIData->pUserInterfaceQuad->Destroy();
    }

    if (m_pUIData->pUserInterfaceBlendState)
    {
        m_pUIData->pUserInterfaceBlendState->Release();
    }

    if (m_pUIData->pUserInterfaceSRV)
    {
        m_pUIData->pUserInterfaceSRV->Release();
    }

    if (m_pUIData->pUserInterfaceOverlay)
    {
        m_pUIData->pUserInterfaceOverlay->Release();
    }

    if (m_pUIData->pUserInterfaceMutexD3D)
    {
        m_pUIData->pUserInterfaceMutexD3D->Release();
    }

    ///@todo Clean this up, perhaps make it more general
    typedef HANDLE(__stdcall *fPtr)(const IID&, void**);
    HMODULE hDxgiDebug = GetModuleHandle("DXGIDebug.dll");
    fPtr DXGIGetDebugInterface = (fPtr) GetProcAddress(hDxgiDebug, "DXGIGetDebugInterface");

    IDXGIDebug* pDxgiDebug = NULL;
    DXGIGetDebugInterface(__uuidof(IDXGIDebug), (void**)&pDxgiDebug);
    pDxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// IvyApp::UpdateSwapChain
///
/// @brief
///     
/// @return
///     N/A
///////////////////////////////////////////////////////////////////////////////////////////////////
void IvyApp::UpdateSwapChain()
{
    Rect viewportRect = m_pWindow->GetDrawableArea();
    m_pCamera->UpdateViewport(viewportRect);

    m_screenWidth = viewportRect.right - viewportRect.left;
    m_screenHeight = viewportRect.bottom - viewportRect.top;

    // Setup the viewport
    m_pDxData->viewport.Width = static_cast<FLOAT>(m_screenWidth);
    m_pDxData->viewport.Height = static_cast<FLOAT>(m_screenHeight);
    m_pDxData->viewport.MinDepth = 0.0f;
    m_pDxData->viewport.MaxDepth = 1.0f;
    m_pDxData->viewport.TopLeftX = 0;
    m_pDxData->viewport.TopLeftY = 0;

    m_pDxData->pD3D11Context->OMSetRenderTargets(0, 0, 0);

    // Release all outstanding references to the swap chain's buffers.
    m_pDxData->pAppRenderTargetView->Release();

    HRESULT hr;
    hr = m_pDxData->pDXGISwapChain->ResizeBuffers(BufferCount,
        m_screenWidth,
        m_screenHeight,
        DXGI_FORMAT_R8G8B8A8_UNORM,
        0);

    IvyAssert(!DxFAIL(hr));

    // Get buffer and create a render-target-view.
    ID3D11Texture2D* pBackBuffer;
    hr = m_pDxData->pDXGISwapChain->GetBuffer(0,
        __uuidof( ID3D11Texture2D),
        (void**) &pBackBuffer );
    // Perform error handling here!

    hr = m_pDxData->pD3D11Device->CreateRenderTargetView(pBackBuffer,
        NULL,
        &m_pDxData->pAppRenderTargetView);
    // Perform error handling here!
    pBackBuffer->Release();

    if (m_pDxData->pAppDepthStencilTex != NULL)
    {
        m_pDxData->pAppDepthStencilTex->Destroy();
        m_pDxData->pAppDepthStencilTex = NULL;

        DxTextureCreateInfo depthStencilCreateInfo;
        memset(&depthStencilCreateInfo, 0, sizeof(DxTextureCreateInfo));
        depthStencilCreateInfo.flags.DepthStencil = TRUE;
        depthStencilCreateInfo.flags.ShaderInput = TRUE;
        depthStencilCreateInfo.format = DXGI_FORMAT_D32_FLOAT;	//@todo texture doesnt read format currently
        depthStencilCreateInfo.width = m_screenWidth;
        depthStencilCreateInfo.height = m_screenHeight;

        m_pDxData->pAppDepthStencilTex = DxTexture::Create(m_pDxData->pD3D11Device, &depthStencilCreateInfo);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// IvyApp::DrawUI
///
/// @brief
///     
/// @return
///     N/A
///////////////////////////////////////////////////////////////////////////////////////////////////
void IvyApp::DrawUI()
{
    ID3D11DeviceContext* pContext = m_pDxData->pD3D11Context;
    ID3D11Device* pDevice = m_pDxData->pD3D11Device;

    UINT acquireKey = 1;
    UINT releaseKey = 0;
    UINT timeout = 5;

    HRESULT hr = m_pUIData->pUserInterfaceMutexD3D->AcquireSync(acquireKey, timeout);

    FLOAT blendFactors[4];
    UINT sampleMask = 0xFFFFFFFF;

    ID3D11BlendState* pLastBlendState = NULL;
    pContext->OMGetBlendState(&pLastBlendState, blendFactors, &sampleMask);

    if (hr == WAIT_OBJECT_0)
    {
        pContext->OMSetRenderTargets(1, &m_pDxData->pAppRenderTargetView, NULL);

        CameraBufferData* pCameraData = reinterpret_cast<CameraBufferData*>(m_pUIData->pUserInterfaceCameraBuf->Map(m_pDxData->pD3D11Context));
        pCameraData->worldMatrix = XMMatrixRotationX(-3.14f/2.0f) * XMMatrixScaling(2, 2, 1);
        pCameraData->viewMatrix = XMMatrixTranslation(0, 0, 2.0f) * m_pCamera->W2C();
        pCameraData->projectionMatrix = m_pCamera->C2S();

        m_pUIData->pUserInterfaceCameraBuf->Unmap(pContext);

        m_pUIData->pUserInterfaceVS->Bind(pContext);
        m_pUIData->pUserInterfaceCameraBuf->BindVS(pContext, 0);

        m_pDxData->pD3D11Context->PSSetShaderResources(0, 1, &m_pUIData->pUserInterfaceSRV);
        m_pUIData->pUserInterfacePS->Bind(pContext);

        m_pDxData->pD3D11Context->OMSetBlendState(m_pUIData->pUserInterfaceBlendState, blendFactors, sampleMask);

        m_pUIData->pUserInterfaceQuad->Bind(m_pDxData->pD3D11Context);
        m_pUIData->pUserInterfaceQuad->Draw(m_pDxData->pD3D11Context);
    }

    hr = m_pUIData->pUserInterfaceMutexD3D->ReleaseSync(releaseKey);

    m_pDxData->pD3D11Context->OMSetBlendState(pLastBlendState, blendFactors, sampleMask);

}


