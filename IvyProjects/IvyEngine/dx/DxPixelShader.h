///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2010-2011, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DXPIXELSHADER_H_
#define _DXPIXELSHADER_H_

#include "DxShader.h"

class DxPixelShader :
    public DxShader
{
public:
    /// @todo DxPixelShader::Create called by a shader manager?
    static DxPixelShader* CreateFromFile(
        ID3D11Device* pDevice,
        const CHAR* pShaderName,
        const CHAR* pShaderFilename);

    static DxPixelShader* CreateFromSource(
        ID3D11Device* pDevice,
        const CHAR* pShaderName,
        const CHAR* pShaderSource);

    virtual void Destroy();

    virtual void Bind(ID3D11DeviceContext* pContext);

protected:
    DxPixelShader(ID3D11Device* pDevice, const CHAR* pShaderName, const CHAR* pShaderFilename);
    virtual ~DxPixelShader();

private:
    DxPixelShader(const DxPixelShader& copy);     // Disallow copy constructor

    ID3D11PixelShader* m_pPixelShader;
};

VOID Reload(ID3D11Device* pDevice, DxPixelShader** ppPixelShader);

#endif // _DXPIXELSHADER_H_

