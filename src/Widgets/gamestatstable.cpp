#include "gamestatstable.h"
#include "ui_gamestatstable.h"
#include "src/PackageManager/Manager/GamePackage.h"
#include <QPushButton>
#include <QDebug>

GameStatsTable::GameStatsTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameStatsTable)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose,true);
}

GameStatsTable::~GameStatsTable()
{
    delete ui;
}

void PopulateRightTable(QTableWidget* table,CGamePackage* selectedMod){
    QStringList headerLabels = { "File Size(kb)", "Mod Index", "File Name", "Replaces" };
    table->setRowCount( headerLabels.size() );
    table->setColumnCount(1);
    table->setVerticalHeaderLabels( headerLabels );

    // Populate Items
    QString fileName = QString::fromStdString(selectedMod->getPath()).split("/").last();
    table->setItem( 0,0,new QTableWidgetItem( QString::number( selectedMod->getSize() )));
    table->setItem( 1,0,new QTableWidgetItem( QString::number( selectedMod->getIndex() )));
    table->setItem( 2,0,new QTableWidgetItem(  fileName ));
    table->setItem( 3,0,new QTableWidgetItem( selectedMod->getReplaceAssetName().c_str() ));
}

void PopulateLeftTable(QTableWidget* table,CGamePackage* selectedMod){

    QStringList headerLabels = { "Mod Title", "Mod Author", "Creation Date", "Mod Type", "File Name" };
    table->setRowCount( headerLabels.size() );
    table->setColumnCount(1);
    table->setVerticalHeaderLabels( headerLabels );

    // Populate Items
    table->setItem( 0,0,new QTableWidgetItem( selectedMod->getName().c_str() ) );
    table->setItem( 1,0,new QTableWidgetItem( selectedMod->getAuthor().c_str() ) );
    table->setItem( 2,0,new QTableWidgetItem( selectedMod->getDate().c_str() ) );
    table->setItem( 3,0,new QTableWidgetItem( selectedMod->getType().c_str() ) );
    table->setItem( 4,0,new QTableWidgetItem( selectedMod->getPath().c_str() ) );
}

void
GameStatsTable::addEnableButtonToCell(QTableWidget* table, int row, int column){
    QPushButton* button = new QPushButton();
    button->setText("Enable");
    table->setCellWidget(row,column,button);
    QObject::connect(button, &QPushButton::clicked,
                     this, &GameStatsTable::toggleItem );
}

void
GameStatsTable::addExtractButtonToCell(QTableWidget* table, int row, int column){
    QPushButton* button = new QPushButton();
    button->setText("Extract Contents");
    table->setCellWidget(row,column,button);
    QObject::connect(button, &QPushButton::clicked,
                     this, &GameStatsTable::saveItemLocally );
}


void
GameStatsTable::addEditButtonToCell(QTableWidget* table, int row, int column){
    QPushButton* button = new QPushButton();
    button->setText("Edit Details");
    table->setCellWidget(row,column,button);
    QObject::connect(button, &QPushButton::clicked,
                     this, &GameStatsTable::editTableItem );
}

void
GameStatsTable::PopulatePropertyTable(QTableWidget* table,CGamePackage* selectedMod){

    QStringList headerLabels = { "Mod Version", "File MD5/SHA", "Set Enabled", "Extract Contents", "Edit Details" };
    table->setRowCount( headerLabels.size() );
    table->setColumnCount(1);
    table->setVerticalHeaderLabels( headerLabels );

    // Populate Items
    table->setItem( 0,0,new QTableWidgetItem( QString::number( selectedMod->getFileVersion())) );
    table->setItem( 1,0,new QTableWidgetItem( QString::number( selectedMod->getMD5())) );

    addEnableButtonToCell (table, 2, 0);
    addExtractButtonToCell(table, 3, 0);
    addExtractButtonToCell(table, 4, 0);
}

void
GameStatsTable::UpdateStatsTable(CGamePackage* selectedMod){
    if (!selectedMod){ return; }

    PopulateLeftTable(ui->leftTable,selectedMod);
    PopulateRightTable(ui->rightTable,selectedMod);
    PopulatePropertyTable(ui->PropertyTable,selectedMod);
    this->pGameMod = selectedMod;
}

QString
GameStatsTable::getCurrentTableItem(){
    if (pGameMod == nullptr)
        return "";
    return QString::fromStdString(pGameMod->getName());
}

void
GameStatsTable::editTableItem(){
    emit editItem(pGameMod);
}

void
GameStatsTable::saveItemLocally()
{
    qDebug() << "Placeholder zip save.";
}

void
GameStatsTable::toggleItem()
{
    pGameMod->setEnabled( !pGameMod->getStatus() );
    qDebug() << "Mod Status: " << pGameMod->getStatus();
}




