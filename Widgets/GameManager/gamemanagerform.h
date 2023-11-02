#ifndef GAMEMANAGERFORM_H
#define GAMEMANAGERFORM_H

#include "PackageManager/Manager/ManagerController.h"
#include <QWidget>

enum{
    GAME_WWE_23 = 0xbf5d9035,
    GAME_WWE_22 = 0xbf5d9034,
};

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

private:
    void GetManagerLayoutGeneral();
    void InitializeManagerSettings();
    void PopulateManagerGUI();
    Ui::GameManagerForm *ui;
    CManagerController* m_CTRLManager = nullptr;
    CGameManager* pGameManager = nullptr;
};

#endif // GAMEMANAGERFORM_H
