///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2010-2011, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _IVYUTILS_H_
#define _IVYUTILS_H_

#include "IvyTypes.h"

#include <string>
#include <fstream>
#include <iostream>

static const FLOAT IvyPi = 3.14159f;

inline void IvyPrint (const char* message, const char* file, unsigned int line)
{
    std::cout << message << " Line(" << line << ") " << file << std::endl;
}

///@todo logging should dump to the console & file
inline void Log(CHAR* component, CHAR* msg)
{
    static const CHAR* pFilename = "log.txt";

    std::fstream fsLog(pFilename, std::ios_base::app);

    fsLog << component << msg << std::endl;

    fsLog.close();
}

inline void Log(CHAR* msg)
{
    Log("", msg);
}

inline void IvyAssert(bool e)
{
    bool QuietAssert = false;

    ///@todo add 'quiet' asserts
    if (e == false && QuietAssert == false)
    {
        ///@todo add logging to asserts
        DebugBreak();
    }
}

inline void IvyAssertAlways()
{
    IvyAssert(false);
}

class Plane
{
public:
    Plane();
    virtual ~Plane();

    const VertexPTN* GetVB() { return m_pVB; }
    static UINT NumVertices() { return VertCount; }

private:
    void Create();

    static const UINT VertCount = 6;
    VertexPTN* m_pVB;
};

class Cube
{
public:
    Cube();
    virtual ~Cube();

    const VertexPT* GetVB() { return m_pVB; }
    UINT NumVertices() { return VertCount; }

private:
    void Create();

    static const UINT VertCount = 36;
    VertexPT* m_pVB;
};


#endif // _IVYUTILS_H_
