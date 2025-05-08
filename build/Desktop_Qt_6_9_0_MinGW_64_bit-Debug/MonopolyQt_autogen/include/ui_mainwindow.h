/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGraphicsView *boardView;
    QPushButton *rollButton;
    QLabel *diceLabel;
    QPushButton *buyButton;
    QLabel *statusLabel;
    QGroupBox *playerGroup;
    QLabel *playerNameLabel;
    QLabel *moneyLabel;
    QGroupBox *propertyGroup;
    QLabel *propertyNameLabel;
    QLabel *propertyPriceLabel;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1259, 929);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        boardView = new QGraphicsView(centralwidget);
        boardView->setObjectName("boardView");
        boardView->setGeometry(QRect(10, 10, 1000, 1000));
        rollButton = new QPushButton(centralwidget);
        rollButton->setObjectName("rollButton");
        rollButton->setGeometry(QRect(990, 330, 121, 41));
        rollButton->setStyleSheet(QString::fromUtf8("QPushButton { background-color: #4CAF50; color: white; }"));
        diceLabel = new QLabel(centralwidget);
        diceLabel->setObjectName("diceLabel");
        diceLabel->setGeometry(QRect(990, 300, 121, 16));
        buyButton = new QPushButton(centralwidget);
        buyButton->setObjectName("buyButton");
        buyButton->setGeometry(QRect(990, 390, 121, 21));
        statusLabel = new QLabel(centralwidget);
        statusLabel->setObjectName("statusLabel");
        statusLabel->setGeometry(QRect(1110, 300, 131, 16));
        playerGroup = new QGroupBox(centralwidget);
        playerGroup->setObjectName("playerGroup");
        playerGroup->setGeometry(QRect(990, 10, 211, 131));
        playerNameLabel = new QLabel(playerGroup);
        playerNameLabel->setObjectName("playerNameLabel");
        playerNameLabel->setGeometry(QRect(0, 20, 101, 20));
        moneyLabel = new QLabel(playerGroup);
        moneyLabel->setObjectName("moneyLabel");
        moneyLabel->setGeometry(QRect(0, 40, 111, 16));
        propertyGroup = new QGroupBox(centralwidget);
        propertyGroup->setObjectName("propertyGroup");
        propertyGroup->setGeometry(QRect(990, 150, 211, 131));
        propertyNameLabel = new QLabel(propertyGroup);
        propertyNameLabel->setObjectName("propertyNameLabel");
        propertyNameLabel->setGeometry(QRect(0, 20, 241, 31));
        propertyPriceLabel = new QLabel(propertyGroup);
        propertyPriceLabel->setObjectName("propertyPriceLabel");
        propertyPriceLabel->setGeometry(QRect(0, 50, 81, 16));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1259, 21));
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
        rollButton->setText(QCoreApplication::translate("MainWindow", "Rol!", nullptr));
        diceLabel->setText(QCoreApplication::translate("MainWindow", "Dobbelsteen: 0", nullptr));
        buyButton->setText(QCoreApplication::translate("MainWindow", "Buy property", nullptr));
        statusLabel->setText(QCoreApplication::translate("MainWindow", "Ready", nullptr));
        playerGroup->setTitle(QCoreApplication::translate("MainWindow", "Player info", nullptr));
        playerNameLabel->setText(QCoreApplication::translate("MainWindow", "Player 1", nullptr));
        moneyLabel->setText(QCoreApplication::translate("MainWindow", "Money: $1500", nullptr));
        propertyGroup->setTitle(QCoreApplication::translate("MainWindow", "Property  info", nullptr));
        propertyNameLabel->setText(QCoreApplication::translate("MainWindow", "Select property", nullptr));
        propertyPriceLabel->setText(QCoreApplication::translate("MainWindow", "Price: $0", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
