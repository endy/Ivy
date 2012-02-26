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

///@todo: Support multiple adapters
///@todo: Support framerate capping
///@todo: Create generic config manager

#include "IvyObject.h"


#include "SystemEventDispatcher.h"
#include "IvyInput.h"
#include "IvyPerf.h"

#include <vector>

class GLWindow;
class IvyCamera;

struct GLAppCreateInfo
{
    UINT screenWidth;
    UINT screenHeight;
    FLOAT nearZ;
    FLOAT farZ;
};

class GLApp :
    public IvyObject,
    public IEventReceiver
{
public:
    static GLApp* Create(GLAppCreateInfo* pAppInfo);

    virtual void Destroy();

    virtual void Run();

    virtual void ReceiveEvent(const Event* pEvent);

    Point2 GetMousePos() { return m_mousePos; }

protected:
    GLApp(GLAppCreateInfo* pAppInfo);
    virtual ~GLApp();

    virtual bool Init();

    void UpdateSwapChain();
    void UpdateMousePosition();

    void BeginFrame();
    void EndFrame();

    GLWindow* m_pWindow;

    Point2 m_mousePos;

    FLOAT m_fovX;           // FOV X
    FLOAT m_fovY;           // FOV Y
    FLOAT m_nearZ;          // near plane
    FLOAT m_farZ;           // far plane
    UINT  m_screenWidth;    // viewport width
    UINT  m_screenHeight;   // viewport height

    static const UINT BufferCount;

    IvyCamera* m_pCamera;

private:
    GLApp(const GLApp& copy);               // Disallow copy constructor
    GLApp& operator=(const GLApp& copy);    // Disallow assignment operator

    FramerateTracker m_framerateTracker;

    KeyboardState m_keys;
};

#endif // _GLAPP_H_
