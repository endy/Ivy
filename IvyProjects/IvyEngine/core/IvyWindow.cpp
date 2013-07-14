///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2012, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "IvyWindow.h"
#include "SystemEventDispatcher.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
/// WndProc
///
/// @brief
///     Message handler for the window
/// @return
///     LRESULT
///////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WndProc(
    HWND hWnd,          ///< A handle to the window
    UINT Msg,           ///< A message
    WPARAM wParam,      ///< wParam, used by the Windows OS
    LPARAM lParam)      ///< lParam, used by the Windows OS
{
    Event* pEvent = NULL;
    EventDataMouseButton mouseData;
    EventDataKey keyData;

    switch(Msg)
    {
        case WM_CLOSE:
            DestroyWindow(hWnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        case WM_SIZE:
            pEvent = new EventWindowResize();
            break;
        case WM_MOUSEMOVE:
            pEvent = new EventMouseMove();
            break;
        case WM_LBUTTONDOWN:
            mouseData.button = MouseLeft;
            pEvent = new EventMouseButtonDown(mouseData);
            break;
        case WM_LBUTTONUP:
            mouseData.button = MouseLeft;
            pEvent = new EventMouseButtonUp(mouseData);
            break;
        case WM_LBUTTONDBLCLK:
            mouseData.button = MouseLeft;
            pEvent = new EventMouseDoubleClick(mouseData);
            break;
        case WM_RBUTTONDOWN:
            mouseData.button = MouseRight;
            pEvent = new EventMouseButtonDown(mouseData);
            break;
        case WM_RBUTTONUP:
            mouseData.button = MouseRight;
            pEvent = new EventMouseButtonUp(mouseData);
            break;
        case WM_RBUTTONDBLCLK:
            mouseData.button = MouseRight;
            pEvent = new EventMouseDoubleClick(mouseData);
            break;
        case WM_KEYDOWN:
            if ((Key_0 <= wParam && wParam <= Key_9) ||
                (Key_A <= wParam && wParam <= Key_Z))
            {
                keyData.key = (KeyboardKey)wParam;
                pEvent = new EventKeyDown(keyData);
            }
            break;
        case WM_KEYUP:
            if ((Key_0 <= wParam && wParam <= Key_9) ||
                (Key_A <= wParam && wParam <= Key_Z))
            {
                keyData.key = (KeyboardKey)wParam;
                pEvent = new EventKeyUp(keyData);
            }
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
/// IvyWindow::IvyWindow
///
/// @brief
///     Constructor
/// @return
///     N/A
///////////////////////////////////////////////////////////////////////////////////////////////////
IvyWindow::IvyWindow()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// IvyWindow::~IvyWindow
///
/// @brief
///     Destructor
/// @return
///     N/A
///////////////////////////////////////////////////////////////////////////////////////////////////
IvyWindow::~IvyWindow()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// IvyWindow::Create
///
/// @brief
///     A static method used for instantiation of IvyWindow
/// @return
///     A pointer to a newly created IvyWindow instance
///////////////////////////////////////////////////////////////////////////////////////////////////
IvyWindow* IvyWindow::Create(UINT clientWidth, UINT clientHeight)
{
    IvyWindow* pNewWindow = new IvyWindow();

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
/// IvyWindow::Destroy
///
/// @brief
///     Deletes the instance
/// @return
///     N/A
///////////////////////////////////////////////////////////////////////////////////////////////////
VOID IvyWindow::Destroy()
{
    delete this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// IvyWindow::Show
///
/// @brief
///     Displays the window
/// @return
///     N/A
///////////////////////////////////////////////////////////////////////////////////////////////////
VOID IvyWindow::Show()
{
    // Display the window to the user
    ShowWindow(m_hWindow, SW_SHOWNORMAL);
    UpdateWindow(m_hWindow);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// IvyWindow::ProcessMsg
///
/// @brief
///     Processes all outstanding window messages
/// @return
///     TRUE if a message was processed, FALSE if it was not
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL IvyWindow::ProcessMsg(
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

///////////////////////////////////////////////////////////////////////////////////////////////////
/// IvyWindow::Resize
///
/// @brief
///     
/// @return
///     N/A
///////////////////////////////////////////////////////////////////////////////////////////////////
void IvyWindow::Resize()
{
    GetClientRect(m_hWindow, &m_clientRect);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// IvyWindow::GetDrawableArea
///
/// @brief
///     
/// @return
///     N/A
///////////////////////////////////////////////////////////////////////////////////////////////////
Rect IvyWindow::GetDrawableArea()
{
    Rect area;
    area.top = m_clientRect.top;
    area.left = m_clientRect.left;
    area.right = m_clientRect.right;
    area.bottom = m_clientRect.bottom;

    return area;
}


