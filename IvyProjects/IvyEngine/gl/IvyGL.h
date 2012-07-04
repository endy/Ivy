///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2012, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _IVYGL_H_
#define _IVYGL_H_

#define IVY_GL_ES 1

#if IVY_GL_ES

#include "EGL/egl.h"
#include "GLES2/gl2.h"

#pragma comment(lib, "libGLESv2.lib")

#else

#define GLEW_STATIC 1
#include "GL/glew.h"
#ifdef WIN32
#include "GL/wglew.h"
#endif // WIN32

#pragma comment(lib, "OpenGL32.lib")

#endif // IVY_GL_ES

#endif _// _IVYGL_H_

