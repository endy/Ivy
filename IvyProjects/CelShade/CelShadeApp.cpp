///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine - Cel Shading Demo
///
///     Copyright 2010-2012, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "CelShadeApp.h"
#include "IvyDX.h"
#include "IvyWindow.h"
#include "IvyCamera.h"

#include "IvyImporter.h"
#include "DxShader.h"
#include "DxTexture.h"
#include "DxMesh.h"
#include "DxBuffer.h"
#include "IvyUtils.h"

#include "DxUI.h"

#include "IvyGL.h"

///@todo: remove global setting asap
const bool CelShadeAppUseGL = FALSE;

///////////////////////////////////////////////////////////////////////////////////////////////////
/// CelShadeApp::Create
///////////////////////////////////////////////////////////////////////////////////////////////////
CelShadeApp* CelShadeApp::Create()
{
    CelShadeApp* pApp = new CelShadeApp();

    pApp->Init();

    return pApp;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// CelShadeApp::CelShadeApp
///////////////////////////////////////////////////////////////////////////////////////////////////
CelShadeApp::CelShadeApp()
    :
    IvyApp(),
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
    IvyApp::Destroy();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// CelShadeApp::Init
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CelShadeApp::Init()
{
    bool success = IvyApp::Init();

    if (CelShadeAppUseGL)
    {
#if !(IVY_GL_ES)
        IVY_PRINT("GLTestApp OpenGL 2.0 Path");

        static PIXELFORMATDESCRIPTOR pfd =              // pfd Tells Windows How We Want Things To Be
        {
            sizeof(PIXELFORMATDESCRIPTOR),              // Size Of This Pixel Format Descriptor
            1,                                          // Version Number
            PFD_DRAW_TO_WINDOW |                        // Format Must Support Window
            PFD_SUPPORT_OPENGL |                        // Format Must Support OpenGL
            PFD_DOUBLEBUFFER,                           // Must Support Double Buffering
            PFD_TYPE_RGBA,                              // Request An RGBA Format
            32,                                         // Select Our Color Depth
            0, 0, 0, 0, 0, 0,                           // Color Bits Ignored
            0,											// No Alpha Buffer
            0,											// Shift Bit Ignored
            0,											// No Accumulation Buffer
            0, 0, 0, 0,									// Accumulation Bits Ignored
            32,											// 16Bit Z-Buffer (Depth Buffer)  
            8,											// No Stencil Buffer
            0,											// No Auxiliary Buffer
            PFD_MAIN_PLANE,								// Main Drawing Layer
            0,											// Reserved
            0, 0, 0										// Layer Masks Ignored
        };

        m_hDC = GetDC(m_pWindow->GetHwnd());


        int PixelFormat = ChoosePixelFormat(m_hDC,&pfd);
        BOOL setPixFmt = SetPixelFormat(m_hDC, PixelFormat, &pfd);

        m_hGLRC = wglCreateContext(m_hDC);
        wglMakeCurrent(m_hDC, m_hGLRC);

        if( glewInit() != GLEW_OK)
        {
            exit(1);
        }

        //  Error Setup


        //  glDebugMessageCallbackAMD((GLDEBUGPROCAMD)DebugCallbackAMD, NULL);

        //  glDebugMessageEnableAMD(0, 0, 0, NULL, TRUE);
        //glDebugMessageInsertAMDattribute  vec2 in_Position;                    
        //attribute  vec3 in_Color;                       
        ////////////glGetDebugMessageLogAMD

        const GLubyte* pString = glGetString(GL_VERSION);

        GLint majorVersion, minorVersion;
        glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
        glGetIntegerv(GL_MINOR_VERSION, &minorVersion);

        const GLubyte* pExtension = glGetString(GL_EXTENSIONS);
        IvyLog(reinterpret_cast<const char*>(pExtension));
#endif // !(IVY_GL_ES)
    }
    else
    {
        InitDX();

        DxShader* pVertexShader = DxShader::CreateFromSource(m_pDxData->pD3D11Device, "IvyVsPosTex", IvyVsPosTex,PosTexVertexDesc, PosTexElements);

        // Setup Camera
        m_pCamera->Position().x = 0;
        m_pCamera->Position().y = -3;
        m_pCamera->Position().z = 4;
    }

    return success;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// CelShadeApp::Run
///
/// @brief
///     Run the demo
/// @return
///     N/A
///////////////////////////////////////////////////////////////////////////////////////////////////
void CelShadeApp::Run()
{
    if (CelShadeAppUseGL)
    {
        CelShadeGL();
    }
    else
    {
        CelShadeD3D();
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// CelShadeApp::CelShadeGL
///
/// @brief
///     Render a cel-shading demo using OpenGL
/// @return
///     N/A
///////////////////////////////////////////////////////////////////////////////////////////////////
void CelShadeApp::CelShadeGL()
{
    m_pWindow->Show();

    BOOL quit = false;
    while (!quit)
    {
        m_pWindow->ProcessMsg(&quit);

        glClearColor(0.4f, 1.0f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        SwapBuffers(m_hDC);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// CelShadeApp::CelShadeD3D
///
/// @brief
///     Render a cel-shading demo using Direct3D
/// @return
///     N/A
///////////////////////////////////////////////////////////////////////////////////////////////////
void CelShadeApp::CelShadeD3D()
{
    ID3D11DeviceContext* pContext = m_pDxData->pD3D11Context;
    ID3D11Device* pDevice = m_pDxData->pD3D11Device;

    D3DX11_IMAGE_LOAD_INFO imageLoadInfo;
    memset( &imageLoadInfo, 0, sizeof(D3DX11_IMAGE_LOAD_INFO) );
    imageLoadInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    imageLoadInfo.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    DxTextureCreateInfo shadeTexInfo;
    memset(&shadeTexInfo, 0, sizeof(DxTextureCreateInfo));
    shadeTexInfo.flags.RenderTarget = TRUE;
    shadeTexInfo.flags.ShaderInput = TRUE;
    shadeTexInfo.format = DXGI_FORMAT_R8G8B8A8_UNORM;
    shadeTexInfo.width = m_screenWidth;
    shadeTexInfo.height = m_screenHeight;
    DxTexture* pShadeTex = DxTexture::Create(pDevice, &shadeTexInfo);

    DxTextureCreateInfo edgeTexInfo;
    memset(&edgeTexInfo, 0, sizeof(DxTextureCreateInfo));
    edgeTexInfo.flags.RenderTarget = TRUE;
    edgeTexInfo.flags.ShaderInput = TRUE;
    edgeTexInfo.format = DXGI_FORMAT_R8G8B8A8_UNORM;
    edgeTexInfo.width = m_screenWidth;
    edgeTexInfo.height = m_screenHeight;
    DxTexture* pEdgeTex = DxTexture::Create(pDevice, &edgeTexInfo);

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
    //ImportPly("Content/bun_zipper_res4.ply", numVertices, &pVB, numIndices, &pIB);
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

    m_pPosTexTriVS = DxShader::CreateFromFile(pDevice, "PosTexTri", "Content/shaders/CelShade.hlsl", PosTexVertexDesc, PosTexElements);
    m_pPosTexNormVS = DxShader::CreateFromFile(pDevice, "PosTexNorm", "Content/shaders/CelShade.hlsl", PosTexNormVertexDesc, PosTexNormElements);

    m_pCelShadePS = DxShader::CreateFromFile(pDevice, "CelShade", "Content/shaders/CelShade.hlsl");
    DxShader* pDetectEdges = DxShader::CreateFromFile(pDevice, "DetectEdges", "Content/shaders/CelShade.hlsl");
    DxShader* pApplyTexPS = DxShader::CreateFromFile(pDevice, "ApplyTex", "Content/shaders/CelShade.hlsl");

    DxShader* pCubeVS = DxShader::CreateFromFile(pDevice, "PosTex", "Content/shaders/CelShade.hlsl", PosTexVertexDesc, PosTexElements);
    DxShader* pCubePS = DxShader::CreateFromFile(pDevice, "CubePS", "Content/shaders/CelShade.hlsl");


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
        ProcessUpdates();

        BeginFrame();

        CameraBufferData* pCameraData = NULL;

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
        m_pCelShadePS->Bind(pContext);
        pMesh->Draw(pContext);

        ///// Detect Edges ///////////////////////////////////////////////////////////////////////////


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

        pCubeVS->Bind(pContext);

        pCubePS->Bind(pContext);

        pCubeMesh->Bind(pContext);
        pCubeMesh->Draw(pContext);

        ///// Draw UI ////////////////////////////////////////////////////////////////////////////////

        ///@todo Consider moving the following UI drawing to Draw2D()
        m_pUI->Begin();
        // Draw UI stuff
        m_pUI->RenderRect();
        m_pUI->RenderText();
        m_pUI->End();

        /// Blend UI onto final image
        DrawUI();

        m_pDxData->pDXGISwapChain->Present(0,0);

        EndFrame();

        Sleep(50);
        yRotationAngle += 3.14159f / 60.0f;
    }

    // Shader Resource Views

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

    m_pDxData->pD3D11Context->ClearState();
    m_pDxData->pD3D11Context->Flush(); 
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
        IvyApp::ReceiveEvent(pEvent);
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

