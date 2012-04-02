///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2010-2012, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "DxMesh.h"
#include "DxUtils.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxMesh::DxMesh
///////////////////////////////////////////////////////////////////////////////////////////////////
DxMesh::DxMesh(
    DxMeshCreateInfo* pCreateInfo)  ///< DxMesh create info
    :
    m_pVertexBuffer(NULL),
    m_vertexCount(pCreateInfo->vertexCount),
    m_vertexElementSize(pCreateInfo->vertexElementSize),
    m_pIndexBuffer(NULL),
    m_indexCount(pCreateInfo->indexCount),
    m_indexFormat(pCreateInfo->indexFormat)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxMesh::~DxMesh
///////////////////////////////////////////////////////////////////////////////////////////////////
DxMesh::~DxMesh()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxMesh::Create
///////////////////////////////////////////////////////////////////////////////////////////////////
DxMesh* DxMesh::Create(
    ID3D11Device* pDevice,          ///< ID3D11 device
    DxMeshCreateInfo* pCreateInfo)  ///< DxMesh create info
{
    DxMesh* pMesh = new DxMesh(pCreateInfo);

    D3D11_SUBRESOURCE_DATA initData;

    D3D11_BUFFER_DESC vbDesc;
    vbDesc.Usage = D3D11_USAGE_DEFAULT;
    vbDesc.ByteWidth = pCreateInfo->vertexElementSize * pCreateInfo->vertexCount;
    vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbDesc.CPUAccessFlags = 0;
    vbDesc.MiscFlags = 0;

    initData.pSysMem = pCreateInfo->pVertexArray;
    if( DxFAIL( pDevice->CreateBuffer( &vbDesc, &initData, &pMesh->m_pVertexBuffer ) ) )
    {
        IvyAssertAlways();
    }

    if (pCreateInfo->pIndexArray != NULL)
    {
        D3D11_BUFFER_DESC ibDesc;
        ibDesc.Usage = D3D11_USAGE_DEFAULT;
        ibDesc.ByteWidth = DxgiFormatElementSize(pCreateInfo->indexFormat) * pCreateInfo->indexCount;
        ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        ibDesc.CPUAccessFlags = 0;
        ibDesc.MiscFlags = 0;

        initData.pSysMem = pCreateInfo->pIndexArray;
        if( DxFAIL( pDevice->CreateBuffer( &ibDesc, &initData, &pMesh->m_pIndexBuffer ) ) )
        {
            IvyAssertAlways();
        }
    }

    return pMesh;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxMesh::Destroy
///////////////////////////////////////////////////////////////////////////////////////////////////
void DxMesh::Destroy()
{
    if (m_pVertexBuffer != NULL)
    {
        m_pVertexBuffer->Release();
    }
    else
    {
        // Should be unable to create a Mesh w/o a valid Vertex Buffer
        IvyAssertAlways();
    }

    if (m_pIndexBuffer != NULL)
    {
        m_pIndexBuffer->Release();
    }

    delete this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxMesh::Bind
///////////////////////////////////////////////////////////////////////////////////////////////////
void DxMesh::Bind(
    ID3D11DeviceContext* pContext)
{
    UINT offset = 0;
    pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_vertexElementSize, &offset);
    
    if (m_pIndexBuffer != NULL)
    {
        pContext->IASetIndexBuffer(m_pIndexBuffer, m_indexFormat, 0);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxMesh::Draw
///////////////////////////////////////////////////////////////////////////////////////////////////
void DxMesh::Draw(
    ID3D11DeviceContext* pContext)
{
    if (m_pIndexBuffer != NULL)
    {
        pContext->DrawIndexed(m_indexCount, 0, 0);
    }
    else
    {
        pContext->Draw(m_vertexCount, 0);
    }
}

