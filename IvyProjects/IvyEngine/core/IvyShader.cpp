///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2010-2012, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "IvyShader.h"

using namespace Ivy;

///////////////////////////////////////////////////////////////////////////////////////////////////
/// IvyShader::IvyShader
///////////////////////////////////////////////////////////////////////////////////////////////////
IvyShader::IvyShader(
    IvyShaderType shaderType,
    const CHAR* pShaderName,
    const WCHAR* pShaderFilename)
    :
    m_type(shaderType),
    m_shaderName(pShaderName),
    m_shaderFilename(pShaderFilename)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// IvyShader::~IvyShader
///////////////////////////////////////////////////////////////////////////////////////////////////
IvyShader::~IvyShader()
{

}
