#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QRadioButton>
#include <QKeyEvent>
#include <QDebug>
#include <QMessageBox>
#include <QTimer>
#include "graph.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void mouseReleaseEvent(QMouseEvent* m) override;
    void paintEvent(QPaintEvent* ev) override;
    void mouseMoveEvent(QMouseEvent *m) override;
    void mousePressEvent(QMouseEvent *m) override;
    void radioButtonToggled(bool checked);

    void calculateMaxFlow();
    void updateResidualGraph(const std::vector<std::vector<int>>& residualGraph);
    void showResidualGraphs(const std::vector<std::vector<std::vector<int>>>& residualGraphs, int maxFlow);

private:
    Ui::MainWindow *ui;
    QTimer* timer;
    Graph m_graph;
    Node* m_selectedNode;
    bool isFirstNodeSelected;
    bool isDragging;
    Node* nodeDragged = nullptr;
    QPoint initialPositionOfMouseOnNodeSelected;

    std::vector<std::vector<std::vector<int>>> residualGraphs;
    int currentResidualGraphIndex;
    int maxFlow;
};
#endif // MAINWINDOW_H
