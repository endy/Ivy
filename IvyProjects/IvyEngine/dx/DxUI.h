///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2012, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DXUI_H_
#define _DXUI_H_

#include <D3DX11.h>
#include <D2D1.h>
#include <DWrite.h>

struct ID3D11Device;

class DxUI
{
public:
    static DxUI* Create();
    void Destroy();

    void Begin();
    void End();

    void RenderRect();
    void RenderText();

    ID3D11Texture2D* CreateSharedTextureOverlay(ID3D11Device* pDevice);

protected:
    DxUI();
    virtual ~DxUI();

    bool Init();

    // D2D
    ID2D1Factory* m_pDirect2dFactory;
    ID2D1RenderTarget* m_pRenderTarget;

    ID2D1SolidColorBrush* m_pITextBrush;

    // DWrite
    IDWriteTextFormat* m_pITextFormat;
    IDWriteFactory* m_pIDWriteFactory;

    // D2D Shared Surface
    ID3D10Device1* m_pDevice10_1;
    ID3D10Texture2D* m_pD2DOverlay;
    HANDLE m_hSharedD2DOverlay;
    IDXGIKeyedMutex* m_pUIKeyedMutex_D2D;

private:
    DxUI(const DxUI& copy);               // Disallow copy constructor
    DxUI& operator=(const DxUI& copy);    // Disallow assignment operator

    UINT m_AcquireKey;      ///< Mutex acquire key
    UINT m_ReleaseKey;      ///< Mutex release key
    UINT m_Timeout;         ///< Mutex acquire timeout
};

#endif // _DXUI_H_

