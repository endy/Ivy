///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2010-2011, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DXVERTEXSHADER_H_
#define _DXVERTEXSHADER_H_

#include "DxShader.h"

class DxVertexShader :
    public DxShader
{
public:  
    ///@TODO VertexShader::Create called by a shader manager?
    static DxVertexShader* CreateFromFile(
        ID3D11Device* pDevice,
        const CHAR* pShaderName,
        const CHAR* pShaderFilename,
        const D3D11_INPUT_ELEMENT_DESC* pInputLayoutDesc,
        UINT numVertexDescElements);

    static DxVertexShader* CreateFromSource(
        ID3D11Device* pDevice,
        const CHAR* pShaderName,
        const CHAR* pShaderSource,
        const D3D11_INPUT_ELEMENT_DESC* pInputLayoutDesc,
        UINT numVertexDescElements);

    virtual void Destroy();

    virtual void Bind(ID3D11DeviceContext* pContext);

protected:
    DxVertexShader(ID3D11Device* pDevice, const CHAR* pShaderName, const CHAR* pShaderFilename,
        const D3D11_INPUT_ELEMENT_DESC* pInputLayoutDesc, UINT numVertexDescElements);
    virtual ~DxVertexShader();

private:
    DxVertexShader(const DxVertexShader& copy);   // Disallow copy constructor

    const D3D11_INPUT_ELEMENT_DESC* m_pInputLayoutDesc;
    UINT m_numVertexDescElements;
    ID3D11VertexShader* m_pVertexShader;
    ID3D11InputLayout* m_pInputLayout;

};

#endif // _DXVERTEXSHADER_H_