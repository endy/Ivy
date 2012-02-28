///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2012, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "GLApp.h"
#include "GLWindow.h"

#include "IvyCamera.h"

const UINT GLApp::BufferCount = 1;

///////////////////////////////////////////////////////////////////////////////////////////////////
/// GLApp::GLApp
///////////////////////////////////////////////////////////////////////////////////////////////////
GLApp::GLApp(
    GLAppCreateInfo* pAppInfo)
    :
    m_pWindow(NULL),
    m_screenWidth(pAppInfo->screenWidth),
    m_screenHeight(pAppInfo->screenHeight),
    m_fovX(90 * (IvyPi/180)),
    m_fovY(90 * (IvyPi/180)),
    m_nearZ(pAppInfo->nearZ),
    m_farZ(pAppInfo->farZ),
    m_pCamera(NULL)
{

}

/**************************************************************************************************
    GLApp::~GLApp
**************************************************************************************************/
GLApp::~GLApp()
{

}

/**************************************************************************************************
GLApp::Create
**************************************************************************************************/
GLApp* GLApp::Create(
    GLAppCreateInfo* pAppInfo)
{
    GLApp* pApp = new GLApp(pAppInfo);

    if (pApp->Init() == false)
    {
        pApp->Destroy();
        pApp = NULL;
    }

    return pApp;
}

/**************************************************************************************************
GLApp::Destroy
**************************************************************************************************/
void GLApp::Destroy()
{
    delete this;
}

/**************************************************************************************************
GLApp::Create
**************************************************************************************************/
bool GLApp::Init()
{
    SystemEventDispatcher::Get()->RegisterReceiver(this);

    bool success = TRUE;

    // Setup Window
    m_pWindow = GLWindow::Create(m_screenWidth, m_screenHeight);

    IvyPerspectiveCameraInfo cameraCreateInfo;
    memset(&cameraCreateInfo, 0, sizeof(IvyPerspectiveCameraInfo));
    cameraCreateInfo.farZ = m_farZ;
    cameraCreateInfo.nearZ = m_nearZ;
    cameraCreateInfo.viewport.bottom = m_screenHeight;
    cameraCreateInfo.viewport.left = m_screenWidth;
    cameraCreateInfo.fovX = m_fovX;
    cameraCreateInfo.fovY = m_fovY;

    m_pCamera = new IvyPerspective(&cameraCreateInfo);

    return success;
}

/**************************************************************************************************
GLApp::Run
**************************************************************************************************/
void GLApp::Run()
{

}

void GLApp::ReceiveEvent(
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

void GLApp::UpdateMousePosition()
{
    POINT point;

    GetCursorPos(&point);
    ScreenToClient(m_pWindow->GetHwnd(), &point);

    m_mousePos.x = static_cast<FLOAT>(point.x);
    m_mousePos.y = static_cast<FLOAT>(point.y);
}

void GLApp::BeginFrame()
{
    m_framerateTracker.BeginFrame();
}

void GLApp::EndFrame()
{
    m_framerateTracker.EndFrame();
}

