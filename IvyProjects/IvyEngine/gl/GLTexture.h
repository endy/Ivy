///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2012, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///  
///////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef _GLTEXTURE_H_
#define _GLTEXTURE_H_

#include "IvyTexture.h"

class GLTexture :
    public IvyTexture
{
public:
    static GLTexture* CreateFromFile(IvyTextureType type, const CHAR* pFilename);
    virtual void Destroy();

    /// Returns OpenGL Texture ID
    UINT TextureId() const { return m_glTextureId; }

    void Bind(UINT textureSlot, UINT uniformLoc);

private:
    GLTexture(IvyTextureType type, UINT glTextureId, UINT width, UINT height);
    virtual ~GLTexture();

    UINT m_glTextureId;   ///< OpenGL Texture ID
};

#endif // _GLTEXTURE_H_

