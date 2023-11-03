#ifndef GAMECARD_H
#define GAMECARD_H

#include <QWidget>
class CGamePackage;

namespace Ui {
class GameCard;
}

class GameCard : public QWidget
{
    Q_OBJECT

public:
    explicit GameCard(QWidget *parent = nullptr, CGamePackage* gameMod = nullptr);
    ~GameCard();

private slots:
    void on_GameCardButton_pressed();

Q_SIGNALS:
    void TableUpdate(CGamePackage* gameMod);

private:
    CGamePackage* pGameMod = nullptr;
    Ui::GameCard *ui;
};

#endif // GAMECARD_H
