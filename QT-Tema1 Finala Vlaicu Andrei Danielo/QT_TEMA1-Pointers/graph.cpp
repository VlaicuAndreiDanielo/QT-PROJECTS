#include "graph.h"
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

void Graph::addEdge(Node* first, Node* second, bool isOriented) {
    // Verificăm dacă muchia există deja
    for (auto edge : edges) {
        if (edge->getFirst() == first && edge->getSecond() == second) {
            return; // Muchia există deja, nu o mai adăugăm
        }
        if (!isOriented && edge->getFirst() == second && edge->getSecond() == first) {
            return; // Muchia inversă există deja într-un graf neorientat
        }
    }

    // Adăugăm muchia
    edges.push_back(new Edge(first, second));

    // Dacă graful este neorientat, adăugăm și muchia inversă
    if (!isOriented) {
        edges.push_back(new Edge(second, first));
    }
}
void Graph::saveAdjacencyMatrix() {
    std::ofstream file("fileOut.txt");
    file << nodes.size() << std::endl;
    std::vector<std::vector<int>> matrix(nodes.size(), std::vector<int>(nodes.size(), 0));

    for (auto& edge : edges) {
        int first = edge->getFirst()->getValue();
        int second = edge->getSecond()->getValue();
        matrix[first][second] = 1;
        if (!isGraphOriented()) {
            matrix[second][first] = 1;
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

