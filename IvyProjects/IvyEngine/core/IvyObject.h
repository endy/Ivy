///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2010-2011, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _IVYOBJECT_H_
#define _IVYOBJECT_H_

#include "IvyTypes.h"
#include "IvyMemory.h"

namespace Ivy
{

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    /// IvyObject Class
    ///////////////////////////////////////////////////////////////////////////////////////////////////
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

}

#endif // _IVYOBJECT_H_

