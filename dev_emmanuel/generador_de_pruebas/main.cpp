#include <iostream>
#include <fstream>
#include <filesystem>
#include "funciones.h"
#include "imagenes_y_txt.h"

using namespace std;
namespace fs = std::filesystem;

// Función para guardar la imagen transformada
void guardar_imagen_transformada(
    unsigned char* pixels,
    int width,
    int height,
    const string& ruta_salida
    ) {
    exportImage(pixels, width, height, QString::fromStdString(ruta_salida));
}

void generar_prueba_completa(
    const string& ruta_imagen,
    const string& ruta_mascara,
    const string& carpeta_salida,
    int semilla,
    int tipo_desplazamiento,
    int bits_desplazamiento
    ) {
    // Cargar imágenes
    int width, height, mask_width, mask_height;
    unsigned char* imagen = loadPixels(QString::fromStdString(ruta_imagen), width, height);
    unsigned char* mascara = loadPixels(QString::fromStdString(ruta_mascara), mask_width, mask_height);

    if (!imagen || !mascara) {
        cerr << "Error al cargar imágenes." << endl;
        return;
    }

    // Aplicar desplazamiento a toda la imagen
    unsigned char* imagen_desplazada = new unsigned char[width * height * 3];
    switch (tipo_desplazamiento) {
    case 1: desplazar_derecha(imagen, bits_desplazamiento, imagen_desplazada, width * height * 3); break;
    case 2: desplazar_izquierda(imagen, bits_desplazamiento, imagen_desplazada, width * height * 3); break;
    default: cerr << "Tipo de desplazamiento inválido." << endl; return;
    }

    // Guardar imagen desplazada
    guardar_imagen_transformada(imagen_desplazada, width, height, carpeta_salida + "/desplazada.bmp");

    // Calcular posición de la máscara basada en la semilla
    int x_start = semilla % (width - mask_width);
    int y_start = (semilla / (width - mask_width)) % (height - mask_height);

    // Aplicar XOR solo en la región de la máscara
    unsigned char* enmascarado = new unsigned char[width * height * 3];
    memcpy(enmascarado, imagen_desplazada, width * height * 3); // Copiar imagen desplazada

    for (int y = 0; y < mask_height; y++) {
        for (int x = 0; x < mask_width; x++) {
            int pos_imagen = ((y_start + y) * width + (x_start + x)) * 3;
            int pos_mascara = (y * mask_width + x) * 3;

            if (pos_imagen + 2 < width * height * 3) { // Evitar desbordamiento
                enmascarado[pos_imagen]     ^= mascara[pos_mascara];
                enmascarado[pos_imagen + 1] ^= mascara[pos_mascara + 1];
                enmascarado[pos_imagen + 2] ^= mascara[pos_mascara + 2];
            }
        }
    }

    // Guardar TXT con semilla y datos
    ofstream archivo_txt(carpeta_salida + "/datos.txt");
    if (!archivo_txt.is_open()) {
        cerr << "Error al crear TXT." << endl;
        return;
    }

    archivo_txt << semilla << endl;
    for (int y = 0; y < mask_height; y++) {
        for (int x = 0; x < mask_width; x++) {
            int pos = ((y_start + y) * width + (x_start + x)) * 3;
            archivo_txt << (int)enmascarado[pos] << " "
                        << (int)enmascarado[pos + 1] << " "
                        << (int)enmascarado[pos + 2] << endl;
        }
    }

    // Liberar memoria
    delete[] imagen;
    delete[] mascara;
    delete[] imagen_desplazada;
    delete[] enmascarado;
    archivo_txt.close();
}

int main() {
    generar_prueba_completa(
        "I_D.bmp",
        "M.bmp",
        "pruebas", // Carpeta de salida
        1234,              // Semilla (define posición de la máscara)
        1,                  // Tipo de desplazamiento (1 = SHIFT_R)
        3                   // Bits desplazados
        );

    cout << "Prueba generada exitosamente." << endl;
    return 0;
}
