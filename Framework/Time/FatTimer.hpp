#pragma once

#include <chrono>

namespace fatpound::time
{
    class FatTimer final
    {
    public:
        FatTimer();


    public:
        float Mark() noexcept;
        float Peek() const noexcept;


    protected:


    private:
        std::chrono::steady_clock::time_point last_;
    };
}