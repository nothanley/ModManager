#include "gamemanagerform.h"
#include "ui_gamemanagerform.h"
#include "src/qtgameutils.hpp"
#include "src/PackageManager/Manager/ManagerController.h"
#include "gamecard.h"
#include "gamestatstable.h"
#include "previewpanel.h"
#include <QDebug>
#include <QLayout>
#include <QHoverEvent>
#include <QMimeData>

using namespace QTGameUtils;

GameManagerForm::GameManagerForm(const long long& userTitle, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameManagerForm)
{
    this->setEnabled(false);
    this->setAcceptDrops(true);

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

void AddDebugPackages(CGameManager* manager, int numPlaceholders){
    for (int i = 0; i < numPlaceholders; i++){
        std::string modTitle = "PlaceHolder_Mod " + std::to_string(i);
        CGamePackage* placeholderMod = new CGamePackage(modTitle.c_str(),"Misc","No Path");
        manager->getActiveProfile()->addToRegistry(placeholderMod);
    }
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
            this->m_GameTitle = "WWE 2K23";
            break;
        case GAME_WWE_22:
            break;
        default:
            break;
    }

    if (this->m_GameTitle == ""){ return; }
    pGameManager = m_CTRLManager->getManager(m_GameTitle.c_str());
    #ifdef DEBUG_MODE
        /* Debug Populate empty profile. Alter after GUI build. */
        pGameManager->createProfile("Default",true);
        AddDebugPackages(this->pGameManager,DEBUG_TOTAL_CARDS);
    #endif
    GetManagerLayoutGeneral();
}

void
GameManagerForm::InitializeManagerSettings(){
    QString roamingPath = GetUserRoamingPath();

    this->m_CTRLManager = new CManagerController( roamingPath.toStdString().c_str() );
    if (m_CTRLManager->getGameCount() == 0){
        qDebug() << "No Games Loaded.";
        Q_ASSERT(m_CTRLManager->getGameCount() == 0);
        return; }

    qDebug() << "Loaded User Config: " << roamingPath;
}

void
GameManagerForm::InitializePreviewPanel(){
    this->pPreviewPanel = new PreviewPanel(this);
    pPreviewPanel->setModel( pGameManager->getActiveProfile() );
    ui->RightPanelLayout->layout()->addWidget(this->pPreviewPanel);
}

void
GameManagerForm::RefreshGameStats(){
    ui->GameLabel->setText( pGameManager->getGameName().c_str() );
    ui->ModCountLabel->setText( QString::number( pGameManager->getActiveProfile()->getModCount() ) + " MODS" );
    ui->ProfileLabel->setText( "PROFILE: " + QString::fromStdString( pGameManager->getActiveProfile()->getName() ) ) ;
    ui->CountLabel2->setText( QString::number( pGameManager->getProfileCount() ) + " PROFILES" ) ;
}

void
GameManagerForm::RefreshRibbonStats(){
    int numCharacters = 0, numArenas = 0,
        numMovies = 0, numMiscMods= 0;

    for (const auto& gameMod : this->pGameManager->getActiveProfile()->getAllMods() )
        switch( qHash( gameMod->getType().c_str() ) ){
            case TYPE_CHAR:
                numCharacters++;
                break;
            case TYPE_ARENA:
                numArenas++;
                break;
            case TYPE_MOVIE:
                numMovies++;
                break;
            default:
                numMiscMods++;
                break;  }

    ui->CharacterLabel->setText("Characters Installed: " + QString::number(numCharacters));
    ui->ArenasLabel->setText("Arenas Installed: " + QString::number(numArenas));
    ui->MoviesLabel->setText("Movies Installed: " + QString::number(numMovies));
    ui->MiscLabel->setText("Misc Installed: " + QString::number(numMiscMods));
}

bool
GameManagerForm::ValidateManager(const char* gameName, const bool override)
{
    /* Creates a new manager if one doesn't exist. Returns false if setup fails. */
    if ( this->pGameManager && !override ){ return true; }

    this->m_CTRLManager->createManager(gameName);
    pGameManager = this->m_CTRLManager->getManager(gameName);
    return (this->pGameManager != nullptr);
}

void
GameManagerForm::GetManagerLayoutGeneral()
{
    if (!this->ValidateManager(m_GameTitle.c_str())){
        qDebug() << "Could not load or create configuration.";
        return;   }

    this->pGameManager->save();
    this->setEnabled(true);
    ui->setupUi(this);

    RefreshAll();
    ui->GameCardGrid->layout()->setSpacing(20);
}

void
GameManagerForm::PopulatePreviewPanel(CGamePackage *selectedMod){
    if ( !this->pPreviewPanel || !pGameManager->hasActiveProfile() ) { return; }
    pPreviewPanel->updatePreview(selectedMod);
}

void
GameManagerForm::PopulateCardGrid(){
    QGridLayout* gridLayout = static_cast<QGridLayout*>( ui->GameCardGrid->layout() );

    ClearGrid();
    int numGameCards = pGameManager->getActiveProfile()->getAllMods().size();
    for (int i = 0; i < numGameCards+1; i++){
        CGamePackage* gamePack = nullptr;
        if (i < numGameCards)
            gamePack = pGameManager->getActiveProfile()->getAllMods()[i];

        GameCard* gameTile = new GameCard( this, gamePack, this );
        gameTile->setFixedSize( QSize(
                                   m_CustomGridScale/10.0 * CARD_WIDTH/10.0,
                                   m_CustomGridScale/10.0 * CARD_HEIGHT/10.0) );
        gameTile->decorate();
        this->pGameCards.push_back(gameTile);
        gridLayout->addWidget( gameTile,
                             i / int(GRID_SCALE/10.0 * m_CustomGridScale/10.0),
                             i % int(GRID_SCALE /10.0* m_CustomGridScale/10.0) );

        QObject::connect(gameTile, &GameCard::TableUpdate, this->pStatsTable, &GameStatsTable::UpdateStatsTable );
        QObject::connect(gameTile, &GameCard::TableUpdate, this, &GameManagerForm::PopulatePreviewPanel );
    }
}

void
GameManagerForm::RefreshAll(){
    if (pPreviewPanel != nullptr) pPreviewPanel->close();
    if (pStatsTable != nullptr) pStatsTable->close();

    RefreshGameStats();
    RefreshRibbonStats();
    InitializeStatsTable();
    InitializePreviewPanel();
    PopulateCardGrid();
}

void
GameManagerForm::AddCardToLayout(CGamePackage* gameItem){
    if (this->pGameManager == nullptr) return;
    if (!pGameManager->hasActiveProfile()) return;

    /* Update registry and GUI */
    gameItem->saveTo( pGameManager->getJsonPath() ); /* Save items to root manager path */
    pGameManager->getActiveProfile()->addToRegistry(gameItem);
    pGameManager->save();

    qDebug() << "\nAdded Mod to registry: " << gameItem->getName().c_str();
    this->RefreshAll();
}

void
GameManagerForm::ClearGrid(){
    for (auto card : this->pGameCards){
        delete card;}
    pGameCards.clear();
}


void
GameManagerForm::on_HomeLabel_clicked()
{
    this->close();
}


void
GameManagerForm::on_sizeDownButton_clicked()
{
    this->m_CustomGridScale = (m_CustomGridScale < MIN_GRID_SCALE) ?
                MIN_GRID_SCALE : m_CustomGridScale-1;

    qDebug() << "VALUE: " << m_CustomGridScale;
    PopulateCardGrid();
}


void
GameManagerForm::on_sizeUpButton_clicked()
{
    this->m_CustomGridScale = (m_CustomGridScale > MAX_GRID_SCALE) ?
                MAX_GRID_SCALE : m_CustomGridScale+1;

    qDebug() << "VALUE: " << m_CustomGridScale;
    PopulateCardGrid();
}

#include <QFileInfo>

void
GameManagerForm::addZippedMod(const QString& path){
    QString appDataPath = GetUserRoamingPath("") + "/";
    appDataPath += QFileInfo(path).baseName() + "/";

    CreateUserDirectory(appDataPath);
    if (!unzipFile(path,appDataPath)){
        qDebug() << "Could not decompress file." << appDataPath;
        return;}

    qDebug() << "Adding file: " << appDataPath;
}


void
GameManagerForm::dropEvent(QDropEvent *event){

    QUrl link = event->mimeData()->urls().last();
    QString dragText = link.toLocalFile();

    if (dragText.toLower().endsWith(".zip") ){
        addZippedMod(dragText);
    }

}

void
GameManagerForm::dragEnterEvent(QDragEnterEvent *event)
{
   QUrl link = event->mimeData()->urls().last();
   QString dragText = link.toLocalFile();

   if ( dragText.toLower().endsWith(".cak") ||
        dragText.toLower().endsWith(".zip") )
       event->acceptProposedAction();
}


















