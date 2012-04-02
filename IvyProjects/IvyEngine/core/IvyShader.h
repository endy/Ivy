///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2012, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _IVYSHADER_H_
#define _IVYSHADER_H_

#include "IvyObject.h"

enum IvyShaderType
{
    IvyVertexShader,
    IvyFragmentShader,

    IvyMaxShaderTypes      // Enum sentinel, invalid shader type
};

///@todo Make an IvyShaderCreateInfo structure, consumed by API shader classes

///////////////////////////////////////////////////////////////////////////////////////////////////
/// IvyShader Class
///////////////////////////////////////////////////////////////////////////////////////////////////
class IvyShader :
    public IvyObject
{
public:
    virtual void Destroy() = 0;

    IvyShaderType Type() const { return m_type; }

    const CHAR* GetShaderName() { return m_shaderName.c_str(); }
    const CHAR* GetShaderFilename() { return m_shaderFilename.c_str(); }

protected:
    IvyShader(IvyShaderType shaderType, const CHAR* pShaderName, const CHAR* pShaderFilename);
    virtual ~IvyShader();

private:
    IvyShaderType m_type;
    std::string m_shaderName;
    std::string m_shaderFilename;
};


#endif // _IVYSHADER_H_

