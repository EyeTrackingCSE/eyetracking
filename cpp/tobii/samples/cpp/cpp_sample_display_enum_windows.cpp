/*
COPYRIGHT 2018-2020  - PROPERTY OF TOBII AB
-------------------------------------
2018-2020 TOBII AB - KARLSROVAGEN 2D, DANDERYD 182 53, SWEDEN - All Rights Reserved.

NOTICE:  All information contained herein is, and remains, the property of Tobii AB and its suppliers, if any.
The intellectual and technical concepts contained herein are proprietary to Tobii AB and its suppliers and may be
covered by U.S.and Foreign Patents, patent applications, and are protected by trade secret or copyright law.
Dissemination of this information or reproduction of this material is strictly forbidden unless prior written
permission is obtained from Tobii AB.
*/

#include <interaction_lib/InteractionLib.h>
#include <interaction_lib/misc/InteractionLibPtr.h>
#include <windows.h>
#include <vector>
#include <functional>
#define DISPLAY_ENUMERATION_HELPER_IMPLEMENTATION
#include <DisplayEnumerationHelper.h>

// This a simple C++ wrapper for a Win32 HWND window representation.
// Note that there is no real error handling, since its main purpose
// is to support the Interaction Library SDK C++ sample that shows how
// to use the display enumeration helper functions.
class Window final
{
public:
    // Window creation might trigger all of the OnPaint, OnMove and OnSize events.
    void Create();

    // When a window is repainted, the OnPaint event is triggered.
    void Repaint();

    // The int& out parameter tells the window what Win32 system color index it should
    // use when painting its client area, for instance COLOR_WINDOW or COLOR_HIGHLIGHT.
    std::function<void(int&)> OnPaint;

    // The float parameters are the new x and y positions of the window client area
    // origo, relative to the primary screen origo.
    std::function<void(float, float)> OnMove;

    // The float parameters are the new width and height of the window client area.
    std::function<void(float, float)> OnSize;

private:
    // The window procedure handles messages that are needed to invoke the public events.
    static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

    HWND m_hwnd{};
};

void Window::Create()
{
    WNDCLASSA wc{};
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpfnWndProc = WindowProcedure;
    wc.lpszClassName = "TobiiInteractorWindow";

    RegisterClassA(&wc);

    CreateWindowA(wc.lpszClassName, "Interactor Window",
                  WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                  CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                  NULL, NULL, GetModuleHandle(nullptr), this);
}

void Window::Repaint()
{
    InvalidateRect(m_hwnd, NULL, FALSE);
}

LRESULT Window::WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    static Window* window{};

    switch (msg)
    {
        case WM_CREATE:
        {
            CREATESTRUCTA* cs{reinterpret_cast<CREATESTRUCTA*>(lp)};
            window = static_cast<Window*>(cs->lpCreateParams);
            window->m_hwnd = hwnd;

            return 0;
        }
        case WM_MOVE:
        {
            if (window->OnMove)
            {
                POINT offset{};
                ClientToScreen(hwnd, &offset);
                const float x = static_cast<float>(offset.x);
                const float y = static_cast<float>(offset.y);
                window->OnMove(x, y);
            }

            return 0;
        }
        case WM_SIZE:
        {
            if (window->OnSize)
            {
                RECT rect{};
                GetClientRect(hwnd, &rect);
                const float width = static_cast<float>(rect.right - rect.left);
                const float height = static_cast<float>(rect.bottom - rect.top);
                window->OnSize(width, height);
            }

            return 0;
        }
        case WM_ERASEBKGND:
        {
            return 1;
        }
        case WM_PAINT:
        {
            PAINTSTRUCT ps{};
            HDC hdc{BeginPaint(hwnd, &ps)};
            int sysColorIndex = COLOR_WINDOW;

            if (window->OnPaint)
                window->OnPaint(sysColorIndex);

            FillRect(hdc, &ps.rcPaint, GetSysColorBrush(sysColorIndex));
            EndPaint(hwnd, &ps);

            return 0;
        }
        case WM_DESTROY:
        {
            window->m_hwnd = nullptr;
            window = nullptr;

            PostQuitMessage(0);

            return 0;
        }
        default:
        {
            return DefWindowProc(hwnd, msg, wp, lp);
        }
    }
}

// This one way to implement a Win32 "message pump" that not only
// handles window input from the user and the system, but also
// integrates with an external "event source" - an eye tracker.
bool ProcessMessages()
{
    MSG msg{};

    while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))
    {
        // When the window is destroyed, it posts a quit message,
        // which is picked up here as "stop processing messages".
        if (msg.message == WM_QUIT)
        {
            return false;
        }

        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    return true;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    // create the interaction library
    IL::UniqueInteractionLibPtr intlib{IL::CreateInteractionLib(IL::FieldOfUse::Interactive)};

    // This is just a helper function to make the underlying C function
    // easier to use in a C++ context.
    auto GetMonitorDisplayAreas = []
    {
        std::vector<MonitorDisplayAreaA> areas{5}; // unusual to have more than 5 monitors...
        int found{};
        
        // This is the raw C display area enumeration helper function.
        // It will enumerate all displays on the windows machine, including their virtual 
        // screen bounds and display IDs, and put the results into the supplied array of structs.
        // The display IDs have the same formatting as are used on Tobii eyetrackers to 
        // identify which display they are attached to. 
        EnumerateDisplayAreasA(areas.data(), &found, static_cast<int>(areas.size()));
        areas.resize(found);
        return areas;
    };

    // When all displays are identified, pass their information to the Interaction Library.
    for (const auto& area : GetMonitorDisplayAreas())
    {
        // For each display we tell the Interaction Library
        // 1) the coordinate scaling we want to use by giving the size as we see it
        // 2) the size and origin in virtual screen coordinates, and 
        // 3) the ID of the display.
        // 
        // Interaction Library will use this information to map connected devices to display areas,
        // to transform all data streams when outputting them and to properly transform all added 
        // interactors to its internal, global coordinate space when performing gaze focus calculations.
        // 
        // Here we are using the native scaling (so first to parameters are the same as the virtual 
        // screen size) but we could have  used any scaling we like, eg (1.0, 1.0) for relative 
        // coordinates.
        intlib->CoordinateTransformAddOrUpdateDisplayArea(
            area.wVirtual, area.hVirtual,
            area.wVirtual, area.hVirtual,
            area.xVirtual, area.yVirtual,
            area.monitorId);
    }

    // All events must have handlers before the physical window is created, so that
    // any resizing, moving and painting done by the system during window creation
    // is picked up by our application logic.
    Window window;

    // By telling the Interaction Library to offset the coordinate system origin
    // so that it matches the location of the window client area, we can add and
    // update interactors using window-local coordinates.
    // The origin offset has to be in the virtual coordinate space we used when 
    // adding the display areas above.
    window.OnMove = [&] (float x, float y)
    {
        intlib->CoordinateTransformSetOriginOffset(x, y);
    };

    // In this sample we want the entire window client area to be an interactor
    // that changes color when it gets and loses gaze focus.
    window.OnSize = [&] (float width, float height)
    {
        // setup the window client area as an interactor
        intlib->BeginInteractorUpdates();
        intlib->AddOrUpdateInteractor(0, {0.0f, 0.0f, width, height}, 0.0f);
        intlib->CommitInteractorUpdates();
    };

    struct GazeFocusEventContext final
    {
        bool focused;
        Window& window;
    } gazeFocusEventContext{false, window};

    // Subscribe to gaze focus events. When the window client area (which is an
    // interactor) gets or loses gaze focus, the "focused" flag is set and then
    // the window is repainted.
    intlib->SubscribeGazeFocusEvents([](IL::GazeFocusEvent evt, void* context)
    {
        auto gazeFocusEventContext{static_cast<GazeFocusEventContext*>(context)};
        gazeFocusEventContext->focused = evt.hasFocus;
        gazeFocusEventContext->window.Repaint();
    }, &gazeFocusEventContext);

    // When the window repaints itself, it invokes this event to ask for the color to use.
    window.OnPaint = [&] (int& sysColorIndex)
    {
        sysColorIndex = gazeFocusEventContext.focused ? COLOR_HIGHLIGHT : COLOR_WINDOW;
    };

    // Now that all events and subscriptions are set up, the physical window can be created.
    window.Create();
    
    // Handle window events and update interaction library to trigger all callbacks
    while (ProcessMessages())
    {
        intlib->Update();
    }

    return 0;
}
