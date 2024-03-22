#pragma once

#include <stdexcept>
#include <string>

namespace GraphC {

#define UsefulException(MSG) UsefulException_(MSG, __FILE__, __func__, __LINE__)

class UsefulException_ : public std::exception {
    std::string message;
    std::string file;
    std::string function;
    int line_n;
    std::string complete_text;

public:
    UsefulException_(std::string msg, std::string _file, std::string _func, int line_n);

    virtual char const* what() const noexcept;
};

}
