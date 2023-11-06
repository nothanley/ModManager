#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Widgets/BurgerMenu/burgermenu.h"
#include "Widgets/GameManager/gamemanagerform.h"

#include <QGraphicsBlurEffect>

void iterateWidgetsRecursively(QObject* widget) {

    if (!widget) return;
    for (int i = 0; i < widget->children().size(); ++i) {
        QObject* childWidget = widget->children().at(i);
        if (childWidget->isWidgetType()) {
            QWidget* widget = static_cast<QWidget*>(childWidget);
            widget->setGraphicsEffect({});
            iterateWidgetsRecursively(childWidget);
        }
    }
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    CreateBuildID();

    ui->IconFrame->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->BuildLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->leftAppFrame->setAttribute(Qt::WA_TransparentForMouseEvents);
    qDebug() << "Running CakeSuite " + ui->BuildLabel->text();

    InitLayoutWidgets();
}


MainWindow::~MainWindow()
{
    delete ui;
}

void
MainWindow::CreateBuildID(){
    QStringList buildList = {
            QString::number( MAJOR_VER_ ),
            QString::number( UI_VER_ ),
            QString::number( TOOL_VER_),
            QString::number( SYS_VER_ ),
            QString::number( REVISION_ )
    };

#ifdef DEV_BUILD
    this->buildType = "DEV";
#endif

    buildType = QStringList( { buildType,
                               "Build",
                               buildList.join(".") } ).join(" ");
    ui->BuildLabel->setText( buildType );
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
MainWindow::ClearContentForm(){
    if (pManagerForm){
        delete this->pManagerForm;
        this->pManagerForm = nullptr;
    }
}

void
MainWindow::PerformSideBarAction(QAction* action){
    unsigned int actionMagic = qHash(action->objectName());
    ClearContentForm();

    switch(actionMagic){
        //todo: if manager exists, refocus old manager else create new manager
        case GAME_WWE_23:
            CreateGameManager(GAME_WWE_23);
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

    pSideBar->setMinimumWidth(62);
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

    ui->centralwidget->layout()->replaceWidget(ui->SideBarDummy,menu);
    delete ui->SideBarDummy;
}


void
MainWindow::CreateGameManager(const long long& gameHash){
    //todos: initialize CGameController to handle game types.
    // GameManagerForm should perform this init and could be subclassed for game types
    this->pManagerForm     = new GameManagerForm(gameHash,this);
    if (!pManagerForm->isEnabled()){ return; }

   ui->BodyDummy->layout()->addWidget(pManagerForm);

}

#include <QUrl>
#include <QDesktopServices>

void MainWindow::on_githubButton_clicked()
{
    QDesktopServices::openUrl ( QUrl("https://github.com/nothanley/ModManager") );
}

