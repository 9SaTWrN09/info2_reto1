#ifndef IMAGENES_Y_TXT_H
#define IMAGENES_Y_TXT_H

#include <QString>

// CARGA/EXPORTACIÓN DE IMÁGENES
unsigned char* loadPixels(QString input, int &width, int &height);
bool exportImage(unsigned char* pixelData, int width, int height, QString archivoSalida);

// MANEJO DE TXT
unsigned int* loadSeedMasking(const char* nombreArchivo, int &seed, int &n_pixels);

#endif
