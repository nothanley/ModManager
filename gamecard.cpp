#include "gamecard.h"
#include "ui_gamecard.h"
#include "PackageManager/Manager/GamePackage.h"
#include <QDebug>
#include <QBitmap>
#include <QImage>
#include <QGraphicsBlurEffect>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPicture>
#include <QVBoxLayout>
#include <QFile>
#include <QPaintEngine>
#include <QPainterPath>
#include <QHoverEvent>

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

void GameCard::hoverEnter(QHoverEvent * event) {
    this->m_IsUserHovering = true;
    this->m_IsHoverDecorated = false;
}

void GameCard::hoverLeave(QHoverEvent * event) {
    this->m_IsUserHovering = false;
    this->m_IsHoverDecorated = false;

    delete this->pLabelGraphic;
    this->pLabelGraphic = nullptr;
}

void GameCard::hoverMove(QHoverEvent * event) {
//    this->m_IsUserHovering = true;
//    qDebug() << Q_FUNC_INFO << this->objectName();
}

bool GameCard::event(QEvent * e)
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


QPixmap
GameCard::drawCardGraphics(){

    QPixmap PixmapToBeMasked(this->size());
    PixmapToBeMasked.fill(Qt::transparent);

    QPainter backgroundPainter(&PixmapToBeMasked);
    backgroundPainter.setRenderHint(QPainter::Antialiasing);
    backgroundPainter.setRenderHint(QPainter::SmoothPixmapTransform);

    // ** Draw Image
    QImage sa(":/icons/card_bg_dummy.png");
    backgroundPainter.drawImage(rect(), sa);
    backgroundPainter.end();
    return PixmapToBeMasked;
}

void
toggleTransparentPainter(QPainter* painter){
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->setCompositionMode(QPainter::CompositionMode_Xor);
}

void
GameCard::drawBorderMask( QPixmap* background ){
    QPainter maskPainter(background);
    toggleTransparentPainter(&maskPainter);

    m_SvgRenderer.render(&maskPainter);
    maskPainter.end();
}

QPixmap CreateGradientMap( const QSize& size, QColor gradient_x, QColor gradient_y=Qt::transparent,
                           QSvgRenderer* mask=nullptr ) {

    QPixmap pixmap(size);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    QLinearGradient gradient(0, 0, size.width(), 0);

    gradient_y = (gradient_y == Qt::transparent) ? gradient_x : gradient_y;
    gradient.setColorAt(0, gradient_x);
    gradient.setColorAt(1, gradient_y);

    painter.setBrush(gradient);
    painter.drawRect(0, 0, size.width(), size.height());
    painter.end();
    if (!mask){ return pixmap; }

    // Apply Mask
    QPainter fill(&pixmap);
    fill.setCompositionMode(QPainter::CompositionMode_Xor);
    mask->render(&fill);
    fill.end();

    return pixmap;
}

void
GameCard::drawHoverOverlay( QPixmap* background, const qreal opacity ){

    // Draw Gradient Borders
    QPixmap gradientBorder = CreateGradientMap( size(),
                                          QColor(90,90,235),
                                          QColor(180,120,235),
                                          &m_OverlaySvg  );
    // Draw Border as overlay
    drawBorderMask(&gradientBorder);
    QPainter maskPainter(background);
    toggleTransparentPainter(&maskPainter);
    maskPainter.setCompositionMode(QPainter::CompositionMode_Screen);

    // Apply painter
    maskPainter.setOpacity(opacity);
    maskPainter.drawPixmap(0,0,gradientBorder);
    maskPainter.end();
}

void
GameCard::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);

    // redraw only if we need to
    if ( !this->pLabelGraphic ){
        pLabelGraphic = new QPixmap();
        *pLabelGraphic = drawCardGraphics();
        drawBorderMask(this->pLabelGraphic);
    }

    // implement unique hover style
    if ( m_IsUserHovering && !m_IsHoverDecorated ){
        drawHoverOverlay(this->pLabelGraphic,.6);
        this->m_IsHoverDecorated = true;
    }

    // draw final graphic
    QPainter final_painter(this);
    final_painter.drawPixmap(0,0,*pLabelGraphic);
    final_painter.end();
 }


void
GameCard::decorate(){
    auto effect = new QGraphicsDropShadowEffect;
    effect->setBlurRadius(9);
    effect->setColor(QColor(0,0,0,60));
    effect->setOffset(2,2);
    this->setGraphicsEffect(effect);
    ui->GameCardButton->setText(this->pGameMod->getName().c_str());

}



void GameCard::on_GameCardButton_clicked()
{
    TableUpdate(this->pGameMod);
}

