#pragma once

#include "gui.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"
#include "../../imgui/imgui.h"
#include "../../imgui/backends/imgui_impl_sdl2.h"
#include "../../imgui/backends/imgui_impl_opengl3.h"

const u8 frame_rate_max=60; // frames per second
extern ImVec4 clear_color; // background color for main window

/// @brief frees all allocated resources
void main_window_destroy();
Maybe demo_ui_draw(ImGuiIO&);
