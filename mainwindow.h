#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

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

private:
    Ui::MainWindow *ui;

    void InitLayoutWidgets();
    void InitGUIButtons();
    void AddBurgerMenu();
    void CreateGameManager();
//    void AddBurgerMenu();
};

#endif // MAINWINDOW_H
