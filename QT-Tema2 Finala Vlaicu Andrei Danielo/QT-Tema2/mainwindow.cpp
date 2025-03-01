#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QGraphicsRectItem>
#include <QBrush>
#include <QPen>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , scene(new QGraphicsScene(this))
{
    ui->setupUi(this);


    graphicsView = new QGraphicsView(scene, this);
    graphicsView->setGeometry(10, 10, 500, 500);
    graphicsView->show();

    loadLabirint("labirint.txt");
    drawLabirint();
    bfsFromExits();
}

MainWindow::~MainWindow() {
    delete ui;
    delete graphicsView;
}

void MainWindow::drawGraph() {
    int cellSize = 30;
    int graphOffsetX = labirint[0].size() * cellSize + 20;


    for (int i = 0; i < labirint.size(); ++i) {
        for (int j = 0; j < labirint[i].size(); ++j) {

            if (labirint[i][j] != 0) {

                int x = j * cellSize + cellSize / 2 + graphOffsetX;
                int y = i * cellSize + cellSize / 2;


                QColor nodeColor;
                if (labirint[i][j] == 1) {
                    nodeColor = Qt::white;
                } else if (labirint[i][j] == 3) {
                    nodeColor = Qt::blue;
                } else if (labirint[i][j] == 2) {
                    nodeColor = Qt::red;
                } else {
                    continue;
                }


                QGraphicsEllipseItem *node = scene->addEllipse(x - 10, y - 10, 25, 25, QPen(Qt::black), QBrush(nodeColor));


                QVector<QPoint> neighbors = findNeighbors(QPoint(i, j));


                for (const QPoint& neighbor : neighbors) {

                    int x1 = j * cellSize + cellSize / 2 + graphOffsetX;
                    int y1 = i * cellSize + cellSize / 2;
                    int x2 = neighbor.y() * cellSize + cellSize / 2 + graphOffsetX;
                    int y2 = neighbor.x() * cellSize + cellSize / 2;


                    QGraphicsLineItem *line = scene->addLine(x1, y1, x2, y2, QPen(Qt::black));
                }
            }
        }
    }
}



void MainWindow::loadLabirint(const QString &filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    int row = 0;
    while (!in.atEnd()) {
        QString line = in.readLine();
        QVector<int> rowValues;
        for (int col = 0; col < line.size(); ++col) {
            int value = line[col].digitValue();
            rowValues.append(value);
            if (value == 3) start = QPoint(row, col);
            if (value == 2) exits.append(QPoint(row, col));
        }
        labirint.append(rowValues);
        ++row;
    }

    visited = QVector<QVector<bool>>(labirint.size(), QVector<bool>(labirint[0].size(), false));
}

void MainWindow::drawLabirint() {
    int cellSize = 30;


    for (int i = 0; i < labirint.size(); ++i) {
        for (int j = 0; j < labirint[i].size(); ++j) {
            QRect rect(j * cellSize, i * cellSize, cellSize, cellSize);
            QGraphicsRectItem *cell = scene->addRect(rect);

            if (labirint[i][j] == 0) {
                cell->setBrush(Qt::black);
            } else if (labirint[i][j] == 1) {
                cell->setBrush(Qt::white);
            } else if (labirint[i][j] == 3) {
                cell->setBrush(Qt::blue);
            } else if (labirint[i][j] == 2) {
                cell->setBrush(Qt::red);
            }

            cell->setPen(QPen(Qt::black));
        }
    }


    drawGraph();
}

QVector<QPoint> MainWindow::findNeighbors(QPoint p) {
    QVector<QPoint> neighbors;
    QVector<QPoint> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

    for (const QPoint &dir : directions) {
        QPoint neighbor = p + dir;
        if (neighbor.x() >= 0 && neighbor.y() >= 0 &&
            neighbor.x() < labirint.size() &&
            neighbor.y() < labirint[0].size() &&
            labirint[neighbor.x()][neighbor.y()] != 0) {
            neighbors.append(neighbor);
        }
    }

    return neighbors;
}

void MainWindow::bfsFromExits() {

    for (const QPoint &exit : exits) {
        std::queue<QPoint> q;
        QVector<QPoint> path;
        QVector<QVector<bool>> localVisited(labirint.size(), QVector<bool>(labirint[0].size(), false));
        QVector<QVector<QPoint>> cameFrom(labirint.size(), QVector<QPoint>(labirint[0].size()));

        q.push(exit);
        localVisited[exit.x()][exit.y()] = true;

        bool pathFound = false;

        while (!q.empty() && !pathFound) {
            QPoint current = q.front();
            q.pop();


            if (current == start) {
                QPoint temp = current;
                path.append(temp);


                while (temp != exit) {
                    temp = cameFrom[temp.x()][temp.y()];
                    path.append(temp);
                }
                std::reverse(path.begin(), path.end());
                drawPath(path);
                pathFound = true;
                break;
            }


            for (const QPoint &neighbor : findNeighbors(current)) {
                if (!localVisited[neighbor.x()][neighbor.y()]) {
                    localVisited[neighbor.x()][neighbor.y()] = true;
                    cameFrom[neighbor.x()][neighbor.y()] = current;
                    q.push(neighbor);
                }
            }
        }
    }
}

void MainWindow::drawPath(const QVector<QPoint> &path) {
    int cellSize = 30;


    for (int i = 0; i < labirint.size(); ++i) {
        for (int j = 0; j < labirint[i].size(); ++j) {
            if (visited[i][j] && labirint[i][j] != 2 && labirint[i][j] != 3) {

                QRect rect(j * cellSize, i * cellSize, cellSize, cellSize);
                QGraphicsRectItem *cell = scene->addRect(rect);
                cell->setBrush(Qt::green);
                cell->setPen(QPen(Qt::black));
            }
        }
    }


    for (const QPoint &p : path) {
        QRect rect(p.y() * cellSize, p.x() * cellSize, cellSize, cellSize);
        QGraphicsRectItem *cell = scene->addRect(rect);


        if (labirint[p.x()][p.y()] == 2) {
            cell->setBrush(Qt::red);
        }

        else if (p == start) {
            cell->setBrush(Qt::blue);
        }

        else {
            cell->setBrush(Qt::green);
        }

        cell->setPen(QPen(Qt::black));
    }
}
