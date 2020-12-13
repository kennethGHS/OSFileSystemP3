#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "qconsole.h"
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    this->ui->setupUi(this);
    this->setWindowTitle("File System");
    QConsole *console = new QConsole();
    this->setCentralWidget(console);
    layout()->setAlignment(console, Qt::AlignmentFlag::AlignCenter);
    this->ui->statusbar->hide();
    console->setBackgroundColor(QColor(43, 43, 43));
    console->setErrColor(QColor(255, 85, 85));
    console->setCmdColor(QColor(169, 183, 198));
//    console->setOutColor(QColor(80, 250, 123));
    console->setOutColor(QColor(169, 183, 198));
    console->setPathColor(QColor(98, 114, 164));
    console->setUserColor(QColor(97, 150, 71));
    console->setUsername("root");
    console->setPrompt("/", true);
    QFont font = QFont("monospace", 12);
    console->setFont(font);
}

MainWindow::~MainWindow() {
    delete ui;
}

