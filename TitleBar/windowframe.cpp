#include "windowframe.h"
#include "ui_windowframe.h"

#include <QMouseEvent>
#include <windows.h>
#include <windowsx.h>
#include <QDebug>
#include "BurgerMenu/burgermenu.h"

/// @brief Конструктор класса WindowFrame.
/// @param parent Родительский виджет.
/// @param child Дочерний виджет, который будет добавлен в окно (опционально).
WindowFrame::WindowFrame(QWidget *parent, QWidget *child)
    : QFrame(parent), ui(new Ui::WindowFrame){

    ui->setupUi(this);

    AddBurgerMenu();
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);

    this->ui->CollectionDummy->hide();
    ui->WindowContent->layout()->replaceWidget( ui->CollectionDummy, child );

}


/// @brief Деструктор класса WindowFrame.
WindowFrame::~WindowFrame(){
    int subBodies=ui->body->layout()->count();
    if(subBodies>0) {
        for(int i=0; i<subBodies; i++) {
            QWidget *subBody=ui->body->layout()->itemAt(i)->widget();
            delete subBody;
        }
    }
    delete ui;
}


/// @brief Обработчик сигнала клика на кнопке "Закрыть".
void WindowFrame::on_close_clicked(){
    close();
}


/// @brief Обработчик сигнала клика на кнопке "Максимизировать/Восстановить".
void WindowFrame::on_maximum_clicked(){
    if(isMaximized()) {
        showNormal();
    } else {
        showMaximized();
    }
}


/// @brief Обработчик сигнала клика на кнопке "Свернуть".
void WindowFrame::on_minimum_clicked(){
    showMinimized();
}


/// @brief Обработчик события нажатия кнопки мыши.
/// @param event Указатель на объект QMouseEvent с информацией о событии.
void WindowFrame::mousePressEvent(QMouseEvent *event) {
    if (event->buttons() == Qt::LeftButton) {
        QWidget* widget = childAt(event->x(), event->y());
        if(widget==ui->header) {
            position.setX(event->x());
            position.setY(event->y());
        }
    }
}


/// @brief Обработчик события перемещения мыши в окне.
/// @param event Указатель на объект события перемещения мыши (QMouseEvent).
/// @return Нет возвращаемого значения.
void WindowFrame::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() == Qt::LeftButton) {
        if (position.x() != 0 || position.y() != 0) {
            move(event->globalX() - position.x(), event->globalY() - position.y());
        }
    }
}


/// @brief Обработчик события отпускания кнопки мыши в окне.
/// @param event Указатель на объект события отпускания кнопки мыши (QMouseEvent).
void WindowFrame::mouseReleaseEvent(QMouseEvent *event) {
    position.setX(0);
    position.setY(0);
}


/// @brief Обработчик события двойного щелчка мыши в окне.
/// @param event Указатель на объект события двойного щелчка мыши (QMouseEvent).
void WindowFrame::mouseDoubleClickEvent(QMouseEvent *event) {
    if (event->buttons() == Qt::LeftButton) {
        QWidget* widget = childAt(event->x(), event->y());
        if(widget == ui->header) {
            isMaximized() ? showNormal() : showMaximized();
        }
    }
}


/// @brief Обработчик нативного события окна.
/// @param eventType Тип события, в виде массива байт (QByteArray).
/// @param message Указатель на структуру с информацией о событии (void*).
/// @param result Указатель на переменную для возвращения результата (long*).
/// @return Возвращаемое значение, true если событие было обработано, иначе false.
bool WindowFrame::nativeEvent(const QByteArray &eventType, void *message, long *result) {
    Q_UNUSED(eventType)
    MSG *param = static_cast<MSG *>(message);
    switch (param->message){
        case WM_NCHITTEST:{
            int nX = GET_X_LPARAM(param->lParam) - geometry().x();
            int nY = GET_Y_LPARAM(param->lParam) - geometry().y();

            if (childAt(nX, nY) != ui->header && childAt(nX, nY) != ui->body){
                return QWidget::nativeEvent(eventType, message, result);
            }

            *result = HTCAPTION;

            if ((nX > 0) && (nX < borderSize)){
                *result = HTLEFT;
            }

            if ((nX > width() - borderSize) && (nX < width())){
                *result = HTRIGHT;
            }

            if ((nY > 0) && (nY < borderSize)){
                *result = HTTOP;
            }

            if ((nY > height() - borderSize) && (nY < height())){
                *result = HTBOTTOM;
            }

            if ((nX > 0) && (nX < borderSize) && (nY > 0) && (nY < borderSize)){
                *result = HTTOPLEFT;
            }

            if ((nX > width() - borderSize) && (nX < width()) && (nY > 0) && (nY < borderSize)){
                *result = HTTOPRIGHT;
            }

            if ((nX > 0) && (nX < borderSize) && (nY > height() - borderSize) && (nY < height())){
                *result = HTBOTTOMLEFT;
            }

            if ((nX > width() - borderSize) && (nX < width()) && (nY > height() - borderSize) && (nY < height())){
                *result = HTBOTTOMRIGHT;
            }

            return true;
        }
    }

    return QWidget::nativeEvent(eventType, message, result);
}


/// @brief Показать или скрыть кнопку минимизации окна.
/// @param enable Если true, то кнопка будет показана, если false - скрыта.
void WindowFrame::enableMinimum(bool enable) {
    !enable ? ui->minimum->hide() : ui->minimum->show();
}


/// @brief Показать или скрыть кнопку максимизации окна.
/// @param enable Если true, то кнопка будет показана, если false - скрыта.
void WindowFrame::enableMaximum(bool enable) {
    !enable ? ui->maximum->hide() : ui->maximum->show();
}


/// @brief Показать или скрыть кнопку закрытия окна.
/// @param enable Если true, то кнопка будет показана, если false - скрыта.
void WindowFrame::enableClose(bool enable) {
    !enable ? ui->close->hide() : ui->close->show();
}


/// @brief Переопределение функции фильтрации событий для класса WindowFrame.
/// @param obj Указатель на объект, для которого было сгенерировано событие.
/// @param event Указатель на объект класса QEvent, представляющий событие.
/// @return `true`, если событие было обработано, иначе `false`.
bool WindowFrame::eventFilter(QObject *obj, QEvent *event) {
    if(obj==mainBody) {
        if(event->type()==QEvent::HideToParent) {
            hide();
            return true;
        }
        if(event->type()==QEvent::ShowToParent) {
            show();
            return true;
        }
        return QObject::eventFilter(obj,event);
    } else {
        return QFrame::eventFilter(obj,event);
    }
    return false;
}

void WindowFrame::AddBurgerMenu(){
    BurgerMenu* menu     = new BurgerMenu();
    this->layout()->replaceWidget(ui->SideBarDummy,menu);

//    menu->setMenuWidth(100);
    menu->setBurgerIcon(QIcon(":/icons/BurgerMenu/icons/burger.png"));
    menu->addMenuAction(QIcon(":/icons/BurgerMenu/icons/collections.png"), "Collection");
    menu->addMenuAction(QIcon(":/icons/BurgerMenu/icons/folders.png"),     "Folders");
    menu->addMenuAction(QIcon(":/icons/BurgerMenu/icons/albums.png"),      "Albums");

    this->setStyleSheet("BurgerMenu                  { background-color: black;   }"
                        "#BurgerMenu                 { background-color: black;   }"
                        "#BurgerButton               { background-color: black;   color: white; font-size: 18px; }"
                        "#BurgerButton:hover         { background-color: #3480D2; }"
                        "#BurgerButton:checked       { background-color: #106EBE; }"
                        "#BurgerButton:checked:hover { background-color: #3480D2; }"
                        "#MainBurgerButton           { background-color: black;   border: none; } "
                        "#MainBurgerButton:hover     { background-color: #333;    } "
                        );

    QObject::connect(menu, &BurgerMenu::triggered, [&](QAction* action)
                     {
                         this->ui->CollectionDummy->setText(QString("Action \"%1\" clicked.").arg(action->iconText()));
                     });

    QObject::connect(menu, &BurgerMenu::expandedChanged, [&](bool expanded)
                     {
                         this->ui->CollectionDummy->setText(QString("Expanded changed: %1").arg(expanded ? "expanded" : "collapsed"));
                     });

}
