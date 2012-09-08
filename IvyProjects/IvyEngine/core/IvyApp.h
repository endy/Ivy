///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2012, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _IVYAPP_H_
#define _IVYAPP_H_

#include "IvyObject.h"

#include "SystemEventDispatcher.h"
#include "IvyInput.h"
#include "IvyPerf.h"

class IvyWindow;
class IvyCamera;
struct IvyConfigItem;

class IvyApp : 
    public IvyObject,
    public IEventReceiver
{
public:

    virtual void Destroy();

    virtual void Run();

    virtual void ReceiveEvent(const Event* pEvent);

    Point2 GetMousePos() { return m_mousePos; }

protected:
    IvyApp();
    virtual ~IvyApp();

    virtual bool Init();

    void UpdateSwapChain();
    void UpdateMousePosition();

    void BeginFrame();
    void EndFrame();

    IvyWindow* m_pWindow;

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
    IvyApp(const IvyApp& copy);               // Disallow copy constructor
    IvyApp& operator=(const IvyApp& copy);    // Disallow assignment operator

    FramerateTracker m_framerateTracker;

    KeyboardState m_keys;

    // Configuration
    static unsigned int ScreenWidth;
    static unsigned int ScreenHeight;
    static bool UseGL;
    static bool UseD3D;

    static IvyConfigItem IvyAppConfig[];
};

#endif // _IVYAPP_H_

