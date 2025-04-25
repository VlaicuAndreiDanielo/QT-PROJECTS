#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <set>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), m_selectedNode(nullptr), timer(new QTimer(this)),
    isFirstNodeSelected(false), isDragging(false), nodeDragged(nullptr),
    currentResidualGraphIndex(0), maxFlow(0)
{
    ui->setupUi(this);

    connect(ui->radioButton, &QRadioButton::toggled, this, &MainWindow::radioButtonToggled);
    connect(ui->GenerateMaxFlow, &QPushButton::pressed, this, &MainWindow::calculateMaxFlow);

    connect(timer, &QTimer::timeout, this, [this]() {
        if (currentResidualGraphIndex < residualGraphs.size()) {
            updateResidualGraph(residualGraphs[currentResidualGraphIndex]);
            currentResidualGraphIndex++;
        } else {
            timer->stop();
            QMessageBox::information(this, "Flux Maxim", "Fluxul maxim este: " + QString::number(maxFlow));
        }
    });
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

#include <QInputDialog>

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
                        bool ok;
                        int cost = QInputDialog::getInt(this, "Introduceti costul",
                                                        "Costul muchiei:", 1, 1, 1000, 1, &ok);
                        if (ok) {
                            m_graph.addEdge(m_selectedNode, n, m_graph.isGraphOriented(), cost);
                            m_graph.saveAdjacencyMatrix();
                            update();
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

void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    const int nodeRadius = 10;
    for (Edge* edge : m_graph.getEdges()) {
        Node* first = edge->getFirst();
        Node* second = edge->getSecond();

        QPointF p1 = first->getPos();
        QPointF p2 = second->getPos();
        double dx = p2.x() - p1.x();
        double dy = p2.y() - p1.y();
        double dist = std::sqrt(dx * dx + dy * dy);

        QPointF adjustedP1 = p1 + QPointF(dx / dist * nodeRadius, dy / dist * nodeRadius);
        QPointF adjustedP2 = p2 - QPointF(dx / dist * nodeRadius, dy / dist * nodeRadius);

        bool hasReverseEdge = false;
        for (Edge* reverseEdge : m_graph.getEdges()) {
            if (reverseEdge->getFirst() == second && reverseEdge->getSecond() == first) {
                hasReverseEdge = true;
                break;
            }
        }

        QPointF offset(0, 0);
        if (hasReverseEdge) {

            offset = QPointF(-dy / dist * 10, dx / dist * 10);
        }
        if (edge->isHighlighted()) {
            painter.setPen(QPen(Qt::red, 2));
        } else {
            painter.setPen(QPen(Qt::white, 1));
        }

        if (m_graph.isGraphOriented()) {

            painter.drawLine(adjustedP1 + offset, adjustedP2 + offset);
            QPointF arrowP1 = adjustedP2 + offset - QPointF(dx / dist * 10 - dy / dist * 5, dy / dist * 10 + dx / dist * 5);
            QPointF arrowP2 = adjustedP2 + offset - QPointF(dx / dist * 10 + dy / dist * 5, dy / dist * 10 - dx / dist * 5);
            painter.drawLine(adjustedP2 + offset, arrowP1);
            painter.drawLine(adjustedP2 + offset, arrowP2);

            if (hasReverseEdge) {
                painter.drawLine(adjustedP2 - offset, adjustedP1 - offset);
                QPointF reverseArrowP1 = adjustedP1 - offset - QPointF(-dx / dist * 10 - dy / dist * 5, -dy / dist * 10 + dx / dist * 5);
                QPointF reverseArrowP2 = adjustedP1 - offset - QPointF(-dx / dist * 10 + dy / dist * 5, -dy / dist * 10 - dx / dist * 5);
                painter.drawLine(adjustedP1 - offset, reverseArrowP1);
                painter.drawLine(adjustedP1 - offset, reverseArrowP2);
            }
        } else {
            painter.drawLine(adjustedP1, adjustedP2);
        }

        painter.drawText((adjustedP1 + adjustedP2) / 2 + offset, QString::number(edge->getCost()));
    }

    for (Node* node : m_graph.getNodes()) {
        QRectF rect(node->getPos().x() - nodeRadius, node->getPos().y() - nodeRadius, 2 * nodeRadius, 2 * nodeRadius);
        painter.setPen(Qt::white);
        painter.drawEllipse(rect);
        painter.drawText(rect, Qt::AlignCenter, QString::number(node->getValue() + 1));
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

void MainWindow::showResidualGraphs(const std::vector<std::vector<std::vector<int>>>& residualGraphs, int maxFlow) {

    static int currentIteration = 0;

    if (currentIteration >= residualGraphs.size()) {
        QMessageBox::information(this, "Flux Maxim", "Fluxul maxim este: " + QString::number(maxFlow));
        currentIteration = 0;
        return;
    }

    updateResidualGraph(residualGraphs[currentIteration]);

    QTimer::singleShot(5000, [this, &residualGraphs, maxFlow]() {
        currentIteration++;
        showResidualGraphs(residualGraphs, maxFlow);
    });
}

void MainWindow::calculateMaxFlow() {
    int source = 0;
    int sink = m_graph.getNodes().size() - 1;

    residualGraphs.clear();
    currentResidualGraphIndex = 0;

    maxFlow = m_graph.fordFulkerson(source, sink, this, residualGraphs);

    if (!residualGraphs.empty()) {
        timer->start(5000);
    } else {
        QMessageBox::information(this, "Flux Maxim", "Fluxul maxim este: " + QString::number(maxFlow));
    }


}

void MainWindow::updateResidualGraph(const std::vector<std::vector<int>>& residualGraph) {

    std::vector<bool> reachable = m_graph.findReachableNodesInResidualGraph(residualGraph, 0);

    qDebug() << "Reachable nodes:";
    for (int i = 0; i < reachable.size(); ++i) {
        qDebug() << "Node" << i + 1 << ":" << (reachable[i] ? "Yes" : "No");
    }

    m_graph.getEdges().clear();

    for (int u = 0; u < residualGraph.size(); ++u) {
        for (int v = 0; v < residualGraph[u].size(); ++v) {

            if (residualGraph[u][v] > 0) {
                Node* nodeU = m_graph.getNodeByValue(u);
                Node* nodeV = m_graph.getNodeByValue(v);

                Edge* edge = m_graph.addEdge(nodeU, nodeV, true, residualGraph[u][v]);
                qDebug() << "Added edge: Start:" << edge->getFirst()->getValue() + 1
                         << ", Finish:" << edge->getSecond()->getValue() + 1
                         << ", Cost:" << edge->getCost();

                if ((reachable[u] && !reachable[v]) || (!reachable[u] && reachable[v])) {
                    edge->setHighlight(true);
                    qDebug() << "Highlighted edge: Start:" << edge->getFirst()->getValue() + 1
                             << ", Finish:" << edge->getSecond()->getValue() + 1;
                } else {
                    qDebug() << "Edge not highlighted: Start:" << edge->getFirst()->getValue() + 1
                             << ", Finish:" << edge->getSecond()->getValue() + 1
                             << ", Reachable U:" << reachable[u]
                             << ", Reachable V:" << reachable[v];
                }
            }

            if (residualGraph[v][u] > 0) {
                Node* nodeV = m_graph.getNodeByValue(v);
                Node* nodeU = m_graph.getNodeByValue(u);

                Edge* reverseEdge = m_graph.addEdge(nodeV, nodeU, true, residualGraph[v][u]);
                qDebug() << "Added reverse edge: Start:" << reverseEdge->getFirst()->getValue() + 1
                         << ", Finish:" << reverseEdge->getSecond()->getValue() + 1
                         << ", Cost:" << reverseEdge->getCost();

                if ((reachable[v] && !reachable[u]) || (!reachable[v] && reachable[u])) {
                    reverseEdge->setHighlight(true);
                    qDebug() << "Highlighted reverse edge: Start:" << reverseEdge->getFirst()->getValue() + 1
                             << ", Finish:" << reverseEdge->getSecond()->getValue() + 1;
                } else {
                    qDebug() << "Reverse edge not highlighted: Start:" << reverseEdge->getFirst()->getValue() + 1
                             << ", Finish:" << reverseEdge->getSecond()->getValue() + 1
                             << ", Reachable U:" << reachable[u]
                             << ", Reachable V:" << reachable[v];
                }
            }
        }
    }

    m_graph.saveEdgesToFile("UltimulGrafRezidual.txt");

    update();
}
