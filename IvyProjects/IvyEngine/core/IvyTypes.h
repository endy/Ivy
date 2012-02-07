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

#ifdef _WIN32
#include <Windows.h>
#endif

typedef char CHAR;
typedef unsigned int UINT;
typedef float FLOAT;
typedef unsigned __int64 UINT64;

struct Rect
{
    UINT left;
    UINT right;
    UINT top;
    UINT bottom;
};

struct Point2
{
    union
    {
        FLOAT x;
        FLOAT u;
    };
    union
    {
        FLOAT y;
        FLOAT v;
    };
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

#endif // _IVYTYPES_H_
