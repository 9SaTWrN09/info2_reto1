#include <iostream>
#include <fstream>
#include "funciones.cpp"
#include "imagenes_y_txt.cpp"
#include <QImage>  // Incluir QImage para manejo de imágenes

using namespace std;

void generar_txt_con_desplazamiento(
    const string& ruta_imagen,
    const string& ruta_mascara,
    const string& ruta_salida,
    int semilla,
    int t_desplazamiento,
    int bits
    ) {
    // --- Cargar imágenes ---
    int width, height;
    unsigned char* imagen = loadPixels(ruta_imagen.c_str(), width, height);
    unsigned char* mascara = loadPixels(ruta_mascara.c_str(), width, height);

    if (!imagen || !mascara) {
        cerr << "Error al cargar archivos BMP." << endl;
        return;
    }

    // --- Aplicar desplazamiento ---
    unsigned char* imagen_desplazada = new unsigned char[width * height * 3];
    if (t_desplazamiento == 1) { // SHIFT_R (desplazamiento derecha)
        desplazar_derecha(imagen, bits, imagen_desplazada, width * height * 3);
    } else { // SHIFT_L (desplazamiento izquierda)
        desplazar_izquierda(imagen, bits, imagen_desplazada, width * height * 3);
    }

    // --- Enmascarar con XOR ---
    unsigned char* enmascarado = new unsigned char[width * height * 3];
    aplicar_xor(imagen_desplazada, mascara, enmascarado, width * height * 3);

    // --- Generar archivo TXT ---
    ofstream archivo_txt(ruta_salida);
    if (!archivo_txt.is_open()) {
        cerr << "Error al crear: " << ruta_salida << endl;
        return;
    }

    // Escribir semilla
    archivo_txt << semilla << endl;

    // Escribir datos enmascarados (primeros 100 píxeles)
    int inicio = semilla % (width * height * 3);
    for (int i = 0; i < 100; i++) {
        int pos = (inicio + i) % (width * height * 3);
        archivo_txt << (int)enmascarado[pos] << " ";
        if ((i + 1) % 3 == 0) archivo_txt << endl; // Formato RGB
    }

    // --- Liberar memoria ---
    delete[] imagen;
    delete[] mascara;
    delete[] imagen_desplazada;
    delete[] enmascarado;
    archivo_txt.close();
}

int main() {
    // Ejemplo: Generar TXT con desplazamiento derecha de 3 bits
    generar_txt_con_desplazamiento(
        "archivos/I_D.bmp",         // Ruta de la imagen original
        "archivos/M.bmp",           // Ruta de la máscara
        "archivos/txt/M001.txt",    // Ruta de salida
        1234,                       // Semilla
        1,                          // 1=SHIFT_R, 2=SHIFT_L
        3                           // Bits desplazados
        );

    cout << "TXT generado exitosamente." << endl;
    return 0;
}
