

#include "MlaaApp.h"
#include "DxShader.h"
#include "DxShader.h"
#include "DxTexture.h"
#include "DxMesh.h"
#include "IvyUtils.h"

MlaaApp* MlaaApp::Create(MlaaAppCreateInfo* pAppInfo)
{
    return new MlaaApp(pAppInfo);
}

MlaaApp::MlaaApp(MlaaAppCreateInfo* pAppInfo)
    :
    DxApp(pAppInfo),
    m_linearZoomFilter(false)
{
    m_passDisplayed = MlaaPassPrePass;
}

MlaaApp::~MlaaApp()
{

}

void MlaaApp::Destroy()
{

}


void MlaaApp::Run()
{
    bool success = DxApp::Init();

    // Texture2D <float4> colorTex : register(t0);

    D3DX11_IMAGE_LOAD_INFO imageLoadInfo;
    memset( &imageLoadInfo, 0, sizeof(D3DX11_IMAGE_LOAD_INFO) );
    imageLoadInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    imageLoadInfo.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    ID3D11ShaderResourceView *pColorTexSRV = NULL;
    D3DX11CreateShaderResourceViewFromFile(m_pDevice, "content/bc3.dds", &imageLoadInfo, NULL, &pColorTexSRV, NULL );

    UINT colorTexWidth = 1680;
    UINT colorTexHeight = 1050;

    // Texture2D <float4> edgesTex : register(t1);

    DxTextureCreateInfo edgesTexInfo;
    memset(&edgesTexInfo, 0, sizeof(DxTextureCreateInfo));

    edgesTexInfo.flags.RenderTarget = TRUE;
    edgesTexInfo.flags.ShaderInput = TRUE;
    edgesTexInfo.format = DXGI_FORMAT_R8G8_UNORM;
    edgesTexInfo.width = colorTexWidth;
    edgesTexInfo.height = colorTexHeight;
    
    DxTexture* pEdgesTex = DxTexture::Create(m_pDevice, &edgesTexInfo);

    // Texture2D <float4> areaTex : register(t2);

    const UINT PatternWidth = 5;
    const UINT PatternHeight = 5;
    const UINT MaxDistance = 8;

    DxTextureCreateInfo areaTexInfo;
    memset(&areaTexInfo, 0, sizeof(DxTextureCreateInfo));

    areaTexInfo.flags.CpuWrite = TRUE;
    areaTexInfo.flags.ShaderInput = TRUE;
    areaTexInfo.format = DXGI_FORMAT_R8G8_UNORM;
      // 5 x 5 x (MaxDistX+1) x (MaxDistY+1)
    areaTexInfo.width = PatternWidth * (MaxDistance+1);
    areaTexInfo.height = PatternHeight * (MaxDistance+1);
    
    DxTexture* pAreaTex = DxTexture::Create(m_pDevice, &areaTexInfo);
    
    
    BYTE* pAreaElement = static_cast<BYTE*>(pAreaTex->Lock(m_pContext));

    for (UINT ey = 0; ey < PatternHeight; ey++)
    {
        for (UINT ex = 0; ex < PatternWidth; ex++)
        {
            for (UINT y = 0; y < (MaxDistance+1); ++y)
            {
                for (UINT x = 0; x < (MaxDistance+1); ++x)
                {
                    UINT xCoord = (ex * (MaxDistance+1)) + x;
                    UINT yCoord = (ey * (MaxDistance+1)) + y;
                    UINT index = (yCoord * ((MaxDistance+1) * PatternWidth * 2)) + (xCoord * 2); // Texel size = 2 bytes

                    BYTE areaValue    = 0x0;
                    BYTE oppAreaValue = 0x0;
                    if ((ex == 2) || (ey == 2))
                    {
                        // No Mans Land
                        areaValue = 0; //255;
                        oppAreaValue = 0; //255;
                    }
                    else if (((ey == 0) && ((ex == 0) || (ex == 4))) ||
                             ((ey == 4) && ((ex == 0) || (ex == 4))))
                    {
                        // Pattern 0, 5
                        // Pattern 10, 15
                        oppAreaValue = 0;
                    }
                    else
                    {
                        areaValue = 255;
                        oppAreaValue = 0;
                    }


                    pAreaElement[index] = areaValue;
                    pAreaElement[index+1] = oppAreaValue;
                }
            }
        }
    }

    pAreaTex->Unlock(m_pContext);

    // Texture2D <float4> blendTex : register(t3);

    DxTextureCreateInfo blendTexInfo;
    memset(&blendTexInfo, 0, sizeof(DxTextureCreateInfo));

    blendTexInfo.flags.RenderTarget = TRUE;
    blendTexInfo.flags.ShaderInput = TRUE;
    blendTexInfo.format = DXGI_FORMAT_R8G8B8A8_UNORM;
    blendTexInfo.width = colorTexWidth;
    blendTexInfo.height = colorTexHeight;
    
    DxTexture* pBlendTex = DxTexture::Create(m_pDevice, &blendTexInfo);

    //

    DxTextureCreateInfo finalImageInfo;
    memset(&finalImageInfo, 0, sizeof(DxTextureCreateInfo));

    finalImageInfo.flags.RenderTarget = TRUE;
    finalImageInfo.flags.ShaderInput = TRUE;
    finalImageInfo.format = DXGI_FORMAT_R8G8B8A8_UNORM;
    finalImageInfo.width = colorTexWidth;
    finalImageInfo.height = colorTexHeight;
    
    DxTexture* pFinalImage = DxTexture::Create(m_pDevice, &finalImageInfo);


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

    // SamplerState LinearSampler : register(s1);

    D3D11_SAMPLER_DESC linearSamplerDesc;
    memset(&linearSamplerDesc, 0, sizeof(D3D11_SAMPLER_DESC));
    linearSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    linearSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    linearSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    linearSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    linearSamplerDesc.MinLOD = -FLT_MAX;
    linearSamplerDesc.MaxLOD = FLT_MAX;
    linearSamplerDesc.MipLODBias = 0.0f;
    linearSamplerDesc.MaxAnisotropy = 16;

    ID3D11SamplerState* pLinearSampler = NULL;
    m_pDevice->CreateSamplerState(&linearSamplerDesc, &pLinearSampler);

    // Constant Buffers ///////////////////////////////////////////////////////////////////////////

    struct ZoomInfo
    {
        Point4 zoomRect;
    };

    // ZoomInfo Buffer
    D3D11_BUFFER_DESC cbZoomInfoDesc;
    memset(&cbZoomInfoDesc, 0, sizeof(D3D11_BUFFER_DESC));
    cbZoomInfoDesc.Usage = D3D11_USAGE_DYNAMIC;
    cbZoomInfoDesc.ByteWidth = sizeof(ZoomInfo);
    cbZoomInfoDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbZoomInfoDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cbZoomInfoDesc.MiscFlags = 0;

    ZoomInfo zoomInfo;
    memset(&zoomInfo, 0, sizeof(ZoomInfo));

    D3D11_SUBRESOURCE_DATA cbInitData;
    memset(&cbInitData, 0, sizeof(D3D11_SUBRESOURCE_DATA));
    cbInitData.pSysMem = &zoomInfo;

    ID3D11Buffer* pZoomInfoBuffer = NULL;
    m_pDevice->CreateBuffer(&cbZoomInfoDesc, &cbInitData, &pZoomInfoBuffer);

    // Shaders ////////////////////////////////////////////////////////////////////////////////////

    DxShader* pPosTexTriVS = DxShader::CreateFromFile(m_pDevice, "PosTexTri", "MlaaShaders.hlsl", PosTexVertexDesc, PosTexElements);
    DxShader* pGradientPS = DxShader::CreateFromFile(m_pDevice, "Gradient", "MlaaShaders.hlsl");

    DxShader* pApplyTexPS = DxShader::CreateFromFile(m_pDevice, "ApplyTexture", "MlaaShaders.hlsl");

    DxShader* pEdgeDetectPS = DxShader::CreateFromFile(m_pDevice, "ColorEdgeDetectionPS", "MlaaShaders.hlsl");
    DxShader* pBlendWeightsPS = DxShader::CreateFromFile(m_pDevice, "BlendingWeightCalculationPS", "MlaaShaders.hlsl");
    DxShader* pBlendEdgesPS = DxShader::CreateFromFile(m_pDevice, "NeighborhoodBlendingPS", "MlaaShaders.hlsl");

    DxShader* pZoomVS = DxShader::CreateFromFile(m_pDevice, "ZoomVS", "MlaaShaders.hlsl", PosTexVertexDesc, PosTexElements);
    DxShader* pZoomPS = DxShader::CreateFromFile(m_pDevice, "ZoomPS", "MlaaShaders.hlsl");

    ////////////////////////////////////////////////////////////////////////////////////////

    m_pContext->ClearState();

    // SET RENDER STATE

    Point3 rotation = Point3(0.0f, 0.0f, 0.0f);
    FLOAT clearColor[4];
    clearColor[0] = 0.0f;
    clearColor[1] = 0.0f;
    clearColor[2] = 0.0f; 
    clearColor[3] = 1.0f;

 
    D3D11_RASTERIZER_DESC rsDesc;
    rsDesc.FillMode = D3D11_FILL_SOLID;
    rsDesc.CullMode = D3D11_CULL_BACK;
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

    m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_pContext->RSSetState(pRasterizerState);

    m_pWindow->DisplayWindow();

    BOOL quit = false;
    while (!quit)
    {
        BeginFrame();

        m_pWindow->ProcessMsg(&quit);
        
        input();

        // new frame, clear state
        m_pContext->ClearState();

        m_pContext->RSSetViewports(1, &m_viewport);
        m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        m_pContext->RSSetState(pRasterizerState);

        m_pContext->PSSetSamplers(0, 1, &pPointSampler);
        m_pContext->PSSetSamplers(1, 1, &pLinearSampler);

        m_pContext->ClearRenderTargetView(m_pRenderTargetView, clearColor);

        m_pContext->OMSetRenderTargets( 1, &m_pRenderTargetView, NULL);

        pPosTexTriVS->Bind(m_pContext);

        m_pContext->PSSetShaderResources(0, 1, &pColorTexSRV);
        pApplyTexPS->Bind(m_pContext);

        //m_pContext->DrawInstanced(6, 1, 0, 0);
         

        D3D11_VIEWPORT edgeViewport;
        memset(&edgeViewport, 0, sizeof(D3D11_VIEWPORT));
        edgeViewport.Width = static_cast<FLOAT>(colorTexWidth);
        edgeViewport.Height = static_cast<FLOAT>(colorTexHeight);
        edgeViewport.MaxDepth =1.0f;
        m_pContext->RSSetViewports(1, &edgeViewport);

        // Step 1 - Edge Detection
        {
            

            ID3D11RenderTargetView* pEdgesRTV = pEdgesTex->GetRenderTargetView();
            m_pContext->ClearRenderTargetView(pEdgesRTV, clearColor);
            m_pContext->OMSetRenderTargets(1, &pEdgesRTV, NULL);

            pEdgeDetectPS->Bind(m_pContext);

            m_pContext->DrawInstanced(6, 1, 0, 0);
        }

        // Step 2 - Blending Weights
        {
            ID3D11RenderTargetView* pBlendRTV = pBlendTex->GetRenderTargetView();
       
            m_pContext->OMSetRenderTargets(1, &pBlendRTV, NULL);
            m_pContext->ClearRenderTargetView(pBlendRTV, clearColor);

            ID3D11ShaderResourceView* const pEdgesSRV = pEdgesTex->GetShaderResourceView();
            m_pContext->PSSetShaderResources(1, 1, &pEdgesSRV);

            ID3D11ShaderResourceView* const pAreaSRV = pAreaTex->GetShaderResourceView();
            m_pContext->PSSetShaderResources(2, 1, &pAreaSRV);

            pBlendWeightsPS->Bind(m_pContext);

            m_pContext->DrawInstanced(6, 1, 0, 0);
        }

        // Step 3 - Blending Image
        {
            ID3D11RenderTargetView* pFinalImageRTV = pFinalImage->GetRenderTargetView();
            m_pContext->OMSetRenderTargets(1, &pFinalImageRTV, NULL);

            ID3D11ShaderResourceView* const pBlendSRV = pBlendTex->GetShaderResourceView();
            m_pContext->PSSetShaderResources(3, 1, &pBlendSRV);

            pBlendEdgesPS->Bind(m_pContext);
            //pApplyTexPS->SetShader(m_pContext);
            m_pContext->DrawInstanced(6, 1, 0, 0);
        }

        // Display Result
        {
            m_pContext->RSSetViewports(1, &m_viewport);
            m_pContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);

            if (m_passDisplayed == MlaaPassPrePass)
            {
                m_pContext->PSSetShaderResources(0, 1, &pColorTexSRV);
            }
            else if(m_passDisplayed == MlaaPassEdgeDetect)
            {
                ID3D11ShaderResourceView* const pEdgesSRV = pEdgesTex->GetShaderResourceView();
                m_pContext->PSSetShaderResources(0, 1, &pEdgesSRV);
            }
            else if(m_passDisplayed == MlaaPassBlendWeightCalc)
            {
                ID3D11ShaderResourceView* const pBlendSRV = pBlendTex->GetShaderResourceView();
                m_pContext->PSSetShaderResources(0, 1, &pBlendSRV);
            }
            else if(m_passDisplayed == MlaaPassBlendEdge)
            {
                ID3D11ShaderResourceView* const pFinalImageSRV = pFinalImage->GetShaderResourceView();
                m_pContext->PSSetShaderResources(0, 1, &pFinalImageSRV);
            }
            else if(m_passDisplayed == MlaaAreaTex)
            {
                ID3D11ShaderResourceView* const pAreaSRV = pAreaTex->GetShaderResourceView();
                m_pContext->PSSetShaderResources(0, 1, &pAreaSRV);
            }


            pApplyTexPS->Bind(m_pContext);
            m_pContext->DrawInstanced(6, 1, 0, 0);
        }

        // Draw Zoom Region
        D3D11_VIEWPORT zoomViewport;

        UINT zoomRectWidth = 64;
        UINT zoomRectHeight = 64;

        UINT zoomViewportWidth = 256;
        UINT zoomViewportHeight = 256;

        if ((zoomViewportWidth*2 <= m_viewport.Width) && (zoomViewportHeight <= m_viewport.Height))
        {
            memset(&zoomViewport, 0, sizeof(D3D11_VIEWPORT));

            zoomViewport.TopLeftX = m_viewport.Width - zoomViewportWidth;
            zoomViewport.TopLeftY = m_viewport.Height - zoomViewportHeight;
            zoomViewport.Width = static_cast<FLOAT>(zoomViewportHeight);
            zoomViewport.Height = static_cast<FLOAT>(zoomViewportHeight);
            zoomViewport.MaxDepth = 1.0f;

            m_pContext->RSSetViewports(1, &zoomViewport);

            D3D11_MAPPED_SUBRESOURCE mappedCB;

            ZoomInfo* pZoomInfo = NULL;

            m_pContext->Map(pZoomInfoBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedCB);

            pZoomInfo = reinterpret_cast<ZoomInfo*>(mappedCB.pData);

            Point2 zoomCenter = GetMousePos();
            zoomCenter.x *= colorTexWidth / m_viewport.Width;
            zoomCenter.y *= colorTexHeight / m_viewport.Height;

            UINT halfWidth = zoomRectWidth / 2;
            UINT halfHeight = zoomRectHeight / 2;
            if (zoomCenter.x - halfWidth < 0)
            {
                zoomCenter.x = zoomRectWidth / 2.0f;
            }
            else if (zoomCenter.x + halfWidth > colorTexWidth)
            {
                zoomCenter.x = colorTexWidth - (zoomRectWidth / 2.0f);
            }

            if (zoomCenter.y - halfHeight < 0)
            {
                zoomCenter.y = zoomRectHeight / 2.0f;
            }
            else if (zoomCenter.y + halfHeight > colorTexHeight)
            {
                zoomCenter.y = colorTexHeight - (zoomRectHeight / 2.0f);
            }

            pZoomInfo->zoomRect.x = (zoomCenter.x - halfWidth) / (float)colorTexWidth;
            pZoomInfo->zoomRect.y = (zoomCenter.y  - halfHeight) / (float)colorTexHeight;
            pZoomInfo->zoomRect.z = (zoomCenter.x + halfWidth) / (float)colorTexWidth;
            pZoomInfo->zoomRect.w = (zoomCenter.y + halfHeight) / (float)colorTexHeight;

            m_pContext->Unmap(pZoomInfoBuffer, 0);

            m_pContext->VSSetConstantBuffers(0, 1, &pZoomInfoBuffer);
            m_pContext->PSSetConstantBuffers(0, 1, &pZoomInfoBuffer);

            if (m_passDisplayed == MlaaPassPrePass)
            {
                m_pContext->PSSetShaderResources(0, 1, &pColorTexSRV);
            }
            else
            {
                ID3D11ShaderResourceView* const pFinalImageSRV = pFinalImage->GetShaderResourceView();
                m_pContext->PSSetShaderResources(0, 1, &pFinalImageSRV);
            }

            pZoomVS->Bind(m_pContext);
            pZoomPS->Bind(m_pContext);

            if (m_linearZoomFilter)
            {
                m_pContext->PSSetSamplers(0, 1, &pLinearSampler);
            }
            else
            {
                m_pContext->PSSetSamplers(0, 1, &pPointSampler);
            }

            m_pContext->DrawInstanced(6, 1, 0, 0);
        }

        m_pSwapChain->Present(0,0);

        EndFrame();

        Sleep(50);
    }

    // Shader Resource Views
    pColorTexSRV->Release();
    pEdgesTex->Destroy();
    pAreaTex->Destroy();
    pBlendTex->Destroy();

    // Const Buffer
    pZoomInfoBuffer->Release();

    // Shaders
    pApplyTexPS->Destroy();
    pPosTexTriVS->Destroy();
    pGradientPS->Destroy();

    // Samplers
    pPointSampler->Release();
    pLinearSampler->Release();

    // Rasterizer State
    pRasterizerState->Release();

    m_pContext->ClearState();
    m_pContext->Flush(); 
}

void MlaaApp::ReceiveEvent(const Event* pEvent)
{
    if (pEvent->GetType() == EventTypeKeyDown)
    {
        // Toggle linear filter on keydown
        //m_linearZoomFilter = !m_linearZoomFilter;

        HandleKeyboardInput(pEvent);
    }
    else
    {
        DxApp::ReceiveEvent(pEvent);
    }
}

void MlaaApp::HandleKeyboardInput(const Event* pEvent)
{
    if (pEvent->GetType() == EventTypeKeyDown)
    {
        const EventKeyDown* pKeyDownEvent = reinterpret_cast<const EventKeyDown*>(pEvent);

        switch(pKeyDownEvent->GetData().key)
        {
            case EKey1:
                m_passDisplayed = MlaaPassPrePass;
                break;
            case EKey2:
                m_passDisplayed = MlaaPassEdgeDetect;
                break;
            case EKey3:
                m_passDisplayed = MlaaPassBlendWeightCalc;
                break;
            case EKey4:
                m_passDisplayed = MlaaPassBlendEdge;
                break;
            case EKey5:
                m_passDisplayed = MlaaAreaTex;
                break;
            default:
                break;
        }
    }
}

