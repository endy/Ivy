///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2012, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _IVYMESH_H_
#define _IVYMESH_H_

#include "IvyObject.h"

namespace Ivy
{

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    /// IvyMeshCreateInfo Struct
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    struct IvyMeshCreateInfo
    {
        void* pVertexData;          ///< Pointer to vertex data
        UINT vertexSizeInBytes;     ///< Single vertex size
        UINT numVertices;           ///< Num vertices in the vertex data
        void* pIndexData;           ///< Pointer to index data (can be NULL)
        UINT numIndicies;           ///< Num indicies in the index data
    };

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    /// IvyMesh Class
    ///
    /// @note
    ///     Supports:
    ///         1. TRIANGLE primitive type only
    ///         2. Index buffers
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    class IvyMesh :
        public IvyObject
    {
    public:
        virtual void Destroy() = 0;

    protected:
        IvyMesh();
        virtual ~IvyMesh();

    private:
        IvyMesh(const IvyMesh& copy);   // Disallow copy constructor

    };

}

#endif // _IVYMESH_H_

