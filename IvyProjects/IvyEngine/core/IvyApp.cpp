///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2012, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "IvyApp.h"
#include "IvyWindow.h"
#include "IvyCamera.h"

const UINT IvyApp::BufferCount = 1;

unsigned int IvyApp::ScreenWidth  = 512;
unsigned int IvyApp::ScreenHeight = 512;

IvyConfigItem IvyApp::IvyAppConfig[] = 
{
    { "uScreenWidth",   "Screen Width",                 IvyUint,    &ScreenWidth,   false },
    { "uScreenHeight",  "Screen Height",                IvyUint,    &ScreenHeight,  false },

    // Sentinel
    { NULL, NULL, IvyUnknown, NULL, false }
};

// ConsoleHandler routine for Console App events
BOOL WINAPI ConsoleHandlerRoutine(
    DWORD dwCtrlType)
{
    BOOL quit = FALSE;

    if (dwCtrlType == CTRL_CLOSE_EVENT)
    {
        IvyLog("IvyApp: ", "Console Closed, Force Immediate Shutdown");
    }

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// IvyApp::IvyApp
///
/// @brief
///     
/// @return
///     N/A
///////////////////////////////////////////////////////////////////////////////////////////////////
IvyApp::IvyApp() :
    m_pWindow(NULL),
    m_screenWidth(ScreenWidth),
    m_screenHeight(ScreenHeight),
    m_fovY(90 * (IvyPi/180)),
    m_nearZ(0.00001),
    m_farZ(20.0),
    m_pCamera(NULL),
    m_exit(FALSE)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// IvyApp::~IvyApp
///
/// @brief
///     
/// @return
///     N/A
///////////////////////////////////////////////////////////////////////////////////////////////////
IvyApp::~IvyApp()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// IvyApp::Destroy
///
/// @brief
///     
/// @return
///     N/A
///////////////////////////////////////////////////////////////////////////////////////////////////
void IvyApp::Destroy()
{
    delete this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// IvyApp::Configure
///
/// @brief
///     
/// @return
///     N/A
///////////////////////////////////////////////////////////////////////////////////////////////////
bool IvyApp::Configure(unsigned int argc, const char** argv)
{
    bool success = true;

    char* pConfigString = IvyConfigBuildString(argc, argv);

    if (strlen(pConfigString) > 0)
    {
        success = IvyConfigParseConfigString(pConfigString, IvyAppConfig);
    }

    IVY_FREE(pConfigString);

    return success;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// IvyApp::QueryConfig
///
/// @brief
///     
/// @return
///     Pointer to the IvyConfigItem that has the given name
///////////////////////////////////////////////////////////////////////////////////////////////////
 IvyConfigItemValue* IvyApp::QueryConfig(
     const char* pName) ///< Name of config item
{
    IvyConfigItemValue* pItemValue = NULL;

    unsigned int idx = 0;
    while (IvyAppConfig[idx].name != NULL)
    {
        if (stricmp(IvyAppConfig[idx].name, pName) == 0)
        {
            pItemValue = &IvyAppConfig[idx].value;
            break;
        }
        idx++;
    }

    return pItemValue;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// IvyApp::Init
///
/// @brief
///     
/// @return
///     N/A
///////////////////////////////////////////////////////////////////////////////////////////////////
bool IvyApp::Init()
{
    SystemEventDispatcher::Get()->RegisterReceiver(this);

    bool success = TRUE;

    // Setup Window
    m_pWindow = IvyWindow::Create(m_screenWidth, m_screenHeight);

    // Setup Console Event Handler
    SetConsoleCtrlHandler(ConsoleHandlerRoutine, TRUE);

    // Setup Camera
    IvyPerspectiveCameraInfo cameraCreateInfo;
    memset(&cameraCreateInfo, 0, sizeof(IvyPerspectiveCameraInfo));
    cameraCreateInfo.farZ = m_farZ;
    cameraCreateInfo.nearZ = m_nearZ;
    cameraCreateInfo.viewport.bottom = m_screenHeight;
    cameraCreateInfo.viewport.right = m_screenWidth;
    cameraCreateInfo.fovY = m_fovY;

    m_pCamera = new IvyPerspective(&cameraCreateInfo);

    /*
	///@TODO Expose orthographic camera option in config
    IvyOrthographicCameraInfo orthoInfo;
    memset(&orthoInfo, 0, sizeof(IvyOrthographicCameraInfo));
    orthoInfo.farZ = m_farZ;
    orthoInfo.nearZ = m_nearZ;
    orthoInfo.viewport.bottom = m_screenHeight;
    orthoInfo.viewport.right = m_screenWidth;
    m_pCamera = new IvyOrthographic(&orthoInfo);
    //*/

    return success;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// IvyApp::Run
///
/// @brief
///     
/// @return
///     N/A
///////////////////////////////////////////////////////////////////////////////////////////////////
void IvyApp::Run()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// IvyApp::ProcessUpdates
///
/// @brief
///     
/// @return
///     N/A
///////////////////////////////////////////////////////////////////////////////////////////////////
void IvyApp::ProcessUpdates()
{
    m_pWindow->ProcessMsg(&m_exit);

    IvyGetGamepadStates(&m_gamepad, 1);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// IvyApp::ReceiveEvent
///
/// @brief
///     
/// @return
///     N/A
///////////////////////////////////////////////////////////////////////////////////////////////////
void IvyApp::ReceiveEvent(
    const Event* pEvent)
{
    switch (pEvent->GetType())
    {
        case EventTypeWindowResize:
            m_pWindow->Resize();
#ifndef IVY_GL_ONLY
            UpdateSwapChain();
#endif // IVY_GL_ONLY
            break;
        case EventTypeMouseMove:
        case EventTypeMouseButtonDown:
        case EventTypeMouseButtonUp:
        case EventTypeMouseDoubleClick:
            ProcessMouseEvent(pEvent);
            break;
        case EventTypeKeyDown:
        case EventTypeKeyUp:
            ProcessKeyboardEvent(pEvent);
        default:
            // Do nothing
            break;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// IvyApp::ProcessMouseEvent
///
/// @brief
///     
/// @return
///     N/A
///////////////////////////////////////////////////////////////////////////////////////////////////
void IvyApp::ProcessMouseEvent(
    const Event* pEvent)    ///< Mouse event
{
    if (pEvent->GetType() == EventTypeMouseMove)
    {
        POINT point;
        GetCursorPos(&point);
        ScreenToClient(m_pWindow->GetHwnd(), &point);

        m_mouse.x = point.x;
        m_mouse.y = point.y;
    }
    else if (pEvent->GetType() == EventTypeMouseButtonDown)
    {
        EventDataMouseButton mouseData = reinterpret_cast<const EventMouseButtonDown*>(pEvent)->GetData();
        m_mouse.Pressed[mouseData.button] = TRUE;
    }
    else if (pEvent->GetType() == EventTypeMouseButtonUp)
    {
        EventDataMouseButton mouseData = reinterpret_cast<const EventMouseButtonDown*>(pEvent)->GetData();
        m_mouse.Pressed[mouseData.button] = FALSE;
    }
    else
    {
        IvyAssert(pEvent->GetType() == EventTypeMouseDoubleClick);
        
    }
}

void IvyApp::ProcessKeyboardEvent(
    const Event* pEvent)
{
    KeyboardKey key;

    if (pEvent->GetType() == EventTypeKeyDown)
    {
        key = reinterpret_cast<const EventKeyUp*>(pEvent)->GetData().key;
        m_keys.Pressed[key] = TRUE;
    }
    else
    {
        IvyAssert(pEvent->GetType() == EventTypeKeyUp);

        key = reinterpret_cast<const EventKeyUp*>(pEvent)->GetData().key;
        m_keys.Pressed[key] = FALSE;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// IvyApp::BeginFrame
///
/// @brief
///     
/// @return
///     N/A
///////////////////////////////////////////////////////////////////////////////////////////////////
void IvyApp::BeginFrame()
{
    m_framerateTracker.BeginFrame();

    ProcessUpdates();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// IvyApp::EndFrame
///
/// @brief
///     
/// @return
///     N/A
///////////////////////////////////////////////////////////////////////////////////////////////////
void IvyApp::EndFrame()
{
    m_framerateTracker.EndFrame();

    ///@TODO Allow unlockable framerate via config
    // if (lock framerate)
    {
        // 16 ms = 60 FPS
        int maxFrametime = 16;  

        ///@TODO consider remainder handling, +1 rounds up
        int actualFrameTime = ((int)m_framerateTracker.LastFrameTime() * 1000) + 1;

        Sleep(maxFrametime - (actualFrameTime % maxFrametime));
    }
}

