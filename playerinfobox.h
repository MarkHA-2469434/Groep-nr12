#include <QGroupBox>
#include <QPalette>

class PlayerInfoBox : public QGroupBox {
    Q_OBJECT
public:
    explicit PlayerInfoBox(QWidget *parent = nullptr);
    void setHighlighted(bool highlighted);};
