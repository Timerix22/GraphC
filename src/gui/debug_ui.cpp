#include "gui_internal.hpp"

bool show_demo_window = false;
bool show_metrics_window = false;

void draw_debug_window(ImGuiIO& io, bool* main_loop_wait_for_input){
    ImGui::Begin("Debug Options");
    ImGui::ColorEdit3("clear_color", (float*)&clear_color);
    ImGui::Checkbox("main_loop_wait_for_input", main_loop_wait_for_input);
    ImGui::Text("Application average %.3f ms/frame (%.2f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::Checkbox("Demo Window", &show_demo_window);
    ImGui::Checkbox("Metrics/Debug Window", &show_metrics_window);
    ImGui::End();

    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    if (show_metrics_window)
        ImGui::ShowMetricsWindow(&show_metrics_window);
}
