#ifndef UI_TOPOMAINWINDOW_H
#define UI_TOPOMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QWidget>

class QVBoxLayout;
class QPushButton;
class QLabel;

namespace Ui {
class TopologicalGraphWindow;
}

class Ui_TopologicalGraphWindow
{
public:
    QPushButton *sortTopologicallyButton;
    QLabel *statusLabel;

    void setupUi(QWidget *TopologicalGraphWindow)
    {
        if (TopologicalGraphWindow->objectName().isEmpty())
            TopologicalGraphWindow->setObjectName(QString::fromUtf8("TopologicalGraphWindow"));
        TopologicalGraphWindow->resize(400, 300);
        QVBoxLayout *verticalLayout = new QVBoxLayout(TopologicalGraphWindow);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));

        sortTopologicallyButton = new QPushButton(TopologicalGraphWindow);
        sortTopologicallyButton->setObjectName(QString::fromUtf8("sortTopologicallyButton"));
        sortTopologicallyButton->setText(QString::fromUtf8("Sort Topologically"));

        verticalLayout->addWidget(sortTopologicallyButton);

        statusLabel = new QLabel(TopologicalGraphWindow);
        statusLabel->setObjectName(QString::fromUtf8("statusLabel"));
        statusLabel->setText(QString::fromUtf8("Status:"));

        verticalLayout->addWidget(statusLabel);

        retranslateUi(TopologicalGraphWindow);

        QMetaObject::connectSlotsByName(TopologicalGraphWindow);
    }

    void retranslateUi(QWidget *TopologicalGraphWindow)
    {
        TopologicalGraphWindow->setWindowTitle(QCoreApplication::translate("TopologicalGraphWindow", "Topological Sort", nullptr));
    }
};

#endif // UI_TOPOMAINWINDOW_H
