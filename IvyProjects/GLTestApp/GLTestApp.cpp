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
#include "GL\wglew.h"

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
    RenderGL4();
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

const CHAR* gVS4String = "                      \n\
#version 130                                    \n\
in  vec3 in_Position;                           \n\
in  vec4 in_Color;                              \n\
out    vec4 v_Color;                            \n\
                                                \n\
void main(void)                                 \n\
{                                               \n\
    gl_Position = vec4(in_Position, 1.0);       \n\
    v_Color = in_Color;                         \n\
}                                               \n\
                                                ";

const CHAR* gFS4String = "                      \n\
#version 130                                    \n\
in  vec4 v_Color;                               \n\
out vec4 out_Color;                             \n\
                                                \n\
void main(void)                                 \n\
{                                               \n\
    out_Color = v_Color;                        \n\
}                                               \n\
                                                ";

void GLTestApp::RenderGL4()
{
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

    HGLRC hTempGlc = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hTempGlc);

    if( glewInit() != GLEW_OK)
    {
        exit(1);
    }

    if (GLEW_VERSION_4_1 == 0)
    {
        exit(1);
    }

    ///@todo detect CreateContextAttribsARB 

    int attribs[] =
    {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 1,
        WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB | WGL_CONTEXT_DEBUG_BIT_ARB,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
        0
    };

    HGLRC hGlrc = wglCreateContextAttribsARB(hDC,0, attribs);


    wglMakeCurrent(NULL, NULL); // can't we just make current on the context we just created?
    wglDeleteContext(hTempGlc);

    wglMakeCurrent(hDC, hGlrc);

    glViewport(0, 0, 800, 450);             // context state
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);   // context state


    // setup vertex data
    // setup shaders
    int vsLength = 0;
    int fsLength = 0; 
    GLenum errorEnum = glGetError();
    GLuint vsId = glCreateShader(GL_VERTEX_SHADER);
    errorEnum = glGetError();
    glShaderSource(vsId, 1, &gVS4String, NULL);
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
    glShaderSource(fsId, 1, &gFS4String, NULL);

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

    triangle[0].Pos = Point3(-0.5f, 0.5f, 0.5f);
    triangle[0].N   = Point4(1.0f, 0.0f, 0.0f, 1.0f);
    triangle[1].Pos = Point3(-0.5f, -0.5f, 0.0f);
    triangle[1].N   = Point4(0.0f, 1.0f, 0.0f, 1.0f);
    triangle[2].Pos = Point3(0.5f, 0.5f, 0.0f);
    triangle[2].N   = Point4(0.0f, 0.0f, 1.0f, 1.0f);

    GLuint vbId = 0;
    glGenBuffers(1, &vbId);
    glBindBuffer(GL_ARRAY_BUFFER, vbId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPTN)*3, triangle, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(VertexPTN), 0);
    glEnableVertexAttribArray(0);

    GLuint offset = 5*4;
    glVertexAttribPointer(1, 4, GL_FLOAT, FALSE, sizeof(VertexPTN), (const GLvoid*)offset);
    glEnableVertexAttribArray(1);

    glBindAttribLocation(programId, 0, "in_Position");
    glBindAttribLocation(programId, 1, "in_Color");

    glLinkProgram(programId);

   // GLint positionAttribLoc = glGetAttribLocation(programId, "in_Position");
   // GLint colorAttribLoc = glGetAttribLocation(programId, "in_Color");

    memset(buffer, 0, bufferLength);
    logLength = 0;
    glGetProgramInfoLog(programId, bufferLength, &logLength, buffer);
    printf(buffer);

    glUseProgram(programId);

    m_pWindow->Show();

    BOOL quit = FALSE;
    while(!quit)
    {
        m_pWindow->ProcessMsg(&quit);

        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        SwapBuffers(hDC);
    }

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hGlrc);
}
