#include "UsefulException.hpp"
#include <sstream>

namespace GraphC {

UsefulException_::UsefulException_(std::string _message, std::string _file, std::string _func, int _line_n)
    : message(_message), file(_file), function(_func), line_n(_line_n)
{
    std::stringstream ss;
    ss<<message<<'\n';
    ss<<file<<':'<<_line_n<<" in "<<function;
    complete_text = ss.str();
}

char const* UsefulException_::what() const noexcept {
    return complete_text.c_str();
}

}
