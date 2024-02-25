#pragma once

#include <exception>
#include <string>

namespace fatpound::win::exception
{
    class FatException : public std::exception
    {
    public:
        FatException(int line_num, const char* file_name) noexcept;


    public:
        std::string GetOriginString() const noexcept;
        std::string GetFileName() const noexcept;

        virtual const char* GetType() const noexcept;
        virtual const char* what() const noexcept override;

        int GetLineNum() const noexcept;


    protected:
        mutable std::string whatBuffer_;


    private:
        std::string filename_;

        int linenum_;
    };
}