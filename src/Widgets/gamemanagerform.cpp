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
#include <QFileInfo>
#include <QMessageBox>
#include <QGraphicsBlurEffect>
#include "additemdialog.h"

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
    if (this->hasItemDialog())
        this->pItemDialog->close();

    for (auto card: this->pGameCards)
        card->close();

    delete this->m_CTRLManager;
    delete ui;
}

bool
GameManagerForm::hasItemDialog(){
    return this->pItemDialog != nullptr;
}

void
AddDebugPackages(CGameManager* manager, int numPlaceholders){
    for (int i = 0; i < numPlaceholders; i++){
        std::string modTitle = "PlaceHolder_Mod " + std::to_string(i);
        CGamePackage* placeholderMod = new CGamePackage(modTitle.c_str(),"Misc","No Path");
        manager->getActiveProfile()->addToRegistry(placeholderMod);
    }
}


void
GameManagerForm::openPakInExplorer(CGamePackage* mod){
    qDebug() << "Open explorer page here";
}

void
GameManagerForm::InitializeStatsTable(){
    delete pStatsTable;
    this->pStatsTable = new GameStatsTable(this);

    QObject::connect(pStatsTable, &GameStatsTable::editItem, this, &GameManagerForm::createItemDialog );
    QObject::connect(pStatsTable, &GameStatsTable::extractItem, this, &GameManagerForm::openPakInExplorer );

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
    GetManagerLayoutGeneral();
}

void
GameManagerForm::clearItemDialog(){
    this->pItemDialog = nullptr;
}

void
GameManagerForm::createItemDialog(CGamePackage* mod){
    if (this->hasItemDialog())
        return;

    pItemDialog = new AddItemDialog(nullptr,mod);
    pItemDialog->show();

    QObject::connect(pItemDialog, &AddItemDialog::sendItem, /* Connect SEND signals in mainlayout & dialogue */
                     this,
                     &GameManagerForm::AddCardToLayout );

    QObject::connect(pItemDialog, &AddItemDialog::interfaceClose,
                     this, &GameManagerForm::clearItemDialog); /* Connect close signals in this & child dialog */
}

void
GameManagerForm::InitializeManagerSettings(){
    QString roamingPath = GetUserRoamingPath();

    this->m_CTRLManager = new CManagerController( roamingPath.toStdString().c_str() );
    if (m_CTRLManager->getGameCount() == 0){
        qDebug() << "No Games Loaded.";
        Q_ASSERT(m_CTRLManager->getGameCount() == 0);
        return; }

    qDebug() << "\nLoaded User Config: " << roamingPath;
}

void
GameManagerForm::InitializePreviewPanel(){
    this->pPreviewPanel = new PreviewPanel(this);
    pPreviewPanel->setModel( pGameManager->getActiveProfile() );
    ui->RightPanelLayout->layout()->addWidget(this->pPreviewPanel);

    connect(pPreviewPanel, &PreviewPanel::sendLocalSave,
                     this, &GameManagerForm::saveLocalConfigs); /* Connect close signals in this & child dialog */

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
GameManagerForm::refreshProfileComboBox()
{
    ui->comboBox->clear();
    for (auto& profile : pGameManager->getProfiles()){
        std::string name = profile.getName();
        ui->comboBox->addItem(name.c_str());
    }
}

void
GameManagerForm::saveLocalConfigs(){
    /* save active item */
    QString activeItem;
    if (pStatsTable != nullptr)
        activeItem = pStatsTable->getCurrentTableItem();

    /* save config and redraw UI */
    this->pGameManager->save();
    this->RefreshAll();

    /* Refocus previous grid card */
    this->focusItem(activeItem);
}

void
GameManagerForm::focusItem(const QString name){
    if (name == "") return;

    for (const auto& card : this->pGameCards)
        if( card->getItemName().toUpper() == name.toUpper() )  {
            card->focus();
            break;
        }
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
GameManagerForm::removeProfileMod(CGamePackage* mod){
    CGameProfile* profile = pGameManager->getActiveProfile();
    profile->removeFromRegistry(mod);
    pGameManager->save();
    this->RefreshAll();
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
    refreshProfileComboBox();
    PopulateCardGrid();
}

void
GameManagerForm::AddCardToLayout(CGamePackage* gameItem){
    if (this->pGameManager == nullptr) return;
    if (!pGameManager->hasActiveProfile()) return;

    /* Update registry and GUI */
    gameItem->saveTo( pGameManager->getJsonPath() ); /* Save items to root manager path */
    addToActiveProfile(gameItem);
}

void
GameManagerForm::ClearGrid(){
    for (auto card : this->pGameCards){
        card->close();
    }
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

CGamePackage*
GameManagerForm::getZippedGamePak(const QString& zipPath, const QString& appPath ){
    CGamePackage* gameMod;

    CreateUserDirectory(appPath);    /* Unzip contents to manager root */
    if (!unzipFile(zipPath,appPath)) return nullptr;

    std::string configPath = appPath.toStdString() + "mod_config.json";
    gameMod = new CGamePackage(configPath.c_str());
    if (gameMod == nullptr) return nullptr;

    return createGameZipPrompt(gameMod); /* Query's user if mod is preexisting */
}

void
GameManagerForm::createYesNoDialog(const char* prompt, bool* ok){
    QMessageBox msgBox;
    msgBox.setWindowTitle("Mod Manager");
    msgBox.setText(prompt);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.addButton(QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    *ok = (msgBox.exec() == QMessageBox::Ok);
}

CGamePackage*
GameManagerForm::createGameZipPrompt(CGamePackage* gameMod){
    /* Returns null if user does not want to replace existing mod */
    bool isExistingMod = pGameManager->getActiveProfile()->
                         getIndex(gameMod->getName()) != -1;
    if (!isExistingMod) return gameMod;

    bool replaceMod;
    std::string userPrompt = "Overwrite existing mod: "+gameMod->getName()+"?";
    createYesNoDialog(userPrompt.c_str(),&replaceMod);

    // cleanup memory if user declines overwrite
    if (!replaceMod){
        delete gameMod;
        gameMod = nullptr;  }

    return gameMod;
}

bool
GameManagerForm::moveModtoManagerRoot(CGamePackage* gameMod, const QString &path){
    std::string targetPath = pGameManager->getPath() + gameMod->getName();
    ConfigUtils::removeDirectory(targetPath); /* Clear target folder first */

    /* Update Paths (todo: these should be relative) */
    gameMod->setJsonPath( targetPath + "/mod_config.json" );
    gameMod->setThumbnailPath( targetPath + "/thumbnail.jpg" );
    gameMod->setAssetPath( targetPath + "/" + gameMod->getAssetFileName() );

    return ConfigUtils::moveDirectory( path.toStdString(),
                                      targetPath);
}

void
GameManagerForm::addToActiveProfile(CGamePackage* mod){
    this->pGameManager->getActiveProfile()->addToRegistry(mod);
    pGameManager->save();
    qDebug() << "Added Mod to registry: " << mod->getName().c_str();
    this->RefreshAll();
}

void
GameManagerForm::addZippedMod(const QString& path){
    QString appDataPath = GetUserRoamingPath("") + "/" +
                          QFileInfo(path).baseName() + "/";

    /* Check zip contents for mod_config.json */
    if (!QTGameUtils::isGamePackageZip(path))
        return;
    CGamePackage* gameMod = getZippedGamePak(path,appDataPath);

    if (gameMod == nullptr){
        qDebug() << "No assets loaded.";
        return; }

    if (!this->moveModtoManagerRoot(gameMod,appDataPath)){
        qDebug() << "Failed to save assets to manager.";
        return; }

    addToActiveProfile(gameMod);
}

void
GameManagerForm::addCakeMod(const QString& path){
    std::string baseName = QFileInfo(path).baseName().toStdString();
    CGamePackage* gameMod = new CGamePackage(  baseName.c_str(), "Misc",
                                               path.toStdString().c_str() );
    // Check existing
    createGameZipPrompt(gameMod);
    if (gameMod == nullptr)
        return;

    // Add Data to Item
    gameMod->setEnabled(true);
    gameMod->setDate( QTGameUtils::getCurrentDate().toStdString() );

    if ( !gameMod->saveTo(pGameManager->getPath()) ){
        qDebug() << "Failed to save assets to manager.";
        return; }

    addToActiveProfile(gameMod);
}

void
GameManagerForm::dropEvent(QDropEvent *event){
    QList<QUrl> links = event->mimeData()->urls();
    dragLeaveEvent(nullptr);

    for (const auto& link : links){
        QString item = link.toLocalFile();

        if (item.toLower().endsWith(".zip") ){
            addZippedMod(item); }

        if (item.toLower().endsWith(".cak") ){
            addCakeMod(item); }
    }
}

void
GameManagerForm::dragLeaveEvent(QDragLeaveEvent *event)
{
    delete ui->GameCardGrid->graphicsEffect();
    ui->GameCardGrid->setGraphicsEffect(nullptr);
    ui->GameCardGrid->repaint();
}


void
GameManagerForm::dragEnterEvent(QDragEnterEvent *event)
{
    QList<QUrl> links = event->mimeData()->urls();

    bool isValidBatch = false;
    for (const auto& link : links)
    {
        QString dragText = link.toLocalFile();
        isValidBatch = dragText.toLower().endsWith(".cak") ||
                       dragText.toLower().endsWith(".zip");
        if (!isValidBatch) break;
    }


   if ( isValidBatch ){
       ui->GameCardGrid->setGraphicsEffect(new QGraphicsBlurEffect);
       event->acceptProposedAction();
   }
}



void GameManagerForm::on_GameTopButton_2_clicked()
{
    this->createItemDialog(nullptr);
}





