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


#include <QDir>
#include <QFileInfo>
#include <QRandomGenerator>

QT_BEGIN_NAMESPACE
  extern Q_WIDGETS_EXPORT void qt_blurImage( QPainter *p, QImage &blurImage, qreal radius, bool quality, bool alphaOnly, int transposed = 0 );
QT_END_NAMESPACE

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
}

void
GameCard::hoverLeave(QHoverEvent * event) {
    this->m_IsUserHovering = false;
    this->m_IsHoverDecorated = false;

    delete this->pLabelGraphic;
    this->pLabelGraphic = nullptr;
}

void
GameCard::hoverMove(QHoverEvent * event) {
//    this->m_IsUserHovering = true;
//    qDebug() << Q_FUNC_INFO << this->objectName();
}

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
toggleTransparentPainter(QPainter* painter){
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->setCompositionMode(QPainter::CompositionMode_Xor);
}

QPixmap
CreateGradientMap( const QSize& size, QColor gradient_x, QColor gradient_y=Qt::transparent,
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

QPixmap
GameCard::drawVacantGraphics(){
    QPixmap PixmapToBeMasked(this->size());
    PixmapToBeMasked.fill(Qt::black);

    QPainter backgroundPainter(&PixmapToBeMasked);
    backgroundPainter.setRenderHint(QPainter::Antialiasing);
    backgroundPainter.setRenderHint(QPainter::SmoothPixmapTransform);

    // Draw Gradient
    QPixmap gradient = CreateGradientMap( size(),
                                          QColor(60,60,80),
                                          QColor(70,70,80) );
    backgroundPainter.drawPixmap(0,0,gradient);

    // ** Draw add symbol overlay
    backgroundPainter.setOpacity(1);
    backgroundPainter.setCompositionMode(QPainter::CompositionMode_Screen);
    QSvgRenderer layoutGenShineSvg(QString(":/icons/card_add_overlay.svg"));
    layoutGenShineSvg.render(&backgroundPainter);

    if (this->m_IsUserHovering){
        // ** Draw hover effects
        layoutGenShineSvg.render(&backgroundPainter);
        backgroundPainter.end();
        drawBorderMask(&PixmapToBeMasked);
    }
    else{
        backgroundPainter.end();
    }

    // Draw a border gradient
    drawHoverOverlay(&PixmapToBeMasked,1,QColor(10,10,10), QColor(95,95,95));
    return PixmapToBeMasked;
}

QString getRandomFilePath(const QString &directoryPath) {
    QDir directory(directoryPath);
    QStringList files = directory.entryList(QDir::Files);

    if (files.isEmpty()) {
        return QString(); // Return an empty QString if the directory is empty
    }

    int randomIndex = QRandomGenerator::global()->bounded(files.size());
    QString randomFile = files[randomIndex];

    return directoryPath + "/" + randomFile;
}


QPixmap
GameCard::drawCardGraphics(){

    QPixmap PixmapToBeMasked(this->size());
    PixmapToBeMasked.fill(Qt::black);

    QPainter backgroundPainter(&PixmapToBeMasked);
    backgroundPainter.setRenderHint(QPainter::Antialiasing);
    backgroundPainter.setRenderHint(QPainter::SmoothPixmapTransform);

    // ** Draw Image
    QImage layoutGenBGImg(":/icons/card_bg_dummy.png");
#if defined(DEBUG_MODE_ENABLED)
    layoutGenBGImg = QImage( getRandomFilePath(
                QString("C:/Users/wauke/source/repos/ModManager/Widgets/BurgerMenu/icons/card_templates" ) ) );
#endif
//    layoutGenBGImg = layoutGenBGImg.scaled( this->size());
    backgroundPainter.drawImage(rect(), layoutGenBGImg.scaled( this->size())  );

//    // ** Blur Test Overlay
//    if ( this->m_IsUserHovering && !this->isEmptyCard() ){
//        qt_blurImage( &backgroundPainter,layoutGenBGImg , 6, true, false );
//        backgroundPainter.setFont( QFont("Segoe UI Variable Small",5));
//        backgroundPainter.setPen(QPen(Qt::white));
//        backgroundPainter.drawText(layoutGenBGImg.rect(), Qt::AlignCenter, this->pGameMod->getName().c_str());
//    }
//    else{
//        backgroundPainter.drawImage(rect(), layoutGenBGImg );
//    }

    // ** Add Card Effects
    backgroundPainter.setOpacity(OVERLAY_WEIGHT);
    backgroundPainter.setCompositionMode(QPainter::CompositionMode_Screen);
    QImage layoutGenShineImg(":/icons/card_sheen_overlay.png");
    backgroundPainter.drawImage(rect(), layoutGenShineImg.scaled( this->size()) );

    // ** Finalize Painter
    backgroundPainter.end();
    return PixmapToBeMasked;
}

void
shaveAndStretchPixmap(QPixmap* pixmap, int left, int right, int top, int bottom) {
    QPixmap originalPixmap = *pixmap;
    int width = originalPixmap.width() - left - right;
    int height = originalPixmap.height() - top - bottom;

    if (width > 0 && height > 0) {
        QPixmap stretchedPixmap = originalPixmap.copy(left, top, width, height);
        *pixmap = stretchedPixmap.scaled(originalPixmap.size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation); }
}

void
GameCard::drawBorderMask( QPixmap* background ){
    QPainter maskPainter(background);
    toggleTransparentPainter(&maskPainter);
    maskPainter.setCompositionMode(QPainter::CompositionMode_Xor);

    m_SvgRenderer.render(&maskPainter);
    maskPainter.end();
    shaveAndStretchPixmap(background,1,1,1,1);
}


void
GameCard::drawHoverOverlay( QPixmap* background, const qreal opacity,
                            const QColor& gradientX, const QColor& gradientY ){

    // Draw Gradient Borders
    QPixmap gradientMap = CreateGradientMap( size(),
                                          gradientX,
                                          gradientY,
                                          &m_OverlaySvg  );
    // Draw Border as overlay
    drawBorderMask(&gradientMap);
    QPainter maskPainter(background);
    toggleTransparentPainter(&maskPainter);

    // Apply painter
    maskPainter.setCompositionMode(QPainter::CompositionMode_Screen);
    maskPainter.setOpacity(opacity);
    maskPainter.drawPixmap(0,0,gradientMap);

    maskPainter.end();
}


void
GameCard::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);

    // redraw only if we need to
    if ( !this->pLabelGraphic ){
        pLabelGraphic = new QPixmap();
        *pLabelGraphic = this->isEmptyCard() ? drawVacantGraphics() : drawCardGraphics();
        drawBorderMask(this->pLabelGraphic);
    }

    // implement unique hover style
    if ( m_IsUserHovering && !m_IsHoverDecorated ){
        if (this->isEmptyCard())
            *pLabelGraphic = drawVacantGraphics();
        drawHoverOverlay(this->pLabelGraphic,1,
                         QColor(10,10,10), QColor(150,190,250) );
        drawHoverOverlay(this->pLabelGraphic,1,
                         QColor(10,10,10), QColor(150,190,250)  );
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

    if ( this->isEmptyCard() || !this->isTextEnabled() ) return;
    ui->GameCardButton->setText(this->pGameMod->getName().c_str());
}



void GameCard::on_GameCardButton_clicked()
{
    if ( this->isEmptyCard() ) return;
    TableUpdate(this->pGameMod);
}

