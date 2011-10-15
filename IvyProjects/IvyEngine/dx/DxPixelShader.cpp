///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2010-2011, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "DxPixelShader.h"
#include "DxUtils.h"

#include <D3DX11.h>

using std::string;

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxPixelShader::DxPixelShader
///////////////////////////////////////////////////////////////////////////////////////////////////
DxPixelShader::DxPixelShader(
    ID3D11Device* pDevice,
    const CHAR* pShaderName,
    const CHAR* pShaderFilename)
    :
    DxShader(pDevice, pShaderName, pShaderFilename),
    m_pPixelShader(NULL)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxPixelShader::~DxPixelShader
///////////////////////////////////////////////////////////////////////////////////////////////////
DxPixelShader::~DxPixelShader()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxPixelShader::CreateFromFile
///////////////////////////////////////////////////////////////////////////////////////////////////
DxPixelShader* DxPixelShader::CreateFromFile(
    ID3D11Device* pDevice,
    const CHAR* shaderName,
    const CHAR* shaderFilename)
{
    BOOL statusOK = TRUE;

    DxPixelShader* pPSShader = new DxPixelShader(pDevice, shaderName, shaderFilename);

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
            &pPSShader->m_pPixelShader) );
    }

    if ((statusOK == FALSE) && (pPSShader != NULL))
    {
        pPSShader->Destroy();
        pPSShader = NULL;
    }

    return pPSShader;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxPixelShader::CreateFromSource
///////////////////////////////////////////////////////////////////////////////////////////////////
DxPixelShader* DxPixelShader::CreateFromSource(
    ID3D11Device* pDevice,
    const CHAR* pShaderName,
    const CHAR* pShaderSource)
{
    BOOL statusOK = TRUE;

    DxPixelShader* pPSShader = new DxPixelShader(pDevice, pShaderName, "");

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
            &pPSShader->m_pPixelShader) );
    }

    if ((statusOK == FALSE) && (pPSShader != NULL))
    {
        pPSShader->Destroy();
        pPSShader = NULL;
    }

    return pPSShader;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxPixelShader::Destroy
///////////////////////////////////////////////////////////////////////////////////////////////////
void DxPixelShader::Destroy()
{
    if (m_pPixelShader != NULL)
    {
        m_pPixelShader->Release();
        m_pPixelShader = NULL;
    }

    delete this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxPixelShader::SetShader
///////////////////////////////////////////////////////////////////////////////////////////////////
VOID DxPixelShader::Bind(
    ID3D11DeviceContext* pContext)
{
    pContext->PSSetShader(m_pPixelShader, NULL, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// Reload
///////////////////////////////////////////////////////////////////////////////////////////////////
VOID Reload(
    ID3D11Device* pDevice,
    DxPixelShader** ppPixelShader)
{
    std::string shaderFile = (*ppPixelShader)->GetShaderFilename();
    std::string shaderName = (*ppPixelShader)->GetShaderName();

    if (shaderFile.size() > 0)
    {
        DxPixelShader* pReloadedShader = DxPixelShader::CreateFromFile(pDevice, shaderFile.c_str(), shaderName.c_str());

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