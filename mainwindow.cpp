#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "BurgerMenu/burgermenu.h"
#include "TitleBar/windowframe.h"
#include <QAction>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    QCoreApplication::instance()->installEventFilter(this);
//    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlags(Qt::FramelessWindowHint);
}

MainWindow::~MainWindow()
{
    delete ui;
}








