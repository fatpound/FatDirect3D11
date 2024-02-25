#include "App.hpp"

#include "Math/FatMath.hpp"

#include <cmath>

#include <memory>
#include <numbers>
#include <optional>

namespace dx = DirectX;

namespace fatpound::dx11
{
    App::App()
        :
        wnd_(1366, 768, "FatDirect3D11 Window"),
        gfx_(wnd_.Gfx())
    {

    }

    App::~App() noexcept
    {

    }

    int App::Go()
    {
        std::optional<int> errorCode;

        while (true)
        {
            errorCode = fatpound::win::Window::ProcessMessages();

            if (errorCode)
            {
                return *errorCode;
            }

            gfx_.BeginFrame();
            DoFrame_();
            gfx_.EndFrame();
        }
    }

    void App::DoFrame_()
    {

    }
}