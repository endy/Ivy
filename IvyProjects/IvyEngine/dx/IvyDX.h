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

#include <D3DX11.h>

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
    ID3D11RenderTargetView* m_pRenderTargetView;
    DxTexture* m_pDepthStencilBuffer;

    ///@TODO Create better mapping between viewport & camera
    D3D11_VIEWPORT viewport;
};



#endif // _IVYDX_H_
