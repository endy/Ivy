///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2010-2011, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _IVYMEMORY_H_
#define _IVYMEMORY_H_

#include "IvyUtils.h"

class IvyMemoryMgr
{
public:
    static void* Alloc(size_t size)
    {
        std::cout << "Alloc size=" << size << std::endl;
        return calloc(1, size);
    };

    static void Free(void* pMem)
    {
        std::cout << "Free Pointer=" << pMem << std::endl;
        free(pMem);
    }
};

inline void* IvyTrackedAlloc (size_t n, const char* file, unsigned int line)
{
    IvyPrint("IvyAlloc", file, line);
    return IvyMemoryMgr::Alloc(n);
}

inline void IvyTrackedFree (void* in, const char* file, unsigned int line)
{
    IvyPrint("IvyFree", file, line);
    IvyMemoryMgr::Free(in);
}

#define TrackMemory 1

#if TrackMemory

#define IvyRawAlloc(size) IvyTrackedAlloc((size), (__FILE__), (__LINE__))
#define IvyTypeAlloc(datatype, count) reinterpret_cast<datatype*>(IvyTrackedAlloc(sizeof(datatype)*count, (__FILE__), (__LINE__)))
#define IvyFree(ptr) IvyTrackedFree((ptr), (__FILE__), (__LINE__))

#define IvyNew(datatype) new datatype; IvyPrint("IvyNew", __FILE__, __LINE__)
#define IvyArrayNew(datatype, count) (new datatype[count]); IvyPrint("IvyArrayNew", __FILE__, __LINE__)
#define IvyDelete(ptr) IvyPrint("IvyDelete", __FILE__, __LINE__); delete (ptr)
#define IvyArrayDelete(ptr) IvyPrint("IvyArrayDelete", __FILE__, __LINE__); delete [](ptr)

#else
#define IvyAlloc(size) malloc(size)
#define IvyFree(ptr) free(ptr)
#define IvyNew(datatype) new (datatype)
#define IvyArrayNew(datatype, count) (new datatype[count])
#define IvyDelete(ptr) delete (ptr)
#define IvyArrayDelete(ptr) delete [] (ptr)
#endif


#endif // _IVYMEMORY_H_

