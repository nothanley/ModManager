#include "gamemanagerform.h"
#include "ui_gamemanagerform.h"
#include "qtgameutils.hpp"
#include "gamecard.h"
#include "gamestatstable.h"
#include <QDebug>
#include <QLayout>
using namespace QTGameUtils;

static const int GRID_WIDTH = 4;
static const int CARD_WIDTH = 40;
static const int CARD_HEIGHT = 22;
static const float GRID_SCALE = 0.8;

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
    for (auto card: this->pGameCards) delete card;
    delete this->m_CTRLManager;
    delete ui;
}

void
GameManagerForm::InitializeStatsTable(){
    delete pStatsTable;
    this->pStatsTable = new GameStatsTable(this);
    ui->PackageDetails->layout()->addWidget(pStatsTable);
}

void
GameManagerForm::ResetLayout(){
    this->pGameManager = nullptr;
    delete pStatsTable;
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

void
GameManagerForm::GetManagerLayoutGeneral(){
    if (!this->pGameManager){
        Q_ASSERT(this->pGameManager);
        qDebug() << "Could not load layout. Invalid Manager";
    }

    this->setEnabled(true);
    ui->setupUi(this);

    InitializeStatsTable();
    QGridLayout* cardGrid = static_cast<QGridLayout*>( ui->GameCardGrid->layout() );
    PopulateCardGrid( cardGrid );
}

void
GameManagerForm::PopulateCardGrid(QGridLayout* gridLayout){

    int numGameCards = 32; /* Placeholder count */

    for (int i = 0; i < numGameCards; i++){
        std::string modTitle = "PlaceHolder_Mod " + std::to_string(i);
        CGamePackage* placeholderMod = new CGamePackage(modTitle.c_str(),-1);
        GameCard* gameTile = new GameCard(this,placeholderMod);
        gameTile->setFixedSize( QSize(
                                   m_CustomGridScale * CARD_WIDTH,
                                   m_CustomGridScale * CARD_HEIGHT) );

        this->pGameCards.push_back(gameTile);
        gridLayout->addWidget( gameTile,
                             i / int(GRID_SCALE * m_CustomGridScale),
                             i % int(GRID_SCALE * m_CustomGridScale) );

        QObject::connect(gameTile, &GameCard::TableUpdate, this->pStatsTable, &GameStatsTable::UpdateStatsTable );
    }

    qDebug() << "Created " << QString::number(numGameCards) << " Game Card(s)";
}

void GameManagerForm::ClearGrid(){
    for (auto card : this->pGameCards){
        delete card;}
    pGameCards.clear();
}


void GameManagerForm::on_GridSizeSlider_valueChanged(int value)
{
    ClearGrid();
    this->m_CustomGridScale = value;
    int numGameCards = 32;

    QGridLayout* cardGrid = static_cast<QGridLayout*>( ui->GameCardGrid->layout() );
    int numCells = GRID_WIDTH;

    for (int i = 0; i < numGameCards; i++){
        std::string modTitle = "PlaceHolder_Mod " + std::to_string(i);
        CGamePackage* placeholderMod = new CGamePackage(modTitle.c_str(),-1);
        GameCard* gameTile = new GameCard(this,placeholderMod);
        gameTile->setFixedSize( QSize(
                                   m_CustomGridScale * CARD_WIDTH,
                                   m_CustomGridScale * CARD_HEIGHT) );

        this->pGameCards.push_back(gameTile);
        if (m_CustomGridScale * CARD_WIDTH >= 240)
            numCells = GRID_WIDTH - abs(GRID_WIDTH - (int(GRID_SCALE * m_CustomGridScale)));

        cardGrid->addWidget( gameTile,
                             i / numCells,
                             i % numCells );

        QObject::connect(gameTile, &GameCard::TableUpdate, this->pStatsTable, &GameStatsTable::UpdateStatsTable );
    }
    qDebug() << "Created " << QString::number(numGameCards) << " Game Card(s)";
}






















