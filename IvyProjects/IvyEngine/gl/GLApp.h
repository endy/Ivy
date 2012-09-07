///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2012, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _GLAPP_H_
#define _GLAPP_H_

#include "IvyApp.h"

///@todo: Support multiple adapters
///@todo: Support framerate capping
///@todo: Create generic config manager

#include "IvyApp.h"

#include "SystemEventDispatcher.h"
#include "IvyInput.h"
#include "IvyPerf.h"

#include <vector>

class IvyWindow;
class IvyCamera;

struct GLAppCreateInfo
{
    UINT screenWidth;
    UINT screenHeight;
    FLOAT nearZ;
    FLOAT farZ;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
/// GLApp Class
///////////////////////////////////////////////////////////////////////////////////////////////////
class GLApp :
    public IvyApp
{
public:
    static GLApp* Create(GLAppCreateInfo* pAppInfo);

    virtual void Destroy();

protected:
    GLApp(GLAppCreateInfo* pAppInfo);
    virtual ~GLApp();

private:
    GLApp(const GLApp& copy);               // Disallow copy constructor
    GLApp& operator=(const GLApp& copy);    // Disallow assignment operator
};

#endif // _GLAPP_H_

