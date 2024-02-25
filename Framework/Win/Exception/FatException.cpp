#include "FatException.hpp"

#include <sstream>

namespace fatpound::win::exception
{
    FatException::FatException(int line_num, const char* file_name) noexcept
        :
        linenum_(line_num),
        filename_(file_name)
    {

    }

    std::string FatException::GetOriginString() const noexcept
    {
        std::ostringstream oss;

        oss << "[File] : " << filename_ << '\n'
            << "[Line] : " << linenum_;

        return oss.str();
    }
    std::string FatException::GetFileName() const noexcept
    {
        return filename_;
    }

    const char* FatException::GetType() const noexcept
    {
        return "Fat Exception";
    }
    const char* FatException::what() const noexcept
    {
        std::ostringstream oss;

        oss << GetType() << '\n'
            << GetOriginString();

        whatBuffer_ = oss.str();

        return whatBuffer_.c_str();
    }

    int FatException::GetLineNum() const noexcept
    {
        return linenum_;
    }
}