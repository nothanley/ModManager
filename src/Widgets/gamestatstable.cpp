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
}

GameStatsTable::~GameStatsTable()
{
    delete ui;
}

void PopulateRightTable(QTableWidget* table,CGamePackage* selectedMod){

    QStringList headerLabels = { "File Size(kb)", "Mod Index", "Mod Game", "Replaces" };
    table->setRowCount( headerLabels.size() );
    table->setColumnCount(1);
    table->setVerticalHeaderLabels( headerLabels );

    // Populate Items
    table->setItem( 0,0,new QTableWidgetItem( QString::number( selectedMod->getSize() )));
    table->setItem( 1,0,new QTableWidgetItem( QString::number( selectedMod->getIndex() )));
    table->setItem( 3,0,new QTableWidgetItem( selectedMod->getName().c_str() ) );
    table->setItem( 4,0,new QTableWidgetItem( selectedMod->getReplaceAssetName().c_str() ));
}

void PopulateLeftTable(QTableWidget* table,CGamePackage* selectedMod){

    QStringList headerLabels = { "Mod Title", "Mod Author", "Creation Date", "Mod Type", "File Name" };
    table->setRowCount( headerLabels.size() );
    table->setColumnCount(1);
    table->setVerticalHeaderLabels( headerLabels );

    // Populate Items
    table->setItem( 0,0,new QTableWidgetItem( selectedMod->getName().c_str() ) );
    table->setItem( 1,0,new QTableWidgetItem( selectedMod->getAuthor().c_str() ) );
    table->setItem( 3,0,new QTableWidgetItem( selectedMod->getType().c_str() ) );
    table->setItem( 4,0,new QTableWidgetItem( selectedMod->getPath().c_str() ) );
}


void PopulatePropertyTable(QTableWidget* table,CGamePackage* selectedMod){

    QStringList headerLabels = { "Mod Version", "File MD5/SHA", "Set Enabled", "Extract Contents", "Edit Details" };
    table->setRowCount( headerLabels.size() );
    table->setColumnCount(1);
    table->setVerticalHeaderLabels( headerLabels );

    // Populate Items
    table->setItem( 0,0,new QTableWidgetItem( QString::number( selectedMod->getFileVersion())) );
    table->setItem( 1,0,new QTableWidgetItem( QString::number( selectedMod->getMD5())) );


    // Enable Button
    QPushButton* enableButton = new QPushButton();
    enableButton->setText("Enable");
    table->setCellWidget(2,0,enableButton);

    // Extract Button
    QPushButton* extractButton = new QPushButton();
    extractButton->setText("Extract Contents");
    table->setCellWidget(3,0,extractButton);

    // Edit Button
    QPushButton* editButton = new QPushButton();
    editButton->setText("Edit Details");
    table->setCellWidget(4,0,editButton);
}

void
GameStatsTable::UpdateStatsTable(CGamePackage* selectedMod){
    if (!selectedMod){ return; }
    qDebug() << "Mod Selected: " << QString::fromStdString(selectedMod->getName());

    PopulateLeftTable(ui->leftTable,selectedMod);
    PopulateRightTable(ui->rightTable,selectedMod);
    PopulatePropertyTable(ui->PropertyTable,selectedMod);
}














