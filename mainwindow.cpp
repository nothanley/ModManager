#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Widgets/BurgerMenu/burgermenu.h"
#include "Widgets/GameManager/gamemanagerform.h"

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

    // debug. remove later.
    CreateGameManager();
}

void MainWindow::AddBurgerMenu(){
    BurgerMenu* menu     = new BurgerMenu();
    menu->setMaximumWidth(120);
    menu->setBurgerIcon(QIcon(":/icons/burger.png"));
    menu->addMenuAction(QIcon(":/icons/collections.png"), "Action 1");
    menu->addMenuAction(QIcon(":/icons/folders.png"),     "Action 2");
    menu->addMenuAction(QIcon("://icons/albums.png"),      "Action 3");
    this->centralWidget()->layout()->replaceWidget(ui->SideBarDummy,menu);
    delete ui->SideBarDummy;
}


void MainWindow::CreateGameManager(){
    GameManagerForm* menu     = new GameManagerForm();
    this->centralWidget()->layout()->replaceWidget(ui->BodyDummy,menu);
    ui->BodyDummy->hide();
}













