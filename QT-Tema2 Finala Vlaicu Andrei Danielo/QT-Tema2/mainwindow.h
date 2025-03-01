#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QVector>
#include <QPoint>
#include <queue>

struct QPointComparator {
    bool operator()(const QPoint& a, const QPoint& b) const {
        if (a.x() == b.x()) {
            return a.y() < b.y();
        }
        return a.x() < b.x();
    }
};

using Graph = std::map<QPoint, std::vector<QPoint>, QPointComparator>;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsView *graphicsView;
    QVector<QVector<int>> labirint;
    QPoint start;
    QVector<QPoint> exits;
    QVector<QVector<bool>> visited;
    void drawGraph();
    void loadLabirint(const QString &filename);
    void drawLabirint();
    void bfsFromExits();
    QVector<QPoint> findNeighbors(QPoint p);
    void drawPath(const QVector<QPoint> &path);
};

#endif // MAINWINDOW_H
