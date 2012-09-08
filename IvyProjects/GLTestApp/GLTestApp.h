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

    void IvySwapBuffers();

private:
    GLTestApp(const GLTestApp& copy); // disallow copy constructor

    void InitGL2();
    void InitGL4();
    void InitGLES2();

    void DrawTestGL2();
    void DrawTestGL4();
    void DrawTestGLES2();

    void ParticlesTest();
    void ReceiveEventParticles(const Event* pEvent);

    BOOL m_useEgl;

    // Win32 handles
    HDC m_hDC;
    HGLRC m_hGLRC;

#if IVY_GL_ES
    // EGL & GLES
    EGLDisplay m_eglDisplay;
    EGLSurface m_eglSurface;
    EGLContext m_eglContext;
#endif // IVY_GL_ES

};

#endif // _GLTESTAPP_H_