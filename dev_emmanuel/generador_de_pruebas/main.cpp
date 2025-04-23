#include <iostream>
#include <fstream>
#include "funciones.h"
#include "imagenes_y_txt.h"

using namespace std;

void generar_txt_enmascarado_lineal
    (unsigned char* imagen,unsigned char* mascara,int semilla,int largo_mascara,const char* nombre_txt);

void generar_prueba_completa
    (const char* ruta_I_O,const char* ruta_mascara,int semilla,int tipo_desplazamiento,int bits_desplazamiento);

int main()
{
    generar_prueba_completa(
        "archivos/I_O.bmp",    // Imagen original
        "archivos/M.bmp",      // Máscara
        1234,         // Semilla
        2,            // Tipo de desplazamiento (1 = SHIFT_R) (2 = SHIFT_L)
        8             // Bits de desplazamiento
        );

    return 0;
}

void generar_prueba_completa(
    const char* ruta_I_O,
    const char* ruta_mascara,
    int semilla,
    int tipo_desplazamiento,
    int bits_desplazamiento
    )
{
    // Cargar imágenes
    int width, height, mask_width, mask_height;
    unsigned char* IO = loadPixels(QString(ruta_I_O), width, height);
    if (IO != nullptr) cout << "I_O cargada correctamente" << endl;else cout << "Fallo en cargo de I_D" << endl;

    unsigned char* mascara = loadPixels(QString(ruta_mascara), mask_width, mask_height);
    if (mascara != nullptr) cout << "mascara cargada correctamente" << endl;else cout << "Fallo en cargo de mascara" << endl;

    // Aplicar desplazamiento a toda la imagen
    unsigned char* imagen_desplazada = new unsigned char[width * height * 3];
    switch (tipo_desplazamiento)
    {
        case 1: desplazar_derecha(IO, bits_desplazamiento, imagen_desplazada, width * height * 3); break;
        case 2: desplazar_izquierda(IO, bits_desplazamiento, imagen_desplazada, width * height * 3); break;
        default: cerr << "Tipo de desplazamiento inválido." << endl; return;
    }

    // Guardar imagen desplazada
    exportImage(imagen_desplazada, width, height, "archivos/pruebas/I_D.bmp");

    // Largo de la región enmascarada (en bytes)
    int largo_mascara = mask_width * mask_height * 3;

    // Validación de seguridad (opcional pero recomendable)
    if (semilla + largo_mascara > width * height * 3)
    {
        cerr << "Error: la semilla + tamaño de máscara excede el tamaño de la imagen." << endl;
        return;
    }

    // Generar el .txt con los valores imagen + máscara (formato real del desafío)

    generar_txt_enmascarado_lineal(IO, mascara, semilla, largo_mascara, "archivos/txt/M0.txt");

    generar_txt_enmascarado_lineal(imagen_desplazada, mascara, semilla, largo_mascara, "archivos/txt/M1.txt");

    // Liberar memoria
    delete[] IO;
    delete[] mascara;
    delete[] imagen_desplazada;
}

void generar_txt_enmascarado_lineal
    (
        unsigned char* imagen,
        unsigned char* mascara,
        int semilla,
        int largo_mascara,
        const char* nombre_txt
        )
{
    ofstream archivo(nombre_txt);
    if (!archivo.is_open())
    {
        cerr << "No se pudo crear " << nombre_txt << endl;
        return;
    }

    archivo << semilla << endl;

    for (int i = 0; i < largo_mascara; i += 3)
    {
        int r = imagen[semilla + i]     + mascara[i];
        int g = imagen[semilla + i + 1] + mascara[i + 1];
        int b = imagen[semilla + i + 2] + mascara[i + 2];

        archivo << (r % 256) << " " << (g % 256) << " " << (b % 256) << endl;
    }

    archivo.close();
}
