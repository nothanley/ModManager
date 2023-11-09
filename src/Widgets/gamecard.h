#ifndef GAMECARD_H
#define GAMECARD_H
#include <QWidget>
#include "QtSvg/qsvgrenderer.h"
#include <QSvgRenderer>
#include <QPainter>

class CGamePackage;
class QSvgRenderer;
class AddItemDialog;
class GameManagerForm;

namespace Ui {
class GameCard;
}

class GameCard : public QWidget
{
    Q_OBJECT

public:
    explicit GameCard(QWidget*parent = nullptr,
                      CGamePackage* gameMod = nullptr,
                      GameManagerForm* parentForm=nullptr);
    ~GameCard();

    void decorate();
    bool isEmptyCard();
    bool isTextEnabled(){ return this->m_bShowText; }
    bool isUserHovering(){ return this->m_IsUserHovering; }
    void setTextVisible(bool toggle){ this->m_bShowText =toggle;}
    void setHoverDecoration();

protected:
    void paintEvent(QPaintEvent *event) override;
    bool event(QEvent * e) override;
    void createAddItemDialog();

private slots:
    void on_GameCardButton_clicked();
    void addDialogClosed();

Q_SIGNALS:
    void TableUpdate(CGamePackage* gameMod);

private:
    constexpr static QColor PALLETE_GRAD_X = QColor(90,90,235);
    constexpr static QColor PALLETE_GRAD_Y = QColor(180,120,235);
    constexpr static double OVERLAY_WEIGHT = .2;
    QSvgRenderer m_SvgBorderMsk = QSvgRenderer(QString(":/icons/card_mask.svg"));
    QSvgRenderer m_SvgOutlineMsk = QSvgRenderer(QString(":/icons/card_overlay_outline.svg"));

    /* Debug Constants */
    constexpr static bool DEBUG_CARDS = true;
    const QString DBG_TEMPLATE_DIR = "C:/Users/wauke/source/repos/ModManager/Widgets/BurgerMenu/icons/card_templates";

    void mousePress(QEvent* event);
    void hoverEnter(QHoverEvent * event);
    void hoverLeave(QHoverEvent * event);
    void hoverMove(QHoverEvent * event);

    bool m_bShowText = false;
    bool m_IsUserHovering = false;
    bool m_IsHoverDecorated = false;
    bool m_IsEmptyCard = true;

    QPixmap* pLabelGraphic = nullptr;
    QPixmap drawVacantGraphics();
    QPixmap drawCardGraphics();

    void drawStylizedTextToMap(QPainter *painter, const QString& text, const qreal opacity=1.0 );
    void drawImageToPainter(QPainter* painter, const QString& imagePath, QSize *mapSize=nullptr );
    void drawImageOverlayToPainter(QPainter* painter, const QString& overlayPath, const float &opacity, QSize *mapSize=nullptr );
    void drawBorderMask( QPixmap* background );
    void drawHoverOverlay( QPixmap* background, const qreal opacity=1.0,
                            const QColor& gradientX=PALLETE_GRAD_X, const QColor& gradientY=PALLETE_GRAD_Y );

    GameManagerForm* pParentForm = nullptr;
    CGamePackage* pGameMod = nullptr;
    AddItemDialog* pItemDialog = nullptr;
    Ui::GameCard *ui;
};

#endif // GAMECARD_H





