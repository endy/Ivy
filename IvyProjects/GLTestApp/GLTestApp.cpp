///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine - Generic DirectX Test App
///
///     Copyright 2010-2011, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////


#include "GLTestApp.h"
#include "IvyUtils.h"

#include "IvyImporter.h"

#include "GLWindow.h"

#define GLEW_STATIC 1
#include "GL\glew.h"

GLTestApp::GLTestApp(GLTestAppCreateInfo* pAppInfo)
    :
    GLApp(pAppInfo)
{

}

GLTestApp::~GLTestApp()
{

}

GLTestApp* GLTestApp::Create(GLTestAppCreateInfo* pAppInfo)
{
    GLTestApp* pApp = new GLTestApp(pAppInfo);

    if (pApp->Init() == false)
    {
        pApp->Destroy();
        pApp = NULL;
    }

    return pApp;
}

void GLTestApp::Destroy()
{
    GLApp::Destroy();
}

bool GLTestApp::Init()
{
    bool success = GLApp::Init();

    return success;
}

const CHAR* gVS2String = "                       \n\
attribute  vec3 in_Position;                    \n\
attribute  vec4 in_Color;                       \n\
varying    vec4 v_Color;                        \n\
                                                \n\
void main(void)                                 \n\
{                                               \n\
    gl_Position = vec4(in_Position, 1.0);       \n\
    v_Color = in_Color;                         \n\
}                                               \n\
                                                ";

const CHAR* gFS2String = "                      \n\
varying  vec4 v_Color;                          \n\
out vec4 out_Color;                             \n\
                                                \n\
void main(void)                                 \n\
{                                               \n\
    gl_FragColor = v_Color;                     \n\
}                                               \n\
                                                ";


void FormatDebugOutputAMD(char outStr[], size_t outStrSize, GLenum category, GLuint id,
    GLenum severity, const char *msg)
{
    char categoryStr[32];
    const char *categoryFmt = "UNDEFINED(0x%04X)";
    switch(category)
    {
    case GL_DEBUG_CATEGORY_API_ERROR_AMD:          categoryFmt = "API_ERROR"; break;
    case GL_DEBUG_CATEGORY_WINDOW_SYSTEM_AMD:      categoryFmt = "WINDOW_SYSTEM"; break;
    case GL_DEBUG_CATEGORY_DEPRECATION_AMD:        categoryFmt = "DEPRECATION"; break;
    case GL_DEBUG_CATEGORY_UNDEFINED_BEHAVIOR_AMD: categoryFmt = "UNDEFINED_BEHAVIOR"; break;
    case GL_DEBUG_CATEGORY_PERFORMANCE_AMD:        categoryFmt = "PERFORMANCE"; break;
    case GL_DEBUG_CATEGORY_SHADER_COMPILER_AMD:    categoryFmt = "SHADER_COMPILER"; break;
    case GL_DEBUG_CATEGORY_APPLICATION_AMD:        categoryFmt = "APPLICATION"; break;
    case GL_DEBUG_CATEGORY_OTHER_AMD:              categoryFmt = "OTHER"; break;
    }
    _snprintf(categoryStr, 32, categoryFmt, category);
    char severityStr[32];
    const char *severityFmt = "UNDEFINED";
    switch(severity)
    {
    case GL_DEBUG_SEVERITY_HIGH_AMD:   severityFmt = "HIGH";   break;
    case GL_DEBUG_SEVERITY_MEDIUM_AMD: severityFmt = "MEDIUM"; break;
    case GL_DEBUG_SEVERITY_LOW_AMD:    severityFmt = "LOW";    break;
    }
    _snprintf(severityStr, 32, severityFmt, severity);
    _snprintf(outStr, outStrSize, "OpenGL: %s [category=%s severity=%s id=%d]",
        msg, categoryStr, severityStr, id);
}
/*
typedef void (APIENTRY *GLDEBUGPROCAMD)(GLuint id,
    GLenum category,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    GLvoid* userParam);
*/

void DebugCallbackAMD(
    GLuint id,
    GLenum category,
    GLenum severity,
    GLsizei length,
    const GLchar *message,
    GLvoid *userParam)
{
    (void)length;
    FILE *outFile = (FILE*)userParam;
    char finalMsg[256];
    FormatDebugOutputAMD(finalMsg, 256, category, id, severity, message);
    fprintf(outFile, "%s\n", finalMsg);
}


void GLTestApp::Run()
{
    RenderGL2();
}


void GLTestApp::RenderGL2()
{
    BOOL quit = FALSE;

    m_pWindow->Show();

    static PIXELFORMATDESCRIPTOR pfd =              // pfd Tells Windows How We Want Things To Be
    {
        sizeof(PIXELFORMATDESCRIPTOR),              // Size Of This Pixel Format Descriptor
        1,                                          // Version Number
        PFD_DRAW_TO_WINDOW |                        // Format Must Support Window
        PFD_SUPPORT_OPENGL |                        // Format Must Support OpenGL
        PFD_DOUBLEBUFFER,                           // Must Support Double Buffering
        PFD_TYPE_RGBA,                              // Request An RGBA Format
        32,                                         // Select Our Color Depth
        0, 0, 0, 0, 0, 0,                           // Color Bits Ignored
        0,											// No Alpha Buffer
        0,											// Shift Bit Ignored
        0,											// No Accumulation Buffer
        0, 0, 0, 0,									// Accumulation Bits Ignored
        32,											// 16Bit Z-Buffer (Depth Buffer)  
        8,											// No Stencil Buffer
        0,											// No Auxiliary Buffer
        PFD_MAIN_PLANE,								// Main Drawing Layer
        0,											// Reserved
        0, 0, 0										// Layer Masks Ignored
    };

    HDC hDC = GetDC(m_pWindow->GetHwnd());


    int PixelFormat = ChoosePixelFormat(hDC,&pfd);
    BOOL setPixFmt = SetPixelFormat(hDC, PixelFormat, &pfd);

    HGLRC hGlc = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hGlc);

    if( glewInit() != GLEW_OK)
    {
        exit(1);
    }

    //  Error Setup


    //  glDebugMessageCallbackAMD((GLDEBUGPROCAMD)DebugCallbackAMD, NULL);

    //  glDebugMessageEnableAMD(0, 0, 0, NULL, TRUE);
    //glDebugMessageInsertAMDattribute  vec2 in_Position;                    \
    attribute  vec3 in_Color;                       \
    ////////////glGetDebugMessageLogAMD




    const GLubyte* pString = glGetString(GL_VERSION);

    GLint majorVersion, minorVersion;
    glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
    glGetIntegerv(GL_MINOR_VERSION, &minorVersion);

    const GLubyte* pExtension = glGetString(GL_EXTENSIONS);



    int vsLength = 0;
    int fsLength = 0; 
    GLenum errorEnum = glGetError();
    GLuint vsId = glCreateShader(GL_VERTEX_SHADER);
    errorEnum = glGetError();
    glShaderSource(vsId, 1, &gVS2String, NULL);
    errorEnum = glGetError();

    ////////////////////////////  glGetError()

    glCompileShader(vsId);

    GLint vsOK = 0;
    glGetShaderiv(vsId, GL_COMPILE_STATUS, &vsOK);

    GLsizei bufferLength = 1024;
    GLchar buffer[1024];
    memset(buffer, 0, bufferLength);
    GLsizei logLength = 0;
    glGetShaderInfoLog(vsId, bufferLength, &logLength, buffer);
    printf(buffer);

    GLuint fsId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fsId, 1, &gFS2String, NULL);

    glCompileShader(fsId);

    GLint fsOK = 0;
    glGetShaderiv(fsId, GL_COMPILE_STATUS, &fsOK);

    memset(buffer, 0, bufferLength);
    logLength = 0;
    glGetShaderInfoLog(fsId, bufferLength, &logLength, buffer);
    printf(buffer);

    GLuint programId = glCreateProgram();
    glAttachShader(programId, vsId);
    glAttachShader(programId, fsId);

    VertexPTN triangle[3];
    memset(triangle, 0, sizeof(triangle));

    triangle[0].Pos = Point3(-0.5f, 0.5f, 0.0f);
    triangle[0].N   = Point4(1.0f, 0.0f, 0.0f, 1.0f);
    triangle[1].Pos = Point3(0.5f, 0.5f, 0.0f);
    triangle[1].N   = Point4(0.0f, 1.0f, 0.0f, 1.0f);
    triangle[2].Pos = Point3(0.5f, -0.5f, 0.0f);
    triangle[2].N   = Point4(0.0f, 0.0f, 1.0f, 1.0f);

    glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, 9*4, triangle);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, FALSE, 9*4, &(triangle[0].N));
    glEnableVertexAttribArray(1);

    glBindAttribLocation(programId, 0, "in_Position");
    glBindAttribLocation(programId, 1, "in_Color");

    glLinkProgram(programId);

    GLint positionAttribLoc = glGetAttribLocation(programId, "in_Position");
    GLint colorAttribLoc = glGetAttribLocation(programId, "in_Color");

    memset(buffer, 0, bufferLength);
    logLength = 0;
    glGetProgramInfoLog(programId, bufferLength, &logLength, buffer);
    printf(buffer);

    glUseProgram(programId);

    glViewport(0, 0, 800, 450);

    while (!quit)
    {
        m_pWindow->ProcessMsg(&quit);

        glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        SwapBuffers(hDC);
    }

    glDeleteProgram(programId);
    glDeleteShader(vsId);
    glDeleteShader(fsId);
}

void GLTestApp::RenderGL4()
{

}
