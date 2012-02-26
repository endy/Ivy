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

#include "GLWindow.h"

#include "IvyGL.h"
#include "GLShader.h"


#include "SOIL.h"

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
    RenderGL2();
    //RenderGL4();
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
    //glDebugMessageInsertAMDattribute  vec2 in_Position;                    
    //attribute  vec3 in_Color;                       
    ////////////glGetDebugMessageLogAMD

    const GLubyte* pString = glGetString(GL_VERSION);

    GLint majorVersion, minorVersion;
    glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
    glGetIntegerv(GL_MINOR_VERSION, &minorVersion);

    const GLubyte* pExtension = glGetString(GL_EXTENSIONS);

    GLShader* pVSShader = GLShader::CreateFromFile(IvyVertexShader, "VertexShader", "GLTestApp/gl2.vert");
    GLShader* pFSShader = GLShader::CreateFromFile(IvyFragmentShader, "FragmentShader", "GLTestApp/gl2.frag");

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

    glViewport(0, 0, 800, 450);

    GLuint texId = 0;
    glGenTextures(1, &texId);

    // GL_TEXTURE_1D
    // GL_TEXTURE_2D
    // GL_TEXTURE_3D
    // GL_TEXTURE_CUBE_MAP
    glBindTexture(GL_TEXTURE_2D, texId);

    GLint textureAttribLoc = glGetUniformLocation(pProgram->ProgramId(), "s_texture");
    glUniform1i(textureAttribLoc, 0);

    glActiveTexture(GL_TEXTURE0);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    int imageWidth = 0, imageHeight = 0, channels = 0, force_channels = 0;
    unsigned char* pImageData = SOIL_load_image("Content/kitten_rgb.dds", &imageWidth, &imageHeight, &channels, SOIL_LOAD_RGB);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, pImageData);
    SOIL_free_image_data(pImageData);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


   // GLuint fbId;    // framebuffer id
  //  glGenFramebuffers(1, &fbId);
 //   glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbId);
 //   glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texId, 0); //(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE)

   // GL_FRAMEBUFFER_UNSUPPORTED ;
 //   if (glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
 //       exit(1);
    }

    GLubyte indices[] = {0, 1, 2, 0, 2, 3 };
    while (!quit)
    {
        m_pWindow->ProcessMsg(&quit);

        glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

       // glDrawArrays(GL_TRIANGLES, 0, 3);

      //  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

        glClearColor(0.4f, 1.0f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);

        SwapBuffers(hDC);
    }


    glDeleteTextures(1, &texId);

    pProgram->Destroy();
    pFSShader->Destroy();
    pVSShader->Destroy();
}


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
    cameraBufferData.worldMatrix      = XMMatrixScaling(0.5, 1.0, 1); //XMMatrixIdentity(); //XMMatrixRotationX(-3.14f/2.0f) * XMMatrixScaling(2, 2, 1); //XMMatrixIdentity();
    cameraBufferData.viewMatrix = XMMatrixTranslation(0, 0, 3.0f) * m_pCamera->W2C();
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
