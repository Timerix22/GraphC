#pragma once

#include "../../dependencies/imnodes/imnodes.h"
#include <vector>
#include <string>

namespace GraphC::gui {

typedef i32 id_t;

enum class NodeAttributeType {
    Input, Output, Static
};

struct NodeAttribute {
    id_t id;
    NodeAttributeType type;
    std::string title;

    NodeAttribute(id_t _id, NodeAttributeType _type, std::string _title);
};

struct Node {
    id_t id;
    std::string title;
    std::vector<NodeAttribute> attributes;

    Node(id_t _id, std::string _title);

    void draw();
};

struct NodeAttributeLink {
    id_t id;
    id_t in_attr_id;
    id_t out_attr_id;

    NodeAttributeLink(id_t _id, id_t _in, id_t _out);
};

class NodeEditor {
    std::string title=nullptr;
    bool editor_open=false;
    ImNodesContext* editor_context=nullptr;
    id_t next_id=1;
    std::vector<Node> nodes;
    std::vector<NodeAttributeLink> links;

public:
    NodeEditor(std::string _title);

    void draw();
    void show();
    void hide();
};

}