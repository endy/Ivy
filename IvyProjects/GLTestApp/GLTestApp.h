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

#include "GLApp.h"

struct GLTestAppCreateInfo : public GLAppCreateInfo
{

};

class GLTestApp : public GLApp
{
public:
    static GLTestApp* Create(GLTestAppCreateInfo* pAppInfo);

    virtual void Destroy();

    virtual void Run();

protected:
    GLTestApp(GLTestAppCreateInfo* pAppInfo);
    virtual ~GLTestApp();

    virtual bool Init();

private:
    GLTestApp(const GLTestApp& copy); // disallow copy constructor

    void InitGL2();
    void InitGL4();

    void DrawTestGL2();
    void DrawTestGL4();

    void DrawParticles();

    // Win32 handles
    HDC m_hDC;
    HGLRC m_hGLRC;
};

#endif // _GLTESTAPP_H_