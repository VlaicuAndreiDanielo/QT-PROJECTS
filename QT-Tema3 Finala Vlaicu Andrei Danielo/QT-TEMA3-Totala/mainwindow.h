#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QRadioButton>
#include <QTextStream>
#include <QKeyEvent>
#include <QDebug>
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
    void performTopologicalSort();
    void findAndDisplayRoot();
    void findAndDisplayConnectedComponents();
    void toggleConnectedComponents();
    void findAndDisplayStrogConnectedComponents();

    void ClearAll();
    void ClearEdges();
    void undoChange();

    QString readFirstLineFromFile(const QString& fileName);
    QString reverseString(const QString& input);

private:
    Ui::MainWindow *ui;
    Graph m_graph;
    Node* m_selectedNode;
    bool isFirstNodeSelected;
    bool isDragging;
    Node* nodeDragged = nullptr;
    QPoint initialPositionOfMouseOnNodeSelected;
    bool m_showConnectedComponents = false;

};

#endif // MAINWINDOW_H
