///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2010-2011, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

///@todo Add Reload shader utility function

#ifndef _DXSHADER_H_
#define _DXSHADER_H_

#include "IvyShader.h"

#include <D3D11.h>
#include <string>

///@todo Remove generic shader decls and shader source to a separate header

static const D3D11_INPUT_ELEMENT_DESC PosVertexDesc[] =
{
    { "SV_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA },
};

static const UINT PosElements = sizeof(PosVertexDesc)/sizeof(PosVertexDesc[0]);

static const D3D11_INPUT_ELEMENT_DESC PosTexVertexDesc[] =
{
    { "SV_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }, 
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

static const UINT PosTexElements = sizeof(PosTexVertexDesc)/sizeof(PosTexVertexDesc[0]);

static const D3D11_INPUT_ELEMENT_DESC PosTexNormVertexDesc[] =
{
    { "SV_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }, 
};

static const UINT PosTexNormElements = sizeof(PosTexNormVertexDesc)/sizeof(PosTexNormVertexDesc[0]);


static const CHAR* IvyVsPosTex = "                              \
struct VS_PT_OUT                                                \
{                                                               \
    float4 position : SV_POSITION;                              \
    float2 tex0     : TEXCOORD;                                 \
};                                                              \
                                                                \
cbuffer MB : register(c0)                                       \
{                                                               \
    row_major matrix world;                                     \
    row_major matrix view;                                      \
    row_major matrix projection;                                \
};                                                              \
                                                                \
                                                                \
VS_PT_OUT IvyPosTex( float4 Pos : SV_POSITION,                  \
    float2 Tex : TEXCOORD )                                     \
{                                                               \
    VS_PT_OUT vsOut;                                            \
                                                                \
    vsOut.position = mul(Pos, world);                           \
    vsOut.position = mul(vsOut.position, view);                 \
    vsOut.position = mul(vsOut.position, projection);           \
                                                                \
    vsOut.tex0 = Tex;                                           \
                                                                \
    return vsOut;                                               \
}";

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxShader Class
///////////////////////////////////////////////////////////////////////////////////////////////////
class DxShader :
    public IvyShader
{
public:
    ///@todo Refactor these to use an info structure?
    static DxShader* CreateFromFile(
        ID3D11Device* pDevice,
        const CHAR* pShaderName,
        const CHAR* pShaderFilename,
        const D3D11_INPUT_ELEMENT_DESC* pInputLayoutDesc,
        UINT numVertexDescElements);

    static DxShader* CreateFromSource(
        ID3D11Device* pDevice,
        const CHAR* pShaderName,
        const CHAR* pShaderSource,
        const D3D11_INPUT_ELEMENT_DESC* pInputLayoutDesc,
        UINT numVertexDescElements);

    static DxShader* CreateFromFile(
        ID3D11Device* pDevice,
        const CHAR* pShaderName,
        const CHAR* pShaderFilename);

    static DxShader* CreateFromSource(
        ID3D11Device* pDevice,
        const CHAR* pShaderName,
        const CHAR* pShaderSource);

    virtual void Destroy();

    virtual void Bind(ID3D11DeviceContext* pContext);

private:
    DxShader(IvyShaderType type, const CHAR* pShaderName, const CHAR* pShaderFilename, ID3D11Device* pDevice);
    virtual ~DxShader(); 

    DxShader(const DxShader& copy);   // Disallow copy constructor

    // General DxShader Data
    ID3D11Device* m_pDevice;

    typedef union _DxShaderInterface
    {
        ID3D11VertexShader* pVertexShader;
        ID3D11PixelShader*  pPixelShader;
    } DxShaderInterface;

    DxShaderInterface m_shaderInterface;

    // Vertex Shader Data
    const D3D11_INPUT_ELEMENT_DESC* m_pInputLayoutDesc;
    UINT m_numVertexDescElements;
    ID3D11InputLayout* m_pInputLayout;
};

// VOID Reload(ID3D11Device* pDevice, DxShader** ppPixelShader);

#endif // _DXSHADER_H_
