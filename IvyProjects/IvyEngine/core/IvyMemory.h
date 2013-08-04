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
#include <cstdlib>
/**
/// @note

1.  All allocations should be done thru use of macros.
2.  Objects are created with new/delete and POD with malloc/free.
3.  No objects created with Ivy allocators can take arguments in the constructors*
        a. General rule for engine objects, all should have simple constructors that are called
           by static create methods that call initializers after construction.
4.  Tracking
        a. All allocations with IvyAlloc can be tracked but only IvyObjects can be tracked with IvyNew
        b. All tracked objects should inherit from IvyObject



This is somewhat counter to the 'When using C++, use new/delete'.  I initially thought of this as a workaround for
how new/delete have some funky asymmetry with how they are called.  Now I prefer the macros because the coder
should always take care of how they are allocating/freeing objects.  

Overriding global new/delete is simple, but calling new/delete are different.  calling the Overloading delete requires the user
to call the custom delete as a function.  This could be detrimental where objects are created with
custom new but freed with standard delete.  Array new and array delete also appear to clutter things.

To fix this, I define two sets of macros IvyNew/IvyDelete and IvyArrayNew/IvyArrayDelete.




1.  Nothing is overloaded.  Cleaner in some respects, this 'behind the back' stuff doesn't have
enough PROs to out weigh the CONSs.  Junior coders are less likely to appreciate not having to
understand the details (particularly because they are most likely to make the mistakes), 
and senior coders may be upset at the lack of options to override (although should there be in a good design?).

Libraries may do their own overloading which could cause conflict.  Why one uses a library
that does this without any way of disabling/overriding the 'feature' is also suspect.



**/

namespace Ivy
{

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

}

#define IVY_TRACK_MEMORY 1

#if IVY_TRACK_MEMORY

#define IVY_RAW_ALLOC(size) IvyTrackedAlloc((size), (__FILE__), (__LINE__))
#define IVY_TYPE_ALLOC(datatype, count) reinterpret_cast<datatype*>(IvyTrackedAlloc(sizeof(datatype)*count, (__FILE__), (__LINE__)))
#define IVY_FREE(ptr) IvyTrackedFree((ptr), (__FILE__), (__LINE__))

#define IVY_NEW(datatype) new datatype; IvyPrint("IvyNew", __FILE__, __LINE__)
#define IVY_ARRAY_NEW(datatype, count) (new datatype[count]); IvyPrint("IvyArrayNew", __FILE__, __LINE__)
#define IVY_DELETE(ptr) IvyPrint("IvyDelete", __FILE__, __LINE__); delete (ptr)
#define IVY_ARRAY_DELETE(ptr) IvyPrint("IvyArrayDelete", __FILE__, __LINE__); delete [](ptr)

#else
#define IVY_RAW_ALLOC(size) malloc(size)
#define IVY_TYPE_ALLOC(datatype, count) reinterpret_cast<datatype*>(malloc(count*sizeof(datatype)))
#define IVY_FREE(ptr) free(ptr)
#define IVY_NEW(datatype) new (datatype)
#define IVY_ARRAY_NEW(datatype, count) (new datatype[count])
#define IVY_DELETE(ptr) delete (ptr)
#define IVY_ARRAY_DELETE(ptr) delete [] (ptr)
#endif


#endif // _IVYMEMORY_H_

