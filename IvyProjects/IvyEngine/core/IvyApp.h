///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2012, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _IVYAPP_H_
#define _IVYAPP_H_

#include "IvyObject.h"

#include "SystemEventDispatcher.h"
#include "IvyInput.h"
#include "IvyPerf.h"
#include "IvyConfig.h"

class DxUI;

namespace Ivy
{
    class IvyWindow;
    class IvyCamera;

    struct IvyAppDxData;
    struct IvyDxUIData;

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    /// IvyApp Class
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    class IvyApp :
        public IvyObject,
        public IEventReceiver
    {
    public:
        static bool Configure(unsigned int argc, const char** argv);

        static IvyConfigItemValue* QueryConfig(const char* pName);

        virtual void Destroy();

        virtual void Run();

        virtual void ReceiveEvent(const Event* pEvent);

    protected:
        IvyApp();
        virtual ~IvyApp();

        virtual bool Init();
        bool InitDX();

        bool InitGL2();
        bool InitGL4();
        bool InitGLES2();

        void IvySwapBuffers();

        void ProcessUpdates();
        BOOL ExitApp() { return m_exit; }

        const IvyGamepadState* GetGamepadState() { return &m_gamepad; }
        const KeyboardState* GetKeyboardState() { return &m_keys; }
        const MouseState* GetMouseState() { return &m_mouse; }

        bool DeinitDX();
        bool DeinitGL();

        void BeginFrame();
        void EndFrame();

        // dx functions
        void UpdateSwapChain();
        void DrawUI();

        IvyWindow* m_pWindow;

        ///@todo Refactor into IvyUI
        DxUI* m_pUI;    ///< DxUI for the user interface overlay
        IvyDxUIData* m_pUIData;

        Point2 m_mousePos;

        FLOAT m_fovY;           // FOV Y
        FLOAT m_nearZ;          // near plane
        FLOAT m_farZ;           // far plane
        UINT  m_screenWidth;    // viewport width
        UINT  m_screenHeight;   // viewport height

        static const UINT BufferCount;

        IvyCamera* m_pCamera;

        /// D3D Data
        IvyAppDxData* m_pDxData;

        /// OpenGL Member Data
        // Win32 handles
        HDC m_hDC;
        HGLRC m_hGLRC;

#if IVY_GL_ES
        // EGL & GLES
        EGLDisplay m_eglDisplay;
        EGLSurface m_eglSurface;
        EGLContext m_eglContext;
#endif // IVY_GL_ES

    private:
        IvyApp(const IvyApp& copy);               // Disallow copy constructor
        IvyApp& operator=(const IvyApp& copy);    // Disallow assignment operator

        void ProcessKeyboardEvent(const Event* pEvent);
        void ProcessMouseEvent(const Event* pEvent);

        FramerateTracker m_framerateTracker;

        MouseState m_mouse;
        KeyboardState m_keys;
        IvyGamepadState m_gamepad;

        BOOL m_exit;

        // Configuration
        static unsigned int ScreenWidth;
        static unsigned int ScreenHeight;

        static IvyConfigItem IvyAppConfig[];

    };

}

#endif // _IVYAPP_H_

