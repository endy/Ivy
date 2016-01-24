///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2010-2012, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef _IVYAPPGL_H_
#define _IVYAPPGL_H_

#include "IvyApp.h"
#include "IvyGL.h"
#include "IvyWindow.h"

using namespace Ivy;

void GLCallBack(
	GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar *message,
	void *userParam)
{
	std::cout << source << " " << type << " " << id << " " << severity << " " << length << " " << message << std::endl;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// IvyApp::InitGL2
///
/// @brief
///     Initializes an OpenGL 2.x context
/// @return
///     TRUE if success, FALSE otherwise
///////////////////////////////////////////////////////////////////////////////////////////////////
bool IvyApp::InitGL2()
{
#if !(IVY_GL_ES)
    IVY_PRINT("GLTestApp OpenGL 2.0 Path");

    static PIXELFORMATDESCRIPTOR pfd =              // pfd Tells Windows How We Want Things To Be
    {
        sizeof(PIXELFORMATDESCRIPTOR),              // Size Of This Pixel Format Descriptor
        1,                                          // Version Number
        PFD_DRAW_TO_WINDOW |                        // Format Must Support Window
        PFD_SUPPORT_OPENGL |                        // Format Must Support OpenGL
        PFD_DOUBLEBUFFER,                           // Must Support Double Buffering
        PFD_TYPE_RGBA,                              // Request An RGBA Format
        32,                                         // Select Our Color Depth
        0, 0, 0, 0, 0, 0,                           // Color Bits Ignored
        0,											// No Alpha Buffer
        0,											// Shift Bit Ignored
        0,											// No Accumulation Buffer
        0, 0, 0, 0,									// Accumulation Bits Ignored
        32,											// 16Bit Z-Buffer (Depth Buffer)  
        8,											// No Stencil Buffer
        0,											// No Auxiliary Buffer
        PFD_MAIN_PLANE,								// Main Drawing Layer
        0,											// Reserved
        0, 0, 0										// Layer Masks Ignored
    };

    m_hDC = GetDC(m_pWindow->GetHwnd());


    int PixelFormat = ChoosePixelFormat(m_hDC,&pfd);
    BOOL setPixFmt = SetPixelFormat(m_hDC, PixelFormat, &pfd);

    m_hGLRC = wglCreateContext(m_hDC);
    wglMakeCurrent(m_hDC, m_hGLRC);

    if( glewInit() != GLEW_OK)
    {
        exit(1);
    }


    const GLubyte* pString = glGetString(GL_VERSION);

    GLint majorVersion, minorVersion;
    glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
    glGetIntegerv(GL_MINOR_VERSION, &minorVersion);

    const GLubyte* pExtension = glGetString(GL_EXTENSIONS);
    IvyLog(reinterpret_cast<const char*>(pExtension));
#endif // !(IVY_GL_ES)

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// IvyApp::InitGL4
///
/// @brief
///     Initializes an OpenGL 4.x context
/// @return
///     TRUE if success, FALSE otherwise
///////////////////////////////////////////////////////////////////////////////////////////////////
bool IvyApp::InitGL4()
{
#if !(IVY_GL_ES)
    static PIXELFORMATDESCRIPTOR pfd =              // pfd Tells Windows How We Want Things To Be
    {
        sizeof(PIXELFORMATDESCRIPTOR),              // Size Of This Pixel Format Descriptor
        1,                                          // Version Number
        PFD_DRAW_TO_WINDOW |                        // Format Must Support Window
        PFD_SUPPORT_OPENGL |                        // Format Must Support OpenGL
        PFD_DOUBLEBUFFER,                           // Must Support Double Buffering
        PFD_TYPE_RGBA,                              // Request An RGBA Format
        32,                                         // Select Our Color Depth
        0, 0, 0, 0, 0, 0,                           // Color Bits Ignored
        0,											// No Alpha Buffer
        0,											// Shift Bit Ignored
        0,											// No Accumulation Buffer
        0, 0, 0, 0,									// Accumulation Bits Ignored
        32,											// 16Bit Z-Buffer (Depth Buffer)  
        8,											// No Stencil Buffer
        0,											// No Auxiliary Buffer
        PFD_MAIN_PLANE,								// Main Drawing Layer
        0,											// Reserved
        0, 0, 0										// Layer Masks Ignored
    };

    m_hDC = GetDC(m_pWindow->GetHwnd());

    int PixelFormat = ChoosePixelFormat(m_hDC,&pfd);
    BOOL setPixFmt = SetPixelFormat(m_hDC, PixelFormat, &pfd);

    HGLRC tempGLRC = wglCreateContext(m_hDC);
    wglMakeCurrent(m_hDC, tempGLRC);

    if( glewInit() != GLEW_OK)
    {
        exit(1);
    }

    if (GLEW_VERSION_4_1 == 0)
    {
        exit(1);
    }

    ///@todo detect CreateContextAttribsARB 

    int attribs[] =
    {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 1,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB, // | WGL_CONTEXT_DEBUG_BIT_ARB,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
        0
    };

    m_hGLRC = wglCreateContextAttribsARB(m_hDC,0, attribs);

    wglMakeCurrent(NULL, NULL); // can't we just make current on the context we just created?
    wglDeleteContext(tempGLRC);

    wglMakeCurrent(m_hDC, m_hGLRC);

	//  Error Setup

	glDebugMessageCallbackARB((GLDEBUGPROCARB)&GLCallBack, NULL);

#endif // !(IVY_GL_ES)

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// IvyApp::InitGLES2
///
/// @brief
///     Initializes an OpenGL ES 2.0 context
/// @return
///     TRUE if success, FALSE otherwise
///////////////////////////////////////////////////////////////////////////////////////////////////
bool IvyApp::InitGLES2()
{
#if IVY_GL_ES

    IVY_PRINT("GLTestApp OpenGL ES 2 Path");

    GLenum err = glGetError();
    typedef EGLAPI EGLDisplay EGLAPIENTRY FPTR_EGL_GET_DISPLAY(EGLNativeDisplayType display_id);

    FPTR_EGL_GET_DISPLAY* pEglGetDisplay;

    pEglGetDisplay = &eglGetDisplay;

    EGLint majorVersion;
    EGLint minorVersion;

    m_eglDisplay = (*pEglGetDisplay)(EGL_DEFAULT_DISPLAY);
    m_eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (m_eglDisplay == EGL_NO_DISPLAY)
    {
        // unable to get display
        return;
    }


    if (!eglInitialize(m_eglDisplay, &majorVersion, &minorVersion))
    {
        // unable to init display
        return;
    }

    EGLint numConfigs;
    eglGetConfigs(m_eglDisplay, NULL, 0, &numConfigs);

    EGLConfig* pConfigs = new EGLConfig[numConfigs];

    eglGetConfigs(m_eglDisplay, pConfigs, numConfigs, &numConfigs);

    struct EglAttribList
    {
        EGLint attribType;
        const char* attribName;
    };

    EglAttribList attribList[] = 
    {
        { EGL_RED_SIZE, "EGL_RED_SIZE" },
        { EGL_GREEN_SIZE, "EGL_GREEN_SIZE" },
        { EGL_BLUE_SIZE, "EGL_BLUE_SIZE" },
        { EGL_LUMINANCE_SIZE, "EGL_LUMINANCE_SIZE" },
        { EGL_ALPHA_SIZE, "EGL_ALPHA_SIZE" },
        { EGL_DEPTH_SIZE, "EGL_DEPTH_SIZE" },
        { EGL_STENCIL_SIZE, "EGL_STENCIL_SIZE" },

        { EGL_MAX_PBUFFER_WIDTH, "EGL_MAX_PBUFFER_WIDTH" },
        { EGL_MAX_PBUFFER_HEIGHT, "EGL_MAX_PBUFFER_HEIGHT" },
        { EGL_MAX_PBUFFER_PIXELS, "EGL_MAX_PBUFFER_PIXELS" },

        { EGL_SAMPLES, "EGL_SAMPLES" },
        { EGL_SAMPLE_BUFFERS, "EGL_SAMPLE_BUFFERS" }
    };

    const int AttribListSize = sizeof(attribList) / sizeof(EglAttribList);

    for (int configIdx = 0; configIdx < numConfigs; ++configIdx)
    {
        std::cout << "Config " << configIdx << ":" << std::endl;

        EGLint attribValue;
        for (int attribIdx = 0; attribIdx < AttribListSize; ++attribIdx)
        {
            eglGetConfigAttrib(m_eglDisplay, pConfigs[0], attribList[attribIdx].attribType, &attribValue);
            std::cout << "\t" << attribList[attribIdx].attribName << "=" << attribValue << std::endl;
        }
    }

    const EGLint windowAttribList[] = 
    {
        EGL_RENDER_BUFFER, EGL_BACK_BUFFER,
        EGL_NONE
    };

    m_eglSurface = eglCreateWindowSurface(m_eglDisplay, pConfigs[0], m_pWindow->GetHwnd(), NULL); //, &windowAttribList[0]);

    if (m_eglSurface == EGL_NO_SURFACE)
    {
        // no surface
        switch (eglGetError())
        {
        case EGL_BAD_NATIVE_WINDOW:
            break;
        }
    }

    EGLint contextAttribs[] =
    {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE,
    };

    m_eglContext = eglCreateContext(m_eglDisplay, pConfigs[0], EGL_NO_CONTEXT, NULL); // &contextAttribs[0]);

    eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext);

    // Dont syncronize with the display by default
    eglSwapInterval(m_eglDisplay, 0);

    delete [] pConfigs;
    pConfigs = NULL;

    m_useEgl = TRUE;

#endif // IVY_GL_ES

    return true;
}

void IvyApp::IvySwapBuffers()
{
#if IVY_GL_ES
    if (m_useEgl)
    {
        eglSwapBuffers(m_eglDisplay, m_eglSurface);
    }
    else
#endif // IVY_GL_ES
    {
        SwapBuffers(m_hDC);
    }
}

#endif // _IVYAPPGL_H_

