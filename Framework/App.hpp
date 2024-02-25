#pragma once

#include "Time/FatTimer.hpp"

#include "Win/Window.hpp"

namespace fatpound::dx11
{
    class App final
    {
    public:
        App();
        App(const App& src) = delete;
        App(App&& src) = delete;
        App& operator = (const App& src) = delete;
        App& operator = (App&& src) = delete;
        ~App() noexcept;


    public:
        int Go();


    protected:


    private:
        void DoFrame_();
        

    private:
        fatpound::time::FatTimer timer_;

        fatpound::win::Window wnd_;
        fatpound::win::Graphics& gfx_;
    };
}