#pragma once

#include "gui.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"
#include "../../imgui/imgui.h"
#include "../../imgui/backends/imgui_impl_sdl2.h"
#include "../../imgui/backends/imgui_impl_opengl3.h"
#include "../../kerep/src/Filesystem/filesystem.h"

//////////////////////////////////////
//              Fonts               //
//////////////////////////////////////
#include "../generated/fonts_embedded.h"

#define __CAT3(A,B,C) A##B##C
#define embedded_font_data(FONT_NAME) __CAT3(font_,FONT_NAME,_compressed_data)
#define embedded_font_size(FONT_NAME) __CAT3(font_,FONT_NAME,_compressed_size)

f32 getMainWindowDPI();
ImFont* ImFont_LoadFromFile(const char* file_path, f32 font_size);
ImFont* _ImFont_LoadEmbedded(const void* data, int data_size, f32 font_size);

#define ImFont_LoadEmbedded(FONT_NAME, FONT_SIZE) _ImFont_LoadEmbedded( \
    embedded_font_data(FONT_NAME), \
    embedded_font_size(FONT_NAME), \
    FONT_SIZE)

//////////////////////////////////////
//     Variables and constants      //
//////////////////////////////////////
const u8 frame_rate_max=60; // frames per second
extern ImVec4 clear_color; // background color for main window

//////////////////////////////////////
//           UI Functions           //
//////////////////////////////////////
/// @brief frees all allocated resources
void main_window_destroy();
void draw_demo_windows(ImGuiIO&);
void draw_bg_window();

void node_editor_open(const char* title);
void node_editor_draw();
void node_editor_close();

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
