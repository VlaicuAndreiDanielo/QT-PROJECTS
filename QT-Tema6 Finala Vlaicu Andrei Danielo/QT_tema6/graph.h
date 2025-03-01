#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <QPoint>
#include <queue>
#include <fstream>
#include "node.h"
#include <QDebug>
#include <vector>
#include <QTimer>
#include "edge.h"

class MainWindow;
class Graph {
public:
    Graph() : oriented(false) {}
    ~Graph();

    void addNode(const QPoint& pos);
    Edge* addEdge(Node* first, Node* second, bool isOriented, int cost);
    const std::vector<Node*>& getNodes() const { return nodes; }
    const std::vector<Edge*>& getEdges() const;
    std::vector<Edge*>& getEdges();
    void setOriented(bool value) { oriented = value; }
    bool isGraphOriented() const { return oriented; }
    void saveAdjacencyMatrixInFile();
    void saveAdjacencyMatrix();
    const std::vector<std::vector<int>>& getAdjacencyMatrix() const;
    void saveEdgesToFile(const std::string& filename);


    Node* getNodeByValue(int value);
    bool bfs(const std::vector<std::vector<int>>& residualGraph, int source, int sink, std::vector<int>& parent);
    int fordFulkerson(int source, int sink, MainWindow* mainWindow, std::vector<std::vector<std::vector<int>>>& residualGraphs);
    std::vector<bool> findReachableNodesInResidualGraph(const std::vector<std::vector<int>>& residualGraph, int source);
private:
    std::vector<Node*> nodes;
    std::vector<std::vector<int>> adjacencyMatrix;
    std::vector<Edge*> edges;
    bool oriented;
};

#endif // GRAPH_H
