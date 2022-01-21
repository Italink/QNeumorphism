#ifndef QNeumorphism_h__
#define QNeumorphism_h__

#include <QGraphicsEffect>

class QNeumorphism : public QGraphicsEffect
{
public:
    QNeumorphism(qreal blurRadius = 10, qreal distance = 3, qreal strength = 0.8, qreal angle = 45, bool inset = false);
    ~QNeumorphism();

    qreal strength() const;
    void setStrength(const qreal& strength);

    qreal blurRadius() const;
    void setBlurRadius(const qreal& blurRadius);

    qreal distance() const;
    void setDistance(const qreal& distance);

    qreal angle() const;
    void setAngle(const qreal& angle);

    bool inset() const;
    void setInset(bool inset);


protected:
    QRectF boundingRectFor(const QRectF& rect) const override;
    void draw(QPainter* painter) override;

private:
    qreal blurRadius_;
    qreal distance_;
    qreal strength_;
    qreal angle_;
    bool inset_;
};

#endif // QNeumorphism_h__
