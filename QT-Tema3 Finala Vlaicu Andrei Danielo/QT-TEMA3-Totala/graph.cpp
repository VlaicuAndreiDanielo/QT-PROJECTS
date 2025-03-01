#include "graph.h"
#include <fstream>
#include <iostream>

Graph::~Graph() {
    for (Node* node : nodes) {
        delete node;
    }
    for (Edge* edge : edges) {
        delete edge;
    }
}

void Graph::addNode(const QPoint& pos) {
    Node* newNode = new Node(nodes.size(), pos);
    nodes.push_back(newNode);

    changeHistory.push({ChangeType::AddNode, newNode, nullptr});
}

bool Graph::edgeExists(Node* first, Node* second) const {
    for (const Edge* edge : edges) {
        if ((edge->getFirst() == first && edge->getSecond() == second) ||
            (edge->getFirst() == second && edge->getSecond() == first)) {
            return true;
        }
    }
    return false;
}

bool Graph::respectsTopologicalOrder(Node* first, Node* second) const {

    std::unordered_map<int, int> topOrder;
    int index = 0;
    for (Node* node : getTopologicalOrder()) {
        topOrder[node->getValue()] = index++;
    }

    return topOrder[first->getValue()] < topOrder[second->getValue()];
}
void Graph::addEdge(Node* first, Node* second, bool isOriented) {
    if (!first || !second) {
        qDebug() << "First or second node is null!";
        return;
    }

    if (edgeExists(first, second)) {
        qDebug() << "Edge already exists";
        return;
    }

    Edge* newEdge = new Edge(first, second);
    edges.push_back(newEdge);
    first->addNeighbor(second);

    changeHistory.push({ChangeType::AddEdge, nullptr, newEdge});
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

bool Graph::dfs(Node* node, std::vector<int>& visited, std::stack<Node*>& topologicalOrder) {
    if (visited[node->getValue()] == 1) {
        return false;
    }
    if (visited[node->getValue()] == 2) {
        return true;
    }

    visited[node->getValue()] = 1;

    for (Node* neighbor : node->getNeighbors()) {
        if (!dfs(neighbor, visited, topologicalOrder)) {
            return false;
        }
    }

    visited[node->getValue()] = 2;
    topologicalOrder.push(node);

    return true;
}

bool Graph::isCircuit() {
    if (!isGraphOriented()) {
        qDebug() << "Graful nu este orientat. Nu se poate aplica sortare topologica.";
        return false;
    }

    std::vector<int> visited(nodes.size(), 0);
    std::stack<Node*> topologicalOrder;

    for (Node* node : nodes) {
        if (visited[node->getValue()] == 0) {
            if (!dfs(node, visited, topologicalOrder)) {
                return true;
            }
        }
    }
    return false;
}

void Graph::topologicalSort() {
    if (!isGraphOriented()) {
        qDebug() << "Graful nu este orientat. Nu se poate aplica sortare topologică.";
        return;
    }

    if (isCircuit()) {
        qDebug() << "Graful conține un ciclu. Sortarea topologică nu poate fi efectuată.";
        return;
    }

    std::vector<int> visited(nodes.size(), 0);
    std::stack<Node*> tempStack;

    for (Node* node : nodes) {
        if (visited[node->getValue()] == 0) {
            dfs(node, visited, tempStack);
        }
    }

    topologicalOrder = tempStack;
    std::stack<Node*> tempStackForSaving = topologicalOrder;
    std::vector<int> orderedNodes;
    while (!tempStackForSaving.empty()) {
        orderedNodes.push_back(tempStackForSaving.top()->getValue());
        tempStackForSaving.pop();
        saveTopologicalSort("topological_sort.txt");
    }
}


void Graph::saveTopologicalSort(const std::string& filename) {
    std::ofstream file(filename);
    std::stack<Node*> tempStack = topologicalOrder;

    if (!file.is_open()) {
        qDebug() << "Eroare la deschiderea fișierului:" << QString::fromStdString(filename);
        return;
    }

    while (!tempStack.empty()) {
        file << tempStack.top()->getValue() << " ";
        tempStack.pop();
    }

    file << std::endl;
    file.close();
    qDebug() << "Ordinea topologică a fost salvată în fișierul:" << QString::fromStdString(filename);
}

void Graph::drawGraph(QGraphicsScene* scene) {
    std::stack<Node*> sortedNodes = topologicalOrder;

    int xPos = 50;
    int yPos = 50;

    while (!sortedNodes.empty()) {
        Node* node = sortedNodes.top();
        sortedNodes.pop();

        QGraphicsEllipseItem* ellipse = scene->addEllipse(xPos, yPos, 30, 30);
        ellipse->setBrush(Qt::blue);
        scene->addText(QString::number(node->getValue()))->setPos(xPos + 5, yPos + 5);

        yPos += 50;
    }

    for (Edge* edge : edges) {
        Node* start = edge->getFirst();
        Node* end = edge->getSecond();

        scene->addLine(start->getPos().x(), start->getPos().y(),
                       end->getPos().x(), end->getPos().y(),
                       QPen(Qt::black));
    }
}

std::vector<Node*> Graph::getTopologicalOrder() const{
    if (topologicalOrder.empty()) {
        const_cast<Graph*>(this)->topologicalSort();
    }

    std::vector<Node*> orderedNodes;
    std::stack<Node*> tempStack = topologicalOrder;
    while (!tempStack.empty()) {
        orderedNodes.push_back(tempStack.top());
        tempStack.pop();
    }

    std::reverse(orderedNodes.begin(), orderedNodes.end());
    return orderedNodes;
}

void Graph::setEdges(const std::vector<Edge*>& newEdges) {
    edges = newEdges;
}
Node* Graph::getNodeByValue(int value) {
    for (Node* node : nodes) {
        if (node->getValue() == value) {
            return node;
        }
    }
    return nullptr;
}
void Graph::clear() {
    nodes.clear();
    edges.clear();
    topologicalOrder = std::stack<Node*>();
}
void Graph::loadTopologicalSort(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<Node*> nodesSorted;
    int nodeValue;

    while (file >> nodeValue) {

        Node* node = getNodeByValue(nodeValue);
        if (node) {
            nodesSorted.push_back(node);
        }
    }

    std::unordered_map<Node*, int> nodeIndex;
    for (int i = 0; i < nodesSorted.size(); ++i) {
        nodeIndex[nodesSorted[i]] = i;
    }

    this->nodeIndex = nodeIndex;
}

bool Graph::isCycle() {
    std::vector<int> visited(nodes.size(), 0);
    std::stack<Node*> topologicalOrder;

    for (Node* node : nodes) {
        if (visited[node->getValue()] == 0) {
            if (!dfs(node, visited, topologicalOrder)) {
                return true;
            }
        }
    }
    return false;
}
bool Graph::isTree() {
    if (isCycle()) {
        qDebug() << "Graful conține un ciclu și nu este un arbore.";
        return false;
    }

    if (edges.size() != nodes.size() - 1) {
        qDebug() << "Graful nu are numărul corect de muchii pentru a fi un arbore.";
        return false;
    }

    std::unordered_map<Node*, bool> visited;

    for (Node* node : nodes) {
        if (!visited[node]) {
            dfsConnected(node, visited);
        }
    }

    for (Node* node : nodes) {
        if (!visited[node]) {
            qDebug() << "Graful nu este conectat.";
            return false;
        }
    }

    return true;
}
void Graph::dfsConnected(Node* node, std::unordered_map<Node*, bool>& visited) {
    visited[node] = true;
    for (Node* neighbor : node->getNeighbors()) {
        if (visited[neighbor] == false) {
            dfsConnected(neighbor, visited);
        }
    }
}
Node* Graph::findRoot() {

    if (!isGraphOriented()) {
        qDebug() << "Graful nu este orientat.";
        return nullptr;
    }

    if (!isTree()) {
        qDebug() << "Graful nu este un arbore.";
        return nullptr;
    }

    if (edges.size() != nodes.size() - 1) {
        qDebug() << "Graful nu are numărul corect de muchii pentru a fi un arbore.";
        return nullptr;
    }

    Node* root = nullptr;
    int rootCount = 0;
    for (Node* node : nodes) {
        bool hasParent = false;
        for (Edge* edge : edges) {
            if (edge->getSecond() == node) {
                hasParent = true;
                break;
            }
        }
        if (!hasParent) {
            root = node;
            rootCount++;
        }
    }
    if (rootCount != 1) {
        qDebug() << "Graful nu are o singură rădăcină.";
        return nullptr;
    }

    return root;
}

void Graph::findConnectedComponents() {
    std::unordered_map<Node*, int> visited;
    std::unordered_map<int, QColor> componentColors;
    int componentId = 0;

    for (Node* node : nodes) {
        visited[node] = 0;
    }

    std::function<void(Node*, int)> dfsForComponents = [&](Node* node, int componentId) {
        visited[node] = componentId;

        for (Node* neighbor : node->getNeighbors()) {
            if (neighbor != nullptr && visited[neighbor] == 0) {
                dfsForComponents(neighbor, componentId);
            }
        }
    };

    for (Node* node : nodes) {
        if (visited[node] == 0) {
            dfsForComponents(node, componentId);
            componentId++;
        }
    }

    std::vector<int> parent(componentId);
    for (int i = 0; i < componentId; ++i) {
        parent[i] = i;
    }

    auto find = [&](int x) {
        while (x != parent[x]) {
            parent[x] = parent[parent[x]];
            x = parent[x];
        }
        return x;
    };

    auto unite = [&](int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        if (rootX != rootY) {
            parent[rootY] = rootX;
        }
    };

    for (int i = 0; i < componentId; ++i) {
        if (componentColors.find(i) == componentColors.end()) {
            componentColors[i] = generateRandomColor();
        }
    }

    for (Edge* edge : edges) {
        Node* node1 = edge->getFirst();
        Node* node2 = edge->getSecond();

        int comp1 = visited[node1];
        int comp2 = visited[node2];

        if (comp1 != comp2) {
            unite(comp1, comp2);
        }
    }

    std::unordered_map<int, QColor> unifiedColors;
    for (int i = 0; i < componentId; ++i) {
        int root = find(i);
        if (unifiedColors.find(root) == unifiedColors.end()) {
            unifiedColors[root] = componentColors[root];
        }
    }

    for (Node* node : nodes) {
        int compId = find(visited[node]);
        node->setComponentColor(unifiedColors[compId]);
    }

    std::unordered_set<std::pair<Node*, Node*>, pair_hash> processedEdges;

    for (Edge* edge : edges) {
        Node* node1 = edge->getFirst();
        Node* node2 = edge->getSecond();

        if (processedEdges.find({node1, node2}) == processedEdges.end() &&
            processedEdges.find({node2, node1}) == processedEdges.end()) {
            int compId = find(visited[node1]);
            edge->setComponentColor(unifiedColors[compId]);

            processedEdges.insert({node1, node2});
            processedEdges.insert({node2, node1});
        }
    }
}

void Graph::dfsForComponents(Node* node, int componentId, std::unordered_map<Node*, int>& visited) {
    visited[node] = componentId;
    for (Node* neighbor : node->getNeighbors()) {
        if (neighbor != nullptr && visited[neighbor] == 0) {
            dfsForComponents(neighbor, componentId, visited);
        }
    }
}

void Graph::dfsSCC(int node, std::vector<bool>& visited, std::stack<int>& finishStack) {
    visited[node] = true;
    for (Node* neighbor : nodes[node]->getNeighbors()) {
        if (!visited[neighbor->getValue()]) {
            dfsSCC(neighbor->getValue(), visited, finishStack);
        }
    }
    finishStack.push(node);
}

void Graph::dfsTranspose(int node, std::vector<bool>& visited, std::vector<int>& scc) {
    visited[node] = true;
    scc.push_back(node);

    for (Node* neighbor : transposedGraph[node]) {
        if (!visited[neighbor->getValue()]) {
            dfsTranspose(neighbor->getValue(), visited, scc);
        }
    }
}


std::vector<std::vector<int>> Graph::findSCCs() {
    std::stack<int> finishStack;
    std::vector<bool> visited(nodes.size(), false);

    for (int i = 0; i < nodes.size(); i++) {
        if (!visited[i]) {
            dfsSCC(i, visited, finishStack);
        }
    }

    transposedGraph.clear();
    transposedGraph.resize(nodes.size());
    for (Edge* edge : edges) {
        int u = edge->getFirst()->getValue();
        int v = edge->getSecond()->getValue();
        transposedGraph[v].push_back(nodes[u]);
    }

    std::fill(visited.begin(), visited.end(), false);
    std::vector<std::vector<int>> sccs;

    while (!finishStack.empty()) {
        int node = finishStack.top();
        finishStack.pop();

        if (!visited[node]) {
            std::vector<int> scc;
            dfsTranspose(node, visited, scc);
            sccs.push_back(scc);
        }
    }

    return sccs;
}

void Graph::drawSCCs(QGraphicsScene* scene) {
    std::vector<std::vector<int>> sccs = findSCCs();

    std::vector<QColor> colors = {Qt::red, Qt::blue, Qt::green, Qt::yellow, Qt::cyan, Qt::magenta};

    scene->clear();

    int xOffset = 50;
    int yOffset = 50;
    int colorIndex = 0;

    for (const auto& scc : sccs) {
        QColor color = colors[colorIndex % colors.size()];
        colorIndex++;

        for (int nodeIndex : scc) {
            Node* node = nodes[nodeIndex];
            QRectF rect(node->getPos().x() + xOffset, node->getPos().y() + yOffset, 30, 30);

            QGraphicsEllipseItem* ellipse = scene->addEllipse(rect, QPen(Qt::black), QBrush(color));
            scene->addText(QString::number(node->getValue() + 1))->setPos(rect.x() + 5, rect.y() + 5);
        }

        for (int nodeIndex : scc) {
            Node* node = nodes[nodeIndex];
            for (Node* neighbor : node->getNeighbors()) {
                if (std::find(scc.begin(), scc.end(), neighbor->getValue()) != scc.end()) {
                    scene->addLine(node->getPos().x() + xOffset + 15, node->getPos().y() + yOffset + 15,
                                   neighbor->getPos().x() + xOffset + 15, neighbor->getPos().y() + yOffset + 15,
                                   QPen(color));
                }
            }
        }

        xOffset += 150;
        if (xOffset > 600) {
            xOffset = 50;
            yOffset += 150;
        }
    }
}

void Graph::showSCCs() {
    QGraphicsScene* scene = new QGraphicsScene();

    drawSCCs(scene);

    QGraphicsView* view = new QGraphicsView(scene);
    view->setFixedSize(800, 600);
    view->setRenderHint(QPainter::Antialiasing);

    QMessageBox msgBox;
    msgBox.setWindowTitle("Componente Tare Conexe");
    msgBox.layout()->addWidget(view);
    msgBox.exec();
}

void Graph::clearAll() {
    nodes.clear();
    edges.clear();
}

void Graph::clearEdges() {
    edges.clear();
}

void Graph::undoLastChange() {
    if (changeHistory.empty()) {
        qDebug() << "No changes to undo.";
        return;
    }

    GraphChange lastChange = changeHistory.top();
    changeHistory.pop();

    switch (lastChange.type) {
    case ChangeType::AddNode:
        nodes.pop_back();
        delete lastChange.node;
        break;

    case ChangeType::AddEdge:
        edges.pop_back();
        delete lastChange.edge;
        break;
    }

    qDebug() << "Undo last change: " << (lastChange.type == ChangeType::AddNode ? "Node" : "Edge");
}
