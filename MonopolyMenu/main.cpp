#include "logic.cpp"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    Game Monopoly;
    QMainWindow window;
    window.setWindowTitle("Monopoly UI");
    window.resize(300, 200);

    // Central widget + layout
    QWidget *centralWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    // Naam invoerveld
    QLineEdit *naamInput = new QLineEdit();
    naamInput->setPlaceholderText("Geef naam hier in");
    naamInput->setFixedSize(120,30);
    layout->addWidget(naamInput, 0, Qt::AlignCenter);

    // Voeg Speler Toe knop
    QPushButton *voegSpelerToe = new QPushButton("Voeg Speler Toe");
    voegSpelerToe->setFixedSize(120, 30);  // Set fixed size
    layout->addWidget(voegSpelerToe, 0, Qt::AlignCenter); // Align left (optional)

    // Start knop
    QPushButton *startKnop = new QPushButton("Start");
    layout->addWidget(startKnop, 0, Qt::AlignCenter); // Align left (optional)

    // Apply layout
    centralWidget->setLayout(layout);
    window.setCentralWidget(centralWidget);


    Monopoly.voegSpelerToe("Auto", 1500);
    Monopoly.voegSpelerToe("Hoed", 1500);
    Monopoly.voegSpelerToe("Schoen", 1500);

    window.show();

    return a.exec();
}

