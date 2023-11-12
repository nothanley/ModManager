#ifndef PIXMAPUTILS_HPP
#define PIXMAPUTILS_HPP

#include <QPixmap>
#include <QPainter>
#include "QtSvg/qsvgrenderer.h"

namespace PixMapUtils{

    static void
    drawSvgToPainter(QPainter* painter, const QString& svgPath, QPainter::CompositionMode mode, int multiply=1 ){
        painter->setOpacity(1);
        painter->setCompositionMode(mode);
        QSvgRenderer layoutGenShineSvg(svgPath);

        /* Optionally iterate draw for added weight */
        for (int i = 0; i < multiply; i++)
            layoutGenShineSvg.render(painter);
    }

    static void
    shaveAndStretchPixmap(QPixmap* pixmap, int left, int right, int top, int bottom) {
        QPixmap originalPixmap = *pixmap;
        int width = originalPixmap.width() - left - right;
        int height = originalPixmap.height() - top - bottom;

        if (width > 0 && height > 0) {
            QPixmap stretchedPixmap = originalPixmap.copy(left, top, width, height);
//            *pixmap = stretchedPixmap.scaled(originalPixmap.size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        }
    }

    static void
    toggleTransparentPainter(QPainter* painter){
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setRenderHint(QPainter::SmoothPixmapTransform);
        painter->setCompositionMode(QPainter::CompositionMode_Xor);
    }


    static QPixmap
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

        QPainter fill(&pixmap);
        fill.setCompositionMode(QPainter::CompositionMode_Xor);
        mask->render(&fill);
        fill.end();

        return pixmap;
    }

};

#endif // PIXMAPUTILS_HPP
