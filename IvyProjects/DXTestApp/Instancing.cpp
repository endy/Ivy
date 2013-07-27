
#include "DxTestApp.h"
#include "IvyCamera.h"
#include "IvyWindow.h" ///@todo refactor this header out 
#include "IvyUtils.h"
#include "IvyImporter.h"

#include "IvyDX.h"
#include "DxUI.h"
#include "DxShader.h"
#include "DxTexture.h"
#include "DxBuffer.h"
#include "DxMesh.h"


#include "stb_image.h"

void DxTestApp::RunInstancing()
{
    IVY_PRINT("DxTestApp D3D11 Path");

    struct Projectile
    {
        Point3 position;
        Point3 direction;
        float speed;
        unsigned int age;
    };

    const unsigned int MaxBullets = 10;
    Projectile g_bullets[MaxBullets];
    unsigned int g_nextFreeBullet = 0;

    memset(g_bullets, 0, sizeof(g_bullets));

    ID3D11Device* pDevice = m_pDxData->pD3D11Device;
    ID3D11DeviceContext* pContext = m_pDxData->pD3D11Context;

    HRESULT hr;

    // Resources ////////////////////////////////////////////////////////////////////////////////////   

    int width;
    int height;
    int comp;
    unsigned char* pPixels = stbi_load("Content/kitten_rgb.png", &width, &height, &comp, STBI_rgb_alpha);

    D3D11_TEXTURE2D_DESC texDesc;
    memset(&texDesc, 0, sizeof(D3D11_TEXTURE2D_DESC));
    texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texDesc.Width = width;
    texDesc.Height = height;
    texDesc.ArraySize = 1;
    texDesc.MipLevels = 1;
    texDesc.CPUAccessFlags = 0;
    texDesc.SampleDesc.Count = 1;
    texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    D3D11_SUBRESOURCE_DATA initialData;
    memset(&initialData, 0, sizeof(D3D11_SUBRESOURCE_DATA));
    initialData.pSysMem = pPixels;
    initialData.SysMemPitch = width * 4;

    ID3D11Texture2D* pTex2D = NULL;
    pDevice->CreateTexture2D(&texDesc, &initialData, &pTex2D);


    ID3D11ShaderResourceView *pKittenSRView = NULL;
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

    memset(&srvDesc, 0, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
    srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    srvDesc.Texture2D.MostDetailedMip = 0;

    pDevice->CreateShaderResourceView(pTex2D, &srvDesc, &pKittenSRView);

    stbi_image_free(pPixels);


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
    hr = pDevice->CreateSamplerState(&samplerDesc, &pSamplerState);

    // Buffers   ///////////////////////////////////////////////////////////////////////////

    // Camera Buffer
    CameraBufferData cameraData;
    memset(&cameraData, 0, sizeof(CameraBufferData));

    DxBufferCreateInfo cameraBufferCreateInfo = {0};
    cameraBufferCreateInfo.flags.cpuWriteable = TRUE;
    cameraBufferCreateInfo.elemSizeBytes = sizeof(CameraBufferData);
    cameraBufferCreateInfo.pInitialData = &cameraData;
    DxBuffer* pCameraBuffer = DxBuffer::Create(pDevice, &cameraBufferCreateInfo);

    DxTestAppShaderConsts appConstants;
    appConstants.screenWidth = m_screenWidth;
    appConstants.screenHeight = m_screenHeight;

    DxBufferCreateInfo globalAppConstants = {0};
    globalAppConstants.flags.cpuWriteable = TRUE;
    globalAppConstants.elemSizeBytes = sizeof(DxTestAppShaderConsts);
    globalAppConstants.pInitialData = &appConstants;
    m_pAppConstBuffer = DxBuffer::Create(pDevice, &globalAppConstants);

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
    pDevice->CreateBuffer(&cbMaterialDesc, &cbInitData, &pMaterialBuffer);

    // Shaders ////////////////////////////////////////////////////////////////////////////////////

    DxShader* pPosTexVS = DxShader::CreateFromFile(pDevice, "PosTex", L"DXTestApp/dxtestapp.hlsl", PosTexVertexDesc, PosTexElements);
    DxShader* pApplyTexPS = DxShader::CreateFromFile(pDevice, "ApplyTexture", L"DXTestApp/dxtestapp.hlsl");
    DxShader* pVisDepthPS = DxShader::CreateFromFile(pDevice, "VisDepth", L"DXTestApp/dxtestapp.hlsl");

    DxShader* pPosTexNormVS = DxShader::CreateFromFile(pDevice, "PosTexNorm", L"DXTestApp/dxtestapp.hlsl", PosTexNormVertexDesc, PosTexNormElements);
    DxShader* pVisNormalPS = DxShader::CreateFromFile(pDevice, "VisNormal", L"DXTestApp/dxtestapp.hlsl"); 

    DxShader* pInstanceCubeVS = DxShader::CreateFromFile(pDevice, "InstanceCube", L"DXTestApp/dxtestapp.hlsl", PosVertexDesc, PosElements);
    DxShader* pVisTexCoordPS = DxShader::CreateFromFile(pDevice, "VisTexCoord", L"DXTestApp/dxtestapp.hlsl");

    // Models /////////////////////////////////////////////////////////////////////////////////////

    // Cube
    Cube c;
    DxMeshCreateInfo cubeMeshInfo;
    memset(&cubeMeshInfo, 0, sizeof(cubeMeshInfo));
    cubeMeshInfo.pVertexArray = c.GetVB();
    cubeMeshInfo.vertexCount = c.NumVertices();
    cubeMeshInfo.vertexElementSize = sizeof(VertexPT);

    DxMesh* pCubeMesh = DxMesh::Create(pDevice, &cubeMeshInfo);   

    // Plane
    Plane p;
    DxMeshCreateInfo planeMeshInfo;
    memset(&planeMeshInfo, 0, sizeof(planeMeshInfo));
    planeMeshInfo.pVertexArray = p.GetVB();
    planeMeshInfo.vertexCount = p.NumVertices();
    planeMeshInfo.vertexElementSize = sizeof(VertexPTN);

    DxMesh* pPlaneMesh = DxMesh::Create(pDevice, &planeMeshInfo);

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

    DxMesh* pBunnyMesh = DxMesh::Create(pDevice, &meshCreateInfo);

    ////////////////////////////////////////////////////////////////////////////////////////

    pContext->ClearState();

    // SET RENDER STATE

    Point3 rotation = Point3(0.0f, 0.0f, 0.0f);
    FLOAT clearColor[4];
    clearColor[0] = 0.0f; //0.4f;
    clearColor[1] = 0.0f; //1.0f;
    clearColor[2] = 0.0f; //0.4f; 
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
    dbDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dbDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    dbDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
    dbDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    dbDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;

    ID3D11DepthStencilState* pDbState = NULL;
    hr = pDevice->CreateDepthStencilState(&dbDesc, &pDbState);

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
    pDevice->CreateRasterizerState(&rsDesc, &pRasterizerState);

    // Set Depth Stencil State
    UINT StencilRef = 0x0;
    pContext->OMSetDepthStencilState(pDbState, StencilRef);

    pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    pContext->RSSetState(pRasterizerState);
    pContext->PSSetSamplers(0, 1, &pSamplerState);


    pContext->ClearDepthStencilView(m_pDxData->pAppDepthStencilTex->GetDepthStencilView(), 
        D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 
        depthClearValue, 
        stencilClearValue); 

    CameraBufferData* pCameraBufferData = NULL;
    Material* pMaterialCB = NULL;   

    m_pWindow->Show();

    IvyGamepadState prevGamepadState;
    memset(&prevGamepadState, 0, sizeof(IvyGamepadState));

    MouseState prevMouseState;
    memcpy(&prevMouseState, GetMouseState(), sizeof(MouseState));

    while (ExitApp() == FALSE)
    {            
        BeginFrame();

        /////// UPDATE WORLD MODEL
        const IvyGamepadState* pGamepad = GetGamepadState();
        const KeyboardState* pKeys = GetKeyboardState();
        const MouseState* pMouse = GetMouseState();

        if ((pGamepad->ButtonPressed[IvyGamepadButtons::ButtonY]) ||
            (pKeys->Pressed[Key_R]))
        {
            m_pCamera->Reset();
        }
        else if(pGamepad->ButtonPressed[IvyGamepadButtons::ButtonB])
        {
            // Reset orientation to (0,0,+z) by doing (-phi, -theta)+(IvyPi/2,IvyPi/2)
            FLOAT phi;
            FLOAT theta;
            m_pCamera->Orientation(phi, theta);
            m_pCamera->Move(Point3(), -phi+(IvyPi/2.0f), -theta+(IvyPi/2.0f));
        }
        else if ((pGamepad->ButtonPressed[IvyGamepadButtons::ButtonX]) &&
                 (prevGamepadState.ButtonPressed[IvyGamepadButtons::ButtonX] != pGamepad->ButtonPressed[IvyGamepadButtons::ButtonX]))
        {
            m_pCamera->Print();
        }
        else
        {
            m_pCamera->Move(Point3(pGamepad->ThumbLX * 0.05,
                                   0.0,
                                   pGamepad->ThumbLY * 0.05),
                            -(pGamepad->ThumbRX * 0.05),    // phi = clockwise rotation about y-axis, invert input
                            pGamepad->ThumbRY * 0.05);
        }

        if (pKeys->Pressed[Key_W])
        {
            m_pCamera->Move(Point3(0, 0, 0.05), 0, 0);
        }
        else if (pKeys->Pressed[Key_S])
        {
            m_pCamera->Move(Point3(0, 0, -0.05), 0, 0);
        }

        if (pKeys->Pressed[Key_A])
        {
            m_pCamera->Move(Point3(-0.05, 0, 0), 0, 0);
        }
        if (pKeys->Pressed[Key_D])
        {
            m_pCamera->Move(Point3(0.05, 0, 0), 0, 0);
        }

        if (pMouse->Pressed[MouseLeft])
        {
            INT dx = pMouse->x - prevMouseState.x;
            INT dy = pMouse->y - prevMouseState.y;
            m_pCamera->Move(Point3(), 2.0f * dx / (FLOAT)m_screenWidth, -2.0f * dy / (FLOAT)m_screenHeight);
        }

        XMMATRIX cameraView = m_pCamera->W2C();

        if ((pGamepad->ButtonPressed[IvyGamepadButtons::ButtonA]) &&
            (pGamepad->ButtonPressed[IvyGamepadButtons::ButtonA] != prevGamepadState.ButtonPressed[IvyGamepadButtons::ButtonA]))
        {
            g_bullets[g_nextFreeBullet].age = 500;
            m_pCamera->Position(g_bullets[g_nextFreeBullet].position);
            g_bullets[g_nextFreeBullet].direction = m_pCamera->LookAt();
            g_bullets[g_nextFreeBullet].speed = 0.1f;

            g_nextFreeBullet = (g_nextFreeBullet+1 < MaxBullets) ? g_nextFreeBullet+1 : 0;
        }


        /////// DRAW GRAPHICS

        // new frame, clear state
        pContext->ClearState();

        pContext->RSSetViewports(1, &m_pDxData->viewport);
        pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        m_pAppConstBuffer->BindPS(pContext, 0);

        pCameraBuffer->BindVS(pContext, 0);

        pContext->PSSetConstantBuffers(1, 1, &pMaterialBuffer);
        pContext->RSSetState(pRasterizerState);

        pContext->PSSetSamplers(0, 1, &pSamplerState);

        pContext->OMSetDepthStencilState(pDbState, StencilRef);

        // UPDATE RENDER STATE
        pContext->ClearRenderTargetView(m_pDxData->pAppRenderTargetView, clearColor);
        pContext->ClearDepthStencilView(m_pDxData->pAppDepthStencilTex->GetDepthStencilView(), 
            D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 
            depthClearValue, 
            stencilClearValue);

        // UPDATE SCENE

        // DRAW CUBE /////////////////////////////

        // update cube matrix
        pCameraBufferData = reinterpret_cast<CameraBufferData*>(pCameraBuffer->Map(pContext));
        pCameraBufferData->worldMatrix      = XMMatrixScaling(0.2, 0.2, 0.2); // * XMMatrixRotationX(rotation.x*10 + -3.14f/2.0f); //XMMatrixIdentity();
        pCameraBufferData->viewMatrix       = cameraView;
        pCameraBufferData->projectionMatrix = m_pCamera->C2S(); 
        pCameraBuffer->Unmap(pContext);

        // Bind the view -- note: if either views are bound as resources, this generates warnings
        // calling set render target here after the previous set shader res' calls get rid of the warnings

        pContext->OMSetRenderTargets( 1, &m_pDxData->pAppRenderTargetView, m_pDxData->pAppDepthStencilTex->GetDepthStencilView());

        pInstanceCubeVS->Bind(pContext);
        pVisTexCoordPS->Bind(pContext);
        pCubeMesh->Bind(pContext);
        pCubeMesh->DrawInstanced(pContext, 1000);

        // DRAW BUNNY WITH CAMERA /////////////////////////////

        pCameraBufferData = reinterpret_cast<CameraBufferData*>(pCameraBuffer->Map(pContext));
        pCameraBufferData->worldMatrix = XMMatrixScaling(5, 5, 5);
        pCameraBufferData->viewMatrix = cameraView; 
        pCameraBufferData->projectionMatrix = m_pCamera->C2S();
        pCameraBuffer->Unmap(pContext);

        pPosTexNormVS->Bind(pContext);
        pVisNormalPS->Bind(pContext);

        pBunnyMesh->Bind(pContext);
        pBunnyMesh->DrawInstanced(pContext, 100);


        for (unsigned int idx = 0; idx < MaxBullets; ++idx)
        {
            pPosTexVS->Bind(pContext);

            if (g_bullets[idx].age > 0)
            {
                g_bullets[idx].position.x += g_bullets[idx].direction.x * g_bullets[idx].speed;
                g_bullets[idx].position.y += g_bullets[idx].direction.y * g_bullets[idx].speed;
                g_bullets[idx].position.z += g_bullets[idx].direction.z * g_bullets[idx].speed;

                pCameraBufferData = reinterpret_cast<CameraBufferData*>(pCameraBuffer->Map(pContext));
                pCameraBufferData->worldMatrix      = XMMatrixScaling(0.2, 0.2, 0.2) *
                                                      XMMatrixTranslation(g_bullets[idx].position.x, g_bullets[idx].position.y, g_bullets[idx].position.z);
                pCameraBufferData->viewMatrix       = cameraView;
                pCameraBufferData->projectionMatrix = m_pCamera->C2S(); 
                pCameraBuffer->Unmap(pContext);

                pCubeMesh->Draw(pContext);

                g_bullets[idx].age--;
            }
        }

        pPosTexVS->Bind(pContext);

        pContext->PSSetShaderResources(0, 1, &pKittenSRView);
        pApplyTexPS->Bind(pContext);


        // VISUALIZE DEPTH/STENCIL /////////////////////////////  
        pContext->OMSetRenderTargets( 1, &m_pDxData->pAppRenderTargetView, NULL );

        D3D11_VIEWPORT vizDepth;
        vizDepth.MinDepth = 0;
        vizDepth.MaxDepth = 1.0;
        vizDepth.TopLeftX = m_screenWidth - (m_screenWidth / 4);
        vizDepth.TopLeftY = m_screenHeight - (m_screenHeight / 4);
        vizDepth.Width = m_screenWidth / 4;
        vizDepth.Height = m_screenHeight / 4;
        pContext->RSSetViewports(1, &vizDepth);

        pCameraBufferData = reinterpret_cast<CameraBufferData*>(pCameraBuffer->Map(pContext));
        pCameraBufferData->worldMatrix      = XMMatrixRotationX(-3.14f/2.0f);
        pCameraBufferData->viewMatrix       = XMMatrixIdentity();  
        pCameraBufferData->projectionMatrix = XMMatrixIdentity(); 
        pCameraBuffer->Unmap(pContext);

        pPosTexVS->Bind(pContext);

        ID3D11ShaderResourceView* pDepthView = m_pDxData->pAppDepthStencilTex->GetDepthResourceView();
        pContext->PSSetShaderResources(0, 1, &pDepthView);
        ID3D11ShaderResourceView* pStencilView = m_pDxData->pAppDepthStencilTex->GetStencilResourceView();
        pContext->PSSetShaderResources(1, 1, &pStencilView);

        pVisDepthPS->Bind(pContext);

        pPlaneMesh->Bind(pContext);
        pPlaneMesh->Draw(pContext);

        pContext->RSSetViewports(1, &m_pDxData->viewport);

        wchar_t stringBuffer[1024];
        memset(stringBuffer, 0, 1024*sizeof(wchar_t));

        Point3 position;
        m_pCamera->Position(position);
        FLOAT phi;
        FLOAT theta;
        m_pCamera->Orientation(phi, theta);
        Point3 direction = m_pCamera->LookAt();

        swprintf(stringBuffer,
                 1024,
                 L"Viewport: (%i, %i, %i, %i) FOVY: (%f)\nCamera Pos (%f, %f, %f)\nOrientation [Phi: %f, Theta: %f]\nDirection Vector: (%f, %f, %f)",
                 0, 0, m_screenWidth, m_screenHeight,
                 m_fovY*180/IvyPi,
                 position.x,
                 position.y,
                 position.z,
                 phi*180/IvyPi,
                 theta*180/IvyPi,
                 direction.x,
                 direction.y,
                 direction.z);

        // Draw UI
        m_pUI->Begin();
        m_pUI->RenderText(stringBuffer);
        m_pUI->End();
        DrawUI();

        m_pDxData->pDXGISwapChain->Present(0,0);

        // cannot set clearstate here because we are not setting all correct state in render loop
        //pContext->ClearState();

        rotation.x += 0.002f;
        rotation.y += 0.13f;

        memcpy(&prevGamepadState, pGamepad, sizeof(IvyGamepadState));
        memcpy(&prevMouseState, GetMouseState(), sizeof(MouseState));

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

    if (pCameraBuffer != NULL)
    {
        // Buffers
        pCameraBuffer->Destroy();
    }

    pMaterialBuffer->Release(); 

    if (pTex2D)
    {
        pTex2D->Release();
        pTex2D = NULL;
    }

    // Shader Resource Views
    pKittenSRView->Release();

    // DepthStencilViews

    // Vertex Shaders
    pPosTexVS->Destroy();
    pPosTexNormVS->Destroy();
    pInstanceCubeVS->Destroy();

    // Pixel Shaders
    pApplyTexPS->Destroy();
    pVisNormalPS->Destroy();
    pVisDepthPS->Destroy();
    pVisTexCoordPS->Destroy();

    // Samplers
    pSamplerState->Release();

    // Depth Stencil State
    pDbState->Release();  

    // Rasterizer State
    pRasterizerState->Release();

    pContext->ClearState();

}