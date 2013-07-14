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

#ifndef XNA_MATH
#define STUB 1
#endif 

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
   // DrawTestGL2();
    DrawTestGL4();
    //DrawTestGLES2();

    //ParticlesTest();
    //LightingTest();
    //TessTest();
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
#if XNA_MATH
        XMMATRIX worldMatrix;
        XMMATRIX viewMatrix;
        XMMATRIX projectionMatrix;
#else
        IvyMatrix4x4 worldMatrix;
        IvyMatrix4x4 viewMatrix;
        IvyMatrix4x4 projectionMatrix;
#endif
    };


    VertexPTN triangle[4];
    memset(triangle, 0, sizeof(triangle));

    float z = 0.0f;

    triangle[0].Pos = Point3(-1.0f, 1.0f, z);
    triangle[0].N   = Point4(1.0f, 0.0f, 0.0f, 1.0f);
    triangle[0].Tex = Point2(0.0f, 0.0f);
    triangle[1].Pos = Point3(1.0f, 1.0f, z);
    triangle[1].N   = Point4(0.0f, 1.0f, 0.0f, 1.0f);
    triangle[1].Tex = Point2(1.0f, 0.0f);
    triangle[2].Pos = Point3(1.0f, -1.0f, z);
    triangle[2].N   = Point4(0.0f, 0.0f, 1.0f, 1.0f);
    triangle[2].Tex = Point2(1.0f, 1.0f);
    triangle[3].Pos = Point3(-1.0f, -1.0f, z);
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

    ///@todo Migrate settings into texture object?  Or have separate sampler that is attached to texture?
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glViewport(0, 0, m_screenWidth, m_screenHeight);

    GLubyte indices[] = {0, 1, 2, 0, 2, 3 };
    while (!quit)
    {
        ProcessUpdates();

        const IvyGamepadState* pGamepad = GetGamepadState();
        if (pGamepad->ButtonPressed[IvyGamepadButtons::ButtonA])
        {
            glClearColor(0.0f, 4.0f, 0.0f, 1.0f);
        }
        else if (pGamepad->ButtonPressed[IvyGamepadButtons::ButtonB])
        {
            glClearColor(0.4f, 0.0f, 0.0f, 1.0f);
        }
        else
        {
            glClearColor(0.4f, 1.0f, 0.4f, 1.0f);
        }
        

        glClear(GL_COLOR_BUFFER_BIT);

        CameraBufferData cameraBufferData;
#if XNA_MATH
        // camera is at (0,0).  need to offset world by z+1 to move camera to -1 for LookAt(0,0) (i think...)
        cameraBufferData.worldMatrix = XMMatrixTranslation(0, 0, (IVY_MAX(-1.0, pGamepad->ThumbLY) * 5.0) + 6.0);
        cameraBufferData.viewMatrix = m_pCamera->W2C();
        cameraBufferData.projectionMatrix = m_pCamera->C2S();

        UINT worldMatrixAttribLoc = glGetUniformLocation(pProgram->ProgramId(), "worldMatrix");
        UINT viewMatrixAttribLoc = glGetUniformLocation(pProgram->ProgramId(), "viewMatrix");
        UINT projMatrixAttribLoc = glGetUniformLocation(pProgram->ProgramId(), "projectionMatrix");

        glUniformMatrix4fv(worldMatrixAttribLoc, 1, GL_FALSE, reinterpret_cast<GLfloat*>(&cameraBufferData.worldMatrix));
        glUniformMatrix4fv(viewMatrixAttribLoc, 1, GL_FALSE, reinterpret_cast<GLfloat*>(&cameraBufferData.viewMatrix));
        glUniformMatrix4fv(projMatrixAttribLoc, 1, GL_FALSE, reinterpret_cast<GLfloat*>(&cameraBufferData.projectionMatrix));

#else
        cameraBufferData.worldMatrix = IvyMatrix4x4::Identity(); 
        //XMMatrixIdentity(); //XMMatrixRotationX(-3.14f/2.0f) * XMMatrixScaling(2, 2, 1); //XMMatrixIdentity();
        cameraBufferData.viewMatrix = m_pCamera->W2C();
        cameraBufferData.projectionMatrix = m_pCamera->C2S();

        UINT worldMatrixAttribLoc = glGetUniformLocation(pProgram->ProgramId(), "worldMatrix");
        UINT viewMatrixAttribLoc = glGetUniformLocation(pProgram->ProgramId(), "viewMatrix");
        UINT projMatrixAttribLoc = glGetUniformLocation(pProgram->ProgramId(), "projectionMatrix");

        glUniformMatrix4fv(worldMatrixAttribLoc, 1, GL_FALSE, reinterpret_cast<GLfloat*>(cameraBufferData.worldMatrix.data()));
        glUniformMatrix4fv(viewMatrixAttribLoc, 1, GL_FALSE, reinterpret_cast<GLfloat*>(cameraBufferData.viewMatrix.data()));
        glUniformMatrix4fv(projMatrixAttribLoc, 1, GL_FALSE, reinterpret_cast<GLfloat*>(cameraBufferData.projectionMatrix.data()));
#endif

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

    glViewport(0, 0, m_screenWidth, m_screenHeight);             // context state
    glClearColor(0.4f, 1.0f, 0.4f, 1.0f);   // context state

    GLShader* pVSShader = GLShader::CreateFromFile(IvyVertexShader, "SimpleVS4", "Content/shaders/gl4.vert");
    GLShader* pFSShader = GLShader::CreateFromFile(IvyFragmentShader, "SimpleFS4", "Content/shaders/gl4.frag");

    GLProgram* pProgram = GLProgram::Create();
    pProgram->AttachShader(pVSShader);
    pProgram->AttachShader(pFSShader);
    pProgram->Link();
    pProgram->Bind();

#ifndef STUB
    struct CameraBufferData
    {
        XMMATRIX worldMatrix;
        XMMATRIX viewMatrix;

        XMMATRIX projectionMatrix;
    };

    CameraBufferData cameraBufferData;
    cameraBufferData.worldMatrix      = XMMatrixScaling(5, 5, 1); //XMMatrixIdentity(); //XMMatrixRotationX(-3.14f/2.0f) * XMMatrixScaling(2, 2, 1); //XMMatrixIdentity();
    cameraBufferData.viewMatrix = XMMatrixTranslation(0, 0, 2.0f) * m_pCamera->W2C();
    cameraBufferData.projectionMatrix = m_pCamera->C2S();

    UINT worldMatrixAttribLoc = glGetUniformLocation(pProgram->ProgramId(), "worldMatrix");
    UINT viewMatrixAttribLoc = glGetUniformLocation(pProgram->ProgramId(), "viewMatrix");
    UINT projMatrixAttribLoc = glGetUniformLocation(pProgram->ProgramId(), "projectionMatrix");


    glUniformMatrix4fv(worldMatrixAttribLoc, 1, GL_FALSE, reinterpret_cast<GLfloat*>(&cameraBufferData.worldMatrix));
    glUniformMatrix4fv(viewMatrixAttribLoc, 1, GL_FALSE, reinterpret_cast<GLfloat*>(&cameraBufferData.viewMatrix));
    glUniformMatrix4fv(projMatrixAttribLoc, 1, GL_FALSE, reinterpret_cast<GLfloat*>(&cameraBufferData.projectionMatrix));
#endif

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

    unsigned int triIndicies[3] = { 0, 1, 2 };

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
    meshCreateInfo.pVertexData = &triangle[0];
    meshCreateInfo.numVertices = 3;

    meshCreateInfo.pIndexData = &triIndicies[0];
    meshCreateInfo.numIndicies = 3;

    Import(meshCreateInfo.numVertices, (VertexPTN**)&meshCreateInfo.pVertexData, meshCreateInfo.numIndicies, (UINT**)&meshCreateInfo.pIndexData);

    GLMesh* pMesh = GLMesh::Create(&meshCreateInfo);

    m_pWindow->Show();

    glEnable(GL_DEPTH_TEST);

    BOOL quit = FALSE;
    while (!quit)
    {
        ProcessUpdates();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#ifndef STUB

        // Update camera for Bunny
        cameraBufferData.worldMatrix = XMMatrixScaling(1, 1, 1) * XMMatrixTranslation(0, -1, 0);

        cameraBufferData.viewMatrix = m_pCamera->W2C() * XMMatrixTranslation(0, 0, 1);
        cameraBufferData.projectionMatrix = m_pCamera->C2S();

        glUniformMatrix4fv(worldMatrixAttribLoc, 1, GL_TRUE, reinterpret_cast<GLfloat*>(&cameraBufferData.worldMatrix));
        glUniformMatrix4fv(viewMatrixAttribLoc, 1, GL_TRUE, reinterpret_cast<GLfloat*>(&cameraBufferData.viewMatrix));
        glUniformMatrix4fv(projMatrixAttribLoc, 1, GL_TRUE, reinterpret_cast<GLfloat*>(&cameraBufferData.projectionMatrix));
#endif

        pMesh->Bind(pProgram);
        pMesh->Draw();

        //glDrawArrays(GL_TRIANGLES, 0, 3);

        IvySwapBuffers();
    }

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(m_hGLRC);
#endif // !(IVY_GL_ES)
}



