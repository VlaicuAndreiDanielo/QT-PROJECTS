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
    connect(ui->generateMSTButton, &QPushButton::clicked, this, &MainWindow::onGenerateMSTButtonClicked);
    connect(ui->Prim, &QPushButton::clicked, this, &MainWindow::activatePrim);
    connect(ui->Kruskal, &QPushButton::clicked, this, &MainWindow::performKruskalMST);
    connect(ui->Boruvka,&QPushButton::clicked, this,&MainWindow::performBoruvkaMST);
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
                        int cost = QInputDialog::getInt(this, "Introduceți costul",
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

void MainWindow::paintEvent(QPaintEvent *ev)
{
    QPainter p(this);

    // Desenăm nodurile
    std::vector<Node*> nodes = m_graph.getNodes();
    for (auto& n : nodes) {
        QRect r(n->getPos().x() - 10, n->getPos().y() - 10, 20, 20);
        p.drawEllipse(r);
        QString str = QString::number(n->getValue() + 1);
        p.drawText(r, Qt::AlignCenter, str);
    }

    std::set<std::pair<int, int>> drawnEdges;

    std::vector<Edge*> edges = m_graph.getEdges();
    for (auto& e : edges) {
        int firstId = e->getFirst()->getValue();
        int secondId = e->getSecond()->getValue();

        if (!m_graph.isGraphOriented() && drawnEdges.count({secondId, firstId})) {
            continue;
        }

        drawnEdges.insert({firstId, secondId});

        QPoint start = e->getFirst()->getPos();
        QPoint end = e->getSecond()->getPos();

        double angle = atan2(end.y() - start.y(), end.x() - start.x());
        int nodeRadius = 10;

        QPoint adjustedStart = start + QPoint(nodeRadius * cos(angle), nodeRadius * sin(angle));
        QPoint adjustedEnd = end - QPoint(nodeRadius * cos(angle), nodeRadius * sin(angle));

        p.drawLine(adjustedStart, adjustedEnd);
        QPoint midPoint = (adjustedStart + adjustedEnd) / 2;
        QPoint offset(-10 * sin(angle), 10 * cos(angle));
        p.drawText(midPoint + offset, QString::number(e->getCost()));

        if (m_graph.isGraphOriented()) {
            int arrowSize = 10;
            QPoint arrowP1 = adjustedEnd - QPoint(arrowSize * cos(angle + M_PI / 6), arrowSize * sin(angle + M_PI / 6));
            QPoint arrowP2 = adjustedEnd - QPoint(arrowSize * cos(angle - M_PI / 6), arrowSize * sin(angle - M_PI / 6));
            QPolygon arrowHead;
            arrowHead << adjustedEnd << arrowP1 << arrowP2;
            p.drawPolygon(arrowHead);
        }
    }

    QWidget::paintEvent(ev);
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

void MainWindow::onGenerateMSTButtonClicked()
{

    std::vector<Edge*> mst = m_graph.genericMST();

    qDebug() << "Arborele de cost minim este:";
    for (auto& edge : mst) {
        qDebug() << "Nodul" << edge->getFirst()->getValue() << " -- "

                 << "Nodul" << edge->getSecond()->getValue()
                 << "[Cost:" << edge->getCost() << "]";
    }

    m_graph.clearEdges();
    for (auto& edge : mst) {
        m_graph.addEdge(edge->getFirst(), edge->getSecond(), m_graph.isGraphOriented(), edge->getCost());
    }
    update();
}
void MainWindow::activatePrim()
{

    std::vector<Edge*> mst = m_graph.primMST();

    qDebug() << "Arborele de cost minim este:";
    for (auto& edge : mst) {
        qDebug() << "Nodul" << edge->getFirst()->getValue() << " -- "

                 << "Nodul" << edge->getSecond()->getValue()
                 << "[Cost:" << edge->getCost() << "]";
    }

    m_graph.clearEdges();
    for (auto& edge : mst) {
        m_graph.addEdge(edge->getFirst(), edge->getSecond(), m_graph.isGraphOriented(), edge->getCost());
    }
    update();
}

void MainWindow::performKruskalMST()
{
    std::vector<Edge*> mstEdges = m_graph.kruskalMST();
    qDebug() << "Arborele de cost minim este:";
    for (auto& edge : mstEdges) {
        qDebug() << "Nodul" << edge->getFirst()->getValue() << " -- "

                 << "Nodul" << edge->getSecond()->getValue()
                 << "[Cost:" << edge->getCost() << "]";
    }
    m_graph.clearEdges();

    for (auto& edge : mstEdges) {
        m_graph.addEdge(edge->getFirst(), edge->getSecond(), false, edge->getCost());
    }

    update();
}
void MainWindow::performBoruvkaMST()
{
    std::vector<Edge*> mstEdges = m_graph.boruvkaMST();

    if (mstEdges.empty()) {
        qDebug() << "Graful este gol sau nu există MST.";
        return;
    }

    qDebug() << "Arborele de cost minim este:";
    for (auto& edge : mstEdges) {
        if (edge == nullptr || edge->getFirst() == nullptr || edge->getSecond() == nullptr) {
            qDebug() << "Eroare: Muchie invalidă sau noduri invalide în MST.";
            continue;
        }
        qDebug() << "Nodul" << edge->getFirst()->getValue() << " -- "
                 << "Nodul" << edge->getSecond()->getValue()
                 << "[Cost:" << edge->getCost() << "]";
    }

    std::vector<Edge*> oldEdges = m_graph.getEdges();

    for (auto& edge : oldEdges) {
        if (std::find(mstEdges.begin(), mstEdges.end(), edge) == mstEdges.end()) {
            if (edge != nullptr) {
                qDebug() << "Ștergem muchia: "
                         << edge->getFirst()->getValue() << " -- "
                         << edge->getSecond()->getValue();
                delete edge;
            }
        }
    }

    update();

    for (auto& edge : mstEdges) {
        if (edge == nullptr || edge->getFirst() == nullptr || edge->getSecond() == nullptr) {
            qDebug() << "Eroare: Muchie sau nod invalid!";
            continue;
        }
        qDebug() << "Muchie validă între nodurile:"
                 << edge->getFirst()->getValue() << "--"
                 << edge->getSecond()->getValue();
    }
}
