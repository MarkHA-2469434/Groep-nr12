/********************************************************************************
** Form generated from reading UI file 'player_setup.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLAYER_SETUP_H
#define UI_PLAYER_SETUP_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PlayerSetup
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QSpinBox *playerCountSpinBox;
    QFormLayout *formLayout;
    QDialogButtonBox *buttonBox;
    QLabel *label;

    void setupUi(QDialog *PlayerSetup)
    {
        if (PlayerSetup->objectName().isEmpty())
            PlayerSetup->setObjectName("PlayerSetup");
        PlayerSetup->resize(400, 300);
        verticalLayoutWidget = new QWidget(PlayerSetup);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(20, 30, 331, 141));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        playerCountSpinBox = new QSpinBox(verticalLayoutWidget);
        playerCountSpinBox->setObjectName("playerCountSpinBox");
        playerCountSpinBox->setMinimum(2);
        playerCountSpinBox->setMaximum(8);

        verticalLayout->addWidget(playerCountSpinBox);

        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");

        verticalLayout->addLayout(formLayout);

        buttonBox = new QDialogButtonBox(verticalLayoutWidget);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);

        verticalLayout->addWidget(buttonBox);

        label = new QLabel(PlayerSetup);
        label->setObjectName("label");
        label->setGeometry(QRect(20, 10, 191, 20));

        retranslateUi(PlayerSetup);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, PlayerSetup, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, PlayerSetup, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(PlayerSetup);
    } // setupUi

    void retranslateUi(QDialog *PlayerSetup)
    {
        PlayerSetup->setWindowTitle(QCoreApplication::translate("PlayerSetup", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("PlayerSetup", "Enter Player names:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PlayerSetup: public Ui_PlayerSetup {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLAYER_SETUP_H
