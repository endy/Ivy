///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2010-2012, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _GLSHADER_H_
#define _GLSHADER_H_

/// @todo Add Reload shader utility function

/*
use cases for reloading:

typically edit 1 shader, not multiple
need to reload *all* shaders?
reloading shaders is a debug/development feature...not really a performance concern

easiest is to reload a program, then reload the specific shader (thus creating a new shader)
*/

#include "IvyObject.h"

#include <string>

enum IvyShaderType
{
    IvyVertexShader     = 0,
    IvyFragmentShader
};

class GLShader :
    public IvyObject
{
public:
    static GLShader* CreateFromSource(IvyShaderType shaderType, const CHAR* pShaderName, const CHAR* pShaderSource);
    static GLShader* CreateFromFile(IvyShaderType shaderType, const CHAR* pShaderName, const CHAR* pFilename);

    virtual void Destroy();

    IvyShaderType Type() const { return m_type; }
    int ShaderId() const { return m_glShaderId; }

    const CHAR* GetShaderName() { return m_shaderName.c_str(); }
    const CHAR* GetShaderFilename() { return m_shaderFile.c_str(); }

    void Reload();

protected:
    GLShader(IvyShaderType shaderType, const CHAR* pShaderName, const CHAR* pShaderFilename);
    virtual ~GLShader(); 

    /// @todo move these values up into IvyShader class
    IvyShaderType m_type;           ///< Shader type
    std::string m_shaderName;
    std::string m_shaderFile;

    // GL-specific shader stuff
    int m_glShaderId;

};

class GLProgram :
    public IvyObject
{
public:
    static GLProgram* Create();

    void Destroy();

    int ProgramId() const { return m_glProgramId; }

    void AttachShader(GLShader* pShader);

    bool Link();    ///< Link the VS and the PS into a complete program
    void Bind();    ///< Bind program to the pipeline

    void Reload();

    // VSSetConstant
    // PSSetConstant
    // VSAttachTexture
    // PSAttachTexture

protected:
    GLProgram();
    virtual ~GLProgram();

    bool m_isLinked;

    int m_glProgramId;

    GLShader* m_pVSShader;
    GLShader* m_pFSShader;
};




#endif // _GLSHADER_H_
