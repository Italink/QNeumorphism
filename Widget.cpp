#include "QNeumorphism.h"
#include "QNumberSlider.h"
#include "Widget.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QColorDialog>
#include <QCheckBox>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setAutoFillBackground(true);
    setMinimumSize(300,350);

    QPushButton *btTest=new QPushButton();
    btTest->setAutoFillBackground(true);
    btTest->setFlat(true);

    QColor color(0,100,200);
    QPalette palette;
    palette.setColor(QPalette::Button,color);
    btTest->setPalette(palette);
    palette.setColor(QPalette::Window,color);
    this->setPalette(palette);

    QWidget *widget=new QWidget;
    QNeumorphism* neum=new QNeumorphism();
    widget->setGraphicsEffect(neum);

    btTest->setGraphicsEffect(neum);
    btTest->setFixedSize(100,100);

    QNumberSlider *blurRadius=new QNumberSlider(neum->blurRadius(),1,50,"blur radius");
    QNumberSlider *distance  =new QNumberSlider(neum->distance(),0,20,"distance");
    QNumberSlider *strength  =new QNumberSlider(neum->strength(),0,1,"strength",2);
    QNumberSlider *angle     =new QNumberSlider(neum->angle(),0,360,"angle",1);

    QVBoxLayout *layout=new QVBoxLayout(this);
    layout->addWidget(btTest,0,Qt::AlignCenter);

    layout->addSpacing(20);

    QCheckBox* inset = new QCheckBox();
    inset->setChecked(neum->inset());
    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->addWidget(new QLabel("inset"));
    hLayout->addWidget(inset);
    layout->addLayout(hLayout);

    layout->addWidget(blurRadius);
    layout->addWidget(distance);
    layout->addWidget(strength);
    layout->addWidget(angle);

    connect(btTest,&QPushButton::clicked,this,[btTest,this](){
        QColorDialog colorDialog;
        colorDialog.setGeometry(200,200,300,280);
        colorDialog.setCurrentColor(btTest->palette().color(QPalette::Button));
        connect(&colorDialog,&QColorDialog::currentColorChanged,[btTest,this](const QColor &color){
            QPalette palette;
            palette.setColor(QPalette::Button,color);
            btTest->setPalette(palette);
            palette.setColor(QPalette::Window,color);
            this->setPalette(palette);
        });
        colorDialog.exec();
    });

    connect(inset,&QCheckBox::stateChanged,this,[neum](int state){
       neum->setInset(state==Qt::Checked);
    });

    connect(blurRadius,&QNumberSlider::valueChanged,this,[neum](double var){
        neum->setBlurRadius(var);
    });

    connect(distance,&QNumberSlider::valueChanged,this,[neum](double var){
        neum->setDistance(var);
    });

    connect(strength,&QNumberSlider::valueChanged,this,[neum](double var){
        neum->setStrength(var);
    });

    connect(angle,&QNumberSlider::valueChanged,this,[neum](double var){
        neum->setAngle(var);
    });
}

Widget::~Widget()
{
}

