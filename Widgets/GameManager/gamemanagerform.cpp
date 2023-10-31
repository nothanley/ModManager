#include "gamemanagerform.h"
#include "ui_gamemanagerform.h"

GameManagerForm::GameManagerForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameManagerForm)
{
    ui->setupUi(this);
}

GameManagerForm::~GameManagerForm()
{
    delete ui;
}
