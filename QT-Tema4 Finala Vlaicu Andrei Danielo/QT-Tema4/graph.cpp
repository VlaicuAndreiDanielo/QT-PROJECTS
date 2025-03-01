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

void Graph::addEdge(Node* first, Node* second, bool isOriented, int cost) {
    for (auto edge : edges) {
        if (edge->getFirst() == first && edge->getSecond() == second) {
            return;
        }

        if (!isOriented && edge->getFirst() == second && edge->getSecond() == first) {
            return;
        }
    }

    edges.push_back(new Edge(first, second, cost));
}

void Graph::saveAdjacencyMatrix() {
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

// Complexitate aprox O(n^2);
std::vector<Edge*> Graph::genericMST() {

    std::vector<std::set<Node*>> N(nodes.size());
    std::vector<Edge*> MST;

    for (size_t i = 0; i < nodes.size(); ++i) {
        N[i].insert(nodes[i]);
    }

    for (size_t k = 1; k < nodes.size(); ++k) {
        int minCost = INT_MAX;
        Edge* selectedEdge = nullptr;

        for (size_t i = 0; i < N.size(); ++i) {
            if (N[i].empty()) continue;

            for (Node* x : N[i]) {
                for (Edge* edge : edges) {
                    Node* u = edge->getFirst();
                    Node* v = edge->getSecond();

                    if ((u == x && !isInSet(N[i], v)) || (v == x && !isInSet(N[i], u))) {
                        if (edge->getCost() < minCost) {
                            minCost = edge->getCost();
                            selectedEdge = edge;
                        }
                    }
                }
            }
        }

        if (!selectedEdge) {
            qDebug() << "Eroare: nu mai există muchii valide pentru MST!";
            break;
        }

        Edge* edgeCopy = new Edge(selectedEdge->getFirst(), selectedEdge->getSecond(), selectedEdge->getCost());
        MST.push_back(edgeCopy);

        Node* u = selectedEdge->getFirst();
        Node* v = selectedEdge->getSecond();
        size_t i = findComponent(N, u);
        size_t j = findComponent(N, v);

        if (i != j) {
            N[i].insert(N[j].begin(), N[j].end());
            N[j].clear();
        }
    }

    return MST;
}

bool Graph::isInSet(const std::set<Node*>& component, Node* node) const {
    return component.find(node) != component.end();
}

size_t Graph::findComponent(const std::vector<std::set<Node*>>& N, Node* node) const {
    for (size_t i = 0; i < N.size(); ++i) {
        if (N[i].find(node) != N[i].end()) {
            return i;
        }
    }
    return N.size();
}
void Graph::clearEdges() {

    for (auto edge : edges) {
        if (edge != nullptr) {
            delete edge;
        }
    }
    edges.clear();
}
//Complexitate O(n^2)
std::vector<Edge*> Graph::primMST() {
    int n = nodes.size();
    if (n == 0) return {};

    std::vector<int> key(n, INT_MAX);
    std::vector<bool> inMST(n, false);
    std::vector<int> parent(n, -1);

    key[0] = 0;
    parent[0] = -1;

    for (int count = 0; count < n - 1; ++count) {

        int minKey = INT_MAX, u = -1;
        for (int v = 0; v < n; ++v) {
            if (!inMST[v] && key[v] < minKey) {
                minKey = key[v];
                u = v;
            }
        }

        if (u == -1) break;

        inMST[u] = true;

        for (auto& edge : edges) {
            Node* startNode = edge->getFirst();
            Node* endNode = edge->getSecond();

            int startIdx = startNode->getValue();
            int endIdx = endNode->getValue();

            if (startIdx == u && !inMST[endIdx] && edge->getCost() < key[endIdx]) {
                key[endIdx] = edge->getCost();
                parent[endIdx] = u;
            }
            if (endIdx == u && !inMST[startIdx] && edge->getCost() < key[startIdx]) {
                key[startIdx] = edge->getCost();
                parent[startIdx] = u;
            }
        }
    }

    std::vector<Edge*> MST;
    for (int v = 1; v < n; ++v) {
        if (parent[v] != -1) {

            for (auto& edge : edges) {
                Node* startNode = edge->getFirst();
                Node* endNode = edge->getSecond();

                if ((startNode->getValue() == parent[v] && endNode->getValue() == v) ||
                    (endNode->getValue() == parent[v] && startNode->getValue() == v)) {
                    MST.push_back(new Edge(startNode, endNode, edge->getCost()));
                    break;
                }
            }
        }
    }

    return MST;
}
//varianta recursiva
/*int Graph::find(int u, std::vector<int>& parent) {
    if (u != parent[u]) {
        parent[u] = find(parent[u], parent); // Compresia drumului
    }
    return parent[u];
}*/
//varianta iterativa
int Graph::find(int u, std::vector<int>& parent) {
    int root = u;

    while (root != parent[root]) {
        root = parent[root];
    }

    while (u != root) {
        int next = parent[u];
        parent[u] = root;
        u = next;
    }

    return root;
}

void Graph::unite(int u, int v, std::vector<int>& parent, std::vector<int>& rank) {
    int rootU = find(u, parent);
    int rootV = find(v, parent);

    if (rootU != rootV) {
        if (rank[rootU] > rank[rootV]) {
            parent[rootV] = rootU;
        } else if (rank[rootU] < rank[rootV]) {
            parent[rootU] = rootV;
        } else {
            parent[rootV] = rootU;
            rank[rootU]++;
        }
    }
}
//Complexitate O(mlog(n))
std::vector<Edge*> Graph::kruskalMST() {

    std::sort(edges.begin(), edges.end(), [](Edge* a, Edge* b) {
        return a->getCost() < b->getCost();
    });

    std::vector<int> parent(nodes.size());
    std::vector<int> rank(nodes.size(), 0);

    for (size_t i = 0; i < nodes.size(); ++i) {
        parent[i] = i;
    }

    std::vector<Edge*> MST;
    for (auto& edge : edges) {
        int u = edge->getFirst()->getValue();
        int v = edge->getSecond()->getValue();

        if (find(u, parent) != find(v, parent)) {
            MST.push_back(new Edge(edge->getFirst(), edge->getSecond(), edge->getCost()));
            unite(u, v, parent, rank);
        }

        if (MST.size() == nodes.size() - 1) {
            break;
        }
    }

    return MST;
}
//Complexitate O(mlog(n))
std::vector<Edge*> Graph::boruvkaMST() {
    int n = nodes.size();
    if (n == 0) return {};

    std::vector<int> parent(n);
    std::vector<int> rank(n, 0);
    std::vector<Edge*> MST;

    for (int i = 0; i < n; ++i) {
        parent[i] = i;
    }

    int numTrees = n;

    while (numTrees > 1) {
        std::vector<Edge*> cheapest(n, nullptr);

        for (auto& edge : edges) {
            int u = edge->getFirst()->getValue();
            int v = edge->getSecond()->getValue();
            int setU = find(u, parent);
            int setV = find(v, parent);

            if (setU != setV) {
                if (cheapest[setU] == nullptr || edge->getCost() < cheapest[setU]->getCost()) {
                    cheapest[setU] = edge;
                }
                if (cheapest[setV] == nullptr || edge->getCost() < cheapest[setV]->getCost()) {
                    cheapest[setV] = edge;
                }
            }
        }

        for (int i = 0; i < n; ++i) {
            if (cheapest[i] != nullptr) {
                int u = cheapest[i]->getFirst()->getValue();
                int v = cheapest[i]->getSecond()->getValue();
                int setU = find(u, parent);
                int setV = find(v, parent);

                if (setU != setV) {
                    MST.push_back(cheapest[i]);
                    unionSets(setU, setV, parent, rank);
                    numTrees--;
                }
            }
        }
    }

    return MST;
}


int Graph::findForBoruvka(int u, std::vector<int>& parent) {
    while (u != parent[u]) {
        parent[u] = parent[parent[u]];
        u = parent[u];
    }
    return u;
}

void Graph::unionSets(int u, int v, std::vector<int>& parent, std::vector<int>& rank) {
    u = find(u, parent);
    v = find(v, parent);

    if (rank[u] < rank[v]) {
        parent[u] = v;
    } else if (rank[u] > rank[v]) {
        parent[v] = u;
    } else {
        parent[v] = u;
        rank[u]++;
    }
}

Node* Graph::getNodeByValue(int value) {
    for (Node* node : nodes) {
        if (node->getValue() == value) {
            return node;
        }
    }
    return nullptr;
}

