///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine - Memory Allocation Test
///
///     Copyright 2011, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////


#include <iostream>
#include <iomanip>

#include "IvyPerf.h"
#include "SimpleMemoryPool.h"

void IvyPrint (const char* message, const char* file, unsigned int line)
{
    std::cout << message << " Line(" << line << ") " << file << std::endl;
}

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

void* IvyTrackedAlloc (size_t n, const char* file, unsigned int line)
{
    IvyPrint("IvyAlloc", file, line);
    return IvyMemoryMgr::Alloc(n);
}

void IvyTrackedFree (void* in, const char* file, unsigned int line)
{
    IvyPrint("IvyFree", file, line);
    IvyMemoryMgr::Free(in);
}

#define TrackMemory 0

#if TrackMemory
#define IvyAlloc(size) IvyTrackedAlloc((size), (__FILE__), (__LINE__))
#define IvyFree(ptr) IvyTrackedFree((ptr), (__FILE__), (__LINE__))

#define IvyNew(datatype) new datatype; IvyPrint("IvyNew", __FILE__, __LINE__)
#define IvyArrayNew(datatype,count) (new datatype[count]); IvyPrint("IvyArrayNew", __FILE__, __LINE__)

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



class IvyObject
{
public:
    void* operator new(size_t size) 
    {
        return IvyMemoryMgr::Alloc(size);
    }
    void* operator new[](size_t size)
    {
        return IvyMemoryMgr::Alloc(size);
    }
    void operator delete(void* pObj)
    {
        IvyMemoryMgr::Free(pObj);
    }
    void operator delete[](void* pObj)
    {
        IvyMemoryMgr::Free(pObj);
    }

protected:
    IvyObject() { }
    virtual ~IvyObject() { }
};

class SimpleClass : public IvyObject
{
public:
    SimpleClass() : m_id(0) {}
    ~SimpleClass(){}

    void PrintName(){ std::cout << "Simple Class Id = " << m_id << std::endl; }

private:
    int m_id;
};

// How long it takes to allocate X times of size Y
void VariedSizeAndCountTest()
{
    std::cout << "Varied Size & Count Test" << std::endl;

    const UINT AllocSizesInBytes[] = 
    { 
        1,                  // 1 B
        1 * 1024,           // 1 KB
        4 * 1024,           // 4 KB
        8 * 1024,           // 8 KB
        256 * 1024,         // 256 KB
        1 * 1024 * 1024,    // 1 MB
    };

    const UINT AllocSizesParamCount = sizeof(AllocSizesInBytes) / sizeof(AllocSizesInBytes[0]);

    const UINT AllocCount[] = { 1, 10, 100, 1000, 10000 };
    const UINT AllocCountParamCount = sizeof(AllocCount) / sizeof(AllocCount[0]);

    PerfData perfData;

    for (UINT sizeIdx = 0; sizeIdx < AllocSizesParamCount; ++sizeIdx)
    {
        for (UINT allocCountIdx = 0; allocCountIdx < AllocCountParamCount; ++allocCountIdx)
        {
            size_t       poolSize = 1024 * 1024;    // 1 MB
            unsigned int pageSize = 4 * 1024;       // 4 KB
            SimpleMemoryPool* pMemPool = SimpleMemoryPool::Create(poolSize, pageSize);

            UINT numBytes = AllocSizesInBytes[sizeIdx];
            UINT allocCount = AllocCount[allocCountIdx];

            perfData.push();

            for (UINT count = 0; count < allocCount; ++count)
            {
                void* pMem = pMemPool->Alloc(numBytes, 0);
                pMemPool->Free(pMem);
            }

            DOUBLE elapsedTime = perfData.pop() * 1000.0f;

            ///@TODO Log this to a file
            std::cout << "Alloc Size: " << std::setw(10) << numBytes << "  "
                      << "Alloc Count: " << std::setw(8) << allocCount << "  "
                      << "Elapsed Time (ms): " << std::setw(15)  << elapsedTime
                      << std::endl;

            pMemPool->Destroy();
            pMemPool = NULL;
        }
    }
}

// How long to allocate all memory X times at 4KB per allocation
void MemoryExhaustTest()
{
    std::cout << "Memory Exhaust Test" << std::endl;
}

// Allocate 8KB memory X times in worst case
void WorstCaseTest()
{
    std::cout << "Worst Case Test" << std::endl;
}

int main()
{
    int* pInt = static_cast<int*>(IvyAlloc(int(4)));
    IvyFree(pInt);

    SimpleClass* pS1 = new SimpleClass();
    pS1->PrintName();
    delete pS1;

    SimpleClass* pS3 = IvyNew(SimpleClass);
    pS3->PrintName();
    IvyDelete(pS3);

    int* pIntArray = IvyArrayNew(int, 40);
    IvyArrayDelete(pIntArray);

    SimpleClass* pSCArray = IvyArrayNew(SimpleClass, 32);
    IvyArrayDelete(pSCArray);

    VariedSizeAndCountTest();
    MemoryExhaustTest();
    WorstCaseTest();

    return 0;
}

