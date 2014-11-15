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

#include <cstdio> // _snprintf

#ifndef XNA_MATH
#define STUB 1
#endif

using namespace Ivy;


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


void GLTestApp::Run()
{
   // DrawTestGL2();
    DrawTestGL4();
    //DrawTestGLES2();

   // ParticlesTest();
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
        const KeyboardState* pKeys = GetKeyboardState();

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
        

        if (pKeys->Pressed[Key_W])
        {
            m_pCamera->Move(Point3(0, 0, 0.05f), 0, 0);
        }
        else if (pKeys->Pressed[Key_S])
        {
            m_pCamera->Move(Point3(0, 0, -0.05f), 0, 0);
        }

        if (pKeys->Pressed[Key_A])
        {
            m_pCamera->Move(Point3(-0.05f, 0, 0), 0, 0);
        }
        if (pKeys->Pressed[Key_D])
        {
            m_pCamera->Move(Point3(0.05f, 0, 0), 0, 0);
        }

        glClear(GL_COLOR_BUFFER_BIT);

        CameraBufferData cameraBufferData;
#if XNA_MATH
        // camera is at (0,0).  need to offset world by z+1 to move camera to -1 for LookAt(0,0) (i think...)
        cameraBufferData.worldMatrix = XMMatrixTranslation(0, 0, (IVY_MAX(-1.0f, pGamepad->ThumbLY) * 5.0f) + 6.0f);
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

	GLShader* pVsFont = GLShader::CreateFromFile(IvyVertexShader, "SimpleVS4", "Content/shaders/gl4/font.vert");
	GLShader* pFsTexture = GLShader::CreateFromFile(IvyFragmentShader, "Texture", "Content/shaders/gl4/texture.frag");

	GLProgram* pTexProgram = GLProgram::Create();
	pTexProgram->AttachShader(pVsFont);
	pTexProgram->AttachShader(pFsTexture);
	pTexProgram->Link();
	pTexProgram->Bind();

	struct CameraBufferData
	{
		XMMATRIX worldMatrix;
		XMMATRIX viewMatrix;
		XMMATRIX projectionMatrix;
	};

	CameraBufferData cameraBufferData;
	cameraBufferData.worldMatrix = XMMatrixScaling(5, 5, 1); //XMMatrixIdentity(); //XMMatrixRotationX(-3.14f/2.0f) * XMMatrixScaling(2, 2, 1); //XMMatrixIdentity();
	cameraBufferData.viewMatrix = XMMatrixTranslation(0, 0, 2.0f) * m_pCamera->W2C();
	cameraBufferData.projectionMatrix = m_pCamera->C2S();


	pProgram->Bind();

	UINT worldMatrixAttribLoc = glGetUniformLocation(pProgram->ProgramId(), "worldMatrix");
	UINT viewMatrixAttribLoc = glGetUniformLocation(pProgram->ProgramId(), "viewMatrix");
	UINT projMatrixAttribLoc = glGetUniformLocation(pProgram->ProgramId(), "projectionMatrix");


	glUniformMatrix4fv(worldMatrixAttribLoc, 1, GL_FALSE, reinterpret_cast<GLfloat*>(&cameraBufferData.worldMatrix));
	glUniformMatrix4fv(viewMatrixAttribLoc, 1, GL_FALSE, reinterpret_cast<GLfloat*>(&cameraBufferData.viewMatrix));
	glUniformMatrix4fv(projMatrixAttribLoc, 1, GL_FALSE, reinterpret_cast<GLfloat*>(&cameraBufferData.projectionMatrix));



	IvyMeshCreateInfo bunnyMesh = { 0 };
	bunnyMesh.vertexSizeInBytes = sizeof(VertexPTN);
	Import(bunnyMesh.numVertices, (VertexPTN**)&bunnyMesh.pVertexData, bunnyMesh.numIndicies, (UINT**)&bunnyMesh.pIndexData);

	GLMesh* pBunnyMesh = GLMesh::Create(&bunnyMesh);

	Plane p;
	IvyMeshCreateInfo quadMeshInfo = { 0 };
	quadMeshInfo.vertexSizeInBytes = sizeof(VertexPTN);
	quadMeshInfo.pVertexData = p.GetVB();
	quadMeshInfo.numVertices = p.NumVertices();
	GLMesh* pQuadMesh = GLMesh::Create(&quadMeshInfo);

	// Setup Textures
	GLint textureAttribLoc = 0;


	GLTexture* pTexture = GLTexture::CreateFromFile(IvyTexture2D, "Content/Font.png");
	textureAttribLoc = glGetUniformLocation(pProgram->ProgramId(), "s_tex0");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	GLTexture* pFirefleaTex = GLTexture::CreateFromFile(IvyTexture2D, "Content/fireflea.png");
	textureAttribLoc = glGetUniformLocation(pProgram->ProgramId(), "s_tex1");

	pTexture->Bind(0, textureAttribLoc);
	pFirefleaTex->Bind(1, textureAttribLoc);

	///@todo Migrate settings into texture object?  Or have separate sampler that is attached to texture?
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	m_pWindow->Show();

	glEnable(GL_DEPTH_TEST);

	Point2 asciiCharMap[128];
	memset(&asciiCharMap, 0, sizeof(asciiCharMap));


	asciiCharMap['!'] = Point2(1, 0);
	asciiCharMap[','] = Point2(4, 1);
	asciiCharMap['.'] = Point2(6, 1);

	asciiCharMap['0'] = Point2(0, 2);
	asciiCharMap['1'] = Point2(1, 2);
	asciiCharMap['2'] = Point2(2, 2);
	asciiCharMap['3'] = Point2(3, 2);
	asciiCharMap['4'] = Point2(4, 2);
	asciiCharMap['5'] = Point2(5, 2);
	asciiCharMap['6'] = Point2(6, 2);
	asciiCharMap['7'] = Point2(7, 2);
	asciiCharMap['8'] = Point2(0, 3);
	asciiCharMap['9'] = Point2(1, 3);

	asciiCharMap['A'] = asciiCharMap['a'] = Point2(1, 4);
	asciiCharMap['B'] = asciiCharMap['b'] = Point2(2, 4);
	asciiCharMap['C'] = asciiCharMap['c'] = Point2(3, 4);
	asciiCharMap['D'] = asciiCharMap['d'] = Point2(4, 4);
	asciiCharMap['E'] = asciiCharMap['e'] = Point2(5, 4);
	asciiCharMap['F'] = asciiCharMap['f'] = Point2(6, 4);
	asciiCharMap['G'] = asciiCharMap['g'] = Point2(7, 4);
	asciiCharMap['I'] = asciiCharMap['i'] = Point2(1, 5);
	asciiCharMap['M'] = asciiCharMap['m'] = Point2(5, 5);
	asciiCharMap['N'] = asciiCharMap['n'] = Point2(6, 5);
	asciiCharMap['O'] = asciiCharMap['o'] = Point2(7, 5);
	asciiCharMap['P'] = asciiCharMap['p'] = Point2(0, 6);
	asciiCharMap['V'] = asciiCharMap['v'] = Point2(6, 6);
	asciiCharMap['Y'] = asciiCharMap['y'] = Point2(1, 7);


	int x, y;
	x = 2;
	y = 2;
    while (ExitApp() == FALSE)
    {
        ProcessUpdates();

        const KeyboardState* pKeys = GetKeyboardState();

        if (pKeys->Pressed[Key_W])
        {
            m_pCamera->Move(Point3(0, 0, 0.01f), 0, 0);
        }
        else if (pKeys->Pressed[Key_S])
        {
            m_pCamera->Move(Point3(0, 0, -0.01f), 0, 0);
        }

        if (pKeys->Pressed[Key_A])
        {
            m_pCamera->Move(Point3(-0.01f, 0, 0), 0, 0);
        }
        if (pKeys->Pressed[Key_D])
        {
            m_pCamera->Move(Point3(0.01f, 0, 0), 0, 0);
        }


        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update camera for Bunny
        cameraBufferData.worldMatrix = XMMatrixScaling(1, 1, 1) * XMMatrixTranslation(0, 1, 1);

        cameraBufferData.viewMatrix = m_pCamera->W2C();
        cameraBufferData.projectionMatrix = m_pCamera->C2S();

		pProgram->Bind();

		worldMatrixAttribLoc = glGetUniformLocation(pProgram->ProgramId(), "worldMatrix");
		viewMatrixAttribLoc = glGetUniformLocation(pProgram->ProgramId(), "viewMatrix");
		projMatrixAttribLoc = glGetUniformLocation(pProgram->ProgramId(), "projectionMatrix");

        glUniformMatrix4fv(worldMatrixAttribLoc, 1, GL_TRUE, reinterpret_cast<GLfloat*>(&cameraBufferData.worldMatrix));
        glUniformMatrix4fv(viewMatrixAttribLoc, 1, GL_TRUE, reinterpret_cast<GLfloat*>(&cameraBufferData.viewMatrix));
        glUniformMatrix4fv(projMatrixAttribLoc, 1, GL_TRUE, reinterpret_cast<GLfloat*>(&cameraBufferData.projectionMatrix));

		pBunnyMesh->Bind(pProgram);
		pBunnyMesh->Draw();

		glDisable(GL_DEPTH_TEST);

		// Quad

		pTexProgram->Bind();

		UINT charPosLoc = glGetUniformLocation(pTexProgram->ProgramId(), "charPos");
		glUniform2f(charPosLoc, (float)x, (float)y);

		//pQuadMesh->Bind(pTexProgram);
		//pQuadMesh->Draw();

		// Text Buffer 
		char buffer[1024];
		Point3 position;
		m_pCamera->Position(position);
		FLOAT phi;
		FLOAT theta;
		m_pCamera->Orientation(phi, theta);
		Point3 direction = m_pCamera->LookAt();

		_snprintf(buffer,
			1024,
			"Viewport: (%i, %i, %i, %i) FOVY: (%f)\nCamera Pos (%f, %f, %f)\nOrientation [Phi: %f, Theta: %f]\nDirection Vector: (%f, %f, %f)",
			0, 0, m_screenWidth, m_screenHeight,
			m_fovY * 180 / IvyPi,
			position.x,
			position.y,
			position.z,
			phi * 180 / IvyPi,
			theta * 180 / IvyPi,
			direction.x,
			direction.y,
			direction.z);

		UINT bufferLength = strlen(buffer);

		UINT charWidth = 15; 
		UINT charHeight = 15; 

		
		Point4* pVerts = static_cast<Point4*>(IVY_RAW_ALLOC(sizeof(Point4)* bufferLength * 6));
		//
		Point2 textPos = Point2(0, 0);

		FLOAT screenWidth = 512;
		FLOAT screenHeight = 512;
		
		for (UINT charQuad = 0; charQuad < bufferLength; ++charQuad)
		{
			Point2 cLoc = asciiCharMap[buffer[charQuad]];

			// BL
			pVerts[(charQuad * 6) + 0].x = (charQuad*charWidth) / (screenWidth / 2.0f);
			pVerts[(charQuad * 6) + 0].y = 0;
			pVerts[(charQuad * 6) + 0].z = cLoc.x;
			pVerts[(charQuad * 6) + 0].w = cLoc.y + 0.05;

			// BR
			pVerts[(charQuad * 6) + 1].x = ((charQuad*charWidth) + charWidth) / (screenWidth / 2.0f);
			pVerts[(charQuad * 6) + 1].y = 0;
			pVerts[(charQuad * 6) + 1].z = cLoc.x;
			pVerts[(charQuad * 6) + 1].w = cLoc.y + 0.15;

			// TR
			pVerts[(charQuad * 6) + 2].x = ((charQuad*charWidth) + charWidth) / (screenWidth / 2.0f);
			pVerts[(charQuad * 6) + 2].y = (charHeight) / (screenHeight / 2.0f);
			pVerts[(charQuad * 6) + 2].z = cLoc.x;
			pVerts[(charQuad * 6) + 2].w = cLoc.y + 0.25;

			// TR
			pVerts[(charQuad * 6) + 3].x = ((charQuad*charWidth) + charWidth) / (screenWidth / 2.0f);
			pVerts[(charQuad * 6) + 3].y = (charHeight) / (screenHeight / 2.0f);
			pVerts[(charQuad * 6) + 3].z = cLoc.x;
			pVerts[(charQuad * 6) + 3].w = cLoc.y + 0.25;

			// TL
			pVerts[(charQuad * 6) + 4].x = (charQuad*charWidth) / (screenWidth / 2.0f);
			pVerts[(charQuad * 6) + 4].y = (charHeight) / (screenHeight / 2.0f);
			pVerts[(charQuad * 6) + 4].z = cLoc.x;
			pVerts[(charQuad * 6) + 4].w = cLoc.y + 0.35;

			// BL
			pVerts[(charQuad * 6) + 5].x = (charQuad*charWidth) / (screenWidth / 2.0f);
			pVerts[(charQuad * 6) + 5].y = 0;
			pVerts[(charQuad * 6) + 5].z = cLoc.x;
			pVerts[(charQuad * 6) + 5].w = cLoc.y + 0.05;

		}


		GLuint vbId = 0;
		glGenBuffers(1, &vbId);
		glBindBuffer(GL_ARRAY_BUFFER, vbId);
		glBufferData(GL_ARRAY_BUFFER,
					 sizeof(Point4)* bufferLength * 6,
					 pVerts,
					 GL_STATIC_DRAW);

		GLint positionAttribLoc = glGetAttribLocation(pTexProgram->ProgramId(), "in_Position");
		if (positionAttribLoc >= 0)
		{
			glBindAttribLocation(pTexProgram->ProgramId(), positionAttribLoc, "in_Position");
			glVertexAttribPointer(positionAttribLoc, 4, GL_FLOAT, FALSE, sizeof(Point4), 0);
			glEnableVertexAttribArray(positionAttribLoc);
		}

		glDrawArrays(GL_TRIANGLES, 0, bufferLength*6);

		glDeleteBuffers(1, &vbId);
		IVY_FREE(pVerts);

        IvySwapBuffers();

    }

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(m_hGLRC);
#endif // !(IVY_GL_ES)
}



