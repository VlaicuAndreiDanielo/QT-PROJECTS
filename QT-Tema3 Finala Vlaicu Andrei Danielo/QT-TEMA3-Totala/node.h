#ifndef NODE_H
#define NODE_H

#include <vector>
#include <QColor>
#include <cstdlib>
#include <ctime>
#include <QPoint>

class Node {
public:
    Node();
    Node(int value, const QPoint& pos) : m_value(value), m_pos(pos) {}

    int getValue() const { return m_value; }
    QPoint getPos() const { return m_pos; }
    void setPos(const QPoint& pos) { m_pos = pos; }

    void addNeighbor(Node* neighbor) { neighbors.push_back(neighbor);}
    const std::vector<Node*>& getNeighbors() const { return neighbors; }

    void setComponentColor(const QColor& color) {
        componentColor = color;
    }

    QColor getComponentColor() const {
        return componentColor;
    }
private:
    int m_value;
    QPoint m_pos;
    QColor componentColor;
    std::vector<Node*> neighbors;
};

#endif // NODE_H
