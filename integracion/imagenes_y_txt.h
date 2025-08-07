#ifndef IMAGENES_Y_TXT_H
#define IMAGENES_Y_TXT_H

#include <QString>

// Función para cargar imágenes BMP (formato RGB888)
unsigned char* loadPixels(QString input, int &width, int &height);

// Función para exportar una imagen a un archivo BMP
bool exportImage(unsigned char* pixelData, int width, int height, QString archivoSalida);

// Función para cargar un archivo .txt con semilla y tripletas RGB
unsigned int* loadSeedMasking(const char* nombreArchivo, int &seed, int &n_pixels);

// LECTURA Y PROCESAMIENTO DE TXT
unsigned int** cargar_txts(unsigned char* M, int& n_txt, int& largo_mascara);

#endif // IMAGENES_Y_TXT_H


/*
 * Desarrollado por:
 *      Juan Pauolo Moncada(Juanpa) - [@JuanPa](https://github.com/Paulo848) correo: moncadamoralesjuanpaulo@gmail.com
 *
 *      Emmanuel López Ramírez(Entropy) - [@EmmaNuel-Network](https://github.com/EmmaNuel-Network) correo: entropy.casp9@gmail.com
*/
