#ifndef GAMECARD_H
#define GAMECARD_H
#include <QWidget>
#include "QtSvg/qsvgrenderer.h"
#include <QSvgRenderer>

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

protected:
    void paintEvent(QPaintEvent *event) override;
    bool event(QEvent * e) override;

private slots:
    void on_GameCardButton_clicked();

Q_SIGNALS:
    void TableUpdate(CGamePackage* gameMod);

private:
    void hoverEnter(QHoverEvent * event);
    void hoverLeave(QHoverEvent * event);
    void hoverMove(QHoverEvent * event);

    bool m_IsUserHovering = false;
    bool m_IsHoverDecorated = false;

    QSvgRenderer m_SvgRenderer = QSvgRenderer(QString(":/icons/card_mask_0_io.svg"));
    QSvgRenderer m_OverlaySvg = QSvgRenderer(QString(":/icons/card_overlay_outline.svg"));
    QPixmap* pLabelGraphic = nullptr;

    QPixmap drawCardGraphics();
    void drawBorderMask( QPixmap* background );
    void drawHoverOverlay(QPixmap* background , const qreal opacity=1.0);

    CGamePackage* pGameMod = nullptr;
    Ui::GameCard *ui;
};

#endif // GAMECARD_H





