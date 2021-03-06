

#include "RTApp.h"
#include "IvyWindow.h"
#include "IvyCamera.h"

#include "IvyUtils.h"

#include "DxTypes.h"
#include "IvyDX.h"
#include "DxShader.h"
#include "DxBuffer.h"
#include "DxLight.h"
#include "DxMesh.h"
#include "DxTexture.h"

RTApp::RTApp()
    :
    IvyApp()
{

}

RTApp::~RTApp()
{

}

RTApp* RTApp::Create()
{
    RTApp* pApp = new RTApp();

    if (pApp->Init() == false)
    {
        pApp->Destroy();
        pApp = NULL;
    }

    return pApp;
}

void RTApp::Destroy()
{
    IvyApp::DeinitDX();

    IvyApp::Destroy();
}

bool RTApp::Init()
{
    return (IvyApp::Init() && InitDX());
}

void RTApp::Run()
{
    BOOL statusOK = TRUE;
    HRESULT hr;

    ID3D11DeviceContext* pContext = m_pDxData->pD3D11Context;
    ID3D11Device* pDevice = m_pDxData->pD3D11Device;

    // Resources ////////////////////////////////////////////////////////////////////////////////////

    ID3D11Texture2D* pBackBuffer = NULL;

    // Get a pointer to the back buffer
    hr = m_pDxData->pDXGISwapChain->GetBuffer(0, __uuidof( ID3D11Texture2D ), (LPVOID*)&pBackBuffer);

    ID3D11RenderTargetView* pRenderTargetView = NULL;

    // Create a render-target view
    pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);

    // Quad Mesh
    Plane p;
    DxMeshCreateInfo planeMeshInfo;
    memset(&planeMeshInfo, 0, sizeof(DxMeshCreateInfo));

    planeMeshInfo.vertexCount = p.NumVertices();
    planeMeshInfo.pVertexArray = p.GetVB();
    planeMeshInfo.vertexElementSize = sizeof(VertexPTN);

    DxMesh* planeMesh = DxMesh::Create(pDevice, &planeMeshInfo);

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

    // Light
    DxLightCreateInfo lightInfo;
    memset(&lightInfo, 0, sizeof(DxLightCreateInfo));
    lightInfo.pDevice = pDevice;
    lightInfo.pContext = pContext;
    DxLight* pLight = DxLight::Create(&lightInfo);

    // Eye Vector Texture
    DxTextureCreateInfo hwRtEyeTexInfo;
    memset(&hwRtEyeTexInfo, 0, sizeof(DxTextureCreateInfo));

    hwRtEyeTexInfo.flags.CpuWrite = TRUE;
    hwRtEyeTexInfo.flags.ShaderInput = TRUE;
    hwRtEyeTexInfo.format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    hwRtEyeTexInfo.width = m_screenWidth;
    hwRtEyeTexInfo.height = m_screenHeight;

    DxTexture* pHwRtEyeTexture = DxTexture::Create(pDevice, &hwRtEyeTexInfo);

    // SW RT Image Texture
    DxTextureCreateInfo swRtImageTexInfo;
    memset(&swRtImageTexInfo, 0, sizeof(DxTextureCreateInfo));

    swRtImageTexInfo.flags.CpuWrite = TRUE;
    swRtImageTexInfo.flags.ShaderInput = TRUE;
    swRtImageTexInfo.format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    swRtImageTexInfo.width = m_screenWidth;
    swRtImageTexInfo.height = m_screenHeight;

    DxTexture* pSwRtImage = DxTexture::Create(pDevice, &swRtImageTexInfo);

    // Image Sampler
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
    hr = pDevice->CreateSamplerState(&samplerDesc, &pSamplerState);
    
    // Shaders ////////////////////////////////////////////////////////////////////////////////////

    DxShader* pVSShader = DxShader::CreateFromFile(pDevice, "RtVS", "rtshaders.txt",  PosTexVertexDesc, PosTexElements);
    statusOK = (statusOK && pVSShader != NULL);

    DxShader* pPSShader = DxShader::CreateFromFile(pDevice, "RtPS", "rtshaders.txt");
    statusOK = (statusOK && pPSShader != NULL);

    ////////////////////////////////////////////////////////////////////////////////////////

    pContext->ClearState();

    // SET RENDER STATE

    FLOAT clearColor[4];
    clearColor[0] = 1.0f;
    clearColor[1] = 0.0f;
    clearColor[2] = 0.0f; 
    clearColor[3] = 1.0f;

    // Setup the viewport
    D3D11_VIEWPORT vp;
    vp.Width = static_cast<FLOAT>(m_screenWidth);
    vp.Height = static_cast<FLOAT>(m_screenHeight);
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;

    D3D11_RASTERIZER_DESC rsDesc;
    rsDesc.FillMode = D3D11_FILL_SOLID;
    rsDesc.CullMode = D3D11_CULL_NONE;
    rsDesc.FrontCounterClockwise = FALSE;
    rsDesc.DepthBias = 0;
    rsDesc.DepthBiasClamp = 0.0f;
    rsDesc.SlopeScaledDepthBias = 0;
    rsDesc.DepthClipEnable = true;
    rsDesc.ScissorEnable = false;
    rsDesc.MultisampleEnable = false;
    rsDesc.AntialiasedLineEnable = false;

    ID3D11RasterizerState* pRasterizerState = NULL;
    pDevice->CreateRasterizerState(&rsDesc, &pRasterizerState);

    m_pWindow->Show();

    BOOL quit = false;
    while (statusOK && !quit)
    {            
        m_pWindow->ProcessMsg(&quit);

        // New frame, clear state
        pContext->ClearState();

        // Setup Raster State
        pContext->RSSetViewports(1, &vp);
        pContext->RSSetState(pRasterizerState);

        // Begin Draw Pass
        SetupHwRtEyeTexture(pHwRtEyeTexture);
        Render(pSwRtImage);

        pContext->ClearRenderTargetView(pRenderTargetView, clearColor);

        pContext->OMSetRenderTargets( 1, &pRenderTargetView, NULL );
        
        // Setup Camera
        CameraBufferData* pCameraBufferData = NULL;

        pCameraBufferData = static_cast<CameraBufferData*>(pCameraBuffer->Map(pContext));
        pCameraBufferData->worldMatrix =  XMMatrixTranslation(0, 0, 0) * 
                                          XMMatrixRotationX(-3.14f/2.0f) *
                                          XMMatrixScaling(1, 1, 1) *
                                          XMMatrixTranslation(0, 0, 1);
        pCameraBufferData->viewMatrix       = m_pCamera->W2C();
        pCameraBufferData->projectionMatrix = m_pCamera->C2S();
        pCameraBuffer->Unmap(pContext);

        // Setup VS
        pCameraBuffer->BindVS(pContext, 0);
        pVSShader->Bind(pContext);

        // Setup PS
        ID3D11Buffer* pLightBuffer = pLight->GetBuffer();
        pContext->PSSetConstantBuffers(0, 1, &pLightBuffer);

        ID3D11ShaderResourceView* pTexView = pHwRtEyeTexture->GetShaderResourceView();
        pContext->PSSetShaderResources(0, 1, &pTexView);

        pContext->PSSetSamplers(0, 1, &pSamplerState);

        pPSShader->Bind(pContext);

        // Draw Quad
        pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        planeMesh->Bind(pContext);
        planeMesh->Draw(pContext);

        // Present
        m_pDxData->pDXGISwapChain->Present(0,0);

        //Sleep(100); 
    }


    if (pLight != NULL)
    {
        pLight->Destroy();
        pLight = NULL;
    }

    if (pCameraBuffer != NULL)
    {
        pCameraBuffer->Destroy();
        pCameraBuffer = NULL;
    }

    if (pRenderTargetView != NULL)
    {
        pRenderTargetView->Release();
        pRenderTargetView = NULL;
    }

    if (pVSShader != NULL)
    {
        pVSShader->Destroy();
        pVSShader = NULL;
    }

    if (pPSShader != NULL)
    {
        pPSShader->Destroy();
        pPSShader = NULL;
    }

    if (pRasterizerState != NULL)
    {
        pRasterizerState->Release();
        pRasterizerState = NULL;
    }

    pContext->ClearState();
    pContext->Flush();
}

// width, height, x, y, origin,

Point3 SampleScreen(UINT x, UINT y, UINT width, UINT height)
{
    Point3 origin = Point3(0.0f, 0.0f, 0.0f);

    Point3 eyeVector;
    eyeVector.x = (x / (FLOAT)width) - 0.5f;
    eyeVector.y = (y / (FLOAT)height) - 0.5f;
    eyeVector.z = 1;

    return eyeVector;
}

void RTApp::SetupHwRtEyeTexture(
    DxTexture* pImage)
{
    /// Update Image!
    ///@todo: Take into account padding!
    FLOAT* texData = reinterpret_cast<FLOAT*>(pImage->Lock(m_pDxData->pD3D11Context));
    for (UINT x = 0; x < m_screenWidth; ++x)
    {
        for (UINT y = 0; y < m_screenHeight; ++y)
        {
            Point3 vector = SampleScreen(x, y, m_screenWidth, m_screenHeight);
            UINT index = (y * m_screenWidth + x) * 4;
            texData[index+0] = vector.x;
            texData[index+1] = vector.y;
            texData[index+2] = vector.z;
            texData[index+3] = 1.0f;
        }
    }
    pImage->Unlock(m_pDxData->pD3D11Context);
}

void RTApp::Render(
    DxTexture* pImage)
{
    FLOAT* texData = reinterpret_cast<FLOAT*>(pImage->Lock(m_pDxData->pD3D11Context));
    for (UINT x = 0; x < m_screenWidth; ++x)
    {
        for (UINT y = 0; y < m_screenHeight; ++y)
        {
            Point3 vector = SampleScreen(x, y, m_screenWidth, m_screenHeight);

            UINT index = (y * m_screenWidth + x) * 4;
            texData[index+0] = vector.x;
            texData[index+1] = vector.y;
            texData[index+2] = vector.z;
            texData[index+3] = 1.0f;
        }
    }
    pImage->Unlock(m_pDxData->pD3D11Context);
}

