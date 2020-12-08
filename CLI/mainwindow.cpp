#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "qconsole.h"
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    this->ui->setupUi(this);
    QConsole *console = new QConsole(NULL, "TEST");
    layout()->addWidget(console);
    console->setPrompt(QString());

}

MainWindow::~MainWindow() {
    delete ui;
}

