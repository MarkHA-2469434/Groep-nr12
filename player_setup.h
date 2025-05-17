#ifndef PLAYERSETUP_H
#define PLAYERSETUP_H

#include <QDialog>
#include <QVector>
#include <QString>
#include <QLineEdit>

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

private slots:
    void updateNameFields(int count);

private:
    Ui::PlayerSetup *ui;
    QVector<QLineEdit*> nameFields;
};

#endif // PLAYERSETUP_H
