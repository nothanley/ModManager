#ifndef PREVIEWPANEL_H
#define PREVIEWPANEL_H

#include <QWidget>

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

private:

    void RefreshListPanel();

    Ui::PreviewPanel *ui;
    CGameProfile* pUserProfile = nullptr;
};

#endif // PREVIEWPANEL_H
