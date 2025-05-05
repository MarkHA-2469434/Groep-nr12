#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow window;
    window.show(); // This makes the window visible

    return app.exec(); // Starts the event loop
}
