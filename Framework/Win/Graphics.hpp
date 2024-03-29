#pragma once

#include "FatWin.hpp"

#include "../Color/Color.hpp"

#include "Dxerr/DxgiInfoManager.hpp"

#include "Exception/FatException.hpp"

#include "Macro/GraphicsThrowMacros.hpp"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <wrl.h>

#include <vector>
#include <array>
#include <memory>
#include <random>

namespace fatpound::win
{
    class Graphics final
    {
    public:
        Graphics() = delete;

        Graphics(HWND hWnd, int width, int height);

        Graphics(const Graphics& src) = delete;
        Graphics(Graphics&& src) = delete;
        Graphics& operator = (const Graphics& src) = delete;
        Graphics& operator = (Graphics&& src) = delete;
        ~Graphics() = default;


    public:
        class Exception : public exception::FatException
        {
            using FatException::FatException;
        };
        class HrException : public Exception
        {
        public:
            HrException(int line, const char* file, HRESULT hresult, std::vector<std::string> infoMsgs = { }) noexcept;

        public:
            HRESULT GetErrorCode() const noexcept;

            std::string GetErrorString() const noexcept;
            std::string GetErrorDescription() const noexcept;
            std::string GetErrorInfo() const noexcept;

            const char* what() const noexcept override;
            const char* GetType() const noexcept override;

        private:
            HRESULT hresult_;

            std::string info_;
        };
        class InfoException : public Exception
        {
        public:
            InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept;

        public:
            std::string GetErrorInfo() const noexcept;

            const char* what() const noexcept override;
            const char* GetType() const noexcept override;

        private:
            std::string info_;
        };
        class DeviceRemovedException : public HrException
        {
            using HrException::HrException;

        public:
            const char* GetType() const noexcept override;

        private:
            std::string reason_;
        };


    public:
        void BeginFrame();
        void EndFrame();
        void ClearBuffer(float red, float green, float blue) noexcept;


    protected:


    private:
#ifndef NDEBUG
        dxerr::DxgiInfoManager infoManager_;
#endif

        Microsoft::WRL::ComPtr<ID3D11Device> pDevice_ = nullptr;
        Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain_ = nullptr;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext_ = nullptr;
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget_ = nullptr;
        Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV_ = nullptr;

        const int width_;
        const int height_;
    };

}