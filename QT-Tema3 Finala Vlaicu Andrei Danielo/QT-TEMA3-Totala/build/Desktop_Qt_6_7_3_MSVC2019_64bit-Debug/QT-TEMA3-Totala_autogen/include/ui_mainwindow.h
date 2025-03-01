/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QRadioButton *radioButton;
    QPushButton *TopologicalOrder;
    QPushButton *findRootButton;
    QPushButton *ClearAll;
    QPushButton *ClearEdges;
    QPushButton *ConnectedComponents;
    QPushButton *StrongConnectedComponents;
    QPushButton *Undo;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        radioButton = new QRadioButton(centralwidget);
        radioButton->setObjectName("radioButton");
        radioButton->setGeometry(QRect(680, 10, 112, 26));
        TopologicalOrder = new QPushButton(centralwidget);
        TopologicalOrder->setObjectName("TopologicalOrder");
        TopologicalOrder->setGeometry(QRect(640, 520, 141, 29));
        findRootButton = new QPushButton(centralwidget);
        findRootButton->setObjectName("findRootButton");
        findRootButton->setGeometry(QRect(520, 520, 83, 29));
        ClearAll = new QPushButton(centralwidget);
        ClearAll->setObjectName("ClearAll");
        ClearAll->setGeometry(QRect(700, 60, 83, 29));
        ClearEdges = new QPushButton(centralwidget);
        ClearEdges->setObjectName("ClearEdges");
        ClearEdges->setGeometry(QRect(700, 100, 83, 29));
        ConnectedComponents = new QPushButton(centralwidget);
        ConnectedComponents->setObjectName("ConnectedComponents");
        ConnectedComponents->setGeometry(QRect(300, 520, 171, 29));
        StrongConnectedComponents = new QPushButton(centralwidget);
        StrongConnectedComponents->setObjectName("StrongConnectedComponents");
        StrongConnectedComponents->setGeometry(QRect(40, 520, 211, 29));
        Undo = new QPushButton(centralwidget);
        Undo->setObjectName("Undo");
        Undo->setGeometry(QRect(700, 140, 83, 29));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        radioButton->setText(QCoreApplication::translate("MainWindow", "Oriented", nullptr));
        TopologicalOrder->setText(QCoreApplication::translate("MainWindow", "TopologicalOrder", nullptr));
        findRootButton->setText(QCoreApplication::translate("MainWindow", "FindRoot", nullptr));
        ClearAll->setText(QCoreApplication::translate("MainWindow", "ClearAll", nullptr));
        ClearEdges->setText(QCoreApplication::translate("MainWindow", "ClearEdges", nullptr));
        ConnectedComponents->setText(QCoreApplication::translate("MainWindow", "ConnectedComponents", nullptr));
        StrongConnectedComponents->setText(QCoreApplication::translate("MainWindow", "StrongConnectedComponents", nullptr));
        Undo->setText(QCoreApplication::translate("MainWindow", "Undo", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
