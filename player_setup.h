#ifndef PLAYERSETUP_H
#define PLAYERSETUP_H

#include <QDialog>
#include <QVector>
#include <QString>
#include <QLineEdit>
#include <QCheckBox>


namespace Ui {
class PlayerSetup;
}

class PlayerSetup : public QDialog
{
    Q_OBJECT

public:
    explicit PlayerSetup(QWidget *parent = nullptr);
    ~PlayerSetup();

    QVector<QString> getPlayerNames() const;
    QVector<bool> getIsBotFlags() const;
    int getPlayerCount() const;

private slots:
    void updateNameFields(int count);
    void on_buttonBox_accepted();

private:
    Ui::PlayerSetup *ui;
    QVector<QLineEdit*> nameFields;
    QVector<QCheckBox*> botCheckboxes;
    QVector<QString> playerNames;
    QVector<bool> isBotFlags;

};

#endif // PLAYERSETUP_H
