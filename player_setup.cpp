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

    isBotFlags.fill(false, 8);
    ui->playerCountSpinBox->setRange(2, 8);
    connect(ui->playerCountSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),this, &PlayerSetup::updateNameFields);
    connect(ui->buttonBox, &QDialogButtonBox::accepted,this, &PlayerSetup::on_buttonBox_accepted);

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
    qDeleteAll(botCheckboxes);
    botCheckboxes.clear();

    // Clear layout (keep only the spinbox row)
    while (ui->formLayout->rowCount() > 1) {
        ui->formLayout->removeRow(1);
    }

    // Add new player input rows
    for (int i = 0; i < count; ++i) {
        QHBoxLayout *hLayout = new QHBoxLayout();

        QLineEdit *nameEdit = new QLineEdit(this);
        nameEdit->setPlaceholderText(QString("Player %1").arg(i+1));

        QCheckBox *botCheck = new QCheckBox("Bot", this);

        hLayout->addWidget(nameEdit);
        hLayout->addWidget(botCheck);

        ui->formLayout->addRow(QString("Player %1:").arg(i+1), hLayout);

        nameFields.append(nameEdit);
        botCheckboxes.append(botCheck);
    }
}

void PlayerSetup::on_buttonBox_accepted()
{
    // Collect player names
    playerNames.clear();
    for (const QLineEdit *field : nameFields) {
        QString name = field->text().trimmed();
        playerNames.append(name.isEmpty() ? field->placeholderText() : name);
    }

    // Collect bot flags
    isBotFlags.clear();
    for (const QCheckBox *checkbox : botCheckboxes) {
        isBotFlags.append(checkbox->isChecked());
    }

    accept();
}

QVector<QString> PlayerSetup::getPlayerNames() const
{
    return playerNames;
}

QVector<bool> PlayerSetup::getIsBotFlags() const
{
    return isBotFlags;
}

int PlayerSetup::getPlayerCount() const
{
    return ui->playerCountSpinBox->value();
}

