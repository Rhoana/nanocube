#pragma once

#include <vector>
#include <cstdint>

#include <unordered_set>

struct Node;
struct ParentChildLink;
struct ContentLink;

//-----------------------------------------------------------------------------
// Label, DimAddress and Address
//-----------------------------------------------------------------------------

using  Label      = int;
using  DimAddress = std::vector<Label>;
using  Address    = std::vector<DimAddress>;


//-----------------------------------------------------------------------------
// Object and Summary
//-----------------------------------------------------------------------------

using  Object = int;

struct Summary {
    Summary() = default;

    Summary(const Summary& other) = default;
    Summary& operator=(const Summary& other) = default;

    std::string info() const;
    void insert(const Object &obj);

public:

    std::unordered_set<Object> objects;
};

//-----------------------------------------------------------------------------
// LinkType
//-----------------------------------------------------------------------------

enum LinkType { SHARED, PROPER };

//-----------------------------------------------------------------------------
// ContentLink
//-----------------------------------------------------------------------------

struct ContentLink {

    ContentLink() = default;
    ContentLink(void* content, LinkType link_type);

    ContentLink(const ContentLink& other) = default;
    ContentLink& operator=(const ContentLink& other) = default;


    void*     content   { nullptr }; // might be a node or a summary
    LinkType  link_type { PROPER };
};

//-----------------------------------------------------------------------------
// ParentChildLink
//-----------------------------------------------------------------------------

struct ParentChildLink {
    ParentChildLink() = default;
    ParentChildLink(Label label, Node* child, LinkType link_type);

    ParentChildLink(const ParentChildLink& other) = default;
    ParentChildLink& operator=(const ParentChildLink& other) = default;

    Label    label { 0 };
    Node*    child { nullptr };
    LinkType link_type { PROPER };
};

//-----------------------------------------------------------------------------
// Nanocube
//-----------------------------------------------------------------------------

struct Nanocube {
public:
    Nanocube(const std::vector<int> &levels);
    void insert(const Address &addr, const Object &object);
    Summary* query(const Address &addr);
public:

    Node *root { nullptr };

    std::vector<int> levels;
    int              dimension;
};

//-----------------------------------------------------------------------------
// Node
//-----------------------------------------------------------------------------

struct Node {
public: // nanocube API
    void prepareOutdatedProperPath(const Node* child_updated_path, const DimAddress &addr, std::vector<Node*> &result_path);
    Node* shallowCopy() const;
public:
    void* getContent() const;
    void  setContent(void *content, LinkType type);

    LinkType getContentType() const;
    bool     hasContent() const;

    Summary* getContentAsSummary();
    Node*    getContentAsNode();

    // ParentChildLink* getParentChildLink(Label label) const;
    // ParentChildLink* getOrCreateParentChildLink(Label label, bool &created);
    void setParentChildLink(Label label, Node* node, LinkType link_type, bool &created);

    Node* getChild(Label label) const;
    Node* getChild(Label label, LinkType &link_type) const;

public:
    std::vector<ParentChildLink> children;
    ContentLink content_link;
};
