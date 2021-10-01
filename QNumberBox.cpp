#include "QNumberBox.h"
#include <QDoubleValidator>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMouseEvent>
#include <QPainter>

QNumberBox::QNumberBox(double def,int precision,QWidget *parent)
    : QWidget(parent)
    , numberEdit_(new QLineEdit())
    , slider_(new QNumberBoxSlider)
    , precision_(precision)
{
    QHBoxLayout *h=new QHBoxLayout(this);
    h->addWidget(numberEdit_);
    h->addWidget(slider_);
    h->setMargin(0);
    numberEdit_->setValidator(new QDoubleValidator);
    numberEdit_->setAlignment(Qt::AlignCenter);
    numberEdit_->setFixedSize(50,20);
    numberEdit_->setFrame(false);
    connect(numberEdit_,&QLineEdit::textChanged,this,[this](){
        Q_EMIT valueChanged(getValue());
    });
    connect(slider_,&QNumberBoxSlider::moveSlider,this,[this](qreal offset){
        setValue(getValue()+offset*pow(10,-precision_));
    });
    setValue(def);
}

void QNumberBox::setValue(double var)
{
    numberEdit_->setText(QString::number(var,'f',precision_));
}

double QNumberBox::getValue()
{
    return numberEdit_->text().toDouble();
}

QNumberBoxSlider::QNumberBoxSlider(){
    setFixedSize(30,16);
    enter=false;
}

void QNumberBoxSlider::enterEvent(QEvent *){
    setCursor(Qt::SplitHCursor);
    enter=true;
    update();
}

void QNumberBoxSlider::leaveEvent(QEvent *){
    setCursor(Qt::ArrowCursor);
    enter=false;
    update();
}

void QNumberBoxSlider::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen;
    pen.setBrush(QColor(0,0,0));
    pen.setCapStyle(Qt::RoundCap);
    if(enter)
        pen.setWidth(3);
    else
        pen.setWidth(2);
    painter.setPen(pen);
    QVector<QPoint> points;
    points<<QPoint(13,2)<<QPoint(5,8)<<QPoint(5,8)<<QPoint(13,14)
          <<QPoint(17,2)<<QPoint(25,8)<<QPoint(25,8)<<QPoint(17,14);
    painter.drawLines(points);
//    painter.fillRect(rect(),Qt::red);
}

void QNumberBoxSlider::mousePressEvent(QMouseEvent *event){
    lastPos=event->pos();
    QWidget::mousePressEvent(event);
}

void QNumberBoxSlider::mouseMoveEvent(QMouseEvent *e){
    if(e->buttons()&Qt::LeftButton){
        QPoint offset=e->pos()-lastPos;
        Q_EMIT moveSlider((offset.x()-offset.y()));
    }
    lastPos=e->pos();
}
