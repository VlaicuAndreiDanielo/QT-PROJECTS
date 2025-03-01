#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <QPoint>
#include <fstream>
#include "node.h"
#include <QDebug>
#include <set>
#include <vector>
#include "edge.h"

class Graph {
public:
    Graph() : oriented(false) {}
    ~Graph();

    void addNode(const QPoint& pos);
    void addEdge(Node* first, Node* second, bool isOriented, int cost);
    const std::vector<Node*>& getNodes() const { return nodes; }
    const std::vector<Edge*>& getEdges() const { return edges; }
    void setOriented(bool value) { oriented = value; }
    bool isGraphOriented() const { return oriented; }
    void saveAdjacencyMatrix();

    std::vector<Edge*> genericMST();
    size_t findComponent(const std::vector<std::set<Node*>>& N, Node* node) const;
    bool isInSet(const std::set<Node*>& component, Node* node) const;
    void clearEdges();

    std::vector<Edge*> primMST();

    int find(int u, std::vector<int>& parent);
    void unite(int u, int v, std::vector<int>& parent, std::vector<int>& rank);
    std::vector<Edge*> kruskalMST();

    std::vector<Edge*> boruvkaMST();
    int findForBoruvka(int u, std::vector<int>& parent);
    void unionSets(int u, int v, std::vector<int>& parent, std::vector<int>& rank);
    Node* getNodeByValue(int value);

private:
    std::vector<Node*> nodes;
    std::vector<Edge*> edges;
    bool oriented;
};

#endif // GRAPH_H
