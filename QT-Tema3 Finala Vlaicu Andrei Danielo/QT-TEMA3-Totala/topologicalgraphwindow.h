#ifndef TOPOLOGICALGRAPHWINDOW_H
#define TOPOLOGICALGRAPHWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include "graph.h"  // Include clasa Graph pentru a accesa datele graficului

namespace Ui {
class TopologicalGraphWindow;
}

class TopologicalGraphWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TopologicalGraphWindow(Graph& graph, QWidget *parent = nullptr);
    ~TopologicalGraphWindow();

protected:
    void paintEvent(QPaintEvent *event) override;  // Funcție pentru desenarea graficului

private:
    Ui::TopologicalGraphWindow *ui;
    Graph& m_graph;  // Referință la obiectul Graph
};

#endif // TOPOLOGICALGRAPHWINDOW_H
