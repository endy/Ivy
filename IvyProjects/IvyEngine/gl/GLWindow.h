///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2012, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef _GLWINDOW_H_
#define _GLWINDOW_H_

#include "IvyObject.h"

/**
***************************************************************************************************
* @brief This class encapsulates a Win32 window class for tests when run
*        as an application.
***************************************************************************************************
*/

class GLWindow :
    public IvyObject
{
public:
    // static method used for instantiation of DxWindow
    static GLWindow* Create(UINT windowWidth, UINT windowHeight);
    VOID Destroy();

    // Process a window message, if any
    BOOL ProcessMsg(BOOL* pQuit);

    VOID Show();

    /// Get a handle to the window
    HWND GetHwnd() const { return m_hWindow; }

    void Resize();
    
    Rect GetDrawableArea();

private:
    GLWindow();
    virtual ~GLWindow();

    // disallow use of copy constructor
    GLWindow(const GLWindow& window);
    // disallow use of assignment operator
    GLWindow& operator=(const GLWindow& window);

    // Private Member Data

    HWND m_hWindow;         ///< a handle to the window this object represents

    LPCSTR m_pClassName;     ///< the window class' name
    LPCSTR m_pWindowName;    ///< the window's name

    RECT m_clientRect;  ///< The drawable region of the window

};

#endif // _GLWINDOW_H_