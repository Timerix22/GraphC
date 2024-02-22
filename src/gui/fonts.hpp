#pragma once

#include "../../dependencies/imgui/imgui.h"
#include "../generated/fonts_embedded.h"

namespace GraphC::gui::fonts {

#define __CAT3(A,B,C) A##B##C
#define embedded_font_data(FONT) __CAT3(font_,FONT,_compressed_data)
#define embedded_font_data_size(FONT) __CAT3(font_,FONT,_compressed_size)
#define embedded_font_name(FONT) #FONT


ImFont* ImFont_LoadFromFile(const char* file_path, f32 font_size, f32 dpi);

#define ImFont_LoadEmbedded(FONT, FONT_SIZE, DPI) _ImFont_LoadEmbedded( \
    embedded_font_data(FONT), \
    embedded_font_data_size(FONT), \
    embedded_font_name(FONT), \
    FONT_SIZE, DPI)
ImFont* _ImFont_LoadEmbedded(const void* data, int data_size, const char* font_name, f32 font_size, f32 dpi);

}
