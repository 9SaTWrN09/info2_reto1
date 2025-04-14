#ifndef IMAGENES_Y_TXT_H
#define IMAGENES_Y_TXT_H

#include <QString>

// Función para cargar imágenes BMP (formato RGB888)
unsigned char* loadPixels(QString input, int &width, int &height);

// Función para exportar una imagen a un archivo BMP
bool exportImage(unsigned char* pixelData, int width, int height, QString archivoSalida);

// Función para cargar un archivo .txt con semilla y tripletas RGB
unsigned int* loadSeedMasking(const char* nombreArchivo, int &seed, int &n_pixels);

#endif // IMAGENES_Y_TXT_H
