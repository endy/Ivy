///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine - Cel Shading Demo
///
///     Copyright 2010-2011, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "CelShadeApp.h"

#include "IvyImporter.h"
#include "DxShader.h"
#include "DxTexture.h"
#include "DxMesh.h"
#include "DxBuffer.h"
#include "IvyUtils.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
/// CelShadeApp::Create
///////////////////////////////////////////////////////////////////////////////////////////////////
CelShadeApp* CelShadeApp::Create(
    CelShadeAppCreateInfo* pAppInfo)
{
    CelShadeApp* pApp = new CelShadeApp(pAppInfo);

    pApp->Init();

    return pApp;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// CelShadeApp::CelShadeApp
///////////////////////////////////////////////////////////////////////////////////////////////////
CelShadeApp::CelShadeApp(
    CelShadeAppCreateInfo* pAppInfo)
    :
    DxApp(pAppInfo),
    m_pPosTexTriVS(NULL),
    m_pPosTexNormVS(NULL),
    m_pCelShadePS(NULL)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// CelShadeApp::~CelShadeApp
///////////////////////////////////////////////////////////////////////////////////////////////////
CelShadeApp::~CelShadeApp()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// CelShadeApp::Destroy
///////////////////////////////////////////////////////////////////////////////////////////////////
void CelShadeApp::Destroy()
{
    if (m_pLightSlateGrayBrush)
    {
        m_pLightSlateGrayBrush->Release();
    }
    
    if (m_pCornflowerBlueBrush)
    {
        m_pCornflowerBlueBrush->Release();
    }

    delete this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// CelShadeApp::Init
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CelShadeApp::Init()
{
    bool success = DxApp::Init();

    DxShader* pVertexShader = DxShader::CreateFromSource(m_pDevice, "IvyVsPosTex", IvyVsPosTex,PosTexVertexDesc, PosTexElements);

    if (m_pRenderTarget)
    {
        RECT rc;
        GetClientRect(m_pWindow->GetHwnd(), &rc);

        D2D1_SIZE_U size = D2D1::SizeU(
            rc.right - rc.left,
            rc.bottom - rc.top
            );

        // Create a gray brush.
        m_pRenderTarget->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::LightSlateGray),
            &m_pLightSlateGrayBrush
            );

        // Create a blue brush.
        m_pRenderTarget->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::CornflowerBlue),
            &m_pCornflowerBlueBrush
            );
    }

    // Setup Camera
    m_pCamera->Position().x = 0;
    m_pCamera->Position().y = -3;
    m_pCamera->Position().z = 4;

    return success;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// CelShadeApp::Run
///////////////////////////////////////////////////////////////////////////////////////////////////
void CelShadeApp::Run()
{
    D3DX11_IMAGE_LOAD_INFO imageLoadInfo;
    memset( &imageLoadInfo, 0, sizeof(D3DX11_IMAGE_LOAD_INFO) );
    imageLoadInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    imageLoadInfo.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    ID3D11ShaderResourceView *pColorTexSRV = NULL;
    HRESULT imageHr = D3DX11CreateShaderResourceViewFromFile(m_pDevice, "halo.jpg", &imageLoadInfo, NULL, &pColorTexSRV, NULL );

    DxTextureCreateInfo shadeTexInfo;
    memset(&shadeTexInfo, 0, sizeof(DxTextureCreateInfo));
    shadeTexInfo.flags.RenderTarget = TRUE;
    shadeTexInfo.flags.ShaderInput = TRUE;
    shadeTexInfo.format = DXGI_FORMAT_R8G8B8A8_UNORM;
    shadeTexInfo.width = m_screenWidth;
    shadeTexInfo.height = m_screenHeight;
    DxTexture* pShadeTex = DxTexture::Create(m_pDevice, &shadeTexInfo);

    DxTextureCreateInfo edgeTexInfo;
    memset(&edgeTexInfo, 0, sizeof(DxTextureCreateInfo));
    edgeTexInfo.flags.RenderTarget = TRUE;
    edgeTexInfo.flags.ShaderInput = TRUE;
    edgeTexInfo.format = DXGI_FORMAT_R8G8B8A8_UNORM;
    edgeTexInfo.width = m_screenWidth;
    edgeTexInfo.height = m_screenHeight;
    DxTexture* pEdgeTex = DxTexture::Create(m_pDevice, &edgeTexInfo);


    ID3D11ShaderResourceView* pUI_SRV = NULL;

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
    m_pDevice->CreateBlendState(&uiBlendDesc, &pUIBlendState);

    // Samplers  /////////////////////////////////////////////////////////////////////////////

    // SamplerState PointSampler : register(s0);

    D3D11_SAMPLER_DESC pointSamplerDesc;
    memset(&pointSamplerDesc, 0, sizeof(D3D11_SAMPLER_DESC));
    pointSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    pointSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    pointSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    pointSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    pointSamplerDesc.MinLOD = -FLT_MAX;
    pointSamplerDesc.MaxLOD = FLT_MAX;
    pointSamplerDesc.MipLODBias = 0.0f;
    pointSamplerDesc.MaxAnisotropy = 16;

    ID3D11SamplerState* pPointSampler = NULL;
    m_pDevice->CreateSamplerState(&pointSamplerDesc, &pPointSampler);

    //
    UINT numVertices = 0, numIndices = 0;
    VertexPTN* pVB = NULL;
    UINT* pIB = NULL;
    ImportPly("../Content/dragon_vrip_res3.ply", numVertices, &pVB, numIndices, &pIB);
    //ImportPly("../Content/bun_zipper_res4.ply", numVertices, &pVB, numIndices, &pIB);
    DxMeshCreateInfo meshCreateInfo = {0};
    meshCreateInfo.indexCount = numIndices;
    meshCreateInfo.pIndexArray = pIB;
    meshCreateInfo.indexFormat = DXGI_FORMAT_R32_UINT;
    meshCreateInfo.pVertexArray = pVB;
    meshCreateInfo.vertexCount = numVertices;
    meshCreateInfo.vertexElementSize = sizeof(VertexPTN);
    DxMesh* pMesh = DxMesh::Create(m_pDevice, &meshCreateInfo);

    Plane p;
    DxMeshCreateInfo planeMeshInfo;
    memset(&planeMeshInfo, 0, sizeof(planeMeshInfo));
    planeMeshInfo.pVertexArray = p.GetVB();
    planeMeshInfo.vertexCount = p.NumVertices();
    planeMeshInfo.vertexElementSize = sizeof(VertexPTN);

    DxMesh* pPlaneMesh = DxMesh::Create(m_pDevice, &planeMeshInfo);

    Cube c;
    DxMeshCreateInfo cubeMeshInfo;
    memset(&cubeMeshInfo, 0, sizeof(cubeMeshInfo));
    cubeMeshInfo.pVertexArray = c.GetVB();
    cubeMeshInfo.vertexCount = c.NumVertices();
    cubeMeshInfo.vertexElementSize = sizeof(VertexPT);

    DxMesh* pCubeMesh = DxMesh::Create(m_pDevice, &cubeMeshInfo);

    D3D11_SUBRESOURCE_DATA cbInitData;
    memset(&cbInitData, 0, sizeof(D3D11_SUBRESOURCE_DATA));	

    // Camera Buffer
    CameraBufferData cameraData;
    memset(&cameraData, 0, sizeof(CameraBufferData));

    DxBufferCreateInfo cameraBufferCreateInfo = {0};
    cameraBufferCreateInfo.flags.cpuWriteable = TRUE;
    cameraBufferCreateInfo.elemSizeBytes = sizeof(CameraBufferData);
    cameraBufferCreateInfo.pInitialData = &cameraData;
    DxBuffer* pCameraBuffer = DxBuffer::Create(m_pDevice, &cameraBufferCreateInfo);

    // Shaders ////////////////////////////////////////////////////////////////////////////////////

    m_pPosTexTriVS = DxShader::CreateFromFile(m_pDevice, "PosTexTri", "CelShade.hlsl", PosTexVertexDesc, PosTexElements);
    m_pPosTexNormVS = DxShader::CreateFromFile(m_pDevice, "PosTexNorm", "CelShade.hlsl", PosTexNormVertexDesc, PosTexNormElements);

    m_pCelShadePS = DxShader::CreateFromFile(m_pDevice, "CelShade", "CelShade.hlsl");
    DxShader* pDetectEdges = DxShader::CreateFromFile(m_pDevice, "DetectEdges", "CelShade.hlsl");
    DxShader* pApplyTexPS = DxShader::CreateFromFile(m_pDevice, "ApplyTex", "CelShade.hlsl");

    DxShader* pCubeVS = DxShader::CreateFromFile(m_pDevice, "PosTex", "CelShade.hlsl", PosTexVertexDesc, PosTexElements);
    DxShader* pCubePS = DxShader::CreateFromFile(m_pDevice, "CubePS", "CelShade.hlsl");


    ////////////////////////////////////////////////////////////////////////////////////////

    m_pContext->ClearState();

    // SET RENDER STATE

    FLOAT clearColor[4];
    clearColor[0] = 0.2f;
    clearColor[1] = 0.2f;
    clearColor[2] = 0.2f; 
    clearColor[3] = 1.0f;

    D3D11_RASTERIZER_DESC shadeDesc;
    shadeDesc.FillMode = D3D11_FILL_SOLID;
    shadeDesc.CullMode = D3D11_CULL_BACK;
    shadeDesc.FrontCounterClockwise = FALSE;
    shadeDesc.DepthBias = 0;
    shadeDesc.DepthBiasClamp = 0.0f;
    shadeDesc.SlopeScaledDepthBias = 0;
    shadeDesc.DepthClipEnable = false;
    shadeDesc.ScissorEnable = false;
    shadeDesc.MultisampleEnable = false;
    shadeDesc.AntialiasedLineEnable = false;

    ID3D11RasterizerState* pShadeRS = NULL;
    m_pDevice->CreateRasterizerState(&shadeDesc, &pShadeRS);

    m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    m_pWindow->DisplayWindow();

    BOOL quit = false;
    FLOAT yRotationAngle = 0.0f;
    while (!quit)
    {
        BeginFrame();

        CameraBufferData* pCameraData = NULL;

        m_pWindow->ProcessMsg(&quit);

        input();

        // new frame, clear state
        m_pContext->ClearState();

        m_pContext->RSSetViewports(1, &m_viewport);
        m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        m_pContext->RSSetState(pShadeRS);
        m_pContext->PSSetSamplers(0, 1, &pPointSampler);

        m_pContext->OMSetRenderTargets(1,
                                       &m_pRenderTargetView,
                                       m_pDepthStencilBuffer->GetDepthStencilView());
        m_pContext->ClearRenderTargetView(m_pRenderTargetView, clearColor);
        m_pContext->ClearDepthStencilView(m_pDepthStencilBuffer->GetDepthStencilView(),
                                          D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
                                          1.0,
                                          0);

        ///// Draw Mesh ///////////////////////////////////////////////////////////////////////////

        FLOAT viewRotationY = (GetMousePos().x - (m_screenWidth / 2.0f)) /(m_screenWidth / 2.0f);
        viewRotationY *= (3.14159f / 4.0f);

        FLOAT viewRotationZ = (GetMousePos().y - (m_screenHeight / 2.0f)) /(m_screenHeight / 2.0f);
        viewRotationZ *= (3.14159f / 4.0f);

        pCameraData = reinterpret_cast<CameraBufferData*>(pCameraBuffer->Map(m_pContext));
        pCameraData->worldMatrix = XMMatrixScaling(25, 25, 25) * XMMatrixRotationY(yRotationAngle) *
                                   XMMatrixTranslation(m_pCamera->Position().x,
                                                       m_pCamera->Position().y,
                                                       m_pCamera->Position().z) ;
        // translate world +6 in Z to position camera -9 from world origin
        pCameraData->viewMatrix = m_pCamera->W2C();
        pCameraData->projectionMatrix = m_pCamera->C2S();

        pCameraBuffer->Unmap(m_pContext);

        pCameraBuffer->BindVS(m_pContext, 0);

        pMesh->Bind(m_pContext);

        m_pPosTexNormVS->Bind(m_pContext);
        m_pCelShadePS->Bind(m_pContext);
        pMesh->Draw(m_pContext);

        ///// Detect Edges ///////////////////////////////////////////////////////////////////////////


        ///// Draw Light Position ////////////////////////////////////////////////////////////////////

        //yRotationAngle = 0;
        pCameraData = reinterpret_cast<CameraBufferData*>(pCameraBuffer->Map(m_pContext));
        pCameraData->worldMatrix = XMMatrixScaling(1, 1, 1);
                                 //  XMMatrixRotationY(yRotationAngle);
                                  // XMMatrixTranslation(-10, 10, 10);
        // translate world +6 in Z to position camera -9 from world origin
        pCameraData->viewMatrix = XMMatrixTranslation(0, 0, 10) * m_pCamera->W2C();
        pCameraData->projectionMatrix = m_pCamera->C2S();

        pCameraBuffer->Unmap(m_pContext);
        pCameraBuffer->BindVS(m_pContext, 0);

        pCubeVS->Bind(m_pContext);

        pCubePS->Bind(m_pContext);

        pCubeMesh->Bind(m_pContext);
        pCubeMesh->Draw(m_pContext);

        ///// Draw UI ////////////////////////////////////////////////////////////////////////////////

        Draw2D();

        UINT acquireKey = 1;
        UINT releaseKey = 0;
        UINT timeout = 5;

        HRESULT hr = m_pUIKeyedMutex_D3D->AcquireSync(acquireKey, timeout);

        FLOAT blendFactors[4];
        UINT sampleMask = 0xFFFFFFFF;

        ID3D11BlendState* pBackupBlendState = NULL;
        m_pContext->OMGetBlendState(&pBackupBlendState, blendFactors, &sampleMask);

        if (hr == WAIT_OBJECT_0)
        {
            m_pContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);

            pCameraData = reinterpret_cast<CameraBufferData*>(pCameraBuffer->Map(m_pContext));
            pCameraData->worldMatrix = XMMatrixRotationX(-3.14f/2.0f) * XMMatrixScaling(2, 2, 1);
            pCameraData->viewMatrix = XMMatrixTranslation(0, 0, 2.0f) * m_pCamera->W2C();
            pCameraData->projectionMatrix = m_pCamera->C2S();

            pCameraBuffer->Unmap(m_pContext);

            m_pPosTexTriVS->Bind(m_pContext);

            m_pContext->PSSetShaderResources(0, 1, &pUI_SRV);
            pApplyTexPS->Bind(m_pContext);

            m_pContext->OMSetBlendState(pUIBlendState, blendFactors, sampleMask);

            pPlaneMesh->Bind(m_pContext);
            pPlaneMesh->Draw(m_pContext);
        }

        hr = m_pUIKeyedMutex_D3D->ReleaseSync(releaseKey);

        m_pContext->OMSetBlendState(pBackupBlendState, blendFactors, sampleMask);

        m_pSwapChain->Present(0,0);

        EndFrame();

        Sleep(50);
        yRotationAngle += 3.14159f / 60.0f;
    }

    // Shader Resource Views
    pColorTexSRV->Release();

    pCameraBuffer->Destroy();

    // Shaders
    m_pCelShadePS->Destroy();
    m_pCelShadePS = NULL;

    m_pPosTexTriVS->Destroy();
    m_pPosTexTriVS = NULL;

    m_pPosTexNormVS->Destroy();
    m_pPosTexNormVS = NULL;

    pApplyTexPS->Destroy();
    pApplyTexPS = NULL;

    pPlaneMesh->Destroy();
    pPlaneMesh = NULL;

    // Samplers
    pPointSampler->Release();

    // Rasterizer State
    pShadeRS->Release();

    m_pContext->ClearState();
    m_pContext->Flush(); 
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// CelShadeApp::ReceiveEvent
///////////////////////////////////////////////////////////////////////////////////////////////////
void CelShadeApp::ReceiveEvent(
    const Event* pEvent)
{
    if (pEvent->GetType() == EventTypeKeyDown)
    {
        HandleKeyboardInput(pEvent);
    }
    else
    {
        DxApp::ReceiveEvent(pEvent);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// CelShadeApp::HandleKeyboardInput
///////////////////////////////////////////////////////////////////////////////////////////////////
void CelShadeApp::HandleKeyboardInput(
    const Event* pEvent)
{
    if (pEvent->GetType() == EventTypeKeyDown)
    {
        const EventKeyDown* pKeyDownEvent = reinterpret_cast<const EventKeyDown*>(pEvent);

        switch(pKeyDownEvent->GetData().key)
        {
        case EKeyR:
            IvyAssertAlways(); // Reload disabled after shader refactoring
            //Reload(m_pDevice, &m_pCelShadePS);
            break;
        case EKeyW:
            m_pCamera->Position().z -= 1;
            break;
        case EKeyS:
            m_pCamera->Position().z += 1;
            break;
        case EKeyA:
            m_pCamera->Position().x += 1;
            break;
        case EKeyD:
            m_pCamera->Position().x -= 1;
            break;
        default:
            break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// CelShadeApp::Draw2D
///////////////////////////////////////////////////////////////////////////////////////////////////
void CelShadeApp::Draw2D()
{
    UINT acquireKey = 0;
    UINT releaseKey = 1;
    UINT timeout = 5;

    if (m_pRenderTarget)
    {
        HRESULT hr = m_pUIKeyedMutex_D2D->AcquireSync(acquireKey, timeout);

        if (hr == WAIT_OBJECT_0)
        {
            
            m_pRenderTarget->BeginDraw();
            D2D1_COLOR_F clearColor = {0};
            m_pRenderTarget->Clear(clearColor);
            m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

            D2D1_RECT_F rect;
            rect.left = 0.0f;
            rect.top = 0.0f;
            rect.right = 20.0f;
            rect.bottom = 20.0f;

           m_pRenderTarget->DrawRectangle(&rect, m_pCornflowerBlueBrush, 3.0f);

            D2D1_RECT_F layoutRect = D2D1::RectF(0.f, 0.f, 300.f, 100.f);

            wchar_t stringBuffer[1024];
            memset(stringBuffer, 0, 1024*sizeof(wchar_t));
            swprintf(stringBuffer,
                    1024,
                    L"Camera Pos (%f, %f, %f)",
                    m_pCamera->Position().x,
                    m_pCamera->Position().y,
                    m_pCamera->Position().z);

            m_pRenderTarget->DrawText(
                stringBuffer,
                wcslen(stringBuffer),
                m_pITextFormat,
                layoutRect, 
                m_pITextBrush);

            m_pRenderTarget->EndDraw();
            m_pRenderTarget->Flush();
            
        }

        m_pUIKeyedMutex_D2D->ReleaseSync(releaseKey);
    }
}

