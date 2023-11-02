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

private slots:
    void on_GridSizeSlider_valueChanged(int value);

private:
    int m_CustomGridScale = 5;
    void ClearGrid();
    void GetManagerLayoutGeneral();
    void InitializeManagerSettings();
    void PopulateManagerGUI();
    Ui::GameManagerForm *ui;
    CManagerController* m_CTRLManager = nullptr;
    CGameManager* pGameManager = nullptr;
    std::vector<GameCard*> pGameCards;
};

#endif // GAMEMANAGERFORM_H
