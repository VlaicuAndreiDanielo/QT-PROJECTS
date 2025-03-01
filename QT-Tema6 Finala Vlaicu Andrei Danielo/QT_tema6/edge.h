#ifndef EDGE_H
#define EDGE_H

#include "node.h"

class Edge {
public:
    Edge();
    Edge(Node* first, Node* second, int cost = 1)
        : m_first(first), m_second(second), m_cost(cost), highlighted(false) {}
    Node* getFirst() const { return m_first; }
    Node* getSecond() const { return m_second; }
    int getCost() const { return m_cost; }
    void setCost(int cost) { m_cost = cost; }
    void setHighlight(bool highlight) {
        this->highlighted = highlight;
    }

    bool isHighlighted() const {
        return highlighted;
    }
private:
    Node* m_first;
    Node* m_second;
    int m_cost;
    bool highlighted;
};

#endif // EDGE_H
