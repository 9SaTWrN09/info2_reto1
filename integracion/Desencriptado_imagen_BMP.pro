TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle

SOURCES += \
        imagenes_y_txt.cpp \
        main.cpp \
        operaciones_bits.cpp \
        otros.cpp \
        strings_dinamicos.cpp

HEADERS += \
    imagenes_y_txt.h \
    operaciones_bits.h \
    otros.h \
    strings_dinamicos.h


DISTFILES += \
    archivos/*.bmp \
    archivos/*.txt


