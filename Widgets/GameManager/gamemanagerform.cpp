#include "gamemanagerform.h"
#include "ui_gamemanagerform.h"
#include "qtgameutils.hpp"
#include <QDebug>
using namespace QTGameUtils;


GameManagerForm::GameManagerForm(const long long& userTitle, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameManagerForm)
{
    this->setEnabled(false);

    // Setup low-level manager.
    InitializeManagerSettings();

    // Populate UI with specified title manager
    this->m_GameHash = userTitle;
    PopulateManagerGUI();
}

GameManagerForm::~GameManagerForm()
{
    delete this->m_CTRLManager;
    delete ui;
}

void
GameManagerForm::GetManagerLayoutGeneral(){

    if (!this->pGameManager){
        Q_ASSERT(this->pGameManager);
        qDebug() << "Could not load layout. Invalid Manager";
    }

    this->setEnabled(true);
    ui->setupUi(this);

}

void
GameManagerForm::PopulateManagerGUI(){
    switch(m_GameHash){
        case GAME_WWE_23:
            pGameManager = m_CTRLManager->GetGameManager("WWE 2K23");
            GetManagerLayoutGeneral();
            break;
        case GAME_WWE_22:
            break;
        default:
            break;
    }
}

void
GameManagerForm::InitializeManagerSettings(){
    QString roamingPath = GetUserRoamingPath();
    SetupManagerConfig(roamingPath);

    this->m_CTRLManager = new CManagerController( roamingPath.toStdString().c_str() );
    if (m_CTRLManager->m_NumGames == 0){
        qDebug() << "No Games Loaded.";
        Q_ASSERT(m_CTRLManager->m_NumGames == 0);
        return; }

    qDebug() << "Loaded user config";
}
















