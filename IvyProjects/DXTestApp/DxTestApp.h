///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine - Generic DirectX Test App
///
///     Copyright 2010-2011, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DXTESTAPP_H_
#define _DXTESTAPP_H_

#include "IvyApp.h"

class DxBuffer;

struct DxTestAppShaderConsts
{
    UINT screenWidth;
    UINT screenHeight;
    UINT padding[2];
};

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxTestApp Class
///////////////////////////////////////////////////////////////////////////////////////////////////
class DxTestApp : public Ivy::IvyApp
{
public:
    static DxTestApp* Create();

    virtual void Destroy();

    virtual void Run();

    virtual void ReceiveEvent(const Event* pEvent);

protected:
    DxTestApp();
    virtual ~DxTestApp();

    virtual bool Init();

private:
    DxTestApp(const DxTestApp& copy);	// disallow copy constructor

    void RunInstancing();
    void RunLighting();

    DxBuffer* m_pAppConstBuffer;    ///< Global shader constants
};

#endif // _DXTESTAPP_H_

