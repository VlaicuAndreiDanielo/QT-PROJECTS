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
                        m_graph.addEdge(m_selectedNode, n, m_graph.isGraphOriented());
                        m_graph.saveAdjacencyMatrix();
                        update();
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
//varianta anterioara
/*void MainWindow::paintEvent(QPaintEvent *ev)
{
    QPainter p(this);

    std::vector<Node*> nodes = m_graph.getNodes();
    for (auto& n : nodes) {
        QRect r(n->getPos().x() - 10, n->getPos().y() - 10, 20, 20);
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
} */
void MainWindow::paintEvent(QPaintEvent *ev)
{
    QPainter p(this);

    // Desenarea nodurilor
    std::vector<Node*> nodes = m_graph.getNodes();
    for (auto& n : nodes) {
        QRect r(n->getPos().x() - 10, n->getPos().y() - 10, 20, 20);
        p.drawEllipse(r);
        QString str = QString::number(n->getValue() + 1);
        p.drawText(r, Qt::AlignCenter, str);
    }

    // Desenarea muchiilor
    std::vector<Edge*> edges = m_graph.getEdges();
    for (auto& e : edges) {
        QPoint start = e->getFirst()->getPos();
        QPoint end = e->getSecond()->getPos();

        double angle = atan2(end.y() - start.y(), end.x() - start.x());
        int nodeRadius = 10;

        // Ajustarea punctelor pentru a evita suprapunerea săgeților pe noduri
        QPoint adjustedStart = start + QPoint(nodeRadius * cos(angle), nodeRadius * sin(angle));
        QPoint adjustedEnd = end - QPoint(nodeRadius * cos(angle), nodeRadius * sin(angle));

        // Desenăm linia muchiei
        p.drawLine(adjustedStart, adjustedEnd);

        if (m_graph.isGraphOriented()) {
            // Desenăm săgeata pentru graf orientat
            int arrowSize = 10;

            // Calculăm punctele triunghiului pentru săgeată
            QPoint arrowP1 = adjustedEnd - QPoint(arrowSize * cos(angle + M_PI / 6), arrowSize * sin(angle + M_PI / 6));
            QPoint arrowP2 = adjustedEnd - QPoint(arrowSize * cos(angle - M_PI / 6), arrowSize * sin(angle - M_PI / 6));
            QPolygon arrowHead;
            arrowHead << adjustedEnd << arrowP1 << arrowP2;
            p.drawPolygon(arrowHead);
        }
    }

    // Actualizare pentru afișarea corectă
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
