#ifndef OSCILLOSCOPE_H
#define OSCILLOSCOPE_H

#include <QtGui/QWidget>
#include "ui_oscilloscope.h"

class Oscilloscope : public QWidget
{
    Q_OBJECT
    struct Private; Private* d;
public:
    Oscilloscope(QWidget *parent = 0);
    ~Oscilloscope();
private:
    Ui::OscilloscopeClass ui;
    void timerEvent(QTimerEvent* e);
};

#endif // OSCILLOSCOPE_H
