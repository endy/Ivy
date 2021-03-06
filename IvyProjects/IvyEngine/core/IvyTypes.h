///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2010-2011, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _IVYTYPES_H_
#define _IVYTYPES_H_

#include "IvyPlatform.h"

#ifdef _WIN32
#else
#include <inttypes.h>
#endif // _WIN32

#ifdef _WIN32
typedef char CHAR;
typedef unsigned int UINT;
typedef float FLOAT;
#else
typedef char CHAR;
typedef unsigned int UINT;
typedef float FLOAT;
typedef double DOUBLE;
typedef uint64_t UINT64;
#endif

#ifndef _WIN32
#define FALSE false
#endif

namespace Ivy
{

    struct Rect
    {
        UINT left;
        UINT right;
        UINT top;
        UINT bottom;
    };

    struct Point2
    {
        Point2() : x(0), y(0) {};
        Point2(FLOAT xIn, FLOAT yIn) : x(xIn), y(yIn) {};

        FLOAT x;
        FLOAT y;
    };

    struct Point3
    {
        Point3() : x(0), y(0), z(0) {};
        Point3(FLOAT xIn, FLOAT yIn, FLOAT zIn) : x(xIn), y(yIn), z(zIn) {};

        FLOAT x;
        FLOAT y;
        FLOAT z;
    };

    struct Point4
    {
        Point4() : x(0), y(0), z(0), w(0) {};
        Point4(FLOAT xIn, FLOAT yIn, FLOAT zIn, FLOAT wIn) : x(xIn), y(yIn), z(zIn), w(wIn) {};

        union
        {
            FLOAT x;
            FLOAT r;
        };
        union
        {
            FLOAT y;
            FLOAT g;
        };
        union
        {
            FLOAT z;
            FLOAT b;
        };
        union
        {
            FLOAT w;
            FLOAT a;
        };
    };

    struct VertexPT
    {
        Point3 Pos;		// Position
        Point2 Tex;		// Texture Coord
    };

    struct VertexPTN
    {
        Point3 Pos;     // Position
        Point2 Tex;     // Texture Coord
        Point4 N;       // Normal
    };

    struct Material
    {
        Point4 diffuse;
        Point4 specular;
        Point4 ambient;
    };

}

#endif // _IVYTYPES_H_
