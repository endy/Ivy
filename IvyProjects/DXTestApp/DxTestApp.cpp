///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine - Generic DirectX Test App
///
///     Copyright 2010-2011, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////


#include "DxTestApp.h"
#include "IvyWindow.h"
#include "IvyCamera.h"

#include "DxShader.h"
#include "DxTexture.h"
#include "DxBuffer.h"
#include "DxMesh.h"
#include "IvyUtils.h"

#include "DxUI.h"

#include "IvyImporter.h"
#include "DxLight.h"

DxTestApp::DxTestApp()
    :
    DxApp()
{

}

DxTestApp::~DxTestApp()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxTestApp::Create
///
/// @brief
///     Creates the DX Test App instance
/// @return
///     DxTestApp instance
///////////////////////////////////////////////////////////////////////////////////////////////////
DxTestApp* DxTestApp::Create()
{
    DxTestApp* pApp = new DxTestApp();

    if (pApp->Init() == false)
    {
        pApp->Destroy();
        pApp = NULL;
    }

    return pApp;
}

void DxTestApp::Destroy()
{
    DxApp::Destroy();
}

bool DxTestApp::Init()
{
    bool success = DxApp::Init();

    return success;
}

void DxTestApp::Run()
{
    IVY_PRINT("DxTestApp D3D11 Path");

    HRESULT hr;

    // Resources ////////////////////////////////////////////////////////////////////////////////////   

    D3DX11_IMAGE_LOAD_INFO kittenLoadInfo;
    memset( &kittenLoadInfo, 0, sizeof(D3DX11_IMAGE_LOAD_INFO) );
    kittenLoadInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    kittenLoadInfo.Format = DXGI_FORMAT_BC1_UNORM;

    ID3D11ShaderResourceView *pKittenSRView = NULL;
    D3DX11CreateShaderResourceViewFromFile(m_pDevice, "Content/kitten_rgb.dds", &kittenLoadInfo, NULL, &pKittenSRView, NULL );


    // Samplers  /////////////////////////////////////////////////////////////////////////////

    D3D11_SAMPLER_DESC samplerDesc;
    memset(&samplerDesc, 0, sizeof(D3D11_SAMPLER_DESC));
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.MinLOD = -FLT_MAX;
    samplerDesc.MaxLOD = FLT_MAX;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 16;

    ID3D11SamplerState* pSamplerState = NULL;
    hr = m_pDevice->CreateSamplerState(&samplerDesc, &pSamplerState);

    // Buffers   ///////////////////////////////////////////////////////////////////////////

    // Camera Buffer
    CameraBufferData cameraData;
    memset(&cameraData, 0, sizeof(CameraBufferData));

    DxBufferCreateInfo cameraBufferCreateInfo = {0};
    cameraBufferCreateInfo.flags.cpuWriteable = TRUE;
    cameraBufferCreateInfo.elemSizeBytes = sizeof(CameraBufferData);
    cameraBufferCreateInfo.pInitialData = &cameraData;
    DxBuffer* pCameraBuffer = DxBuffer::Create(m_pDevice, &cameraBufferCreateInfo);

    // Light
    DxLightCreateInfo lightInfo;
    memset(&lightInfo, 0, sizeof(DxLightCreateInfo));
    lightInfo.pDevice = m_pDevice;
    lightInfo.pContext = m_pContext;
    DxLight* pLight = DxLight::Create(&lightInfo);

    // Material
    D3D11_BUFFER_DESC cbMaterialDesc;
    memset(&cbMaterialDesc, 0, sizeof(D3D11_BUFFER_DESC));
    cbMaterialDesc.Usage = D3D11_USAGE_DYNAMIC;
    cbMaterialDesc.ByteWidth = sizeof( Material );
    cbMaterialDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbMaterialDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cbMaterialDesc.MiscFlags = 0; 

    D3D11_SUBRESOURCE_DATA cbInitData;

    Material myMaterial;
    memset(&myMaterial, 0, sizeof(myMaterial));
    cbInitData.pSysMem = &myMaterial;

    ID3D11Buffer* pMaterialBuffer = NULL;
    m_pDevice->CreateBuffer(&cbMaterialDesc, &cbInitData, &pMaterialBuffer);

    // Shaders ////////////////////////////////////////////////////////////////////////////////////

    DxShader* pPosTexVS = DxShader::CreateFromFile(m_pDevice, "PosTex", "DXTestApp/dxtestapp.hlsl", PosTexVertexDesc, PosTexElements);
    DxShader* pApplyTexPS = DxShader::CreateFromFile(m_pDevice, "ApplyTexture", "DXTestApp/dxtestapp.hlsl");
    DxShader* pVisDepthPS = DxShader::CreateFromFile(m_pDevice, "VisDepth", "DXTestApp/dxtestapp.hlsl");

    DxShader* pPosTexNormVS = DxShader::CreateFromFile(m_pDevice, "PosTexNorm", "DXTestApp/dxtestapp.hlsl", PosTexNormVertexDesc, PosTexNormElements);
    DxShader* pVisNormalPS = DxShader::CreateFromFile(m_pDevice, "VisNormal", "DXTestApp/dxtestapp.hlsl");

    // Models /////////////////////////////////////////////////////////////////////////////////////

    // Cube
    Cube c;
    DxMeshCreateInfo cubeMeshInfo;
    memset(&cubeMeshInfo, 0, sizeof(cubeMeshInfo));	
    cubeMeshInfo.pVertexArray = c.GetVB();
    cubeMeshInfo.vertexCount = c.NumVertices();
    cubeMeshInfo.vertexElementSize = sizeof(VertexPT);

    DxMesh* pCubeMesh = DxMesh::Create(m_pDevice, &cubeMeshInfo);   

    // Plane
    Plane p;
    DxMeshCreateInfo planeMeshInfo;
    memset(&planeMeshInfo, 0, sizeof(planeMeshInfo));
    planeMeshInfo.pVertexArray = p.GetVB();
    planeMeshInfo.vertexCount = p.NumVertices();
    planeMeshInfo.vertexElementSize = sizeof(VertexPTN);

    DxMesh* pPlaneMesh = DxMesh::Create(m_pDevice, &planeMeshInfo);

    // Bunny
    DxMeshCreateInfo meshCreateInfo;
    memset(&meshCreateInfo, 0, sizeof(DxMeshCreateInfo));

    VertexPTN* pVB = NULL;
    UINT* pIB = NULL;

    Import(meshCreateInfo.vertexCount, &pVB, meshCreateInfo.indexCount, &pIB);

    meshCreateInfo.pVertexArray = pVB;
    meshCreateInfo.pIndexArray = pIB;
    meshCreateInfo.vertexElementSize = sizeof(VertexPTN);
    meshCreateInfo.indexFormat = DXGI_FORMAT_R32_UINT;

    DxMesh* pBunnyMesh = DxMesh::Create(m_pDevice, &meshCreateInfo);

    ////////////////////////////////////////////////////////////////////////////////////////

    m_pContext->ClearState();

    // SET RENDER STATE

    Point3 rotation = Point3(0.0f, 0.0f, 0.0f);
    FLOAT clearColor[4];
    clearColor[0] = 0.4f;
    clearColor[1] = 1.0f;
    clearColor[2] = 0.4f; 
    clearColor[3] = 1.0f;

    FLOAT depthClearValue = 1.0f;
    UINT8 stencilClearValue = 0x0;

    D3D11_DEPTH_STENCIL_DESC dbDesc;

    memset(&dbDesc, 0, sizeof(D3D11_DEPTH_STENCIL_DESC));
    dbDesc.DepthEnable = TRUE;
    dbDesc.DepthFunc = D3D11_COMPARISON_LESS;
    dbDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dbDesc.StencilEnable = TRUE;
    dbDesc.StencilWriteMask = 0xFF;
    dbDesc.StencilReadMask = 0xFF;
    dbDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    dbDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
    dbDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    dbDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_INCR;
    dbDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    dbDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
    dbDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    dbDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_INCR;

    ID3D11DepthStencilState* pDbState = NULL;
    hr = m_pDevice->CreateDepthStencilState(&dbDesc, &pDbState);

    D3D11_RASTERIZER_DESC rsDesc;
    rsDesc.FillMode = D3D11_FILL_SOLID;
    rsDesc.CullMode = D3D11_CULL_NONE;
    rsDesc.FrontCounterClockwise = FALSE;
    rsDesc.DepthBias = 0;
    rsDesc.DepthBiasClamp = 0.0f;
    rsDesc.SlopeScaledDepthBias = 0;
    rsDesc.DepthClipEnable = false;
    rsDesc.ScissorEnable = false;
    rsDesc.MultisampleEnable = false;
    rsDesc.AntialiasedLineEnable = false;

    ID3D11RasterizerState* pRasterizerState = NULL;
    m_pDevice->CreateRasterizerState(&rsDesc, &pRasterizerState);

    // Set Depth Stencil State
    UINT StencilRef = 0x0;
    m_pContext->OMSetDepthStencilState(pDbState, StencilRef);

    m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_pContext->RSSetState(pRasterizerState);
    m_pContext->PSSetSamplers(0, 1, &pSamplerState);


    m_pContext->ClearDepthStencilView(m_pDepthStencilBuffer->GetDepthStencilView(), 
        D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 
        depthClearValue, 
        stencilClearValue); 

    CameraBufferData* pCameraBufferData = NULL; ;
    Material* pMaterialCB = NULL;   

    m_pWindow->Show();

    /* // IHV-independent counters were removed from D3D11 (change from D3D10)
    D3D11_COUNTER_DESC counterDesc;
    memset(&counterDesc, 0, sizeof(D3D11_COUNTER_DESC));

    ID3D11Counter* pCounter = NULL;
    m_pDevice->CreateCounter(&counterDesc, &pCounter);
    //*/

    D3D11_QUERY_DESC queryDesc;
    memset(&queryDesc, 0, sizeof(D3D11_QUERY_DESC));

    queryDesc.Query = D3D11_QUERY_PIPELINE_STATISTICS;

    ID3D11Query* pQuery = NULL;
    m_pDevice->CreateQuery(&queryDesc, &pQuery);


    ID3D11ShaderResourceView* pUI_SRV = NULL;

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvDesc.Texture2D.MipLevels = 1;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

    m_pDevice->CreateShaderResourceView(m_pUIoverlay, &srvDesc, &pUI_SRV);

    D3D11_BLEND_DESC blendDesc;
    memset(&blendDesc, 0, sizeof(D3D11_BLEND_DESC));

    blendDesc.AlphaToCoverageEnable = FALSE;
    blendDesc.IndependentBlendEnable = FALSE;
    blendDesc.RenderTarget[0].BlendEnable = TRUE;

    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    ID3D11BlendState* pBlendState = NULL;
    hr = m_pDevice->CreateBlendState(&blendDesc, &pBlendState);

    BOOL quit = false;
    while (!quit)
    {            
        m_pWindow->ProcessMsg(&quit);

        BeginFrame();

        // new frame, clear state
        m_pContext->ClearState();

        m_pContext->RSSetViewports(1, &m_viewport);
        m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        pCameraBuffer->BindVS(m_pContext, 0);

        ID3D11Buffer* pLightBuffer = pLight->GetBuffer();
        m_pContext->PSSetConstantBuffers(0, 1, &pLightBuffer);

        m_pContext->PSSetConstantBuffers(1, 1, &pMaterialBuffer);
        m_pContext->RSSetState(pRasterizerState);

        m_pContext->PSSetSamplers(0, 1, &pSamplerState);

        m_pContext->OMSetDepthStencilState(pDbState, StencilRef);

        // UPDATE RENDER STATE
        m_pContext->ClearRenderTargetView(m_pRenderTargetView, clearColor);
        m_pContext->ClearDepthStencilView(m_pDepthStencilBuffer->GetDepthStencilView(), 
            D3D11_CLEAR_DEPTH, 
            depthClearValue, 
            stencilClearValue);

        // UPDATE SCENE

        //m_pContext->Begin(pQuery);

        // DRAW CUBE /////////////////////////////   

        // update cube matrix
        pCameraBufferData = reinterpret_cast<CameraBufferData*>(pCameraBuffer->Map(m_pContext));
        pCameraBufferData->worldMatrix      = XMMatrixRotationX(-3.14f/2.0f) * XMMatrixScaling(2, 2, 1); //XMMatrixIdentity();
        pCameraBufferData->viewMatrix       = XMMatrixTranslation(0, 0, 3.0f) * m_pCamera->W2C(); 
        pCameraBufferData->projectionMatrix = m_pCamera->C2S(); 
        pCameraBuffer->Unmap(m_pContext);

        pPosTexVS->Bind(m_pContext);

        // Bind the view -- note: if either views are bound as resources, this generates warnings
        // calling set render target here after the previous set shader res' calls get rid of the warnings

        m_pContext->OMSetRenderTargets( 1, &m_pRenderTargetView, m_pDepthStencilBuffer->GetDepthStencilView());

        m_pContext->PSSetShaderResources(0, 1, &pKittenSRView);
        pApplyTexPS->Bind(m_pContext);

        pPlaneMesh->Bind(m_pContext);
        //pPlaneMesh->Draw(m_pContext);

        pCubeMesh->Bind(m_pContext);
        //pCubeMesh->Draw(m_pContext);

        // DRAW BUNNY WITH CAMERA /////////////////////////////

        pCameraBufferData = reinterpret_cast<CameraBufferData*>(pCameraBuffer->Map(m_pContext));
        pCameraBufferData->worldMatrix = XMMatrixScaling(5, 5, 1); // * XMMatrixRotationY(rotation.x);
        
        // translate world +6 in Z to position camera -9 from world origin
        pCameraBufferData->viewMatrix = XMMatrixIdentity(); 
        pCameraBufferData->viewMatrix = XMMatrixTranslation(0, 0, 2.0f) * m_pCamera->W2C(); 
        
        pCameraBufferData->projectionMatrix = m_pCamera->C2S();
        
        pCameraBuffer->Unmap(m_pContext);

        pPosTexNormVS->Bind(m_pContext);
        pVisNormalPS->Bind(m_pContext);

        pBunnyMesh->Bind(m_pContext);
        pBunnyMesh->Draw(m_pContext);

        // VISUALIZE DEPTH/STENCIL /////////////////////////////  
        //*
        m_pContext->OMSetRenderTargets( 1, &m_pRenderTargetView, NULL );

        pCameraBufferData = reinterpret_cast<CameraBufferData*>(pCameraBuffer->Map(m_pContext));
        pCameraBufferData->worldMatrix      = XMMatrixRotationX(-3.14f/2.0f) * XMMatrixScaling(2, 2, 1);
        pCameraBufferData->viewMatrix       = XMMatrixTranslation(0, 0, 4.0f) * m_pCamera->W2C(); 
        pCameraBufferData->projectionMatrix = m_pCamera->C2S();
        pCameraBuffer->Unmap(m_pContext);


        pPosTexVS->Bind(m_pContext);

        ID3D11ShaderResourceView* pDepthView = m_pDepthStencilBuffer->GetDepthResourceView();
        m_pContext->PSSetShaderResources(0, 1, &pDepthView);
        ID3D11ShaderResourceView* pStencilView = m_pDepthStencilBuffer->GetStencilResourceView();
        m_pContext->PSSetShaderResources(1, 1, &pStencilView);

        pVisDepthPS->Bind(m_pContext);

        pPlaneMesh->Bind(m_pContext);
        //pPlaneMesh->Draw(m_pContext);
        //*/

        //m_pContext->End(pQuery);

        // Perf Query
        D3D11_QUERY_DATA_PIPELINE_STATISTICS pipelineStats;
        memset(&pipelineStats, 0, sizeof(D3D11_QUERY_DATA_PIPELINE_STATISTICS));

        //while (S_OK != m_pContext->GetData(pQuery, &pipelineStats, sizeof(D3D11_QUERY_DATA_PIPELINE_STATISTICS), 0))
        {

        }


        // Draw UI
        m_pUI->Begin();
        m_pUI->RenderRect();
        m_pUI->RenderText();
        m_pUI->End();
        DrawUI();

        m_pSwapChain->Present(0,0);

        // cannot set clearstate here because we are not setting all correct state in render loop
        //pContext->ClearState();

        rotation.x += 0.002f;
        rotation.y += 0.13f;

        EndFrame();
    }


    if (pCubeMesh != NULL)
    {
        pCubeMesh->Destroy();
        pCubeMesh = NULL;
    }

    if (pPlaneMesh != NULL)
    {
        pPlaneMesh->Destroy();
        pPlaneMesh = NULL;
    }

    if (pBunnyMesh != NULL)
    {
        pBunnyMesh->Destroy();
        pBunnyMesh = NULL;
    }

    if (pLight != NULL)
    {
        pLight->Destroy();
        pLight = NULL;
    }

    if (pCameraBuffer != NULL)
    {
        // Buffers
        pCameraBuffer->Destroy();
    }

    pMaterialBuffer->Release(); 

    // Texture2D
    //pDynamic->Release();

    // Shader Resource Views
    pKittenSRView->Release();

    // DepthStencilViews

    // Vertex Shaders
    pPosTexVS->Destroy();
    pPosTexNormVS->Destroy();  

    // Pixel Shaders
    pApplyTexPS->Destroy();
    pVisNormalPS->Destroy();
    pVisDepthPS->Destroy();

    // Samplers
    pSamplerState->Release();

    // Depth Stencil State
    pDbState->Release();  

    // Rasterizer State
    pRasterizerState->Release();


    if (pQuery != NULL)
    {
        pQuery->Release();
        pQuery = NULL;
    }

    m_pContext->ClearState();
    m_pContext->Flush(); 
}
