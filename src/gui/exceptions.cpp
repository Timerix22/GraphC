#include "exceptions.hpp"
#include "../../dependencies/SDL2/include/SDL.h"

namespace GraphC::gui {

SDLException_::SDLException_(std::string _file, std::string _func, int _line_n)
    : UsefulException_(SDL_GetError(), _file, _func, _line_n)
{
    SDL_ClearError();
}

}