///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine - Cel Shading Demo
///
///     Copyright 2010-2011, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CELSHADEAPP_H_
#define _CELSHADEAPP_H_

#include "DxApp.h"

class DxVertexShader;
class DxPixelShader;

struct CelShadeAppCreateInfo : public DxAppCreateInfo
{

};


class CelShadeApp : public DxApp
{
public:
    static CelShadeApp* Create(CelShadeAppCreateInfo* pAppInfo);

    virtual void Destroy();

    virtual void Run();

    virtual void ReceiveEvent(const Event* pEvent);

protected:
    CelShadeApp(CelShadeAppCreateInfo* pAppInfo);
    virtual ~CelShadeApp();

    virtual bool Init();

    void HandleKeyboardInput(const Event* pEvent);

    void Draw2D();

private:
    DxVertexShader* m_pPosTexTriVS;
    DxVertexShader* m_pPosTexNormVS;
    DxPixelShader* m_pCelShadePS;

    ID2D1SolidColorBrush* m_pLightSlateGrayBrush;
    ID2D1SolidColorBrush* m_pCornflowerBlueBrush;
};

#endif // _CELSHADEAPP_H_