///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2010-2011, Brandon Light
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
    IvyTextureFlags flags;
    UINT width;
    UINT height;
    DXGI_FORMAT format;
};

class DxTexture :
    public IvyObject
{
public:
    virtual void Destroy();

    VOID* Lock(ID3D11DeviceContext* pContext);
    VOID  Unlock(ID3D11DeviceContext* pContext);

protected:
    DxTexture(DxTextureCreateInfo* pInfo);
    virtual ~DxTexture();

    virtual bool Init(ID3D11Device* pDevice) = 0;

    IvyTextureFlags m_flags;
    UINT m_width;
    UINT m_height;
    DXGI_FORMAT m_format;

    ID3D11Texture2D* m_pTexture2D;
};


class DxColorTexture : public DxTexture
{
public:
    static DxColorTexture* Create(ID3D11Device* pDevice, DxTextureCreateInfo* pInfo);
    virtual void Destroy();

    ID3D11ShaderResourceView* GetShaderResourceView() { return m_pResourceView; }
    ID3D11RenderTargetView* GetRenderTargetView() { return m_pRenderTargetView; }

protected:
    DxColorTexture(DxTextureCreateInfo* pInfo);
    virtual ~DxColorTexture();

    virtual bool Init(ID3D11Device* pDevice);
    
    ID3D11ShaderResourceView* m_pResourceView;
    ID3D11RenderTargetView* m_pRenderTargetView;

private:
    DxColorTexture(const DxColorTexture& copy);     // Disallow copy constructor

};

class DxDepthTexture : public DxTexture
{
public:
    static DxDepthTexture* Create(ID3D11Device* pDevice, DxTextureCreateInfo* pInfo);
    virtual void Destroy();

    ID3D11ShaderResourceView* GetDepthResourceView() { return m_pDepthResourceView; }
    ID3D11ShaderResourceView* GetStencilResourceView() { return m_pStencilResourceView; }
    ID3D11DepthStencilView* GetDepthStencilView() { return m_pDepthStencilView; }

protected:
    DxDepthTexture(DxTextureCreateInfo* pInfo);
    virtual ~DxDepthTexture();

    virtual bool Init(ID3D11Device* pDevice);
    
    ID3D11ShaderResourceView* m_pDepthResourceView;
    ID3D11ShaderResourceView* m_pStencilResourceView;
    ID3D11DepthStencilView* m_pDepthStencilView;

private:
    DxDepthTexture(const DxDepthTexture& copy);     // Disallow copy constructor

};


#endif // _DXTEXTURE_H_
