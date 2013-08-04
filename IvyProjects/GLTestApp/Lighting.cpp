///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine - Generic OpenGL Test App
///
///     Copyright 2010-2012, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "GLTestApp.h"
#include "IvyWindow.h"
#include "IvyCamera.h"

#include "GLTexture.h"
#include "GLShader.h"

using namespace Ivy;

void GLTestApp::LightingTest()
{

#if IVY_GL_ES
    InitGLES2();
#else
    InitGL2();
#endif // IVY_GL_ES

    m_pWindow->Show();

    GLShader* pVSShader = GLShader::CreateFromFile(IvyVertexShader, "LightingVS", "Content/shaders/lighting.vert");
    GLShader* pFSShader = GLShader::CreateFromFile(IvyFragmentShader, "LightingFS", "Content/shaders/lighting.frag");
    GLShader* pPointVS = GLShader::CreateFromFile(IvyVertexShader, "PointVS", "Content/shaders/point.vert");
    GLShader* pPointFS = GLShader::CreateFromFile(IvyFragmentShader, "PointFS", "Content/shaders/point.frag");

    GLProgram* pProgram = GLProgram::Create();
    pProgram->AttachShader(pVSShader);
    pProgram->AttachShader(pFSShader);
    pProgram->Link();

    GLProgram* pPointProgram = GLProgram::Create();
    pPointProgram->AttachShader(pPointVS);
    pPointProgram->AttachShader(pPointFS);
    pPointProgram->Link();

    IVY_PRINT((const CHAR*)gluErrorString(glGetError()));

#if XNA_MATH
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

#endif

	pProgram->Bind();

    IVY_PRINT((const CHAR*)gluErrorString(glGetError()));

    GLint positionAttribLoc = glGetAttribLocation(pProgram->ProgramId(), "in_Position");
    GLint texAttribLoc = glGetAttribLocation(pProgram->ProgramId(), "in_TexCoord");
    GLint normalAttribLoc = glGetAttribLocation(pProgram->ProgramId(), "in_Normal");

    glBindAttribLocation(pProgram->ProgramId(), positionAttribLoc, "in_Position");
    glBindAttribLocation(pProgram->ProgramId(), texAttribLoc, "in_TexCoord");
    glBindAttribLocation(pProgram->ProgramId(), normalAttribLoc, "in_Normal");

    VertexPTN tri[3];
    tri[0].Pos = Point3(0.0f, 0.75f, 0.0f);
    tri[0].Tex = Point2(0.5f, 0.0f);
    tri[0].N =   Point4(0.0, 0.1, 1.0, 1.0);

    tri[1].Pos = Point3(-0.75f, -0.75f, 0.0f);
    tri[1].Tex = Point2(0.0, 1.0f);
    tri[1].N   = Point4(-0.3, -0.3, 1.0, 1.0);

    tri[2].Pos = Point3(0.75, -0.75f, 0.0f);
    tri[2].Tex = Point2(1.0f, 1.0f);
    tri[2].N   = Point4(0.3, -0.3, 1.0f, 1.0f);

    GLint lightPosAttribLoc = glGetUniformLocation(pProgram->ProgramId(), "lightPos");
    Point4 lightPos = Point4(1.0f, 0.0f, 0.5f, 1.0f);
    glUniform4fv(lightPosAttribLoc, 1, &lightPos.x);

    IVY_PRINT((const CHAR*)gluErrorString(glGetError()));

    GLint pointVBloc = glGetAttribLocation(pPointProgram->ProgramId(), "in_Position");
    glBindAttribLocation(pPointProgram->ProgramId(), pointVBloc, "in_Position");

    IVY_PRINT((const CHAR*)gluErrorString(glGetError()));
    // Setup Textures
    GLint textureAttribLoc = 0;

    glActiveTexture(GL_TEXTURE0);
    GLTexture* pTexture = GLTexture::CreateFromFile(IvyTexture2D, "Content/WeatheredStoneWall-ColorMap.png");
    textureAttribLoc = glGetUniformLocation(pProgram->ProgramId(), "s_tex0");
    pTexture->Bind(0, textureAttribLoc);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    IVY_PRINT((const CHAR*)gluErrorString(glGetError()));
    glActiveTexture(GL_TEXTURE1);
    GLTexture* pFirefleaTex = GLTexture::CreateFromFile(IvyTexture2D, "Content/WeatheredStoneWall-NormalMap.png");
    textureAttribLoc = glGetUniformLocation(pProgram->ProgramId(), "s_tex1");

    pFirefleaTex->Bind(1, textureAttribLoc);

    IVY_PRINT((const CHAR*)gluErrorString(glGetError()));
    ///@todo Migrate settings into texture object?  Or have separate sampler that is attached to texture?
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glViewport(0, 0, m_pWindow->GetDrawableArea().right, m_pWindow->GetDrawableArea().bottom);

    BOOL quit = false;

    float incAmt = 0.001;
    float xDirection = -1.0, zDirection = -1.0;

#if !(IVY_GL_ES)
    glEnable(GL_PROGRAM_POINT_SIZE);
#endif //!(IVY_GL_ES)


    glEnable(GL_DEPTH_TEST);

    while (!quit)
    {
        BeginFrame();

        m_pWindow->ProcessMsg(&quit);

        glClearColor(0.0f,0.4,0.4,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightPos.x = cos(incAmt) * 1;
        lightPos.z = sin(incAmt) * 1;
        incAmt += 0.001;

        pProgram->Bind();

        glUniform4fv(lightPosAttribLoc, 1, &lightPos.x);

        glVertexAttribPointer(positionAttribLoc, 3, GL_FLOAT, FALSE, 9*4, &tri[0].Pos);
        glEnableVertexAttribArray(positionAttribLoc);

        glVertexAttribPointer(texAttribLoc, 2, GL_FLOAT, FALSE, 9*4, &tri[0].Tex);
        glEnableVertexAttribArray(texAttribLoc);

        glVertexAttribPointer(normalAttribLoc, 4, GL_FLOAT, FALSE, 9*4, &tri[0].N);
        glEnableVertexAttribArray(normalAttribLoc);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        pPointProgram->Bind();

        glVertexAttribPointer(pointVBloc, 3, GL_FLOAT, FALSE, 3*4, &lightPos.x);
        glEnableVertexAttribArray(pointVBloc);

        glDrawArrays(GL_POINTS, 0, 1);

        IvySwapBuffers();

        EndFrame();
    }
}
