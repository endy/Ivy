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

inline void IvyPrint (const CHAR* message, const CHAR* file, unsigned int line)
{
    std::cout << message << " Line(" << line << ") " << file << std::endl;
}

#define IVY_PRINT(msg) IvyPrint((msg), __FILE__, __LINE__)

///@todo logging should dump to the console & file
inline void IvyLog(const CHAR* component, const CHAR* msg)
{
    static const CHAR* pFilename = "log.txt";

    std::fstream fsLog(pFilename, std::ios_base::app);

    fsLog << component << msg << std::endl;

    fsLog.close();
}

inline void IvyLog(const CHAR* msg)
{
    IvyLog("", msg);
}

inline void IvyAssert(bool e)
{
    bool QuietAssert = false;

    ///@todo add 'quiet' asserts
    if (e == false && QuietAssert == false)
    {
        ///@todo add logging to asserts
#ifdef _WIN32
        DebugBreak();
#else
        asm("int3");
#endif
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
