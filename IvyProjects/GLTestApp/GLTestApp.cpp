///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine - Generic OpenGL Test App
///
///     Copyright 2010-2012, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////


#include "GLTestApp.h"

#include "IvyUtils.h"
#include "IvyImporter.h"
#include "IvyCamera.h"

#include "IvyWindow.h"
#include "GLTexture.h"
#include "GLShader.h"
#include "GLMesh.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
/// GlTestApp::GlTestApp
///
/// @brief
///     Constructor
/// @return
///     N/A
///////////////////////////////////////////////////////////////////////////////////////////////////
GLTestApp::GLTestApp()
    :
    IvyApp(),
    m_useEgl(FALSE)
{

}

GLTestApp::~GLTestApp()
{

}

GLTestApp* GLTestApp::Create()
{
    GLTestApp* pApp = new GLTestApp();

    if (pApp->Init() == false)
    {
        pApp->Destroy();
        pApp = NULL;
    }

    return pApp;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// GLTestApp::Destroy
///
/// @brief
///     Destroys the instance
/// @return
///     N/A
///////////////////////////////////////////////////////////////////////////////////////////////////
void GLTestApp::Destroy()
{
    IvyApp::Destroy();
}


bool GLTestApp::Init()
{
    bool success = IvyApp::Init();

    return success;
}

/*
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
*/
/*
typedef void (APIENTRY *GLDEBUGPROCAMD)(GLuint id,
    GLenum category,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    GLvoid* userParam);

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
*/

void GLTestApp::Run()
{
    //DrawTestGL2();
    //DrawTestGL4();
    //DrawTestGLES2();

    //ParticlesTest();
    LightingTest();
}

void GLTestApp::ReceiveEvent(
    const Event* pEvent)
{
    IvyApp::ReceiveEvent(pEvent);

    switch (pEvent->GetType())
    {
        case EventTypeWindowResize:
            glViewport(0, 0, m_pWindow->GetDrawableArea().right, m_pWindow->GetDrawableArea().bottom);
            break;
        case EventTypeKeyDown:
            ReceiveEventParticles(pEvent);
            break;
        default:
            break;
    }

}

void GLTestApp::InitGL2()
{
#if !(IVY_GL_ES)
    IVY_PRINT("GLTestApp OpenGL 2.0 Path");

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

    m_hDC = GetDC(m_pWindow->GetHwnd());


    int PixelFormat = ChoosePixelFormat(m_hDC,&pfd);
    BOOL setPixFmt = SetPixelFormat(m_hDC, PixelFormat, &pfd);

    m_hGLRC = wglCreateContext(m_hDC);
    wglMakeCurrent(m_hDC, m_hGLRC);

    if( glewInit() != GLEW_OK)
    {
        exit(1);
    }

    //  Error Setup


    //  glDebugMessageCallbackAMD((GLDEBUGPROCAMD)DebugCallbackAMD, NULL);

    //  glDebugMessageEnableAMD(0, 0, 0, NULL, TRUE);
    //glDebugMessageInsertAMDattribute  vec2 in_Position;                    
    //attribute  vec3 in_Color;                       
    ////////////glGetDebugMessageLogAMD

    const GLubyte* pString = glGetString(GL_VERSION);

    GLint majorVersion, minorVersion;
    glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
    glGetIntegerv(GL_MINOR_VERSION, &minorVersion);

    const GLubyte* pExtension = glGetString(GL_EXTENSIONS);
    IvyLog(reinterpret_cast<const char*>(pExtension));
#endif // !(IVY_GL_ES)
}

void GLTestApp::InitGL4()
{
#if !(IVY_GL_ES)
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

    m_hDC = GetDC(m_pWindow->GetHwnd());

    int PixelFormat = ChoosePixelFormat(m_hDC,&pfd);
    BOOL setPixFmt = SetPixelFormat(m_hDC, PixelFormat, &pfd);

    HGLRC tempGLRC = wglCreateContext(m_hDC);
    wglMakeCurrent(m_hDC, tempGLRC);

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

    m_hGLRC = wglCreateContextAttribsARB(m_hDC,0, attribs);

    wglMakeCurrent(NULL, NULL); // can't we just make current on the context we just created?
    wglDeleteContext(tempGLRC);

    wglMakeCurrent(m_hDC, m_hGLRC);
#endif // !(IVY_GL_ES)
}

void GLTestApp::InitGLES2()
{
#if IVY_GL_ES

    IVY_PRINT("GLTestApp OpenGL ES 2 Path");

    GLenum err = glGetError();
    typedef EGLAPI EGLDisplay EGLAPIENTRY FPTR_EGL_GET_DISPLAY(EGLNativeDisplayType display_id);

    FPTR_EGL_GET_DISPLAY* pEglGetDisplay;

    pEglGetDisplay = &eglGetDisplay;

    EGLint majorVersion;
    EGLint minorVersion;

    m_eglDisplay = (*pEglGetDisplay)(EGL_DEFAULT_DISPLAY);
    m_eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (m_eglDisplay == EGL_NO_DISPLAY)
    {
        // unable to get display
        return;
    }


    if (!eglInitialize(m_eglDisplay, &majorVersion, &minorVersion))
    {
        // unable to init display
        return;
    }

    EGLint numConfigs;
    eglGetConfigs(m_eglDisplay, NULL, 0, &numConfigs);

    EGLConfig* pConfigs = new EGLConfig[numConfigs];

    eglGetConfigs(m_eglDisplay, pConfigs, numConfigs, &numConfigs);

    struct EglAttribList
    {
        EGLint attribType;
        const char* attribName;
    };

    EglAttribList attribList[] = 
    {
        { EGL_RED_SIZE, "EGL_RED_SIZE" },
        { EGL_GREEN_SIZE, "EGL_GREEN_SIZE" },
        { EGL_BLUE_SIZE, "EGL_BLUE_SIZE" },
        { EGL_LUMINANCE_SIZE, "EGL_LUMINANCE_SIZE" },
        { EGL_ALPHA_SIZE, "EGL_ALPHA_SIZE" },
        { EGL_DEPTH_SIZE, "EGL_DEPTH_SIZE" },
        { EGL_STENCIL_SIZE, "EGL_STENCIL_SIZE" },

        { EGL_MAX_PBUFFER_WIDTH, "EGL_MAX_PBUFFER_WIDTH" },
        { EGL_MAX_PBUFFER_HEIGHT, "EGL_MAX_PBUFFER_HEIGHT" },
        { EGL_MAX_PBUFFER_PIXELS, "EGL_MAX_PBUFFER_PIXELS" },

        { EGL_SAMPLES, "EGL_SAMPLES" },
        { EGL_SAMPLE_BUFFERS, "EGL_SAMPLE_BUFFERS" }
    };

    const int AttribListSize = sizeof(attribList) / sizeof(EglAttribList);

    for (int configIdx = 0; configIdx < numConfigs; ++configIdx)
    {
        std::cout << "Config " << configIdx << ":" << std::endl;

        EGLint attribValue;
        for (int attribIdx = 0; attribIdx < AttribListSize; ++attribIdx)
        {
            eglGetConfigAttrib(m_eglDisplay, pConfigs[0], attribList[attribIdx].attribType, &attribValue);
            std::cout << "\t" << attribList[attribIdx].attribName << "=" << attribValue << std::endl;
        }
    }

    const EGLint windowAttribList[] = 
    {
        EGL_RENDER_BUFFER, EGL_BACK_BUFFER,
        EGL_NONE
    };

    m_eglSurface = eglCreateWindowSurface(m_eglDisplay, pConfigs[0], m_pWindow->GetHwnd(), NULL); //, &windowAttribList[0]);

    if (m_eglSurface == EGL_NO_SURFACE)
    {
        // no surface
        switch (eglGetError())
        {
        case EGL_BAD_NATIVE_WINDOW:
            break;
        }
    }

    EGLint contextAttribs[] =
    {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE,
    };

    m_eglContext = eglCreateContext(m_eglDisplay, pConfigs[0], EGL_NO_CONTEXT, NULL); // &contextAttribs[0]);

    eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext);

    // Dont syncronize with the display by default
    eglSwapInterval(m_eglDisplay, 0);

    delete [] pConfigs;
    pConfigs = NULL;

    m_useEgl = TRUE;

#endif // IVY_GL_ES
}

void GLTestApp::IvySwapBuffers()
{
#if IVY_GL_ES
    if (m_useEgl)
    {
        eglSwapBuffers(m_eglDisplay, m_eglSurface);
    }
    else
#endif // IVY_GL_ES
    {
        SwapBuffers(m_hDC);
    }
}

void GLTestApp::DrawTestGLES2()
{
#if IVY_GL_ES

    InitGLES2();

    m_pWindow->Show();

    GLenum err = glGetError();

    BOOL quit = FALSE;
    while (!quit)
    {
        m_pWindow->ProcessMsg(&quit);

        // draw stuff

        glClearColor(1.0f, 0.0f, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        IvySwapBuffers();
    }

    eglDestroyContext(m_eglDisplay, m_eglContext);
    eglDestroySurface(m_eglDisplay, m_eglSurface);
#endif // IVY_GL_ES
}

void GLTestApp::DrawTestGL2()
{
#if !(IVY_GL_ES)
    InitGL2();

    BOOL quit = FALSE;
    m_pWindow->Show();
    
    GLShader* pVSShader = GLShader::CreateFromFile(IvyVertexShader, "VertexShader", "Content/shaders/gl2.vert");
    GLShader* pFSShader = GLShader::CreateFromFile(IvyFragmentShader, "FragmentShader", "Content/shaders/gl2.frag");

    GLProgram* pProgram = GLProgram::Create();
    pProgram->AttachShader(pVSShader);
    pProgram->AttachShader(pFSShader);

    pProgram->Link();
    pProgram->Bind();

    struct CameraBufferData
    {
        XMMATRIX worldMatrix;
        XMMATRIX viewMatrix;
        XMMATRIX projectionMatrix;
    };

    CameraBufferData cameraBufferData;
    cameraBufferData.worldMatrix = XMMatrixScaling(1.0, 1.0, 1); 
    //XMMatrixIdentity(); //XMMatrixRotationX(-3.14f/2.0f) * XMMatrixScaling(2, 2, 1); //XMMatrixIdentity();
    cameraBufferData.viewMatrix = XMMatrixTranslation(0, 0, 3.0f) * m_pCamera->W2C();
    cameraBufferData.projectionMatrix = m_pCamera->C2S();

    UINT worldMatrixAttribLoc = glGetUniformLocation(pProgram->ProgramId(), "worldMatrix");
    UINT viewMatrixAttribLoc = glGetUniformLocation(pProgram->ProgramId(), "viewMatrix");
    UINT projMatrixAttribLoc = glGetUniformLocation(pProgram->ProgramId(), "projectionMatrix");

    glUniformMatrix4fv(worldMatrixAttribLoc, 1, GL_FALSE, reinterpret_cast<GLfloat*>(&cameraBufferData.worldMatrix));
    glUniformMatrix4fv(viewMatrixAttribLoc, 1, GL_FALSE, reinterpret_cast<GLfloat*>(&cameraBufferData.viewMatrix));
    glUniformMatrix4fv(projMatrixAttribLoc, 1, GL_FALSE, reinterpret_cast<GLfloat*>(&cameraBufferData.projectionMatrix));


    VertexPTN triangle[4];
    memset(triangle, 0, sizeof(triangle));

    triangle[0].Pos = Point3(-1.0f, 1.0f, 0.0f);
    triangle[0].N   = Point4(1.0f, 0.0f, 0.0f, 1.0f);
    triangle[0].Tex = Point2(0.0f, 0.0f);
    triangle[1].Pos = Point3(1.0f, 1.0f, 0.0f);
    triangle[1].N   = Point4(0.0f, 1.0f, 0.0f, 1.0f);
    triangle[1].Tex = Point2(1.0f, 0.0f);
    triangle[2].Pos = Point3(1.0f, -1.0f, 0.0f);
    triangle[2].N   = Point4(0.0f, 0.0f, 1.0f, 1.0f);
    triangle[2].Tex = Point2(1.0f, 1.0f);
    triangle[3].Pos = Point3(-1.0f, -1.0f, 0.0f);
    triangle[3].N   = Point4(1.0f, 1.0f, 1.0f, 1.0f);
    triangle[3].Tex = Point2(0.0f, 1.0f);

    GLint positionAttribLoc = glGetAttribLocation(pProgram->ProgramId(), "in_Position");
    GLint colorAttribLoc = glGetAttribLocation(pProgram->ProgramId(), "in_Color");
    GLint texAttribLoc = glGetAttribLocation(pProgram->ProgramId(), "in_TexCoord");

    glBindAttribLocation(pProgram->ProgramId(), positionAttribLoc, "in_Position");
    glBindAttribLocation(pProgram->ProgramId(), colorAttribLoc, "in_Color");
    glBindAttribLocation(pProgram->ProgramId(), texAttribLoc, "in_TexCoord");

    glVertexAttribPointer(positionAttribLoc, 3, GL_FLOAT, FALSE, 9*4, triangle);
    glEnableVertexAttribArray(positionAttribLoc);

    glVertexAttribPointer(colorAttribLoc, 4, GL_FLOAT, FALSE, 9*4, &(triangle[0].N));
    glEnableVertexAttribArray(colorAttribLoc);

    glVertexAttribPointer(texAttribLoc, 2, GL_FLOAT, FALSE, 9*4, &(triangle[0].Tex));
    glEnableVertexAttribArray(texAttribLoc);


    IvyMeshCreateInfo meshCreateInfo = {0};
    meshCreateInfo.vertexSizeInBytes = sizeof(VertexPTN);

    VertexPTN* pVB;
    UINT* pIB;

    Import(meshCreateInfo.numVertices, &pVB, meshCreateInfo.numIndicies, &pIB);
    meshCreateInfo.pVertexData = pVB;
    meshCreateInfo.pIndexData = pIB;

    GLMesh* pMesh = GLMesh::Create(&meshCreateInfo);

    // Setup Textures
    GLint textureAttribLoc = 0;

    //glActiveTexture(GL_TEXTURE0);
    GLTexture* pTexture = GLTexture::CreateFromFile(IvyTexture2D, "Content/kitten_rgb.png");
    textureAttribLoc = glGetUniformLocation(pProgram->ProgramId(), "s_tex0");

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    //glActiveTexture(GL_TEXTURE1);
    GLTexture* pFirefleaTex = GLTexture::CreateFromFile(IvyTexture2D, "Content/fireflea.png");
    textureAttribLoc = glGetUniformLocation(pProgram->ProgramId(), "s_tex1");

    pTexture->Bind(0, textureAttribLoc);
    pFirefleaTex->Bind(1, textureAttribLoc);

    ///@ todo Migrate settings into texture object?  Or have separate sampler that is attached to texture?
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glViewport(0, 0, 800, 450);

    GLubyte indices[] = {0, 1, 2, 0, 2, 3 };
    while (!quit)
    {
        m_pWindow->ProcessMsg(&quit);

        glClearColor(0.4f, 1.0f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);

        IvySwapBuffers();
    }

    pTexture->Destroy();
    pProgram->Destroy();
    pFSShader->Destroy();
    pVSShader->Destroy();
#endif // !(IVY_GL_ES)
}

void GLTestApp::DrawTestGL4()
{
#if !(IVY_GL_ES)
    IVY_PRINT("GLTestApp OpenGL 4.0 Path");

    InitGL4();

    glViewport(0, 0, 800, 450);             // context state
    glClearColor(0.4f, 1.0f, 0.4f, 1.0f);   // context state

    GLShader* pVSShader = GLShader::CreateFromFile(IvyVertexShader, "SimpleVS4", "gl4.vert");
    GLShader* pFSShader = GLShader::CreateFromFile(IvyFragmentShader, "SimpleFS4", "gl4.frag");

    GLProgram* pProgram = GLProgram::Create();
    pProgram->AttachShader(pVSShader);
    pProgram->AttachShader(pFSShader);
    pProgram->Link();
    pProgram->Bind();

    struct CameraBufferData
    {
        XMMATRIX worldMatrix;
        XMMATRIX viewMatrix;

        XMMATRIX projectionMatrix;
    };

    CameraBufferData cameraBufferData;
    cameraBufferData.worldMatrix      = XMMatrixScaling(5, 5, 1); //XMMatrixIdentity(); //XMMatrixRotationX(-3.14f/2.0f) * XMMatrixScaling(2, 2, 1); //XMMatrixIdentity();
    cameraBufferData.viewMatrix = XMMatrixTranslation(0, 0, 2.0f) * m_pCamera->W2C();
    //cameraBufferData.viewMatrix       = XMMatrixIdentity(); //XMMatrixTranslation(0, 0, 3.0f) * pCamera->W2C(); 
    cameraBufferData.projectionMatrix = m_pCamera->C2S();
    //cameraBufferData.projectionMatrix = XMMatrixIdentity(); // 

    UINT worldMatrixAttribLoc = glGetUniformLocation(pProgram->ProgramId(), "worldMatrix");
    UINT viewMatrixAttribLoc = glGetUniformLocation(pProgram->ProgramId(), "viewMatrix");
    UINT projMatrixAttribLoc = glGetUniformLocation(pProgram->ProgramId(), "projectionMatrix");


    glUniformMatrix4fv(worldMatrixAttribLoc, 1, GL_FALSE, reinterpret_cast<GLfloat*>(&cameraBufferData.worldMatrix));
    glUniformMatrix4fv(viewMatrixAttribLoc, 1, GL_FALSE, reinterpret_cast<GLfloat*>(&cameraBufferData.viewMatrix));
    glUniformMatrix4fv(projMatrixAttribLoc, 1, GL_FALSE, reinterpret_cast<GLfloat*>(&cameraBufferData.projectionMatrix));


    VertexPTN triangle[3];
    memset(triangle, 0, sizeof(triangle));

    triangle[0].Pos = Point3(-0.5f, 0.5f, 0.0f);
    triangle[0].N   = Point4(1.0f, 0.0f, 0.0f, 1.0f);
    triangle[0].Tex = Point2(0.0f, 0);
    triangle[1].Pos = Point3(0.5f, 0.5f, 0.0f);
    triangle[1].N   = Point4(0.0f, 1.0f, 0.0f, 1.0f);
    triangle[1].Tex = Point2(1.0f, 0);
    triangle[2].Pos = Point3(0.5f, -0.5f, 0.0f);
    triangle[2].N   = Point4(0.0f, 0.0f, 1.0f, 1.0f);
    triangle[2].Tex = Point2(1.0f, 1);

    GLuint vbId = 0;
    glGenBuffers(1, &vbId);
    glBindBuffer(GL_ARRAY_BUFFER, vbId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPTN)*3, triangle, GL_STATIC_DRAW);

    GLint positionAttribLoc = glGetAttribLocation(pProgram->ProgramId(), "in_Position");
    GLint colorAttribLoc = glGetAttribLocation(pProgram->ProgramId(), "in_Color");
    GLint texAttribLoc = glGetAttribLocation(pProgram->ProgramId(), "in_Tex");

    glBindAttribLocation(pProgram->ProgramId(), positionAttribLoc, "in_Position");
    glBindAttribLocation(pProgram->ProgramId(), colorAttribLoc, "in_Color");
    glBindAttribLocation(pProgram->ProgramId(), texAttribLoc, "in_Tex");

    glVertexAttribPointer(positionAttribLoc, 3, GL_FLOAT, FALSE, sizeof(VertexPTN), 0);
    glEnableVertexAttribArray(positionAttribLoc);

    GLuint offset = 5*4;
    glVertexAttribPointer(colorAttribLoc, 4, GL_FLOAT, FALSE, sizeof(VertexPTN), (const GLvoid*)offset);
    glEnableVertexAttribArray(colorAttribLoc);

    offset = 3*4;
    glVertexAttribPointer(texAttribLoc, 4, GL_FLOAT, FALSE, sizeof(VertexPTN), (const GLvoid*)offset);
    glEnableVertexAttribArray(texAttribLoc);


    IvyMeshCreateInfo meshCreateInfo = {0};
    meshCreateInfo.vertexSizeInBytes = sizeof(VertexPTN);

    VertexPTN* pVB;
    UINT* pIB;

    Import(meshCreateInfo.numVertices, &pVB, meshCreateInfo.numIndicies, &pIB);
    meshCreateInfo.pVertexData = pVB;
    meshCreateInfo.pIndexData = pIB;

    GLMesh* pMesh = GLMesh::Create(&meshCreateInfo);

    m_pWindow->Show();

    BOOL quit = FALSE;
    while(!quit)
    {
        m_pWindow->ProcessMsg(&quit);

        glClear(GL_COLOR_BUFFER_BIT);

        // Update camera for Bunny
        cameraBufferData.worldMatrix = XMMatrixIdentity(); 
        cameraBufferData.worldMatrix = XMMatrixScaling(5, 5.0, 1.0); //XMMatrixTranslation(0, 0, 0); // * XMMatrixRotationY(0);

        // translate world +6 in Z to position camera -9 from world origin
        //cameraBufferData.viewMatrix = XMMatrixIdentity(); 
        cameraBufferData.viewMatrix = m_pCamera->W2C(); //XMMatrixTranslation(0, -3, 10) * m_pCamera->W2C();

        cameraBufferData.projectionMatrix = XMMatrixIdentity();
        cameraBufferData.projectionMatrix = m_pCamera->C2S();

        glUniformMatrix4fv(worldMatrixAttribLoc, 1, GL_FALSE, reinterpret_cast<GLfloat*>(&cameraBufferData.worldMatrix));
        glUniformMatrix4fv(viewMatrixAttribLoc, 1, GL_FALSE, reinterpret_cast<GLfloat*>(&cameraBufferData.viewMatrix));
        glUniformMatrix4fv(projMatrixAttribLoc, 1, GL_FALSE, reinterpret_cast<GLfloat*>(&cameraBufferData.projectionMatrix));

        pMesh->Bind(pProgram);
        //pMesh->Draw();
        glDrawElements(GL_TRIANGLES, meshCreateInfo.numIndicies, GL_UNSIGNED_INT, meshCreateInfo.pIndexData);

        //glDrawArrays(GL_TRIANGLES, 0, 3);

        IvySwapBuffers();
    }

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(m_hGLRC);
#endif // !(IVY_GL_ES)
}



