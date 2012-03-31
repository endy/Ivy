///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2010-2012, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "DxShader.h"
#include "DxUtils.h"
#include <D3DX11.h>

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxShader::DxShader
///////////////////////////////////////////////////////////////////////////////////////////////////
DxShader::DxShader(
    IvyShaderType type,
    const CHAR* pShaderName,
    const CHAR* pShaderFilename,
    ID3D11Device* pDevice)
    :
    IvyShader(type, pShaderName, pShaderFilename),
    m_pDevice(pDevice)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxShader::~DxShader
///////////////////////////////////////////////////////////////////////////////////////////////////
DxShader::~DxShader()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxShader::CreateFromFile
///////////////////////////////////////////////////////////////////////////////////////////////////
DxShader* DxShader::CreateFromFile(
    ID3D11Device* pDevice,
    const CHAR* pShaderName,
    const CHAR* pShaderFilename,
    const D3D11_INPUT_ELEMENT_DESC* pInputLayoutDesc,
    UINT numElements)
{
    BOOL statusOK = TRUE;

    DxShader* pVSShader = new DxShader(
        IvyVertexShader,
        pShaderName,
        pShaderFilename,
        pDevice);

    ///@todo Refactor this into an initialization step, constructor, or common IvyShaderCreateInfo
    pVSShader->m_pInputLayoutDesc = pInputLayoutDesc;
    pVSShader->m_numVertexDescElements = numElements;

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
            &pVSShader->m_shaderInterface.pVertexShader));
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
/// DxShader::CreateFromSource
///////////////////////////////////////////////////////////////////////////////////////////////////
DxShader* DxShader::CreateFromSource(
    ID3D11Device* pDevice,
    const CHAR* pShaderName,
    const CHAR* pShaderSource,
    const D3D11_INPUT_ELEMENT_DESC* pInputLayoutDesc,
    UINT numVertexDescElements)
{
    ID3D10Blob* pErrorMsgs = NULL;
    ID3D10Blob* pVSByteCode = NULL;
    char errorMsg[1024] = {0};

    DxShader* pVSShader = new DxShader(
        IvyVertexShader,
        pShaderName,
        "",
        pDevice);

    ///@todo Refactor this into an initialization step, constructor, or common IvyShaderCreateInfo
    pVSShader->m_pInputLayoutDesc = pInputLayoutDesc;
    pVSShader->m_numVertexDescElements = numVertexDescElements;

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
            &pVSShader->m_shaderInterface.pVertexShader));
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
/// DxShader::CreateFromFile
///////////////////////////////////////////////////////////////////////////////////////////////////
DxShader* DxShader::CreateFromFile(
    ID3D11Device* pDevice,
    const CHAR* shaderName,
    const CHAR* shaderFilename)
{
    BOOL statusOK = TRUE;

    DxShader* pPSShader = new DxShader(IvyFragmentShader, shaderName, shaderFilename, pDevice);

    ID3D10Blob* pErrorMsgs = NULL;
    ID3D10Blob* pPSByteCode = NULL;

    char errorMsg[1024] = {0};     

    statusOK = DxOK( D3DX11CompileFromFile(
        pPSShader->GetShaderFilename(),
        NULL,
        NULL,
        pPSShader->GetShaderName(),
        "ps_4_0",
        0,
        0,
        NULL,
        &pPSByteCode,
        &pErrorMsgs,
        NULL) );

    if (pErrorMsgs != NULL)
    {
        memcpy(errorMsg, pErrorMsgs->GetBufferPointer(), pErrorMsgs->GetBufferSize());
        Log("PIXEL SHADER: ", errorMsg);
        //MessageBox(NULL, errorMsg, "Pixel Shader", MB_ICONERROR);
        memset(errorMsg, 0, sizeof(errorMsg));
    }

    if (statusOK == TRUE)
    {
        statusOK = DxOK( pDevice->CreatePixelShader(
            pPSByteCode->GetBufferPointer(),
            pPSByteCode->GetBufferSize(), 
            NULL, 
            &pPSShader->m_shaderInterface.pPixelShader) );
    }

    if ((statusOK == FALSE) && (pPSShader != NULL))
    {
        pPSShader->Destroy();
        pPSShader = NULL;
    }

    return pPSShader;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxShader::CreateFromSource
///////////////////////////////////////////////////////////////////////////////////////////////////
DxShader* DxShader::CreateFromSource(
    ID3D11Device* pDevice,
    const CHAR* pShaderName,
    const CHAR* pShaderSource)
{
    BOOL statusOK = TRUE;

    DxShader* pPSShader = new DxShader(IvyFragmentShader, pShaderName, "", pDevice);

    ID3D10Blob* pErrorMsgs = NULL;
    ID3D10Blob* pPSByteCode = NULL;

    char errorMsg[1024] = {0};     

    statusOK = DxOK( D3DX11CompileFromMemory(
        pShaderSource,
        strlen(pShaderSource),
        NULL,
        NULL,
        NULL,
        pShaderName,
        "ps_4_0",
        0,
        0,
        NULL,
        &pPSByteCode,
        &pErrorMsgs,
        NULL) );

    if (pErrorMsgs != NULL)
    {
        memcpy(errorMsg, pErrorMsgs->GetBufferPointer(), pErrorMsgs->GetBufferSize());
        Log("PIXEL SHADER: ", errorMsg);
        //MessageBox(NULL, errorMsg, "Pixel Shader", MB_ICONERROR);
        memset(errorMsg, 0, sizeof(errorMsg));
    }

    if (statusOK == TRUE)
    {
        statusOK = DxOK( pDevice->CreatePixelShader(
            pPSByteCode->GetBufferPointer(),
            pPSByteCode->GetBufferSize(), 
            NULL, 
            &pPSShader->m_shaderInterface.pPixelShader) );
    }

    if ((statusOK == FALSE) && (pPSShader != NULL))
    {
        pPSShader->Destroy();
        pPSShader = NULL;
    }

    return pPSShader;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxShader::Destroy
///////////////////////////////////////////////////////////////////////////////////////////////////
void DxShader::Destroy()
{
    if (Type() ==  IvyVertexShader)
    {
        if (m_shaderInterface.pVertexShader!= NULL)
        {
            m_shaderInterface.pVertexShader->Release();
            m_shaderInterface.pVertexShader = NULL;
        }

        if (m_pInputLayout != NULL)
        {
            m_pInputLayout->Release();
            m_pInputLayout = NULL;
        }
    }
    else if (Type() == IvyFragmentShader)
    {
        if (m_shaderInterface.pPixelShader != NULL)
        {
            m_shaderInterface.pPixelShader->Release();
            m_shaderInterface.pPixelShader = NULL;
        }
    }

    delete this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxShader::Bind
///////////////////////////////////////////////////////////////////////////////////////////////////
void DxShader::Bind(
    ID3D11DeviceContext* pContext)
{
    if (Type() == IvyVertexShader)
    {
        pContext->IASetInputLayout(m_pInputLayout);
        pContext->VSSetShader(m_shaderInterface.pVertexShader, NULL, 0);
    }
    else if (Type() == IvyFragmentShader)
    {
        pContext->PSSetShader(m_shaderInterface.pPixelShader, NULL, 0);
    }
}
/*
///////////////////////////////////////////////////////////////////////////////////////////////////
/// Reload
///////////////////////////////////////////////////////////////////////////////////////////////////
VOID Reload(
    ID3D11Device* pDevice,
    DxShader** ppPixelShader)
{
    std::string shaderFile = (*ppPixelShader)->GetShaderFilename();
    std::string shaderName = (*ppPixelShader)->GetShaderName();

    if (shaderFile.size() > 0)
    {
        DxShader* pReloadedShader = DxShader::CreateFromFile(pDevice, shaderFile.c_str(), shaderName.c_str());

        if (pReloadedShader != NULL)
        {
            (*ppPixelShader)->Destroy();
            *ppPixelShader = pReloadedShader;
        }
        else
        {
            // Raise error
        }
    }
}
*/