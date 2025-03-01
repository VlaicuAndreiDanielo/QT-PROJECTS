#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <QPoint>
#include <fstream>
#include "node.h"
#include "edge.h"

class Graph {
public:
    Graph() : oriented(false) {}
    ~Graph();

    void addNode(const QPoint& pos);
    void addEdge(Node* first, Node* second, bool isOriented);
    const std::vector<Node*>& getNodes() const { return nodes; }
    const std::vector<Edge*>& getEdges() const { return edges; }
    void setOriented(bool value) { oriented = value; }
    bool isGraphOriented() const { return oriented; }
    void saveAdjacencyMatrix();

private:
    std::vector<Node*> nodes;
    std::vector<Edge*> edges;
    bool oriented;
};

#endif // GRAPH_H
