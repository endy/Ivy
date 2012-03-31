///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2010-2011, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "GLWindow.h"
#include "SystemEventDispatcher.h"

/**
***************************************************************************************************
*   WndProc
*
*   @brief
*       Message handler for the window
*   @return
*       LRESULT
***************************************************************************************************
*/
LRESULT CALLBACK WndProc(
    HWND hWnd,          ///< A handle to the window
    UINT Msg,           ///< A message
    WPARAM wParam,      ///< wParam, used by the Windows OS
    LPARAM lParam)      ///< lParam, used by the Windows OS
{
    Event* pEvent = NULL;

    switch(Msg)
    {
        case WM_DESTROY:
            PostQuitMessage(WM_QUIT);
            break;
        case WM_SIZE:
            pEvent = new EventWindowResize();
            break;
        case WM_MOUSEMOVE:
            pEvent = new EventMouseMove();
            break;
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_LBUTTONDBLCLK:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_RBUTTONDBLCLK:
            break;
        case WM_KEYDOWN:
            EventDataKeyDown data;
            switch(wParam)
            {
                case '1':
                    data.key = EKey1;
                    break;
                case '2':
                    data.key = EKey2;
                    break;
                case '3':
                    data.key = EKey3;
                    break;
                case '4':
                    data.key = EKey4;
                    break;
                case '5':
                    data.key = EKey5;
                    break;
                case '6':
                    data.key = EKey6;
                    break;
                case '7':
                    data.key = EKey7;
                    break;
                case '8':
                    data.key = EKey8;
                    break;
                case '9':
                    data.key = EKey9;
                    break;
                case '0':
                    data.key = EKey0;
                    break;
                case 'r':
                case 'R':
                    data.key = EKeyR;
                    break;
                case 'w':
                case 'W':
                    data.key = EKeyW;
                    break;
                case 'a':
                case 'A':
                    data.key = EKeyA;
                    break;
                case 's':
                case 'S':
                    data.key = EKeyS;
                    break;
                case 'd':
                case 'D':
                    data.key = EKeyD;
                    break;
            }
            pEvent = new EventKeyDown(data);
            break;
        default:
            return DefWindowProc(hWnd, Msg, wParam, lParam);
    }

    if (pEvent != NULL)
    {
        SystemEventDispatcher::Get()->Dispatch(pEvent);
        
        delete pEvent;
        pEvent = NULL;
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// GLWindow::GLWindow
///
/// @brief
///     Constructor
/// @return
///     N/A
///////////////////////////////////////////////////////////////////////////////////////////////////
GLWindow::GLWindow()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// GLWindow::~GLWindow
///
/// @brief
///     Destructor
/// @return
///     N/A
///////////////////////////////////////////////////////////////////////////////////////////////////
GLWindow::~GLWindow()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// GLWindow::Create
///
/// @brief
///     A static method used for instantiation of GLWindow
/// @return
///     A pointer to a newly created GLWindow instance
///////////////////////////////////////////////////////////////////////////////////////////////////
GLWindow* GLWindow::Create(UINT clientWidth, UINT clientHeight)
{
    GLWindow* pNewWindow = new GLWindow();

    pNewWindow->m_pClassName  = "IvyWindowClass";
    pNewWindow->m_pWindowName = "IvyEngine";

    WNDCLASSEX WndClsEx;

    HINSTANCE hInstance = GetModuleHandle(NULL);

    WndClsEx.hInstance      = hInstance;
    WndClsEx.lpfnWndProc    = WndProc;
    WndClsEx.cbSize         = sizeof(WNDCLASSEX);
    WndClsEx.style          = CS_HREDRAW | CS_VREDRAW;
    WndClsEx.cbClsExtra     = 0;
    WndClsEx.cbWndExtra     = 0;
    WndClsEx.hIcon          = LoadIcon(NULL, IDI_APPLICATION);
    WndClsEx.hCursor        = LoadCursor(NULL, IDC_ARROW);
    WndClsEx.hIconSm        = LoadIcon(NULL, IDI_APPLICATION);
    WndClsEx.hbrBackground  = (HBRUSH) GetStockObject(BLACK_BRUSH);
    WndClsEx.lpszMenuName   = NULL;
    WndClsEx.lpszClassName  = pNewWindow->m_pClassName;

    RegisterClassEx(&WndClsEx);

    RECT winRect;
    winRect.top = 0;
    winRect.bottom = clientHeight;
    winRect.left = 0;
    winRect.right = clientWidth;
    AdjustWindowRect(&winRect,WS_OVERLAPPEDWINDOW,FALSE);

    // Create the window object
    pNewWindow->m_hWindow = CreateWindow(pNewWindow->m_pClassName,
                                         pNewWindow->m_pWindowName,
                                         WS_OVERLAPPEDWINDOW,
                                         CW_USEDEFAULT,
                                         CW_USEDEFAULT,
                                         winRect.right - winRect.left,
                                         winRect.bottom - winRect.top,
                                         NULL,
                                         NULL,
                                         hInstance,
                                         NULL);

    if( !pNewWindow->m_hWindow )
    {
        // could not create a window
        delete pNewWindow;
        pNewWindow = NULL;
    }

    if (pNewWindow != NULL)
    {
        pNewWindow->Resize();
    }

    return pNewWindow;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// GLWindow::Destroy
///
/// @brief
///     Deletes the instance
/// @return
///     N/A
///////////////////////////////////////////////////////////////////////////////////////////////////
VOID GLWindow::Destroy()
{
    delete this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// GLWindow::Show
///
/// @brief
///     Displays the window
/// @return
///     N/A
///////////////////////////////////////////////////////////////////////////////////////////////////
VOID GLWindow::Show()
{
    // Display the window to the user
    ShowWindow(m_hWindow, SW_SHOWNORMAL);
    UpdateWindow(m_hWindow);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// GLWindow::ProcessMsg
///
/// @brief
///     Processes all outstanding window messages
/// @return
///     TRUE if a message was processed, FALSE if it was not
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL GLWindow::ProcessMsg(
    BOOL* pQuit) ///< [out] TRUE if a quit message was received, false otherwise
{
    BOOL processedMsg = FALSE;
    MSG  msg;

    while (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
    {
        processedMsg = TRUE;
        if(msg.message == WM_QUIT)
        {
            *pQuit = TRUE;
        }
        else if (msg.message == WM_SIZE)
        {
            
        }
        else
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return processedMsg;
}

void GLWindow::Resize()
{
    GetClientRect(m_hWindow, &m_clientRect);
}

Rect GLWindow::GetDrawableArea()
{
    Rect area;
    area.top = m_clientRect.top;
    area.left = m_clientRect.left;
    area.right = m_clientRect.right;
    area.bottom = m_clientRect.bottom;

    return area;
}