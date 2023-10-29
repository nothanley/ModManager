#include <QApplication>
#include "WindowHandler/qwinwidget.h"

int main(int argc, char *argv[])
{    
    //This has the app draw at HiDPI scaling on HiDPI displays, usually two pixels for every one logical pixel
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication app(argc, argv);

    int windowXPos = 450;
    int windowYPos = 125;
    int windowWidth = 1024;
    int windowHeight = 768;

    QWinWidget w;
    w.setGeometry(windowXPos, windowYPos, windowWidth, windowHeight);
    w.show();

    return app.exec();
}
