#ifndef NODE_H
#define NODE_H

#include <QPoint>

class Node {
public:
    Node();
    Node(int value, const QPoint& pos) : m_value(value), m_pos(pos) {}

    int getValue() const { return m_value; }
    QPoint getPos() const { return m_pos; }
    void setPos(const QPoint& pos) { m_pos = pos; }

private:
    int m_value;
    QPoint m_pos;
};

#endif // NODE_H
