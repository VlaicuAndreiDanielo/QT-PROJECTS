#ifndef EDGE_H
#define EDGE_H

#include "node.h"

class Edge {
public:
    Edge();
    Edge(Node* first, Node* second) : m_first(first), m_second(second) {}

    Node* getFirst() const { return m_first; }
    Node* getSecond() const { return m_second; }

private:
    Node* m_first;
    Node* m_second;
};

#endif // EDGE_H
