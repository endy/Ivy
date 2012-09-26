///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine - Instancing Demo
///
///     Copyright 2011, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "InstancingApp.h"
#include "IvyDX.h"
#include "IvyWindow.h"
#include "IvyCamera.h"
#include "IvyImporter.h"
#include "DxShader.h"
#include "DxTexture.h"
#include "DxMesh.h"
#include "DxBuffer.h"
#include "IvyUtils.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
/// InstancingApp::Create
///////////////////////////////////////////////////////////////////////////////////////////////////
InstancingApp* InstancingApp::Create()
{
    InstancingApp* pApp = new InstancingApp();

    pApp->Init();

    return pApp;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// InstancingApp::InstancingApp
///////////////////////////////////////////////////////////////////////////////////////////////////
InstancingApp::InstancingApp()
    :
    IvyApp(),
    m_pPosTexTriVS(NULL),
    m_pPosTexNormVS(NULL)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// InstancingApp::~InstancingApp
///////////////////////////////////////////////////////////////////////////////////////////////////
InstancingApp::~InstancingApp()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// InstancingApp::Destroy
///////////////////////////////////////////////////////////////////////////////////////////////////
void InstancingApp::Destroy()
{
    IvyApp::Destroy();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// InstancingApp::Init
///////////////////////////////////////////////////////////////////////////////////////////////////
bool InstancingApp::Init()
{
    bool success = IvyApp::Init();

    InitDX();

    DxShader* pVertexShader = DxShader::CreateFromSource(m_pDxData->pD3D11Device, "IvyVsPosTex", IvyVsPosTex,PosTexVertexDesc, PosTexElements);

    // Setup Camera
    m_pCamera->Position().x = 0;
    m_pCamera->Position().y = -3;
    m_pCamera->Position().z = 4;

    return success;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// InstancingApp::Run
///////////////////////////////////////////////////////////////////////////////////////////////////
void InstancingApp::Run()
{
    ID3D11DeviceContext* pContext = m_pDxData->pD3D11Context;
    ID3D11Device* pDevice = m_pDxData->pD3D11Device;

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
    pDevice->CreateSamplerState(&pointSamplerDesc, &pPointSampler);

    //
    UINT numVertices = 0, numIndices = 0;
    VertexPTN* pVB = NULL;
    UINT* pIB = NULL;
    ImportPly("Content/dragon_vrip_res3.ply", numVertices, &pVB, numIndices, &pIB);
    //ImportPly("../Content/bun_zipper_res4.ply", numVertices, &pVB, numIndices, &pIB);
    DxMeshCreateInfo meshCreateInfo = {0};
    meshCreateInfo.indexCount = numIndices;
    meshCreateInfo.pIndexArray = pIB;
    meshCreateInfo.indexFormat = DXGI_FORMAT_R32_UINT;
    meshCreateInfo.pVertexArray = pVB;
    meshCreateInfo.vertexCount = numVertices;
    meshCreateInfo.vertexElementSize = sizeof(VertexPTN);
    DxMesh* pMesh = DxMesh::Create(pDevice, &meshCreateInfo);

    Plane p;
    DxMeshCreateInfo planeMeshInfo;
    memset(&planeMeshInfo, 0, sizeof(planeMeshInfo));
    planeMeshInfo.pVertexArray = p.GetVB();
    planeMeshInfo.vertexCount = p.NumVertices();
    planeMeshInfo.vertexElementSize = sizeof(VertexPTN);

    DxMesh* pPlaneMesh = DxMesh::Create(pDevice, &planeMeshInfo);

    Cube c;
    DxMeshCreateInfo cubeMeshInfo;
    memset(&cubeMeshInfo, 0, sizeof(cubeMeshInfo));
    cubeMeshInfo.pVertexArray = c.GetVB();
    cubeMeshInfo.vertexCount = c.NumVertices();
    cubeMeshInfo.vertexElementSize = sizeof(VertexPT);

    DxMesh* pCubeMesh = DxMesh::Create(pDevice, &cubeMeshInfo);

    D3D11_SUBRESOURCE_DATA cbInitData;
    memset(&cbInitData, 0, sizeof(D3D11_SUBRESOURCE_DATA));	

    // Camera Buffer
    CameraBufferData cameraData;
    memset(&cameraData, 0, sizeof(CameraBufferData));

    DxBufferCreateInfo cameraBufferCreateInfo = {0};
    cameraBufferCreateInfo.flags.cpuWriteable = TRUE;
    cameraBufferCreateInfo.elemSizeBytes = sizeof(CameraBufferData);
    cameraBufferCreateInfo.pInitialData = &cameraData;
    DxBuffer* pCameraBuffer = DxBuffer::Create(pDevice, &cameraBufferCreateInfo);

    // Shaders ////////////////////////////////////////////////////////////////////////////////////

    m_pPosTexTriVS = DxShader::CreateFromFile(pDevice, "PosTexTri", "Content/shaders/Instancing.hlsl", PosTexVertexDesc, PosTexElements);
    m_pPosTexNormVS = DxShader::CreateFromFile(pDevice, "PosTexNorm", "Content/shaders/Instancing.hlsl", PosTexNormVertexDesc, PosTexNormElements);

    DxShader* pApplyTexPS = DxShader::CreateFromFile(pDevice, "ApplyTex", "Content/shaders/Instancing.hlsl");

    DxShader* pCubeVS = DxShader::CreateFromFile(pDevice, "PosTex", "Content/shaders/Instancing.hlsl", PosTexVertexDesc, PosTexElements);
    DxShader* pCubePS = DxShader::CreateFromFile(pDevice, "CubePS", "Content/shaders/Instancing.hlsl");
    ////////////////////////////////////////////////////////////////////////////////////////

    pContext->ClearState();

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
    pDevice->CreateRasterizerState(&shadeDesc, &pShadeRS);

    pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    m_pWindow->Show();

    BOOL quit = false;
    FLOAT yRotationAngle = 0.0f;
    while (!quit)
    {
        BeginFrame();

        CameraBufferData* pCameraData = NULL;

        m_pWindow->ProcessMsg(&quit);

        input();

        // new frame, clear state
        pContext->ClearState();

        pContext->RSSetViewports(1, &m_pDxData->viewport);
        pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        pContext->RSSetState(pShadeRS);
        pContext->PSSetSamplers(0, 1, &pPointSampler);

        pContext->OMSetRenderTargets(1,
                                       &m_pDxData->pAppRenderTargetView,
                                       m_pDxData->pAppDepthStencilTex->GetDepthStencilView());
        pContext->ClearRenderTargetView(m_pDxData->pAppRenderTargetView, clearColor);
        pContext->ClearDepthStencilView(m_pDxData->pAppDepthStencilTex->GetDepthStencilView(),
                                          D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
                                          1.0,
                                          0);

        ///// Draw Mesh ///////////////////////////////////////////////////////////////////////////

        FLOAT viewRotationY = (GetMousePos().x - (m_screenWidth / 2.0f)) /(m_screenWidth / 2.0f);
        viewRotationY *= (3.14159f / 4.0f);

        FLOAT viewRotationZ = (GetMousePos().y - (m_screenHeight / 2.0f)) /(m_screenHeight / 2.0f);
        viewRotationZ *= (3.14159f / 4.0f);

        pCameraData = reinterpret_cast<CameraBufferData*>(pCameraBuffer->Map(pContext));
        pCameraData->worldMatrix = XMMatrixScaling(25, 25, 25) * XMMatrixRotationY(yRotationAngle) *
                                   XMMatrixTranslation(m_pCamera->Position().x,
                                                       m_pCamera->Position().y,
                                                       m_pCamera->Position().z) ;
        // translate world +6 in Z to position camera -9 from world origin
        pCameraData->viewMatrix = m_pCamera->W2C();
        pCameraData->projectionMatrix = m_pCamera->C2S();

        pCameraBuffer->Unmap(pContext);

        pCameraBuffer->BindVS(pContext, 0);

        pMesh->Bind(pContext);

        m_pPosTexNormVS->Bind(pContext);
        pCubePS->Bind(pContext);

        pMesh->Draw(pContext);


        ///// Draw Light Position ////////////////////////////////////////////////////////////////////

        //yRotationAngle = 0;
        pCameraData = reinterpret_cast<CameraBufferData*>(pCameraBuffer->Map(pContext));
        pCameraData->worldMatrix = XMMatrixScaling(1, 1, 1);
                                 //  XMMatrixRotationY(yRotationAngle);
                                  // XMMatrixTranslation(-10, 10, 10);
        // translate world +6 in Z to position camera -9 from world origin
        pCameraData->viewMatrix = XMMatrixTranslation(0, 0, 10) * m_pCamera->W2C();
        pCameraData->projectionMatrix = m_pCamera->C2S();

        pCameraBuffer->Unmap(pContext);
        pCameraBuffer->BindVS(pContext, 0);

        // Draw Cube
        pCubeVS->Bind(pContext);
        pCubePS->Bind(pContext);

        pCubeMesh->Bind(pContext);
        pCubeMesh->Draw(pContext);

        ///@todo Draw UI


        m_pDxData->pDXGISwapChain->Present(0,0);

        EndFrame();

        Sleep(50);
        yRotationAngle += 3.14159f / 60.0f;
    }

    // Shader Resource Views
    pCameraBuffer->Destroy();

    // Shaders
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

    pContext->ClearState();
    pContext->Flush(); 
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// InstancingApp::ReceiveEvent
///////////////////////////////////////////////////////////////////////////////////////////////////
void InstancingApp::ReceiveEvent(
    const Event* pEvent)
{
    if (pEvent->GetType() == EventTypeKeyDown)
    {
        HandleKeyboardInput(pEvent);
    }
    else
    {
        IvyApp::ReceiveEvent(pEvent);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// InstancingApp::HandleKeyboardInput
///////////////////////////////////////////////////////////////////////////////////////////////////
void InstancingApp::HandleKeyboardInput(
    const Event* pEvent)
{
    if (pEvent->GetType() == EventTypeKeyDown)
    {
        const EventKeyDown* pKeyDownEvent = reinterpret_cast<const EventKeyDown*>(pEvent);

        switch(pKeyDownEvent->GetData().key)
        {
            case EKeyR:
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

