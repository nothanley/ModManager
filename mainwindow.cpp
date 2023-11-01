#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Widgets/BurgerMenu/burgermenu.h"
#include "Widgets/GameManager/gamemanagerform.h"


enum{
    GAME_WWE_23 = 3210580021,
    GAME_WWE_22 = 3210580020,
};


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

void
MainWindow::InitGUIButtons(){
    this->minimizeButton = ui->Minimize;
    this->maximizeButton = ui->Maximize;
    this->closeButton = ui->Close;
    this->toolBar = ui->Toolbar;
    maximizeButton->setCheckable(true);
}

void
MainWindow::InitLayoutWidgets(){
    InitGUIButtons();
    AddBurgerMenu();
}

void
MainWindow::PerformSideBarAction(QAction* action){
    quint32 actionMagic = qHash(action->iconText());

    switch(actionMagic){
        //todo: if manager exists, refocus old manager else create new manager
        case GAME_WWE_23:
            CreateGameManager();
            break;
        case GAME_WWE_22:
            break;
        default:
            /* Unsupported Game. */
            qDebug() << "Unknown title. Either hash mismatch or action text incorrect";
            break;
    }
}

void
MainWindow::SetupGameSidebar(BurgerMenu* pSideBar){

    pSideBar->setMaximumWidth(120);
    pSideBar->setBurgerIcon(QIcon(":/icons/burger.png"));
    pSideBar->addMenuAction(QIcon("://icons/game-sidebar-icon-placeholderdummy.png"),"WWE 2K23");
    pSideBar->addMenuAction(QIcon("://icons/game-sidebar-icon-placeholderdummy.png"),"WWE 2K22");

    QObject::connect(pSideBar, &BurgerMenu::triggered, [&](QAction* action){
        PerformSideBarAction(action);
    });
}

void
MainWindow::AddBurgerMenu(){
    BurgerMenu* menu     = new BurgerMenu();
    SetupGameSidebar(menu);

    this->centralWidget()->layout()->replaceWidget(ui->SideBarDummy,menu);
    delete ui->SideBarDummy;
}


#include "PackageManager/Manager/ManagerController.h"

void
MainWindow::CreateGameManager(){
    //todos: initialize CGameController to handle game types.
    // GameManagerForm should perform this init and could be subclassed for game types
    GameManagerForm* menu     = new GameManagerForm();
    this->centralWidget()->layout()->replaceWidget(ui->BodyDummy,menu);
    ui->BodyDummy->hide();
}













