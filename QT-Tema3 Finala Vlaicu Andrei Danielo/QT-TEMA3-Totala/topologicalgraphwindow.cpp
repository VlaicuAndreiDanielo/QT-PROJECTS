#include "topologicalgraphwindow.h"
#include "ui_topologicalgraphwindow.h"
#include <QPainter>

TopologicalGraphWindow::TopologicalGraphWindow(Graph& graph, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::TopologicalGraphWindow), m_graph(graph)
{
    ui->setupUi(this);
    setWindowTitle("Grafic Sortat Topologic");

    // Conectează butonul la funcția de sortare
    connect(ui->sortTopologicallyButton, &QPushButton::clicked, this, &TopologicalGraphWindow::sortTopologically);
}

TopologicalGraphWindow::~TopologicalGraphWindow()
{
    delete ui;
}

void TopologicalGraphWindow::sortTopologically()
{
    // Adaugă logica pentru sortarea topologică
    std::vector<Node*> topologicalOrder = m_graph.getTopologicalOrder();
    // Actualizează interfața (ex: afișează nodurile sortate)
}
