#include "Window.hpp"

#include <sstream>
#include <exception>
#include <stdexcept>

#include <sstream>

namespace fatpound::win
{
    // Window

    Window::Window(int width, int height, const char* name)
        :
        width_(width),
        height_(height)
    {
        RECT wr = {};
        wr.left = 100;
        wr.right = width_ + wr.left;
        wr.top = 100;
        wr.bottom = height_ + wr.top;

        if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0)
        {
            throw FHWND_LAST_EXCEPT();
        }

        hWnd_ = CreateWindow(
            WindowClass_::GetName(),
            name,
            WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            wr.right - wr.left,
            wr.bottom - wr.top,
            nullptr,
            nullptr,
            WindowClass_::GetInstance(),
            this
        );

        if (hWnd_ == nullptr)
        {
            throw FHWND_LAST_EXCEPT();
        }
        else
        {
            ShowWindow(hWnd_, /*SW_SHOW*/ SW_SHOWDEFAULT);
        }

        pGfx_ = std::make_unique<Graphics>(hWnd_, width_, height_);
    }
    Window::~Window()
    {
        DestroyWindow(hWnd_);
    }

    std::optional<int> Window::ProcessMessages() noexcept
    {
        MSG msg;

        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                return static_cast<int>(msg.wParam);
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        return {};
    }

    void Window::SetTitle(const std::string& title)
    {
        if (SetWindowText(hWnd_, title.c_str()) == 0)
        {
            throw FHWND_LAST_EXCEPT();
        }
    }

    LRESULT CALLBACK Window::HandleMsgSetup_(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
    {
        if (msg == WM_NCCREATE)
        {
            const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
            Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);

            SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
            SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk_));

            return pWnd->HandleMsg_(hWnd, msg, wParam, lParam);
        }

        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    LRESULT CALLBACK Window::HandleMsgThunk_(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
    {
        Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

        return pWnd->HandleMsg_(hWnd, msg, wParam, lParam);
    }
    LRESULT Window::HandleMsg_(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
    {
        switch (msg)
        {
        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;

        case WM_KILLFOCUS:
            kbd.ClearKeyStateBitset_();
            break;

            /******** KEYBOARD MESSAGES ********/
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            if (!(lParam & 0x40000000) || kbd.AutoRepeatIsEnabled())
            {
                kbd.OnKeyPressed_(static_cast<unsigned char>(wParam));
            }
            break;

        case WM_KEYUP:
        case WM_SYSKEYUP:
            kbd.OnKeyReleased_(static_cast<unsigned char>(wParam));
            break;

        case WM_CHAR:
            kbd.OnChar_(static_cast<unsigned char>(wParam));
            break;
            /****** END KEYBOARD MESSAGES ******/


            /********* MOUSE  MESSAGES *********/
        case WM_MOUSEMOVE:
        {
            const POINTS pt = MAKEPOINTS(lParam);

            if (pt.x >= 0 && pt.x < width_ && pt.y >= 0 && pt.y < height_)
            {
                mouse.OnMouseMove_(pt.x, pt.y);

                if (!mouse.IsInWindow())
                {
                    SetCapture(hWnd);
                    mouse.OnMouseEnter_();
                }
            }
            else
            {
                if (wParam & (MK_LBUTTON | MK_RBUTTON))
                {
                    mouse.OnMouseMove_(pt.x, pt.y);
                }
                else
                {
                    ReleaseCapture();
                    mouse.OnMouseLeave_();
                }
            }
        }
        break;

        case WM_LBUTTONDOWN:
            mouse.OnLeftPressed_();
            break;

        case WM_LBUTTONUP:
            mouse.OnLeftReleased_();
            break;

        case WM_RBUTTONDOWN:
            mouse.OnRightPressed_();
            break;

        case WM_RBUTTONUP:
            mouse.OnRightReleased_();
            break;

        case WM_MBUTTONDOWN:
            mouse.OnWheelPressed_();
            break;

        case WM_MBUTTONUP:
            mouse.OnWheelReleased_();
            break;

        case WM_MOUSEWHEEL:
            mouse.OnWheelDelta_(GET_WHEEL_DELTA_WPARAM(wParam));
            break;
            /******* END. MOUSE MESSAGES *******/


        default:
            break;
        }

        return DefWindowProc(hWnd, msg, wParam, lParam);
    }


    // Exception

    std::string Window::Exception::TranslateErrorCode(HRESULT hresult) noexcept
    {
        char* pMsgBuf = nullptr;

        const DWORD nMsgLen = FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr,
            hresult,
            MAKELANGID(
                LANG_NEUTRAL,
                SUBLANG_DEFAULT
            ),
            reinterpret_cast<LPSTR>(&pMsgBuf),
            0,
            nullptr
        );

        if (nMsgLen == 0)
        {
            return "Unidentified error code";
        }

        std::string errorString = pMsgBuf;

        LocalFree(pMsgBuf);

        return errorString;
    }


    // HrException

    Window::HrException::HrException(int line, const char* file, HRESULT hresult) noexcept
        :
        Exception(line, file),
        hresult_(hresult)
    {

    }

    HRESULT Window::HrException::GetErrorCode() const noexcept
    {
        return hresult_;
    }

    std::string Window::HrException::GetErrorDescription() const noexcept
    {
        return Exception::TranslateErrorCode(hresult_);
    }

    const char* Window::HrException::what() const noexcept
    {
        std::ostringstream oss;

        oss << GetType() << std::endl
            << "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
            << std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
            << "[Description] " << GetErrorDescription() << std::endl
            << GetOriginString();

        whatBuffer_ = oss.str();

        return whatBuffer_.c_str();
    }
    const char* Window::HrException::GetType() const noexcept
    {
        return "Fat Window Exception";
    }


    // NoGfxException

    const char* Window::NoGfxException::GetType() const noexcept
    {
        return "Fat Window Exception [No Graphics]";
    }


    // WindowClass_

    Window::WindowClass_ Window::WindowClass_::wndClass_;

    Window::WindowClass_::WindowClass_()
        :
        hInst_(GetModuleHandle(nullptr))
    {
        WNDCLASSEX wc = { 0 };

        wc.cbSize = sizeof(wc);
        wc.style = CS_OWNDC;
        wc.lpfnWndProc = HandleMsgSetup_;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = GetInstance();
        wc.hIcon = nullptr;
        wc.hCursor = nullptr;
        wc.hbrBackground = nullptr;
        wc.lpszMenuName = nullptr;
        wc.lpszClassName = GetName();
        wc.hIconSm = nullptr;

        RegisterClassEx(&wc);
    }
    Window::WindowClass_::~WindowClass_()
    {
        UnregisterClass(wndClassName_, GetInstance());
    }

    HINSTANCE Window::WindowClass_::GetInstance() noexcept
    {
        return wndClass_.hInst_;
    }

    const char* Window::WindowClass_::GetName() noexcept
    {
        return wndClassName_;
    }
}