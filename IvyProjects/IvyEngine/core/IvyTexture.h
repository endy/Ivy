///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2012, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef _IVYTEXTURE_H_
#define _IVYTEXTURE_H_

#include "IvyObject.h"

struct IvyTextureFlags
{
    UINT ShaderInput    : 1;
    UINT RenderTarget   : 1;
    UINT DepthStencil   : 1;
    UINT CpuRead        : 1;
    UINT CpuWrite       : 1;
};

enum IvyTextureType
{
    IvyTexture1D            = 0,
    IvyTexture2D,
    IvyTexture3D,
    IvyTextureCubeMap,

    IvyMaxTextureTypes      // Enum sentinel, invalid texture type
};

enum IvyTextureFormat
{
    IvyFormatR8G8B8A8,
    IvyFormatR8G8B8,
};

class IvyTexture :
    public IvyObject
{
public:
    IvyTextureType Type() const { return m_type; }

protected:
    IvyTexture(IvyTextureType type, UINT width, UINT height);
    virtual ~IvyTexture();

    IvyTextureType m_type;      ///< Texture type

    UINT m_width;               ///< Texture width
    UINT m_height;              ///< Texture height

};


#endif // _IVYTEXTURE_H_

