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



#endif // _IVYMATH_H_
