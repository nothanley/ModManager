#include "mainwindow.h"

#include "TitleBar/windowframe.h"
#include <QLayout>
#include <QWidget>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow WindowContent;
    WindowFrame* titleBar = new WindowFrame(nullptr,WindowContent.centralWidget());
    titleBar->show();


    return a.exec();
}
