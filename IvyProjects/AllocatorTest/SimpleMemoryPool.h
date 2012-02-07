///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2011, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef _SIMPLEMEMORYPOOL_H_
#define _SIMPLEMEMORYPOOL_H_

#include "IvyTypes.h"

class SimpleMemoryPool
{
public:
    static SimpleMemoryPool* Create(size_t poolSizeInBytes, unsigned int pageSize);
    void Destroy();

    void* Alloc(size_t numBytes, unsigned int alignment);
    void  Free(void* pMemory);

private:
    SimpleMemoryPool();
    virtual ~SimpleMemoryPool();

    bool Init(size_t poolSizeInBytes, unsigned int pageSizeInBytes);
    void Deinit();

    void* StdAlloc(size_t numBytes, unsigned int alignment);
    void StdFree(void* pMemory);

    void* PoolAlloc(size_t numBytes, unsigned int alignment);
    void PoolFree(void* pMemory);

    size_t m_poolSizeInBytes;
    unsigned int m_pageSizeInBytes;
    unsigned int m_numPages;

    void* m_pPoolBaseAddr;      ///< Base address of the memory pool

    // Tracker Info
    const UINT m_bytesPerTracker;
    const UINT m_pagesPerTracker;

    UINT m_numTrackers;
    UINT m_trackerMemSize;
    UINT* m_pTrackerMem;        ///< Tracker memory

};

#endif // _SIMPLEMEMORYPOOL_H_
