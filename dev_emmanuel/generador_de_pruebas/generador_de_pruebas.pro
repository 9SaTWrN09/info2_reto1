QT = core

CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
          main.cpp \
          funciones.cpp \
          imagenes_y_txt.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    funciones.h \
    imagenes_y_txt.h



QT += gui widgets

DISTFILES += \
    archivos/I_D.bmp \
    archivos/I_M.bmp \
    archivos/M.bmp \
    archivos/txt/M0.txt \
    archivos/txt/M001.txt \
    archivos/txt/M001.txt \
    archivos/txt/M001.txt \
    archivos/txt/M1.txt \
    archivos/txt/M2.txt \
    archivos/txt/M3.txt \
    archivos/txt/M4.txt \
    archivos/txt/M5.txt \
    archivos/txt/M6.txt \
    archivos/pruebas \
    build/Desktop_Qt_6_8_3_MinGW_64_bit-Debug/.qmake.stash


INCLUDEPATH += $$PWD/archivos/pruebas
