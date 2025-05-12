#include "mainwindow.h"
#include "logic.cpp"
#include "./ui_mainwindow.h"
#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_SpelerVoegToe_released() {
    QString naam = ui->NaamInput->text();
    Game game;
    if (!naam.isEmpty()) {
        game.voegSpelerToe(naam,1500);
        ui->NaamInput->setText("Speler Toegevoegd!");
    }
    else{
        ui->NaamInput->setText("Geef eerst een naam in!");
    }
}


