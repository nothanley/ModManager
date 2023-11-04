#include "previewpanel.h"
#include "ui_previewpanel.h"
#include "PackageManager/Manager/GameProfile.h"
#include "PackageManager/Manager/GamePackage.h"
#include <QDebug>

PreviewPanel::PreviewPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PreviewPanel)
{
    ui->setupUi(this);
}

PreviewPanel::~PreviewPanel()
{
    delete ui;
}

void
PreviewPanel::setModel(CGameProfile *profile){
    this->pUserProfile = profile;
    RefreshListPanel();
}

void
PreviewPanel::updatePreview(CGamePackage *userPackage){
    //todo: update thumbnail
//    qDebug() << "Thumbnail Height" << ui->PreviewLabel->height();
    qDebug() << "Preview mod: " << userPackage->getName().c_str();

    QString authorText = QString("Author: ") + QString(userPackage->getAuthor().c_str());
    QString itemDescription = QString("Description: ") + QString(userPackage->getDescription().c_str() );

    ui->titleLabel->setText( userPackage->getName().c_str() );
    ui->authorLabel->setText( authorText );
    ui->descriptionLabel->setText( itemDescription );
}

void
PreviewPanel::RefreshListPanel(){
    if (!pUserProfile) return;

    ui->ListWidget->clear();
    for (const auto& gamePackage : this->pUserProfile->getAllMods()){
        ui->ListWidget->addItem( gamePackage->getName().c_str() );
    }
}














