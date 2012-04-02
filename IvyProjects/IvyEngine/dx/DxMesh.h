///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2010-2011, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef _DXMESH_H_
#define _DXMESH_H_

#include "IvyObject.h"

#include <D3D11.h>
#include <DXGI.h>

struct DxMeshCreateInfo
{
    const VOID* pVertexArray;
    UINT vertexCount;
    UINT vertexElementSize;
    const VOID* pIndexArray;
    UINT indexCount;
    DXGI_FORMAT indexFormat;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxMesh Class
///////////////////////////////////////////////////////////////////////////////////////////////////
class DxMesh :
    public IvyObject
{
public:
    static DxMesh* Create(ID3D11Device* pDevice, DxMeshCreateInfo* pCreateInfo);
    virtual void Destroy();

    void Bind(ID3D11DeviceContext* pContext);
    void Draw(ID3D11DeviceContext* pContext);

protected:
    DxMesh(DxMeshCreateInfo* pCreateInfo);
    virtual ~DxMesh();

    ID3D11Buffer* m_pVertexBuffer;
    UINT m_vertexCount;
    UINT m_vertexElementSize;

    ID3D11Buffer* m_pIndexBuffer;
    UINT m_indexCount;
    DXGI_FORMAT m_indexFormat;

private:
    DxMesh(const DxMesh& copy);     // Disallow copy constructor
};

#endif // _DXMESH_H_
