#pragma once

#include "FatWin.hpp"
#include "Graphics.hpp"

#include "../Math/FatMath.hpp"

#include "Exception/FatException.hpp"

#include "IO/Keyboard.hpp"
#include "IO/Mouse.hpp"

#include "Macro/WindowThrowMacros.hpp"

#include <optional>
#include <memory>
#include <concepts>

namespace fatpound::win
{
    class Window final
    {
    public:
        Window() = delete;

        Window(int width, int height, const char* name);

        Window(const Window& src) = delete;
        Window(Window&& src) = delete;
        Window& operator = (const Window& src) = delete;
        Window& operator = (Window&& src) = delete;
        ~Window();


    public:
        class Exception : public exception::FatException
        {
            using FatException::FatException;

        public:
            static std::string TranslateErrorCode(HRESULT hresult) noexcept;

        protected:

        private:
        };
        class HrException : public Exception
        {
        public:
            HrException(int line, const char* file, HRESULT hresult) noexcept;

        public:
            HRESULT GetErrorCode() const noexcept;

            std::string GetErrorDescription() const noexcept;

            const char* what() const noexcept override;
            const char* GetType() const noexcept override;

        protected:

        private:
            HRESULT hresult_;
        };
        class NoGfxException : public Exception
        {
            using Exception::Exception;

        public:
            const char* GetType() const noexcept override;

        protected:

        private:
        };


    public:
        static std::optional<int> ProcessMessages() noexcept;

        __forceinline Graphics& Gfx()
        {
            if (pGfx_ == nullptr)
            {
                throw FHWND_NOGFX_EXCEPT();
            }

            return *pGfx_;
        }

        template <fatpound::math::Number N>
        __forceinline N GetWidth() const
        {
            return static_cast<N>(width_);
        }

        template <fatpound::math::Number N>
        __forceinline N GetHeight() const
        {
            return static_cast<N>(height_);
        }

        void SetTitle(const std::string& title);


    public:
        io::Keyboard kbd;
        io::Mouse mouse;


    protected:


    private:
        class WindowClass_ final
        {
        public:
            static HINSTANCE GetInstance() noexcept;

            static const char* GetName() noexcept;

        protected:

        private:
            WindowClass_();
            WindowClass_(const WindowClass_& src) = delete;
            WindowClass_(WindowClass_&& src) = delete;
            WindowClass_& operator = (const WindowClass_& src) = delete;
            WindowClass_& operator = (WindowClass_&& src) = delete;
            ~WindowClass_();

        private:
            HINSTANCE hInst_;

            static WindowClass_ wndClass_; // singleton class

            static constexpr const char* const wndClassName_ = "Fat Direct3D Engine Window";
        };


    private:
        static LRESULT CALLBACK HandleMsgSetup_(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
        static LRESULT CALLBACK HandleMsgThunk_(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
        LRESULT HandleMsg_(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;


    private:
        HWND hWnd_;

        std::unique_ptr<Graphics> pGfx_;

        int width_  = 0;
        int height_ = 0;
    };
}