#include "gamecard.h"
#include "ui_gamecard.h"
#include "PackageManager/Manager/GamePackage.h"
#include "qtgameutils.hpp"
#include "pixmaputils.hpp"

#include <QHoverEvent>
#include <QGraphicsDropShadowEffect>
#include <QDebug>

GameCard::GameCard(QWidget *parent, CGamePackage* gameMod) :
    QWidget(parent),
    ui(new Ui::GameCard)
{
    ui->setupUi(this);

    this->setMouseTracking(true);
    this->pGameMod = gameMod;
    this->setAttribute(Qt::WA_Hover, true);
}

GameCard::~GameCard()
{
    delete this->pLabelGraphic;
    delete ui;
}

bool
GameCard::isEmptyCard(){
    return (this->pGameMod == nullptr);
}

void
GameCard::hoverEnter(QHoverEvent * event) {
    this->m_IsUserHovering = true;
    this->m_IsHoverDecorated = false;
    setHoverDecoration();
}

void
GameCard::hoverLeave(QHoverEvent * event) {
    this->m_IsUserHovering = false;
    this->m_IsHoverDecorated = false;

    /* Force graphic redraw */
    delete this->pLabelGraphic;
    this->pLabelGraphic = nullptr;
}

void
GameCard::hoverMove(QHoverEvent * event) {/*todo*/}

bool
GameCard::event(QEvent * e)
{
    switch(e->type())
    {
        case QEvent::HoverEnter:
            hoverEnter(static_cast<QHoverEvent*>(e));
            return true;
            break;
        case QEvent::HoverLeave:
            hoverLeave(static_cast<QHoverEvent*>(e));
            return true;
            break;
        case QEvent::HoverMove:
            hoverMove(static_cast<QHoverEvent*>(e));
            return true;
            break;
        default:
            break;
    }
    return QWidget::event(e);
}

void
GameCard::decorate(){
    auto effect = new QGraphicsDropShadowEffect;
    effect->setBlurRadius(9);
    effect->setColor(QColor(0,0,0,60));
    effect->setOffset(2,2);
    this->setGraphicsEffect(effect);

    if ( this->isEmptyCard() || !this->isTextEnabled() ) return;
    ui->GameCardButton->setText(this->pGameMod->getName().c_str());
}

void
GameCard::drawImageToPainter( QPainter* painter, const QString& imagePath, QSize* mapSize ){
    QImage image = !DEBUG_CARDS ? QImage(imagePath)
                                        : QImage( QTGameUtils::getRandomFilePath( DBG_TEMPLATE_DIR ) );
    QSize targetSize = (mapSize != nullptr) ? *mapSize : this->size();
    painter->drawImage( QRect(QPoint(0,0), targetSize ), image.scaled(targetSize) );
}

void
GameCard::drawImageOverlayToPainter(QPainter* painter, const QString& overlayPath, const float& opacity, QSize *mapSize ){
    QImage overlayImg(overlayPath);
    QSize targetSize = (mapSize != nullptr) ? *mapSize : this->size();

    /* Draw using "SCREEN" blending */
    painter->setOpacity(opacity);
    painter->setCompositionMode(QPainter::CompositionMode_Screen);
    painter->drawImage( QRect(QPoint(0,0), targetSize ), overlayImg.scaled(targetSize) );

    /* Reset Opacity */
    painter->setOpacity(1.0);
}

void
GameCard::drawBorderMask( QPixmap* background ){
    QPainter maskPainter(background);
    PixMapUtils::toggleTransparentPainter(&maskPainter);
    m_SvgBorderMsk.render(&maskPainter);

    maskPainter.end();
    PixMapUtils::shaveAndStretchPixmap(background,1,1,1,1);
}

void
GameCard::drawHoverOverlay( QPixmap* background, const qreal opacity,
                            const QColor& gradientX, const QColor& gradientY ){
    QPainter maskPainter(background);
    PixMapUtils::toggleTransparentPainter(&maskPainter);

    // Draw Gradient Borders
    QPixmap gradientMap = PixMapUtils::CreateGradientMap( size(),
                                          gradientX,
                                          gradientY,
                                          &m_SvgOutlineMsk  );
    // Apply painter
    maskPainter.setCompositionMode(QPainter::CompositionMode_Screen);
    maskPainter.setOpacity(opacity);
    maskPainter.drawPixmap(0,0,gradientMap);

    maskPainter.end();
}

QPixmap
GameCard::drawVacantGraphics(){
    // Initialize pixmap and painter
    QPixmap pixmapGraphic(this->size());
    QPainter backgroundPainter(&pixmapGraphic);

    // Draw a gradient fill
    backgroundPainter.drawPixmap(0,0, PixMapUtils::CreateGradientMap( size(),
                                                        QColor(60,60,80),
                                                        QColor(70,70,80) ));
    // Draw "+" icon overlay
    PixMapUtils::drawSvgToPainter(&backgroundPainter, ":/icons/card_add_overlay.svg",
                     QPainter::CompositionMode_Screen,
                     /* Overlay Weight */ (this->m_IsUserHovering) ? 2 : 1 );

    // Draw highlight borders
    backgroundPainter.end();
    drawHoverOverlay(&pixmapGraphic,1,QColor(10,10,10), QColor(95,95,95));
    return pixmapGraphic;
}

void
GameCard::drawStylizedTextToMap(QPainter* painter, const QString &text, const qreal opacity ){
    QFont font("Segoe UI Variable Display", 7,11);
    font.setBold(true);
    painter->setFont(font);

    // Overlay white text on qpixmap aligned to the center
    painter->setCompositionMode(QPainter::CompositionMode_Screen);
    painter->setOpacity(opacity);
    painter->setPen(QColor(255,255,255,195));
    painter->drawText(QRect(0, 0, width(),height()), Qt::AlignCenter, text.toUpper());

    // reset opacity
    painter->setOpacity(1.0);
}

QPixmap
GameCard::drawCardGraphics(){
    // Initialize pixmap and painter
    QPixmap pixmap(this->size());
    QPainter backgroundPainter(&pixmap);

    // Draw card image and foreground effects
    if ( this->pGameMod->hasThumbnail() )
    {
        drawImageToPainter( &backgroundPainter,":/icons/card_bg_dummy.png" );
    }
    else{
        backgroundPainter.drawPixmap(0,0, PixMapUtils::CreateGradientMap( size(),
                                                            QColor(60,60,80),
                                                            QColor(70,70,80) ));
        drawStylizedTextToMap(&backgroundPainter, pGameMod->getName().c_str() );
    }


    drawImageOverlayToPainter(&backgroundPainter, ":/icons/card_sheen_overlay.png", OVERLAY_WEIGHT);
    backgroundPainter.end();

    // Draw Opaque borders around image
    drawHoverOverlay(&pixmap,.45,
                     QColor(10,10,10), QColor(180,180,200) );
    return pixmap;
}

void
GameCard::setHoverDecoration(){
    if ( isUserHovering() && isEmptyCard() )
        *pLabelGraphic = drawVacantGraphics();

    drawHoverOverlay(this->pLabelGraphic,1,
                     QColor(10,10,10), QColor(150,190,250) );

    drawBorderMask(this->pLabelGraphic);
    this->m_IsHoverDecorated = true;
}

void
GameCard::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);

    // Draw Base Graphic
    if ( !this->pLabelGraphic ){
        pLabelGraphic = new QPixmap();
        *pLabelGraphic = this->isEmptyCard() ? drawVacantGraphics() : drawCardGraphics();
        drawBorderMask(this->pLabelGraphic);
    }

    // Combine final graphic
    QPainter final_painter(this);
    final_painter.drawPixmap(0,0,*pLabelGraphic);
    final_painter.end();
 }



void GameCard::on_GameCardButton_clicked()
{
    if ( this->isEmptyCard() ) return;
    TableUpdate(this->pGameMod);
}

