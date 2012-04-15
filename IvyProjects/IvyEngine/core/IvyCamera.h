///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2010-2012, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _IVYCAMERA_H_
#define _IVYCAMERA_H_

#include "IvyTypes.h"
#include "IvyObject.h"

///@todo Replace xnamath matrix class with own implementation, at least for 'core' Camera class
#ifdef _WIN32
#include <xnamath.h>
#endif // _WIN32

struct CameraBufferData
{
    XMMATRIX worldMatrix;
    XMMATRIX viewMatrix;
    XMMATRIX projectionMatrix;
};

struct IvyCameraInfo
{
    Rect viewport;
    FLOAT nearZ;
    FLOAT farZ;

};

class IvyCamera :
    public IvyObject
{
public:
    IvyCamera(const IvyCameraInfo* pCreateInfo);
    virtual ~IvyCamera();

    void UpdateViewport(Rect viewport);

    // todo: rename these methods to something more reasonable
    XMMATRIX W2C(){ return XMLoadFloat4x4(&m_worldToCamera); }
    XMMATRIX C2S(){ return XMLoadFloat4x4(&m_cameraToScreen); }

    // todo: refactor these away
    Point3& Position() { return m_position; }
    Point3& LookAt() { return m_lookAt; }

protected:

    XMFLOAT4X4 m_worldToCamera;  // view ??
    XMFLOAT4X4 m_cameraToScreen; // projection
    XMFLOAT4X4 m_screenToRaster; // viewport mapping

    Rect m_viewport;
    FLOAT m_nearZ;
    FLOAT m_farZ;

    Point3 m_position;
    Point3 m_lookAt;
};


struct IvyPerspectiveCameraInfo : public IvyCameraInfo
{
    FLOAT fovX;
    FLOAT fovY;
};

class IvyPerspective : public IvyCamera
{
public:
    IvyPerspective(const IvyPerspectiveCameraInfo* pCreateInfo);
    virtual ~IvyPerspective();
};

struct IvyOrthographicCameraInfo : public IvyCameraInfo
{

};

class IvyOrthographic : public IvyCamera
{
public:
    IvyOrthographic(const IvyOrthographicCameraInfo* pCreateInfo);
    virtual ~IvyOrthographic();
};


#endif // _IVYCAMERA_H_

