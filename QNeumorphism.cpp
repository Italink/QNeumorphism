#include "QNeumorphism.h"
#include <QPainter>

QNeumorphism::QNeumorphism(qreal blurRadius, qreal distance, qreal strength, qreal angle, bool inset)
    :blurRadius_(blurRadius)
    , distance_(distance)
    , strength_(strength)
    , angle_(angle)
    , inset_(inset)
{
}

QNeumorphism::~QNeumorphism()
{

}

qreal QNeumorphism::strength() const
{
    return strength_;
}

void QNeumorphism::setStrength(const qreal& strength)
{
    strength_ = strength;
    update();
}

qreal QNeumorphism::blurRadius() const
{
    return blurRadius_;
}

void QNeumorphism::setBlurRadius(const qreal& blurRadius)
{
    blurRadius_ = blurRadius;
    update();
}

qreal QNeumorphism::distance() const
{
    return distance_;
}

void QNeumorphism::setDistance(const qreal& distance)
{
    distance_ = distance;
    update();
}

qreal QNeumorphism::angle() const
{
    return angle_;
}

void QNeumorphism::setAngle(const qreal& angle)
{
    angle_ = angle;
    update();
}

QRectF QNeumorphism::boundingRectFor(const QRectF& rect) const
{
    if (inset_)
        return rect.united(rect.translated(0, 0));
    return rect.united(rect.translated(0, 0).adjusted(-blurRadius_ - distance_, -blurRadius_ - distance_, blurRadius_ + distance_, blurRadius_ + distance_));
}

void QNeumorphism::draw(QPainter* painter)
{
    PixmapPadMode mode = PadToEffectiveBoundingRect;
    QPoint pos;
    const QPixmap px = sourcePixmap(Qt::DeviceCoordinates, &pos, mode);
    if (px.isNull())
        return;
    QTransform restoreTransform = painter->worldTransform();
    painter->setWorldTransform(QTransform());
    if (px.isNull())
        return;

    QImage shadow1(px.size(), QImage::Format_ARGB32_Premultiplied);
    shadow1.setDevicePixelRatio(px.devicePixelRatioF());
    shadow1.fill(0);

    QImage shadow2(px.size(), QImage::Format_ARGB32_Premultiplied);
    shadow2.setDevicePixelRatio(px.devicePixelRatioF());
    shadow2.fill(0);

    QPoint offset;
    qreal radian = angle_ / 180 * M_PI;
    offset.setX(distance_ * qCos(radian));
    offset.setY(distance_ * qSin(radian));
    QT_BEGIN_NAMESPACE
        extern Q_WIDGETS_EXPORT void qt_blurImage(QPainter * p, QImage & blurImage, qreal radius, bool quality, bool alphaOnly, int transposed = 0);
    QT_END_NAMESPACE

        if (inset_) {
            QPainter shadow1Painter(&shadow1);
            shadow1Painter.drawPixmap(0, 0, px);
            shadow1Painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
            shadow1Painter.fillRect(shadow1.rect(), QColor::fromRgbF(0.0, 0.0, 0.0, strength_));
            shadow1Painter.end();

            QPainter shadow2Painter(&shadow2);
            shadow2Painter.drawPixmap(0, 0, px);
            shadow2Painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
            shadow2Painter.fillRect(shadow1.rect(), QColor::fromRgbF(1.0, 1.0, 1.0, strength_));
            shadow2Painter.end();

            QPainter shadowPainter(&shadow1);
            shadowPainter.setCompositionMode(QPainter::CompositionMode_SourceAtop);
            shadowPainter.drawImage(offset, shadow2);
            shadowPainter.setCompositionMode(QPainter::CompositionMode_Clear);
            offset.setX(qAbs(offset.x()));
            offset.setY(qAbs(offset.y()));

            QRect rect;
            rect.setWidth(shadow1.width() / px.devicePixelRatioF() / px.devicePixelRatioF());
            rect.setHeight(shadow1.height() / px.devicePixelRatioF() / px.devicePixelRatioF());
            rect.adjust(offset.x(), offset.y(), -offset.x(), -offset.y());
            shadowPainter.fillRect(rect, Qt::transparent);
            shadowPainter.end();

            QImage blurred(px.size(), QImage::Format_ARGB32);
            blurred.setDevicePixelRatio(px.devicePixelRatioF());
            blurred.fill(0);
            QPainter blurPainter(&blurred);
            qt_blurImage(&blurPainter, shadow1, blurRadius_, false, false);
            blurPainter.end();

            QImage image = px.toImage();
            QPainter lastPainter(&image);
            lastPainter.setCompositionMode(QPainter::CompositionMode_SourceOver);
            lastPainter.drawImage(blurred.rect(), blurred);
            lastPainter.end();

            painter->drawImage(pos, image);
        }
        else {
            QPainter shadowPainter(&shadow1);
            shadowPainter.setCompositionMode(QPainter::CompositionMode_Source);
            shadowPainter.drawPixmap(0, 0, px);
            shadowPainter.end();

            QImage blurred(shadow1.size(), QImage::Format_ARGB32_Premultiplied);
            blurred.setDevicePixelRatio(px.devicePixelRatioF());
            blurred.fill(0);

            QPainter blurPainter(&blurred);
            qt_blurImage(&blurPainter, shadow1, blurRadius_, false, true);
            blurPainter.end();

            shadow1 = std::move(blurred);

            // blacken the image...
            shadowPainter.begin(&shadow1);
            shadowPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
            shadowPainter.fillRect(shadow1.rect(), QColor::fromRgbF(0.0, 0.0, 0.0, strength_));
            shadowPainter.end();
            shadow2 = shadow1;

            // blacken the image...
            shadowPainter.begin(&shadow2);
            shadowPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
            shadowPainter.fillRect(shadow2.rect(), QColor::fromRgbF(1.0, 1.0, 1.0, strength_));
            shadowPainter.end();

            // draw the blurred drop shadow...
            painter->drawImage(pos + offset, shadow1);
            painter->drawImage(pos - offset, shadow2);

            // Draw the actual pixmap...
            painter->drawPixmap(pos, px);
        }
    painter->setWorldTransform(restoreTransform);
}

bool QNeumorphism::inset() const
{
    return inset_;
}

void QNeumorphism::setInset(bool inset)
{
    inset_ = inset;
    update();
}

