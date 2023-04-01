#include "gui_internal.hpp"
#include "../../imgui-node-editor/imgui_node_editor.h"
namespace NE = ax::NodeEditor;

NE::EditorContext* editor_context=nullptr;
const char* editor_title;

void node_editor_open(const char* title){
    NE::Config config;
    config.SettingsFile = "node_editor.json";
    editor_context=NE::CreateEditor(&config);
    editor_title=title;
}

void node_editor_close(){
    NE::DestroyEditor(editor_context);
    editor_context=nullptr;
}

void node_editor_draw(){
    if(editor_context==nullptr)
        return;

    ImGui::Begin(editor_title);
    NE::SetCurrentEditor(editor_context);
    NE::Begin(editor_title, ImVec2(0.0, 0.0f));
    int uniqueId = 1;
    // Start drawing nodes.
    NE::BeginNode(uniqueId++);
        ImGui::Text("Node A");
        NE::BeginPin(uniqueId++, NE::PinKind::Input);
            ImGui::Text("-> In");
        NE::EndPin();
        ImGui::SameLine();
        NE::BeginPin(uniqueId++, NE::PinKind::Output);
            ImGui::Text("Out ->");
        NE::EndPin();
    NE::EndNode();
    NE::End();
    NE::SetCurrentEditor(nullptr);
    ImGui::End();
}

