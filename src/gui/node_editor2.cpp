#include "gui_internal.hpp"
#include "../../dependencies/imnodes/imnodes.h"
#include <vector>
#include <algorithm>

struct Link {
    int id;
    int in_attr_id;
    int out_attr_id;
};

const char* editor_title=nullptr;
bool editor_open=false;
ImNodesContext* editor_context=nullptr;
int next_id=1;
std::vector<Link> links;

void node_editor_create(const char* title){
    editor_title=title;
    editor_open=true;
    editor_context=ImNodes::CreateContext();
    ImNodes::StyleColorsDark();
    links=std::vector<Link>();
}

void node_editor_destroy(){
    editor_open=false;
    ImNodes::DestroyContext(editor_context);
    links.clear();
}

void draw_example_node(const char* node_title){
    ImNodes::BeginNode(next_id++);
        ImNodes::BeginNodeTitleBar();
            ImGui::TextUnformatted(node_title);
        ImNodes::EndNodeTitleBar();
        ImNodes::BeginInputAttribute(next_id++);
            ImGui::Text("input");
        ImNodes::EndInputAttribute();
        ImNodes::BeginOutputAttribute(next_id++);
            ImGui::Indent(40);
            ImGui::Text("output");
        ImNodes::EndOutputAttribute();
    ImNodes::EndNode();
}

void draw_node_editor(){
    if(!editor_open)
        return;
    next_id=1;
    ImGui::Begin(editor_title, &editor_open);
        ImGui::SetWindowSizeMin(300,300);
        ImNodes::BeginNodeEditor();
            draw_example_node("node A");
            draw_example_node("node B");
            for(const Link& link : links){

            }
        ImNodes::EndNodeEditor();

        Link link;
        if (ImNodes::IsLinkCreated(&link.in_attr_id, &link.out_attr_id))
        {
            link.id=next_id++;
            links.push_back(link);
        }
    
        
        int link_id;
        if (ImNodes::IsLinkDestroyed(&link_id))
        {
            auto iter = std::find_if(links.begin(), links.end(), 
                [link_id](const Link& link) -> bool {
                    return link.id == link_id;
                });
            assert(iter != links.end());
            links.erase(iter);   
        }

    ImGui::End();
}
