#include "FatTimer.hpp"

using namespace std::chrono;

namespace fatpound::time
{
    FatTimer::FatTimer()
    {
        last_ = steady_clock::now();
    }

    float FatTimer::Mark() noexcept
    {
        const auto old = last_;
        last_ = steady_clock::now();

        const duration<float> frameTime = last_ - old;

        return frameTime.count();
    }
    float FatTimer::Peek() const noexcept
    {
        return duration<float>(steady_clock::now() - last_).count();
    }
}