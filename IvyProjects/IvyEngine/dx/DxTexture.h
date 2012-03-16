///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2010-2012, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef _DXTEXTURE_H_
#define _DXTEXTURE_H_

#include "IvyTexture.h"
#include "DxTypes.h"

#include <D3DX11.h>


struct DxTextureCreateInfo
{
    IvyTextureType type;
    IvyTextureFlags flags;
    UINT width;
    UINT height;
    DXGI_FORMAT format;
};

class DxTexture :
    public IvyTexture
{
public:
    static DxTexture* Create(ID3D11Device* pDevice, DxTextureCreateInfo* pInfo);
    virtual void Destroy();

    VOID* Lock(ID3D11DeviceContext* pContext);
    VOID  Unlock(ID3D11DeviceContext* pContext);

    // Color
    ID3D11ShaderResourceView* GetShaderResourceView() const { return m_pResourceView; }
    ID3D11RenderTargetView* GetRenderTargetView() { return m_pRenderTargetView; }

    // Depth/Stencil Views
    ID3D11ShaderResourceView* GetDepthResourceView() { return m_pDepthResourceView; }
    ID3D11ShaderResourceView* GetStencilResourceView() { return m_pStencilResourceView; }
    ID3D11DepthStencilView* GetDepthStencilView() { return m_pDepthStencilView; }

protected:
    DxTexture(DxTextureCreateInfo* pInfo);
    virtual ~DxTexture();

    virtual bool Init(ID3D11Device* pDevice);

    DXGI_FORMAT m_dxFormat;

    // Texture 
    ID3D11Texture2D* m_pTexture2D;

    // Color views
    ID3D11ShaderResourceView* m_pResourceView;
    ID3D11RenderTargetView* m_pRenderTargetView;

    // Depth/Stencil views
    ID3D11ShaderResourceView* m_pDepthResourceView;
    ID3D11ShaderResourceView* m_pStencilResourceView;
    ID3D11DepthStencilView* m_pDepthStencilView;
};



#endif // _DXTEXTURE_H_
