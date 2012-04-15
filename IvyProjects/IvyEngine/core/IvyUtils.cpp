///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2010-2011, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "IvyPlatform.h"
#include "IvyUtils.h"

Plane::Plane() :
	m_pVB(NULL)
{
	Create();
}

Plane::~Plane()
{
	if (m_pVB != NULL)
	{
		delete [] m_pVB;
	}
}

void Plane::Create()
{
	static const Point3 PosArray[VertCount] = 
	{
		Point3(-1, 0, 1), Point3( 1, 0, 1 ), Point3(1, 0, -1 ),
		Point3(-1, 0, 1), Point3(1, 0, -1 ), Point3(-1, 0, -1)
	};

	static const Point2 TexArray[VertCount] = 
	{
		Point2(0, 0), Point2(1, 0), Point2(1, 1),
		Point2(0, 0), Point2(1, 1), Point2(0, 1)
	};

	static const Point4 NormalArray[VertCount] =
	{
		Point4(0, 1, 0, 1), Point4(0, 1, 0, 1), Point4(0, 1, 0, 1),
		Point4(0, 1, 0, 1), Point4(0, 1, 0, 1), Point4(0, 1, 0, 1)
	};

	if (m_pVB == NULL)
	{
		m_pVB = new VertexPTN[VertCount];
	}

	for (UINT idx = 0; idx < VertCount; ++idx)
	{
		m_pVB[idx].Pos = PosArray[idx];
		m_pVB[idx].Tex = TexArray[idx];
		m_pVB[idx].N   = NormalArray[idx];
	}
}



Cube::Cube() :
	m_pVB(NULL)
{
	Create();
}

Cube::~Cube()
{
	if (m_pVB != NULL)
	{
		delete [] m_pVB;
	}
}

void Cube::Create()
{
	static const Point3 PosArray[VertCount] = 
	{
        /*
		// n = <1, 0, 0>
		{ 1, 1, -1}, { 1, 1, 1 }, { 1, -1, 1 },
		{ 1, 1, -1}, { 1, -1, 1 }, { 1, -1, -1 },

		// n = <-1, 0, 0>
		{ -1, 1, 1 }, { -1, 1, -1 }, { -1, -1, -1 },
		{ -1, 1, 1 }, { -1, -1, -1 }, { -1, -1, 1 },

		// n = <0, 1, 0>
		{ 1, 1, 1 }, { 1, 1, -1 }, { -1, 1, -1 },
		{ 1, 1, 1 }, { -1, 1, -1 }, { -1, 1, 1 },

		// n = <0, -1, 0>
		{ 1, -1, -1 }, { 1, -1, 1 }, { -1, -1, 1 },
		{ 1, -1, -1 }, { -1, -1, 1 }, { -1, -1, -1 },

		// n = <0, 0, 1>
		{ 1, 1, 1 }, { -1, 1, 1 }, { -1, -1, 1 },
		{ 1, 1, 1 }, { -1, -1, 1 }, { 1, -1, 1 },

		// n = <0, 0, -1>
		{ -1, 1, -1 }, { 1, 1, -1 }, { 1, -1, -1 },
		{ -1, 1, -1 }, { 1, -1, -1, }, { -1, -1, -1 }
        */
	};
	
	static const Point2 TexArray[VertCount] = 
	{
        /*
		// n = <1, 0, 0>
		{ 0, 0 }, { 1, 0 }, { 1, 1 },
		{ 0, 0 }, { 1, 1 }, { 0, 1 },

		// n = <-1, 0, 0>
		{ 0, 0 }, { 1, 0 }, { 1, 1 },
		{ 0, 0 }, { 1, 1 }, { 0, 1 },

		// n = <0, 1, 0>
		{ 0, 0 }, { 1, 0 }, { 1, 1 },
		{ 0, 0 }, { 1, 1 }, { 0, 1 },

		// n = <0, -1, 0>
		{ 0, 0 }, { 1, 0 }, { 1, 1 },
		{ 0, 0 }, { 1, 1 }, { 0, 1 },

		// n = <0, 0, 1>
		{ 0, 0 }, { 1, 0 }, { 1, 1 },
		{ 0, 0 }, { 1, 1 }, { 0, 1 },

		// n = <0, 0, -1>
		{ 0, 0 }, { 1, 0 }, { 1, 1 },
		{ 0, 0 }, { 1, 1 }, { 0, 1 },
        */
	};

	if (m_pVB == NULL)
	{
		m_pVB = new VertexPT[VertCount];
	}

	for (UINT idx = 0; idx < VertCount; ++idx)
	{
		m_pVB[idx].Pos = PosArray[idx];
		m_pVB[idx].Tex = TexArray[idx];
	}
}

