///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2012, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "GLMesh.h"
#include "IvyGL.h"
#include "GLShader.h"

using namespace Ivy;

///////////////////////////////////////////////////////////////////////////////////////////////////
/// GLMesh::GLMesh
///////////////////////////////////////////////////////////////////////////////////////////////////
GLMesh::GLMesh()
    :
    m_glVertexBufferId(0),
    m_numVertices(0),
    m_glIndexBufferId(0),
    m_numIndices(0)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// GLMesh::~GLMesh
///////////////////////////////////////////////////////////////////////////////////////////////////
GLMesh::~GLMesh()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// GLMesh::Create
///////////////////////////////////////////////////////////////////////////////////////////////////
GLMesh* GLMesh::Create(
    IvyMeshCreateInfo* pMeshCreateInfo)   ///< Mesh create info
{
    GLuint vbId = 0;
    glGenBuffers(1, &vbId);
    glBindBuffer(GL_ARRAY_BUFFER, vbId);
    glBufferData(GL_ARRAY_BUFFER,
                 pMeshCreateInfo->vertexSizeInBytes * pMeshCreateInfo->numVertices,
                 pMeshCreateInfo->pVertexData,
                 GL_STATIC_DRAW);

    GLMesh* pMesh = new GLMesh();
    pMesh->m_glVertexBufferId = vbId;
    pMesh->m_numVertices = pMeshCreateInfo->numVertices;

    if (pMeshCreateInfo->numIndicies > 0 && pMeshCreateInfo->pIndexData)
    {
        GLuint ibId = 0;
        glGenBuffers(1, &ibId);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, pMeshCreateInfo->numIndicies * 4, pMeshCreateInfo->pIndexData, GL_STATIC_DRAW);

        pMesh->m_glIndexBufferId = ibId;
        pMesh->m_numIndices = pMeshCreateInfo->numIndicies;
    }

    return pMesh;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// GLMesh::Destroy
///////////////////////////////////////////////////////////////////////////////////////////////////
void GLMesh::Destroy()
{
    if (m_glVertexBufferId > 0)
    {
        glDeleteBuffers(1, &m_glVertexBufferId);
    }

    delete this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// GLMesh::Bind
///////////////////////////////////////////////////////////////////////////////////////////////////
void GLMesh::Bind(
    GLProgram* pProgram)    ///< Bind the mesh to the current program
{
    ///@todo  Need to invert the relationship of the GLMesh and the GLProgram

	glBindBuffer(GL_ARRAY_BUFFER, m_glVertexBufferId);

	GLuint offset = 0;

    GLint positionAttribLoc = glGetAttribLocation(pProgram->ProgramId(), "in_Position");
	if (positionAttribLoc >= 0)
	{
		glBindAttribLocation(pProgram->ProgramId(), positionAttribLoc, "in_Position");
		glVertexAttribPointer(positionAttribLoc, 3, GL_FLOAT, FALSE, sizeof(VertexPTN), 0);
		glEnableVertexAttribArray(positionAttribLoc);
	}

    GLint colorAttribLoc = glGetAttribLocation(pProgram->ProgramId(), "in_Color");
	if (colorAttribLoc >= 0)
	{
		glBindAttribLocation(pProgram->ProgramId(), colorAttribLoc, "in_Color");
		offset = 5 * 4;
		glVertexAttribPointer(colorAttribLoc, 4, GL_FLOAT, FALSE, sizeof(VertexPTN), (const GLvoid*)offset);
		glEnableVertexAttribArray(colorAttribLoc);
	}

    GLint texAttribLoc = glGetAttribLocation(pProgram->ProgramId(), "in_Tex");
	if (texAttribLoc >= 0)
	{
		glBindAttribLocation(pProgram->ProgramId(), texAttribLoc, "in_Tex");
		offset = 3 * 4;
		glVertexAttribPointer(texAttribLoc, 2, GL_FLOAT, FALSE, sizeof(VertexPTN), (const GLvoid*)offset);
		glEnableVertexAttribArray(texAttribLoc);
	}

}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// GLMesh::Draw
///////////////////////////////////////////////////////////////////////////////////////////////////
void GLMesh::Draw()
{
    if (m_glIndexBufferId > 0)
    {
        glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, NULL);
    }
    else
    {
        glDrawArrays(GL_TRIANGLES, 0, m_numVertices);
    }
}


