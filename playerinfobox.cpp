#include "playerinfobox.h"

PlayerInfoBox::PlayerInfoBox(QWidget *parent) : QGroupBox(parent) {
    setAutoFillBackground(true);
    setHighlighted(false);
}

void PlayerInfoBox::setHighlighted(bool highlighted) {
    QPalette pal = palette();
    pal.setColor(QPalette::Window, highlighted ? QColor(255, 255, 200, 150) : Qt::transparent);
    setPalette(pal);
}
