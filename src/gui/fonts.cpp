#include "gui_internal.hpp"

ImFont* ImFont_LoadFromFile(const char* file_path, f32 font_size){
    ImGuiIO& io = ImGui::GetIO();
    font_size *= getMainWindowDPI();
    return io.Fonts->AddFontFromFileTTF(file_path, font_size);
}

ImFont* _ImFont_LoadEmbedded(const void* data, int data_size, f32 font_size){
    ImGuiIO& io = ImGui::GetIO();
    font_size *= getMainWindowDPI();
    return io.Fonts->AddFontFromMemoryCompressedTTF(data, data_size, font_size);
}
