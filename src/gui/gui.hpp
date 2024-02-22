#pragma once

#include "../../dependencies/kerep/src/base/base.h"
#include "../../dependencies/SDL2/include/SDL.h"
#include "../../dependencies/SDL2/include/SDL_opengl.h"
#include "../../dependencies/imgui/imgui.h"
#include "NodeEditor.hpp"
#include "imgui_extensions.hpp"
#include "fonts.hpp"
#include "exceptions.hpp"

/// converts hex color to float vector
#define RGBAHexToF(R8,G8,B8,A8) ImVec4(((u8)35)/255.0f, ((u8)35)/255.0f, ((u8)50)/255.0f, ((u8)255)/255.0f)
/// converts float vector to hex color
#define RGBAFToHex(VEC4) {(u8)(VEC4.x*255), (u8)(VEC4.y*255), (u8)(VEC4.z*255), (u8)(VEC4.w*255)}

namespace GraphC::gui {

#define default_font font_DroidSans

class GUI {
public:
    ImVec4 clear_color = RGBAHexToF(35,35,50,255);
    f32 default_font_size = 14.0f;
    u8 fps_min = 30;
    u8 fps_max = 60;

private:
    bool loop_running=false;
    bool main_loop_wait_for_input=true;
    bool show_demo_window = false;
    bool show_metrics_window = false;
    SDL_Window* sdl_window;
    SDL_GLContext gl_context;
    NodeEditor node_editor = NodeEditor("new editor");

public:
    void init(const char* window_title);
    void startAndWait();
    void close();
    f32 getDPI();

private:
    void destroy();
    void poll_events(u16& frame_updates_requested, bool wait);
    void draw_frame();
    void draw_debug_window(ImGuiIO &io, bool *main_loop_wait_for_input);
    void draw_bg_window();
};

}
