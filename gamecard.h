#ifndef GAMECARD_H
#define GAMECARD_H
#include <QWidget>
#include "QtSvg/qsvgrenderer.h"
#include <QSvgRenderer>
#define DEBUG_MODE_ENABLED

class CGamePackage;
class QSvgRenderer;

namespace Ui {
class GameCard;
}

class GameCard : public QWidget
{
    Q_OBJECT

public:
    explicit GameCard(QWidget *parent = nullptr, CGamePackage* gameMod = nullptr);
    ~GameCard();

    void decorate();
    bool isEmptyCard();
    bool isTextEnabled(){ return this->m_bShowText; }
    void setTextVisible(bool toggle){ this->m_bShowText =toggle;}

protected:
    void paintEvent(QPaintEvent *event) override;
    bool event(QEvent * e) override;

private slots:
    void on_GameCardButton_clicked();

Q_SIGNALS:
    void TableUpdate(CGamePackage* gameMod);

private:
    constexpr static const QColor PALLETE_GRAD_X = QColor(90,90,235);
    constexpr static const QColor PALLETE_GRAD_Y = QColor(180,120,235);
    constexpr static const double OVERLAY_WEIGHT = .2;

    void hoverEnter(QHoverEvent * event);
    void hoverLeave(QHoverEvent * event);
    void hoverMove(QHoverEvent * event);

    bool m_bShowText = false;
    bool m_IsUserHovering = false;
    bool m_IsHoverDecorated = false;
    bool m_IsEmptyCard = true;

    QSvgRenderer m_SvgRenderer = QSvgRenderer(QString(":/icons/card_mask.svg"));
    QSvgRenderer m_OverlaySvg = QSvgRenderer(QString(":/icons/card_overlay_outline.svg"));
    QPixmap* pLabelGraphic = nullptr;

    QPixmap drawVacantGraphics();
    QPixmap drawCardGraphics();
    void drawBorderMask( QPixmap* background );
    void drawHoverOverlay( QPixmap* background, const qreal opacity=1.0,
                            const QColor& gradientX=PALLETE_GRAD_X, const QColor& gradientY=PALLETE_GRAD_Y );

    CGamePackage* pGameMod = nullptr;
    Ui::GameCard *ui;
};

#endif // GAMECARD_H





