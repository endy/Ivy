///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2010-2012, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "IvyCamera.h"

#include <sstream>
using std::stringstream;

#define STUB 1

// Calculates perspective transform using PBRT's perspective transform
IvyMatrix4x4 PerspectivePBRT(FLOAT nearZ, FLOAT farZ, FLOAT fovY)
{
    FLOAT invDenom = 1.0f / (farZ - nearZ);

#if XNA_MATH
    XMVECTOR r0 = {1.0f, 0.0f, 0.0f, 0.0f};
    XMVECTOR r1 = {0.0f, 1.0f, 0.0f, 0.0f};
    XMVECTOR r2 = {0.0f, 0.0f, farZ*invDenom, 1.0f };
    XMVECTOR r3 = {0.0f, 0.0f, -farZ*nearZ*invDenom, 0.0f};

    XMMATRIX perspective(r0, r1, r2, r3);
#else
    IvyMatrix4x4 perspective;
    perspective << 1.0f, 0.0f, 0.0f, 0.0f;
#endif
    FLOAT invTanAng = tanf(fovY/2);
   
#if XNA_MATH
    return XMMatrixScaling(invTanAng, invTanAng, 1) * perspective;
#else
    return perspective;
#endif
}

IvyMatrix4x4 Perspective(FLOAT nearZ, FLOAT farZ, FLOAT aspectRatio, FLOAT fovY)
{
    
    FLOAT fovX = 2*atanf(tanf(fovY/2)*aspectRatio);

    // PBRT 
    //return PerspectivePBRT(nearZ, farZ, fovY);

    CHAR buffer[1024];
    memset(&buffer[0], 0, 1024);
    sprintf(buffer, "FovX: %f radians, %f degrees\nFovY: %f radians, %f dgrees\n",
        fovX, fovX*180/IvyPi, fovY, fovY*180/IvyPi);

    IVY_PRINT(buffer);

    FLOAT f = 1.0f / tan(fovY/2.0f);

    // gluPerspective transform
    XMVECTOR r0 = {f / aspectRatio, 0.0f, 0.0f, 0.0f};
    XMVECTOR r1 = {0.0f, f, 0.0f, 0.0f};
    XMVECTOR r2 = {0.0f, 0.0f, (farZ+nearZ)/ (nearZ-farZ), -1};
    XMVECTOR r3 = {0.0f, 0.0f, (2*farZ*nearZ)/(nearZ-farZ), 0.0f};

    return XMMATRIX(r0, r1, r2, r3);
}


IvyMatrix4x4 Orthographic(FLOAT near, FLOAT far)
{
#if XNA_MATH
    return XMMatrixIdentity();
#else
	return IvyMatrix4x4::Identity();
#endif
}


IvyCamera::IvyCamera(const IvyCameraInfo* pInfo)
{
#if XNA_MATH
    memset(&m_worldToCamera, 0, sizeof(XMMATRIX));
    memset(&m_cameraToScreen, 0, sizeof(XMMATRIX));

    m_viewport = pInfo->viewport;
    m_nearZ = pInfo->nearZ;
    m_farZ = pInfo->farZ;

    XMStoreFloat4x4(&m_worldToCamera, XMMatrixIdentity());
    XMStoreFloat4x4(&m_cameraToScreen, XMMatrixIdentity());
#endif
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

    FLOAT aspectRatio = imagePlaneWidth / (FLOAT) imagePlaneHeight;
    XMStoreFloat4x4(&m_cameraToScreen, 
        Perspective(m_nearZ, m_farZ, aspectRatio, m_fovY));

#if XNA_MATH
    XMStoreFloat4x4(&m_screenToRaster, XMMatrixScaling(1.0f/imagePlaneWidth, 1.0f/imagePlaneHeight, 1.0f) *
                                       XMMatrixTranslation(-m_viewport.left, -m_viewport.top, 0));
#endif

    stringstream ss;
    ss << "IvyCamera: UpdateViewport\n";
#if XNA_MATH
    ss << "World To Camera...\n";

    for (UINT x = 0; x < 4; ++x)
    {
        for (UINT y = 0; y < 4; ++y)
        {
            ss << "[" << x << "][" << y << "]=" << m_worldToCamera.m[x][y] << " ";
        }
        ss << std::endl;
    }

    ss << "Camera To Screen...\n";

    for (UINT x = 0; x < 4; ++x)
    {
        for (UINT y = 0; y < 4; ++y)
        {
            ss << "[" << x << "][" << y << "]=" << m_cameraToScreen.m[x][y] << " ";
        }
        ss << std::endl;
    }

    ss << "Screen To Raster...\n";

    for (UINT x = 0; x < 4; ++x)
    {
        for (UINT y = 0; y < 4; ++y)
        {
            ss << "[" << x << "][" << y << "]=" << m_screenToRaster.m[x][y] << " ";
        }
        ss << std::endl;
    }
#endif // XNA_MATH

    IvyLog(ss.str().c_str());
    IVY_PRINT(ss.str().c_str());
}


IvyPerspective::IvyPerspective(const IvyPerspectiveCameraInfo* pInfo)
    :
    IvyCamera(pInfo)
{
#if XNA_MATH
    if (pInfo->pbrtPerspective)
    {
        XMStoreFloat4x4(&m_cameraToScreen, 
                        PerspectivePBRT(pInfo->nearZ, pInfo->farZ, pInfo->fovY));
    }
    else
    {
        m_fovY = pInfo->fovY;
        FLOAT aspectRatio = (pInfo->viewport.right - pInfo->viewport.left) / 
                            (FLOAT)(pInfo->viewport.bottom - pInfo->viewport.top);
        XMStoreFloat4x4(&m_cameraToScreen, 
                        Perspective(pInfo->nearZ, pInfo->farZ, aspectRatio, pInfo->fovY));
    }
#else
#endif // XNA_MATH
}

IvyPerspective::~IvyPerspective()
{

}


IvyOrthographic::IvyOrthographic(const IvyOrthographicCameraInfo* pInfo)
    : 
    IvyCamera(pInfo)
{
#if XNA_MATH
   XMStoreFloat4x4(&m_cameraToScreen, Orthographic(pInfo->nearZ, pInfo->farZ));
#else
#endif // XNA_MATH
}

IvyOrthographic::~IvyOrthographic()
{

}
