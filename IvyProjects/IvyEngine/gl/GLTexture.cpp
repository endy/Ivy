///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2012, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "GLTexture.h"
#include "IvyGL.h"

#include <corona.h>

///@todo Add a compile-time assert to check this table matches IvyTextureType enums
const GLenum IvyToGLTexTypeTable[] =
{
#if IVY_GL_ES
    0,                      // IvyTexture1D
    GL_TEXTURE_2D,          // IvyTexture2D
    0,                      // IvyTexture3D
    GL_TEXTURE_CUBE_MAP     // IvyTextureCubeMap
#else
    GL_TEXTURE_1D,          // IvyTexture1D
    GL_TEXTURE_2D,          // IvyTexture2D
    GL_TEXTURE_3D,          // IvyTexture3D
    GL_TEXTURE_CUBE_MAP,    // IvyTextureCubeMap
#endif // IVY_GL_ES
};

const GLenum IvyToGLFormatTable[] =
{
    GL_RGBA,        // IvyFormatR8G8B8A8
    GL_RGB          // IvyFormatR8G8B8
};

///////////////////////////////////////////////////////////////////////////////////////////////////
/// GLTexture::GLTexture
///
/// @brief
///     Constructor
///
/// @note
///     test
///////////////////////////////////////////////////////////////////////////////////////////////////
GLTexture::GLTexture(
    IvyTextureType type,
    UINT glTextureId,
    UINT width,
    UINT height)
    :
    IvyTexture(type, width, height),
    m_glTextureId(glTextureId)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// GLTexture::~GLTexture
///
/// @brief
///     Destructor
///
/// @return
///     N/A
///////////////////////////////////////////////////////////////////////////////////////////////////
GLTexture::~GLTexture()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// GLTexture::CreateFromFile
///
/// @brief
///     Creates a GLTexture instance from an image file
///
/// @return
///     GLTexture instance
///////////////////////////////////////////////////////////////////////////////////////////////////
GLTexture* GLTexture::CreateFromFile(
    IvyTextureType type,        ///< Texture type
    const CHAR* pFilename)       ///< Image filename
{
    UINT texId = 0;
    glGenTextures(1, &texId);

    GLenum glTexType = IvyToGLTexTypeTable[type];
    glBindTexture(glTexType, texId);

    /*
    corona::Image *image = corona::OpenImage(filename.c_str(), corona::PF_R8G8B8A8, corona::FF_PNG);

    if(!image)
    {
        return NULL;
    }
    CTexture *tex = new CTexture(image->getWidth(), image->getHeight(), RGBA);

    byte* pixels = (byte*)image->getPixels();

    for(int row = 0; row < image->getHeight(); ++row)
    {
        for(int col = 0; col < image->getWidth(); ++col)
        {
            float r = (*pixels)/255.f; pixels++;
            float g = (*pixels)/255.f; pixels++;
            float b = (*pixels)/255.f; pixels++;
            float a = (*pixels)/255.f; pixels++;

            int rowrow = image->getHeight() - row - 1;
            tex->SetPixel(col, rowrow, CTexel(r, g, b, a));
        }
    }
    */


    corona::Image *pImage = corona::OpenImage(pFilename, corona::PF_R8G8B8A8, corona::FF_PNG);

    GLTexture* pNewTexture = NULL;

    if (pImage != NULL)
    {
        pNewTexture = new GLTexture(type, texId, pImage->getWidth(), pImage->getHeight());

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pImage->getWidth(), pImage->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, pImage->getPixels());
    }

    delete pImage;

    return pNewTexture;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// GLTexture::Destroy
///
/// @brief
///     Destroy the GLTexture instance
/// @return
///     N/A
///////////////////////////////////////////////////////////////////////////////////////////////////
void GLTexture::Destroy()
{
    if (m_glTextureId != 0)
    {
        glDeleteTextures(1, &m_glTextureId);
    }

    delete this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// GLTexture::Bind
///
/// @brief
///     
/// @return
///     N/A
///////////////////////////////////////////////////////////////////////////////////////////////////
void GLTexture::Bind(UINT textureSlot, UINT uniformLoc)
{
    GLenum texSlots[] = 
    {
        GL_TEXTURE0,
        GL_TEXTURE1,
        GL_TEXTURE2,
        GL_TEXTURE3
    };

    glActiveTexture(texSlots[textureSlot]);
    glUniform1i(uniformLoc, textureSlot);
    glBindTexture(IvyToGLTexTypeTable[m_type], m_glTextureId);
}

