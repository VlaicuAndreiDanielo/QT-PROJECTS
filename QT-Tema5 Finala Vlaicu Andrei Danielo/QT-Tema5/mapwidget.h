#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>
#include <QVector>
#include <QList>
#include <QPair>
#include <QPointF>

class MapWidget : public QWidget {
    Q_OBJECT

public:
    explicit MapWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    //QVector<QPointF> nodes;
    QVector<QPointF> scaledNodes; // Nodurile scalate
    QVector<QPair<int, int>> edges; // Muchiile
    QVector<double> costs; // Costurile muchiilor
    QVector<QList<QPair<int, double>>> adjacencyList; // Lista de adiacență
    QVector<int> selectedNodes; // Nodurile selectate la click
    QVector<QPointF> clickPoints; // Pozițiile unde utilizatorul a dat click
    QVector<int> shortestPath; // Drumul minim găsit
    bool isConnectedChecked = false; // Flag pentru verificarea conexității
    bool isGraphConnectedFlag = false; // Rezultatul verificării conexității
    bool isMapLoaded = false;
    // Funcții private
    bool isGraphConnected(const QVector<QPointF> &nodes, const QVector<QPair<int, int>> &edges);
    bool isGraphConnectedAdjec(const QVector<QList<QPair<int, double>>> &adjList);
    void readMap(QVector<QPointF> &nodes, QVector<QPair<int, int>> &edges, QVector<double> &costs);
    QVector<QList<QPair<int, double>>> buildAdjacencyList(const QVector<QPair<int, int>> &edges, const QVector<double> &costs, int numNodes);
    QVector<int> dijkstraAdjec(const QVector<QList<QPair<int, double>>> &adjList, int source, int target);
    QVector<int> dijkstra(const QVector<QPair<int, int>> &edges, const QVector<double> &costs, int numNodes, int source, int target);
    int findClosestNode(const QVector<QPointF> &scaledNodes, const QPointF &clickPosition);
};

#endif // MAPWIDGET_H
