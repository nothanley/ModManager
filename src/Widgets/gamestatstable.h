#ifndef GAMESTATSTABLE_H
#define GAMESTATSTABLE_H

#include <QWidget>
class CGamePackage;

namespace Ui {
class GameStatsTable;
}

class GameStatsTable : public QWidget
{
    Q_OBJECT

public:
    explicit GameStatsTable(QWidget *parent = nullptr);
    ~GameStatsTable();

public Q_SLOTS:
    void UpdateStatsTable(CGamePackage* selectedMod);

private:
    Ui::GameStatsTable *ui;
};

#endif // GAMESTATSTABLE_H
