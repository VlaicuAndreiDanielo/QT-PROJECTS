#ifndef EDGE_H
#define EDGE_H

#include "node.h"
#include <QColor>

class Edge {
public:
    Edge();
    Edge(Node* first, Node* second) : m_first(first), m_second(second) {}

    Node* getFirst() const { return m_first; }
    Node* getSecond() const { return m_second; }


    void setComponentColor(const QColor& color) {
        componentColor = color;
    }

    QColor getComponentColor() const {
        return componentColor;
    }

private:
    Node* m_first;
    Node* m_second;
    QColor componentColor;
};

#endif // EDGE_H
