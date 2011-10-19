TEMPLATE = app
TARGET = Oscilloscope
QT += core gui opengl
CONFIG += debug
HEADERS += qgt/qgtaxis.h \
    qgt/qgtcanvas.h \
    qgt/qgtcoordinates.h \
    qgt/qgtcurve.h \
    qgt/qgtgraphicobject.h \
    qgt/qgthorizontalaxis.h \
    qgt/qgtlayout.h \
    qgt/qgtlayoutitem.h \
    qgt/qgtplot.h \
    qgt/qgtrendergraphicitem.h \
    qgt/qgtrenderobject.h \
    qgt/qgtrendertextitem.h \
    qgt/qgtverticalaxis.h \
    oscilloscope.h
SOURCES += qgt/qgtaxis.cpp \
    qgt/qgtcanvas.cpp \
    qgt/qgtcurve.cpp \
    qgt/qgtgraphicobject.cpp \
    qgt/qgthorizontalaxis.cpp \
    qgt/qgtlayout.cpp \
    qgt/qgtplot.cpp \
    qgt/qgtverticalaxis.cpp \
    main.cpp \
    oscilloscope.cpp
LIBS += -lqwt -L./lib -lfitpack    
FORMS += oscilloscope.ui
RESOURCES += 
QMAKE_CXXFLAGS += -std=c++0x
