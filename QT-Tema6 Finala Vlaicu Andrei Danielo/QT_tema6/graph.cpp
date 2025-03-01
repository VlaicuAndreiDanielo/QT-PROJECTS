#include "graph.h"
#include "edge.h"
#include "mainwindow.h"
#include <fstream>
Graph::~Graph() {

    for (Node* node : nodes) {
        delete node;
    }

    for (Edge* edge : edges) {
        delete edge;
    }
}

void Graph::addNode(const QPoint& pos) {
    nodes.push_back(new Node(nodes.size(), pos));
}

Edge* Graph::addEdge(Node* first, Node* second, bool isOriented, int cost) {
    for (auto edge : edges) {
        if (edge->getFirst() == first && edge->getSecond() == second) {
            return edge;
        }
    }

    Edge* newEdge = new Edge(first, second, cost);
    edges.push_back(newEdge);
    return newEdge;
}

const std::vector<Edge*>& Graph::getEdges() const {
    return edges;
}

std::vector<Edge*>& Graph::getEdges() {
    return edges;
}

Node* Graph::getNodeByValue(int value) {
    for (Node* node : nodes) {
        if (node->getValue() == value) {
            return node;
        }
    }
    return nullptr;
}
void Graph::saveAdjacencyMatrixInFile() {
    std::ofstream file("fileOut.txt");
    file << nodes.size() << std::endl;
    std::vector<std::vector<int>> matrix(nodes.size(), std::vector<int>(nodes.size(), 0));

    for (auto& edge : edges) {
        int first = edge->getFirst()->getValue();
        int second = edge->getSecond()->getValue();
        matrix[first][second] = edge->getCost();
        if (!isGraphOriented()) {
            matrix[second][first] = edge->getCost();
        }
    }

    for (const auto& row : matrix) {
        for (int value : row) {
            file << value << " ";
        }
        file << std::endl;
    }

    file.close();
}

bool Graph::bfs(const std::vector<std::vector<int>>& residualGraph, int source, int sink, std::vector<int>& parent) {
    int n = residualGraph.size();
    std::vector<bool> visited(n, false);

    std::queue<int> q;
    q.push(source);
    visited[source] = true;
    parent[source] = -1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v = 0; v < n; v++) {
            if (!visited[v] && residualGraph[u][v] > 0) {
                q.push(v);
                parent[v] = u;
                visited[v] = true;

                if (v == sink) {
                    return true;
                }
            }
        }
    }
    return false;
}


int Graph::fordFulkerson(int source, int sink, MainWindow* mainWindow, std::vector<std::vector<std::vector<int>>>& residualGraphs) {
    int u, v;

    std::vector<std::vector<int>> residualGraph = adjacencyMatrix;

    int maxFlow = 0;

    std::vector<int> parent(nodes.size());

    while (bfs(residualGraph, source, sink, parent)) {
        int pathFlow = INT_MAX;
        for (v = sink; v != source; v = parent[v]) {
            u = parent[v];
            pathFlow = std::min(pathFlow, residualGraph[u][v]);
        }

        for (v = sink; v != source; v = parent[v]) {
            u = parent[v];
            residualGraph[u][v] -= pathFlow;
            residualGraph[v][u] += pathFlow;
        }

        residualGraphs.push_back(residualGraph);

        maxFlow += pathFlow;
    }

    return maxFlow;
}

void Graph::saveAdjacencyMatrix() {

    int size = nodes.size();
    adjacencyMatrix.assign(size, std::vector<int>(size, 0));

    for (Edge* edge : edges) {
        int firstIndex = edge->getFirst()->getValue();
        int secondIndex = edge->getSecond()->getValue();
        adjacencyMatrix[firstIndex][secondIndex] = edge->getCost();

        if (!oriented) {
            adjacencyMatrix[secondIndex][firstIndex] = edge->getCost();
        }
    }
}

const std::vector<std::vector<int>>& Graph::getAdjacencyMatrix() const {
    return adjacencyMatrix;
}


std::vector<bool> Graph::findReachableNodesInResidualGraph(const std::vector<std::vector<int>>& residualGraph, int source) {
    std::vector<bool> visited(residualGraph.size(), false);
    std::queue<int> queue;

    queue.push(source);
    visited[source] = true;

    while (!queue.empty()) {
        int current = queue.front();
        queue.pop();

        for (int v = 0; v < residualGraph[current].size(); ++v) {
            if (residualGraph[current][v] > 0 && !visited[v]) {
                visited[v] = true;
                queue.push(v);
            }
        }
    }

    return visited;
}

void Graph::saveEdgesToFile(const std::string& filename) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        qDebug() << "Nu s-a putut deschide fișierul pentru salvare.";
        return;
    }

    file << "Numar total de muchii: " << edges.size() << "\n";

    for (Edge* edge : edges) {
        Node* first = edge->getFirst();
        Node* second = edge->getSecond();
        int cost = edge->getCost();
        bool isHighlighted = edge->isHighlighted();

        file << "Start: " << first->getValue() + 1
             << ", Finish: " << second->getValue() + 1
             << ", Cost: " << cost
             << ", Highlighted: " << (isHighlighted ? "Da" : "Nu")
             << ", Edge to: " << second->getValue() + 1
             << "\n";
    }

    file.close();
    qDebug() << "Informațiile despre muchii au fost salvate în fișier.";
}
