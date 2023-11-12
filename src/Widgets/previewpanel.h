#ifndef PREVIEWPANEL_H
#define PREVIEWPANEL_H
//#define DEBUG_MODE

#include <QModelIndexList>
#include <QWidget>
#include <QListWidgetItem>

namespace Ui {
class PreviewPanel;
}

class CGameProfile;
class CGamePackage;

class PreviewPanel : public QWidget
{
    Q_OBJECT

public:
    explicit PreviewPanel(QWidget *parent = nullptr);
    ~PreviewPanel();

    void setModel(CGameProfile* userProfile);
    void updatePreview(CGamePackage* userPackage);

Q_SIGNALS:
    void sendLocalSave();

private slots:
    void listUpdate(QModelIndex, int, int, QModelIndex, int);

private:

    void refresh();
    /**
     * @brief Update's active profile's load order with respect to a specified string vector.
     * @param String vector containing sequential mod titles in a unique order provided by the user.
     */
    void updateProfileLoadOrder();

    Ui::PreviewPanel *ui;
    CGameProfile* pUserProfile = nullptr;
};

#endif // PREVIEWPANEL_H
