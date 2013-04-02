///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2012, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _IVYDX_H_
#define _IVYDX_H_

//#include <D3DX11.h>
#include <d3d11.h>
#include <D2D1.h>
#include <DWrite.h>


class DxMesh;
class DxShader;
class DxBuffer;
class DxTexture;

struct IvyAppDxData
{
    /// Direct3D Member Data
    ID3D11Device* pD3D11Device;
    ID3D11DeviceContext* pD3D11Context;
    IDXGISwapChain* pDXGISwapChain;

    ///@todo Refactor into a group
    ID3D11RenderTargetView* pAppRenderTargetView;
    DxTexture* pAppDepthStencilTex;

    ///@TODO Create better mapping between viewport & camera
    D3D11_VIEWPORT viewport;
};



#endif // _IVYDX_H_
