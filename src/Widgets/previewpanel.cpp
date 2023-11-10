#include "previewpanel.h"
#include "ui_previewpanel.h"
#include "src/PackageManager/Manager/GameProfile.h"
#include "src/PackageManager/Manager/GamePackage.h"
#include <QDebug>
#include "src/qtgameutils.hpp"

PreviewPanel::PreviewPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PreviewPanel)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
}

PreviewPanel::~PreviewPanel()
{
    delete ui;
}

void
PreviewPanel::setModel(CGameProfile *profile){
    this->pUserProfile = profile;
    this->refresh();
}



void
PreviewPanel::updatePreview(CGamePackage *userPackage){
    QString authorText = QString("Author: ") + QString(userPackage->getAuthor().c_str());
    QString itemDescription = QString("Description: ") + QString(userPackage->getDescription().c_str() );

    ui->titleLabel->setText( userPackage->getName().c_str() );
    ui->authorLabel->setText( authorText );
    ui->descriptionLabel->setText( itemDescription );

    // Update Preview Image
    if (!userPackage->hasThumbnail()){ ui->PreviewLabel->clear(); return;}
    QImage layoutGenBGImg(userPackage->getThumbnailPath().c_str());

    layoutGenBGImg = layoutGenBGImg.scaled( ui->PreviewLabel->size(),Qt::KeepAspectRatio );
    ui->PreviewLabel->setPixmap(QPixmap::fromImage(layoutGenBGImg));
}

void
PreviewPanel::refresh(){
    if (!pUserProfile) return;

    ui->ListWidget->clear();
    for (const auto& gamePackage : this->pUserProfile->getAllMods()){
        ui->ListWidget->addItem( gamePackage->getName().c_str() );
    }
}














