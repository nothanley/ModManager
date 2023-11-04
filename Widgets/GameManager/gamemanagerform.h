#ifndef GAMEMANAGERFORM_H
#define GAMEMANAGERFORM_H
#include "PackageManager/Manager/ManagerController.h"
#include <QWidget>
#include <QTableView>
#include <QGridLayout>

enum{
    GAME_WWE_23 = 0xbf5d9035,
    GAME_WWE_22 = 0xbf5d9034,
};
class GameCard;
class GameStatsTable;
class PreviewPanel;

namespace Ui {
class GameManagerForm;
}

class GameManagerForm : public QWidget
{
    Q_OBJECT

public:
    long long m_GameHash;
    explicit GameManagerForm(const long long& gameHash, QWidget *parent = nullptr);
    ~GameManagerForm();
    void
    PopulateCardGrid(QGridLayout *gridWidget);

public Q_SLOTS:
    void PopulatePreviewPanel(CGamePackage* selectedMod);

private slots:
    void on_GridSizeSlider_valueChanged(int value);
    void on_HomeLabel_clicked();

protected:
    void ClearGrid();
    void ResetLayout();
    void RefreshGameStats();

private:
    int m_CustomGridScale = 5;
    void GetManagerLayoutGeneral();
    void InitializeManagerSettings();
    void PopulateManagerGUI();
    void InitializeStatsTable();
    void InitializePreviewPanel();


    GameStatsTable* pStatsTable = nullptr;
    Ui::GameManagerForm *ui;
    CManagerController* m_CTRLManager = nullptr;
    CGameManager* pGameManager = nullptr;
    PreviewPanel* pPreviewPanel = nullptr;
    std::vector<GameCard*> pGameCards;
};

#endif // GAMEMANAGERFORM_H
