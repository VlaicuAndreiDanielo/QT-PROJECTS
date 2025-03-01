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
    QPushButton *generateMSTButton;
    QPushButton *Prim;
    QPushButton *Kruskal;
    QPushButton *Boruvka;
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
        radioButton->setGeometry(QRect(670, 10, 112, 26));
        generateMSTButton = new QPushButton(centralwidget);
        generateMSTButton->setObjectName("generateMSTButton");
        generateMSTButton->setGeometry(QRect(680, 500, 83, 29));
        Prim = new QPushButton(centralwidget);
        Prim->setObjectName("Prim");
        Prim->setGeometry(QRect(580, 500, 83, 29));
        Kruskal = new QPushButton(centralwidget);
        Kruskal->setObjectName("Kruskal");
        Kruskal->setGeometry(QRect(480, 500, 83, 29));
        Boruvka = new QPushButton(centralwidget);
        Boruvka->setObjectName("Boruvka");
        Boruvka->setGeometry(QRect(380, 500, 83, 29));
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
        generateMSTButton->setText(QCoreApplication::translate("MainWindow", "Generic", nullptr));
        Prim->setText(QCoreApplication::translate("MainWindow", "Prim", nullptr));
        Kruskal->setText(QCoreApplication::translate("MainWindow", "Kruskal", nullptr));
        Boruvka->setText(QCoreApplication::translate("MainWindow", "Boruvka", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
