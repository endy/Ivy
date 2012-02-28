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
#include "SOIL.h"

///@ todo Add a compile-time assert to check this table matches IvyTextureType enums
const GLenum IvyToGLTexTypeTable[] =
{
    GL_TEXTURE_1D,          // IvyTexture1D
    GL_TEXTURE_2D,          // IvyTexture2D
    GL_TEXTURE_3D,          // IvyTexture3D
    GL_TEXTURE_CUBE_MAP,    // IvyTextureCubeMap
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

    int texWidth = 0, texHeight = 0, channels = 0;
    unsigned char* pImageData = SOIL_load_image(pFilename, &texWidth, &texHeight, &channels, SOIL_LOAD_RGB);

    GLTexture* pNewTexture = NULL;

    if (pImageData != NULL)
    {
        pNewTexture = new GLTexture(type, texId, texWidth, texHeight);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, pImageData);

        SOIL_free_image_data(pImageData);
    }


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

