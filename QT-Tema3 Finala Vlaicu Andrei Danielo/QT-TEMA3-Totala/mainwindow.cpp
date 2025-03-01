#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), m_selectedNode(nullptr)
{
    ui->setupUi(this);
    isFirstNodeSelected = false;
    isDragging = false;
    nodeDragged = nullptr;

    connect(ui->radioButton, &QRadioButton::toggled, this, &MainWindow::radioButtonToggled);
    connect(ui->TopologicalOrder, &QPushButton::clicked, this, &MainWindow::performTopologicalSort);
    connect(ui->findRootButton, &QPushButton::clicked, this, &MainWindow::findAndDisplayRoot);

    connect(ui->ConnectedComponents, &QPushButton::clicked, this, &MainWindow::findAndDisplayConnectedComponents);
    connect(ui->StrongConnectedComponents, &QPushButton::clicked, this, &MainWindow::findAndDisplayStrogConnectedComponents);

     connect(ui->ClearAll, &QPushButton::clicked, this, &MainWindow::ClearAll);
     connect(ui->ClearEdges, &QPushButton::clicked, this, &MainWindow::ClearEdges);
     connect(ui->Undo, &QPushButton::clicked, this, &MainWindow::undoChange);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::radioButtonToggled(bool checked)
{
    m_graph.setOriented(checked);
    update();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *m)
{
    if (isDragging && m->button() == Qt::LeftButton && (m->modifiers() & Qt::ShiftModifier)) {
        isDragging = false;
        nodeDragged = nullptr;
        update();
    }

    if (m->button() == Qt::LeftButton) {
        bool overlap = false;
        for (auto& n : m_graph.getNodes()) {
            if (std::hypot(n->getPos().x() - m->pos().x(), n->getPos().y() - m->pos().y()) < 30) {
                overlap = true;
                break;
            }
        }

        if (!overlap) {
            m_graph.addNode(m->pos());
            m_graph.saveAdjacencyMatrix();
        }
        update();
    }

    if (m->button() == Qt::RightButton) {
        std::vector<Node*> nodes = m_graph.getNodes();
        for (auto& n : nodes) {
            if ((n->getPos().x() - m->pos().x()) * (n->getPos().x() - m->pos().x()) +
                    (n->getPos().y() - m->pos().y()) * (n->getPos().y() - m->pos().y()) < 100) {
                if (isFirstNodeSelected) {
                    if (m_selectedNode->getValue() != n->getValue()) {
                        if (!m_graph.edgeExists(m_selectedNode, n)) {
                            m_graph.addEdge(m_selectedNode, n, m_graph.isGraphOriented());
                            m_graph.saveAdjacencyMatrix();
                            update();
                        } else {
                            qDebug() << "Edge already exists.";
                        }
                    } else {
                        qDebug() << "Cannot create an edge to the same node.";
                    }
                    isFirstNodeSelected = false;
                } else {
                    isFirstNodeSelected = true;
                    m_selectedNode = n;
                    update();
                }
                break;
            }
        }
    }
}

void MainWindow::paintEvent(QPaintEvent *ev)
{
    QPainter p(this);

    std::vector<Node*> nodes = m_graph.getNodes();

    for (auto& n : nodes) {
        QRect r(n->getPos().x() - 10, n->getPos().y() - 10, 20, 20);

        QColor nodeColor = n->getComponentColor();
        if (nodeColor.isValid()) {
            p.setPen(QPen(nodeColor));
        } else {
            p.setPen(QPen(Qt::white));
        }

        p.drawEllipse(r);

        QString str = QString::number(n->getValue() + 1);
        p.drawText(r, Qt::AlignCenter, str);
    }

    std::vector<Edge*> edges = m_graph.getEdges();
    for (auto& e : edges) {
        QPoint start = e->getFirst()->getPos();
        QPoint end = e->getSecond()->getPos();
        double angle = atan2(end.y() - start.y(), end.x() - start.x());
        int nodeRadius = 10;

        QPoint adjustedStart = start + QPoint(nodeRadius * cos(angle), nodeRadius * sin(angle));
        QPoint adjustedEnd = end - QPoint(nodeRadius * cos(angle), nodeRadius * sin(angle));

        QColor edgeColor = e->getFirst()->getComponentColor();
        if (edgeColor.isValid()) {
            p.setPen(QPen(edgeColor, 2));
        } else {
            p.setPen(QPen(Qt::white, 2));
        }

        p.drawLine(adjustedStart, adjustedEnd);

        if (m_graph.isGraphOriented()) {
            int arrowSize = 10;
            QPoint arrowP1 = adjustedEnd - QPoint(arrowSize * cos(angle + M_PI / 6), arrowSize * sin(angle + M_PI / 6));
            QPoint arrowP2 = adjustedEnd - QPoint(arrowSize * cos(angle - M_PI / 6), arrowSize * sin(angle - M_PI / 6));
            QPolygon arrowHead;
            arrowHead << adjustedEnd << arrowP1 << arrowP2;
            p.drawPolygon(arrowHead);
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *m)
{
    if (isDragging && nodeDragged) {
        QPoint newMousePos = m->pos();

        bool overlap = false;
        for (const auto& n : m_graph.getNodes()) {
            if (n != nodeDragged && std::hypot(n->getPos().x() - newMousePos.x(), n->getPos().y() - newMousePos.y()) < 30) {
                overlap = true;
                break;
            }
        }
        if (!overlap) {
            nodeDragged->setPos(newMousePos);
            update();
        } else {
            qDebug() << "Overlap detected, cannot move node.";
        }
    }
}
void MainWindow::mousePressEvent(QMouseEvent *m)
{
    if (m->button() == Qt::LeftButton && (m->modifiers() & Qt::ShiftModifier)) {
        for (auto& n : m_graph.getNodes()) {
            if (std::hypot(n->getPos().x() - m->pos().x(), n->getPos().y() - m->pos().y()) < 30) {
                isDragging = true;
                nodeDragged = n;
                initialPositionOfMouseOnNodeSelected = m->pos();
                break;
            }
        }
    }
}

void MainWindow::toggleConnectedComponents() {
    m_showConnectedComponents = !m_showConnectedComponents;
    update();
}

void MainWindow::performTopologicalSort() {

    std::vector<Node*> topologicalOrder = m_graph.getTopologicalOrder();

    if (topologicalOrder.empty()) {
        qDebug() << "Nu există o ordine topologică validă sau graful conține un ciclu.";
        QMessageBox::warning(this, "Eroare", "Nu există o ordine topologică validă sau graful conține un ciclu.");
        return;
    }
    m_graph.saveTopologicalSort("topological_order.txt");

    QFile outFile("topological_order.txt");
    if (outFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&outFile);

        QString topologicalOrderString;
        for (Node* node : topologicalOrder) {
            topologicalOrderString += QString::number(node->getValue()) + " ";
        }
        out << topologicalOrderString;
        outFile.close();
    } else {
        QMessageBox::warning(this, "Eroare", "Nu s-a putut salva ordinea topologică în fișier.");
        return;
    }

    std::unordered_map<Node*, int> nodeIndex;
    for (int i = 0; i < topologicalOrder.size(); ++i) {
        nodeIndex[topologicalOrder[i]] = i;
        qDebug() << "Nod:" << topologicalOrder[i]->getValue() << " Index:" << i;
    }

    std::vector<Edge*> edgesToKeep;
    for (auto& edge : m_graph.getEdges()) {
        Node* startNode = edge->getFirst();
        Node* endNode = edge->getSecond();

        qDebug() << "Verificăm muchia: " << startNode->getValue() << " -> " << endNode->getValue();
        if (nodeIndex[startNode] < nodeIndex[endNode]) {
            edgesToKeep.push_back(edge);
            qDebug() << "Muchia păstrată.";
        } else {
            qDebug() << "Muchie ștearsă între nodurile: "
                     << startNode->getValue() << " -> " << endNode->getValue();
        }
    }

    m_graph.setEdges(edgesToKeep);
    m_graph.saveAdjacencyMatrix();

    QString topologicalOrderStringFromFile;
    QFile inFile("topological_order.txt");

    if (inFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&inFile);
        topologicalOrderStringFromFile = in.readLine();
        qDebug() << "Sortarea topologică citită din fișier:" << topologicalOrderStringFromFile;
        inFile.close();
    } else {
        QMessageBox::warning(this, "Eroare", "Nu s-a putut citi ordinea topologică din fișier.");
        return;
    }

    topologicalOrderStringFromFile = reverseString(topologicalOrderStringFromFile);

    QMessageBox::information(this, "Sortare Topologică", "Ordinea topologică a nodurilor este:\n" + topologicalOrderStringFromFile);
    update();
}

QString MainWindow::reverseString(const QString& input) {
    QString reversedString;
    for (int i = input.length() - 1; i >= 0; --i) {
        reversedString.append(input[i]);
    }
    return reversedString;
}
QString MainWindow::readFirstLineFromFile(const QString& fileName) {
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Nu s-a putut deschide fișierul pentru citire.";
        return "";
    }

    QTextStream in(&file);
    QString firstLine = in.readLine();

    file.close();

    return firstLine;
}
void MainWindow::findAndDisplayRoot() {
    Node* rootNode = m_graph.findRoot();
    if (rootNode) {
        qDebug() << "Rădăcina grafului este nodul:" << rootNode->getValue() + 1;
    } else {
        qDebug() << "Nu există rădăcină sau graful nu este un arbore.";
    }
}

void MainWindow::findAndDisplayConnectedComponents() {

    m_graph.findConnectedComponents();
    update();
}

void MainWindow::ClearAll()
{
    m_graph.clearAll();
    m_graph.saveTopologicalSort("topological_order.txt");
    qDebug() << "Graful a fost șters și fișierul a fost actualizat.";
    update();
}

void MainWindow::ClearEdges()
{
    m_graph.clearEdges();
    m_graph.saveTopologicalSort("topological_order.txt");
    update();
}

void MainWindow::undoChange()
{
    m_graph.undoLastChange();
    m_graph.saveTopologicalSort("topological_order.txt");
    update();
}

void MainWindow::findAndDisplayStrogConnectedComponents()
{
    std::vector<std::vector<int>> sccIndices = m_graph.findSCCs();

    std::vector<std::vector<Node*>> sccs;
    const std::vector<Node*>& nodes = m_graph.getNodes();
    if(m_graph.isGraphOriented()==false)
        {
            QMessageBox::information(this, "SCC", "Graful nu este orientat.");
            return;
        }
    for (const auto& component : sccIndices) {
        std::vector<Node*> sccComponent;
        for (int index : component) {
            sccComponent.push_back(nodes[index]);
        }
        sccs.push_back(sccComponent);
    }

    if (sccs.empty()) {
        QMessageBox::information(this, "SCC", "Graful nu conține componente tare conexe.");
        return;
    }

    std::vector<QColor> colors = {Qt::red, Qt::blue, Qt::green, Qt::yellow, Qt::cyan, Qt::magenta, Qt::white, Qt::lightGray, Qt::darkBlue, Qt::darkRed,
                                  Qt::darkCyan, Qt::darkGreen, Qt::darkMagenta, Qt::darkYellow};
    int colorIndex = 0;

    for (const auto& component : sccs) {
        QColor color = colors[colorIndex % colors.size()];
        for (Node* node : component) {
            node->setComponentColor(color);
        }
        colorIndex++;
    }

    update();

    QString sccMessage = "Componentele tare conexe sunt:\n";
    for (const auto& component : sccs) {
        sccMessage += "{ ";
        for (Node* node : component) {
            sccMessage += QString::number(node->getValue() + 1) + " ";
        }
        sccMessage += "}\n";
    }
    QMessageBox::information(this, "SCC", sccMessage);
}
