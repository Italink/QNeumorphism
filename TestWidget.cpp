#include "TestWidget.h"

#include <QVBoxLayout>

TestWidget::TestWidget(QWidget *parent)
    : QWidget(parent)
    , btTest(new QPushButton("Test"))
    , effect(new QNeumorphism)
{
    btTest->setGraphicsEffect(effect);
    QVBoxLayout *v = new QVBoxLayout(this);
    v->addWidget(btTest);

}
