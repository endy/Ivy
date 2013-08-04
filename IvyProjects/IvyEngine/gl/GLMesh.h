///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2012, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _GLMESH_H_
#define _GLMESH_H_

#include "IvyMesh.h"


// Forward decls
class GLProgram;

///////////////////////////////////////////////////////////////////////////////////////////////////
/// GLMesh Class
///////////////////////////////////////////////////////////////////////////////////////////////////
class GLMesh :
    public Ivy::IvyMesh
{
public:
    static GLMesh* Create(Ivy::IvyMeshCreateInfo* pMeshCreateInfo);
    virtual void Destroy();

    void Bind(GLProgram* pProgram);
    void Draw();

private:
    GLMesh();
    virtual ~GLMesh();

    UINT m_glVertexBufferId;    ///< GL Vertex Buffer Id
    UINT m_numVertices;         ///< Num vertices
    UINT m_glIndexBufferId;     ///< GL Index Buffer Id
    UINT m_numIndices;          ///< Num indicies
};


#endif // _GLMESH_H_

