#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Widgets/BurgerMenu/burgermenu.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Inits layout functionality and MinMaxClose buttons
    InitLayoutWidgets();
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::InitGUIButtons(){
    this->minimizeButton = ui->Minimize;
    this->maximizeButton = ui->Maximize;
    this->closeButton = ui->Close;
    this->toolBar = ui->Toolbar;
    maximizeButton->setCheckable(true);
}

void MainWindow::InitLayoutWidgets(){
    InitGUIButtons();
    AddBurgerMenu();
}

void MainWindow::AddBurgerMenu(){
    BurgerMenu* menu     = new BurgerMenu();
    menu->setMaximumWidth(120);
    menu->setBurgerIcon(QIcon(":/icons/Widgets/BurgerMenu/icons/burger.png"));
    menu->addMenuAction(QIcon(":/icons/Widgets/BurgerMenu/icons/collections.png"), "Collection");
    menu->addMenuAction(QIcon(":/icons/Widgets/BurgerMenu/icons/folders.png"),     "Folders");
    menu->addMenuAction(QIcon(":/icons/Widgets/BurgerMenu/icons/albums.png"),      "Albums");
    this->centralWidget()->layout()->replaceWidget(ui->SideBarDummy,menu);
    delete ui->SideBarDummy;
}














