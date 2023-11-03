#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QAction>
#include <QDebug>

class GameManagerForm;
class BurgerMenu;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QPushButton* maximizeButton = nullptr;
    QPushButton* minimizeButton = nullptr;
    QPushButton* closeButton = nullptr;
    QWidget* toolBar = nullptr;
protected:
    void ClearContentForm();
private:
    Ui::MainWindow *ui;
    GameManagerForm* pManagerForm=nullptr;

    void InitLayoutWidgets();
    void InitGUIButtons();
    void AddBurgerMenu();
    void CreateGameManager(const long long& gameHash);
    void PerformSideBarAction(QAction* action);
    void SetupGameSidebar(BurgerMenu* pSideBar);
//    void AddBurgerMenu();
};

#endif // MAINWINDOW_H
