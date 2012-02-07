///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2011, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "SimpleMemoryPool.h"

#include <cstdlib>
#include <cstring>

const bool gStdMallocAndFree = false;

static const UINT FullyAllocatedTracker = 0xFFFFFFFF;

struct PageHeader
{
    size_t size;
    unsigned int alignment;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
/// SimpleMemoryPool::Create
///////////////////////////////////////////////////////////////////////////////////////////////////
SimpleMemoryPool* SimpleMemoryPool::Create(
    size_t poolSizeInBytes,
    unsigned int pageSize)
{
    SimpleMemoryPool* pMemPool = new SimpleMemoryPool();

    if (pMemPool->Init(poolSizeInBytes, pageSize) == false)
    {
        delete pMemPool;
        pMemPool = NULL;
    }

    return pMemPool;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// SimpleMemoryPool::Destroy
///////////////////////////////////////////////////////////////////////////////////////////////////
void SimpleMemoryPool::Destroy()
{
    Deinit();

    delete this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// SimpleMemoryPool::SimpleMemoryPool
///////////////////////////////////////////////////////////////////////////////////////////////////
SimpleMemoryPool::SimpleMemoryPool()
    :
    m_poolSizeInBytes(0),
    m_pageSizeInBytes(0),
    m_pPoolBaseAddr(NULL),
    m_bytesPerTracker(4),
    m_pagesPerTracker(32),
    m_numTrackers(0),
    m_trackerMemSize(0),
    m_pTrackerMem(NULL)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// SimpleMemoryPool::~SimpleMemoryPool
///////////////////////////////////////////////////////////////////////////////////////////////////
SimpleMemoryPool::~SimpleMemoryPool()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// SimpleMemoryPool::Init
///////////////////////////////////////////////////////////////////////////////////////////////////
bool SimpleMemoryPool::Init(
    size_t poolSizeInBytes,
    unsigned int pageSize)
{
    bool initOk = true;

    if ((m_poolSizeInBytes % pageSize) != 0)
    {
        initOk = false;
    }

    // Allocate the pool
    if (initOk == true)
    {
        m_pageSizeInBytes = pageSize;
        m_numPages = poolSizeInBytes / pageSize;

        // Requested pool size will be reduced if it isn't a multiple of page size
        m_poolSizeInBytes = m_numPages * pageSize;

        m_pPoolBaseAddr = malloc(poolSizeInBytes);
        initOk = (m_pPoolBaseAddr != NULL);
    }

    // Setup page trackers
    if (initOk == true)
    {
        m_numTrackers = m_numPages / m_pagesPerTracker;
        m_numTrackers += ((m_numPages % m_pagesPerTracker) != 0) ? 1 : 0;
        m_trackerMemSize = m_numTrackers * m_bytesPerTracker;

        // Use first set of pages from pool to store the tracker mem
        m_pTrackerMem = static_cast<UINT*>(m_pPoolBaseAddr);

        // Clear tracker memory
        memset(m_pTrackerMem, 0, m_trackerMemSize);

        // Mark first page used for tracker memory used
        m_pTrackerMem[0] |= (1 << 31);
    }

    return initOk;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// SimpleMemoryPool::Deinit
///////////////////////////////////////////////////////////////////////////////////////////////////
void SimpleMemoryPool::Deinit()
{
    if (m_pPoolBaseAddr)
    {
        free(m_pPoolBaseAddr);
        m_pPoolBaseAddr = NULL;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// SimpleMemoryPool::Alloc
///////////////////////////////////////////////////////////////////////////////////////////////////
void* SimpleMemoryPool::Alloc(
    size_t numBytes,
    unsigned int alignment)
{
    void* pMemory = NULL;

    if (gStdMallocAndFree)
    {
        pMemory = StdAlloc(numBytes, alignment);
    }
    else
    {
        pMemory = PoolAlloc(numBytes, alignment);
    }

    return pMemory;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// SimpleMemoryPool::StdAlloc
///////////////////////////////////////////////////////////////////////////////////////////////////
void* SimpleMemoryPool::StdAlloc(
    size_t numBytes,
    unsigned int alignment)
{
    return malloc(numBytes);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// SimpleMemoryPool::PoolAlloc
///////////////////////////////////////////////////////////////////////////////////////////////////
void* SimpleMemoryPool::PoolAlloc(
    size_t numBytes,
    unsigned int alignment)
{
    size_t allocSize = numBytes;
    unsigned int headerSize = sizeof(PageHeader);
    unsigned int headerPadding = 0;

    if (alignment > 0)
    {
        // 1. add padding to compensate for alignment
        allocSize += (allocSize % alignment) ?
            (alignment - (allocSize % alignment)) : 0;

        // 2. align the header so the beginning address of the memory will be aligned
        headerPadding = (sizeof(PageHeader) % alignment != 0) ?
            (alignment - (sizeof(PageHeader) % alignment)) : 0;

        headerSize += headerPadding;
    }

    // 3. add the size of the header to the allocation
    allocSize += headerSize;

    // test
    PageHeader header;
    header.size = numBytes;
    header.alignment = alignment;

    char* pTestMem = (char*) malloc(allocSize);
    memcpy(&pTestMem[headerPadding], &header, sizeof(PageHeader));

    return &pTestMem[headerSize];

    // 4. check the request to make sure it will fit in the memory being tracked
    // 5. find enough contiguous pages of memory to satisfy the allocation
    //    a. find a starting point of available pages within a tracking unit
    //    b. find whole tracking units where all pages are available and required in fulfilling the request
    //    c. find the remaining pages needed to fulfill the request

    //void* pMemory = NULL;

    //return pMemory;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// SimpleMemoryPool::Free
///////////////////////////////////////////////////////////////////////////////////////////////////
void SimpleMemoryPool::Free(
    void* pMemory)
{
    if (gStdMallocAndFree)
    {
        StdFree(pMemory);
    }
    else
    {
        PoolFree(pMemory);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// SimpleMemoryPool::StdFree
///////////////////////////////////////////////////////////////////////////////////////////////////
void SimpleMemoryPool::StdFree(
    void* pMemory)
{
    free(pMemory);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// SimpleMemoryPool::PoolFree
///////////////////////////////////////////////////////////////////////////////////////////////////
void SimpleMemoryPool::PoolFree(
    void* pMemory)
{
    if (pMemory != NULL)
    {
        // 1. Retrieve the allocation header
        unsigned int alignment = (((PageHeader*)pMemory) - 1)->alignment;
        unsigned int size = (((PageHeader*)pMemory) - 1)->size;

        unsigned int headerPadding = 0;
        
        if (alignment > 0)
        {
            headerPadding = (sizeof(PageHeader) % alignment != 0) ?
            (alignment - (sizeof(PageHeader) % alignment)) : 0;
        }

        unsigned int headerSize = headerPadding + sizeof(PageHeader);

        // test
        free(((char*)pMemory) - headerSize);

        // 2. Determine the pages used to track the memory

        // 3. Release the pages back into the memory heap
    }
}














