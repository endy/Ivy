///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2010-2012, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "GLShader.h"
#include "IvyGL.h"

const CHAR* gVS2String = "                       \n\
                         attribute  vec3 in_Position;                    \n\
                         attribute  vec4 in_Color;                       \n\
                         varying    vec4 v_Color;                        \n\
                         varying    vec2 v_TexCoord;                     \n\
                         \n\
                         void main(void)                                 \n\
                         {                                               \n\
                         gl_Position = vec4(in_Position, 1.0);       \n\
                         v_TexCoord = vec2(in_Position.x, in_Position.y); \n\
                         v_Color = in_Color;                         \n\
                         }                                               \n\
                         ";

const CHAR* gFS2String = "                      \n\
                         varying vec2 v_TexCoord; \n\
                         uniform sampler2D s_texture;                    \n\
                         varying  vec4 v_Color;                          \n\
                         out vec4 out_Color;                             \n\
                         \n\
                         void main(void)                                 \n\
                         {                                               \n\
                         gl_FragColor = v_Color; //texture2D(s_texture, v_TexCoord);;                     \n\
                         }                                               \n\
                         ";

///////////////////////////////////////////////////////////////////////////////////////////////////
/// GLShader::GLShader
///////////////////////////////////////////////////////////////////////////////////////////////////
GLShader::GLShader(
    IvyShaderType shaderType,
    const CHAR* pShaderName,
    const CHAR* pShaderFilename)
    :
    IvyShader(shaderType, pShaderName, pShaderFilename),
    m_glShaderId(0)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// GLShader::~GLShader
///////////////////////////////////////////////////////////////////////////////////////////////////
GLShader::~GLShader()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// GLShader::CreateFromSource
///////////////////////////////////////////////////////////////////////////////////////////////////
GLShader* GLShader::CreateFromSource(
    IvyShaderType shaderType,
    const CHAR* pShaderName,
    const CHAR* pShaderSource)
{
    GLShader* pNewShader = NULL;
    GLint compileStatus = 0;
    int shaderId = 0;

    if (shaderType == IvyVertexShader)
    {
        shaderId = glCreateShader(GL_VERTEX_SHADER);

        ///@todo remove this
        pShaderSource = gVS2String;
    }
    else if (shaderType == IvyFragmentShader)
    {
        shaderId = glCreateShader(GL_FRAGMENT_SHADER);

        ///@todo remove this
        pShaderSource = gFS2String;
    }

    glShaderSource(shaderId, 1, &pShaderSource, NULL);
    glCompileShader(shaderId);

    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileStatus);

    if (compileStatus == GL_FALSE)
    {
        // Shader compile log
        GLsizei bufferLength = 1024;
        GLchar buffer[1024];
        memset(buffer, 0, bufferLength);
        GLsizei logLength = 0;

        glGetShaderInfoLog(shaderId, bufferLength, &logLength, buffer);
    }
    else
    {
        pNewShader = new GLShader(shaderType, pShaderName, "");
        pNewShader->m_glShaderId = shaderId;
    }

    return pNewShader;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// GLShader::CreateFromFile
///////////////////////////////////////////////////////////////////////////////////////////////////
GLShader* GLShader::CreateFromFile(
    IvyShaderType shaderType,
    const CHAR* pShaderName,
    const CHAR* pFilename)
{
    GLShader* pNewShader = NULL;
    GLint compileStatus = 0;
    int shaderId = 0;

    ///@TODO This is pretty wasteful, and unsafe.  Refactor to some generic IvyFile.h/cpp
    FILE* pShaderFile = fopen(pFilename, "r");
    int ShaderBufferSize = 16384;
    CHAR* pShaderSource = new CHAR[ShaderBufferSize];
    memset(pShaderSource, 0, ShaderBufferSize);
    size_t charsRead = fread(pShaderSource, sizeof(CHAR), ShaderBufferSize, pShaderFile);
    fclose(pShaderFile);

    if (shaderType == IvyVertexShader)
    {
        shaderId = glCreateShader(GL_VERTEX_SHADER);
    }
    else if (shaderType == IvyFragmentShader)
    {
        shaderId = glCreateShader(GL_FRAGMENT_SHADER);
    }

    const GLchar* pGLsource = reinterpret_cast<GLchar*>(pShaderSource);
    glShaderSource(shaderId, 1, &pGLsource, (GLint*)&charsRead);
    glCompileShader(shaderId);

    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileStatus);

    if (compileStatus == GL_FALSE)
    {
        // Shader compile log
        GLsizei bufferLength = 1024;
        GLchar buffer[1024];
        memset(buffer, 0, bufferLength);
        GLsizei logLength = 0;

        glGetShaderInfoLog(shaderId, bufferLength, &logLength, buffer);
        printf(buffer);
        IvyAssertAlways();
    }
    else
    {
        pNewShader = new GLShader(shaderType, pShaderName, "");
        pNewShader->m_glShaderId = shaderId;
    }

    delete [] pShaderSource;

    return pNewShader;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// GLShader::Destroy
///////////////////////////////////////////////////////////////////////////////////////////////////
void GLShader::Destroy()
{
    if (m_glShaderId != 0)
    {
        glDeleteShader(m_glShaderId);
    }

    delete this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// GLShader::Reload
///////////////////////////////////////////////////////////////////////////////////////////////////
void GLShader::Reload()
{
    glDeleteShader(m_glShaderId);

    /*
    while ()
    {

    }*/
}


///////////////////////////////////////////////////////////////////////////////////////////////////
/// GLProgram::GLProgram
///////////////////////////////////////////////////////////////////////////////////////////////////
GLProgram::GLProgram()
    :
    m_isLinked(FALSE),
    m_glProgramId(0),
    m_pVSShader(NULL),
    m_pFSShader(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// GLProgram::~GLProgram
///////////////////////////////////////////////////////////////////////////////////////////////////
GLProgram::~GLProgram()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// GLProgram::Create
///////////////////////////////////////////////////////////////////////////////////////////////////
GLProgram* GLProgram::Create()
{
    GLProgram* pNewProgram = NULL;

    int programId = glCreateProgram();

    if (programId != 0)
    {
        pNewProgram = new GLProgram();
        pNewProgram->m_glProgramId = programId;
    }

    return pNewProgram;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// GLProgram::Destroy
///////////////////////////////////////////////////////////////////////////////////////////////////
void GLProgram::Destroy()
{
    if (m_glProgramId != 0)
    {
        glDeleteProgram(m_glProgramId);
    }

    delete this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// GLProgram::AttachShader
///////////////////////////////////////////////////////////////////////////////////////////////////
void GLProgram::AttachShader(
    GLShader* pShader)
{
    if ((pShader->Type() == IvyVertexShader) && 
        (m_pVSShader != pShader))
    {
        m_pVSShader = pShader;
        glAttachShader(m_glProgramId, m_pVSShader->ShaderId());

        m_isLinked = FALSE;
    }
    else if ((pShader->Type() == IvyFragmentShader) && 
             (m_pFSShader != pShader))
    {
        m_pFSShader = pShader;
        glAttachShader(m_glProgramId, m_pFSShader->ShaderId());

        m_isLinked = FALSE;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// GLProgram::Link
///////////////////////////////////////////////////////////////////////////////////////////////////
bool GLProgram::Link()
{
    if (m_isLinked == false)
    {
        glLinkProgram(m_glProgramId);

        // Shader compile log
        GLsizei bufferLength = 1024;
        GLchar buffer[1024];
        memset(buffer, 0, bufferLength);
        GLsizei logLength = 0;

        glGetProgramInfoLog(m_glProgramId, bufferLength, &logLength, buffer);
        printf(buffer);
    }

    return m_isLinked;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// GLProgram::Bind
///////////////////////////////////////////////////////////////////////////////////////////////////
void GLProgram::Bind()
{
    glUseProgram(m_glProgramId);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// GLProgram::Reload
///////////////////////////////////////////////////////////////////////////////////////////////////
void GLProgram::Reload()
{
    
}


