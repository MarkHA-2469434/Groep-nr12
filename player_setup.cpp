#include "player_setup.h"
#include "ui_player_setup.h"

#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QSpinBox>
#include <QLineEdit>
#include <QDialogButtonBox>

PlayerSetup::PlayerSetup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlayerSetup)
{
    ui->setupUi(this);

    // Connect spinbox to update name fields
    connect(ui->playerCountSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),this, &PlayerSetup::updateNameFields);

    // Initial setup
    ui->playerCountSpinBox->setValue(2);
    updateNameFields(2);
}

PlayerSetup::~PlayerSetup()
{
    delete ui;
}

void PlayerSetup::updateNameFields(int count)
{
    // Clear existing fields
    qDeleteAll(nameFields);
    nameFields.clear();

    // Clear layout (except first two items - label and spinbox)
    while (ui->formLayout->rowCount() > 0) {
        ui->formLayout->removeRow(0);
    }

    // Add new fields
    for (int i = 0; i < count; ++i) {
        QLineEdit *lineEdit = new QLineEdit(this);
        lineEdit->setPlaceholderText(QString("Player %1").arg(i+1));
        ui->formLayout->addRow(QString("Player %1:").arg(i+1), lineEdit);
        nameFields.append(lineEdit);
    }
}

QVector<QString> PlayerSetup::getPlayerNames() const
{
    QVector<QString> names;
    for (const QLineEdit *field : nameFields) {
        QString name = field->text().trimmed();
        names.append(name.isEmpty() ? field->placeholderText() : name);
    }
    return names;
}
