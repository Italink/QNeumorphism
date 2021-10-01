#include "QNumberSlider.h"
#include "QNumberBox.h"
#include <QDoubleValidator>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSlider>

QNumberSlider::QNumberSlider(double number, double min, double max, QString label, int precision)
    : min_(min)
    , max_(max)
    , precision_(precision)
    , numberEdit_(new QLineEdit)
    , slider_(new QSlider(Qt::Horizontal))
    , sliderBox_(new QNumberBoxSlider)
{
    QHBoxLayout *h=new QHBoxLayout(this);
    h->setMargin(0);
    if(!label.isEmpty()){
        h->addWidget(new QLabel(label));
    }
    h->addWidget(numberEdit_);
    h->addWidget(slider_);
    h->addWidget(sliderBox_);
    numberEdit_->setValidator(new QDoubleValidator);
    numberEdit_->setAlignment(Qt::AlignCenter);
    numberEdit_->setFixedSize(40,20);
    numberEdit_->setFrame(false);
    numberEdit_->setText(QString::number(number,'f',precision_));
    slider_->setMinimumWidth(50);
    slider_->setFixedHeight(15);
    slider_->setRange(min*pow(10,precision_),max*pow(10,precision_));

    connect(slider_,&QSlider::valueChanged,this,[this](int value){
        double var = value * pow(10, -precision_);
        Q_EMIT valueChanged(var);
        numberEdit_->setText(QString::number(var,'f',precision_));
    });

    connect(numberEdit_,&QLineEdit::editingFinished,this,[this](){
        if(!setValue(numberEdit_->text().toDouble())){
            numberEdit_->setText(QString::number(slider_->value()*pow(10,-precision_),'f',precision_));
        }
    });

    connect(sliderBox_,&QNumberBoxSlider::moveSlider,this,[this](qreal offset){
        setValue(getValue()+offset*pow(10,-precision_));
    });
    setValue(number);
}



bool QNumberSlider::setValue(double value)
{
    int tmp=value*pow(10,precision_);
    if(tmp>=slider_->minimum()&&tmp<=slider_->maximum()){
         slider_->setValue(tmp);
         return true;
    }
    return false;
}

double QNumberSlider::getValue()
{
    return slider_->value()*pow(10,-precision_);
}
