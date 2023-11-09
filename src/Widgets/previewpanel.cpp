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
    //todo: update thumbnail
//    qDebug() << "Thumbnail Height" << ui->PreviewLabel->height();
    qDebug() << "Preview mod: " << userPackage->getName().c_str();

    QString authorText = QString("Author: ") + QString(userPackage->getAuthor().c_str());
    QString itemDescription = QString("Description: ") + QString(userPackage->getDescription().c_str() );

    ui->titleLabel->setText( userPackage->getName().c_str() );
    ui->authorLabel->setText( authorText );
    ui->descriptionLabel->setText( itemDescription );

    // Update Preview Image
    QImage layoutGenBGImg(":/icons/card_bg_dummy.png");
    #if defined(DEBUG_MODE_ENABLED)
    layoutGenBGImg = QImage( QTGameUtils::getRandomFilePath(
                QString("C:/Users/wauke/source/repos/ModManager/Widgets/BurgerMenu/icons/card_templates" ) ) );
    layoutGenBGImg = layoutGenBGImg.scaled( ui->PreviewLabel->size(),Qt::KeepAspectRatio );

    ui->PreviewLabel->setPixmap(QPixmap::fromImage(layoutGenBGImg));
    #endif
}

void
PreviewPanel::refresh(){
    if (!pUserProfile) return;

    ui->ListWidget->clear();
    for (const auto& gamePackage : this->pUserProfile->getAllMods()){
        ui->ListWidget->addItem( gamePackage->getName().c_str() );
    }
}














