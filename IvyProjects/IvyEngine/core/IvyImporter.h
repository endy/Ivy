///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2010-2013, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _IVYIMPORTER_H_
#define _IVYIMPORTER_H_

namespace Ivy
{
    void Import(UINT& numVertices, VertexPTN** ppVB, UINT& numIndices, UINT** ppIB);
    void ImportPly(const CHAR* plyFilename, UINT& numVertices, VertexPTN** ppVB, UINT& numIndices, UINT** ppIB);
}

#endif // _IVYIMPORTER_H_
