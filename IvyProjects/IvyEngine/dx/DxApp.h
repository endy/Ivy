///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2010-2011, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DXAPP_H_
#define _DXAPP_H_

///@todo: Support multiple adapters
///@todo: Support framerate capping

#include "IvyApp.h"
#include "DxTexture.h"

#include <vector>

class DxUI;
class DxShader;
class DxBuffer;
class DxMesh;
struct D3D11BlendState;
struct ID3D11ShaderResourceView;

class DxApp :
    public IvyApp
{
public:
    static DxApp* Create();

    virtual void Destroy();

    virtual void ReceiveEvent(const Event* pEvent);

protected:
    DxApp();
    virtual ~DxApp();

    virtual bool Init();

    void UpdateSwapChain();

    void DrawUI();

    // D3D
    ID3D11Device* m_pDevice;
    ID3D11DeviceContext* m_pContext;
    IDXGISwapChain* m_pSwapChain;

    ///@todo Refactor into a group
    ID3D11RenderTargetView* m_pRenderTargetView;
    DxTexture* m_pDepthStencilBuffer;

    ///@todo Create better mapping between viewport & camera
    D3D11_VIEWPORT m_viewport;

    // UI Objects
    DxUI* m_pUI;    ///< DxUI for the user interface overlay

    DxShader* m_pUIVS;
    DxShader* m_pUIPS;
    DxBuffer* m_pUICameraBuffer;
    DxMesh* m_pUIQuadMesh;
    ID3D11BlendState* m_pUIBlendState;
    ID3D11Texture2D* m_pUIoverlay;
    ID3D11ShaderResourceView* pUI_SRV;
    IDXGIKeyedMutex* m_pUIKeyedMutex_D3D;

private:
    DxApp(const DxApp& copy);               // Disallow copy constructor
    DxApp& operator=(const DxApp& copy);    // Disallow assignment operator

};

#endif // _DXAPP_H_
