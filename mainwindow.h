#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define DEV_BUILD

#include <QMainWindow>
#include <QRandomGenerator>
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

private slots:
    void on_githubButton_clicked();

private:
    constexpr static int MAJOR_VER_ = 1;
    constexpr static int UI_VER_ = 3;
    constexpr static int TOOL_VER_ = 1;
    constexpr static int SYS_VER_ = 1;
    constexpr static int REVISION_ = 231106298;
    QString buildType = "Release";

    Ui::MainWindow *ui;
    GameManagerForm* pManagerForm=nullptr;

    void CreateBuildID();
    void InitLayoutWidgets();
    void InitGUIButtons();
    void AddBurgerMenu();
    void CreateGameManager(const long long& gameHash);
    void PerformSideBarAction(QAction* action);
    void SetupGameSidebar(BurgerMenu* pSideBar);
//    void AddBurgerMenu();
};

#endif // MAINWINDOW_H







