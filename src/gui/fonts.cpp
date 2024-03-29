#include "gui.hpp"

namespace GraphC::gui::fonts {

ImFont* ImFont_LoadFromFile(const char* file_path, f32 font_size, f32 dpi){
    ImGuiIO& io = ImGui::GetIO();
    font_size *= dpi;
    return io.Fonts->AddFontFromFileTTF(file_path, font_size);
}

ImFont* _ImFont_LoadEmbedded(const void* data, int data_size, const char* font_name, f32 font_size, f32 dpi){
    ImGuiIO& io = ImGui::GetIO();
    font_size *= dpi;
    ImFontConfig font_cfg = ImFontConfig();
    sprintf_s(font_cfg.Name, IM_ARRAYSIZE(font_cfg.Name), "%s, %.0fpx", font_name, font_size);
    return io.Fonts->AddFontFromMemoryCompressedTTF(data, data_size, font_size, &font_cfg);
}

}
