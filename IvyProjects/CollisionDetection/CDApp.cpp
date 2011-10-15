

#include "CDApp.h"
#include "DxShader.h"
#include "DxTexture.h"

CDApp::CDApp(CDAppCreateInfo* pAppInfo)
	:
	DxApp(pAppInfo)
{

}

CDApp::~CDApp()
{

}

CDApp* CDApp::Create(CDAppCreateInfo* pAppInfo)
{
	CDApp* pApp = new CDApp(pAppInfo);

	if (pApp->Init() == false)
	{
		pApp->Destroy();
		pApp = NULL;
	}

	return pApp;
}

void CDApp::Destroy()
{
	DxApp::Destroy();
}

bool CDApp::Init()
{
	return DxApp::Init();
}

void CDApp::Run()
{
	HRESULT hr;
    
    // Resources ////////////////////////////////////////////////////////////////////////////////////   

	ID3D11Texture2D* pBackBuffer = NULL;

	// Get a pointer to the back buffer
	hr = m_pSwapChain->GetBuffer( 0, 
								__uuidof( ID3D11Texture2D ), 
                            	(LPVOID*)&pBackBuffer );

    ID3D11RenderTargetView* pRenderTargetView = NULL;

	// Create a render-target view
	m_pDevice->CreateRenderTargetView( pBackBuffer, 
									 NULL,
                                     &pRenderTargetView );


    

    D3DX11_IMAGE_LOAD_INFO kittenLoadInfo;
	memset( &kittenLoadInfo, 0, sizeof(D3DX11_IMAGE_LOAD_INFO) );
	kittenLoadInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	kittenLoadInfo.Format = DXGI_FORMAT_BC1_UNORM;

	ID3D11ShaderResourceView *pKittenSRView = NULL;
	D3DX11CreateShaderResourceViewFromFile(m_pDevice, "kitten.dds", &kittenLoadInfo, NULL, &pKittenSRView, NULL );

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

    D3D11_SUBRESOURCE_DATA cbInitData;
	memset(&cbInitData, 0, sizeof(D3D11_SUBRESOURCE_DATA));	
	
    // Matrix Buffer
	D3D11_BUFFER_DESC cbMatrixDesc;
	memset(&cbMatrixDesc, 0, sizeof(D3D11_BUFFER_DESC));
	cbMatrixDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbMatrixDesc.ByteWidth = sizeof( MatrixBuffer );
	cbMatrixDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbMatrixDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbMatrixDesc.MiscFlags = 0;

    MatrixBuffer myMatrixBuffer;
	memset(&myMatrixBuffer, 0, sizeof(myMatrixBuffer));
    cbInitData.pSysMem = &myMatrixBuffer;

	ID3D11Buffer* pMatrixBuffer = NULL;
	m_pDevice->CreateBuffer(&cbMatrixDesc, &cbInitData, &pMatrixBuffer);


    // Shaders ////////////////////////////////////////////////////////////////////////////////////

    DxVertexShader* pVSShader = DxVertexShader::Create(m_pDevice, "shaders.txt", "MyVS", PosTexVertexDesc, PosTexElements);
    DxPixelShader* pPSShader = DxPixelShader::Create(m_pDevice, "shaders.txt", "MyPS");      

    // Models /////////////////////////////////////////////////////////////////////////////////////


    ////////////////////////////////////////////////////////////////////////////////////////

    // SET RENDER STATE

    m_pContext->ClearState();

	Point3 rotation = { 0.0f, 0.0f, 0.0f };
    FLOAT clearColor[4];
	clearColor[0] = 0.0f;
	clearColor[1] = 0.0f;
	clearColor[2] = 0.0f; 
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

    // Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = static_cast<FLOAT>(m_screenWidth);
	vp.Height = static_cast<FLOAT>(m_screenHeight);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
             
    m_pContext->RSSetViewports( 1, &vp );
    m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_pContext->VSSetConstantBuffers(0, 1, &pMatrixBuffer);

    D3D11_RASTERIZER_DESC rsDesc;
    rsDesc.FillMode = D3D11_FILL_SOLID;
    rsDesc.CullMode = D3D11_CULL_BACK;
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

    m_pContext->RSSetState(pRasterizerState);    
                              
    // Set Depth Stencil State
    UINT StencilRef = 0x0;
    m_pContext->OMSetDepthStencilState(pDbState, StencilRef);                             

	m_pContext->PSSetSamplers(0, 1, &pSamplerState);

    m_pContext->ClearDepthStencilView(pDepthStencilView, 
                                    D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 
                                    depthClearValue, 
                                    stencilClearValue); 

    D3D11_MAPPED_SUBRESOURCE mappedCB;

	MatrixBuffer* pMatrixCB = NULL;
    Light* pLightCB = NULL;
    Material* pMaterialCB = NULL;   

    m_pWindow->DisplayWindow();

    BOOL quit = false;
	while (!quit)
	{            
        m_pWindow->ProcessMsg(&quit);

                // UPDATE RENDER STATE
        m_pContext->ClearRenderTargetView(pRenderTargetView, clearColor);
        m_pContext->ClearDepthStencilView(pDepthStencilView, 
                                        D3D11_CLEAR_DEPTH, 
                                        depthClearValue, 
                                        stencilClearValue);                 
        
        // UPDATE SCENE


        // DRAW CUBE /////////////////////////////   

        // update cube matrix
		m_pContext->Map(pMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedCB);  
		pMatrixCB = reinterpret_cast<MatrixBuffer*>(mappedCB.pData);
		pMatrixCB->worldMatrix      = XMMatrixTranslation(0, 0, 0);
		pMatrixCB->viewMatrix 		= XMMatrixTranslation(0, 0, 9.0f) * m_pCamera->W2C(); 
        pMatrixCB->projectionMatrix = m_pCamera->C2S(); 
		m_pContext->Unmap(pMatrixBuffer, 0); 
	

        pVSShader->Bind(m_pContext);

	    //m_pContext->PSSetShaderResources(0, 1, &pResourceView);
	    m_pContext->PSSetShaderResources(1, 1, &pKittenSRView);

        // Bind the view -- note: if either views are bound as resources, this generates warnings
        // calling set render target here after the previous set shader res' calls get rid of the warnings
        m_pContext->OMSetRenderTargets( 1, &pRenderTargetView, pDepthStencilView );





		m_pSwapChain->Present(0,0);

        // cannot set clearstate here because we are not setting all correct state in render loop
        //pContext->ClearState();

		rotation.x += 0.2f;
		rotation.y += 0.13f;

		//Sleep(100); 

    }

    if (pMatrixBuffer != NULL)
    {
        // Buffers
        pMatrixBuffer->Release();

        // Texture2D
        pDepthStencilTex->Release();
        //pBackBuffer->Release();
        
        // Shader Resource Views
        //pResourceView->Release();
        pKittenSRView->Release();
        pDepthResView->Release();
        pStencilResView->Release();

        // RenderTargerViews
        pRenderTargetView->Release();

        // DepthStencilViews
        pDepthStencilView->Release();

        // Vertex Shaders
        pVSShader->Destroy();         

        // Pixel Shaders
        pPSShader->Destroy();

            
        // Samplers
        pSamplerState->Release();

        // Depth Stencil State
        pDbState->Release();  

        // Rasterizer State
        pRasterizerState->Release();
    }

    m_pContext->ClearState();
    m_pContext->Flush(); 
}