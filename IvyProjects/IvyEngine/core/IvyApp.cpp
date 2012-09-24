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
bool IvyApp::UseGL                = true;
bool IvyApp::UseD3D               = false;

IvyConfigItem IvyApp::IvyAppConfig[] = 
{
    { "uScreenWidth",   "Screen Width",                 IvyUint,    &ScreenWidth,   false },
    { "uScreenHeight",  "Screen Height",                IvyUint,    &ScreenHeight,  false },
    { "bUseGL",         "Use OpenGL",                   IvyBool,    &UseGL,         false },
    { "bUseD3D",        "Use Direct3D",                 IvyBool,    &UseD3D,        false },

    // Sentinel
    { NULL, NULL, IvyUnknown, NULL, false }
};

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
    m_fovX(90 * (IvyPi/180)),
    m_fovY(90 * (IvyPi/180)),
    m_nearZ(1.0),
    m_farZ(20.0),
    m_pCamera(NULL)
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
    char* pConfigString = IvyConfigBuildString(argc, argv);

    bool success = IvyConfigParseConfigString(pConfigString, IvyAppConfig);

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

    // Setup Camera
    IvyPerspectiveCameraInfo cameraCreateInfo;
    memset(&cameraCreateInfo, 0, sizeof(IvyPerspectiveCameraInfo));
    cameraCreateInfo.farZ = m_farZ;
    cameraCreateInfo.nearZ = m_nearZ;
    cameraCreateInfo.viewport.bottom = m_screenHeight;
    cameraCreateInfo.viewport.right = m_screenWidth;
    cameraCreateInfo.fovX = m_fovX;
    cameraCreateInfo.fovY = m_fovY;

    m_pCamera = new IvyPerspective(&cameraCreateInfo);

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
        break;
    case EventTypeMouseMove:
        UpdateMousePosition();
        break;
    default:
        // Do nothing
        break;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// IvyApp::UpdateMousePosition
///
/// @brief
///     
/// @return
///     N/A
///////////////////////////////////////////////////////////////////////////////////////////////////
void IvyApp::UpdateMousePosition()
{
    POINT point;

    GetCursorPos(&point);
    ScreenToClient(m_pWindow->GetHwnd(), &point);

    m_mousePos.x = static_cast<FLOAT>(point.x);
    m_mousePos.y = static_cast<FLOAT>(point.y);
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
}

