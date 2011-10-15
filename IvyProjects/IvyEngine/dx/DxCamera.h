///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2010-2011, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DXCAMERA_H_
#define _DXCAMERA_H_

#include "IvyTypes.h"
#include "IvyObject.h"

//TODO: replace xnamath matrix class with own implementation, at least for 'core' Camera class
#include <Windows.h>
#include <xnamath.h>

struct CameraBufferData
{
    XMMATRIX worldMatrix;
    XMMATRIX viewMatrix;
    XMMATRIX projectionMatrix;
};

struct DxCameraInfo
{
    Rect viewport;
    FLOAT nearZ;
    FLOAT farZ;

};

class DxCamera :
    public IvyObject
{
public:
    DxCamera(const DxCameraInfo* pCreateInfo);
    virtual ~DxCamera();

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
    XMFLOAT4X4 m_screenToRaster; // wut

    Rect m_viewport;
    FLOAT m_nearZ;
    FLOAT m_farZ;

    Point3 m_position;
    Point3 m_lookAt;
};


struct DxPerspectiveCameraInfo : public DxCameraInfo
{
    FLOAT FovX;
    FLOAT FovY;
};

class DxPerspective : public DxCamera
{
public:
    DxPerspective(const DxPerspectiveCameraInfo* pCreateInfo);
    virtual ~DxPerspective();
};

struct DxOrthographicCameraInfo : public DxCameraInfo
{

};

class DxOrthographic : public DxCamera
{
public:
    DxOrthographic(const DxOrthographicCameraInfo* pCreateInfo);
    virtual ~DxOrthographic();
};


#endif // _DXCAMERA_H_

