#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <QPoint>
#include <fstream>
#include <stack>
#include <unordered_set>
#include <cstdlib>
#include "node.h"
#include "edge.h"
#include <QGraphicsScene>
#include <QFile>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QMessageBox>
#include <QLayout>
#include <QGraphicsView>

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& pair) const {
        auto hash1 = std::hash<T1>{}(pair.first);
        auto hash2 = std::hash<T2>{}(pair.second);
        return hash1 ^ hash2;
    }
};
enum class ChangeType {
    AddNode,
    AddEdge
};

struct GraphChange {
    ChangeType type;
    Node* node;
    Edge* edge;
};

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
    bool edgeExists(Node* first, Node* second) const;

    void topologicalSort();
    void saveTopologicalSort(const std::string& filename);
    void drawGraph(QGraphicsScene* scene);
    void loadTopologicalSort(const std::string& filename);
    bool respectsTopologicalOrder(Node* first, Node* second) const;

    bool dfs(Node* node, std::vector<int>& visited, std::stack<Node*>& topologicalOrder);
    bool isCircuit();
    void clear();
    void setEdges(const std::vector<Edge*>& edges);
    std::vector<Node*> getTopologicalOrder() const;
    Node* getNodeByValue(int value);

    bool isTree();
    void dfsConnected(Node* node, std::unordered_map<Node*, bool>& visited);
    Node* findRoot();
    bool isCycle();

    QColor generateRandomColor() {
        int red = rand() % 256;
        int green = rand() % 256;
        int blue = rand() % 256;
        return QColor(red, green, blue);
    }
    void findConnectedComponents();
    void dfsForComponents(Node* node, int componentId, std::unordered_map<Node*, int>& visited);

    void findStronglyConnectedComponents();
    Graph buildSCCGraph();
    void dfsSCC(int node, std::vector<bool>& visited, std::stack<int>& finishStack);
    void drawSCCs(QGraphicsScene* scene);
    std::vector<std::vector<int>> findSCCs();
    void dfsTranspose(int node, std::vector<bool>& visited, std::vector<int>& scc);
    void showSCCs();
    void clearAll();
    void clearEdges();
    void undoLastChange();
private:
    std::vector<Node*> nodes;
    std::vector<Edge*> edges;
    std::stack<GraphChange> changeHistory;
    bool oriented = false;

    std::stack<Node*> topologicalOrder;
    std::unordered_map<Node*, int> nodeIndex;
    std::vector<std::vector<Node*>> transposedGraph;
};

#endif // GRAPH_H
