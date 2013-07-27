///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine - Generic DirectX Test App
///
///     Copyright 2010-2013, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////


#include "DxTestApp.h"
#include "IvyDX.h"
#include "IvyWindow.h"

#include "DxUI.h"
#include "DxBuffer.h"

#include "IvyImporter.h"
#include "DxLight.h"



DxTestApp::DxTestApp()
    :
    IvyApp(),
    m_pAppConstBuffer(NULL)
{

}

DxTestApp::~DxTestApp()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxTestApp::Create
///
/// @brief
///     Creates the DX Test App instance
/// @return
///     DxTestApp instance
///////////////////////////////////////////////////////////////////////////////////////////////////
DxTestApp* DxTestApp::Create()
{
    DxTestApp* pApp = new DxTestApp();

    if (pApp->Init() == false)
    {
        pApp->Destroy();
        pApp = NULL;
    }

    return pApp;
}

void DxTestApp::Destroy()
{
    DeinitDX();

    IvyApp::Destroy();
}

bool DxTestApp::Init()
{
    bool success = IvyApp::Init();

    InitDX();

    return success;
}

void DxTestApp::Run()
{
    //RunInstancing();
    RunLighting();
}

void DxTestApp::ReceiveEvent(const Event* pEvent)
{
    if (pEvent->GetType() == EventTypeWindowResize)
    {
        IvyApp::ReceiveEvent(pEvent);

        // Update constant buffer with new screen width/height
        DxTestAppShaderConsts* pAppConstData = reinterpret_cast<DxTestAppShaderConsts*>(m_pAppConstBuffer->Map(m_pDxData->pD3D11Context));
        pAppConstData->screenWidth  = m_screenWidth;
        pAppConstData->screenHeight = m_screenHeight;
        m_pAppConstBuffer->Unmap(m_pDxData->pD3D11Context);
    }
    else
    {
        IvyApp::ReceiveEvent(pEvent);
    }
}
