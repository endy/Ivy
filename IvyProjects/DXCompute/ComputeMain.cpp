
#include <D3DX11.h>


int main()
{

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory( &sd, sizeof( sd ) );
    sd.BufferCount = 1;
    sd.BufferDesc.Width = 256;
    sd.BufferDesc.Height = 256;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = NULL; //m_pWindow->GetHwnd();
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    // CreateComputeDevice( &g_pDevice, &g_pContext, FALSE ) 

    // CreateComputeShader( L"BasicCompute11.hlsl", "CSMain", g_pDevice, &g_pCS );


    // printf( "Creating buffers and filling them with initial data..." );

    // structured buffers vs raw buffers

    // set private data


    // create buffer views

    // set more private data?

    // run compute shader

    // Read back the result from GPU, verify its correctness against result computed by CPU

    // ID3D11Buffer* debugbuf = CreateAndCopyToDebugBuf( g_pDevice, g_pContext, g_pBufResult );
    // D3D11_MAPPED_SUBRESOURCE MappedResource; 
    // BufType *p;
    //g_pContext->Map( debugbuf, 0, D3D11_MAP_READ, 0, &MappedResource );

    // Set a break point here and put down the expression "p, 1024" in your watch window to see what has been written out by our CS
    // This is also a common trick to debug CS programs.
    // p = (BufType*)MappedResource.pData;

    // Verify that if Compute Shader has done right

    // g_pContext->Unmap( debugbuf, 0 );  

    return 0;
}

/*
ID3D11ComputeShader* g_pFinalPassCS = NULL;

pd3dDevice->CreateComputeShader(
pBlobFinalPassCS->GetBufferPointer(), 
pBlobFinalPassCS->GetBufferSize(), 
NULL, &g_pFinalPassCS );

*/