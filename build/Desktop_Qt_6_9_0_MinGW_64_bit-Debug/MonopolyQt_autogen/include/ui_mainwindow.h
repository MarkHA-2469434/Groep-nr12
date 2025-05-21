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
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *buyButton;
    QLabel *diceLabel;
    QGraphicsView *boardView;
    QLabel *statusLabel;
    QPushButton *rollButton;
    QGroupBox *playerGroup;
    QLabel *playerNameLabel;
    QLabel *moneyLabel;
    QLabel *KansAlg;
    QLabel *BalanceLabel;
    QLabel *LoonLabel;
    QGroupBox *propertyGroup;
    QLabel *propertyNameLabel;
    QLabel *propertyPriceLabel;
    QLabel *propertyOwner;
    QLabel *propertyOwnerLabel;
    QLabel *propertyPrice;
    QPushButton *EndTurnButton;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->setEnabled(true);
        MainWindow->resize(1382, 1000);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        centralwidget->setEnabled(true);
        buyButton = new QPushButton(centralwidget);
        buyButton->setObjectName("buyButton");
        buyButton->setGeometry(QRect(1040, 470, 101, 20));
        diceLabel = new QLabel(centralwidget);
        diceLabel->setObjectName("diceLabel");
        diceLabel->setGeometry(QRect(910, 440, 231, 16));
        boardView = new QGraphicsView(centralwidget);
        boardView->setObjectName("boardView");
        boardView->setGeometry(QRect(0, 0, 880, 880));
        statusLabel = new QLabel(centralwidget);
        statusLabel->setObjectName("statusLabel");
        statusLabel->setGeometry(QRect(910, 560, 231, 31));
        rollButton = new QPushButton(centralwidget);
        rollButton->setObjectName("rollButton");
        rollButton->setGeometry(QRect(910, 470, 121, 81));
        rollButton->setStyleSheet(QString::fromUtf8("QPushButton { background-color: #4CAF50; color: white; }"));
        playerGroup = new QGroupBox(centralwidget);
        playerGroup->setObjectName("playerGroup");
        playerGroup->setGeometry(QRect(890, 10, 191, 161));
        playerNameLabel = new QLabel(playerGroup);
        playerNameLabel->setObjectName("playerNameLabel");
        playerNameLabel->setGeometry(QRect(10, 20, 101, 20));
        moneyLabel = new QLabel(playerGroup);
        moneyLabel->setObjectName("moneyLabel");
        moneyLabel->setGeometry(QRect(10, 40, 201, 20));
        KansAlg = new QLabel(playerGroup);
        KansAlg->setObjectName("KansAlg");
        KansAlg->setGeometry(QRect(10, 60, 181, 51));
        BalanceLabel = new QLabel(playerGroup);
        BalanceLabel->setObjectName("BalanceLabel");
        BalanceLabel->setGeometry(QRect(80, 40, 51, 16));
        LoonLabel = new QLabel(playerGroup);
        LoonLabel->setObjectName("LoonLabel");
        LoonLabel->setGeometry(QRect(6, 110, 141, 20));
        propertyGroup = new QGroupBox(centralwidget);
        propertyGroup->setObjectName("propertyGroup");
        propertyGroup->setGeometry(QRect(900, 200, 181, 221));
        propertyNameLabel = new QLabel(propertyGroup);
        propertyNameLabel->setObjectName("propertyNameLabel");
        propertyNameLabel->setGeometry(QRect(0, 30, 191, 16));
        propertyPriceLabel = new QLabel(propertyGroup);
        propertyPriceLabel->setObjectName("propertyPriceLabel");
        propertyPriceLabel->setGeometry(QRect(0, 100, 221, 31));
        propertyOwner = new QLabel(propertyGroup);
        propertyOwner->setObjectName("propertyOwner");
        propertyOwner->setGeometry(QRect(0, 70, 241, 20));
        propertyOwnerLabel = new QLabel(propertyGroup);
        propertyOwnerLabel->setObjectName("propertyOwnerLabel");
        propertyOwnerLabel->setGeometry(QRect(0, 60, 171, 31));
        propertyPrice = new QLabel(propertyGroup);
        propertyPrice->setObjectName("propertyPrice");
        propertyPrice->setGeometry(QRect(0, 130, 171, 20));
        EndTurnButton = new QPushButton(centralwidget);
        EndTurnButton->setObjectName("EndTurnButton");
        EndTurnButton->setGeometry(QRect(1040, 500, 101, 51));
        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Monopoly Game", nullptr));
        buyButton->setText(QCoreApplication::translate("MainWindow", "Buy property", nullptr));
        diceLabel->setText(QCoreApplication::translate("MainWindow", "Dobbelsteen: 0", nullptr));
        statusLabel->setText(QCoreApplication::translate("MainWindow", "Ready", nullptr));
        rollButton->setText(QCoreApplication::translate("MainWindow", "Rol!", nullptr));
        playerGroup->setTitle(QCoreApplication::translate("MainWindow", "Player info", nullptr));
        playerNameLabel->setText(QCoreApplication::translate("MainWindow", "Player 1", nullptr));
        moneyLabel->setText(QCoreApplication::translate("MainWindow", "Money: $", nullptr));
        KansAlg->setText(QString());
        BalanceLabel->setText(QString());
        LoonLabel->setText(QString());
        propertyGroup->setTitle(QCoreApplication::translate("MainWindow", "Property  info", nullptr));
        propertyNameLabel->setText(QCoreApplication::translate("MainWindow", "Select property", nullptr));
        propertyPriceLabel->setText(QCoreApplication::translate("MainWindow", "Price: ", nullptr));
        propertyOwner->setText(QString());
        propertyOwnerLabel->setText(QString());
        propertyPrice->setText(QString());
        EndTurnButton->setText(QCoreApplication::translate("MainWindow", "End Turn", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
