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

#include "DxApp.h"

class DxShader;

struct InstancingAppCreateInfo : public DxAppCreateInfo
{

};


class InstancingApp : public DxApp
{
public:
    static InstancingApp* Create(InstancingAppCreateInfo* pAppInfo);

    virtual void Destroy();

    virtual void Run();

    virtual void ReceiveEvent(const Event* pEvent);

protected:
    InstancingApp(InstancingAppCreateInfo* pAppInfo);
    virtual ~InstancingApp();

    virtual bool Init();

    void HandleKeyboardInput(const Event* pEvent);

    void Draw2D();

private:
    DxShader* m_pPosTexTriVS;
    DxShader* m_pPosTexNormVS;

    ID2D1SolidColorBrush* m_pLightSlateGrayBrush;
    ID2D1SolidColorBrush* m_pCornflowerBlueBrush;
};

#endif // _CELSHADEAPP_H_
