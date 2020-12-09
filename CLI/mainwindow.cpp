#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "qconsole.h"
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    this->ui->setupUi(this);
    QConsole *console = new QConsole(nullptr, "TEST");
    this->setCentralWidget(console);
    layout()->setAlignment(console, Qt::AlignmentFlag::AlignCenter);
    console->setCmdColor(QColor(0, 0, 0));
    console->setOutColor(QColor(100, 200, 30));
    console->setPrompt(">> ", true);


}

MainWindow::~MainWindow() {
    delete ui;
}

