///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2010-2011, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DXWINDOW_H_
#define _DXWINDOW_H_

#include "IvyObject.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
/// DxWindow Class
///
/// @brief
///     This class encapsulates a Win32 window class for tests when run
///     as an application.
///////////////////////////////////////////////////////////////////////////////////////////////////
class DxWindow :
    public IvyObject
{
public:
    // static method used for instantiation of DxWindow
    static DxWindow* Create(UINT windowWidth, UINT windowHeight);
    VOID Destroy();

    /// Process a window message, if any
    BOOL ProcessMsg(BOOL* pQuit);

    VOID DisplayWindow();

    /// Get a handle to the window
    HWND GetHwnd() const { return m_hWindow; }

    void Resize();
    
    Rect GetDrawableArea();

private:
    DxWindow();
    virtual ~DxWindow();

    // disallow use of copy constructor
    DxWindow(const DxWindow& window);
    // disallow use of assignment operator
    DxWindow& operator=(const DxWindow& window);

    // Private Member Data

    HWND m_hWindow;         ///< a handle to the window this object represents

    LPCSTR m_pClassName;     ///< the window class' name
    LPCSTR m_pWindowName;    ///< the window's name

    RECT m_clientRect;  ///< The drawable region of the window

};

#endif // _DXWINDOW_H_