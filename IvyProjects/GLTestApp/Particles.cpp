

#include "GLTestApp.h"
#include "IvyUtils.h"

#include "IvyImporter.h"
#include "IvyCamera.h"

#include "IvyWindow.h"
#include "GLTexture.h"

#include "IvyGL.h"
#include "GLShader.h"

inline float IvyMax(float a, float b) { return (a > b) ? a : b; }
inline float IvyMin(float a, float b) { return (a < b) ? a : b; }
inline float IvyClamp(float minClamp, float maxClamp, float value) { return IvyMax(IvyMin(value, maxClamp), minClamp); }


struct Particles
{
    int count;
    Point3* pPositions;
    Point2* pTexCoords;
    Point3* pVelocity;
};


///@TODO get rid of these globals
Particles* pParticles = NULL;
int width = 100, height = 100;


void InitParticles2DArray(Particles* pParticles, int width, int height)
{
    for (int w = 0; w < width; ++w)
    {
        for (int h = 0; h < height; ++h)
        {
            int index = (h * width) + w;

            float posx = -1.0f + ((2.0f / width) * w);
            float posy = -1.0f + ((2.0f / height) * h);

            pParticles->pPositions[index].x = posx;
            pParticles->pPositions[index].y = posy;
            pParticles->pPositions[index].z = 0.5;

            pParticles->pTexCoords[index].x = w / (float)width;
            pParticles->pTexCoords[index].y = 1.0f - h / (float)height;

            float magnitude = sqrt(posx*posx + posy*posy);

            pParticles->pVelocity[index].x = posx / magnitude;
            pParticles->pVelocity[index].y = posy / magnitude;
            pParticles->pVelocity[index].z = 0;
        }
    }
}

void UpdateParticles(Particles* particles, float timestep)
{
    float bounds = 3.0f;

    timestep *= 1;

    // calculate new velocity with x/'friction' or y/'gravity' constants
    float gravityDecel = 4 * timestep;
    float xFriction = 3.0f * timestep;
    for(int i = 0; i < particles->count; ++i)
    {
       // particles->pVelocity[i].x *= (particles->pVelocity[i].x > 0) ? -xFriction : xFriction;
        particles->pVelocity[i].y -= gravityDecel; // gravity is always negative

        
        // update position
        particles->pPositions[i].x = IvyClamp(-bounds, bounds, particles->pPositions[i].x + (particles->pVelocity[i].x * timestep));

        if ((particles->pPositions[i].x <= -bounds) || (particles->pPositions[i].x >= bounds))
        {
            particles->pVelocity[i].x *= -0.25f;
        }

        
        
        particles->pPositions[i].y = IvyClamp(-bounds, bounds, particles->pPositions[i].y + (particles->pVelocity[i].y * timestep));

        // change direction
        if ((particles->pPositions[i].y <= -bounds) || (particles->pPositions[i].y >= bounds))
        {
            particles->pVelocity[i].y *= -0.8f;
        }
    }
}

void GLTestApp::ParticlesTest()
{
#if IVY_GL_ES
    InitGLES2();
#else
    InitGL2();
#endif // IVY_GL_ES

    m_pWindow->Show();

    GLShader* pVSShader = GLShader::CreateFromFile(IvyVertexShader, "ParticleVS", "Content/shaders/particles.vert");
    GLShader* pFSShader = GLShader::CreateFromFile(IvyFragmentShader, "ParticlesFS", "Content/shaders/particles.frag");

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

    pParticles = new Particles();
    pParticles->count = width * height;
    pParticles->pPositions = new Point3[pParticles->count];
    pParticles->pTexCoords = new Point2[pParticles->count];
    pParticles->pVelocity = new Point3[pParticles->count];

    InitParticles2DArray(pParticles, width, height);

    GLint positionAttribLoc = glGetAttribLocation(pProgram->ProgramId(), "in_Position");
    GLint texAttribLoc = glGetAttribLocation(pProgram->ProgramId(), "in_TexCoord");

    glBindAttribLocation(pProgram->ProgramId(), positionAttribLoc, "in_Position");
    glBindAttribLocation(pProgram->ProgramId(), texAttribLoc, "in_TexCoord");

    glVertexAttribPointer(positionAttribLoc, 3, GL_FLOAT, FALSE, 3*4, pParticles->pPositions);
    glEnableVertexAttribArray(positionAttribLoc);

    glVertexAttribPointer(texAttribLoc, 2, GL_FLOAT, FALSE, 2*4, pParticles->pTexCoords);
    glEnableVertexAttribArray(texAttribLoc);

    GLenum error = glGetError();
    // Setup Textures
    GLint textureAttribLoc = 0;

    glActiveTexture(GL_TEXTURE0);
    GLTexture* pTexture = GLTexture::CreateFromFile(IvyTexture2D, "Content/kitten_rgb.png");
    textureAttribLoc = glGetUniformLocation(pProgram->ProgramId(), "s_tex0");

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    error = glGetError();
    glActiveTexture(GL_TEXTURE1);
    GLTexture* pFirefleaTex = GLTexture::CreateFromFile(IvyTexture2D, "Content/fireflea.png");
    textureAttribLoc = glGetUniformLocation(pProgram->ProgramId(), "s_tex1");

   // pTexture->Bind(0, textureAttribLoc);
    pFirefleaTex->Bind(1, textureAttribLoc);
    error = glGetError();
    ///@ todo Migrate settings into texture object?  Or have separate sampler that is attached to texture?
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glViewport(0, 0, m_pWindow->GetDrawableArea().right, m_pWindow->GetDrawableArea().bottom);

    BOOL quit = false;

    GLubyte* pIndices = NULL;

#if !(IVY_GL_ES)
    glEnable(GL_PROGRAM_POINT_SIZE);
#endif !(IVY_GL_ES)

    GLubyte* pIndicies = new GLubyte[width*height];

    for (int i = 0; i < width * height; ++i)
    {
        pIndicies[i] = i;
    }

    while (!quit)
    {
        m_pWindow->ProcessMsg(&quit);
        error = glGetError();
        //glClearColor(0.4f, 1.0f, 0.4f, 1.0f);

        glClearColor(0.0f,0.4,0.4,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        UpdateParticles(pParticles, 0.005f);

        glDrawArrays(GL_POINTS, 0, width * height);
        IvySwapBuffers();
    }

    pTexture->Destroy();
    pProgram->Destroy();
    pFSShader->Destroy();
    pVSShader->Destroy();
}

void GLTestApp::ReceiveEventParticles(
    const Event* pEvent)
{
    if (pEvent->GetType() == EventTypeKeyDown)
    {
        const EventKeyDown* pKeyDown = reinterpret_cast<const EventKeyDown*>(pEvent); 
        if (pKeyDown->GetData().key == EKeyR)
        {
            InitParticles2DArray(pParticles, width, height);
        }
    }
}