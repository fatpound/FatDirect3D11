#pragma once

#include <cstdint>

namespace fatpound::colors
{
    class Color final
    {
    public:
        constexpr Color() = default;

        constexpr Color(unsigned char r, unsigned char g, unsigned char b, unsigned char alpha = 0xFFui8)
        {
            dword = (static_cast<uint32_t>(alpha) << 24u)
                | (static_cast<uint32_t>(r) << 16u)
                | (static_cast<uint32_t>(g) << 8u)
                | (static_cast<uint32_t>(b));
        }
        constexpr Color(int r, int g, int b)
            :
            Color(static_cast<unsigned char>(r), static_cast<unsigned char>(g), static_cast<unsigned char>(b))
        {

		}
        constexpr Color(uint32_t dw)
            :
            dword(dw | 0xFF'00'00'00u)
        {

		}
        constexpr Color(Color col, unsigned char alpha)
            :
            Color((static_cast<uint32_t>(alpha) << 24u) | col.dword)
        {

		}

        constexpr Color(const Color& src) noexcept
            :
            dword(src.dword)
        {

        }
        constexpr Color(Color&& src) noexcept = default;
        constexpr Color& operator = (const Color& src) noexcept
        {
            dword = src.dword;

            return *this;
        }
        constexpr Color& operator = (Color&& src) noexcept = default;
        constexpr ~Color() noexcept = default;



    public:
        void SetAlpha(unsigned char x)
        {
            dword = (dword & 0x00'FF'FF'FFu) | (uint32_t(x) << 24u);
        }
        void SetR(unsigned char r)
        {
            dword = (dword & 0xFF'00'FF'FFu) | (uint32_t(r) << 16u);
        }
        void SetG(unsigned char g)
        {
            dword = (dword & 0xFF'FF'00'FFu) | (uint32_t(g) << 8u);
        }
        void SetB(unsigned char b)
        {
            dword = (dword & 0xFF'FF'FF'00u) | uint32_t(b);
        }

        constexpr unsigned char GetAlpha() const
        {
            return dword >> 24u;
        }
        constexpr unsigned char GetR()     const
        {
            return (dword >> 16u) & 0xFFu;
        }
        constexpr unsigned char GetG()     const
        {
            return (dword >> 8u) & 0xFFu;
        }
        constexpr unsigned char GetB()     const
        {
            return dword & 0xFFu;
        }

        bool operator == (const Color src) const
        {
            return dword == src.dword;
        }
		bool operator != (const Color src) const
		{
			return ! (dword == src.dword);
		}


    public:
        uint32_t dword = 0xFF'FF'FF'FFu;


    protected:


    private:
    };


    static constexpr Color MakeRGB(unsigned char r, unsigned char g, unsigned char b)
    {
        return (r << 16u) | (g << 8u) | b;
    }

    static constexpr Color Black = MakeRGB(0u, 0u, 0u);
    static constexpr Color Gray = MakeRGB(128u, 128u, 128u);
    static constexpr Color LightGray = MakeRGB(192u, 192u, 192u);
    static constexpr Color White = MakeRGB(255u, 255u, 255u);
	
    static constexpr Color Red = MakeRGB(255u, 0u, 0u);
    static constexpr Color Green = MakeRGB(0u, 255u, 0u);
    static constexpr Color Blue = MakeRGB(0u, 0u, 255u);
    static constexpr Color Yellow = MakeRGB(255u, 255u, 0u);
    static constexpr Color Cyan = MakeRGB(0u, 255u, 255u);
    static constexpr Color Magenta = MakeRGB(255u, 0u, 255u);
}