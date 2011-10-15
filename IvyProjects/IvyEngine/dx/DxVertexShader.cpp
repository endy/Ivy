///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2010-2011, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "DxVertexShader.h"
#include "DxUtils.h"

#include <D3DX11.h>

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxVertexShader::DxVertexShader
///////////////////////////////////////////////////////////////////////////////////////////////////
DxVertexShader::DxVertexShader(
    ID3D11Device* pDevice, 
    const CHAR* pShaderFilename, 
    const CHAR* pShaderName,
    const D3D11_INPUT_ELEMENT_DESC* pInputLayoutDesc,
    UINT numVertexDescElements)
    :
DxShader(pDevice, pShaderFilename, pShaderName),
    m_numVertexDescElements(numVertexDescElements),
    m_pInputLayoutDesc(pInputLayoutDesc),
    m_pInputLayout(NULL),
    m_pVertexShader(NULL)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxVertexShader::~DxVertexShader
///////////////////////////////////////////////////////////////////////////////////////////////////
DxVertexShader::~DxVertexShader()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxVertexShader::CreateFromFile
///////////////////////////////////////////////////////////////////////////////////////////////////
DxVertexShader* DxVertexShader::CreateFromFile(
    ID3D11Device* pDevice,
    const CHAR* pShaderName,
    const CHAR* pShaderFilename,
    const D3D11_INPUT_ELEMENT_DESC* pInputLayoutDesc,
    UINT numElements)
{
    BOOL statusOK = TRUE;

    DxVertexShader* pVSShader = new DxVertexShader(
        pDevice,
        pShaderName,
        pShaderFilename,
        pInputLayoutDesc,
        numElements);

    ID3D10Blob* pErrorMsgs = NULL;
    ID3D10Blob* pVSByteCode = NULL;
    char errorMsg[1024] = {0};

    // Compile the shader
    statusOK = DxOK( D3DX11CompileFromFile(
        pVSShader->GetShaderFilename(),
        NULL,
        NULL,
        pVSShader->GetShaderName(),
        "vs_4_0",
        0,
        0,
        NULL,
        &pVSByteCode,
        &pErrorMsgs,
        NULL) );

    if (pErrorMsgs != NULL)
    {
        //AssertAlways();
        memcpy(errorMsg, pErrorMsgs->GetBufferPointer(), pErrorMsgs->GetBufferSize());
        Log("VERTEX SHADER: ", errorMsg);
        MessageBox(NULL, errorMsg, "Vertex Shader", MB_ICONERROR);
        memset(errorMsg, 0, sizeof(errorMsg));
    }

    // Create the shader
    if (statusOK == TRUE)
    {
        statusOK = DxOK( pDevice->CreateVertexShader(
            pVSByteCode->GetBufferPointer(), 
            pVSByteCode->GetBufferSize(), 
            NULL,
            &pVSShader->m_pVertexShader));
    }

    // Create the input layout
    if (statusOK == TRUE)
    {
        statusOK = DxOK( pDevice->CreateInputLayout(
            pVSShader->m_pInputLayoutDesc,
            pVSShader->m_numVertexDescElements,
            pVSByteCode->GetBufferPointer(), 
            pVSByteCode->GetBufferSize(), 
            &pVSShader->m_pInputLayout) );
    }

    if ((statusOK == FALSE) && (pVSShader != NULL))
    {
        pVSShader->Destroy();
        pVSShader = NULL;
    }


    return pVSShader;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxVertexShader::CreateFromSource
///////////////////////////////////////////////////////////////////////////////////////////////////
DxVertexShader* DxVertexShader::CreateFromSource(
    ID3D11Device* pDevice,
    const CHAR* pShaderName,
    const CHAR* pShaderSource,
    const D3D11_INPUT_ELEMENT_DESC* pInputLayoutDesc,
    UINT numVertexDescElements)
{
    ID3D10Blob* pErrorMsgs = NULL;
    ID3D10Blob* pVSByteCode = NULL;
    char errorMsg[1024] = {0};

    DxVertexShader* pVSShader = new DxVertexShader(
        pDevice,
        pShaderName,
        "",
        pInputLayoutDesc,
        numVertexDescElements);

    // Compile the shader
    BOOL statusOK = DxOK( D3DX11CompileFromMemory(
        pShaderSource,
        strlen(pShaderSource),
        NULL,
        NULL,
        NULL,
        "IvyPosTex",
        "vs_4_0",
        0,
        0,
        NULL,
        &pVSByteCode,
        &pErrorMsgs,
        NULL) );

    if (pErrorMsgs != NULL)
    {
        //AssertAlways();
        memcpy(errorMsg, pErrorMsgs->GetBufferPointer(), pErrorMsgs->GetBufferSize());
        Log("VERTEX SHADER: ", errorMsg);
        MessageBox(NULL, errorMsg, "Vertex Shader", MB_ICONERROR);
        memset(errorMsg, 0, sizeof(errorMsg));
    }

    // Create the shader
    if (statusOK == TRUE)
    {
        statusOK = DxOK( pDevice->CreateVertexShader(
            pVSByteCode->GetBufferPointer(), 
            pVSByteCode->GetBufferSize(), 
            NULL,
            &pVSShader->m_pVertexShader));
    }

    // Create the input layout
    if (statusOK == TRUE)
    {
        statusOK = DxOK( pDevice->CreateInputLayout(
            pVSShader->m_pInputLayoutDesc,
            pVSShader->m_numVertexDescElements,
            pVSByteCode->GetBufferPointer(), 
            pVSByteCode->GetBufferSize(), 
            &pVSShader->m_pInputLayout) );
    }

    return pVSShader;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxVertexShader::Destroy
///////////////////////////////////////////////////////////////////////////////////////////////////
void DxVertexShader::Destroy()
{
    if (m_pVertexShader!= NULL)
    {
        m_pVertexShader->Release();
    }

    if (m_pInputLayout != NULL)
    {
        m_pInputLayout->Release();
    }

    delete this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxVertexShader::Bind
///////////////////////////////////////////////////////////////////////////////////////////////////
void DxVertexShader::Bind(
    ID3D11DeviceContext* pContext)
{
    pContext->IASetInputLayout(m_pInputLayout);
    pContext->VSSetShader(m_pVertexShader, NULL, 0);
}
