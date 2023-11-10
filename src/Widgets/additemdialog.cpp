#include "additemdialog.h"
#include "ui_additemdialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include "src/PackageManager/Manager/GamePackage.h"
#include <QDebug>
#include "src/qtgameutils.hpp"

AddItemDialog::AddItemDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddItemDialog)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
}

AddItemDialog::~AddItemDialog()
{
    emit interfaceClose();
    delete ui;
}

void
AddItemDialog::on_pathBrowseButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Select mod (.cak/.zip)"),
                                                     this->m_ExplorerPath,
                                                     tr("All Supported Files (*.cak; *.zip)") );
    if (filePath == ""){ return; }
    m_ExplorerPath = filePath;
    ui->pathBox->setText(filePath);
}


void
AddItemDialog::on_thumbnailBrowseButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Select Preview Image"),
                                                     this->m_ExplorerPath,
                                                     tr("All Supported Files (*.png; *.jpg; *.jpeg)") );
    if (filePath == ""){ return; }
    m_ExplorerPath = filePath;
    ui->thumbnailBox->setText(filePath);
}

void
AddItemDialog::exit(){
    this->close();
    delete this;
}

void
AddItemDialog::on_titleBox_textChanged(const QString &arg1)
{
    if (ui->titleBox->text() == ""){
        ui->PreviewModTitle->setText(ui->titleBox->placeholderText());
        return;}
    ui->PreviewModTitle->setText(arg1);
}


void
AddItemDialog::on_authorBox_textChanged(const QString &arg1)
{
    if (ui->authorBox->text() == ""){
        ui->PreviewModAuthor->setText(ui->authorBox->placeholderText());
        return;}
    ui->PreviewModAuthor->setText(arg1);
}

void
AddItemDialog::on_comboBox_currentTextChanged(const QString &arg1)
{
    ui->PrevieiwModType->setText("Type: "+arg1);
}

void AddItemDialog::on_addModButton_clicked()
{
    /* Generate new mod item from user input */
    if( !this->hasValidInput() ) return;
    CGamePackage* gameItem = CreatePackageFromInterface();

    /* Send item to layout and closes interface */
    emit this->sendItem(gameItem);
}


void AddItemDialog::on_cancelButton_clicked()
{
    this->exit();
}

void
AddItemDialog::on_thumbnailBox_textChanged(const QString &arg1)
{
    /* Updates preview pic if matching extension */
    QString thumbnailPath = ui->thumbnailBox->text();
    if (thumbnailPath.endsWith(".png") ||
        thumbnailPath.endsWith(".jpg") ||
        thumbnailPath.endsWith(".jpeg") ) {

        QPixmap thumbnailMap = QPixmap::fromImage(QImage(thumbnailPath));
        thumbnailMap = thumbnailMap.scaled(ui->ThumbnailLabel->size(),Qt::KeepAspectRatio);
        ui->ThumbnailLabel->setPixmap( thumbnailMap );  }
}


CGamePackage*
AddItemDialog::CreatePackageFromInterface(){
    QString modName = ui->titleBox->text();
    QString modPath = ui->pathBox->text();
    QString modType = ui->comboBox->currentText();
    CGamePackage* gameItem = new CGamePackage(modName.toStdString().c_str(),
                                              modType.toStdString().c_str(),
                                              modPath.toStdString().c_str());
    gameItem->setEnabled(true);
    gameItem->setDate( QTGameUtils::getCurrentDate().toStdString() );
    gameItem->setDescription(    ui->descBox->toPlainText().toStdString().c_str() );
    gameItem->setReplacements(   ui->replaceBox->text().toStdString().c_str()     );
    gameItem->setThumbnailPath(  ui->thumbnailBox->text().toStdString().c_str()   );
    return gameItem;
}

bool
AddItemDialog::hasValidInput(){

    if ( !ui->pathBox->text().endsWith(".cak") && !ui->pathBox->text().endsWith(".zip") ){
        QMessageBox::warning(this,"Warning", "Invalid mod path entered.");
        return false;}

    if ( ui->authorBox->text() == "" ){
        QMessageBox::warning(this,"Warning", "Author cannot be empty.");
        return false;   }

    if ( ui->titleBox->text() == "" ){
        QMessageBox::warning(this,"Warning", "Mod title cannot be empty");
        return false;    }

    return true;
}















