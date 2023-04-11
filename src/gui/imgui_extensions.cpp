#include "gui_internal.hpp"

namespace ImGui {

void SetWindowSizeMin(f32 window_width_min, f32 window_height_min){
    ImVec2 window_size=ImGui::GetWindowSize();
    ImVec2 new_size=window_size;
    bool changed=false;
    if(window_size.x<window_width_min){
        new_size.x=window_width_min;
        changed=true;    
    }
    if(window_size.y<window_height_min){
        new_size.y=window_height_min;
        changed=true;
    }
    if(changed)
        ImGui::SetWindowSize(new_size);
}

} // namespace


ImVec2 ImVec2Add(ImVec2 a, ImVec2 b) {
    a.x+=b.x;
    a.y+=b.y;
    return a;
}
