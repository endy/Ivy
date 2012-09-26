///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine - Generic DirectX Test App
///
///     Copyright 2010-2011, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _GLTESTAPP_H_
#define _GLTESTAPP_H_

#include "IvyApp.h"
#include "IvyGL.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
/// GLTestApp Class
///////////////////////////////////////////////////////////////////////////////////////////////////
class GLTestApp : public IvyApp
{
public:
    static GLTestApp* Create();

    virtual void Destroy();

    virtual void Run();

protected:
    GLTestApp();
    virtual ~GLTestApp();

    virtual bool Init();

    virtual void ReceiveEvent(const Event* pEvent);

private:
    GLTestApp(const GLTestApp& copy); // disallow copy constructor

    void DrawTestGL2();
    void DrawTestGL4();
    void DrawTestGLES2();

    void ParticlesTest();
    void LightingTest();

    void ReceiveEventParticles(const Event* pEvent);

    BOOL m_useEgl;

};

#endif // _GLTESTAPP_H_