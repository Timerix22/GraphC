#include "gui_internal.hpp"
#include <algorithm>
#include "NodeEditor.hpp"

namespace GraphC::gui {

NodeAttribute::NodeAttribute(id_t _id, NodeAttributeType _type, std::string _title) 
    : id(_id), type(_type), title(_title) {}

NodeAttributeLink::NodeAttributeLink(id_t _id, id_t _in, id_t _out) 
    : id(_id), in_attr_id(_in), out_attr_id(_out) {}

Node::Node(id_t _id, std::string _title) 
    : id(_id), title(_title) {}

void Node::draw(){
    ImNodes::BeginNode(id);
        ImNodes::BeginNodeTitleBar();
            ImGui::TextUnformatted(title.c_str());
        ImNodes::EndNodeTitleBar();

        for(NodeAttribute& a : attributes)
        {
            switch (a.type)
            {
                case NodeAttributeType::Input:
                    ImNodes::BeginInputAttribute(a.id);
                        ImGui::Text("%s", a.title.c_str());
                    ImNodes::EndInputAttribute();
                    break;
                case NodeAttributeType::Output:
                    ImNodes::BeginOutputAttribute(a.id);
                        ImGui::Text("%s", a.title.c_str());
                        ImGui::Indent(40);
                    ImNodes::EndOutputAttribute();
                    break;
                case NodeAttributeType::Static:
                    ImNodes::BeginStaticAttribute(a.id);
                        ImGui::Text("%s", a.title.c_str());
                    ImNodes::EndStaticAttribute();
                    break;
                default:
                    throw "Node::draw() invalid type";
                    break;
            }
        }
    ImNodes::EndNode();
}


Node CreateExampleNode(id_t* next_id, std::string title){
    Node a = Node((*next_id)++, title);
    a.attributes.push_back(NodeAttribute((*next_id)++, NodeAttributeType::Input, "In"));
    a.attributes.push_back(NodeAttribute((*next_id)++, NodeAttributeType::Output, "Out"));
    a.attributes.push_back(NodeAttribute((*next_id)++, NodeAttributeType::Static, "Static"));
    return a;
}

NodeEditor::NodeEditor(std::string _title) : title(_title) {
    nodes.push_back(CreateExampleNode(&next_id, "node A"));
    nodes.push_back(CreateExampleNode(&next_id, "node B"));
}

void NodeEditor::show(){
    editor_open = true;
}

void NodeEditor::hide(){
    editor_open = false;
}

void NodeEditor::draw(){
    if(!editor_open)
        return;

    ImGui::Begin(title.c_str(), &editor_open);

        ImGui::SetWindowSizeMin(300,300);
        ImNodes::BeginNodeEditor();
            // draw nodes
            for(Node& n : nodes){
                n.draw();
            }
            // draw links
            for(const NodeAttributeLink& l : links){
                ImNodes::Link(l.id, l.in_attr_id, l.out_attr_id);
            }
        ImNodes::EndNodeEditor();

        // handle link creation
        id_t in_attr_id;
        id_t out_attr_id;
        if (ImNodes::IsLinkCreated(&in_attr_id, &out_attr_id))
        {
            kprintf("new link id: %i\n", next_id);
            NodeAttributeLink link(next_id++, in_attr_id, out_attr_id);
            links.push_back(link);
        }
    
        // handle link destruction
        id_t link_id;
        if (ImNodes::IsLinkDestroyed(&link_id))
        {
            kprintf("link destroyed %i\n", link_id);
            auto iter = std::find_if(links.begin(), links.end(), 
                [link_id](const NodeAttributeLink& link) -> bool {
                    kprintf("destroyed link found\n");
                    return link.id == link_id;
                });
            assert(iter != links.end());
            links.erase(iter);
            kprintf("destroyed link erased\n");
        }

    ImGui::End();
}

}
