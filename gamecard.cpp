#include "gamecard.h"
#include "ui_gamecard.h"
#include "PackageManager/Manager/GamePackage.h"
#include <QDebug>

GameCard::GameCard(QWidget *parent, CGamePackage* gameMod) :
    QWidget(parent),
    ui(new Ui::GameCard)
{
    ui->setupUi(this);
    this->setMouseTracking(true);
    this->pGameMod = gameMod;
    ui->GameCardButton->setText(pGameMod->getName().c_str());
}

GameCard::~GameCard()
{
    delete ui;
}


void GameCard::on_GameCardButton_pressed()
{
    TableUpdate(this->pGameMod);
}

