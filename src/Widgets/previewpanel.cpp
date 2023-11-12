#include "previewpanel.h"
#include "ui_previewpanel.h"
#include "src/PackageManager/Manager/GameProfile.h"
#include "src/PackageManager/Manager/GamePackage.h"
#include "src/qtgameutils.hpp"
#include <QDebug>

#include <QEvent>
#include <QDropEvent>
#include <QAction>

PreviewPanel::PreviewPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PreviewPanel)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose,true);

    connect(ui->ListWidget->model(),
            SIGNAL(rowsMoved(QModelIndex, int, int, QModelIndex, int)),
            this,
            SLOT(listUpdate(QModelIndex, int, int, QModelIndex, int)));
}

void
PreviewPanel::listUpdate(QModelIndex, int, int, QModelIndex, int){
    updateProfileLoadOrder();
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

void
PreviewPanel::updateProfileLoadOrder()
{
    std::vector<CGamePackage*> loadOrder;

    for (int i = 0; i < pUserProfile->getModCount(); i++) {
        std::string itemName = ui->ListWidget->item(i)->text().toStdString();
        CGamePackage* mod = pUserProfile->getMod(itemName);

        if (mod != nullptr)
            loadOrder.push_back(mod);
    }

    /* Check validity and update registry*/
    if (loadOrder.size() == pUserProfile->getModCount())
    {
        pUserProfile->setRegistry(loadOrder);
        emit sendLocalSave(); /* Update GUI and local files */
    }
}










