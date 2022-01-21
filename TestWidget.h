#ifndef TESTWIDGET_H
#define TESTWIDGET_H

#include <QNeumorphism.h>
#include <QPushButton>
#include <QWidget>

class TestWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TestWidget(QWidget *parent = nullptr);
private:
    QPushButton *btTest;
    QNeumorphism *effect;
};

#endif // TESTWIDGET_H
