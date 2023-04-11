#include "gui_internal.hpp"
#include "../../imgui-node-editor/imgui_node_editor.h"
namespace NE = ax::NodeEditor;

NE::EditorContext* editor_context=nullptr;
const char* editor_title;

void draw_example_node();


void node_editor_open(const char* title){
    NE::Config config;
    config.SettingsFile = "node_editor.json";
    editor_context=NE::CreateEditor(&config);
    editor_title=title;
    NE::Style& style=NE::GetStyle();
}

void node_editor_close(){
    NE::DestroyEditor(editor_context);
    editor_context=nullptr;
}

u32 node_id_next;
u32 pin_id_next;
u32 link_id_next;

void draw_node_editor(){
    if(editor_context==nullptr)
        return;

    node_id_next=0;
    pin_id_next=0;
    link_id_next=0;
    ImGui::Begin(editor_title);
        ImGui::SetWindowSizeMin(300,300);
        NE::SetCurrentEditor(editor_context);
        NE::Begin(editor_title);
            // Start drawing nodes.
            draw_example_node();
            ImVec2 node_pos=NE::GetNodePosition(node_id_next-1);
            NE::SetNodePosition(node_id_next, ImVec2Add(node_pos, ImVec2(50, 20)));
            draw_example_node();
        NE::End();
        NE::SetCurrentEditor(nullptr);
    ImGui::End();
}

void draw_example_node(){
    NE::BeginNode(node_id_next++);
        ImGui::Text("Node A");
        NE::BeginPin(pin_id_next++, NE::PinKind::Input);
            ImGui::Text("-> In");
        NE::EndPin();
        ImGui::SameLine();
        NE::BeginPin(pin_id_next++, NE::PinKind::Output);
            ImGui::Text("Out ->");
        NE::EndPin();
    NE::EndNode();
}
