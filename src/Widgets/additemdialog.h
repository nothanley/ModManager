#ifndef ADDITEMDIALOG_H
#define ADDITEMDIALOG_H

#include <QWidget>
#include <QStandardPaths>

namespace Ui {
class AddItemDialog;
}

class CGamePackage;

class AddItemDialog : public QWidget
{
    Q_OBJECT

public:
    explicit AddItemDialog(QWidget *parent = nullptr);
    ~AddItemDialog();

Q_SIGNALS:
    void sendItem(CGamePackage* gameItem);
    void interfaceClose();

private slots:
    void on_pathBrowseButton_clicked();
    void on_thumbnailBrowseButton_clicked();
    void on_titleBox_textChanged(const QString &arg1);
    void on_authorBox_textChanged(const QString &arg1);
    void on_comboBox_currentTextChanged(const QString &arg1);
    void on_thumbnailBox_textChanged(const QString &arg1);
    void on_addModButton_clicked();
    void on_cancelButton_clicked();

private:
    bool hasValidInput();
    void exit();
    QString m_ExplorerPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation); // Cache Explorer Directory
    CGamePackage* CreatePackageFromInterface();
    Ui::AddItemDialog *ui;
};

#endif // ADDITEMDIALOG_H
