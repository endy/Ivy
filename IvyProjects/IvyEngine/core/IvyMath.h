///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2010-2012, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _IVYMATH_H_
#define _IVYMATH_H_

#include "IvyTypes.h"

#define DIRECTX_MATH 1
//#define EIGEN_MATH 1

///@todo Replace xnamath matrix class with own implementation, at least for 'core' Camera class
#if XNA_MATH__OLD
#include <xnamath.h>
#elif DIRECTX_MATH
#include <DirectXMath.h>
using namespace DirectX;
#define XNA_MATH 1
#elif EIGEN_MATH
#include <Eigen/Core>
using namespace Eigen;
#endif // XNA_MATH

//#include "..\..\..\Lib\Eigen_3.1.2\Eigen\Core"


#if XNA_MATH
typedef XMMATRIX IvyMatrix4x4;
#elif EIGEN_MATH

typedef Matrix4f IvyMatrix4x4;
#else
union Vector4f
{
    struct
    {
        FLOAT x;
        FLOAT y;
        FLOAT z;
        FLOAT w;
    };
    FLOAT v[4];
};

union Matrix4x4
{
    struct
    {
        Vector4f r0;
        Vector4f r1;
        Vector4f r2;
        Vector4f r3;
    };
    FLOAT m[16];
};
#endif


#endif // _IVYMATH_H_
