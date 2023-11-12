#ifndef GAMESTATSTABLE_H
#define GAMESTATSTABLE_H

#include <QWidget>
class CGamePackage;
class QTableWidget;

namespace Ui {
class GameStatsTable;
}

class GameStatsTable : public QWidget
{
    Q_OBJECT

public:
    explicit GameStatsTable(QWidget *parent = nullptr);
    ~GameStatsTable();

    QString getCurrentTableItem();

public Q_SLOTS:
    void UpdateStatsTable(CGamePackage* selectedMod);

private slots:
    void saveItemLocally();
    void toggleItem();
    void editTableItem();

Q_SIGNALS:
    void editItem(CGamePackage* selectedMod=nullptr);
    void extractItem(CGamePackage* selectedMod=nullptr);

private:
    void addEnableButtonToCell(QTableWidget* table, int row, int column);
    void addExtractButtonToCell(QTableWidget* table, int row, int column);
    void addEditButtonToCell(QTableWidget* table, int row, int column);
    void PopulatePropertyTable(QTableWidget* table,CGamePackage* selectedMod);

    CGamePackage* pGameMod = nullptr;
    Ui::GameStatsTable *ui;
};

#endif // GAMESTATSTABLE_H
