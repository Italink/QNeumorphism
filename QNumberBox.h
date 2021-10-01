#ifndef QNUMBER_BOX_H
#define QNUMBER_BOX_H

#include <QWidget>

class QLineEdit;
class QNumberBoxSlider;

class QNumberBox : public QWidget
{
    Q_OBJECT
public:
    explicit QNumberBox(double def,int precision =3, QWidget *parent = nullptr);
    void setValue(double var);
    double getValue();
private:
    QLineEdit *numberEdit_;
    QNumberBoxSlider *slider_;
    int precision_;
Q_SIGNALS:
    void valueChanged(double);
};

class QNumberBoxSlider : public QWidget{
    Q_OBJECT
public:
    QNumberBoxSlider();
protected:
    virtual void enterEvent(QEvent *) override;
    virtual void leaveEvent(QEvent *) override;
    virtual void paintEvent(QPaintEvent *) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *e) override;
Q_SIGNALS:
    void moveSlider(qreal);
private:
    bool enter;
    QPoint lastPos;
};


#endif // QNumberBox_H
