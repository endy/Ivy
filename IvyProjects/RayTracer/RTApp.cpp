

#include "RTApp.h"

#include "IvyUtils.h"

#include "DxTypes.h"
#include "DxVertexShader.h"
#include "DxPixelShader.h"
#include "DxBuffer.h"
#include "DxLight.h"
#include "DxMesh.h"
#include "DxTexture.h"

RTApp::RTApp(RTAppCreateInfo* pAppInfo)
    :
    DxApp(pAppInfo)
{

}

RTApp::~RTApp()
{

}

RTApp* RTApp::Create(RTAppCreateInfo* pAppInfo)
{
    RTApp* pApp = new RTApp(pAppInfo);

    if (pApp->Init() == false)
    {
        pApp->Destroy();
        pApp = NULL;
    }

    return pApp;
}

void RTApp::Destroy()
{
    DxApp::Destroy();
}

bool RTApp::Init()
{
    return DxApp::Init();
}

void RTApp::Run()
{
    BOOL statusOK = TRUE;
    HRESULT hr;

    // Resources ////////////////////////////////////////////////////////////////////////////////////

    ID3D11Texture2D* pBackBuffer = NULL;

    // Get a pointer to the back buffer
    hr = m_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (LPVOID*)&pBackBuffer );

    ID3D11RenderTargetView* pRenderTargetView = NULL;

    // Create a render-target view
    m_pDevice->CreateRenderTargetView( pBackBuffer, NULL, &pRenderTargetView );

    // Quad Mesh
    Plane p;
    DxMeshCreateInfo planeMeshInfo;
    memset(&planeMeshInfo, 0, sizeof(DxMeshCreateInfo));

    planeMeshInfo.vertexCount = p.NumVertices();
    planeMeshInfo.pVertexArray = p.GetVB();
    planeMeshInfo.vertexElementSize = sizeof(VertexPTN);

    DxMesh* planeMesh = DxMesh::Create(m_pDevice, &planeMeshInfo);

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

    // Light
    DxLightCreateInfo lightInfo;
    memset(&lightInfo, 0, sizeof(DxLightCreateInfo));
    lightInfo.pDevice = m_pDevice;
    lightInfo.pContext = m_pContext;
    DxLight* pLight = DxLight::Create(&lightInfo);

    // Eye Vector Texture
    DxTextureCreateInfo hwRtEyeTexInfo;
    memset(&hwRtEyeTexInfo, 0, sizeof(DxTextureCreateInfo));

    hwRtEyeTexInfo.flags.CpuWrite = TRUE;
    hwRtEyeTexInfo.flags.ShaderInput = TRUE;
    hwRtEyeTexInfo.format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    hwRtEyeTexInfo.width = m_screenWidth;
    hwRtEyeTexInfo.height = m_screenHeight;

    DxColorTexture* pHwRtEyeTexture = DxColorTexture::Create(m_pDevice, &hwRtEyeTexInfo);

    // SW RT Image Texture
    DxTextureCreateInfo swRtImageTexInfo;
    memset(&swRtImageTexInfo, 0, sizeof(DxTextureCreateInfo));

    swRtImageTexInfo.flags.CpuWrite = TRUE;
    swRtImageTexInfo.flags.ShaderInput = TRUE;
    swRtImageTexInfo.format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    swRtImageTexInfo.width = m_screenWidth;
    swRtImageTexInfo.height = m_screenHeight;

    DxColorTexture* pSwRtImage = DxColorTexture::Create(m_pDevice, &swRtImageTexInfo);

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
    hr = m_pDevice->CreateSamplerState(&samplerDesc, &pSamplerState);
    
    // Shaders ////////////////////////////////////////////////////////////////////////////////////

    DxVertexShader* pVSShader = DxVertexShader::CreateFromFile(m_pDevice, "RtVS", "rtshaders.txt",  PosTexVertexDesc, PosTexElements);
    statusOK = (statusOK && pVSShader != NULL);

    DxPixelShader* pPSShader = DxPixelShader::CreateFromFile(m_pDevice, "RtPS", "rtshaders.txt");
    statusOK = (statusOK && pPSShader != NULL);

    ////////////////////////////////////////////////////////////////////////////////////////

    m_pContext->ClearState();

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
    m_pDevice->CreateRasterizerState(&rsDesc, &pRasterizerState);

    m_pWindow->DisplayWindow();

    BOOL quit = false;
    while (statusOK && !quit)
    {            
        m_pWindow->ProcessMsg(&quit);

        // New frame, clear state
        m_pContext->ClearState();

        // Setup Raster State
        m_pContext->RSSetViewports(1, &vp);
        m_pContext->RSSetState(pRasterizerState);

        // Begin Draw Pass
        SetupHwRtEyeTexture(pHwRtEyeTexture);
        Render(pSwRtImage);

        m_pContext->ClearRenderTargetView(pRenderTargetView, clearColor);

        m_pContext->OMSetRenderTargets( 1, &pRenderTargetView, NULL );
        
        // Setup Camera
        CameraBufferData* pCameraBufferData = NULL;

        pCameraBufferData = static_cast<CameraBufferData*>(pCameraBuffer->Map(m_pContext));
        pCameraBufferData->worldMatrix =  XMMatrixTranslation(0, 0, 0) * 
                                          XMMatrixRotationX(-3.14f/2.0f) *
                                          XMMatrixScaling(1, 1, 1) *
                                          XMMatrixTranslation(0, 0, 1);
        pCameraBufferData->viewMatrix       = m_pCamera->W2C();
        pCameraBufferData->projectionMatrix = m_pCamera->C2S();
        pCameraBuffer->Unmap(m_pContext);

        // Setup VS
        pCameraBuffer->BindVS(m_pContext, 0);
        pVSShader->Bind(m_pContext);

        // Setup PS
        ID3D11Buffer* pLightBuffer = pLight->GetBuffer();
        m_pContext->PSSetConstantBuffers(0, 1, &pLightBuffer);

        ID3D11ShaderResourceView* pTexView = pHwRtEyeTexture->GetShaderResourceView();
        m_pContext->PSSetShaderResources(0, 1, &pTexView);

        m_pContext->PSSetSamplers(0, 1, &pSamplerState);

        pPSShader->Bind(m_pContext);

        // Draw Quad
        m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        planeMesh->Bind(m_pContext);
        planeMesh->Draw(m_pContext);

        // Present
        m_pSwapChain->Present(0,0);

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

    m_pContext->ClearState();
    m_pContext->Flush();
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
    ///@ TODO: Take into account padding!
    FLOAT* texData = reinterpret_cast<FLOAT*>(pImage->Lock(m_pContext));
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
    pImage->Unlock(m_pContext);
}

void RTApp::Render(
    DxTexture* pImage)
{
    FLOAT* texData = reinterpret_cast<FLOAT*>(pImage->Lock(m_pContext));
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
    pImage->Unlock(m_pContext);
}

