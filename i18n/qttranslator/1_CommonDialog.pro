
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 1_CommonDialog
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp

HEADERS  += widget.h

TRANSLATIONS = Translate_EN.ts \
Translate_CN.ts

DISTFILES += \
    icon.png

RESOURCES += \
    translator.qrc
