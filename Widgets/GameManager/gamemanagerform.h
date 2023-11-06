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
    void on_HomeLabel_clicked();
    void on_sizeDownButton_clicked();
    void on_sizeUpButton_clicked();

protected:
    void ClearGrid();
    void ResetLayout();
    void RefreshGameStats();

private:
    static const int CARD_WIDTH = 30 * 10;
    static const int CARD_HEIGHT = 18 * 10;
    static constexpr float GRID_SCALE = 13;
    static constexpr float MIN_GRID_SCALE = 44;
    static constexpr float MAX_GRID_SCALE = 90;
    static const int DEBUG_TOTAL_CARDS = 106;

    double m_CustomGridScale = 44;
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
