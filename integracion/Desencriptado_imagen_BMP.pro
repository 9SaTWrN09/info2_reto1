TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle

SOURCES += \
        funciones.cpp \
        generador_casos_de_prueba.cpp \
        imagenes_y_txt.cpp \
        main.cpp

HEADERS += \
    funciones.h \
    generador_casos_de_prueba.h \
    imagenes_y_txt.h


DISTFILES += \
    archivos/*.bmp \
    archivos/*.txt


