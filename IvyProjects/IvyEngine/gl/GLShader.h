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

#include "IvyShader.h"

#include <string>

///////////////////////////////////////////////////////////////////////////////////////////////////
/// GLShader Class
///////////////////////////////////////////////////////////////////////////////////////////////////
class GLShader :
    public IvyShader
{
public:
    static GLShader* CreateFromSource(IvyShaderType shaderType, const CHAR* pShaderName, const CHAR* pShaderSource);
    static GLShader* CreateFromFile(IvyShaderType shaderType, const CHAR* pShaderName, const CHAR* pFilename);

    virtual void Destroy();

    int ShaderId() const { return m_glShaderId; }

    void Reload();

protected:
    GLShader(IvyShaderType shaderType, const CHAR* pShaderName, const CHAR* pShaderFilename);
    virtual ~GLShader(); 

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
