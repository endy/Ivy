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

    void RenderGL2();
    void RenderGL4();
};

#endif // _GLTESTAPP_H_