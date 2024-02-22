#pragma once

#include "../UsefulException.hpp"

namespace GraphC::gui {

#define SDLException() SDLException_(__FILE__, __func__, __LINE__)

class SDLException_ : UsefulException_ {
public:
    SDLException_(std::string _file, std::string _func, int line_n);
};

#define SDL_TRY_ZERO(FUNC_CALL) if(FUNC_CALL != 0) throw SDLException();
#define SDL_TRY_ONE(FUNC_CALL) if(FUNC_CALL != 1) throw SDLException();

}