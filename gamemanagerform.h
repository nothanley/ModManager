#ifndef GAMEMANAGERFORM_H
#define GAMEMANAGERFORM_H

#include <QWidget>

namespace Ui {
class GameManagerForm;
}

class GameManagerForm : public QWidget
{
    Q_OBJECT

public:
    explicit GameManagerForm(QWidget *parent = nullptr);
    ~GameManagerForm();

private:
    Ui::GameManagerForm *ui;
};

#endif // GAMEMANAGERFORM_H
