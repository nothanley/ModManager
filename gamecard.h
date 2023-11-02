#ifndef GAMECARD_H
#define GAMECARD_H

#include <QWidget>

namespace Ui {
class GameCard;
}

class GameCard : public QWidget
{
    Q_OBJECT

public:
    explicit GameCard(QWidget *parent = nullptr);
    ~GameCard();

private:
    Ui::GameCard *ui;
};

#endif // GAMECARD_H
