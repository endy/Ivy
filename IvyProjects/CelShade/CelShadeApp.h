///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine - Cel Shading Demo
///
///     Copyright 2010-2012, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CELSHADEAPP_H_
#define _CELSHADEAPP_H_

#include "IvyApp.h"

class DxShader;

///////////////////////////////////////////////////////////////////////////////////////////////////
/// CelShadeApp Class
///////////////////////////////////////////////////////////////////////////////////////////////////
class CelShadeApp : public IvyApp
{
public:
    static CelShadeApp* Create();

    virtual void Destroy();

    virtual void Run();

    virtual void ReceiveEvent(const Event* pEvent);

protected:
    CelShadeApp();
    virtual ~CelShadeApp();

    virtual bool Init();

    void CelShadeGL();
    void CelShadeD3D();

    void HandleKeyboardInput(const Event* pEvent);

private:
    DxShader* m_pPosTexTriVS;
    DxShader* m_pPosTexNormVS;
    DxShader* m_pCelShadePS;
};

#endif // _CELSHADEAPP_H_
