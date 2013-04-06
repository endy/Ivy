///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2010-2012, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "DxTexture.h"
#include "DxUtils.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxTexture::DxTexture
///////////////////////////////////////////////////////////////////////////////////////////////////
DxTexture::DxTexture(
    DxTextureCreateInfo* pInfo)
    :
    IvyTexture(pInfo->type, pInfo->width, pInfo->height),
    m_dxFormat(pInfo->format),
    m_pTexture2D(NULL),
    m_pResourceView(NULL),
    m_pRenderTargetView(NULL),
    m_pDepthResourceView(NULL),
    m_pStencilResourceView(NULL),
    m_pDepthStencilView(NULL)
{
    m_flags = pInfo->flags;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxTexture::Destroy
///////////////////////////////////////////////////////////////////////////////////////////////////
DxTexture::~DxTexture()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxTexture::Create
///////////////////////////////////////////////////////////////////////////////////////////////////
DxTexture* DxTexture::Create(
    ID3D11Device* pDevice,          ///< D3D11 Device
    DxTextureCreateInfo* pInfo)     ///< DxTexture Create Info
{
    DxTexture* pTexture = new DxTexture(pInfo);

    if (pTexture->Init(pDevice) == false)
    {
        IvyAssertAlways();
        pTexture->Destroy();
        pTexture = NULL;
    }

    return pTexture;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxTexture::Destroy
///////////////////////////////////////////////////////////////////////////////////////////////////
void DxTexture::Destroy()
{
    if (m_pResourceView != NULL)
    {
        m_pResourceView->Release();
        m_pResourceView = NULL;
    }

    if (m_pRenderTargetView != NULL)
    {
        m_pRenderTargetView->Release();
        m_pRenderTargetView = NULL;
    }

    if (m_pDepthStencilView != NULL)
    {
        m_pDepthStencilView->Release();
        m_pDepthStencilView = NULL;
    }

    if (m_pDepthResourceView != NULL)
    {
        m_pDepthResourceView->Release();
        m_pDepthResourceView = NULL;
    }

    if (m_pStencilResourceView != NULL)
    {
        m_pStencilResourceView->Release();
        m_pStencilResourceView = NULL;
    }

    if (m_pTexture2D != NULL)
    {
        m_pTexture2D->Release();
        m_pTexture2D = NULL;
    }

    delete this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxTexture::Lock
///////////////////////////////////////////////////////////////////////////////////////////////////
VOID* DxTexture::Lock(
    ID3D11DeviceContext* pContext)  ///< D3D11 Device Context
{
    VOID* pTextureData = NULL; 

    // Only support dynamic, write discard
    if (m_flags.CpuWrite)
    {
        D3D11_MAPPED_SUBRESOURCE mappedTexture;
        pContext->Map(m_pTexture2D, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedTexture); 
        pTextureData = mappedTexture.pData;
    }

    return pTextureData;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxTexture::Unlock
///////////////////////////////////////////////////////////////////////////////////////////////////
void DxTexture::Unlock(
    ID3D11DeviceContext* pContext)  ///< D3D11 Device Context
{
    pContext->Unmap(m_pTexture2D, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxTexture::Init
///
/// @todo Refactor this method
///////////////////////////////////////////////////////////////////////////////////////////////////
bool DxTexture::Init(
    ID3D11Device* pDevice)  ///< D3D11 Device
{
    bool statusOK = true;

    if (m_flags.DepthStencil != TRUE)
    {
        UINT bindFlags = 0;
        bindFlags |= (m_flags.RenderTarget) ? D3D11_BIND_RENDER_TARGET : 0;
        bindFlags |= (m_flags.ShaderInput) ? D3D11_BIND_SHADER_RESOURCE : 0;

        UINT cpuAccessFlags = 0;
        cpuAccessFlags |= (m_flags.CpuRead) ? D3D11_CPU_ACCESS_READ : 0;
        cpuAccessFlags |= (m_flags.CpuWrite) ? D3D11_CPU_ACCESS_WRITE : 0;

        D3D11_TEXTURE2D_DESC colorTexDesc;
        memset(&colorTexDesc, 0, sizeof(D3D11_TEXTURE2D_DESC));
        colorTexDesc.Format = m_dxFormat;
        colorTexDesc.Width = m_width;
        colorTexDesc.Height = m_height;
        colorTexDesc.CPUAccessFlags = cpuAccessFlags;
        colorTexDesc.BindFlags = bindFlags;

        if (m_flags.CpuRead == TRUE)
        {
            colorTexDesc.Usage = D3D11_USAGE_STAGING;
        }
        else if (m_flags.CpuWrite == TRUE)
        {
            colorTexDesc.Usage = D3D11_USAGE_DYNAMIC;
        }
        else
        {
            colorTexDesc.Usage = D3D11_USAGE_DEFAULT;
        }

        colorTexDesc.SampleDesc.Count = 1;
        colorTexDesc.SampleDesc.Quality = 0;
        colorTexDesc.ArraySize = 1;  
        colorTexDesc.MipLevels = 1;
        colorTexDesc.MiscFlags = 0;

        statusOK = DxOK(pDevice->CreateTexture2D(&colorTexDesc, NULL, &m_pTexture2D));
    }
    else
    {
        if (DxgiHasDepth(m_dxFormat) == false)
        {
            statusOK = false;
        }

        if (statusOK == true)
        {
            // DXGI_FORMAT_D24_UNORM_S8_UINT

            /// @todo pipe in format correctly

            UINT bindFlags = D3D11_BIND_DEPTH_STENCIL;
            bindFlags |= (m_flags.ShaderInput) ? D3D11_BIND_SHADER_RESOURCE : 0;

            UINT cpuAccessFlags = 0;
            cpuAccessFlags |= (m_flags.CpuRead) ? D3D11_CPU_ACCESS_READ : 0;
            cpuAccessFlags |= (m_flags.CpuWrite) ? D3D11_CPU_ACCESS_WRITE : 0;

            D3D11_TEXTURE2D_DESC depthStencilDesc;
            memset(&depthStencilDesc, 0, sizeof(D3D11_TEXTURE2D_DESC));
            depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
            depthStencilDesc.Width = m_width;
            depthStencilDesc.Height = m_height;
            depthStencilDesc.CPUAccessFlags = cpuAccessFlags;
            depthStencilDesc.BindFlags = bindFlags;
            depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
            depthStencilDesc.SampleDesc.Count = 1;
            depthStencilDesc.SampleDesc.Quality = 0;  
            depthStencilDesc.ArraySize = 1;  
            depthStencilDesc.MipLevels = 1;
            depthStencilDesc.MiscFlags = 0;

            statusOK = DxOK(pDevice->CreateTexture2D(&depthStencilDesc, NULL, &m_pTexture2D));
        }
    }

    if (statusOK == true)
    {


        // SHADER INPUT//////

        if (m_flags.DepthStencil != TRUE)
        {
            if (statusOK == true && m_flags.ShaderInput)
            {
                D3D11_TEXTURE2D_DESC texDesc;
                m_pTexture2D->GetDesc(&texDesc);

                D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
                memset(&viewDesc, 0, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

                viewDesc.Format = m_dxFormat;
                viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
                viewDesc.Texture2D.MipLevels = texDesc.MipLevels;
                viewDesc.Texture2D.MostDetailedMip = 0;

                statusOK = DxOK(pDevice->CreateShaderResourceView(m_pTexture2D, &viewDesc, &m_pResourceView));
            }

            if (statusOK == true && m_flags.RenderTarget)
            {
                D3D11_RENDER_TARGET_VIEW_DESC rtDesc;
                memset(&rtDesc, 0, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));

                rtDesc.Format = m_dxFormat;
                rtDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
                rtDesc.Texture2D.MipSlice = 0;
                statusOK = DxOK(pDevice->CreateRenderTargetView(m_pTexture2D, &rtDesc, &m_pRenderTargetView));
            }
        }

        if (m_flags.DepthStencil == TRUE)
        {
            if (statusOK == true)
            {
                D3D11_DEPTH_STENCIL_VIEW_DESC dsViewDesc;
                memset(&dsViewDesc, 0, sizeof(dsViewDesc));

                dsViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
                dsViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

                statusOK = DxOK(pDevice->CreateDepthStencilView(m_pTexture2D, &dsViewDesc, &m_pDepthStencilView));
            }

            if (statusOK == true && m_flags.ShaderInput)
            {
                D3D11_SHADER_RESOURCE_VIEW_DESC depthResView;
                memset(&depthResView, 0, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

                depthResView.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
                depthResView.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
                depthResView.Texture2D.MipLevels = 1;
                depthResView.Texture2D.MostDetailedMip = 0;    

                statusOK = DxOK(pDevice->CreateShaderResourceView(m_pTexture2D, &depthResView, &m_pDepthResourceView));

                if ((statusOK == true))//  && DxgiHasStencil(m_format))
                {
                    D3D11_SHADER_RESOURCE_VIEW_DESC stencilResView;
                    memset(&stencilResView, 0, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

                    stencilResView.Format = DXGI_FORMAT_X24_TYPELESS_G8_UINT;
                    stencilResView.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
                    stencilResView.Texture2D.MipLevels = 1;
                    stencilResView.Texture2D.MostDetailedMip = 0;

                    statusOK = DxOK(pDevice->CreateShaderResourceView(m_pTexture2D, &stencilResView, &m_pStencilResourceView));
                }
            }
        }
    }

    return statusOK;
}

