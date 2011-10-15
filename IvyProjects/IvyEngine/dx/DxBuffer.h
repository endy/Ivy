///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2010-2011, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DXBUFFER_H_
#define _DXBUFFER_H_

#include "IvyObject.h"

// Forward declarations
struct ID3D11Device;
struct ID3D11Buffer;
struct ID3D11DeviceContext;

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxBufferCreateFlags
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef union _DxBufferCreateFlags
{
    struct
    {
        UINT cpuWriteable    :  1;
        UINT reserved        : 31;
    };
    UINT value;
} DxBufferCreateFlags;

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxBufferCreateInfo Struct
///////////////////////////////////////////////////////////////////////////////////////////////////
struct DxBufferCreateInfo
{
    DxBufferCreateFlags flags;
    UINT elemSizeBytes;
    VOID* pInitialData;
    UINT initialDataPitch;
    UINT initialDataSlicePitch;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxBuffer Class
///////////////////////////////////////////////////////////////////////////////////////////////////
class DxBuffer :
    IvyObject
{
public:
    static DxBuffer* Create(ID3D11Device* pDevice, DxBufferCreateInfo* pCreateInfo);

    VOID Destroy();

    VOID* Map(ID3D11DeviceContext* pContext);
    VOID Unmap(ID3D11DeviceContext* pContext);

    VOID BindVS(ID3D11DeviceContext* pContext, UINT slot);
    VOID BindPS(ID3D11DeviceContext* pContext, UINT slot);

private:
    DxBuffer();
    virtual ~DxBuffer();

    BOOL Init(ID3D11Device* pDevice, DxBufferCreateInfo* pCreateInfo);

    DxBuffer(const DxBuffer&);              // Disallow copy constructor
    DxBuffer& operator=(const DxBuffer&);   // Disallow assignment operator


    ID3D11Buffer* m_pDxBuffer;
};

#endif // _DXBUFFER_H_
