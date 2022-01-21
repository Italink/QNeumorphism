#ifndef QNUMBER_SLIDER_H
#define QNUMBER_SLIDER_H

#include <QWidget>
class QLineEdit;
class QSlider;
class QNumberBoxSlider;

class QNumberSlider : public QWidget
{
    Q_OBJECT
public:
    explicit QNumberSlider(double number,double min,double max, QString label="", int precision = 0);
    bool setValue(double value);
    double getValue();
private:
    double min_,max_;
    int precision_;
    QLineEdit *numberEdit_;
    QSlider *slider_;
    QNumberBoxSlider *sliderBox_;
Q_SIGNALS:
    void valueChanged(double);
};

#endif // QNumberSlider_H
