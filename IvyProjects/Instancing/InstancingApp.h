///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine - Instancing Demo
///
///     Copyright 2011, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _INSTANCINGAPP_H_
#define _INSTANCINGAPP_H_

#include "IvyApp.h"

class DxShader;

class InstancingApp : public IvyApp
{
public:
    static InstancingApp* Create();

    virtual void Destroy();

    virtual void Run();

    virtual void ReceiveEvent(const Event* pEvent);

protected:
    InstancingApp();
    virtual ~InstancingApp();

    virtual bool Init();

    void HandleKeyboardInput(const Event* pEvent);

private:
    DxShader* m_pPosTexTriVS;
    DxShader* m_pPosTexNormVS;
};

#endif // _CELSHADEAPP_H_
