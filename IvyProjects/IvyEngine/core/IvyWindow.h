///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2012, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _IVYWINDOW_H_
#define _IVYWINDOW_H_

#include "IvyObject.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
/// IvyWindow Class
///////////////////////////////////////////////////////////////////////////////////////////////////
class IvyWindow : public IvyObject
{
public:
    // static method used for instantiation of DxWindow
    static IvyWindow* Create(UINT windowWidth, UINT windowHeight);
    VOID Destroy();

    /// Process a window message, if any
    BOOL ProcessMsg(BOOL* pQuit);

    VOID Show();

    /// Get a handle to the window
    HWND GetHwnd() const { return m_hWindow; }

    void Resize();

    Rect GetDrawableArea();
protected:

    IvyWindow();
    virtual ~IvyWindow();

    // disallow use of copy constructor
    IvyWindow(const IvyWindow& window);
    // disallow use of assignment operator
    IvyWindow& operator=(const IvyWindow& window);

    HWND m_hWindow;         ///< a handle to the window this object represents

    LPCSTR m_pClassName;    ///< the window class' name
    LPCSTR m_pWindowName;   ///< the window's name

    RECT m_clientRect;      ///< The drawable region of the window
};

#endif // _IVYWINDOW_H_

