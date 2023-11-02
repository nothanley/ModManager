#include "gamecard.h"
#include "ui_gamecard.h"

GameCard::GameCard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameCard)
{
    ui->setupUi(this);
}

GameCard::~GameCard()
{
    delete ui;
}
