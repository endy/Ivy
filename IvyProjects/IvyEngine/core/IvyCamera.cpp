///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2010-2012, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "IvyCamera.h"

XMMATRIX Perspective(FLOAT nearZ, FLOAT farZ, FLOAT fovX, FLOAT fovY)
{
    FLOAT invDenom = 1.0f / (farZ - nearZ);

    XMVECTOR r0 = {1.0f, 0.0f, 0.0f, 0.0f};
    XMVECTOR r1 = {0.0f, 1.0f, 0.0f, 0.0f};
    XMVECTOR r2 = {0.0f, 0.0f, farZ*invDenom, 1.0f };
    XMVECTOR r3 = {0.0f, 0.0f, -farZ*nearZ*invDenom, 0.0f};

    XMMATRIX perspective(r0, r1, r2, r3);
    
    FLOAT invTanAngX = tanf(fovX/2);
    FLOAT invTanAngY = tanf(fovY/2);
    

    return XMMatrixScaling(invTanAngX, invTanAngY, 1) * perspective;
}

XMMATRIX Orthographic(FLOAT near, FLOAT far)
{
    return XMMatrixIdentity();
}


IvyCamera::IvyCamera(const IvyCameraInfo* pInfo)
{
    memset(&m_worldToCamera, 0, sizeof(XMMATRIX));
    memset(&m_cameraToScreen, 0, sizeof(XMMATRIX));

    m_viewport = pInfo->viewport;
    m_nearZ = pInfo->nearZ;
    m_farZ = pInfo->farZ;

    XMStoreFloat4x4(&m_worldToCamera, XMMatrixIdentity());
    XMStoreFloat4x4(&m_cameraToScreen, XMMatrixIdentity());

    UpdateViewport(m_viewport);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// IvyCamera::~IvyCamera
///
/// @brief
///     Destructor
/// @return
///     N/A
///////////////////////////////////////////////////////////////////////////////////////////////////
IvyCamera::~IvyCamera()
{

}

void IvyCamera::UpdateViewport(Rect viewport)
{
    m_viewport = viewport;

    UINT imagePlaneWidth = m_viewport.right - m_viewport.left;
    UINT imagePlaneHeight = m_viewport.bottom - m_viewport.top;

    XMStoreFloat4x4(&m_screenToRaster, XMMatrixScaling(1.0f/imagePlaneWidth, 1.0f/imagePlaneHeight, 1.0f) *
                                       XMMatrixTranslation(-m_viewport.left, -m_viewport.top, 0));
}


IvyPerspective::IvyPerspective(const IvyPerspectiveCameraInfo* pInfo)
    :
    IvyCamera(pInfo)
{
    XMStoreFloat4x4(&m_cameraToScreen, 
                    Perspective(pInfo->nearZ, pInfo->farZ, pInfo->fovX, pInfo->fovY));
}

IvyPerspective::~IvyPerspective()
{

}


IvyOrthographic::IvyOrthographic(const IvyOrthographicCameraInfo* pInfo)
    : 
    IvyCamera(pInfo)
{
   XMStoreFloat4x4(&m_cameraToScreen, Orthographic(pInfo->nearZ, pInfo->farZ));
}

IvyOrthographic::~IvyOrthographic()
{

}
