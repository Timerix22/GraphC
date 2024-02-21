#pragma once

#include "gui.h"
#include "../../dependencies/SDL2/include/SDL.h"
#include "../../dependencies/SDL2/include/SDL_opengl.h"
#include "../../dependencies/imgui/imgui.h"
#include "../../dependencies/imgui/backends/imgui_impl_sdl2.h"
#include "../../dependencies/imgui/backends/imgui_impl_opengl3.h"
#include "../../dependencies/kerep/src/Filesystem/filesystem.h"

//////////////////////////////////////
//              Fonts               //
//////////////////////////////////////
#include "../generated/fonts_embedded.h"

#define __CAT3(A,B,C) A##B##C
#define embedded_font_data(FONT) __CAT3(font_,FONT,_compressed_data)
#define embedded_font_size(FONT) __CAT3(font_,FONT,_compressed_size)
#define embedded_font_name(FONT) #FONT

f32 getMainWindowDPI();
ImFont* ImFont_LoadFromFile(const char* file_path, f32 font_size);
ImFont* _ImFont_LoadEmbedded(const void* data, int data_size, const char* font_name, f32 font_size);

#define ImFont_LoadEmbedded(FONT, FONT_SIZE) _ImFont_LoadEmbedded( \
    embedded_font_data(FONT), \
    embedded_font_size(FONT), \
    embedded_font_name(FONT), \
    FONT_SIZE)

//////////////////////////////////////
//     Variables and constants      //
//////////////////////////////////////
const u8 frame_rate_min=30; // frames per second
const u8 frame_rate_max=60; // frames per second
extern ImVec4 clear_color; // background color for main window

//////////////////////////////////////
//           UI Functions           //
//////////////////////////////////////
/// @brief frees all allocated resources
void main_window_destroy();
void draw_debug_window(ImGuiIO& io, bool* main_loop_wait_for_input);
void draw_bg_window();

void node_editor_create(const char* title);
void draw_node_editor();
void node_editor_destroy();

//////////////////////////////////////
//             Macros               //
//////////////////////////////////////
#define SDL_ERROR_SAFETHROW() { \
    const char* sdl_error=SDL_GetError(); \
    safethrow_msg(cptr_concat("SDL Error: ", sdl_error),;); \
    SDL_ClearError(); \
}
#define SDL_TRY_ZERO(FUNC_CALL) if(FUNC_CALL != 0) SDL_ERROR_SAFETHROW();
#define SDL_TRY_ONE(FUNC_CALL) if(FUNC_CALL != 1) SDL_ERROR_SAFETHROW();


//////////////////////////////////////
//        ImGui extensions          //
//////////////////////////////////////
namespace ImGui {
    void SetWindowSizeMin(f32 window_width_min, f32 window_height_min);
}

ImVec2 ImVec2Add(ImVec2 a, ImVec2 b);
