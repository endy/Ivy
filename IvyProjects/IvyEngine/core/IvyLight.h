///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2010-2011, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _IVYLIGHT_H_
#define _IVYLIGHT_H_

#include "IvyTypes.h"

struct LightCreateInfo
{
    Point4 position;
    Point4 color;
};

// Point light source

class Light
{
public:
	static Light* Create(LightCreateInfo* pInfo);
	virtual void Destroy();

protected:
	Light(LightCreateInfo* pInfo);
	virtual ~Light();

    Point4 m_position;
    Point4 m_color;

private:
	Light(const Light& copy);	// disallow copy constructor
};

#endif // _LIGHT_H_
