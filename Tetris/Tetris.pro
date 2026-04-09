QT += core gui widgets

CONFIG += c++17

TARGET   = Tetris
TEMPLATE = app

SOURCES += \
    main.cpp        \
    mainwindow.cpp  \
    tetriswidget.cpp \
    tetromino.cpp

HEADERS += \
    mainwindow.h    \
    tetriswidget.h  \
    tetromino.h

# Отключить предупреждения об устаревших API (опционально)
# DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

qnx:             target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
