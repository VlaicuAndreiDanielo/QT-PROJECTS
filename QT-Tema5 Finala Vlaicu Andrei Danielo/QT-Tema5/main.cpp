#include <QApplication>
#include "MapWidget.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MapWidget widget;
    widget.resize(1000, 800); // Dimensiunea ferestrei
    widget.show();

    return app.exec();
}
