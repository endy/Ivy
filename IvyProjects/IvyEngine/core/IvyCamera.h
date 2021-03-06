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
#include "IvyMath.h"

namespace Ivy
{

    struct CameraBufferData
    {
#if XNA_MATH
        XMMATRIX worldMatrix;
        XMMATRIX viewMatrix;
        XMMATRIX projectionMatrix;
#elif EIGEN_MATH
        IvyMatrix4x4 worldMatrix;
        IvyMatrix4x4 viewMatrix;
        IvyMatrix4x4 projectionMatrix;
#else
        // Ivy Math
        Matrix4x4 worldMatrix;
        Matrix4x4 viewMatrix;
        Matrix4x4 projectionMatrix;
#endif // XNA_MATH
    };

    struct IvyCameraInfo
    {
        Rect viewport;
        FLOAT nearZ;
        FLOAT farZ;

    };

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    /// IvyCamera Class
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    class IvyCamera :
        public IvyObject
    {
    public:

#if EIGEN_MATH
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
#endif

            virtual ~IvyCamera();

        void UpdateViewport(Rect viewport);

        // todo: rename these methods to something more reasonable
#if XNA_MATH
        XMMATRIX W2C(){ return XMLoadFloat4x4(&m_worldToCamera); }
        XMMATRIX C2S(){ return XMLoadFloat4x4(&m_cameraToScreen); }
#else
        IvyMatrix4x4 W2C(){ return m_worldToCamera; }
        IvyMatrix4x4 C2S(){ return m_cameraToScreen; }
#endif // XNA_MATH


        void Position(Point3& position) const { position = m_position; }
        void Orientation(FLOAT& phi, FLOAT& theta) const { phi = m_phi; theta = m_theta; }
        Point3 LookAt();

        void Move(Point3 deltaPosition, FLOAT deltaPhi, FLOAT deltaTheta);
        void Reset();

        void Print();

    protected:
        IvyCamera(const IvyCameraInfo* pCreateInfo);

#if XNA_MATH
        XMFLOAT4X4 m_worldToCamera;  // view ??
        XMFLOAT4X4 m_cameraToScreen; // projection
        XMFLOAT4X4 m_screenToRaster; // viewport mapping
#else
        IvyMatrix4x4 m_worldToCamera;  // view ??
        IvyMatrix4x4 m_cameraToScreen; // projection
        IvyMatrix4x4 m_screenToRaster; // viewport mapping
#endif

        Rect m_viewport;
        FLOAT m_nearZ;
        FLOAT m_farZ;

        FLOAT m_fovY;

        // Position
        Point3 m_position;

        // Orientation
        FLOAT m_phi;
        FLOAT m_theta;
    };


    struct IvyPerspectiveCameraInfo : public IvyCameraInfo
    {
        FLOAT fovY;

        BOOL  pbrtPerspective;
    };

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    /// IvyPerspective Camera Class
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    class IvyPerspective : public IvyCamera
    {
    public:
        IvyPerspective(const IvyPerspectiveCameraInfo* pCreateInfo);
        virtual ~IvyPerspective();
    };

    struct IvyOrthographicCameraInfo : public IvyCameraInfo
    {

    };

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    /// IvyOrthographic Camera Class
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    class IvyOrthographic : public IvyCamera
    {
    public:
        IvyOrthographic(const IvyOrthographicCameraInfo* pCreateInfo);
        virtual ~IvyOrthographic();
    };

}

#endif // _IVYCAMERA_H_

